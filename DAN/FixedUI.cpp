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

