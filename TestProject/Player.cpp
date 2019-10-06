//===================================================================================================================================
//�yPlayer.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/24
// [�X�V��]2019/10/04
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Player.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace playerNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Player::Player() :StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_STATIC_MESH))
{
	ZeroMemory(&keyTable, sizeof(OperationKeyTable));
	onGravity = true;
	radius = 5.0f;
	activation();
	state = GROUND;

	invincibleTimer = 0.0f;					//���G����
	onGround = false;						//�ڒn����
	reverseValueXAxis = CAMERA_SPEED;		//����w��
	reverseValueYAxis = CAMERA_SPEED;		//����x��
	onJump = false;							//�W�����v�t���O
	difference = DIFFERENCE_FIELD;			//�t�B�[���h�␳����
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Player::~Player()
{

}

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
		ImGui::Checkbox("onRender", &onRender);											//�`��L�����t���O
		ImGui::Checkbox("onLighting", &onLighting);										//���������t���O
		ImGui::Checkbox("onTransparent", &onTransparent);								//���߃t���O

		ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);						//���ߒl�̑���L���t���O
	}
#endif // _DEBUG
}


//===================================================================================================================================
//�y�������z
//===================================================================================================================================
//�v���C���[�^�C�v���Ƃɏ��������e��ύX
void Player::initialize(int playerType, int modelType) {
	device = getDevice();
	input = getInput();
	type = playerType;
	keyTable = KEY_TABLE_1P;
	StaticMeshObject::initialize(&(D3DXVECTOR3)START_POSITION);
	bodyCollide.initialize(&position, staticMesh->mesh);
	radius = bodyCollide.getRadius();
}

//===================================================================================================================================
//�y�X�V�z
//[�������e1]�ړ�����
//[�������e2]�W�����v����
//[�������e3]�d�͏���
//[�������e4]�ڒn����
//===================================================================================================================================
void Player::update(float frameTime)
{
	//�O����
	setSpeed(D3DXVECTOR3(0, 0, 0));	//���x�i�ړ��ʁj�̏�����
	onJump = false;					//�W�����v�t���O

	//�ړ�����
	moveOperation();

	//�W�����v
	if (input->wasKeyPressed(keyTable.jump) ||
		input->getController()[type]->wasButton(BUTTON_JUMP))
	{
		onJump = true;
	}

	//�ڒn����
	updateGround(frameTime,onJump);

	//�����x����
	if (D3DXVec3Length(&acceleration) > 0.05f)
	{//�����x���������ꍇ�A���Z���Ȃ�
		speed += acceleration;
	}

	//�ʒu�X�V
	position += speed*frameTime;
	
	//�p������
	postureControl(axisY.direction, -gravityRay.direction,3.0f * frameTime);
	
	//�I�u�W�F�N�g�F�X�V
	Object::update();

	//�J�����̑���
	controlCamera(frameTime);

}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
//======================
//�y�g�D�[�������_�[�z
//======================
void Player::toonRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition,
	LPD3DXEFFECT effect, LPDIRECT3DTEXTURE9 textureShade, LPDIRECT3DTEXTURE9 textureLine)
{
	//Object::toonRender(device,view,projection, cameraPosition,effect,textureShade,textureLine);
	// ���̃I�u�W�F�N�g�̕`��
	//otherRender(device,view,projection,cameraPosition);
}
//======================
//�y�ʏ�`��z
//======================
void Player::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	StaticMeshObject::render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH),view,projection, cameraPosition);
	//���̃I�u�W�F�N�g�̕`��
	//otherRender(device,view,projection,cameraPosition);
}
//======================
//�y�{�̈ȊO�̑��̃I�u�W�F�N�g�`��z
//======================
void Player::otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//�f�o�b�O���`��
#ifdef _DEBUG
	bodyCollide.render(matrixWorld);
#endif // _DEBUG
}

//===================================================================================================================================
//�y�ړ�����z
// [�������e1]���͂��ꂽ�Q�����x�N�g���ɉ����ăJ�������Ɋ�Â��A���x�։��Z�������s���B
//===================================================================================================================================
void Player::moveOperation()
{
	//�L�[�ɂ��ړ�
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

	//���Z�b�g
	if (input->wasKeyPressed(keyTable.reset))
	{
		reset();
	}
	//�R���g���[���X�e�B�b�N�ɂ��ړ�
	if (input->getController()[type]->checkConnect()) {
		move(input->getController()[type]->getLeftStick()*0.001f, camera->getDirectionX(), camera->getDirectionZ());
	}

}

//===================================================================================================================================
//�y�ړ��z
// [�������e1]���͂��ꂽ�Q�����x�N�g���ɉ����ăJ�������Ɋ�Â��A���x�։��Z�������s���B
// [�������e2]�ړ���̎p��������s���B
//===================================================================================================================================
void Player::move(D3DXVECTOR2 operationDirection,D3DXVECTOR3 cameraAxisX,D3DXVECTOR3 cameraAxisZ)
{
	if (operationDirection.x == 0 && operationDirection.y == 0)return;//���͒l��0�ȉ��Ȃ�Έړ����Ȃ�
	//Y�������ւ̐������폜����
	D3DXVECTOR3 front = slip(cameraAxisZ, axisY.direction);
	D3DXVECTOR3 right = slip(cameraAxisX, axisY.direction);
	D3DXVec3Normalize(&front, &front);//���K��
	D3DXVec3Normalize(&right, &right);//���K��

	//����������J������XZ�����ɏ��������ړ��x�N�g���֕ϊ�����
	D3DXVECTOR3 moveDirection = operationDirection.x*right + -operationDirection.y*front;
	if (onGround) {
		addSpeed(moveDirection*SPEED);
	}
	else {
		addSpeed(moveDirection*SPEED/10);
	}
	//�p������
	postureControl(getAxisZ()->direction, moveDirection, 0.1f);
}

//===================================================================================================================================
//�y�W�����v�z
// [�������e]�����x�։��Z�������s��
//===================================================================================================================================
void Player::jump()
{
	acceleration += axisY.direction * JUMP_FORCE+speed/0.9f;
	onGround = false;
}

//===================================================================================================================================
//�y���Z�b�g�z
//===================================================================================================================================
void Player::reset()
{
	position = START_POSITION;
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
//�y�n�ヂ�[�h �X�V�����z
//===================================================================================================================================
void Player::updateGround(float frameTime, bool _onJump)
{
	//===========
	//�y�ڒn�����z
	//===========
	//�d�͐����쐬
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0,-1,0);
	gravityRay.initialize(position, gravityDirection);								//�d�̓��C�̏�����
	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix)&&
		radius + difference >= gravityRay.distance )
	{
		onGround = true;
		onGravity = false;
		//�߂荞�ݕ␳
		//���݈ʒu+ �d�͕���*(�߂荞�݋���)
		setPosition(position + gravityRay.direction * (gravityRay.distance-radius));
		//�ړ��x�N�g���̃X���b�v�i�ʕ����ւ̃x�N�g�������̍폜�j
		setSpeed(slip(speed, gravityRay.normal));
		acceleration *= 0;
		if (_onJump)jump();//�W�����v
	}
	else {
		//��
		onGround = false;
		onGravity = true;
	}
	setGravity(gravityDirection, GRAVITY_FORCE*frameTime);//�d�͏���

}

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
	attractorMesh =_attractorMesh;
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
	if (input->getController()[type]->checkConnect()) {
		camera->rotation(D3DXVECTOR3(0, 1, 0), input->getController()[type]->getRightStick().x*0.1f*frameTime*reverseValueXAxis);
		camera->rotation(camera->getHorizontalAxis(), input->getController()[type]->getRightStick().y*0.1f*frameTime*reverseValueYAxis);
	}

	camera->setUpVector(axisY.direction);
	camera->update();
}

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void Player::setCamera(Camera* _camera) { camera = _camera; }

//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
int Player::getState() { return state; }
BoundingSphere* Player::getBodyCollide() { return &bodyCollide; }
