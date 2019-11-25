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
	buttonP2 = new ButtonUI;
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
	buttonP2->renderP2();
	statusFrameP1->render();
	statusFrameP2->render();
}

//================================
// �X�V
//================================
void FixedUI::update(float gametime)
{
	timer->update(gametime);
}

//================================
// �I��
//================================
void FixedUI::uninitialize()
{
	
}

