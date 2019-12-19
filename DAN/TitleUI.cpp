//============================
//[TitleUI.cpp]
//����W��
//============================

//============================
//�C���N���[�h
//============================
#include "TitleUI.h"
#include "Sound.h"
#include "BasicUI.h"

//============================
//�yusing�錾�z
//============================
using namespace titleUiNS;


//============================
//�O���[�o���ϐ�
//============================

//============================
//������
//============================
void TitleUI::initialize()
{
	//UI�̑������̏�����
	for (int i = 0; i < UI_MAX; i++)
	{
		UI[i] = new Sprite;
	}
	
	//���S�̏�����
	UI[UI_LOGO]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_LOGO								//����
		, HEIGHT_LOGO								//�c��
		, POSITION_LOGO								//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�I�����̏�����
	UI[UI_MENU]->initialize(
		*textureNS::reference(textureNS::TITLE_MENU)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_MENU								//����
		, HEIGHT_MENU								//�c��
		, POSITION_MENU								//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�����̏�����
	UI[UI_INFO_TUTO]->initialize(
		*textureNS::reference(textureNS::TITLE_INFO_TUTO)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_MENU_INFO							//����
		, HEIGHT_MENU_INFO							//�c��
		, POSITION_MENU_INFO						//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	UI[UI_INFO_GAME]->initialize(
		*textureNS::reference(textureNS::TITLE_INFO_GAME)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_MENU_INFO							//����
		, HEIGHT_MENU_INFO							//�c��
		, POSITION_MENU_INFO						//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	UI[UI_INFO_CREDIT]->initialize(
		*textureNS::reference(textureNS::TITLE_INFO_CREDIT)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_MENU_INFO							//����
		, HEIGHT_MENU_INFO							//�c��
		, POSITION_MENU_INFO						//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	UI[UI_INFO_EXIT]->initialize(
		*textureNS::reference(textureNS::TITLE_INFO_EXIT)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_MENU_INFO							//����
		, HEIGHT_MENU_INFO							//�c��
		, POSITION_MENU_INFO						//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//����ҏ��̏�����
	UI[UI_COPYRIGHT]->initialize(
		*textureNS::reference(textureNS::TITLE_COPYRIGHT)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_COPYRIGHT							//����
		, HEIGHT_COPYRIGHT							//�c��
		, POSITION_COPYRIGHT						//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�o�[�̏�����
	UI[UI_BAR]->initialize(
		*textureNS::reference(textureNS::TITLE_BAR)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_BAR									//����
		, HEIGHT_BAR								//�c��
		, POSITION_BAR_TUTO							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�v���X�G�j�[�{�^��
	UI[UI_PRESS]->initialize(
		*textureNS::reference(textureNS::TITLE_PRESS)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_PRESS									//����
		, HEIGHT_PRESS								//�c��
		, POSITION_PRESS							//�\���ʒu
		, ROTATION									//��]
		, COLOR);									//�F

	//�I����Ԃ̏�����
	selectState = TUTORIAL;

	//���l�̏�����
	alpha = 0.0f;

	//�^�C�g����Ԃ̏�����
	titleState = 0;

	//�^�C�g������
	titleTime = 0.0f;
}

//============================
//�`��
//============================
void TitleUI::render()
{
	if (titleState == 0)
	{
		UI[UI_PRESS]->render();
	}
	else
	{
		if (titleTime >FLOAT_LOGO_TIME)
		{
			UI[UI_LOGO]->render();
		}
		UI[UI_BAR]->render();
		UI[UI_MENU]->render();
		switch (selectState)
		{
		case TUTORIAL:
			UI[UI_INFO_TUTO]->render();
			break;

		case GAME:
			UI[UI_INFO_GAME]->render();
			break;

		case CREDIT:
			UI[UI_INFO_CREDIT]->render();
			break;

		case EXIT:
			UI[UI_INFO_EXIT]->render();
			break;

		}
		UI[UI_COPYRIGHT]->render();
	}
}

//============================
//�X�V
//============================
void TitleUI::update(Input *input,float flametime)
{
	if (input->wasKeyPressed(VK_RETURN) ||
		input->wasKeyPressed(VK_SPACE) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::SPECIAL_MAIN)
		)
	{
		titleState=1;
	}

	if (titleState == 1)
	{
		titleTime += flametime;
		if (alpha < 255 && titleTime>FLOAT_LOGO_TIME)
		{
			alpha += ALPHA_SPEED;
			UI[UI_LOGO]->setAlphaAllVertex(alpha);
		}
		else if(alpha>=255)
		{
			UI[UI_LOGO]->setAlphaAllVertex(255);
		}
	
		ringSE(input);

		if (selectState > EXIT)
		{
			selectState = TUTORIAL;
		}

		else if (selectState < TUTORIAL)
		{
			selectState = EXIT;
		}
		moveBar();
	}
}

//============================
//�I������
//============================
void TitleUI::uninitialize()
{
	//UI�����ׂďI��
	for (int i = 0; i < UI_MAX; i++)
	{
		delete UI[i];
	}
	
}

//===========================
//�I������Ԃ̎擾�֐�
//�߂�l�FselectState
//===========================
int TitleUI::getSelectState()
{
	return selectState;
}

//===========================
//�o�[�̈ړ��p�֐�
//===========================
void TitleUI::moveBar()
{
	switch (selectState)
	{
	case TUTORIAL:
		UI[UI_BAR]->setPosition(POSITION_BAR_TUTO);
		break;

	case GAME:
		UI[UI_BAR]->setPosition(POSITION_BAR_GAME);
		break;

	case CREDIT:
		UI[UI_BAR]->setPosition(POSITION_BAR_CREDIT);
		break;

	case EXIT:
		UI[UI_BAR]->setPosition(POSITION_BAR_EXIT);
		break;

	}

	UI[UI_BAR]->setVertex();
}

//===============================
//����炷����
//===============================
void TitleUI::ringSE(Input *input)
{
	if (input->wasKeyPressed(VK_DOWN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::DOWN) ||
		(input->getController()[inputNS::DINPUT_1P]->checkConnect() && input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().y>0))
	{
		//�{�^�����������x��SE�̂�炷����
		PLAY_PARAMETERS playParameters = { 0 };
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,SE_LIST::SE_Cursor, false,NULL,false,NULL };//SE��1�Ȗڂ̐ݒ�
		SoundInterface::SE->playSound(&playParameters);
		selectState++;
	}

	else if (input->wasKeyPressed(VK_UP) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::UP) ||
		(input->getController()[inputNS::DINPUT_1P]->checkConnect() && input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().y<0))
	{
		//�{�^�����������x��SE�̂�炷����
		PLAY_PARAMETERS playParameters = { 0 };
		//FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::HighPassFilter, 0.85f, 1.5f };//�t�B���^�[�̐ݒ�
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,SE_LIST::SE_Cursor, false,NULL,false,NULL };//SE��1�Ȗڂ̐ݒ�
		SoundInterface::SE->playSound(&playParameters);
		selectState--;
	}
}

