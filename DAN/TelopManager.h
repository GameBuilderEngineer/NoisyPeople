//-----------------------------------------------------------------------------
// �e���b�v�}�l�[�W���[�N���X [TelopManager.h]
// Author : �V���@���m
// �쐬�J�n�� : 2019/10/31
//-----------------------------------------------------------------------------
#pragma once
#include "Telop.h"
#include "Sprite.h"


//=============================================================================
// ���O���
//=============================================================================
namespace telopManagerNS
{
	// �e���b�v�^�C�v
	enum TYPE
	{
		TELOP_INFO_BAR,
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

class TelopManager
{
private:
	Telop* telop[telopManagerNS::MAX_TELOP];

	bool playFlag;

public:
	TelopManager();
	~TelopManager();

	void initialize();
	void uninitialize();
	void update(float _frameTime);
	void render();
	void play(int type);
};

