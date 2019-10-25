//-----------------------------------------------------------------------------
// �e���b�v�N���X [Telop.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/17
//-----------------------------------------------------------------------------
#pragma once
#include "Telop.h"
#include "Sprite.h"

//=============================================================================
// ���O���
//=============================================================================
namespace telopNS
{
	// �e���b�v�^�C�v
	enum TYPE
	{
		TELOP_TYPE0,
		TELOP_TYPE1,
		TELOP_TYPE2,
		TELOP_TYPE3,
		TELOP_TYPE4,
		TELOP_TYPE5,
		TELOP_TYPE6,
		MAX_TELOP
	};
}


//=============================================================================
//�N���X��`
//=============================================================================
class Telop
{
private:
	Sprite *telop = new Sprite;

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render();
};
