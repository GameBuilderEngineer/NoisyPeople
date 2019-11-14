//===================================================================================================================================
//�yPlayer.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/24
// [�X�V��]2019/11/05
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
	Object::initialize(&D3DXVECTOR3(0,10.0f,0));
	ZeroMemory(&keyTable, sizeof(OperationKeyTable));
	type				= ObjectType::PLAYER; 
	onGravity			= true;
	//state				= NORMAL;
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
	SAFE_DELETE(bulletManager);
	SAFE_DELETE(state);
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

	//�ʏ���
	state = new NormalState(this);

	//�V���[�e�B���O�A�N�V����
	bulletManager		= new BulletManager;

	//�f�W�^���A�N�V����
	//�f�W�^���V�t�g
	shiftLine.start = position;
	shiftLine.end	= position+axisZ.direction;
	shiftTimer		= 0.0f;
	isShifting		= false;


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
	acceleration *= 0.0f;
	state->update(frameTime);

	//�f�W�^���V�t�g�X�V
	if (shiftTimer > 0) {
		shiftTimer -= frameTime;
		float rate = 1.0f -(shiftTimer/SHIFT_TIME);
		D3DXVec3Lerp(&position, &shiftLine.start, &shiftLine.end,rate);
	}
	else if(isShifting){
		isShifting = false;
	}


	//switch (state)
	//{
	//case NORMAL:
	//	break;
	//
	//case VISION:
	//	break;
	//
	//case SKY_VISION:
	//	break;
	//
	//case SHIFT:
	//	break;
	//}

	//�o���b�g�}�l�[�W���[�̍X�V
	bulletManager->update(frameTime);

	// ����(��ԕ�)
	state->operation();

	//shot();						//�e�̔���
	//digitalShift();				//�f�W�^���V�t�g
	//moveOperation();			// �ړ�����
	//jumpOperation();			// �W�����v����

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

	state->physics();

	// �ȉ��̏��ԓ���ւ��֎~�i�Փ˂���ɕ���������j
	//grounding();				// �ڒn����
	//physicalBehavior();			// ��������
	//updatePhysics(frameTime);	// �����̍X�V

	//controlCamera(frameTime);	// �J��������


	//�Ə����C�̍X�V
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
	//�Ǝˈʒu�̎Z�o
	collideDistance = MAX_DISTANCE;
	aimingPosition = aimingRay.start + (aimingRay.direction * collideDistance);
	if (aimingRay.rayIntersect(attractorMesh, *attractorMatrix) && aimingRay.distance <= MAX_DISTANCE )
	{
		if (collideDistance > aimingRay.distance)
		{
			collideDistance = aimingRay.distance;
			aimingPosition = aimingRay.start + (aimingRay.direction* aimingRay.distance);//�Փˈʒu
		}
	}

	//�p������:�ˌ������֌���
	D3DXVECTOR3 front;
	Base::between2VectorDirection(&front, position, aimingPosition);	//�ˌ������x�N�g���̎Z�o
	front = slip(front, axisY.direction);								//Y�������ւ̐������폜����
	D3DXVec3Normalize(&front, &front);									//���K��
	postureControl(getAxisZ()->direction, front, 0.3f);
	
	// �I�u�W�F�N�g�̍X�V
	Object::update();

	//���R���W�����𒆐S���W�֕␳
	centralPosition = position + bodyCollide.getCenter();
	D3DXMatrixTranslation(&centralMatrixWorld, centralPosition.x, centralPosition.y, centralPosition.z);
	axisX.update(centralPosition, axisX.direction);
	axisY.update(centralPosition, axisY.direction);
	axisZ.update(centralPosition, axisZ.direction);
	reverseAxisX.update(centralPosition, -axisX.direction);
	reverseAxisY.update(centralPosition, -axisY.direction);
	reverseAxisZ.update(centralPosition, -axisZ.direction);

	//���ˈʒu�̍X�V
	launchPosition = centralPosition + axisZ.direction*radius;
	//�_�����C�̍X�V
	Base::between2VectorDirection(&shootingRay.direction, launchPosition, aimingPosition);
	shootingRay.update(launchPosition, shootingRay.direction);
	shootingRay.rayIntersect(attractorMesh, *attractorMatrix);
}

//===================================================================================================================================
//�y�{�̈ȊO�̑��̃I�u�W�F�N�g�`��z
//===================================================================================================================================
void Player::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#ifdef _DEBUG
	aimingRay.render(collideDistance);
	shootingRay.render(MAX_DISTANCE);
	bodyCollide.render(centralMatrixWorld);
	bulletManager->render(view,projection,cameraPosition);
	Object::debugRender();
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
	groundNormal = gravityRay.normal;

	if (hit == false)
	{// �v���C���[�͒n�ʂ̖����󒆂ɂ���
		onGround = false;
		return;
	}

	if (radius + difference >= gravityRay.distance)
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
	//�X�s�[�h
	ray.update(centralPosition, D3DXVECTOR3(speed.x,0, speed.z));
	// �ǂ��菈��
	if (ray.rayIntersect(attractorMesh, *attractorMatrix)&& radius/2 >= ray.distance)
	{
		position += ray.normal*(radius/2-ray.distance);		//�߂荞�ݕ␳
		speed = slip(speed, ray.normal);		// �ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
	}
}

//===================================================================================================================================
//�y�߂肱�ݕ␳�z
//===================================================================================================================================
void Player::insetCorrection()
{
	//Z���߂荞�ݕ␳
	ray.update(centralPosition, axisZ.direction);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		position += ray.normal*(radius / 2 - ray.distance);
	}
	//-Z���߂荞�ݕ␳
	ray.update(centralPosition, reverseAxisZ.direction);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		position += ray.normal*(radius / 2 - ray.distance);
	}
	//X���߂荞�ݕ␳
	ray.update(centralPosition, axisX.direction);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		position += ray.normal*(radius / 2 - ray.distance);
	}
	//-X���߂荞�ݕ␳
	ray.update(centralPosition, reverseAxisX.direction);
	if (ray.rayIntersect(attractorMesh, *attractorMatrix) && radius / 2 >= ray.distance)
	{
		position += ray.normal*(radius / 2 - ray.distance);
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
	}
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
	if (onGround) {
		//���C�������Z�o
		D3DXVECTOR3 frictionDirection;
		D3DXVec3Normalize(&frictionDirection,&speed);
		// ���x�ɖ��C�̉e����^����
		speed -= frictionDirection * MOVE_FRICTION*GRAVITY_FORCE * frameTime;
		//���x�ɍő呬�x�Ő�����������
		if (D3DXVec3Length(&speed) > MAX_SPEED)
		{
			speed = MAX_SPEED * frictionDirection;
		}
		wallScratch();					//�ǂ���
		position += speed* frameTime;	// ���x�̉e�����ʒu�ɗ^����
	}
	else {
		wallScratch();						//�ǂ���
		position += speed * frameTime;		// ���x�̉e�����ʒu�ɗ^����
	}
	insetCorrection();//�ړ���̂߂荞�ݕ␳
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
	if (input->getMouseRButtonTrigger() || input->getController()[infomation.playerType]->wasButton(BUTTON_JUMP))
	{
		if (jumping == false) onJump = true;	// �W�����v���؃t���O���I���ɂ���
	}
	if (input->getMouseRButton() || input->getController()[infomation.playerType]->isButton(BUTTON_JUMP))
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
	//�J�����̒����ʒu���X�V�i���Έʒu�ɂ����Z�j
	D3DXQUATERNION cameraGazeRelativeQ = camera->relativeQuaternion;
	Base::anyAxisRotation(&cameraGazeRelativeQ, D3DXVECTOR3(0, 1, 0), -90);
	D3DXVec3Normalize(&cameraGazeRelative,&(D3DXVECTOR3)cameraGazeRelativeQ);
	//�J���������ʒu�̍X�V
	cameraGaze = position
		+ cameraGazeRelative * CAMERA_GAZE.x
		+ axisY.direction*CAMERA_GAZE.y;
	//�J�����̍X�V
	camera->update();
	aimingRay.update(camera->position + camera->getDirectionZ(), camera->getDirectionZ());
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

//===================================================================================================================================
//�y�ˌ��z
//===================================================================================================================================
void Player::shot()
{
	if (!input->getMouseLButton() && 
		!input->getController()[infomation.playerType]->isButton(BUTTON_BULLET))return;
		bulletManager->launch(shootingRay);
}

//===================================================================================================================================
//�y�f�W�^���V�t�g�z
//===================================================================================================================================
void Player::digitalShift()
{
	if (!input->wasKeyPressed(keyTable.skyVision) &&
		!input->getController()[infomation.playerType]->isButton(BUTTON_SKY_VISION))return;
	if (isShifting)return;//�V�t�g���F�f�W�^���V�t�g���Ȃ�

	//�V�t�g���C���J��������̃��C�Ƃ��čX�V
	shiftRay.update(camera->position, camera->getDirectionZ());

	//�V�t�g���C���Փ˂��Ă�����ꍇ
	if (shiftRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		//�f�W�^���V�t�g�J�n
		shiftLine.start = position;
		shiftLine.end = shiftRay.start + shiftRay.direction*shiftRay.distance;
		isShifting = true;
		shiftTimer = SHIFT_TIME;
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
		ImGui::Text("BulletRmaining [%d/%d]",
			bulletManager->getRemaining(),bulletNS::MAGAZINE_NUM);						//�c�e��
		ImGui::Text("ReloadTime:%.02f", bulletManager->getReloadTime());				//�����[�h����

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
void Player::setCamera(Camera* _camera) { 
	camera = _camera; 
	aimingRay.initialize(camera->position, camera->getDirectionZ());
	aimingRay.color = D3DCOLOR_ARGB(255,253,193,0);
	shootingRay.color = D3DCOLOR_ARGB(255,190, 20, 20);
}
void Player::setInfomation(PlayerTable info) { infomation = info; };

void Player::addpower(int add)
{
	power = UtilityFunction::clamp( power + add, MIN_POWER, MAX_POWER);		//�d�͉�
}					
void Player::pullpower(int pull)
{
	power = UtilityFunction::clamp( power - pull, MIN_POWER, MAX_POWER);		//�d�͏���
}

void Player::damage(int _damage)
{
	hp -= _damage;
	if (hp < 0) hp = 0;
}


//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
int Player::getHp() { return hp; }
int Player::getPower() { return power; }
int Player::getState() { return state->type; }
bool Player::canShot() { return isShotAble; }
bool Player::canJump() { return isJumpAble; }
bool Player::canDoVision(){ return isVisionAble; }
bool Player::canDoSkyVision() { return isSkyVisionAble; }
bool Player::canShift() { return isShiftAble; }
BoundingSphere* Player::getBodyCollide() { return &bodyCollide; }
PlayerTable* Player::getInfomation() { return &infomation; }
D3DXVECTOR3* Player::getCameraGaze() { return &cameraGaze; }
D3DXVECTOR3* Player::getAiming() { return &aimingPosition; }
Bullet* Player::getBullet(int i) { return bulletManager->getBullet(i); }
int Player::getShootingNum() { return bulletManager->getNum(); }
D3DXVECTOR3* Player::getCentralPosition() { return &centralPosition; }
bool Player::getWhetherExecutingMoveOpe() { return isExecutingMoveOperation; }
bool Player::getOnGround() { return onGround; }
D3DXVECTOR3* Player::getGroundNormal() { return &groundNormal; }
D3DXMATRIX* Player::getcentralMatrixWorld() { return &centralMatrixWorld; }




//(��)�ʏ���

NormalState::NormalState(Player* player):AbstractState() 
{
	this->player = player;
	type = NORMAL;
}

void NormalState::start()
{

}

void NormalState::update(float frameTime)
{
	this->frameTime = frameTime;
}


void NormalState::operation()
{
	// ����
	player->shot();						//�e�̔���
	player->digitalShift();				//�f�W�^���V�t�g
	player->moveOperation();			// �ړ�����
	player->jumpOperation();			// �W�����v����
}

void NormalState::physics()
{
	player->grounding();				// �ڒn����
	player->physicalBehavior();			// ��������
	player->updatePhysics(frameTime);	// �����̍X�V
	player->controlCamera(frameTime);
}


AbstractState* NormalState::transition()
{
	return new NormalState(player);
}

void NormalState::end()
{

}

