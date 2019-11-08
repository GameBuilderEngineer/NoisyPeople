//===================================================================================================================================
//�yTutorialUI.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/11/08
// [�X�V��]2019/11/08
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"
#include "GameMaster.h"
#include "TextureLoader.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace tutorialUINS
{
	//�萔
	const int BG_WIDTH = 415;									//����
	const int BG_HEIGHT = 81;									//�c��
	const int ARRAY_WIDTH = 42;									//����
	const int ARRAY_HIGHT = 62;									//�c��

	const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//��]
	const D3DCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255);		//�F
	const D3DXVECTOR3 arrayStPos = D3DXVECTOR3((BG_WIDTH / 2) + 140.0f, 30.0f, 0.0f);
	const D3DXVECTOR3 arrayInterval = D3DXVECTOR3(51.0f, 0.0f, 0.0f);

	enum TUTORIAL_STEP
	{
		TUTORIAL_STEP_1,
		TUTORIAL_STEP_2,
		TUTORIAL_STEP_3,
		TUTORIAL_STEP_4,
		TUTORIAL_STEP_5,
		TUTORIAL_STEP_MAX
	};
}

//===================================================================================================================================
//�y�`���[�g���A��UI�N���X�z
//===================================================================================================================================
class TutorialUI
{
public:
	TutorialUI();
	~TutorialUI();

	void setStep(int playerID, int inStep);
	int  getStep(int playerID) { return step[playerID]; }
	void render(void);

private:
	int step[gameMasterNS::PLAYER_NUM];

	Sprite **tutorialSpriteBG;
	Sprite **tutorialSpriteArray;

	D3DXVECTOR3 arrayPos[gameMasterNS::PLAYER_NUM][tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_MAX];
};