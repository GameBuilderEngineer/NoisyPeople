//===================================================================================================================================
//�yPlayer.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/24
// [�X�V��]2019/11/21
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Player.h"
#include "ImguiManager.h"
#include "UtilityFunction.h"
#include "MoveP.h"
#include "MoveP1.h"
#include "NormalState.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace playerNS;

//[��{����]
#pragma region Basic
//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Player::Player()
{
	{//�I�u�W�F�N�g�^�C�v�ƏՓˑΏۂ̎w��
		using namespace ObjectType;
		treeCell.type = PLAYER;
		treeCell.target = PLAYER | ENEMY | TREE;
	}

	ZeroMemory(&keyTable, sizeof(OperationKeyTable));
	onGravity = true;
	//�^�C�}�[
	invincibleTimer = 0.0f;					//���G����
	recoveryPowerTimer = 0.0f;					//�����d�͉񕜎���

	//�ėp
	reverseValueXAxis = CAMERA_SPEED;			//����w��
	reverseValueYAxis = -CAMERA_SPEED;		//����x��

	//����
	onGround = false;				//�ڒn����
	difference = DIFFERENCE_FIELD;		//�t�B�[���h�␳����

	//�J�����J��
	cameraTransitionTimer = 0.0f;
	cameraTransitionTime = 0.0f;
	nowCameraTransing = false;

	//��{�A�N�V����
	onJump = false;				//�W�����v�t���O

	groundDistance = 0.0f;

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Player::~Player()
{
	SAFE_DELETE(bulletManager);
	SAFE_DELETE(digitalShiftEffect);
	SAFE_DELETE(state);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
//�v���C���[�^�C�v���Ƃɏ��������e��ύX
void Player::initialize(PlayerTable info)
{
	device = getDevice();
	input = getInput();
	infomation = info;
	keyTable = KEY_TABLE[infomation.playerType];
	Object::initialize(&(D3DXVECTOR3)START_POSITION[infomation.playerType]);

	// �R���C�_�̏�����
	bodyCollide.initialize(&position, staticMeshNS::reference(infomation.modelType)->mesh);
	setRadius(bodyCollide.getRadius());	// ���b�V�����a���擾

	//�T�C�Y�̐ݒ�
	setSize(D3DXVECTOR3(0.5f, 1.7f, 0.5f));

	// HP�̐ݒ�
	hp = MAX_HP;

	//�d�͂̐ݒ�
	power = MAX_POWER;							//�L�����N�^�[�d�͊m�F�p

	//�ʏ���
	state = new normalNS::NormalState(this);
	enableOperation(ENABLE_CAMERA);

	//�V���[�e�B���O�A�N�V����
	bulletManager = new BulletManager;
	collideAxisX = false;

	//�f�W�^���A�N�V����
	//�f�W�^���V�t�g
	shiftLine.start		= position;
	shiftLine.end		= position+axisZ.direction;
	//�I�����C�g���Đ���Ԃɂ��Ă����B
	digitalShiftEffect	= new DigitalShiftEffect;
	playSelectLight();
	

	//�Đ��p�����[�^�̍쐬
	//memset(playParameters, 0, sizeof(playParameters));
	//FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	shiftStartSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_ShiftStart, false ,NULL,false,NULL };
	shiftFinishSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_ShiftFinish, false ,NULL,false,NULL };
	visionSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Vision, false ,NULL,false,NULL };
	visionStartSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_VisionStart, false ,NULL,false,NULL };
	visionFinishSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_VisionFinish, false ,NULL,false,NULL };
	skyVisionStartSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_SkyVisionStart, false ,NULL,false,NULL };
	skyVisionFinishSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_SkyVisionStart, false ,NULL,false,NULL };

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Player::update(float frameTime)
{
	// ���O����
	this->frameTime = frameTime;
	friction = 1.0f;
	isExecutingMoveOperation = false;
	onJump = false;
	acceleration *= 0.0f;
	onGroundBefore = onGround;


	//�����d�͉�
	recoveryPower();

	//��ԕʍX�V
	state->update(frameTime);

	//�o���b�g�}�l�[�W���[�̍X�V
	bulletManager->update(frameTime);

	//�f�W�^���V�t�g�G�t�F�N�g�̍X�V
	digitalShiftEffect->update(frameTime);

	// ����(��ԕ�)
	state->operation();

#ifdef _DEBUG
	if (input->isKeyDown('H'))
	{// �W�����v����H�Ŕ�ׂ܂�
		acceleration.y = 0;
		acceleration.x *= 2;
		acceleration.z *= 2;
		speed.y += 1.5f;
	}
	if (input->isKeyDown('J'))
	{// �����ړ�
		position += speed;
	}
	if (input->wasKeyPressed(keyTable.reset))
	{// ���Z�b�g
		reset();
	}
#endif // DEBUG

	//�����X�V(��ԕ�)
	state->physics();

	//�J��������i��ԕʁj
	state->controlCamera();

	// �I�u�W�F�N�g�̍X�V
	Object::update();

}

//===================================================================================================================================
//�y�{�̈ȊO�̑��̃I�u�W�F�N�g�`��z
//===================================================================================================================================
void Player::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//�o���b�g�̕`��
	bulletManager->render(view, projection, cameraPosition);
	//�f�W�^���V�t�g�G�t�F�N�g�̕`��
	digitalShiftEffect->render(view, projection, cameraPosition);

#ifdef _DEBUG
	aimingRay.render(collideDistance);
	shootingRay.render(MAX_DISTANCE);
	Object::debugRender();
#endif // _DEBUG
}

//===================================================================================================================================
//�y��ԑJ�ځz
//===================================================================================================================================
void Player::transState(int next)
{
	state->nextType = next;
	AbstractState* nextState = state->transition();
	SAFE_DELETE(state);
	state = nextState;
}
#pragma endregion

//[�Փ�]
#pragma region BodyCollision
//===================================================================================================================================
// �ڒn����
//===================================================================================================================================
void Player::grounding()
{
	if (onGround)
	{
		if (onJump)
		{

		}
		else {
			dot = D3DXVec3Dot(&gravityRay.normal, &D3DXVECTOR3(0, 1, 0));
			if (dot > MAX_SLOPE) {
				speed = slip(speed, gravityRay.normal);
			}
			else {
				speed = slip(speed, gravityRay.normal);
			}
		}
	}
}

//===================================================================================================================================
// �΃I�u�W�F�N�g�ڒn����
//===================================================================================================================================
bool Player::grounding(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	//�W�����v���A�ڒn�������Ȃ�
	if (onJump)
	{
		onGround = false;
		return false;
	}

	//�d�͕����A���C��ݒ�
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(center, gravityDirection);

	//���C�ɂ��Փˌ��m
	bool hit = gravityRay.rayIntersect(mesh, matrix);
	MOVEP *MoveP = GetMovePAdr();
	if (hit && infomation.playerType == gameMasterNS::PLAYER_1P) {
		MoveP->GroundDistance = gravityRay.distance + size.y / 2;
	}
	else if (infomation.playerType == gameMasterNS::PLAYER_1P)
	{
		MoveP->GroundDistance = 10.0f;
	}

	MOVEP1 *MoveP1 = GetMoveP1Adr();
	if (hit && infomation.playerType == gameMasterNS::PLAYER_2P) {
		MoveP1->GroundDistance = gravityRay.distance + size.y / 2;
	}
	else if (infomation.playerType == gameMasterNS::PLAYER_2P)
	{
		MoveP1->GroundDistance = 10.0f;
	}

	//���C�Փˎ��A���C�������A�v���C���[������菬�����ꍇ�H�����ݏ�ԂƂȂ��Ă���̂ŁA
	if (hit && size.y / 2 + difference >= gravityRay.distance)
	{
		//�ڒn��ԂƂ݂Ȃ��A
		onGround = true;
		//�H������ł��鋗�����A�ʒu��␳����
		position += gravityRay.direction*(gravityRay.distance - size.y / 2);
		return true;
	}

	onGround = false;
	return false;
}

//===================================================================================================================================
// �ǂ��菈��
//===================================================================================================================================
void Player::wallScratch()
{
	//�X�s�[�h
	//ray.update(center, D3DXVECTOR3(speed.x,0, speed.z));
	// �ǂ��菈��
	//if (ray.rayIntersect(attractorMesh, *attractorMatrix)&& radius/2 >= ray.distance)
	//{
	//	position += ray.normal*(radius/2-ray.distance);		//�߂荞�ݕ␳
	//	speed = slip(speed, ray.normal);		// �ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
	//}
}

//===================================================================================================================================
//�y�߂肱�ݕ␳�z
//===================================================================================================================================
bool Player::insetCorrection(int axisID, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	using namespace objectNS;
	switch (axisID)
	{
	case AXIS_X:	
		collideAxisX = insetCorrection(&axisX, distance, mesh, matrix);
		return collideAxisX; break;
	case AXIS_RX:	return insetCorrection(&reverseAxisX, distance, mesh, matrix); break;
	case AXIS_Y:	return insetCorrection(&axisY, distance, mesh, matrix); break;
	case AXIS_RY:	return insetCorrection(&reverseAxisY, distance, mesh, matrix); break;
	case AXIS_Z:	return insetCorrection(&axisZ, distance, mesh, matrix); break;
	case AXIS_RZ:	return insetCorrection(&reverseAxisZ, distance, mesh, matrix); break;
	}
	return false;
}
bool Player::insetCorrection(Ray* ray, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	Ray tmp = *ray;

	if (!tmp.rayIntersect(mesh, matrix))
	{
		ray->distance = 100000.0f;
		return false; 
	}

	ray->distance = tmp.distance;

	if (distance >= tmp.distance)
	{
		position += tmp.normal*(distance - tmp.distance);
	}
	return true;
}
#pragma endregion

//[����]
#pragma region PhysicsMovement

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
	// �󒆂ŏd�͂�������i�n��ł͏d�͂������Ȃ��j
	setGravity(gravityDirection, GRAVITY_FORCE);

	// �n�㖀�C�W��
	if (onGround)
	{
		friction *= MOVE_FRICTION;
	}

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

	MOVEP *MoveP = GetMovePAdr();
	MoveP->MoveY = speed.y;

	if (onGround) {
		//���C�������Z�o
		D3DXVECTOR3 frictionDirection;
		D3DXVec3Normalize(&frictionDirection,&speed);
		dot = D3DXVec3Dot(&gravityRay.normal, &D3DXVECTOR3(0, 1, 0));
		if (dot > MAX_SLOPE) {
			// ���x�ɖ��C�̉e����^����
			speed -= frictionDirection * MOVE_FRICTION * GRAVITY_FORCE * frameTime;
		}
		else {
			// ���x�ɖ��C�̉e����^����
			speed -= frictionDirection * MOVE_FRICTION * GRAVITY_FORCE  * frameTime;

			//�Ζʊ���x�N�g��
			D3DXVECTOR3 slipVector;
			D3DXVec3Normalize(&slipVector, &slip(D3DXVECTOR3(0, -1, 0), gravityRay.normal));
			if (D3DXVec3Length(&speed) < MAX_SLIP)
			{
				speed += slipVector * MOVE_ACC * 2 * frameTime;
			}
		}
	}

	D3DXVECTOR3 ySpeed = D3DXVECTOR3(0.0f, speed.y, 0.0f);
	D3DXVECTOR3 yAcceleration = D3DXVECTOR3(0.0f, acceleration.y, 0.0f);

	D3DXVECTOR3 xzSpeed = D3DXVECTOR3(speed.x, 0.0f, speed.z);
	D3DXVECTOR3 xzAcceleration = D3DXVECTOR3(acceleration.x, 0.0f, acceleration.z);

	//���x�ɍő呬�x�Ő�����������
	if (D3DXVec3Length(&xzSpeed) > MAX_SPEED)
	{
		speed -= xzAcceleration * frameTime;
	}

	position += speed * frameTime;		// ���x�̉e�����ʒu�ɗ^����

}
#pragma endregion

//[����]
#pragma region Operation
//===================================================================================================================================
//�y�ړ�����z
// [�������e]���͂��ꂽ�Q�����x�N�g���ɉ����ăJ�������Ɋ�Â��A���x�։��Z�������s���B
//===================================================================================================================================
void Player::moveOperation()
{

	MOVEP *MoveP = GetMovePAdr();
	MOVEP1 *MoveP1 = GetMoveP1Adr();

	if (!MoveP->IsDie || !MoveP1->IsDie)
	{

		//�ړ����삪����
		if (!whetherValidOperation(ENABLE_MOVE))return;

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

}

//===================================================================================================================================
//�y�W�����v����z
// [�������e]�i1�j�����͂ŃW�����v���؃t���O���I���@(2)�W�����v���X�V
//===================================================================================================================================
void Player::jumpOperation()
{
	MOVEP *MoveP = GetMovePAdr();
	MOVEP1 *MoveP1 = GetMoveP1Adr();

	//�W�����v���삪����
	if (!whetherValidOperation(ENABLE_JUMP))return;

	//���S��������󂯕t���Ȃ�
	switch (infomation.playerType)
	{
	case gameMasterNS::PLAYER_1P:		if (MoveP->IsDie)return;		break;
	case gameMasterNS::PLAYER_2P:		if (MoveP1->IsDie)return;		break;
	}

	//���͊m�F
	if ((input->getMouseRButtonTrigger() || input->getController()[infomation.playerType]->wasButton(BUTTON_JUMP)))
	{
		jump();
		PLAY_PARAMETERS jumpVoice = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::Voice_Man_Jump, false, NULL, false, NULL };
		SoundInterface::SE->playSound(&jumpVoice);	//SE�Đ�
	}

	//�ڒn�t���O�ؑ�
	switch (infomation.playerType)
	{
	case gameMasterNS::PLAYER_1P:	MoveP->IsGround		= onGround;	break;
	case gameMasterNS::PLAYER_2P:	MoveP1->IsGround	= onGround;	break;
	}

}

#pragma endregion

//[�J����]
#pragma region Camera
//===================================================================================================================================
//�y�J�����̑���/�X�V�z
//===================================================================================================================================
void Player::controlCamera(float frameTime)
{
	//����\���`�F�b�N
	if (!(validOperation & ENABLE_CAMERA))return;

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
	//�J�����̒����ʒu���X�V�i���Έʒu�ɂ����Z�j
	D3DXQUATERNION cameraGazeRelativeQ = camera->relativeQuaternion;
	Base::anyAxisRotation(&cameraGazeRelativeQ, D3DXVECTOR3(0, 1, 0), -90);
	D3DXVec3Normalize(&cameraGazeRelative, &(D3DXVECTOR3)cameraGazeRelativeQ);
	//�J���������ʒu�̍X�V
	cameraGaze = position
		+ axisY.direction*CAMERA_GAZE.y;
	if (collideAxisX && CAMERA_GAZE.x > axisX.distance)
	{
		cameraGaze += cameraGazeRelative * axisX.distance;
	}
	else {
		cameraGaze += cameraGazeRelative * CAMERA_GAZE.x;
	}


	//�J�����̍X�V
	camera->update();
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
}

//===================================================================================================================================
//�y�X�J�C�J�����̑���/�X�V�z
//===================================================================================================================================
void Player::controlSkyCamera()
{
	//����\���`�F�b�N
	if (!(validOperation & ENABLE_CAMERA))return;

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
	//�J�����̒����ʒu���X�V�i���Έʒu�ɂ����Z�j
	D3DXQUATERNION cameraGazeRelativeQ = camera->relativeQuaternion;
	Base::anyAxisRotation(&cameraGazeRelativeQ, D3DXVECTOR3(0, 1, 0), -90);
	D3DXVec3Normalize(&cameraGazeRelative, &(D3DXVECTOR3)cameraGazeRelativeQ);

	//�J���������ʒu�̍X�V
	cameraGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*SKY_HEIGH;

	//�J�����̍X�V
	camera->update();
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
}

//===================================================================================================================================
//�y�J������ʏ�̃v���C���[���_�֖߂��z
//===================================================================================================================================
void Player::returnTransitionCamera(float time)
{
	nextGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*CAMERA_GAZE.y;
	startTransitionCamera(time, cameraGaze, nextGaze);
}

//===================================================================================================================================
//�y�J�����J�ڊJ�n�z
//===================================================================================================================================
void Player::startTransitionCamera(float time, D3DXVECTOR3 before, D3DXVECTOR3 next)
{
	//�J�ڒ��ɂ���
	nowCameraTransing = true;

	//���O�̃J�����ʒu��ݒ肷��
	beforeGaze = before;

	//���̃J�����ʒu��ݒ肷��
	nextGaze = next;

	//�J�ڎ��Ԃ�ݒ�
	cameraTransitionTime = time;

	//�J�����̑��싖��؂�
	disableOperation(ENABLE_CAMERA);

	//�J�ڃ^�C�}�[��0�ŏ�����
	cameraTransitionTimer = 0.0f;
}

//===================================================================================================================================
//�y�J�����J�ځz
//===================================================================================================================================
void Player::transitionCamera()
{
	//�J�ڒ��łȂ���Ύ��s���Ȃ�
	if (!nowCameraTransing)return;

	//�J�����J�ڎ��ԍX�V
	if (cameraTransitionTime > cameraTransitionTimer)
	{
		cameraTransitionTimer += frameTime;
	}

	//�ݒ莞�� > �J�ڃ^�C�}�[
	if (cameraTransitionTime > cameraTransitionTimer)
	{
		float rate = cameraTransitionTimer / cameraTransitionTime;
		D3DXVec3Lerp(&cameraGaze, &beforeGaze, &nextGaze, rate);
	}
	else
	{
		//�J�����J�ڒ��t���O���I��
		nowCameraTransing = false;
		//�J�����ʒu��J�ڐ�֐ݒ�
		cameraGaze = nextGaze;
		//�J�ڂ��������Ă���̂ŁA�J�����̑��������
		enableOperation(ENABLE_CAMERA);
	}

}

#pragma endregion

//[�����A�N�V����]
#pragma region AutoAction
//===================================================================================================================================
//�y�d�͂̎����񕜁z
//===================================================================================================================================
void Player::recoveryPower()
{
	if (power >= FULL_POWER)return;
	//1�b��1�x��
	if (recoveryPowerTimer < 1.0f)
	{
		recoveryPowerTimer += frameTime;
		return;
	}

	recoveryPowerTimer = 0.0f;
	power = UtilityFunction::clamp(power + AUTO_RECOVERY_POWER, 0, FULL_POWER);
}
//===================================================================================================================================
//�y���X�|�[���z
//===================================================================================================================================
void Player::respawn()
{
	//position = START_POSITION[infomation.playerType];
	//speed = acceleration = D3DXVECTOR3(0, 0, 0);
	//quaternion = D3DXQUATERNION(0, 0, 0, 1);
	//axisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 0, 0));
	//axisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
	//axisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));
	//reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));
	//reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0));
	//reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));
	//Object::update();
}

#pragma endregion

//[�A�N�V����]
#pragma region Action
//===================================================================================================================================
//�y�ړ��z
// [�������e1]���͂��ꂽ�Q�����x�N�g���ɉ����ăJ�������Ɋ�Â��A���x�։��Z�������s���B
// [�������e2]�ړ���̎p��������s���B
//===================================================================================================================================
void Player::move(D3DXVECTOR2 operationDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ)
{
	if (operationDirection.x == 0 && operationDirection.y == 0)return;//���͒l��0�ȉ��Ȃ�Έړ����Ȃ�
	isExecutingMoveOperation = true;

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
		acceleration.x += moveDirection.x * MOVE_ACC_WHEN_NOT_GROUND;
		acceleration.z += moveDirection.z * MOVE_ACC_WHEN_NOT_GROUND;
	}
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
	if (onGround && !onJump)
	{
		onJump = true;	// �W�����v���؃t���O���I���ɂ���
		speed += axisY.direction * JUMP_SPEED;	// ������������
	}
}

//===================================================================================================================================
//�y�ˌ��z
//===================================================================================================================================
bool Player::shot()
{
	MOVEP *MoveP = GetMovePAdr();
	MOVEP1 *MoveP1 = GetMoveP1Adr();

	//���͊m�F
	if (!input->getMouseLButton() && !input->getController()[infomation.playerType]->isButton(BUTTON_BULLET))
	{
		return false;
	}

	//���S��������󂯕t���Ȃ�
	switch (infomation.playerType)
	{
	case gameMasterNS::PLAYER_1P:		if (MoveP->IsDie)	return false;	break;
	case gameMasterNS::PLAYER_2P:		if (MoveP1->IsDie)	return false;	break;
	}

	//�e�̔���
	if (bulletManager->launch(shootingRay))
	{
		//�G�t�F�N�g�̍Đ�
		bulletNS::Muzzle* muzzle = new bulletNS::Muzzle(&launchPosition, &matrixRotation);
		effekseerNS::play(0, muzzle);
		return true;
	}

	return false;
}

//===================================================================================================================================
//�y�f�W�^���V�t�g�z
//===================================================================================================================================
bool Player::digitalShift()
{
	//�V�t�g����s�\
	if (!whetherValidOperation(ENABLE_SHIFT))	return false;

	if (!input->getMouseLButton() &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_DIGITA_SHIFT))return false;
	if (power < COST_SHIFT)return false;


	//�d�͂̎g�p
	power -= COST_SHIFT;

	//���̑J�ڐ��ݒ�
	nextGaze = shiftLine.end
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*CAMERA_GAZE.y;

	//�J�����J�ڂ��J�n
	startTransitionCamera(SKY_TRANSITION_TIME, cameraGaze, nextGaze);

	//�f�W�^���V�t�g�̊J�n�G�t�F�N�g���Đ�
	digitalShiftEffect->play(DigitalShiftEffectNS::START_SHIFT, center);

	transState(DIGITAL_SHIFT);
	return true;

}

//===================================================================================================================================
//�y�V�t�g���C�i�{�����[�����C�j�Ƃ̏Փˏ����z
//===================================================================================================================================
bool Player::collideShiftRay(Cylinder target)
{
	//�V�t�g���C���J��������̃��C�Ƃ��čX�V
	shiftRay.update(camera->position, camera->getDirectionZ());

	//�J��������̔��a�t�����C��ݒ�
	Cylinder volumeRayFromCamera;
	volumeRayFromCamera.centerLine.start = camera->position;
	volumeRayFromCamera.centerLine.end = camera->position + camera->getDirectionZ()*10000.0f;
	volumeRayFromCamera.radius = 3.0f;

	//
	D3DXVECTOR3 nearest =
		Base::nearestPointOnLine(
			shiftRay.start,
			shiftRay.start + shiftRay.direction*10000.0f,
			target.centerLine.start);

	//�J��������̃��C��ł̋���
	float distanceOnRay = Base::between2VectorLength(nearest, camera->position);

	//�~���Ԃ̋���
	float distance = Base::between2LineDistance(target.centerLine, volumeRayFromCamera.centerLine);

	//�~�����m�̏Փˎ�����
	float radiusDistance = target.radius + volumeRayFromCamera.radius;

	if (distance <= radiusDistance)
	{
		D3DXVECTOR3 shiftDirection;
		//�f�W�^���V�t�g�L���łȂ��ꍇ�F�����V�t�g�ʒu�̐ݒ�
		if (!whetherValidOperation(ENABLE_SHIFT))
		{
			//�V�t�g��L���ɂ���
			enableOperation(ENABLE_SHIFT);
			//�f�W�^���V�t�g���s�����C����ݒ�
			shiftLine.start = this->position;
			shiftLine.end = target.centerLine.start;
			//�f�W�^���V�t�g�̕������Z�o
			Base::between2VectorDirection(&shiftDirection, shiftLine.start, shiftLine.end);//�����Z�o
			shiftDirection = slip(shiftDirection, D3DXVECTOR3(0, 1, 0));//Y�����폜
			D3DXVec3Normalize(&shiftDirection, &shiftDirection);//���K��
			//�V�t�g�ʒu�̌���
			shiftPosition = shiftLine.end;
			shiftPosition += target.height*D3DXVECTOR3(0, 1, 0);
			shiftPosition -= shiftDirection*target.radius;
			//�ŒZ�V�t�g�����X�V
			shiftDistance = distance + distanceOnRay;//���C�ԋ���+���C�㋗��
			return true;
		}
		//�ŒZ�V�t�g�����������Z���ꍇ�F�V�t�g�ʒu�̍Đݒ�
		else if (shiftDistance > distance + distanceOnRay)//���C�ԋ���+���C�㋗��
		{
			//�f�W�^���V�t�g���s�����C����ݒ�
			shiftLine.start = this->position;
			shiftLine.end = target.centerLine.start;
			//�f�W�^���V�t�g�̕������Z�o
			Base::between2VectorDirection(&shiftDirection, shiftLine.start, shiftLine.end);
			shiftDirection = slip(shiftDirection, D3DXVECTOR3(0, 1, 0));//Y�����폜
			D3DXVec3Normalize(&shiftDirection, &shiftDirection);//���K��
			//�V�t�g�ʒu�̌���
			shiftPosition = shiftLine.end;
			shiftPosition += target.height*D3DXVECTOR3(0, 1, 0);
			shiftPosition -= shiftDirection * target.radius;
			//�ŒZ�V�t�g�����X�V
			shiftDistance = distance + distanceOnRay;//���C�ԋ���+���C�㋗��
			return true;
		}
		return false;
	}

	//����
	//�~���Փ˂����Ă��Ȃ��ꍇ��A
	//�~���Փ˂��Ă����Ƃ��Ă��A�ŒZ�V�t�g��łȂ��@
	return false;

}

//===================================================================================================================================
//�y�V�t�g���C�i�{�����[�����C�j�Ƃ̏Փˏ����z
//===================================================================================================================================
void Player::collideShiftRay(D3DXVECTOR3 position)
{
	//�V�t�g���C���J��������̃��C�Ƃ��čX�V
	shiftRay.update(camera->position, camera->getDirectionZ());

	D3DXVECTOR3 nearest =
		Base::nearestPointOnLine(
			shiftRay.start,
			shiftRay.start + shiftRay.direction*10000.0f,
			position);


	float radiusDistance = Base::between2VectorLength(nearest, position);
	float distance = Base::between2VectorLength(nearest, camera->position);

	if (radiusDistance <= 3.0f)
	{
		//�f�W�^���V�t�g�L���łȂ��ꍇ
		if (!whetherValidOperation(ENABLE_SHIFT))
		{
			//�V�t�g��L���ɂ���
			enableOperation(ENABLE_SHIFT);
			//�f�W�^���V�t�g���s�����C����ݒ�
			shiftLine.start = this->position;
			shiftLine.end = position - shiftRay.direction*3.0f;
			shiftDistance = distance;
		}
		else if (shiftDistance > distance)
		{
			//�f�W�^���V�t�g���s�����C����ݒ�
			shiftLine.start = this->position;
			shiftLine.end = position - shiftRay.direction*3.0f;
			shiftDistance = distance;
		}
	}

}

//===================================================================================================================================
//�y�V�t�g���C�Ƃ̏Փˏ����z
//===================================================================================================================================
void Player::collideShiftRay(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	//�V�t�g���C���J��������̃��C�Ƃ��čX�V
	shiftRay.update(camera->position, camera->getDirectionZ());

	//�V�t�g���C�ƏՓ�&&�ŒZ�����X�V
	if (shiftRay.rayIntersect(mesh, matrix))
	{
		//�f�W�^���V�t�g�L���łȂ��ꍇ
		if (!whetherValidOperation(ENABLE_SHIFT))
		{
			//�V�t�g��L���ɂ���
			enableOperation(ENABLE_SHIFT);
			//�f�W�^���V�t�g���s�����C����ݒ�
			shiftLine.start = position;
			shiftLine.end = camera->position + shiftRay.direction*shiftRay.distance;
		}
		else if (shiftRay.distance < Base::between2VectorLength(shiftLine.start, shiftLine.end))
		{
			//�f�W�^���V�t�g���s�����C����ݒ�
			shiftLine.start = position;
			shiftLine.end = camera->position + shiftRay.direction*shiftRay.distance;
		}
	}

}

//===================================================================================================================================
//�y�f�W�^���V�t�g��I��\���G�t�F�N�g�Đ��z
//===================================================================================================================================
void Player::playSelectLight()
{
	//�I���G�t�F�N�g���C�g�\��
	digitalShiftEffect->playSelectLight(infomation.playerType+1,&shiftLine.end);
}

//===================================================================================================================================
//�y�f�W�^���V�t�g��I��\���G�t�F�N�g�Đ��z
//===================================================================================================================================
void Player::shownSelectLight(bool shown)
{
	//�I���G�t�F�N�g���C�g�\��
	digitalShiftEffect->shownSelectLigth(shown);
}

//===================================================================================================================================
//�y�f�W�^���V�t�g���s�z
//===================================================================================================================================
bool Player::executionDigitalShift()
{
	//�f�W�^���V�t�g�J�n
	position = shiftPosition;

	//�Z���^�[�������ɍX�V
	center = position + D3DXVECTOR3(0.0f, size.y / 2, 0.0f);

	//�f�W�^���V�t�g�̏I���G�t�F�N�g���Đ�
	digitalShiftEffect->play(DigitalShiftEffectNS::END_SHIFT, center);

	return true;
}

//===================================================================================================================================
//�y�X�J�C�r�W�������s�z
//===================================================================================================================================
bool Player::executionSkyVision()
{


	return true;
}

//===================================================================================================================================
//�y�r�W�������s�z
//===================================================================================================================================
bool Player::executionVision()
{


	return true;
}

//===================================================================================================================================
//�y�r�W�����z
//===================================================================================================================================
bool Player::vision()
{
	if (!input->wasKeyPressed(keyTable.vision) &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_VISION))return false;

	returnTransitionCamera(0.0f);

	transState(VISION);
	return true;
}

//===================================================================================================================================
//�y�r�W�����̃L�����Z���z
//===================================================================================================================================
bool Player::cancelVision()
{
	if (!input->wasKeyPressed(keyTable.vision) &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_VISION))return false;

	returnTransitionCamera(0.0f);

	transState(NORMAL);
	return true;
}

//===================================================================================================================================
//�y�X�J�C�r�W�����z
//===================================================================================================================================
bool Player::skyVision()
{
	//���͌��m
	if (!input->wasKeyPressed(keyTable.skyVision) &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_SKY_VISION))return false;

	//���̃J�����J�ڐ�Ƃ��ăX�J�C�J�����ʒu��ݒ肷��
	nextGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*SKY_HEIGH;

	//�J�����J�ڂ��J�n
	startTransitionCamera(SKY_TRANSITION_TIME, cameraGaze, nextGaze);

	//�X�J�C�r�W�����֑J��
	transState(SKY_VISION);

	return true;
}

//===================================================================================================================================
//�y�X�J�C�r�W�����̃L�����Z���z
//===================================================================================================================================
bool Player::cancelSkyVision()
{
	//���͌��m
	if (!input->wasKeyPressed(keyTable.skyVision) &&
		!input->getController()[infomation.playerType]->wasButton(BUTTON_SKY_VISION))return false;

	//���̃J�����ʒu��n��v���C���[�J�����ʒu�ɐݒ肷��
	nextGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction;

	//�J�����J�ڂ��J�n
	startTransitionCamera(SKY_RETURN_TIME, cameraGaze, nextGaze);

	//�ʏ��Ԃ֑J��
	transState(NORMAL);

	return true;
}

//===================================================================================================================================
//�y�ˌ��������X�V����z
//===================================================================================================================================
void Player::updateAiming(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	aimingRay.update(*camera->target + camera->getDirectionZ(), camera->getDirectionZ());
	//�Ǝˈʒu�̎Z�o
	collideDistance = MAX_DISTANCE;
	aimingPosition = aimingRay.start + (aimingRay.direction * collideDistance);
	if (aimingRay.rayIntersect(mesh, matrix) && aimingRay.distance <= MAX_DISTANCE)
	{
		if (collideDistance > aimingRay.distance)
		{
			collideDistance = aimingRay.distance;
			aimingPosition = aimingRay.start + (aimingRay.direction* aimingRay.distance);//�Փˈʒu
		}
	}

}

//===================================================================================================================================
//�y�ˌ������֎p����������z
//===================================================================================================================================
void Player::updatePostureByAiming()
{
	D3DXVECTOR3 front;
	Base::between2VectorDirection(&front, position, aimingPosition);	//�ˌ������x�N�g���̎Z�o
	front = slip(front, axisY.direction);								//Y�������ւ̐������폜����
	D3DXVec3Normalize(&front, &front);									//���K��
	postureControl(getAxisZ()->direction, front, 0.3f);
}

//===================================================================================================================================
//�y���ˈʒu�̍X�V�z
//===================================================================================================================================
void Player::updateShooting(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	MOVEP *MoveP = GetMovePAdr();

	//���ˈʒu�̍X�V
	launchPosition = center;// +axisZ.direction*radius;
	//�_�����C�̍X�V
	Base::between2VectorDirection(&shootingRay.direction, launchPosition, aimingPosition);
	shootingRay.update(launchPosition, shootingRay.direction);
	if (shootingRay.rayIntersect(mesh, matrix) &&
		shootingRay.distance < MAX_DISTANCE)
	{
		//�Փˎ��A�_�����C�̏Փˈʒu�Ƀ��e�B�N���ʒu��u��
		aimingPosition = shootingRay.start + (shootingRay.direction * shootingRay.distance);
	}
	else {
		//��Փ˂̏ꍇ�A�ő勗��������B
		shootingRay.distance = MAX_DISTANCE;
	}
}

#pragma endregion

//[�f�o�b�O]
#pragma region Debug
//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void Player::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("PlayerInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;

		ImGui::Text("state[%s]", state->stateName.c_str());


		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));

		//�d�͊֌W
		ImGui::Text("power %d", power);													//�d��
		ImGui::Text("recoveryPowerTimer:%.02f", recoveryPowerTimer);					//�d�͉񕜎���


		//�V���[�e�B���O�֌W
		ImGui::Text("bulletList->nodeNum [%d]", bulletManager->getNum());				//���ۂ̃o���b�g���X�g�̃m�[�h�̐�
		ImGui::Text("BulletRmaining [%d/%d]",
			bulletManager->getRemaining(), bulletNS::MAGAZINE_NUM);						//�c�e��
		ImGui::Text("ReloadTime:%.02f", bulletManager->getReloadTime());				//�����[�h����
		ImGui::Text("shootingRay.distance %.02f", shootingRay.distance);

		//�J�����J��
		ImGui::Text(nowCameraTransing ? "nowCameraTransing:ON" : "nowCameraTransing:OFF");
		ImGui::Text("CameraTransTime : %.02f / %.02f",
			cameraTransitionTimer, cameraTransitionTime);
		//�f�W�^���V�t�g���C
		ImGui::Text("[shiftRay] start(%.02f,%.02f,%.02f):distance(%.02f)",
			shiftRay.start.x, shiftRay.start.y, shiftRay.start.z,
			shiftRay.distance);
		ImGui::Text("[shiftLine] start(%.02f,%.02f,%.02f):end(%.02f,%.02f,%.02f)",
			shiftLine.start.x, shiftLine.start.y, shiftLine.start.z,
			shiftLine.end.x, shiftLine.end.y, shiftLine.end.z);
		ImGui::Text("[shiftDistance] %.02f", shiftDistance);

		//����L���t���O
		//�V�t�g
		ImGui::Text(whetherValidOperation(ENABLE_SHIFT) ? "ENABLE_SHIFT:ON":"ENABLE_SHIFT:OFF");
		ImGui::Text(whetherValidOperation(ENABLE_JUMP) ? "ENABLE_JUMP:ON" : "ENABLE_JUMPT:OFF");
		ImGui::Text(whetherValidOperation(ENABLE_VISION) ? "ENABLE_VISION:ON" : "ENABLE_VISION:OFF");
		ImGui::Text(whetherValidOperation(ENABLE_SKY_VISION) ? "ENABLE_SKY_VISION:ON" : "ENABLE_SKY_VISION:OFF");

		//�ڒn�֌W
		ImGui::Text(onGround ? "onGround:ON" : "onGround:OFF");							//�ڒn�t���O
		ImGui::Text("dot:%.02f", dot);

		//�I�u�W�F�N�g�p�����[�^
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

//[Setter]
#pragma region Setter
//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void Player::setCamera(Camera* _camera) {
	camera = _camera;
	aimingRay.initialize(camera->position, camera->getDirectionZ());
	aimingRay.color = D3DXCOLOR(255, 123, 193, 255);
	shootingRay.color = D3DXCOLOR(120, 10, 20, 255);
}
void Player::setInfomation(PlayerTable info) { infomation = info; };
void Player::addpower(int add)
{
	power = UtilityFunction::clamp(power + add, MIN_POWER, MAX_POWER);		//�d�͉�
}
void Player::pullpower(int pull)
{
	power = UtilityFunction::clamp(power - pull, MIN_POWER, MAX_POWER);		//�d�͏���
}
void Player::damage(int _damage)
{
	hp -= _damage;
	if (hp < 0) hp = 0;
}
void Player::setValidOperation(int value)
{
	validOperation = value;
}
void Player::enableOperation(int value)
{
	validOperation |= value;
}
void Player::disableOperation(int value)
{
	validOperation &= ~value;
}
#pragma endregion

//[Getter]
#pragma region Getter
//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
int Player::getHp() { return hp; }
int Player::getPower() { return power; }
int Player::getState() { return state->type; }
bool Player::whetherValidOperation(int operation) {
	if (validOperation & operation)	return true;
	return false;
}
bool Player::canShot() { return whetherValidOperation(ENABLE_SHOT); }
bool Player::canJump() { return whetherValidOperation(ENABLE_JUMP); }
bool Player::canDoVision() { return whetherValidOperation(ENABLE_VISION); }
bool Player::canDoSkyVision() { return whetherValidOperation(ENABLE_SKY_VISION); }
bool Player::canShift() { return whetherValidOperation(ENABLE_SHIFT); }
BoundingSphere* Player::getBodyCollide() { return &bodyCollide; }
PlayerTable* Player::getInfomation() { return &infomation; }
D3DXVECTOR3* Player::getCameraGaze() { return &cameraGaze; }
D3DXVECTOR3* Player::getAiming() { return &aimingPosition; }
Bullet* Player::getBullet(int i) { return bulletManager->getBullet(i); }
int Player::getShootingNum() { return bulletManager->getNum(); }
bool Player::getWhetherExecutingMoveOpe() { return isExecutingMoveOperation; }
bool Player::getOnGround() { return onGround; }
D3DXVECTOR3* Player::getGroundNormal() { return &groundNormal; }
LPD3DXMESH Player::getMesh() { return box->mesh; };
BulletManager*  Player::getBulletManager() { return bulletManager; }
#pragma endregion

