//===================================================================================================================================
//�yPlayer.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/24
// [�X�V��]2019/10/17
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Player.h"
#include "ImguiManager.h"
#include "UtilityFunction.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace playerNS;


//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Player::Player() 
{
	Object::initialize(&D3DXVECTOR3(0,0,0));
	ZeroMemory(&keyTable, sizeof(OperationKeyTable));

	onGravity			= true;
	state				= NORMAL;
	invincibleTimer		= 0.0f;					//���G����
	onGround			= false;				//�ڒn����
	reverseValueXAxis	= CAMERA_SPEED;			//����w��
	reverseValueYAxis	= CAMERA_SPEED;			//����x��
	onJump				= false;				//�W�����v�t���O
	difference			= DIFFERENCE_FIELD;		//�t�B�[���h�␳����

	isShotAble			= true;
	isJumpAble			= true;
	isVisionAble		= true;
	isSkyVisionAble		= true;
	isShiftAble			= true;
}


//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Player::~Player()
{

}


//===================================================================================================================================
//�y�������z
//===================================================================================================================================
//�v���C���[�^�C�v���Ƃɏ��������e��ύX
void Player::initialize(PlayerTable info)
{
	device				= getDevice();
	input				= getInput();
	infomation			= info;
	keyTable			= KEY_TABLE[infomation.playerType];
	Object::initialize(&(D3DXVECTOR3)START_POSITION[infomation.playerType]);

	// �R���C�_�̏�����
	bodyCollide.initialize(	&position, staticMeshNS::reference(infomation.modelType)->mesh);
	radius				= bodyCollide.getRadius();				// ���b�V�����a���擾
	centralPosition		= position + bodyCollide.getCenter();	// ���S���W��ݒ�
	D3DXMatrixIdentity(&centralMatrixWorld);					// ���S���W���[���h�}�g���N�X��������
	power				= MAX_POWER;							//�L�����N�^�[�d�͊m�F�p
}


//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Player::update(float frameTime)
{
	// ���O����
	friction = 1.0f;
	isExecutingMoveOperation = false;
	onJump = false;
	centralPosition = position + bodyCollide.getCenter();
	acceleration *= 0.0f;


	switch (state)
	{
	case NORMAL:
		break;

	case VISION:
		break;

	case SKY_VISION:
		break;

	case SHIFT:
		break;
	}

	// ����
	moveOperation();			// �ړ�����
	jumpOperation();			// �W�����v����
#ifdef _DEBUG
	if (input->isKeyDown('H'))
	{// �W�����v����H�Ŕ�ׂ܂�
		acceleration.y = 0;
		acceleration.x *= 2;
		acceleration.z *= 2;
		speed.y += 1.5f;
	}
	if (input->wasKeyPressed(keyTable.reset))
	{// ���Z�b�g
		reset();
	}
#endif // DEBUG

	// �ȉ��̏��ԓ���ւ��֎~�i�Փ˂���ɕ���������j
	grounding();				// �ڒn����
	wallScratch();				// �ǂ��菈��
	physicalBehavior();			// ��������
	updatePhysics(frameTime);	// �����̍X�V
	controlCamera(frameTime);	// �J��������

	//// �J�����͉�邪�L�����N�^�[���A�����Ă���Ȃ�
	//�p������
	//if (onGround)
	//{
	//	postureControl(axisY.direction, groundNor, 3.0f * frameTime);
	//}
	
	Object::update();	// �I�u�W�F�N�g�̍X�V
	D3DXMatrixTranslation(&centralMatrixWorld, centralPosition.x, centralPosition.y, centralPosition.z);
	axisX.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	axisY.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	axisZ.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
	reverseAxisX.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	reverseAxisY.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	reverseAxisZ.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
}


//===================================================================================================================================
//�y�{�̈ȊO�̑��̃I�u�W�F�N�g�`��z
//===================================================================================================================================
void Player::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#ifdef _DEBUG
	bodyCollide.render(centralMatrixWorld);
#endif // _DEBUG
}


// [�Փ�]
#pragma region BodyCollision
//===================================================================================================================================
// �ڒn����
//===================================================================================================================================
void Player::grounding()
{
	onGroundBefore = onGround;
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(centralPosition, gravityDirection);
	bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);

	if (hit == false)
	{// �v���C���[�͒n�ʂ̖����󒆂ɂ���
		onGround = false;
		return;
	}

	if (radius /*+ difference*/ >= gravityRay.distance)
	{// �v���C���[�͒n��ɐڒn���Ă���
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
	{// �v���C���[�͒n�ʂ̂���󒆂ɂ���
		onGround = false;
	}
}


//===================================================================================================================================
// �ǂ��菈��
//===================================================================================================================================
void Player::wallScratch()
{
	//// �y�ʎ����Łi�߂荞�ݕ␳���ɔ����ɃK�^�K�^����j
	//ray.update(centralPosition, D3DXVECTOR3(speed.x, 0, speed.z));
	//if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	//{
	//	if (radius >= ray.distance)
	//	{
	//		// �߂荞�ݕ␳�i���݈ʒu + �i�s���� * �߂荞�݋����j
	//		setPosition(position + (-ray.normal)* /*ray.direction */ (ray.distance - radius));
	//		// �ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
	//		setSpeed(slip(speed, ray.normal));
	//		// ��R���C
	//		friction *= WALL_FRICTION;
	//	}
	//}

	bool hit = false;

	// �X�s�[�h�x�N�g������ǂ̖@�������擾����
	ray.update(centralPosition, D3DXVECTOR3(speed.x, 0, speed.z));
	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{
		ray.update(centralPosition, -ray.normal);
		// �ǂ܂ł̋������擾����
		hit = ray.rayIntersect(attractorMesh, *attractorMatrix);
	}

	// �ǂ��菈��
	if (hit)
	{
		if (radius >= ray.distance)
		{
			// �߂荞�ݕ␳�i���݈ʒu + �ǖ@������ * �߂荞�݋����j
			setPosition(position + (-ray.normal) * (ray.distance - radius));

			// �ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
			setSpeed(slip(speed, ray.normal));
		}
	}
}
#pragma endregion

// [����]
#pragma region PhysicsMovement
//===================================================================================================================================
//�y�d�͐ݒ�(���C)�z
//[���e]���C���g�p���ďd�͌��̃��b�V���̖@������肾���A���̖@�����d�͕����Ƃ���
//[����]
// D3DXVECTOR3* attractorPosition	�F���͔����n�_
// LPD3DXMESH _attractorMesh		�F�i���C�����p�j���͔������b�V��
// D3DXMATRIX _attractorMatrix		�F�i���C�����p�j���͔����s��
//[�ߒl]�Ȃ�
//===================================================================================================================================
void Player::configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	//���C������s�����߂ɕK�v�ȗv�f���Z�b�g
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	//�d�͐����쐬
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.initialize(position, gravityDirection);		//�d�̓��C�̏�����

	//���C����
	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{//�d�͐���Ƀ|���S�����Փ˂��Ă����ꍇ�A�|���S���@�����d�͕����Ƃ��A�p����@���ƈ�v������B
		//postureControl(axisY.direction, gravityRay.normal, 1.0f);
		setGravity(-gravityRay.normal, GRAVITY_FORCE);
	}
	else
	{//�Փ˃|���S�������݂��Ȃ��ꍇ�́A�d�͐������̂܂܏d�͕����Ƃ��A�p�����d�͐��ƈ�v������B
		//postureControl(axisY.direction, gravityDirection, 1.0f);
		setGravity(gravityDirection, GRAVITY_FORCE);
	}
}


//===================================================================================================================================
// ��������
//===================================================================================================================================
void Player::physicalBehavior()
{
	//------------
	// �����x����
	//------------
	// �d�͏���
 	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(position, gravityDirection);
	if (onGround == false)
	{
		// �󒆂ŏd�͂�������i�n��ł͏d�͂������Ȃ��j
		setGravity(gravityDirection, GRAVITY_FORCE);
	}

	//// �ړ����͂��Ȃ��Ƃ������x��؂�
	//if (isExecutingMoveOperation == false)
	//{
	//	acceleration.x = 0.0f;
	//	acceleration.z = 0.0f;
	//}

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

	//----------
	// ���x����
	//----------
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


//===================================================================================================================================
// �����̍X�V
//===================================================================================================================================
void Player::updatePhysics(float frameTime)
{
	// �����x�̉e���𑬓x�ɗ^����
	speed += acceleration * frameTime;
	// ���x�ɖ��C�̉e����^����
	speed *= friction;
	// ���x�̉e�����ʒu�ɗ^����
	position += speed * frameTime;
}
#pragma endregion

// [����]
#pragma region Operation
//===================================================================================================================================
//�y�ړ�����z
// [�������e]���͂��ꂽ�Q�����x�N�g���ɉ����ăJ�������Ɋ�Â��A���x�։��Z�������s���B
//===================================================================================================================================
void Player::moveOperation()
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

	//�R���g���[���X�e�B�b�N�ɂ��ړ�
	if (input->getController()[infomation.playerType]->checkConnect()) {
		move(input->getController()[infomation.playerType]->getLeftStick()*0.001f, camera->getDirectionX(), camera->getDirectionZ());
	}
}


//===================================================================================================================================
//�y�W�����v����z
// [�������e]�i1�j�����͂ŃW�����v���؃t���O���I���@(2)�W�����v���X�V
//===================================================================================================================================
void Player::jumpOperation()
{
	if (input->wasKeyPressed(keyTable.jump) || input->getController()[infomation.playerType]->wasButton(BUTTON_JUMP))
	{
		if (jumping == false) onJump = true;	// �W�����v���؃t���O���I���ɂ���
	}
	if (input->isKeyDown(keyTable.jump) || input->getController()[infomation.playerType]->isButton(BUTTON_JUMP))
	{
		jump();
	}
}


//===================================================================================================================================
//�y�J�����̑���/�X�V�z
//===================================================================================================================================
void Player::controlCamera(float frameTime)
{
	//���쎲���]����
	if (input->wasKeyPressed(keyTable.reverseCameraX))reverseValueXAxis *= -1;
	if (input->wasKeyPressed(keyTable.reverseCameraY))reverseValueYAxis *= -1;
	//�}�E�X����
	camera->rotation(D3DXVECTOR3(0, 1, 0), (float)(input->getMouseRawX() * reverseValueXAxis));
	camera->rotation(camera->getHorizontalAxis(), (float)(input->getMouseRawY() * reverseValueYAxis));
	//�R���g���[������
	if (input->getController()[infomation.playerType]->checkConnect()) {
		camera->rotation(D3DXVECTOR3(0, 1, 0), input->getController()[infomation.playerType]->getRightStick().x*0.1f*frameTime*reverseValueXAxis);
		camera->rotation(camera->getHorizontalAxis(), input->getController()[infomation.playerType]->getRightStick().y*0.1f*frameTime*reverseValueYAxis);
	}

	camera->setUpVector(axisY.direction);
	camera->update();
}
#pragma endregion

// [�A�N�V����]
#pragma region Action
//===================================================================================================================================
//�y�ړ��z
// [�������e1]���͂��ꂽ�Q�����x�N�g���ɉ����ăJ�������Ɋ�Â��A���x�։��Z�������s���B
// [�������e2]�ړ���̎p��������s���B
//===================================================================================================================================
void Player::move(D3DXVECTOR2 operationDirection,D3DXVECTOR3 cameraAxisX,D3DXVECTOR3 cameraAxisZ)
{
	isExecutingMoveOperation = true;

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
		acceleration = moveDirection * MOVE_ACC * dash();
	}
	else
	{
		//acceleration += moveDirection * MOVE_ACC_WHEN_NOT_GROUND;
		acceleration.x += moveDirection.x * MOVE_ACC_WHEN_NOT_GROUND;
		acceleration.z += moveDirection.z * MOVE_ACC_WHEN_NOT_GROUND;
	}
	//�p������
	postureControl(getAxisZ()->direction, moveDirection, 0.1f);
}


//===================================================================================================================================
//�y�_�b�V���z
// [�������e]���͂ɉ����ă_�b�V���{����Ԃ�
//===================================================================================================================================
float Player::dash()
{
	if (input->isKeyDown(keyTable.dash))
		return playerNS::DASH_MAGNIFICATION;
	return 1.0f;
}

//===================================================================================================================================
//�y�W�����v�z
//===================================================================================================================================
void Player::jump()
{
	if (onGround && onJump)
	{
		speed += axisY.direction * JUMP_SPEED;	// ������������
		jumping = true;
	}

	if (speed.y > 0)
	{
		acceleration.y += JUMP_CONTROL_SPEED;		// �㏸���͓��͌p���Ŕ򋗗����L�т�
	}
}
#pragma endregion

// [�f�o�b�O]
#pragma region Debug
//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void Player::outputGUI()
{
#ifdef _DEBUG

	//ImGui::Text(sceneName.c_str());

	if (ImGui::CollapsingHeader("PlayerInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;
		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));
		ImGui::Text("power %d", power);													//�d��

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//�ʒu
		ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
		ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);						//�X�P�[��
		ImGui::SliderFloat("radius", &radius, 0, limitTop);								//���a
		ImGui::SliderFloat("alpha", &alpha, 0, 255);									//���ߒl
		ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//���x
		ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//�����x
		ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);					//�d��

		ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
		ImGui::Checkbox("onActive", &onActive);											//�A�N�e�B�u���t���O
	}
#endif // _DEBUG
}

//===================================================================================================================================
//�y���Z�b�g�z
//===================================================================================================================================
void Player::reset()
{
	position = START_POSITION[infomation.playerType];
	speed = acceleration = D3DXVECTOR3(0, 0, 0);
	quaternion = D3DXQUATERNION(0, 0, 0, 1);
	axisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0));
	axisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
	axisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));
	reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));
	reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0));
	reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));
	Object::update();
}
#pragma endregion

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void Player::setCamera(Camera* _camera) { camera = _camera; }
void Player::setInfomation(PlayerTable info) { infomation = info; };

void Player::addpower(int add)
{
	power = UtilityFunction::clamp( power + add, MIN_POWER, MAX_POWER);		//�d�͉�
}					
void Player::pullpower(int pull)
{
	power = UtilityFunction::clamp( power - pull, MIN_POWER, MAX_POWER);		//�d�͏���
}
//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
int Player::getState() { return state; }
int Player::getHp() { return hp; }
int Player::getPower() { return power; }
bool Player::canShot() { return isShotAble; }
bool Player::canJump() { return isJumpAble; }
bool Player::canDoVision(){ return isVisionAble; }
bool Player::canDoSkyVision() { return isSkyVisionAble; }
bool Player::canShift() { return isShiftAble; }
BoundingSphere* Player::getBodyCollide() { return &bodyCollide; }
PlayerTable* Player::getInfomation() { return &infomation; }
