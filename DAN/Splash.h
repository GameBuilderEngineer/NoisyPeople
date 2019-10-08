//===================================================================================================================================
//�ySplash.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
//#include "Splash2D.h"
#include "Sprite.h"
#include "SplashSprite.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace splashNS
{
	//�X�v���b�V���X�v���C�g���̒�`
	const int WIDTH = 800;
	const int HEIGHT = 568;
	const D3DXVECTOR3 POSITION = D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 0);

	//�V�[������
	const float SCENE_TIME = 10.0f;
}

//===================================================================================================================================
//�y�X�v���b�V���V�[���N���X�z
//===================================================================================================================================
class Splash :public AbstractScene
{
private:
	// �X�v���b�V��Sprite
	SplashSprite* splashSprite;

public:
	Splash();
	~Splash();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;
	void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};

