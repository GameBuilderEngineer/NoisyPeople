//================================
// �Œ肳�ꂽUI[FixedUI.cpp]
// ����W��
//================================

//================================
//�C���N���[�h
//================================
#include "FixedUI.h"

//================================
// using�錾
//================================
using namespace fixedUiNS;

//================================
// �R���X�g���N�^
//================================
FixedUI::FixedUI()
{
	timer = new Timer;
	timerFlame = new TimerFlame;
	buttonP1 = new UiButton;
	buttonP2 = new UiButton;
	buttonFrameP1 = new ButtonFrame;
	buttonFrameP2 = new ButtonFrame;
	statusFrameP1 = new StatusFrame;
	statusFrameP2 = new StatusFrame;
}

//================================
// �f�X�g���N�^
//================================
FixedUI::~FixedUI()
{
	delete timer;
	delete timerFlame;
	delete buttonP1;
	delete buttonP2;
	delete buttonFrameP1;
	delete buttonFrameP2;
	delete statusFrameP1;
	delete statusFrameP2;
}

//================================
// ������
//================================
void FixedUI::initialize()
{
	timer->initialize();
	timerFlame->initialize();
	buttonP1->initialize();
	buttonP2->initialize();
	buttonFrameP1->initialize(basicUiNS::P1);
	buttonFrameP2->initialize(basicUiNS::P2);
	statusFrameP1->initialize(basicUiNS::P1);
	statusFrameP2->initialize(basicUiNS::P2);
	
}

//================================
// �`��
//================================
void FixedUI::render()
{
	timerFlame->render();
	timer->render();
	buttonFrameP1->render();
	buttonFrameP2->render();
	buttonP1->renderP1();
	buttonP2->renderP2();
	statusFrameP1->render();
	statusFrameP2->render();
}

//================================
// �X�V
//================================
void FixedUI::update()
{
	//buttonFlag = Getflag()//����Ȋ֐����p�ӂ����H
	timer->update();
	//�}�W�b�N�i���o�[�����͕ϐ���p�ӗ\��
	buttonP1->update(0,1);
	buttonP2->update(1,1);
}

//================================
// �I��
//================================
void FixedUI::uninitialize()
{
	
}

