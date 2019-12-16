//-----------------------------------------------------------------------------
// �G�l�~�[�ǐՃ}�[�N[EnemyChaseMark.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/10
//-----------------------------------------------------------------------------
#include "EnemyChaseMark.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
using namespace enemyChaseMarkNS;

// Static�����o�ϐ�
int StandardChaseMark::cntInstance= 0;

StandardChaseMark::StandardChaseMark(D3DXVECTOR3 position) : InstancingBillboardNS::Instance()
{
	//�����l�̐ݒ�
	this->position	= position;
	this->rotation	= D3DXVECTOR3(-D3DX_PI / 2.0f, 0, 0);
	this->scale		= D3DXVECTOR2(1.0f, 1.0f);
	this->speed		= D3DXVECTOR3(0, 0, 0);
	this->limitTime	= 1.0f;
	//this->uv.x		= 0.125f*7;
	//this->uv.y		= 0.125f*7;
}

void StandardChaseMark::update(float frameTime)
{
	//lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
}


//=============================================================================
// �����_���[�N���X
//=============================================================================
EnemyChaseMark::EnemyChaseMark() :InstancingBillboard::InstancingBillboard()
{
	using namespace InstancingBillboardNS;
	InstancingBillboard::initialize(*textureNS::reference(textureNS::UI_CAHSE_MARK),0,0);
	setRenderType(FOREGROUND_PASS | OFF_BILLBOARD_PASS);
}

EnemyChaseMark::~EnemyChaseMark()
{
}

void EnemyChaseMark::update(float frameTime)
{
	InstancingBillboard::update(frameTime);
}

void EnemyChaseMark::generateMark(enemyChaseMarkNS::StandardChaseMark* mark)
{
	StandardChaseMark::cntInstance++;

	generateInstance(mark);
	updateAccessList();
}

void EnemyChaseMark::deleteMark(enemyChaseMarkNS::StandardChaseMark* mark)
{
	StandardChaseMark::cntInstance--;

	// �A�h���X�l�Ō���
	int target;
	for (int i = 0; i < instanceList->nodeNum; i++)
	{
		if (mark == *instanceList->getValue(i))
		{
			target = i;
			break;
		}
	}

	mark->lifeTimer = mark->limitTime + 1.0f;	// deleteInstance()��L���ɂ��邽��
	deleteInstance(target);
	updateAccessList();
}