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
}

//================================
// �`��
//================================
void FixedUI::render()
{
	timerFlame->render();
	timer->render();
	buttonP1->renderP1();
	buttonP2->renderP2();
	
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

