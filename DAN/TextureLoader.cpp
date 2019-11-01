//===================================================================================================================================
//�yTextureLoader.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/22
// [�X�V��]2019/10/17
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
	if(pointerTextureLoader==NULL)pointerTextureLoader = this;

	// Splash
	textureName[SPLASH] = { "Splash.png" };

	// Tutorial
	textureName[TUTORIAL1] = { "tutorial01.png" };
	textureName[TUTORIAL2] = { "tutorial02.png" };

	// Title
	textureName[TITLE_LOGO] = { "DAN_Logo.png" };
	textureName[TITLE_MENU] = { "Title_Menu.png" };
	textureName[TITLE_INFO_TUTO] = { "Tutorial_info.png" };
	textureName[TITLE_INFO_GAME] = { "GameStart_info.png" };
	textureName[TITLE_INFO_CREDIT] = { "Credits_info.png" };
	textureName[TITLE_INFO_EXIT] = { "Exit_info.png" };
	textureName[TITLE_COPYRIGHT] = { "Copyrights.png" };
	textureName[TITLE_BAR] = {"Select_Bar.png"};


	// Operation
	textureName[OPERATION_GAMEPAD] = { "Operation_GamePad.png" };
	textureName[OPERATION_KEYBOARD] = { "Operation_KeyBoard.png" };

	// Credit
	textureName[CREDIT] = { "Credit_01.png" };

	// Game
	textureName[UI_COUNT_DOWN_ONE] = { "Game_CountDown_One.png" };

	//effect
	textureName[EFFECT_DAMAGE] = { "Game_Damage_Effect.png" };

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
	setVisualDirectory();
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
