//-----------------------------------------------------------------------------
// �G�l�~�[���N���X [Enemy.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "Enemy.h"
#include "ImguiManager.h"
#include "Sound.h"
using namespace enemyNS;
using namespace stateMachineNS;


int Enemy::numOfEnemy = 0;			// �G�l�~�[�̑���
#ifdef _DEBUG
int Enemy::debugEnemyID = -1;		// �f�o�b�O����G�l�~�[��ID
#endif//_DEBUG


//=============================================================================
// �R���X�g���N�^
//=============================================================================
Enemy::Enemy(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData)
{
	numOfEnemy++;								// �G�l�~�[�̐������Z
	enemyData = _enemyData;						// �G�l�~�[�f�[�^���Z�b�g

	// �X�e�[�g�}�V���̏�����
	stateMachine.initialize(enemyData->defaultState);

	// �i�r�Q�[�V�������b�V��
	naviMesh = NavigationMesh::getNaviMesh();
	edgeList = NULL;
	naviFaceIndex = -1;
	shouldSearchPath = false;

	// �ړ�
	onMove = false;
	movingTarget = NULL;
	destination = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �����ݒ�
	onGravity = true;							// �d�͗L����
	difference = DIFFERENCE_FIELD;				// �t�B�[���h�␳������ݒ�

	// �t���O������
	onGround = true;
	onGroundBefore = true;
	onJump = false;
	jumping = false;

	// �I�u�W�F�N�g������
	position = enemyData->position;
	axisZ.direction = enemyData->direction;
	sphereCollider.initialize(&position, _staticMesh->mesh);
	radius = sphereCollider.getRadius();
	Object::initialize(&position);
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);

#ifdef RENDER_SENSOR
	hearingSphere[0].initialize(&centralPosition, NOTICEABLE_DISTANCE_PLAYER[enemyData->type]);
	hearingSphere[1].initialize(&centralPosition, NOTICEABLE_DISTANCE_PLAYER[enemyData->type] * SHOT_SOUND_SCALE);
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

	numOfEnemy--;
}


//=============================================================================
// �X�V����
//=============================================================================
void Enemy::update(float frameTime)
{
#ifdef _DEBUG
	if (enemyData->enemyID == debugEnemyID)
	{
		moveOperation();			// �ړ�����
		controlCamera(frameTime);	// �J��������
		if (input->wasKeyPressed('7'))
		{
			destination = position;
		}
		if (input->wasKeyPressed('8'))
		{
			naviMesh->pathSearch(&edgeList, &naviFaceIndex, centralPosition, destination);
			naviMesh->dumpEdgeList();
			naviMesh->affectToEdgeVertex();
			naviMesh->debugRenderEdge(edgeList);
			setMovingTarget(&destination);
			setMove(true);
		}
	}
#ifdef RENDER_SENSOR
	debugSensor();				// ���E
	// �v���C���[�ւ̒������C��`�悷��
	gazePlayer.initialize(centralPosition, *player->getCentralPosition() - centralPosition);
	gazePlayer.color = D3DXCOLOR(255, 0, 0, 255);
#endif// RENDER_SENSOR
#endif// _DEBUG

	// �Z���T�[
	if (canUseSensor)
	{
		if (eyeSensor(gameMasterNS::PLAYER_1P) || earSensor(gameMasterNS::PLAYER_1P))
		{
			isNoticedPlayer1 = true;
		}
		if (eyeSensor(gameMasterNS::PLAYER_2P) || earSensor(gameMasterNS::PLAYER_2P))
		{
			isNoticedPlayer2 = true;
		}
		canUseSensor = false;
	}

	// �o�H�T��
	if (shouldSearchPath)
	{
		naviMesh->pathSearch(&edgeList, &naviFaceIndex, centralPosition, *movingTarget);
		shouldSearchPath = false;
	}

	// �ړ�
	if (onMove)
	{
		steering();
	}
	// �ڒn����
	grounding();
	// ��������
	physicalBehavior();
	// �����̍X�V
	updatePhysics(frameTime);
	// �I�u�W�F�N�g�̍X�V
	Object::update();
	// ���S���W�̍X�V
	updateCentralCood();
	// �X�e�[�g�̍X�V
	int stateNumber = stateMachine.run(frameTime, enemyData->type);
	// �G�l�~�[�f�[�^�̍X�V
	enemyData->state = stateNumber;
	enemyData->position = position;
	enemyData->direction = axisZ.direction;
}


//=============================================================================
// ���O����
//=============================================================================
void Enemy::preprocess(float frameTime)
{
	// �Z���T�[�X�V�v�Z����
	sensorTime += frameTime;
	if (sensorTime > SENSOR_UPDATE_INTERVAL[enemyData->type])
	{
		sensorTime = 0.0f;
		canUseSensor = true;
	}

	onJump = false;
	friction = 1.0f;		// ���C�W��������
	acceleration *= 0.0f;	// �����x��������
}


//=============================================================================
// ���o�Z���T�[
//=============================================================================
bool Enemy::eyeSensor(int playerType)
{
	float distanceBetweenPlayerAndEnemy;	// �v���C���[�Ƃ̋���
	float horizontalAngle;					// ���ʕ����ƃv���C���[�̐����p�x
	float verticalAngle;					// ���ʕ����ƃv���C���[�̐����p�x

	// �v���C���[�����F�\�ȋ����ɂ��邩���ׂ�
	distanceBetweenPlayerAndEnemy = D3DXVec3Length(&(*player[playerType].getCentralPosition() - centralPosition));
	if (distanceBetweenPlayerAndEnemy > VISIBLE_DISTANCE[enemyData->type])
	{// ���F�\�����ł͂Ȃ�����
		return false;
	}	

	// �v���C���[����������p���ɓ����������ׂ�
	D3DXVECTOR3 horizontalVecToPlayer = *player[playerType].getCentralPosition() - centralPosition;
	slip(horizontalVecToPlayer, axisY.direction);
	slip(horizontalVecToPlayer, reverseAxisY.direction);
	formedRadianAngle(&horizontalAngle, axisZ.direction, horizontalVecToPlayer);
	if (horizontalAngle > HORIZONTAL_HALF_VIEWING_ANGLE[enemyData->type])
	{// ����p�O������
		return false;
	}

	// �v���C���[����������p���ɓ����������ׂ�
	D3DXVECTOR3 verticalVecToPlayer = *player[playerType].getCentralPosition() - centralPosition;
	slip(verticalVecToPlayer, axisX.direction);
	slip(verticalVecToPlayer, reverseAxisX.direction);
	formedRadianAngle(&verticalAngle, axisZ.direction, verticalVecToPlayer);
	if (verticalAngle > VERTICAL_HALF_VIEWING_ANGLE[enemyData->type])
	{// ����p�O������
		return false;
	}

	// �v���C���[�Ƃ̊Ԃɏ�Q�����Ȃ������ׂ�
	Ray ray;
	ray.initialize(centralPosition, *player[playerType].getCentralPosition() - centralPosition);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (ray.distance < distanceBetweenPlayerAndEnemy)
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
	float distanceBetweenPlayerAndEnemy = D3DXVec3Length(&(*player[playerType].getPosition() - position));

	// �v���C���[�̕����𕷂����鋗���i���o�����j�͈͓������ׂ�
	if (distanceBetweenPlayerAndEnemy < NOTICEABLE_DISTANCE_PLAYER[enemyData->type])
	{// ���o�����͈͓��ł���
		return true;
	}

	// �v���C���[�̏e���𕷂����鋗�������ׂ�
	if (distanceBetweenPlayerAndEnemy < NOTICEABLE_DISTANCE_PLAYER[enemyData->type] * SHOT_SOUND_SCALE &&
		player[playerType].getWhetherShot())
	{// �e���𕷂���ꂽ
		return true;
	}

	return false;
}


//=============================================================================
// �X�e�A�����O
//=============================================================================
void Enemy::steering()
{
	D3DXVECTOR3 moveDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (edgeList != NULL && edgeList->isEnpty())
	{
		// �G�b�W���X�g�̔j��
		edgeList->terminate();
		SAFE_DELETE(edgeList);
	}

	if(edgeList == NULL)
	{
		// �ړI�n�ɒ����ړ�
		moveDirection = destination - position;
		D3DXVec3Normalize(&moveDirection, &moveDirection);
	}
	else
	{	
		// �i�r���b�V���ɂ��ړ��x�N�g������
		naviMesh->steering(&moveDirection, &naviFaceIndex, centralPosition, edgeList);
	}

	acceleration += moveDirection * MOVE_ACC[enemyData->type];
}


//=============================================================================
// �ڒn����
//=============================================================================
void Enemy::grounding()
{
	onGroundBefore = onGround;
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(centralPosition, gravityDirection);
	bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);

	if (hit == false)
	{// �G�l�~�[�͒n�ʂ̖����󒆂ɂ���
		onGround = false;
		return;
	}

	if (radius + difference >= gravityRay.distance)
	{// �G�l�~�[�͒n��ɐڒn���Ă���
		onGround = true;

		if (onJump)
		{
			// �߂荞�ݕ␳�i���݈ʒu + �d�͕��� * �߂荞�݋����j
			setPosition(centralPosition + gravityRay.direction * (gravityRay.distance - radius));
			// �d�͕����ɗ�����Ƃ������ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
			if (speed.y < 0) setSpeed(slip(speed, gravityRay.normal));
		}
		else
		{
			// �߂荞�ݕ␳�i���݈ʒu + �d�͕��� * �߂荞�݋����j
			setPosition(position + gravityRay.direction * (gravityRay.distance - radius));
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
// ���S���W�n�̍X�V
//=============================================================================
void Enemy::updateCentralCood()
{
	centralPosition = position + sphereCollider.getCenter();
	D3DXMatrixTranslation(&centralMatrixWorld, centralPosition.x, centralPosition.y, centralPosition.z);
	axisX.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	axisY.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	axisZ.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
	reverseAxisX.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	reverseAxisY.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	reverseAxisZ.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
}


//=============================================================================
// �d�͔������b�V���i�ڒn���b�V���j�̐ݒ�
//=============================================================================
void Enemy::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}


//=============================================================================
// �d�͔������b�V���i�ڒn���b�V���j�̐ݒ�
//=============================================================================
void Enemy::setPlayer(Player* _player)
{
	player = _player;
}


//=============================================================================
// �G�l�~�[�̃I�u�W�F�N�g�̐���������
//=============================================================================
void Enemy::resetNumOfEnemy()
{
	numOfEnemy = 0;
}


//=============================================================================
// �����̏���
//=============================================================================
void Enemy::footsteps(D3DXVECTOR3 playerPos, int playerID)
{
	float distance = D3DXVec3Length(&(position - playerPos));
	float volume = 0.0f;
	if (distance < DISTANCE_MAX)
	{
		volume = (DISTANCE_MAX - distance) / DISTANCE_MAX;
	}

	SoundInterface::S3D->SetVolume(playParameters[playerID], volume);
}


//=============================================================================
// Getter
//=============================================================================
int Enemy::getEnemyID() { return enemyData->enemyID; }
int Enemy::getNumOfEnemy() { return numOfEnemy; }
EnemyData* Enemy::getEnemyData() { return enemyData; }
BoundingSphere*  Enemy::getSphereCollider() { return &sphereCollider; }
D3DXVECTOR3*  Enemy::getCentralPosition() { return &centralPosition; }
D3DXMATRIX* Enemy::getCentralMatrixWorld() { return &centralMatrixWorld; }


//=============================================================================
// Setter
//=============================================================================
void Enemy::setMove(bool setting) { onMove = setting; }
void Enemy::setMovingTarget(D3DXVECTOR3* _target) { movingTarget = _target; }


// [�f�o�b�O]
#pragma region Debug
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
		acceleration += moveDirection * MOVE_ACC[enemyData->type];
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

			if (height == -1 && width == -1) { eyeAngleRay[0].initialize(centralPosition, direction); }
			if (height == -1 && width == 1) { eyeAngleRay[1].initialize(centralPosition, direction); }
			if (height == 1 && width == -1) { eyeAngleRay[2].initialize(centralPosition, direction); }
			if (height == 1 && width == 1) { eyeAngleRay[3].initialize(centralPosition, direction); }
		}
	}

	// ���E���C�𔒂ŕ`��
	for (int i = 0; i < 4; i++)
	{
		eyeAngleRay[i].color = D3DXCOLOR(255, 255, 255, 155);
	}

	// ���o������`��
	// �o�E���f�B���O�X�t�B�A�ŕ`�悷��

	bool sound = false;
	if (canUseSensor)
	{
		if (eyeSensor(gameMasterNS::PLAYER_1P) || eyeSensor(gameMasterNS::PLAYER_2P))
		{
			// ���E�ɓ�������ԓ_��
			for (int i = 0; i < 4; i++)
			{
				eyeAngleRay[i].color = D3DXCOLOR(255, 0, 0, 255);
				sound = true;
			}
		}

		if (earSensor(gameMasterNS::PLAYER_1P) || earSensor(gameMasterNS::PLAYER_2P))
		{

			sound = true;
		}
	}

	if (sound)
	{
		// ����炷
		FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
		PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::SE_GAME_EnemyAttack, false ,NULL,false,NULL,true, filterParameters };
		SoundInterface::SE->playSound(&playParameters);	//SE�Đ�
	}
}

//===================================================================================================================================
// ImGUI�ւ̏o��
//===================================================================================================================================
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
