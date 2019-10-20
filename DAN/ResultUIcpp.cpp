//============================
//[ResultUI.cpp]
//����W��
//============================

//============================
//�C���N���[�h
//============================
#include "ResultUI.h"

//============================
//�yusing�錾�z
//============================
using namespace resultUiNS;

//============================
//�O���[�o���ϐ�
//============================

//============================
//������
//============================
void ResultUI::initialize()
{
	for (int i = 0; i < RESULTUI_MAX; i++)
	{
		resultUI[i] = new Sprite;
	}

	//�t�F�C�Y�̏�����(���t�F�C�Y��)
	resultPhase = PHASE_01;

	//���U���g���S�̏�����
	resultUI[RESULT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_RESULT								//����
		, HEIGHT_RESULT								//�c��
		, POSITION_RESULT							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//���C���̏�����
	resultUI[LINE]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_LINE								//����
		, HEIGHT_LINE								//�c��
		, POSITION_LINE								//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�t���[���̏�����
	resultUI[FLAME]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_FLAME								//����
		, HEIGHT_FLAME								//�c��
		, POSITION_FLAME							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�L����A�̏�����
	resultUI[CHARA_A]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_CHARA_A								//����
		, HEIGHT_CHARA_A							//�c��
		, POSITION_CHARA_A							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�L����B�̏�����
	resultUI[CHARA_B]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_CHARA_B								//����
		, HEIGHT_CHARA_B							//�c��
		, POSITION_CHARA_B							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�v���C���[�̏�����
	resultUI[PLAYER]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_PLAYER								//����
		, HEIGHT_PLAYER								//�c��
		, POSITION_PLAYER							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�Ή�(�p��)�̏�����
	resultUI[GREENIG]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_GREENIG								//����
		, HEIGHT_GREENIG							//�c��
		, POSITION_GREENIG							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�Ή�
	resultUI[RYOKUKA]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_RYOKUKA								//����
		, HEIGHT_RYOKUKA							//�c��
		, POSITION_RYOKUKA							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�p�[�Z���g�̏�����
	resultUI[PERSENT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_PERSENT								//����
		, HEIGHT_PERSENT							//�c��
		, POSITION_PERSENT							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�����̏�����
	resultUI[NUMBER]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_NUMBER								//����
		, HEIGHT_NUMBER								//�c��
		, POSITION_NUMBER							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//����(�p��)�̏�����
	resultUI[DEFEAT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_DEFEAT								//����
		, HEIGHT_DEFEAT								//�c��
		, POSITION_DEFEAT							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//����
	resultUI[GEKITAI]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_GEKITAI								//����
		, HEIGHT_GEKITAI							//�c��
		, POSITION_GEKITAI							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�G�N�Z�����g�̏�����
	resultUI[EXCELLENT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_EXCELLENT							//����
		, HEIGHT_EXCELLENT							//�c��
		, POSITION_EXCELLENT						//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�O���C�g�̏�����
	resultUI[GREAT]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_GREAT								//����
		, HEIGHT_GREAT								//�c��
		, POSITION_GREAT							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�N���A�̏�����
	resultUI[CLEAR]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_CLEAR								//����
		, HEIGHT_CLEAR								//�c��
		, POSITION_CLEAR							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//���s�̏�����
	resultUI[FAIL]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_FAIL								//����
		, HEIGHT_FAIL								//�c��
		, POSITION_FAIL								//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F
}

//============================
//�`��
//============================
void ResultUI::render()
{
	switch (resultPhase)
	{
		case PHASE_01:
			resultUI[RESULT]->render();
			resultUI[LINE]->render();
			break;
		case PHASE_02:
			resultUI[RESULT]->render();
			resultUI[LINE]->render();
			break;
		case PHASE_03:
			resultUI[RESULT]->render();
			resultUI[LINE]->render();
			resultUI[FLAME]->render();
			resultUI[CHARA_A]->render();
			resultUI[CHARA_B]->render();
			break;
		case PHASE_04:
			break;
		default:
			break;
	}
}

//============================
//�X�V
//============================
void ResultUI::update(Input *input)
{
	
}

//============================
//�I������
//============================
void ResultUI::uninitialize()
{
	for (int i = 0; i < RESULTUI_MAX; i++)
	{
		delete resultUI[i];
	}

}

