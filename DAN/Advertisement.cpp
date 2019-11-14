//===================================================================================================================================
//�yAdvertisement.cpp�z
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/12
//===================================================================================================================================
#include "Advertisement.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
using namespace advetisementNS;

// �R���̈ɒB
alphaVer::alphaVer(D3DXVECTOR3 position) : InstancingBillboardNS::Instance()
{
	//�����l�̐ݒ�
	this->position	= position;
	this->rotation	= D3DXVECTOR3(0, 0, 0);
	this->scale		= D3DXVECTOR2(8.0f, 8.0f);
	this->speed		= D3DXVECTOR3(0, 0, 0);
	this->limitTime	= 1.0f;
	//this->uv.x		= 0.125f*7;
	//this->uv.y		= 0.125f*7;
}

void alphaVer::update(float frameTime)
{
	//lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
}


//=============================================================================
// �����_���[�N���X
//=============================================================================
Advertisement::Advertisement() :InstancingBillboard::InstancingBillboard()
{
	InstancingBillboard::initialize(*textureNS::reference(textureNS::DATE_ALPHA),0,0);

	generateInstance(new alphaVer(D3DXVECTOR3(-30, 158, 35)));
}

Advertisement::~Advertisement()
{
}

void Advertisement::update(float frameTime)
{
	InstancingBillboard::update(frameTime);
}
