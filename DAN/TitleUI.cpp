//============================
//[TitleUI.cpp]
//����W��
//============================

//============================
//�C���N���[�h
//============================
#include "TitleUI.h"

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

	//�I����Ԃ̏�����
	selectState = TUTORIAL;
}

//============================
//�`��
//============================
void TitleUI::render()
{
	UI[UI_LOGO]->render();
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

//============================
//�X�V
//============================
void TitleUI::update(Input *input)
{
	if (input->wasKeyPressed(VK_DOWN))
	{
		selectState++;
	}

	else if (input->wasKeyPressed(VK_UP))
	{
		selectState--;
	}

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