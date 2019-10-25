//-----------------------------------------------------------------------------
// �G�l�~�[���N���X [Enemy.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "Enemy.h"
#include "ImguiManager.h"
using namespace enemyNS;

int Enemy::numOfEnemy = 0;			// �G�l�~�[�̐�


//=============================================================================
// �R���X�g���N�^
//=============================================================================
Enemy::Enemy(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData)
{
	numOfEnemy++;							// �G�l�~�[�̐������Z
	enemyData = _enemyData;					// �G�l�~�[�f�[�^���Z�b�g
	difference = DIFFERENCE_FIELD;			// �K�v���v����

	onGravity = true;
	position = enemyData->position;
	axisZ.direction = enemyData->direction;
	sphereCollider.initialize(&position, _staticMesh->mesh);
	radius = sphereCollider.getRadius();
	friction = 1.0f;

	Object::initialize(&position);
	Object::axisZ.direction = axisZ.direction;
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Enemy::~Enemy()
{
	numOfEnemy--;
}


//=============================================================================
// �X�V����
//=============================================================================
void Enemy::update(float frameTime)
{
	// ���O����
	previousWork();

#ifdef _DEBUG
	//moveOperation();
	//controlCamera(frameTime);
#endif

	grounding();				// �ڒn����
	physicalBehavior();			// ��������
	updatePhysics(frameTime);	// �����̍X�V
	Object::update();			// �I�u�W�F�N�g�̍X�V

	postureControl(axisZ.direction, enemyData->defaultDirection, 1.0f * frameTime);

	// �G�l�~�[�f�[�^�̍X�V
	enemyData->position = position;
	enemyData->direction = axisZ.direction;
	if (enemyData->state == DEAD)
	{
		enemyData->isAlive = false;
	}
}


void Enemy::previousWork()
{
	friction = 1.0f;
	isGoingMoveOperation = false;
}


//=============================================================================
// ���������̍X�V
//=============================================================================
void Enemy::physicalBehavior()
{
	//------------
	// �����x����
	//------------
	// �d�͏���
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(position, gravityDirection);
	if (onGround == false)
	{
		setGravity(gravityDirection, GRAVITY_FORCE);
	}

	//// �ړ����͂��Ȃ��Ƃ��d�͈ȊO�̉����x��؂�
	//if (isGoingMoveOperation == false)
	//{
	//	acceleration.x = 0.0f;
	//	acceleration.z = 0.0f;
	//}

	// �󒆂ɕ����^�C�~���O�ŉ����x�̓��Z�b�g�����
	if (onGround == false && onGroundBefore)
	{
		acceleration *= 0.0f;
	}

	//----------
	// ���x����
	//----------
	// ���n����^�C�~���O�ő��x���ቺ����
	if (onGround && onGroundBefore == false)
	{
		friction *= 0.1f;
	}

	// �n�㖀�C�W��
	if (onGround)
	{
		friction *= MOVE_FRICTION;
	}

	//// ��~
	//if (speed.x < 0.1f && speed.x > -0.1f &&
	//	speed.y < 0.1f && speed.y > -0.1f &&
	//	speed.z < 0.1f && speed.z > -0.1f)
	//{
	//	speed *= 0.0f;
	//}

	//// �������x���E�̐ݒ�
	//if (speed.y > -54.0f)
	//{
	//	speed.y = -54.0f;
	//}
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
// �ڒn����
//=============================================================================
void Enemy::grounding()
{
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(position, gravityDirection);

	//if (oldFrameGroundPosition.y > position.y)
	//{
	//	//gravityRay.update(position, -gravityDirection);
	//	//if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	//	//{
	//	//	setPosition(position + gravityRay.direction * (gravityRay.distance + radius));
	//	//}

	//	setSpeed(D3DXVECTOR3(0.0f, 30.0f, 0.0f));
	//}

	onGroundBefore = onGround;
	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (radius + difference >= gravityRay.distance)
		{// �n��
			onGround = true;
			// �߂荞�ݕ␳�i���݈ʒu + �d�͕��� * �߂荞�݋����j
			setPosition(position + gravityRay.direction * (gravityRay.distance - radius));
			// �ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
			setSpeed(slip(speed, gravityRay.normal));			
		}
		else
		{// �󒆁i�n�ʂ�����ꏊ�ŋ󒆁j
			onGround = false;
		}
	}
	else
	{// �󒆁i�n�ʂ��Ȃ��j
		onGround = false;
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
	isGoingMoveOperation = true;

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
		acceleration = moveDirection * MOVE_ACC;
	}
	else
	{
		acceleration = moveDirection * MOVE_ACC / 10.0f;
	}

	//�p������
	postureControl(getAxisZ()->direction, moveDirection, 0.1f);
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
	camera->setUpVector(axisY.direction);
	camera->update();
}


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

	Object::initialize(&(D3DXVECTOR3)START_POSITION);
}


//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void Enemy::outputGUI()
{
#ifdef _DEBUG

	//ImGui::Text(sceneName.c_str());

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
#endif // _DEBUG
}


//=============================================================================
// Getter
//=============================================================================
int Enemy::getEnemyID() { return enemyData->enemyID; }
int Enemy::getNumOfEnemy(){ return numOfEnemy; }
EnemyData* Enemy::getEnemyData() { return enemyData; }


//=============================================================================
// Setter
//=============================================================================
void Enemy::setDataToEnemy(EnemyData* _enemyData){	enemyData = _enemyData;}
void Enemy::setCamera(Camera* _camera) { camera = _camera; }
void Enemy::resetNumOfEnemy() { numOfEnemy = 0; }
