//==================================
//�{�^��UI����[UIButtoon.cpp]
//����@�W��
//==================================

//==================================
//�C���N���[�h
//==================================
#include "ButtonUI.h"

//==================================
// using�錾
//==================================
using namespace uiButtonNS;

//==================================
//�R���X�g���N�^
//==================================
ButtonUI::ButtonUI()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		button[i] = new Sprite;
	}
}

//==================================
//�f�X�g���N�^
//==================================
ButtonUI::~ButtonUI()
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		delete button[i];
	}
}

//==================================
//������
//==================================
void ButtonUI::initialize()
{
	//�����\���ʒu�̏�����
	BasicUI::assingPosition(POSITION_BUTTON);

	//�����T�C�Y�̏�����
	BasicUI::assingSize(WIDTH_BUTTON, HEIGHT_BUTTON);

	//�������l�̏�����
	BasicUI::assingColor(COLOR);

	//�e�N�X�`���̏�����
	BasicUI::initialize(button[SHOT], textureNS::reference(textureNS::UI_BUTTON_SHOT));
	BasicUI::initialize(button[JUMP], textureNS::reference(textureNS::UI_BUTTON_JUMP));
	BasicUI::initialize(button[VS], textureNS::reference(textureNS::UI_BUTTON_VISION));
	BasicUI::initialize(button[SV], textureNS::reference(textureNS::UI_BUTTON_SV));
	BasicUI::initialize(button[SHIFT], textureNS::reference(textureNS::UI_BUTTON_SHIFT));

}

//==================================
//P1�`��
//==================================
void ButtonUI::renderP1(int state)
{
	changeRenderP1(state);
}

//==================================
//P2�`��
//==================================
void ButtonUI::renderP2(int state)
{
	changeRenderP2(state);
}

//==================================
//�X�V
//����:�{�^�����g�p�\���ǂ���
//	  :�ǂ��̃{�^�������w��
//==================================
void ButtonUI::update(bool flag,int buttontype)
{
	changeAlpha(flag,buttontype);
	/*if (buttontype == SHIFT)
	{
		changeRender(flag);
	}*/
}

//==================================
//���l�̕ύX
//����:�{�^�����g�p�\���ǂ���
//==================================
void  ButtonUI::changeAlpha(bool flag,int buttontype)
{
	if (flag == true)//�g�p�\�̏ꍇ�͉���
	{
		BasicUI::changeAlpha(button[buttontype],ALPHA_MAX);
	}
	else//�g�p�s�̏ꍇ�͕s��
	{
		BasicUI::changeAlpha(button[buttontype],ALPHA_MINIMAM);
	}
}

//===================================
//�\���{�^���̕ύX
//===================================
void ButtonUI::changeRenderP1(int state)
{
	switch (state)
	{
	case STATE::VISION:
		button[SHIFT]->render();
		break;

	case STATE::SKY_VISION:
		button[SHIFT]->render();
		break;

	default:
		for (int i = 0; i < SHIFT; i++)
		{
			position.y = HEIGHT_BUTTON * i + POSITION_BUTTON.y;
			button[i]->setPosition(position);
			button[i]->setVertex();
			button[i]->render();
		}
		break;
	}
}

//===================================
//�\���{�^���̕ύX
//===================================
void ButtonUI::changeRenderP2(int state)
{
	switch (state)
	{
	case STATE::VISION:
		position.x = POSITION_BUTTON2.x;
		button[SHIFT]->setPosition(position);
		button[SHIFT]->setVertex();
		button[SHIFT]->render();
		break;

	case STATE::SKY_VISION:
		position.x = POSITION_BUTTON2.x;
		button[SHIFT]->setPosition(position);
		button[SHIFT]->setVertex();
		button[SHIFT]->render();
		break;

	default:
		for (int i = 0; i < SHIFT; i++)
		{
			position.x = POSITION_BUTTON2.x;
			position.y = HEIGHT_BUTTON * i + POSITION_BUTTON.y;
			button[i]->setPosition(position);
			button[i]->setVertex();
			button[i]->render();
		}
		break;
	}
}