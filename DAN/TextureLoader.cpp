//===================================================================================================================================
//�yTextureLoader.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/22
// [�X�V��]2019/10/24
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
	textureName[TUTORIAL1]			= { "tutorial01.png" };
	textureName[TUTORIAL2]			= { "tutorial02.png" };

	// Title
	textureName[TITLE_LOGO]			= { "DAN_Logo.png" };
	textureName[TITLE_MENU]			= { "Title_Menu.png" };
	textureName[TITLE_INFO_TUTO]	= { "Tutorial_info.png" };
	textureName[TITLE_INFO_GAME]	= { "GameStart_info.png" };
	textureName[TITLE_INFO_CREDIT]	= { "Credits_info.png" };
	textureName[TITLE_INFO_EXIT]	= { "Exit_info.png" };
	textureName[TITLE_COPYRIGHT]	= { "Copyrights.png" };
	textureName[TITLE_BAR]			= { "Select_Bar.png"};

	// Credit
	textureName[CREDIT]				= { "Credit_01.png" };

	// Game
	textureName[UI_COUNT_DOWN_ONE]	= { "Game_CountDown_One.png" };
	textureName[UI_INFO_10] = { "Info_10.png" };
	textureName[UI_INFO_30] = { "Info_30.png" };
	textureName[UI_INFO_50] = { "Info_50.png" };
	textureName[UI_INFO_ASSULT] = { "Info_Assult.png" };
	textureName[UI_INFO_LIMIT1] = { "Info_Limit1.png" };
	textureName[UI_INFO_LIMIT2] = { "Info_Limit2.png" };
	textureName[UI_INFO_MISSION] = { "Info_Mission.png" };
	textureName[UI_INFO_BAR] = { "Info_Bar.png" };


	// Result
	textureName[RESULT_WINDOW]		= { "Result_Window.png" };

	// etc
	textureName[TOON_OUT_LINE]		= { "Outline.bmp" };
	textureName[TOON_SHADE]			= { "Shade.bmp" };
	textureName[LIGHT_001]			= { "Light_001.png" };
	textureName[TARGET]				= { "stencil.png" };
	textureName[SAMPLE_TREE]		= { "tree.png" };
	textureName[SAMPLE_GRASS]		= { "middleGrass.png" };
	textureName[NEKO]				= { "Chara_B.png" };

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
