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

	//�������l�̏�����
	BasicUI::assingColor(COLOR);

	//�e�N�X�`���̏�����
	BasicUI::initialize(button[SV], textureNS::reference(textureNS::UI_BUTTON1));
	BasicUI::initialize(button[SHOT], textureNS::reference(textureNS::UI_BUTTON2));
	BasicUI::initialize(button[JUMP], textureNS::reference(textureNS::UI_BUTTON3));
	BasicUI::initialize(button[VISION], textureNS::reference(textureNS::UI_BUTTON4));

}

//==================================
//P1�`��
//==================================
void UiButton::renderP1()
{
	//1P�p�̕\��
	for (int i = 0; i < TYPE_MAX; i++)
	{
		position.y = -HEIGHT_BUTTON * i + POSITION_BUTTON.y;
		button[i]->setPosition(position);
		button[i]->setVertex();
		button[i]->render();
	}
}

//==================================
//P2�`��
//==================================
void UiButton::renderP2()
{
	//2P�p�̕\��
	for (int i = 0; i < TYPE_MAX; i++)
	{
		position.x = POSITION_BUTTON2.x;
		position.y = -HEIGHT_BUTTON * i + POSITION_BUTTON.y;
		button[i]->setPosition(position);
		button[i]->setVertex();
		button[i]->render();
	}
}

//==================================
//�X�V
//����:�{�^�����g�p�\���ǂ���
//	  :�ǂ��̃{�^�������w��
//==================================
void UiButton::update(bool flag,int buttontype)
{
	changeAlpha(flag);
	button[buttontype]->setAlphaAllVertex(alphaValue);
	button[buttontype]->render();
}

//==================================
//���l�̕ύX
//����:�{�^�����g�p�\���ǂ���
//==================================
void  UiButton::changeAlpha(bool flag)
{
	if (flag == true)//�g�p�\�̏ꍇ�͉���
	{
		BasicUI::increaseAlpha(ALPHA_MAX);
	}
	else//�g�p�s�̏ꍇ�͕s��
	{
		BasicUI::increaseAlpha(ALPHA_MINIMAM);
	}
}