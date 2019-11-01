//-----------------------------------------------------------------------------
// �e���b�v�N���X [Telop.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/17
//-----------------------------------------------------------------------------
#pragma once
#include "Telop.h"
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "UtilityFunction.h"

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
		TELOP_INFO_BAR,
		MAX_TELOP
	};
	
	enum STATE
	{
		OPEN,
		DISPLAY,
		CLOSE,
		STATE_NUM,
	};

	const int WIDTH = 1500;
	const int MIN_HEIGHT = 0;
	const int MAX_HEIGHT = 120;
	const int WIDTH_BAR = 1920;
	const int HEIGHT_BAR = 62;
	const D3DXVECTOR3 POSITION = D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);
	const float OPEN_TIME = 0.5f;
	const float CLOSE_TIME = 0.3f;
	const float DISPLAY_TIME = 1.0f;
}


//=============================================================================
//�N���X��`
//=============================================================================
class Telop
{
private:
	Sprite *telop[telopNS::MAX_TELOP];
	Input*	input;

	float telopTimer;
	float displayTimer;
	int heightValue;
	bool telopFlag;
	int state;
public:
	void initialize();
	void uninitialize();
	void update(float _frameTimer);
	void render();
};
