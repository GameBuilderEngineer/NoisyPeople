//===================================================================================================================================
//�yObject.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/09/23
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
//�y�R���X�g���N�^�z
//===================================================================================================================================
Object::Object()
{
	ZeroMemory(&position, sizeof(D3DXVECTOR3));												//�ʒu
	quaternion				= D3DXQUATERNION(0, 0, 0, 1);										//��]
	scale						= D3DXVECTOR3(1.0f,1.0f,1.0f);										//�X�P�[��
	radius					= 0.0f;																				//���a
	alpha						= 1.0f;																				//���l�̐ݒ�

	ZeroMemory(&speed, sizeof(D3DXVECTOR3));													//���x

	onGravity				= false;																				//�d�̓t���O
	onActive				= true;																				//�A�N�e�B�u�t���O

	axisX.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(1, 0, 0));						//x��
	axisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));						//y��
	axisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));						//z��
	reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 0, 0));			//-x��
	reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1, 0));			//-y��
	reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, -1));			//-z��
#ifdef _DEBUG
	axisX.color				= D3DXCOLOR(255, 0, 0, 255);											//x���J���[
	axisY.color				= D3DXCOLOR(0, 255, 0, 255);											//y���J���[
	axisZ.color				= D3DXCOLOR(0, 0, 255, 255);											//z���J���[
	reverseAxisX.color	= D3DXCOLOR(255, 0, 0, 255);											//-x���J���[
	reverseAxisY.color	= D3DXCOLOR(0, 255, 0, 255);											//-y���J���[
	reverseAxisZ.color	= D3DXCOLOR(0, 0, 255, 255);											//-z���J���[
#endif // _DEBUG

	ZeroMemory(&matrixPosition, sizeof(D3DXMATRIX));										//�ʒu�s��
	D3DXMatrixIdentity(&matrixRotation);																//��]�s��
	ZeroMemory(&matrixScale, sizeof(D3DXMATRIX));											//�X�P�[���s��
	ZeroMemory(&matrixWorld, sizeof(D3DXMATRIX));											//���[���h�s��

	existenceTimer = 1.0f;																					//���ݎ���

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Object::~Object()
{
	
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
	D3DXMatrixIdentity(&matrixWorld);														//���K��
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixScale);			//*�X�P�[���s��
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixRotation);		//*��]�s��
	D3DXMatrixMultiply(&matrixWorld, &matrixWorld,&matrixPosition);		//*�ʒu�s��

	//���[���h���W���玩�g�̎����C���X�V����
	axisX.update(position, D3DXVECTOR3(matrixWorld._11,matrixWorld._12,matrixWorld._13));
	axisY.update(position, D3DXVECTOR3(matrixWorld._21,matrixWorld._22,matrixWorld._23));
	axisZ.update(position, D3DXVECTOR3(matrixWorld._31,matrixWorld._32,matrixWorld._33));
	reverseAxisX.update(position, -D3DXVECTOR3(matrixWorld._11, matrixWorld._12, matrixWorld._13));
	reverseAxisY.update(position, -D3DXVECTOR3(matrixWorld._21, matrixWorld._22, matrixWorld._23));
	reverseAxisZ.update(position, -D3DXVECTOR3(matrixWorld._31, matrixWorld._32, matrixWorld._33));
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

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);					//�ʒu
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
//�ysetter�z
//===================================================================================================================================
void Object::activation()					{ onActive = true;}
void Object::inActivation()				{ onActive = false;}
void Object::setAlpha(float value)	{ alpha = value;}
