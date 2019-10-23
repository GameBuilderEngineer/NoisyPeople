//===================================================================================================================================
//�yTmpObject.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/24
// [�X�V��]2019/10/18
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "TmpObject.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace tmpObjNS;


//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TmpObject::TmpObject()
{
	Object::initialize(&(D3DXVECTOR3)tmpObjNS::START_POSITION);
	ZeroMemory(&keyTable, sizeof(OperationKeyTable));

	onGravity = false;
	activation();
	onGround = false;						//�ڒn����
	reverseValueXAxis = CAMERA_SPEED;		//����w��
	reverseValueYAxis = CAMERA_SPEED;		//����x��
	onJump = false;							//�W�����v�t���O
	difference = DIFFERENCE_FIELD;			//�t�B�[���h�␳����
}


//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TmpObject::~TmpObject()
{

}


//===================================================================================================================================
//�y�������z
//===================================================================================================================================
//���I�u�W�F�N�g�^�C�v���Ƃɏ��������e��ύX
void TmpObject::initialize(int tmpObjectType, int modelType)
{
	device = getDevice();
	input = getInput();
	type = tmpObjectType;
	keyTable = KEY_TABLE_1P;
	Object::initialize(&(D3DXVECTOR3)START_POSITION);

	bodyCollide.initialize(&position, staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2)->mesh);	// �R���C�_�̏�����
	ItemListboxMesh = 0;									// ���b�V���̎��(���X�g�{�b�N�X)
	staticMeshNo = staticMeshNS::YAMADA_ROBOT2;				// ���b�V����ID
	radius = bodyCollide.getRadius();						// ���b�V�����a���擾
	centralPosition = position + bodyCollide.getCenter();	// ���S���W��ݒ�
	D3DXMatrixIdentity(&centralMatrixWorld);				// ���S���W���[���h�}�g���N�X��������
}


//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void TmpObject::update(float frameTime)
{
	// ���O����
	friction = 1.0f;
	isExecutingMoveOperation = false;
	onJump = false;
	centralPosition = position + bodyCollide.getCenter();
	acceleration *= 0.0f;

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
//�y�`��z
//===================================================================================================================================
//void TmpObject::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
//{
//	Object::render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH),view,projection, cameraPosition);
//}


//===================================================================================================================================
//�y�{�̈ȊO�̑��̃I�u�W�F�N�g�`��z
//===================================================================================================================================
void TmpObject::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#ifdef _DEBUG
	//bodyCollide.render(centralMatrixWorld);
#endif // _DEBUG
}


// [�Փ�]
#pragma region BodyCollision
//===================================================================================================================================
// �ڒn����
//===================================================================================================================================
void TmpObject::grounding()
{
	onGroundBefore = onGround;
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(centralPosition, gravityDirection);
	bool hit = gravityRay.rayIntersect(attractorMesh, *attractorMatrix);

	if (hit == false)
	{// ���I�u�W�F�N�g�͒n�ʂ̖����󒆂ɂ���
		onGround = false;
		return;
	}

	if (radius /*+ difference*/ >= gravityRay.distance)
	{// ���I�u�W�F�N�g�͒n��ɐڒn���Ă���
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
	{// ���I�u�W�F�N�g�͒n�ʂ̂���󒆂ɂ���
		onGround = false;
	}
}


//===================================================================================================================================
// �ǂ��菈��
//===================================================================================================================================
void TmpObject::wallScratch()
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
void TmpObject::configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
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

//
////===================================================================================================================================
//// ��������
////===================================================================================================================================
//void TmpObject::physicalBehavior()
//{
//	//------------
//	// �����x����
//	//------------
//	// �d�͏���
//	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
//	gravityRay.update(position, gravityDirection);
//	if (onGround == false)
//	{
//		// �󒆂ŏd�͂�������i�n��ł͏d�͂������Ȃ��j
//		setGravity(gravityDirection, GRAVITY_FORCE);
//	}
//
//	//// �ړ����͂��Ȃ��Ƃ������x��؂�
//	//if (isExecutingMoveOperation == false)
//	//{
//	//	acceleration.x = 0.0f;
//	//	acceleration.z = 0.0f;
//	//}
//
//	//if (onGround)
//	//{
//	//	// �n��ɂ���ꍇ�͏d�͕������؂�
//	//	// ��������O���ƍŌ�ɉ����x.y�ɓ����Ă����d�͉����x���X���b�v���č⓹���葱����
//	//	acceleration.y = 0.0f;
//	//}
//
//
//	// �󒆂ɕ����^�C�~���O�ŉ����x�؂�
//	if (onGround == false && onGroundBefore)
//	{
//		acceleration *= 0.0f;
//	}
//
//	//----------
//	// ���x����
//	//----------
//	// ���n����^�C�~���O�ő��x���ቺ����
//	if (onGround && onGroundBefore == false)
//	{
//		friction *= GROUND_FRICTION;
//	}
//
//	// �n�㖀�C�W��
//	if (onGround)
//	{
//		friction *= MOVE_FRICTION;
//	}
//
//	//// ��~
//	//float speedPerSecond = D3DXVec3Length(&speed);
//	//if (isExecutingMoveOperation == false &&
//	//	speedPerSecond < STOP_SPEED)
//	//{
//	//	speed *= 0.0f;
//	//}
//
//	// �������x���E�̐ݒ�
//	if (speed.y < -FALL_SPEED_MAX)
//	{
//		speed.y = -FALL_SPEED_MAX;
//	}
//}


//===================================================================================================================================
// �����̍X�V
//===================================================================================================================================
void TmpObject::updatePhysics(float frameTime)
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
void TmpObject::moveOperation()
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
	if (input->getController()[type]->checkConnect()) {
		move(input->getController()[type]->getLeftStick()*0.001f, camera->getDirectionX(), camera->getDirectionZ());
	}
}


//===================================================================================================================================
//�y�W�����v����z
// [�������e]�i1�j�����͂ŃW�����v���؃t���O���I���@(2)�W�����v���X�V
//===================================================================================================================================
void TmpObject::jumpOperation()
{
	if (input->wasKeyPressed(keyTable.jump) || input->getController()[type]->wasButton(BUTTON_JUMP))
	{
		if (jumping == false) onJump = true;	// �W�����v���؃t���O���I���ɂ���
	}
	if (input->isKeyDown(keyTable.jump) || input->getController()[type]->isButton(BUTTON_JUMP))
	{
		jump();
	}
}


//===================================================================================================================================
//�y�J�����̑���/�X�V�z
//===================================================================================================================================
void TmpObject::controlCamera(float frameTime)
{
	//���쎲���]����
	if (input->wasKeyPressed(keyTable.reverseCameraX))reverseValueXAxis *= -1;
	if (input->wasKeyPressed(keyTable.reverseCameraY))reverseValueYAxis *= -1;
	//�}�E�X����
	camera->rotation(D3DXVECTOR3(0, 1, 0), (float)(input->getMouseRawX() * reverseValueXAxis));
	camera->rotation(camera->getHorizontalAxis(), (float)(input->getMouseRawY() * reverseValueYAxis));
	//�R���g���[������
	if (input->getController()[type]->checkConnect()) {
		camera->rotation(D3DXVECTOR3(0, 1, 0), input->getController()[type]->getRightStick().x*0.1f*frameTime*reverseValueXAxis);
		camera->rotation(camera->getHorizontalAxis(), input->getController()[type]->getRightStick().y*0.1f*frameTime*reverseValueYAxis);
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
void TmpObject::move(D3DXVECTOR2 operationDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ)
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
float TmpObject::dash()
{
	if (input->isKeyDown(keyTable.dash))
		return tmpObjNS::DASH_MAGNIFICATION;
	return 1.0f;
}

//===================================================================================================================================
//�y�W�����v�z
//===================================================================================================================================
void TmpObject::jump()
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
void TmpObject::outputGUI()
{
#ifdef _DEBUG

	//ImGui::Text(sceneName.c_str());

	if (ImGui::CollapsingHeader("TmpObjectInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;
		int oldMeshId = staticMeshNo;

		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));


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

			//�G�l�~�[�̏��
		const char* listboxMesh[] = { "PLAYER", "WOLF", "TIGER","BEAR" ,"BATTERY" };
		ImGui::ListBox("Mesh", &ItemListboxMesh, listboxMesh, TMPOBJ_LIST::TMPOBJ_MAX);
		switch (ItemListboxMesh)
		{
		case TMPOBJ_LIST::TMPOBJ_PLAYER:
			staticMeshNo = staticMeshNS::YAMADA_ROBOT2;
			break;
		case TMPOBJ_LIST::TMPOBJ_WOLF:
			staticMeshNo = staticMeshNS::SAMPLE_REDBULL;
			break;
		case TMPOBJ_LIST::TMPOBJ_TIGER:
			staticMeshNo = staticMeshNS::SAMPLE_BUNNY;
			break;
		case TMPOBJ_LIST::TMPOBJ_BEAR:
			staticMeshNo = staticMeshNS::SAMPLE_HAT;
			break;
		case TMPOBJ_LIST::TMPOBJ_BATTERY:
			staticMeshNo = staticMeshNS::SAMPLE_SCISSORS;
			break;
		default:
			break;
		}
		if (oldMeshId != staticMeshNo)
		{
			resetMesh(staticMeshNo);
			oldMeshId = staticMeshNo;
		}
	}
#endif // _DEBUG
}

//===================================================================================================================================
//�y���Z�b�g�z
//===================================================================================================================================
void TmpObject::reset()
{
	position = START_POSITION;
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

//===================================================================================================================================
//�y���Z�b�g���b�V���z
//===================================================================================================================================
void TmpObject::resetMesh(int meshId)
{
	bodyCollide.~BoundingSphere();
	bodyCollide.initialize(&position, staticMeshNS::reference(meshId)->mesh);	// �R���C�_�̏�����
	radius = bodyCollide.getRadius();											// ���b�V�����a���擾
	centralPosition = position + bodyCollide.getCenter();						// ���S���W��ݒ�
}

#pragma endregion

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void TmpObject::setCamera(Camera* _camera) { camera = _camera; }


//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
int TmpObject::getState() { return state; }
BoundingSphere* TmpObject::getBodyCollide() { return &bodyCollide; }
