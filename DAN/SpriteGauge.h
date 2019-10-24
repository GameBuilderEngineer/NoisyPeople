//-----------------------------------------------------------------------------
// �N���X [.h]�@�����p
// Author : �V��
// �쐬�J�n�� : 2019/10/17  
//-----------------------------------------------------------------------------
#pragma once
#include "Sprite.h"
#include "TextureLoader.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace spriteGaugeNS
{
	//�X�v���b�V���X�v���C�g���̒�`
	const int WIDTH = 800;
	const int HEIGHT = 568;
	const D3DXVECTOR3 POSITION = D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);
}

class SpriteGauge
{
private:
	Sprite *gauge = new Sprite;


public:
	SpriteGauge();
	~SpriteGauge();

	void initialize();
	void render();

};

