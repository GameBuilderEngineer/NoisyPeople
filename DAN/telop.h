//-----------------------------------------------------------------------------
// �e���b�v�N���X [Telop.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/17
//-----------------------------------------------------------------------------
#pragma once
#include "Telop.h"
#include "Sprite.h"
#include "TextureLoader.h"

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

	const int WIDTH = 800;
	const int HEIGHT = 568;
	const D3DXVECTOR3 POSITION = D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);
}


//=============================================================================
//�N���X��`
//=============================================================================
class Telop
{
private:
	Sprite *telop[telopNS::MAX_TELOP];

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render();
};
