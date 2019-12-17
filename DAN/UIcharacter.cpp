//============================
//[UIcharacter.cpp]
//����W��
//============================

//============================
//�C���N���[�h
//============================
#include "UIcharacter.h"

//============================
//�yusing�錾�z
//============================
using namespace uiCharacterNS;

//============================
//�R���X�g���N�^
//============================
UIcharacter::UIcharacter()
{
	result = new Result_Chara;
	player = new Player_Chara;
	greenChara = new Greening_Chara;
	greenMoji = new Greening_Moji;
	defeatChara = new Defeat_Chara;
	defeatMoji = new Defeat_Moji;
	greening = new WholeGreening;
	persent = new Persent;
}

//============================
//�f�X�g���N�^
//============================
UIcharacter::~UIcharacter()
{
	delete result;
	delete player;
	delete greenChara;
	delete greenMoji;
	delete defeatChara;
	delete defeatMoji;
	delete greening;
	delete persent;
}

//============================
//������
//============================
void UIcharacter::initialize(int playerType)
{
	result->initialize();
	player->initialize(playerType);
	greenChara->initialize(playerType);
	greenMoji->initialize(playerType);
	defeatChara->initialize(playerType);
	defeatMoji->initialize(playerType);
	greening->initialize();
	persent->initialize();
}

//============================
//�`��
//���U���g�t�F�C�Y���Q�Ƃ��`��
//============================
void UIcharacter::render(int resultPhase)
{
	switch (resultPhase)
	{
	case PHASE_01:
		result->render();
		break;
	case PHASE_02:
		result->render();
		greening->render();
		persent->render();
		break;
	case PHASE_03:
		result->render();
		greening->render();
		persent->render();
		player->render();
		greenChara->render();
		greenMoji->render();
		defeatChara->render();
		defeatMoji->render();
		break;
	case PHASE_04:
		result->render();
		greening->render();
		persent->render();
		player->render();
		greenChara->render();
		greenMoji->render();
		defeatChara->render();
		defeatMoji->render();
		break;
	case PHASE_05:
		result->render();
		greening->render();
		persent->render();
		player->render();
		greenChara->render();
		greenMoji->render();
		defeatChara->render();
		defeatMoji->render();
		break;
	default:
		break;

	}
	
}

//============================
//�X�V
//�����C����������Α��ɂ��悤���邯�ǎ����D��
//============================
void UIcharacter::update(int resultPhase,int playerType)
{
	switch (resultPhase)
	{
	case PHASE_01:
		result->update();
		break;
	case PHASE_02:
		greening->update();
		break;
	case PHASE_03:
		greening->update();
		player->update(playerType);
		greenChara->update(playerType);
		greenMoji->update(playerType);
		defeatChara->update(playerType);
		defeatMoji->update(playerType);
		break;
	case PHASE_04:
		break;
	default:
		break;
	}
}

