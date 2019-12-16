//===================================================================================================================================
//�yObject.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/12/05
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Object.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace objectNS;

//===================================================================================================================================
//�y�X�^�e�B�b�N�ϐ��z
//===================================================================================================================================
int Object::objectCounter = 0;	//ID�̊����Ɏg�p

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Object::Object()
{
	id					= objectCounter;									//ID�̊���
	objectCounter++;														//�I�u�W�F�N�g�J�E���^�[�̉��Z�FID�̊����Ɏg�p
	treeCell.type		= ObjectType::NONE;									//�I�u�W�F�N�g�^�C�v�F�����lNONE
	treeCell.target		= ObjectType::NONE;									//�I�u�W�F�N�g�^�C�v�F�����lNONE

	position			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ʒu
	quaternion			= D3DXQUATERNION(0, 0, 0, 1);						//��]
	scale				= D3DXVECTOR3(1.0f,1.0f,1.0f);						//�X�P�[��
	radius				= 5.0f;												//���a
	size				= D3DXVECTOR3(1.0f,1.0f,1.0f);						//�T�C�Y�̐ݒ�
	alpha				= 1.0f;												//���l�̐ݒ�
	speed				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//���x
	color				= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	onGravity			= false;											//�d�̓t���O
	onActive			= true;												//�A�N�e�B�u�t���O

	axisX.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(1, 0, 0));			//x��
	axisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));			//y��
	axisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));			//z��
	reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));	//-x��
	reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0));	//-y��
	reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));	//-z��
	sphere = new BoundingSphere(&center, radius);
	box					= new DebugBox(size);
#ifdef _DEBUG
	axisX.color			= D3DXCOLOR(255, 0, 0, 255);						//x���J���[
	axisY.color			= D3DXCOLOR(0, 255, 0, 255);						//y���J���[
	axisZ.color			= D3DXCOLOR(0, 0, 255, 255);						//z���J���[
	reverseAxisX.color	= D3DXCOLOR(255, 0, 0, 255);						//-x���J���[
	reverseAxisY.color	= D3DXCOLOR(0, 255, 0, 255);						//-y���J���[
	reverseAxisZ.color	= D3DXCOLOR(0, 0, 255, 255);						//-z���J���[
#endif // _DEBUG

	D3DXMatrixIdentity(&matrixPosition);									//�ʒu�s��
	D3DXMatrixIdentity(&matrixRotation);									//��]�s��
	D3DXMatrixIdentity(&matrixScale);										//�X�P�[���s��
	D3DXMatrixIdentity(&matrixCenter);										//�Z���^�[�s��
	D3DXMatrixIdentity(&matrixWorld);										//���[���h

	existenceTimer = 1.0f;													//���ݎ���

	treeCell.object = this;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Object::~Object()
{
	treeCell.remove();//���X�g����O���
	SAFE_DELETE(box);
	SAFE_DELETE(sphere);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
HRESULT Object::initialize(D3DXVECTOR3* _position)
{
	//�����ʒu
	memcpy(position, _position, sizeof(D3DXVECTOR3));
	activation();
	//���O�X�V
	update();
	return S_OK;
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Object::update()
{
	if (onActive == false)return;
	//�ʒu�s��̍쐬�i�ʒu���W���ʒu�s��ւ̕ϊ��j	
	D3DXMatrixTranslation(&matrixPosition, position.x, position.y, position.z);

	//��]�s��̍쐬�i�N�H�[�^�j�I������]�s��ւ̕ϊ��j
	D3DXMatrixRotationQuaternion(&matrixRotation, &quaternion);
	
	//�X�P�[���s��̍쐬�i�X�P�[���l���X�P�[���s��ւ̕ϊ��j
	D3DXMatrixScaling(&matrixScale, scale.x, scale.y, scale.z);

	//���[���h�s�񁁃X�P�[���s��*��]�s��*�ʒu�s��
	D3DXMatrixIdentity(&matrixWorld);									//���K��
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixScale);		//*�X�P�[���s��
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixRotation);		//*��]�s��
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixPosition);		//*�ʒu�s��

	//���[���h���W���玩�g�̎����C���X�V����
	center = position + D3DXVECTOR3(0.0f,size.y/2,0.0f);
	//�Z���^�[�s��̍쐬�i�Z���^�[���W���Z���^�[�s��ւ̕ϊ��j
	D3DXMatrixTranslation(&matrixCenter, center.x, center.y, center.z);
	axisX.update(center, D3DXVECTOR3(matrixWorld._11,matrixWorld._12,matrixWorld._13));
	axisY.update(center, D3DXVECTOR3(matrixWorld._21,matrixWorld._22,matrixWorld._23));
	axisZ.update(center, D3DXVECTOR3(matrixWorld._31,matrixWorld._32,matrixWorld._33));
	reverseAxisX.update(center, -D3DXVECTOR3(matrixWorld._11, matrixWorld._12, matrixWorld._13));
	reverseAxisY.update(center, -D3DXVECTOR3(matrixWorld._21, matrixWorld._22, matrixWorld._23));
	reverseAxisZ.update(center, -D3DXVECTOR3(matrixWorld._31, matrixWorld._32, matrixWorld._33));
}

//===================================================================================================================================
//�y�f�o�b�O�p�`��z
//===================================================================================================================================
void Object::debugRender()
{
#ifdef _DEBUG
	axisX.render(10.0f);
	axisY.render(10.0f);
	axisZ.render(10.0f);
	reverseAxisX.render(10.0f);
	reverseAxisY.render(10.0f);
	reverseAxisZ.render(10.0f);
	box->render(matrixCenter);
	sphere->render();
#endif // _DEBUG
}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void Object::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("ObjectInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop		= 1000;
		float limitBottom	= -1000;

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//�ʒu
		ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
		ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);							//�X�P�[��
		ImGui::SliderFloat("radius", &radius, 0, limitTop);										//���a
		ImGui::SliderFloat("alpha", &alpha, 0, 255);												//���ߒl
		ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//���x
		ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//�����x
		ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);					//�d��
		
		ImGui::Checkbox("onGravity", &onGravity);												//�d�͗L�����t���O
		ImGui::Checkbox("onActive", &onActive);												//�A�N�e�B�u���t���O
		
	}
#endif // _DEBUG
}

//===================================================================================================================================
//�y�d�͂�ݒ肵�A�����x�։��Z����z
//===================================================================================================================================
void Object::setGravity(D3DXVECTOR3 source, float power)
{
	D3DXVec3Normalize(&gravity, &source);
	//gravity *= min(power, reverseAxisY.distance);
	gravity *= power;
	
	if (onGravity)acceleration += gravity;
}


//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
D3DXMATRIX*		Object::getMatrixWorld() { return &matrixWorld; }
D3DXVECTOR3*	Object::getPosition() { return &position; };
D3DXQUATERNION	Object::getQuaternion() { return quaternion; };
float			Object::getRadius() { return radius; }
D3DXVECTOR3		Object::getSpeed() { return speed; }
D3DXVECTOR3		Object::getAcceleration() { return acceleration; }
D3DXVECTOR3		Object::getGravity() { return gravity; };
Ray*			Object::getAxisX() { return &axisX; };
Ray*			Object::getAxisY() { return &axisY; };
Ray*			Object::getAxisZ() { return &axisZ; };
Ray*			Object::getReverseAxisX() { return &reverseAxisX; };
Ray*			Object::getReverseAxisY() { return &reverseAxisY; };
Ray*			Object::getReverseAxisZ() { return &reverseAxisZ; };
Ray*			Object::getGravityRay() { return &gravityRay; };
bool			Object::getActive() { return onActive; }
float			Object::getRight() { return position.x + radius; }
float			Object::getLeft() { return position.x - radius; }
float			Object::getTop() { return position.z + radius; }
float			Object::getBottom() { return position.z - radius; }
float			Object::getFront() { return position.z + radius; }
float			Object::getBack() { return position.z - radius; }
D3DXVECTOR3		Object::getMin() { return position - (size / 2); }
D3DXVECTOR3		Object::getMax() { return position + (size / 2); }

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void Object::setSpeed(D3DXVECTOR3 _speed) { speed = _speed; }
void Object::addSpeed(D3DXVECTOR3 add) { speed += add; }
void Object::setPosition(D3DXVECTOR3 _position) { position = _position; }
void Object::setQuaternion(D3DXQUATERNION _quaternion) { quaternion = _quaternion; }
void Object::activation() { onActive = true; }
void Object::inActivation() { onActive = false; }
void Object::setAlpha(float value) { alpha = value; }
void Object::setRadius(float value)
{
	radius = value;
	SAFE_DELETE(sphere);
	sphere = new BoundingSphere(&center,radius);
}
void Object::setSize(D3DXVECTOR3 value) { 
	size = value; 
	SAFE_DELETE(box);
	box = new DebugBox(size);
#ifdef _DEBUG
#endif // _DEBUG
}

//===================================================================================================================================
//�y�I�u�W�F�N�gID�̃J�E���^�[�����Z�b�g�z
//===================================================================================================================================
void objectNS::resetCounter()
{
	Object::objectCounter = 0;
}