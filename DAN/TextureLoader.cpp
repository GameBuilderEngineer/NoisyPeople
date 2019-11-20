//===================================================================================================================================
//�yTextureLoader.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/22
// [�X�V��]2019/11/02
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "TextureLoader.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
using namespace textureNS;

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
TextureLoader* pointerTextureLoader = NULL;	//�O���Q�Ɨp

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TextureLoader::TextureLoader()
{
	//�O���[�o���|�C���^�֎��g�̃|�C���^����
	if (pointerTextureLoader == NULL)pointerTextureLoader = this;

	// Splash
	textureName[SPLASH] = { "Splash.png" };

	// Tutorial
	textureName[TUTORIAL1] = { "Slide_01.png" };
	textureName[TUTORIAL2] = { "Slide_02.png" };
	textureName[TUTORIAL_UI_0] = { "steplist.png" };
	textureName[TUTORIAL_UI_1] = { "check.png" };
	textureName[TUTORIAL_UI_PLANE] = { "comment.png" };

	// Title
	textureName[TITLE_LOGO] = { "DAN_Logo.png" };
	textureName[TITLE_MENU] = { "Title_Menu.png" };
	textureName[TITLE_INFO_TUTO] = { "Tutorial_info.png" };
	textureName[TITLE_INFO_GAME] = { "GameStart_info.png" };
	textureName[TITLE_INFO_CREDIT] = { "Credits_info.png" };
	textureName[TITLE_INFO_EXIT] = { "Exit_info.png" };
	textureName[TITLE_COPYRIGHT] = { "Copyrights.png" };
	textureName[TITLE_BAR] = { "Select_Bar.png" };

	// Credit
	textureName[CREDIT] = { "Credit_01.png" };

	// Game
	textureName[UI_INFO_10] = { "Info_10.png" };
	textureName[UI_INFO_30] = { "Info_30.png" };
	textureName[UI_INFO_50] = { "Info_50.png" };
	textureName[UI_INFO_ASSULT] = { "Info_Assult.png" };
	textureName[UI_INFO_LIMIT1] = { "Info_Limit1.png" };
	textureName[UI_INFO_LIMIT2] = { "Info_Limit2.png" };
	textureName[UI_INFO_MISSION] = { "Info_Mission.png" };
	textureName[UI_INFO_BAR] = { "Info_Bar.png" };
	textureName[UI_RETICLE] = { "Reticle.png" };
	textureName[UI_DIGITAL_RETICLE] = { "Digi_Reticle.png" };
	textureName[UI_TIMERFLAME] = { "Time_Frame.png" };
	textureName[UI_BUTTON1] = { "SV.png" };
	textureName[UI_BUTTON2] = { "shot.png" };
	textureName[UI_BUTTON3] = { "shift.png" };
	textureName[UI_BUTTON4] = { "vision.png" };
	textureName[UI_BUTTON_BOX] = { "CommandBox.png" };
	textureName[UI_STATUSFRAME_P1] = { "Status_Frame_1.png" };
	textureName[UI_STATUSFRAME_P2] = { "Status_Frame_2.png" };
	textureName[UI_HP_GUAGE_P1] = { "HPGuage1.png" };
	textureName[UI_HP_GUAGE_P2] = { "HPGuage2.png" };
	textureName[UI_HP_GUAGE_EMP_P1]={ "HPGuage_Emp1.png" };
	textureName[UI_HP_GUAGE_EMP_P2] = { "HPGuage_Emp2.png" };
	textureName[UI_ICON_P1] = { "Chara_1.png" };
	textureName[UI_ICON_P2] = { "Chara_2.png" };
	textureName[UI_EN_GUAGE_EMP_P1] = { "EnGuage_Emp1.png" };
	textureName[UI_EN_GUAGE_EMP_P2] = { "EnGuage_Emp2.png" };


	// ResultUI�̕�����L��
	textureName[RESULT] = { "Result.png" };
	textureName[RESULT_PLAYER] = { "Player_No.png" };
	textureName[RESULT_GREENIG] = { "Greening_Rate.png" };
	textureName[RESULT_RYOKUKA] = { "Ryokuka_Ritsu.png" };
	textureName[RESULT_PERSENT] = { "Percent.png" };
	textureName[RESULT_DEFEAT] = { "Defeated_Counts.png" };
	textureName[RESULT_GEKITAI] = { "Gekitai_Su.png" };

	//ResultUI�̃e�N�X�`��
	textureName[RESULT_LINE] = { "Line.png" };
	textureName[RESULT_FLAME] = { "Flame.png" };
	textureName[RESULT_CHARA_A] = { "Chara_A.png" };
	textureName[RESULT_CHARA_B] = { "Chara_B.png" };
	textureName[RESULT_NEXT] = { "Next.png" };

	//ResultUI�̃����N
	textureName[RESULT_EXCELLENT] = { "Excellent.png" };
	textureName[RESULT_GREAT] = { "Great.png" };
	textureName[RESULT_CLEARE] = { "Cleared.png" };
	textureName[RESULT_FAILED] = { "Failed.png" };

	// etc
	textureName[NUMBER] = { "num.png" };
	textureName[TOON_OUT_LINE] = { "Outline.bmp" };
	textureName[TOON_SHADE] = { "Shade.bmp" };
	textureName[LIGHT_001] = { "Light_001.png" };
	textureName[TARGET] = { "stencil.png" };
	textureName[SAMPLE_TREE] = { "tree.png" };
	textureName[SAMPLE_GRASS] = { "middleGrass.png" };
	textureName[NEKO] = { "Chara_B.png" };
	textureName[COLOR_SCALE] = { "colorScale.png" };
	textureName[DATE_ALPHA] = { "DateAlpha.png" };
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TextureLoader::~TextureLoader()
{
	pointerTextureLoader = NULL;
	release();
}

//===================================================================================================================================
//�y���[�h�z
//===================================================================================================================================
void TextureLoader::load(LPDIRECT3DDEVICE9 device)
{
	setTextureDirectory();
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(device,textureName[i],&texture[i]);
	}
}

//===================================================================================================================================
//�y����z
//===================================================================================================================================
void TextureLoader::release()
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (texture[i] != NULL)
		{//�e�N�X�`���̉��
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

//===================================================================================================================================
//�y�e�N�X�`���O���Q�Ɗ֐��z
//===================================================================================================================================
LPDIRECT3DTEXTURE9* textureNS::reference(int textureNo)
{
	return pointerTextureLoader->getTexture(textureNo);
}
