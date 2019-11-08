//===================================================================================================================================
//�yTutorialUI.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/11/08
// [�X�V��]2019/11/08
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "TutorialUI.h"
#include "player.h"

//===================================================================================================================================
// �yusing�錾�z
//===================================================================================================================================
using namespace tutorialUINS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TutorialUI::TutorialUI()
{
	RECT recDisplay;
	HWND hDeskWnd;
	hDeskWnd = GetDesktopWindow();
	GetClientRect(hDeskWnd, &recDisplay);

	tutorialSpriteBG = new Sprite *[gameMasterNS::PLAYER_NUM];
	tutorialSpriteArray = new Sprite *[gameMasterNS::PLAYER_NUM];

	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//���
		step[i] = TUTORIAL_STEP::TUTORIAL_STEP_2;

		//���̈ʒu
		for (int j = 0; j < tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_MAX; j++)
		{
			arrayPos[i][j] =
				(arrayStPos + (D3DXVECTOR3(recDisplay.right / 2, 0.0f, 0.0f)*i))
				+ (arrayInterval * j);
		}

		tutorialSpriteBG[i] = new Sprite;
		tutorialSpriteArray[i] = new Sprite;

		tutorialSpriteBG[i]->initialize(
			*textureNS::reference(textureNS::TUTORIAL_UI_0),//�e�N�X�`��
			SpriteNS::TOP_LEFT,		//���S
			BG_WIDTH,				//����
			BG_HEIGHT,				//�c��
			D3DXVECTOR3((recDisplay.right / 2)*i, 0, 0), 	//�\���ʒu
			rot,						//��]
			col);					//�F

		tutorialSpriteArray[i]->initialize(
			*textureNS::reference(textureNS::TUTORIAL_UI_1),//�e�N�X�`��
			SpriteNS::TOP_LEFT,		//���S
			ARRAY_WIDTH,				//����
			ARRAY_HIGHT,				//�c��
			arrayPos[i][step[i]], 	//�\���ʒu
			rot,						//��]
			col);					//�F
	}
}

//===================================================================================================================================
//�y�i���̐ݒ�z
//===================================================================================================================================
void TutorialUI::setStep(int playerID, int inStep)
{
	step[playerID] = inStep;
	tutorialSpriteArray[playerID]->setPosition(arrayPos[playerID][step[playerID]]);
	tutorialSpriteArray[playerID]->setVertex();
}

//===================================================================================================================================
//�y2D�`��z
//===================================================================================================================================
void TutorialUI::render(void)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		tutorialSpriteBG[i]->render();
		tutorialSpriteArray[i]->render();
	}
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TutorialUI::~TutorialUI()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		SAFE_DELETE(tutorialSpriteBG[i]);
		SAFE_DELETE(tutorialSpriteArray[i]);
	}
	SAFE_DELETE_ARRAY(tutorialSpriteBG);
	SAFE_DELETE_ARRAY(tutorialSpriteArray);
}
