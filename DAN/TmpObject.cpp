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
	reverseValueXAxis = CAMERA_SPEED;		//����w��
	reverseValueYAxis = CAMERA_SPEED;		//����x��
	onFly = true;							//��ԃt���O
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

	bodyCollide.initialize(&position, staticMeshNS::reference(staticMeshNS::WOLF)->mesh);	// �R���C�_�̏�����
	staticMeshNo = staticMeshNS::WOLF;						// ���b�V����ID
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
	isExecutingMoveOperation = false;
	centralPosition = position + bodyCollide.getCenter();
	acceleration *= 0.0f;

	// ����
	moveOperation();			// �ړ�����
#ifdef _DEBUG
	if (input->isKeyDown(keyTable.fly))
	{
		acceleration.y = 0;
		acceleration.x *= 2;
		acceleration.z *= 2;
		speed.y += 20.0f;
	}

	if (input->isKeyDown(keyTable.down))
	{
		acceleration.y = 0;
		acceleration.x *= 2;
		acceleration.z *= 2;
		speed.y -= 20.0f;
	}

	if (input->isKeyDown(keyTable.spin))
	{//��]
		Object::anyAxisRotation(axisY.direction, 1);
	}

	if (input->wasKeyPressed(keyTable.reset))
	{// ���Z�b�g
		reset();
	}

	if (input->getMouseRButtonTrigger())
	{
		onFly = !onFly;

		
	}

#endif // DEBUG

	// �ȉ��̏��ԓ���ւ��֎~�i�Փ˂���ɕ���������j
	updatePhysics(frameTime);	// �����̍X�V
	controlCamera(frameTime);	// �J��������

	Object::update();	// �I�u�W�F�N�g�̍X�V
	D3DXMatrixTranslation(&centralMatrixWorld, centralPosition.x, centralPosition.y, centralPosition.z);
	axisX.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	axisY.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	axisZ.update(centralPosition, D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));
	reverseAxisX.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._11, centralMatrixWorld._12, centralMatrixWorld._13));
	reverseAxisY.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._21, centralMatrixWorld._22, centralMatrixWorld._23));
	reverseAxisZ.update(centralPosition, -D3DXVECTOR3(centralMatrixWorld._31, centralMatrixWorld._32, centralMatrixWorld._33));

	//yeah!!!
	PositionYeah = position;
	PositionYeah.y += 1.7f;

}

//===================================================================================================================================
//�y�{�̈ȊO�̑��̃I�u�W�F�N�g�`��z
//===================================================================================================================================
void TmpObject::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#ifdef _DEBUG
	//bodyCollide.render(centralMatrixWorld);
#endif // _DEBUG
}

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
	//�d�͐����쐬
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.initialize(position, gravityDirection);		//�d�̓��C�̏�����

}

//===================================================================================================================================
// �����̍X�V
//===================================================================================================================================
void TmpObject::updatePhysics(float frameTime)
{
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
	speed = D3DXVECTOR3(0, 0, 0);

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
// �΃I�u�W�F�N�g�ڒn����
//===================================================================================================================================
bool TmpObject::grounding(LPD3DXMESH mesh, D3DXMATRIX matrix)
{

	//�d�͕����A���C��ݒ�
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	gravityRay.update(center, gravityDirection);

	//���C�ɂ��Փˌ��m
	bool hit = gravityRay.rayIntersect(mesh, matrix);

	//���C�Փˎ��A���C�������A�v���C���[������菬�����ꍇ�H�����ݏ�ԂƂȂ��Ă���̂ŁA
	if (hit)
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
//�y�J�����̑���/�X�V�z
//===================================================================================================================================
void TmpObject::controlCamera(float frameTime)
{
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
	speed = moveDirection* MOVE_ACC;

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
	staticMeshNo = meshId;
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
