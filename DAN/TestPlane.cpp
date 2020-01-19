//===================================================================================================================================
//�yTestPlane.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/10/29
// [�X�V��]2019/10/29
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "TestPlane.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace testPlaneNS;

//===================================================================================================================================
//�y�C���X�^���X�N���X�F�R���X�g���N�^�z
//===================================================================================================================================
TestPlaneInstance::TestPlaneInstance(D3DXVECTOR3 position) : InstancingBillboardNS::Instance()
{
	//�����l�̐ݒ�
	this->position	= position;
	this->rotation	= D3DXVECTOR3(0, 0, 0);
	this->scale		= D3DXVECTOR2(2.0f, 2.0f);
	this->speed		= D3DXVECTOR3(0, 0, 0);
	this->limitTime	= 1.0f;
	this->uv.x		= 0.125f*7;
	this->uv.y		= 0.125f*7;
}

//===================================================================================================================================
//�y�C���X�^���X�N���X�F�X�V�z
//===================================================================================================================================
void TestPlaneInstance::update(float frameTime)
{
	//lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
}

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TestPlane::TestPlane() :InstancingBillboard::InstancingBillboard()
{
	InstancingBillboard::initialize(*textureNS::reference(textureNS::SPLASH),7,7);

	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(0, 10, 30)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(0, 20, -220)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(-15, 15, -210)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(-10, 15, -210)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(-5,  15, -210)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(0,   15,  -210)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(5,   15,  -210)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(-15, 15, -220)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(-15, 15, -215)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(-15, 15, -210)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(-15, 15, -205)));
	//generateInstance(new TestPlaneInstance(D3DXVECTOR3(-15, 15, -200)));

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TestPlane::~TestPlane()
{
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void TestPlane::update(float frameTime)
{
	InstancingBillboard::update(frameTime);
}
