//===================================================================================================================================
//�yBearGauge.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/10/29
// [�X�V��]2019/10/29
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "BearGauge.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "Bear.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace BearGaugeNS;

//===================================================================================================================================
//�y�C���X�^���X�N���X�F�R���X�g���N�^�z
//===================================================================================================================================
BearGaugeInstance::BearGaugeInstance(D3DXVECTOR3 position, int _hitPointMax) : InstancingBillboardNS::Instance()
{
	//�����l�̐ݒ�
	this->position	= position;
	this->rotation	= D3DXVECTOR3(0, 0, 0);
	this->scale		= D3DXVECTOR2(1.0f, 0.2f) * SCALE_X;
	this->speed		= D3DXVECTOR3(0, 0, 0);
	this->limitTime	= 1000.0f;
	this->uv.x		= 0.0f;
	this->uv.y		= 0.0f;
	this->color		=  D3DCOLOR_RGBA(0, 255, 0, 255);

	hitPointMax		= _hitPointMax;
	offSet			= position;
}

//===================================================================================================================================
//�y�C���X�^���X�N���X�F�X�V�z
//===================================================================================================================================
void BearGaugeInstance::update(float frameTime, D3DXMATRIX* _worldMatrix, int hitPoint)
{
	lifeTimer = 0;
	if (hitPoint == 0) { lifeTimer = limitTime; }

	D3DXVec3TransformCoord(&position, &offSet, _worldMatrix);

	scale.x = (float)hitPoint / (float)hitPointMax * SCALE_X;
	if (scale.x < 0.4 * SCALE_X) color = D3DCOLOR_RGBA(255, 255, 0, 255);
	if (scale.x < 0.2 * SCALE_X) color = D3DCOLOR_RGBA(255, 0, 0, 255);
}
/////////////////////////////////////////////////////
//////////////////////////////////////////////////////

// �ÓI�����o
BearGauge* BearGauge::pointer = NULL;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
BearGauge::BearGauge(int hitPoint1, int hitPoint2, int hitPoint3) :InstancingBillboard::InstancingBillboard()
{
	pointer = this;

	using namespace InstancingBillboardNS;
	InstancingBillboard::initialize(*textureNS::reference(textureNS::UI_HP_GUAGE_ENEMY),0,0);
	//setRenderType(FOREGROUND_PASS | OFF_BILLBOARD_PASS);

	instancePointer[HP] =  new BearGaugeInstance(D3DXVECTOR3(0, 50, 0), hitPoint1);
	instancePointer[LEFT_ARM] = new BearGaugeInstance(D3DXVECTOR3(-20, 0, 0), hitPoint2);
	instancePointer[RIGHT_ARM] = new BearGaugeInstance(D3DXVECTOR3(20, 0, 0), hitPoint3);

	generateInstance(instancePointer[HP]);
	updateAccessList();
	generateInstance(instancePointer[LEFT_ARM]);
	updateAccessList();
	generateInstance(instancePointer[RIGHT_ARM]);
	updateAccessList();
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
BearGauge::~BearGauge()
{
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void BearGauge::update(float frameTime)
{
	InstancingBillboard::update(frameTime);
}
