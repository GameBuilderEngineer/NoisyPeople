//===================================================================================================================================
//�yTextureLoader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/11/02
//===================================================================================================================================
//===================================================================================================================================
//�ۑ�
//�V�[���ʂɓǍ����\�[�X�𕪊����ǂݍ���
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace textureNS
{
	enum TEXTURE_NUMBER
	{
		// Splash
		SPLASH,

		// Tutorial
		TUTORIAL1,
		TUTORIAL2,
		TUTORIAL_UI_0,
		TUTORIAL_UI_1,
		TUTORIAL_UI_PLANE,

		// Title
		TITLE_LOGO,
		TITLE_MENU,
		TITLE_INFO_TUTO,
		TITLE_INFO_GAME,
		TITLE_INFO_CREDIT,
		TITLE_INFO_EXIT,
		TITLE_COPYRIGHT,			
		TITLE_BAR,				

		// Credit
		CREDIT,
		CREDIT2,

		// Game
		UI_INFO_10,
		UI_INFO_30,
		UI_INFO_50,
		UI_INFO_ASSULT,
		UI_INFO_LIMIT1,
		UI_INFO_LIMIT2,
		UI_INFO_MISSION,
		UI_INFO_BAR,
		UI_MAIN_RETICLE,
		UI_SUB_RETICLE,
		UI_MAIN_DIGITAL_RETICLE,
		UI_SUB_DIGITAL_RETICLE,
		UI_TIMERFLAME,
		UI_BUTTON_SHOT,
		UI_BUTTON_JUMP,
		UI_BUTTON_VISION,
		UI_BUTTON_SV,
		UI_BUTTON_SHIFT,
		UI_BUTTON_BOX,
		UI_STATUSFRAME_P1,
		UI_STATUSFRAME_P2,
		UI_HP_GUAGE_P1,
		UI_HP_GUAGE_P2,
		UI_HP_GUAGE_EMP_P1,
		UI_HP_GUAGE_EMP_P2,
		UI_ICON_P1,
		UI_ICON_P2,
		UI_EN_GUAGE_EMP_P1,
		UI_EN_GUAGE_EMP_P2,
		UI_EN_GUAGE_P1,
		UI_EN_GUAGE_P2,
		UI_EN_TANK,
		UI_TREE_MARKER,
		UI_TREE_SIGN,
		UI_TIMER_NUMBER,

		// Result
		RESULT,
		RESULT_LINE,
		RESULT_FLAME,
		RESULT_CHARA_A,
		RESULT_CHARA_B,
		RESULT_PLAYER,
		RESULT_GREENIG,
		RESULT_RYOKUKA,
		RESULT_PERSENT,
		RESULT_NEXT,
		RESULT_BG,
		RESULT_GREEN_NUM,
		RESULT_RYOKUKA_KAZU,
		RESULT_FLAME02,
		//RESULT_NUMBER,
		RESULT_DEFEAT,
		RESULT_GEKITAI,
		RESULT_EXCELLENT,
		RESULT_GREAT,
		RESULT_CLEARE,
		RESULT_FAILED,

		// etc
		NUMBER,
		TOON_OUT_LINE,
		TOON_SHADE,
		LIGHT_001,
		TARGET,
		SAMPLE_TREE,
		SAMPLE_GRASS,
		NEKO,
		COLOR_SCALE,
		DATE_ALPHA,

		MAX_TEXTURE,
	};

	//�e�N�X�`���O���Q�Ɗ֐�
	LPDIRECT3DTEXTURE9* reference(int textureNo);

}

//===================================================================================================================================
//�y�e�N�X�`���Ǎ��N���X�z
//===================================================================================================================================
class TextureLoader : public Base
{
private:
	const char* textureName[textureNS::MAX_TEXTURE];
	LPDIRECT3DTEXTURE9 texture[textureNS::MAX_TEXTURE];
public:
	TextureLoader();
	~TextureLoader();

	void load(LPDIRECT3DDEVICE9 device);
	void release();

	LPDIRECT3DTEXTURE9* getTexture(int textureNo) { return &texture[textureNo]; }
};

