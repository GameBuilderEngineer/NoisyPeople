//==================================
//�{�^���t���[������[ButtonFlame.cpp]
//����@�W��
//==================================

//==================================
//�C���N���[�h
//==================================
#include "ButtonFrame.h"

//==================================
// using�錾
//==================================
using namespace buttonFrameNS;

//==================================
//�R���X�g���N�^
//==================================
ButtonFrame::ButtonFrame()
{
	//�X�v���C�g�̍쐬
	buttonFrame = new Sprite;
}

//==================================
//�f�X�g���N�^
//==================================
ButtonFrame::~ButtonFrame()
{
	delete buttonFrame;
}

//==================================
//������
//����:�v���C���[�^�C�v
//==================================
void ButtonFrame::initialize(int type)
{
	//�����\���ʒu�̑��
	if (type == basicUiNS::P1)
	{
		BasicUI::assingPosition(POSITION_BUTTONFLAME01);
	}
	else 
	{
		BasicUI::assingPosition(POSITION_BUTTONFLAME02);
	}
	//�����T�C�Y�̑��
	BasicUI::assingSize(WIDTH_BUTTONFLAME, HEIGHT_BUTTONFLAME);
	//�e�N�X�`���̏�����
	BasicUI::initialize(buttonFrame, textureNS::reference(textureNS::UI_BUTTON_BOX));
}

//=================================
//�`��
//=================================
void ButtonFrame::render()
{
	buttonFrame->render();
}
