//==================================
//�{�^��UI����[UIButtoon.cpp]
//����@�W��
//==================================

//==================================
//�C���N���[�h
//==================================
#include "UIButton.h"

//==================================
// using�錾
//==================================
using namespace uiButtonNS;

//==================================
//�R���X�g���N�^
//==================================
UiButton::UiButton()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		button[i] = new Sprite;
	}
}

//==================================
//�f�X�g���N�^
//==================================
UiButton::~UiButton()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		delete button[i];
	}
}

//==================================
//������
//==================================
void UiButton::initialize()
{
	//�����\���ʒu�̏�����
	BasicUI::assingPosition(POSITION_BUTTON);

	//�����T�C�Y�̏�����
	BasicUI::assingSize(WIDTH_BUTTON, HEIGHT_BUTTON);

	//�e�N�X�`���̏�����
	BasicUI::initialize(button[SV], textureNS::reference(textureNS::UI_BUTTON1));
	BasicUI::initialize(button[SHOT], textureNS::reference(textureNS::UI_BUTTON2));
	BasicUI::initialize(button[JUMP], textureNS::reference(textureNS::UI_BUTTON3));
	BasicUI::initialize(button[VISION], textureNS::reference(textureNS::UI_BUTTON4));
}

//==================================
//�`��
//==================================
void UiButton::render()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		position.x = -HEIGHT_BUTTON * i + POSITION_BUTTON.x;
		button[i]->setPosition(position);
		button[i]->setVertex();
		button[i]->render();
	}
}

//==================================
//�X�V
//==================================
void UiButton::update()
{

}