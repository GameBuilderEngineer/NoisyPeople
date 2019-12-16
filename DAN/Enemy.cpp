//-----------------------------------------------------------------------------
// �G�l�~�[���N���X [Enemy.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "Enemy.h"
#include "EnemyManager.h"
#include "ImguiManager.h"
#include "Sound.h"
#include "ItemManager.h"
#include <cassert>
using namespace enemyNS;
using namespace stateMachineNS;

// �ÓI�����o�ϐ�
int Enemy::numOfEnemy = 0;			// �G�l�~�[�̑���
#ifdef _DEBUG
int Enemy::debugEnemyID = -1;		// �f�o�b�O����G�l�~�[��ID
#endif//_DEBUG

#pragma region [Basic Process]
//=============================================================================
// �R���X�g���N�^
//=============================================================================
Enemy::Enemy(ConstructionPackage constructionPackage)
{
	numOfEnemy++;	// �G�l�~�[�̐������Z

	enemyData = constructionPackage.enemyData;
	staticMesh = constructionPackage.staticMesh;
	gameMaster = constructionPackage.gameMaster;
	player = constructionPackage.player;
	attractorMesh = constructionPackage.attractorMesh;
	attractorMatrix = constructionPackage.attractorMatrix;
	markRenderer = EnemyManager::markRenderer;// ���O���[�o���łƂ��悤�ɂ������炱�̕K�v���Ȃ�

	// �G�l�~�[�f�[�^���I�u�W�F�N�g�ɃZ�b�g
	position = enemyData->position;
	axisZ.direction = enemyData->defaultDirection;

	// �X�e�[�g�}�V���̏�����
	stateMachine.initialize(enemyData->state);

	// �u���b�N�{�[�h�̏�����
	ZeroMemory(isNoticingPlayer, sizeof(bool) * gameMasterNS::PLAYER_NUM);
	ZeroMemory(noticedTimePlayer, sizeof(float) * gameMasterNS::PLAYER_NUM);
	movingTarget = NULL;
	attackTarget = NULL;
	attentionDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	isPayingNewAttention = false;
	nextIndexOfRoute = 0;

	// �Z���T�[�̏�����
	canSense = false;
	cntSensorInterval = 0.0f;
	ZeroMemory(isSensingPlayer, sizeof(bool) * gameMasterNS::PLAYER_NUM);
	ZeroMemory(distanceToPlayer, sizeof(float) * gameMasterNS::PLAYER_NUM);

	// �o�H�T���̏�����
	canSearch = false;
	cntSensorInterval = 0.0f;
	naviMesh = NavigationMesh::getNaviMesh();
	edgeList = NULL;
	naviFaceIndex = -1;

	// �U���̏�����
	canAttack = false;
	cntAttackInterval = 0.0f;
	isAttacking = false;
	attackTime = 0.0f;
	attackDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ړ��̏�����
	destination = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moveDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQuaternionIdentity(&moveMotion);
	isArraved = false;
	movingTime = 0.0f;
	isDestinationLost = false;
	onJump = false;
	jumping = false;

	// �R���W�����ƕ��������̏�����
	sphereCollider.initialize(&position, staticMesh->mesh);
	difference = DIFFERENCE_FIELD;
	onGround = false;
	onGroundBefore = false;
	groundNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	isHitPlayer = false;
	friction = 1.0f;

	// �I�u�W�F�N�g������(initialize()�͔h���N���X)
	onGravity = true;
	radius = sphereCollider.getRadius();
	{// �I�u�W�F�N�g�^�C�v�ƏՓˑΏۂ̎w��
		using namespace ObjectType;
		treeCell.type = ENEMY;
		treeCell.target = PLAYER | ENEMY | BULLET | TREE;
	}

	// �����X�e�[�g���Ƃ̃Z�b�g�A�b�v
	switch (enemyData->state)
	{
	case CHASE:
		isNoticingPlayer[gameMasterNS::PLAYER_1P] = true;
		isNoticingPlayer[gameMasterNS::PLAYER_2P] = true;
		break;
	case PATROL:
		isDestinationLost = true;
		break;
	case REST:
		break;
	case ATTACK_TREE:
		break;
	case DIE:
		break;
	}

#ifdef _DEBUG
#ifdef RENDER_SENSOR
	hearingSphere[0].initialize(&center, NOTICEABLE_DISTANCE_PLAYER[enemyData->type]);
	hearingSphere[1].initialize(&center, NOTICEABLE_DISTANCE_PLAYER[enemyData->type] * SHOT_SOUND_SCALE);
#endif
#endif
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Enemy::~Enemy()
{
	if (edgeList != NULL)
	{
		edgeList->terminate();
		SAFE_DELETE(edgeList);
	}

	// �ǐՃ}�[�N�̍폜
	deleteMark();

	numOfEnemy--;
}


//=============================================================================
// ���O����
//=============================================================================
void Enemy::preprocess(float frameTime)
{
	// �Z���T�[�̃v���C���[���m�t���O��������
	isSensingPlayer[gameMasterNS::PLAYER_1P] = false;
	isSensingPlayer[gameMasterNS::PLAYER_2P] = false;

	// �Z���T�[���Ď��s�\�ɂ���܂ł̊Ԋu���v�Z
	cntSensorInterval += frameTime;
	if (cntSensorInterval > SENSOR_UPDATE_INTERVAL[enemyData->type])
	{
		cntSensorInterval = 0.0f;
		canSense = true;
	}

	// �o�H�T�����Ď��s�\�ɂ���܂ł̊Ԋu���v�Z
	cntPathSearchInterval += frameTime;
	if (cntPathSearchInterval >= PATH_SEARCH_INTERVAL_WHEN_CHASE)
	{
		cntPathSearchInterval = 0.0f;
		canSearch = true;
	}

	// �U�����Ď��s�\�ɂ���܂ł̊Ԋu���v�Z
	cntAttackInterval += frameTime;
	if (cntAttackInterval >= ATTACK_INTERVAL[enemyData->type])
	{
		cntAttackInterval = 0.0f;
		canAttack = true;
	}

	// �ړI�n���X�g�𔻒肷�邽�߂Ɉړ����Ԃ��v�Z
	movingTime += frameTime;
	if (movingTime > MOVE_LIMIT_TIME)
	{
		movingTime = 0.0f;
		isDestinationLost = true;
	}

	onJump = false;
	friction = 1.0f;		// ���C�W��������
	acceleration *= 0.0f;	// �����x��������

	if (enemyData->type == TIGER || enemyData->type == WOLF)
	{
		animationManager->switchDefault();
	}


#ifdef _DEBUG
	if (enemyData->state == CHASE)
	{
		// �ǐՃX�e�[�g�ŗ��v���C���[�ɔF������Ă��Ȃ���Ԃ̓G���[
		assert(isNoticingPlayer[gameMasterNS::PLAYER_1P] || isNoticingPlayer[gameMasterNS::PLAYER_2P]);
	}
#ifdef RENDER_SENSOR
	debugSensor();
#endif// RENDER_SENSOR
#endif// _DEBUG

#ifdef _DEBUG
	if (enemyData->enemyID == debugEnemyID)
	{
		controlCamera(frameTime);
		moveOperation();
		enemyData->state = stateMachineNS::ENEMY_STATE::REST;

		if (input->wasKeyPressed('8'))
		{
			destination = player->center;
			movingTarget = &destination;
		}
		if (input->wasKeyPressed('7'))
		{
			searchPath();
		}
		if (input->isKeyDown('M'))
		{
			move(frameTime);
		}
		if (input->wasKeyPressed('U'))
		{
			naviMesh->isHitGrounding(NULL, &faceNumber, center);
		}
	}
#endif// _DEBUG
}


//=============================================================================
// ���㏈��
//=============================================================================
void Enemy::postprocess(float frameTime)
{
	// �ڒn����
	grounding();
	// �ǂ��菈��
	wallScratch();
	// ��������
	physicalBehavior();
	// �����̍X�V
	updatePhysics(frameTime);
	// �p���̍X�V
	updatePosture(frameTime);
	// �I�u�W�F�N�g�̍X�V
	Object::update();
	// ��������
	checkArrival();
	// �u���b�N�{�[�h�̍X�V
	updataBlackBoard(frameTime);
	// �X�e�[�g�̍X�V
	int stateNumber = stateMachine.run(frameTime, this);
	// �G�l�~�[�f�[�^�̍X�V
	enemyData->state = stateNumber;
	enemyData->position = position;
	enemyData->direction = axisZ.direction;
	// �����j�����s�����m�F
	checkAutoDestruction();

	//for (int i = 0; i < 2; i++)
	//{
	//	footsteps(player[i].position, i);
	//}
}
#pragma endregion


#pragma region [Action Query]
//=============================================================================
// �Z���T�[
//=============================================================================
bool Enemy::sensor()
{
	canSense = false;
	using namespace gameMasterNS;
	distanceToPlayer[PLAYER_1P] = between2VectorLength(center, player[gameMasterNS::PLAYER_1P].center);
	distanceToPlayer[PLAYER_2P] = between2VectorLength(center, player[gameMasterNS::PLAYER_2P].center);
	if (eyeSensor(PLAYER_1P) || earSensor(PLAYER_1P)) { isSensingPlayer[PLAYER_1P] = true; }
	if (eyeSensor(PLAYER_2P) || earSensor(PLAYER_2P)) { isSensingPlayer[PLAYER_2P] = true; }

	if(isSensingPlayer[PLAYER_1P] || isSensingPlayer[PLAYER_2P])
	{
		return true;
	}
	return false;
}


//=============================================================================
// �p�X����
//=============================================================================
void Enemy::searchPath()
{
	canSearch = false;
	isArraved = false;

	// �ړ��^�[�Q�b�g�̌��݈ʒu�܂ł̃G�b�W���X�g���擾
	//naviMesh->pathSearch(&edgeList, &naviFaceIndex, center, *movingTarget);
	// �ړ��^�[�Q�b�g�̌��݈ʒu��ړI�n�ɐݒ�
	destination = *movingTarget;

#ifdef _DEBUG
	if (enemyData->enemyID == debugEnemyID)
	{
		// �ړ��^�[�Q�b�g�̌��݈ʒu�܂ł̃G�b�W���X�g���擾
		naviMesh->pathSearch(&edgeList, &naviFaceIndex, center, *movingTarget);
		// �ړ��^�[�Q�b�g�̌��݈ʒu��ړI�n�ɐݒ�
		destination = *movingTarget;

		naviMesh->debugEdgeList = &edgeList;
	}
#endif
}


//=============================================================================
// �U��
//=============================================================================
void Enemy::attack()
{
	canAttack = false;
	cntAttackInterval = 0.0f;
	isAttacking = true;
}


//=============================================================================
// �ړ�
//=============================================================================
void Enemy::move(float frameTime)
{
	if (isAttacking)
	{
		attacking(frameTime);
		if (enemyData->type == TIGER || enemyData->type == WOLF)
		{
			animationManager->switchAttack();
		}
	}
	else
	{
		steering(frameTime);
		if (enemyData->type == TIGER || enemyData->type == WOLF)
		{
			animationManager->switchMove();
		}
	}
}
#pragma endregion


#pragma region [Action Implementation]
//=============================================================================
// ���o�Z���T�[
//=============================================================================
bool Enemy::eyeSensor(int playerType)
{
	float horizontalAngle;					// ���ʕ����ƃv���C���[�̐����p�x
	float verticalAngle;					// ���ʕ����ƃv���C���[�̐����p�x

	if (distanceToPlayer[playerType] > VISIBLE_DISTANCE[enemyData->type])
	{// ���F�\�����ł͂Ȃ�����
		return false;
	}	

	// �v���C���[����������p���ɓ����������ׂ�
	D3DXVECTOR3 horizontalVecToPlayer = player[playerType].center - center;
	slip(horizontalVecToPlayer, axisY.direction);
	slip(horizontalVecToPlayer, reverseAxisY.direction);
	formedRadianAngle(&horizontalAngle, axisZ.direction, horizontalVecToPlayer);
	if (horizontalAngle > HORIZONTAL_HALF_VIEWING_ANGLE[enemyData->type])
	{// ����p�O������
		return false;
	}

	// �v���C���[����������p���ɓ����������ׂ�
	D3DXVECTOR3 verticalVecToPlayer = player[playerType].center - center;
	slip(verticalVecToPlayer, axisX.direction);
	slip(verticalVecToPlayer, reverseAxisX.direction);
	formedRadianAngle(&verticalAngle, axisZ.direction, verticalVecToPlayer);
	if (verticalAngle > VERTICAL_HALF_VIEWING_ANGLE[enemyData->type])
	{// ����p�O������
		return false;
	}

	// �v���C���[�Ƃ̊Ԃɏ�Q�����Ȃ������ׂ�
	Ray ray;
	ray.initialize(center, player[playerType].center - center);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (ray.distance < distanceToPlayer[playerType])
		{// �ڒn���b�V����������
			return false;
		}
	}

	return true;	// �v���C���[������ɓ����Ă���
}


//=============================================================================
// ���o�Z���T�[
//=============================================================================
bool Enemy::earSensor(int playerType)
{
	// �v���C���[�̕����𕷂����鋗���i���o�����j�͈͓������ׂ�
	if (distanceToPlayer[playerType] < NOTICEABLE_DISTANCE_PLAYER[enemyData->type])
	{// ���o�����͈͓��ł���
		return true;
	}

	// �v���C���[�̏e���𕷂����鋗�������ׂ�
	if (distanceToPlayer[playerType] < NOTICEABLE_DISTANCE_PLAYER[enemyData->type] * SHOT_SOUND_SCALE &&
		player[playerType].getBulletManager()->getIsLaunched())
	{// �e���𕷂���ꂽ
		return true;
	}

	return false;
}


//=============================================================================
// �U��
//=============================================================================
void Enemy::attacking(float frameTime)
{
	if (isAttacking == false) { return; }

	// �U���x�N�g���쐬
	if (attackTime == 0.0f)
	{
		attackDirection = attackTarget->center - center;
		slip(attackDirection, groundNormal);
		D3DXVec3Normalize(&attackDirection, &attackDirection);
		// ������^����
		speed += attackDirection * ATTACK_SPEED[enemyData->type];
	}
	// �U�����ԏI������
	attackTime += frameTime;
	if (attackTime >= ATTACK_TIME[enemyData->type])
	{
		stopAttacking();
		isAttacking = false;
		attackTime = 0.0f;
	}
	if (onGround)
	{
		// ���x��������x�ێ������悤�����x�����Z�i�n�ʖ��C�W���̑ł������j
		acceleration += attackDirection * ATTACK_SPEED[enemyData->type] * MULTIPLICATION_TO_MAKE_ATTACK_ACC;
	}
}


//=============================================================================
// �X�e�A�����O
//=============================================================================
void Enemy::steering(float frameTime)
{
	D3DXVECTOR3 newMoveDirection;
	D3DXVECTOR3 tempDirection;


	// �i�r���b�V���ɂ��ړ��x�N�g�������i�X�e�A�����O�j
	naviMesh->steering(&moveDirection, &naviFaceIndex, center, destination, &edgeList);

	////moveDirection += newMoveDirection;

	////if (oldMoveDirection == D3DXVECTOR3(0, 0, 0))
	////{
	////	oldMoveDirection = axisZ.direction;
	////}
	//if (moveDirection == D3DXVECTOR3(0, 0, 0))
	//{
	//	tempDirection = axisZ.direction;
	//}
	//else
	//{
	//	tempDirection = moveDirection;
	//}

	////�J�[�u���[�V����
	//float rotationRadian;
	//if (formedRadianAngle(&rotationRadian, tempDirection, newMoveDirection))
	//{
	//	Base::anyAxisRotationSlerp(&moveMotion, groundNormal, rotationRadian, 1);
	//}
	//D3DXMATRIX matrix;
	//D3DXMatrixIdentity(&matrix);
	//D3DXMatrixRotationQuaternion(&matrix, &moveMotion);
	//D3DXVec3TransformCoord(&moveDirection, &tempDirection, &matrix);

	//moveDirection = newMoveDirection;
	
	// �����x�ɉ��Z
	acceleration += moveDirection * MOVE_ACC[enemyData->type];
}
#pragma endregion


#pragma region [Postprocess Implementation]
//=============================================================================
// �ڒn����
//=============================================================================
void Enemy::grounding()
{
	onGroundBefore = onGround;
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(center, gravityDirection);
	bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);
	groundNormal = gravityRay.normal;

	if (hit == false)
	{// �G�l�~�[�͒n�ʂ̖����󒆂ɂ���
		onGround = false;
		return;
	}

	float halfHeight = size.y / 2;
	if (halfHeight + difference >= gravityRay.distance)
	{// �G�l�~�[�͒n��ɐڒn���Ă���
		onGround = true;

		if (onJump)
		{
			// �߂荞�ݕ␳�i���݈ʒu + �d�͕��� * �߂荞�݋����j
			setPosition(center + gravityRay.direction * (gravityRay.distance - halfHeight));
			// �d�͕����ɗ�����Ƃ������ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
			if (speed.y < 0) setSpeed(slip(speed, gravityRay.normal));
		}
		else
		{
			// �߂荞�ݕ␳�i���݈ʒu + �d�͕��� * �߂荞�݋����j
			setPosition(position + gravityRay.direction * (gravityRay.distance - halfHeight));
			// �ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
			setSpeed(slip(speed, gravityRay.normal));
			// ���O�t���[���ŋ󒆂ɂ����Ȃ�W�����v�I���Ƃ���
			if (onGroundBefore == false) jumping = false;
		}
	}
	else
	{// �G�l�~�[�͒n�ʂ̂���󒆂ɂ���
		onGround = false;
	}
}


//=============================================================================
// �ǂ��菈��
//=============================================================================
void Enemy::wallScratch()
{
	ray.update(center, D3DXVECTOR3(speed.x, 0, speed.z));

	// �ǂ��菈��
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		// �߂荞�ݕ␳
		position += ray.normal * (radius / 2 - ray.distance);
		// �ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
		speed = slip(speed, ray.normal);
	}
}


//=============================================================================
// ��������
//=============================================================================
void Enemy::physicalBehavior()
{
	// �ڒn���Ă��Ȃ��Ƃ��̂ݏd�͉����x��������
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(position, gravityDirection);
	if (onGround == false)
	{
		setGravity(gravityDirection, GRAVITY_FORCE);
	}

	//if (onGround)
	//{
	//	// �n��ɂ���ꍇ�͏d�͕������؂�
	//	// ��������O���ƍŌ�ɉ����x.y�ɓ����Ă����d�͉����x���X���b�v���č⓹���葱����
	//	acceleration.y = 0.0f;
	//}

	// �󒆂ɕ����^�C�~���O�ŉ����x�؂�
	if (onGround == false && onGroundBefore)
	{
		acceleration *= 0.0f;
	}

	// ���n����^�C�~���O�ő��x���ቺ����
	if (onGround && onGroundBefore == false)
	{
		friction *= GROUND_FRICTION;
	}

	// �n�㖀�C�W��
	if (onGround)
	{
		friction *= MOVE_FRICTION;
	}

	//// ��~
	//float speedPerSecond = D3DXVec3Length(&speed);
	//if (isExecutingMoveOperation == false &&
	//	speedPerSecond < STOP_SPEED)
	//{
	//	speed *= 0.0f;
	//}

	// �������x���E�̐ݒ�
	if (speed.y < -FALL_SPEED_MAX)
	{
		speed.y = -FALL_SPEED_MAX;
	}
}


//=============================================================================
// �����̍X�V
//=============================================================================
void Enemy::updatePhysics(float frameTime)
{
	// �����x�̉e���𑬓x�ɗ^����
	speed += acceleration * frameTime;
	// ���x�ɖ��C�̉e����^����
	speed *= friction;
	// ���x�̉e�����ʒu�ɗ^����
	position += speed * frameTime;
}


//=============================================================================
// �p���̍X�V
//=============================================================================
void Enemy::updatePosture(float frameTime)
{
	moveDirection = slip(moveDirection, axisY.direction);
	// moveDirection�͈ړ��֐��ōX�V�����
	postureControl(axisZ.direction, moveDirection, frameTime * 6/*1�b��6�{���w�� = 0.1�b�Ŏp���ύX*/);
}


//=============================================================================
// ��������
//=============================================================================
void Enemy::checkArrival()
{
	float distance = between2VectorLength(destination, position);

	if (distance <= ARRIVAL_DISTANCE)
	{
		isArraved = true;
	}
	// ���ʂ�߂��΍��������
	//D3DXVECTOR3 point = nearestPointOnLine(enemyData->position/*1�t���[���O�̍��W*/, position, destination);
}


//=============================================================================
// �u���b�N�{�[�h�̍X�V
//=============================================================================
void Enemy::updataBlackBoard(float frameTime)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		// �v���C���[�̔F��
		if (isSensingPlayer[i])
		{
			isNoticingPlayer[i] = true;
			noticedTimePlayer[i] = 0.0f;
		}

		// �v���C���[�̋L��
		if (isNoticingPlayer[i])
		{
			noticedTimePlayer[i] += frameTime;
			if (noticedTimePlayer[i] >= PLAYER_MEMORIZED_TIME[enemyData->type])
			{
				noticedTimePlayer[i] = 0.0f;
				isNoticingPlayer[i] = false;
			}
		}
	}

	isPayingNewAttention = false;
}
#pragma endregion


#pragma region [Prepare State Change]
//=============================================================================
// �ǐՃX�e�[�g�̏���
//=============================================================================
void Enemy::prepareChase()
{
	// �ŏ�����ǐՃX�e�[�g�ł���G�l�~�[�����Ƃ��̊֐����o�R���Ȃ�
	// �l�X�Ȗ�肪�o�Ă���̂Œ��ӂ��ꂽ��

	cntPathSearchInterval = 0.0f;
	playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_EnemyActive, false, NULL, false, NULL };
	SoundInterface::SE->playSound(&playParameters);

	// �ǐՃ}�[�N�̍쐬
	createMark();
}


//=============================================================================
// �x���X�e�[�g�̏���
//=============================================================================
void Enemy::preparePatrol()
{
	isDestinationLost = true;

	// �ǐՃ}�[�N�̍폜
	deleteMark();
}


//=============================================================================
// �x�e�X�e�[�g�̏���
//=============================================================================
void Enemy::prepareRest()
{
	// �ǐՃ}�[�N�̍폜
	deleteMark();
}


//=============================================================================
// �c���[�U���X�e�[�g�̏���
//=============================================================================
void Enemy::prepareAttackTree()
{
	setAttackTarget(enemyData->targetTree);
	setMovingTarget(enemyData->targetTree->getPosition());

	// �ǐՃ}�[�N�̍폜
	deleteMark();
}


//=============================================================================
// ���S�X�e�[�g�̏���
//=============================================================================
void Enemy::prepareDie()
{
	// ���S�G�t�F�N�g�̍Đ�
	deathEffect = new DeathEffect(&center);
	deathEffect->scale *= DEATH_EFFECT_SCALE[enemyData->type];
	effekseerNS::play(0, deathEffect);

	// �ǐՃ}�[�N�̍폜
	deleteMark();
}
#pragma endregion


#pragma region [State]
//=============================================================================
//�u�ǐՁv�X�e�[�g
//=============================================================================
void Enemy::chase(float frameTime)
{
	if (canSense)
	{
		sensor();
	}

	if (canSearch)
	{
		setPlayerChaseTarget();
		searchPath();
	}

	//D3DXQUATERNION quaternion;
	//D3DXQuaternionIdentity(&quaternion);
	//Base::postureControl(&quaternion, D3DXVECTOR3(0, 0, 0), player[chasingPlayer].position - position, 1.0f);
	//D3DXMATRIX matrix;
	//D3DXMatrixIdentity(&matrix);
	//D3DXMatrixRotationQuaternion(&matrix, &quaternion);
	////D3DXVec3TransformCoord(&markDirection, &markDirection, &matrix);
	//markFront->rotation.x = 

	// �ǐՃ}�[�N�i�\�j�̍X�V
	markFront->position = position + D3DXVECTOR3(0.0f, MARK_FLOATING_HEIGHT, 0.0f);
	formedRadianAngle(&markFront->rotation.y, D3DXVECTOR3(0.0f, 0.0f, 1.0f), player[chasingPlayer].position - position);
	if (position.x < player[chasingPlayer].position.x) { markFront->rotation.y = -markFront->rotation.y; }
	// �ǐՃ}�[�N�i���j�̍X�V
	markBack->position = position + D3DXVECTOR3(0.0f, MARK_FLOATING_HEIGHT, 0.0f);
	formedRadianAngle(&markBack->rotation.y, D3DXVECTOR3(0.0f, 0.0f, 1.0f), player[chasingPlayer].position - position);
	if (position.x < player[chasingPlayer].position.x)
	{
		markBack->rotation.y = D3DX_PI - markBack->rotation.y;
	}
	else
	{
		markBack->rotation.y = -(D3DX_PI - markBack->rotation.y);
	}

	// �ړ�
	move(frameTime);
}


//=============================================================================
//�u�x���v�X�e�[�g
//=============================================================================
void Enemy::patrol(float frameTime)
{
	if (canSense)
	{
		sensor();
	}

	if (isPayingNewAttention)
	{
		static const float TEMP_DISTANCE = 20.0f;
		D3DXVec3Normalize(&attentionDirection, &attentionDirection);
		destination = position + attentionDirection * TEMP_DISTANCE;

		isDestinationLost = false;
		setMovingTarget(&destination);
		searchPath();
	}

	move(frameTime);
}


//=============================================================================
//�u�x�e�v�X�e�[�g
//=============================================================================
void Enemy::rest(float frameTime)
{

}


//=============================================================================
//�u�c���[�U���v�X�e�[�g
//=============================================================================
void Enemy::attackTree(float frameTime)
{
	if (canSense)
	{
		sensor();
	}

	if (canSearch)
	{
		//setPlayerChaseTarget();
		searchPath();
	}

	move(frameTime);
}


//=============================================================================
//�u���S�v�X�e�[�g
//=============================================================================
void Enemy::die(float frameTime)
{
	// ���ʐ��O�̐U��
	speed.x += (rand() % 8) / 10.0f;
	speed.x -= 0.4f;
	speed.z += (rand() % 8) / 10.0f;
	speed.z -= 0.4f;

	if (enemyData->type == TIGER || enemyData->type == WOLF)
	{
		animationManager->switchDead();
	}


	// ���S��
	if (cntTimeDie > DIE_STATE_TIME)
	{
		enemyData->isAlive = false;
		enemyData->deadTime = gameMaster->getGameTime();

		// �A�C�e���h���b�v
		if (rand() % ITEM_DROP_PROBABILITY_DENOMINATOR[enemyData->type] == 0)
		{
			ItemManager* itemManager = ItemManager::get();
			itemNS::ItemData itemData;
			itemData.itemID = itemManager->issueNewItemID();
			itemData.type = itemNS::BATTERY;
			itemData.defaultPosition = position;
			itemData.defaultDirection = axisZ.direction;
			itemManager->createItem(itemData);
		}
	}

	// ���S�X�e�[�g���Ԃ̃J�E���g�A�b�v
	cntTimeDie += frameTime;
}
#pragma endregion


#pragma region [Sound]
//=============================================================================
// �T�E���h�v���C�p�����[�^�̎擾
//=============================================================================
PLAY_PARAMETERS Enemy::getPlayParameters(SE_3D soundType, int enemyType)
{
	PLAY_PARAMETERS parameter = {
		ENDPOINT_VOICE_LIST::ENDPOINT_S3D,	//�G���h�|�C���g�{�C�XID
		NULL,								// �T�E���hID ���Őݒ�
		false,								// ���[�v
		NULL,								// �Đ����x
		true,								// 3D�T�E���h��
		-1,									// �v���C���[ID
	};

	switch (soundType)
	{
	//------
	// ����
	//------
	case FOOT_STEPS_SE:
		switch (enemyType)
		{
		case WOLF:
			parameter.soundId = S3D_LIST::S3D_FOOTSTEP_01;// ��
			break;

		case TIGER:
			parameter.soundId = S3D_LIST::S3D_FOOTSTEP_02;// ��
			break;

		case BEAR:
			parameter.soundId = S3D_LIST::S3D_FOOTSTEP_03;// ��
			break;
		}
		break;

	//--------
	// �U����
	//--------
	case ATTACK_SE:
	{
	}
	break;

	//--------
	// ���S��
	//--------
	case DIE_SE:
	{
	}
	break;

	}// switch(soundType)

	return parameter;
}


//=============================================================================
// �����̏���
//=============================================================================
void Enemy::footsteps(D3DXVECTOR3 playerPos, int playerID)
{
	PLAY_PARAMETERS playParameter = getPlayParameters(FOOT_STEPS_SE, enemyData->type);
	playParameter.playerID = playerID;

	// �悭�킩��񂩂����낵��!

	//float distance = D3DXVec3Length(&(position - playerPos));
	//float volume = 0.0f;
	//if (distance < DISTANCE_MAX)
	//{
	//	volume = (DISTANCE_MAX - distance) / DISTANCE_MAX;
	//}

	//SoundInterface::S3D->SetVolume(footStepsParameters[playerID], volume);
}
#pragma endregion


#pragma region [Other]
//=============================================================================
// ���X�|�[���Ɣj�����s�������Ȕ���
//=============================================================================
void Enemy::reset()
{
	if (isDestinationLost)
	{
		position = enemyData->position;
		axisZ.direction = enemyData->defaultDirection;
		enemyData->state = enemyData->defaultState;
	}

}


//=============================================================================
// �����j�����s�����m�F
//=============================================================================
void Enemy::checkAutoDestruction()
{
	if (position.y <= AUTO_DESTRUCTION_HEIGHT)
	{// ���̉��ɗ��������ꍇ
		enemyData->isAlive = false;
	}
}


//=============================================================================
// �K�؂ȃv���C���[���^�[�Q�b�g�ɐݒ肷��
//=============================================================================
void Enemy::setPlayerChaseTarget()
{
	if (isNoticingPlayer[gameMasterNS::PLAYER_1P] && isNoticingPlayer[gameMasterNS::PLAYER_2P] == false)
	{
		setMovingTarget(&player[gameMasterNS::PLAYER_1P].position);
		attackTarget = &player[gameMasterNS::PLAYER_1P];
		chasingPlayer = gameMasterNS::PLAYER_1P;
	}
	else if (isNoticingPlayer[gameMasterNS::PLAYER_1P] == false && isNoticingPlayer[gameMasterNS::PLAYER_2P])
	{
		setMovingTarget(&player[gameMasterNS::PLAYER_2P].position);
		attackTarget = &player[gameMasterNS::PLAYER_2P];
		chasingPlayer = gameMasterNS::PLAYER_2P;
	}
	else if (isNoticingPlayer[gameMasterNS::PLAYER_1P] && isNoticingPlayer[gameMasterNS::PLAYER_2P])
	{
		if (distanceToPlayer[gameMasterNS::PLAYER_1P] < distanceToPlayer[gameMasterNS::PLAYER_2P])
		{
			setMovingTarget(&player[gameMasterNS::PLAYER_1P].position);
			attackTarget = &player[gameMasterNS::PLAYER_1P];
			chasingPlayer = gameMasterNS::PLAYER_1P;
		}
		else
		{
			setMovingTarget(&player[gameMasterNS::PLAYER_2P].position);
			attackTarget = &player[gameMasterNS::PLAYER_2P];
			chasingPlayer = gameMasterNS::PLAYER_2P;

		}
	}
}


//=============================================================================
// �f�o�b�O�p�ړI�n�ݒ�
//=============================================================================
void Enemy::setDebugDestination()
{
	// �ړI�n�̃Z�b�g
	static const float TEMP_DISTANCE = 20.0f;
	D3DXVECTOR3 tempDir;
	tempDir = D3DXVECTOR3((float)(rand() % 1000), 0, float(rand() % 1000));
	tempDir -= D3DXVECTOR3(500, 0, 500);
	D3DXVec3Normalize(&tempDir, &tempDir);
	tempDir *= TEMP_DISTANCE;
	destination = position + tempDir;

#if 0	// �R����ړI�n�Ƃ���
	destination = D3DXVECTOR3(-30, 158, 35);
#endif
	setMovingTarget(&destination);
}


//=============================================================================
// ����H�����O�o�b�t�@�̍X�V
//=============================================================================
void Enemy::updatePatrolRoute()
{
	if (onGround && isArraved)
	{// �������Ă����玟�̍��W�Ɉړ��^�[�Q�b�g���X�V����
		nextIndexOfRoute %= enemyData->numRoute;
		movingTarget = &enemyData->patrolRoute[nextIndexOfRoute];
	}

	if (isDestinationLost)
	{//��

	}
}


//=============================================================================
// �ǐՃ}�[�N�̍쐬
//=============================================================================
void Enemy::createMark()
{
	// �\
	markFront = new enemyChaseMarkNS::StandardChaseMark(position + D3DXVECTOR3(0.0f, MARK_FLOATING_HEIGHT, 0.0f));
	markFront->scale = MARK_SCALE[enemyData->type];
	markRenderer->generateMark(markFront);
	// ��
	markBack = new enemyChaseMarkNS::StandardChaseMark(position + D3DXVECTOR3(0.0f, MARK_FLOATING_HEIGHT, 0.0f));
	markBack->scale = MARK_SCALE[enemyData->type];
	markBack->rotation = D3DXVECTOR3(D3DX_PI / 2.0f, 0, 0);
	markRenderer->generateMark(markBack);
}


//=============================================================================
// �ǐՃ}�[�N�̔j��
//=============================================================================
void Enemy::deleteMark()
{
	if (markFront != NULL)
	{
		markRenderer->deleteMark(markFront);
		markFront = NULL;	// �_���O�����O�|�C���^�ɂȂ�Ȃ��͂������d�l�ύX���ɔ�����
	}

	if (markBack != NULL)
	{
		markRenderer->deleteMark(markBack);
		markBack = NULL;	// �_���O�����O�|�C���^�ɂȂ�Ȃ��͂������d�l�ύX���ɔ�����
	}
}
#pragma endregion


#pragma region [Getter & Setter]
//=============================================================================
// Getter
//=============================================================================
int Enemy::getEnemyID() { return enemyData->enemyID; }
int Enemy::getNumOfEnemy() { return numOfEnemy; }
EnemyData* Enemy::getEnemyData() { return enemyData; }
BoundingSphere*  Enemy::getSphereCollider() { return &sphereCollider; }
LPD3DXMESH Enemy::getMesh() { return box->mesh; }
bool Enemy::getNoticedOfPlayer(int playerType) { return isNoticingPlayer[playerType]; }
bool Enemy::getIsAttacking() { return isAttacking; }
int Enemy::getChasingPlayer() { return chasingPlayer; };


//=============================================================================
// Setter
//=============================================================================
void Enemy::resetNumOfEnemy()
{
	numOfEnemy = 0;
}

void Enemy::damage(int _damage)
{
	enemyData->hp -= _damage;
	if (enemyData->hp <= 0)
	{
		enemyData->hp = 0;
	}
}

void Enemy::stopAttacking()
{
	isAttacking = false;
	attackTime = 0.0f;
	speed *= ATTACKED_FRICTION;// ����
}

void Enemy::setAttention(D3DXVECTOR3 setting)
{
	attentionDirection = setting;
	isPayingNewAttention = true;
}

void Enemy::setMovingTarget(D3DXVECTOR3* _target)
{
	movingTarget = _target;
}

void Enemy::setAttackTarget(Object* _target)
{
	attackTarget = _target;
}
#pragma endregion


#pragma region [Debug]
#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����ݒ�
//=============================================================================
void Enemy::setDebugEnvironment()
{
	device = getDevice();
	input = getInput();
	keyTable = KEY_TABLE_1P;
	reverseValueXAxis = CAMERA_SPEED;		//����w��
	reverseValueYAxis = CAMERA_SPEED;		//����x��
}


//=============================================================================
// �J�����̑���ƍX�V
//=============================================================================
void Enemy::controlCamera(float frameTime)
{
	// ���쎲���]����
	if (input->wasKeyPressed(keyTable.reverseCameraX))reverseValueXAxis *= -1;
	if (input->wasKeyPressed(keyTable.reverseCameraY))reverseValueYAxis *= -1;
	// �}�E�X����
	camera->rotation(D3DXVECTOR3(0, 1, 0), (float)(input->getMouseRawX() * reverseValueXAxis));
	camera->rotation(camera->getHorizontalAxis(), (float)(input->getMouseRawY() * reverseValueYAxis));
	// �R���g���[������
	if (input->getController()[0]->checkConnect()) {
		camera->rotation(D3DXVECTOR3(0, 1, 0), input->getController()[0]->getRightStick().x*0.1f*frameTime*reverseValueXAxis);
		camera->rotation(camera->getHorizontalAxis(), input->getController()[0]->getRightStick().y*0.1f*frameTime*reverseValueYAxis);
	}
}


//=============================================================================
// �ړ�����
//=============================================================================
void Enemy::moveOperation()
{
	//�O�֐i��
	if (input->isKeyDown(keyTable.front)) {
		move(D3DXVECTOR2(0, -1), camera->getDirectionX(), camera->getDirectionZ());
	}
	//���֐i��
	if (input->isKeyDown(keyTable.back)) {
		move(D3DXVECTOR2(0, 1), camera->getDirectionX(), camera->getDirectionZ());
	}
	//���֐i��
	if (input->isKeyDown(keyTable.left)) {
		move(D3DXVECTOR2(-1, 0), camera->getDirectionX(), camera->getDirectionZ());
	}
	//�E�֐i��
	if (input->isKeyDown(keyTable.right))
	{
		move(D3DXVECTOR2(1, 0), camera->getDirectionX(), camera->getDirectionZ());
	}
}


//=============================================================================
// �ړ�
//=============================================================================
void Enemy::move(D3DXVECTOR2 operationDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ)
{
	if (operationDirection.x == 0 && operationDirection.y == 0)return;//���͒l��0�ȉ��Ȃ�Έړ����Ȃ�
	//Y�������ւ̐������폜����
	D3DXVECTOR3 front = slip(cameraAxisZ, axisY.direction);
	D3DXVECTOR3 right = slip(cameraAxisX, axisY.direction);
	D3DXVec3Normalize(&front, &front);//���K��
	D3DXVec3Normalize(&right, &right);//���K��

	//����������J������XZ�����ɏ��������ړ��x�N�g���֕ϊ�����
	D3DXVECTOR3 moveDirection = operationDirection.x*right + -operationDirection.y*front;
	if (onGround)
	{
		acceleration += moveDirection * MOVE_ACC[enemyData->type] * 3; 
	}
	else
	{
		acceleration += moveDirection * MOVE_ACC[enemyData->type] * AIR_MOVE_ACC_MAGNIFICATION;
	}

	//�p������
	postureControl(getAxisZ()->direction, moveDirection, 0.1f);
}


//=============================================================================
// �f�o�b�O�Z���T�[
//=============================================================================
void Enemy::debugSensor()
{
	D3DXQUATERNION	rayQuaternion1, rayQuaternion2;
	D3DXMATRIX		rayMatrix;
	D3DXVECTOR3		direction;

	// ���E�̃��C�����
	for (int height = -1; height < 2; height += 2)
	{
		for (int width = -1; width < 2; width += 2)
		{
			D3DXQuaternionIdentity(&rayQuaternion1);
			D3DXQuaternionIdentity(&rayQuaternion2);
			D3DXMatrixIdentity(&rayMatrix);
			direction = axisZ.direction;

			D3DXQuaternionRotationAxis(&rayQuaternion1,
				&axisX.direction, height * VERTICAL_HALF_VIEWING_ANGLE[enemyData->type]);

			D3DXQuaternionRotationAxis(&rayQuaternion2,
				&axisY.direction, width * HORIZONTAL_HALF_VIEWING_ANGLE[enemyData->type]);

			rayQuaternion2 *= rayQuaternion1;
			D3DXMatrixRotationQuaternion(&rayMatrix, &rayQuaternion2);
			D3DXVec3TransformCoord(&direction, &direction, &rayMatrix);

			if (height == -1 && width == -1) { eyeAngleRay[0].initialize(center, direction); }
			if (height == -1 && width == 1) { eyeAngleRay[1].initialize(center, direction); }
			if (height == 1 && width == -1) { eyeAngleRay[2].initialize(center, direction); }
			if (height == 1 && width == 1) { eyeAngleRay[3].initialize(center, direction); }
		}
	}

	// ���E���C�𔒂ŕ`��
	for (int i = 0; i < 4; i++)
	{
		eyeAngleRay[i].color = D3DXCOLOR(255, 255, 255, 155);
	}

	bool sound = false;
	if (canSense)
	{
		if (sensor())
		{
			// ���E�ɓ�������ԓ_��
			for (int i = 0; i < 4; i++)
			{
				eyeAngleRay[i].color = D3DXCOLOR(255, 0, 0, 255);
			}

			sound = true;
		}

		if (sound)
		{
			// ����炷
			//playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_AnnounceTelop, false, NULL, false, NULL };
			//SoundInterface::SE->playSound(&playParameters);	//SE�Đ�
		}

	}
}


//=============================================================================
// ImGUI�ւ̏o��
//=============================================================================
void Enemy::outputGUI()
{

	if (ImGui::CollapsingHeader("EnemyInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//�ʒu
		ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
		ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);						//�X�P�[��
		ImGui::SliderFloat("radius", &radius, 0, limitTop);								//���a
		ImGui::SliderFloat("alpha", &alpha, 0, 255);									//���ߒl
		ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//���x
		ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//�����x
		ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);					//�d��
		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));
		ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
		ImGui::Checkbox("onActive", &onActive);											//�A�N�e�B�u���t���O
	}
}
#endif
#pragma endregion
