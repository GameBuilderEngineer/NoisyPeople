//=================================
// �Œ肳��Ă���UI[FixedUI.h]
// ����W��
//=================================
#pragma once

//=================================
// �C���N���[�h
//=================================
#include "Timer.h"
#include "TimerFrame.h"
#include "StatusFrame.h"
#include "CharaIcon.h"

//=================================
// ���O���
//=================================
namespace fixedUiNS
{
	
}

//=================================
//�N���X��`
//=================================
class FixedUI
{
public://�����o�[�ϐ�
private:
	Timer * timer;				//�^�C�}�[
	TimerFlame * timerFlame;	//�^�C�}�[�t���[��
	StatusFrame *statusFrameP1;//HP�Ƃ��̃t���[��
	StatusFrame *statusFrameP2;//HP�Ƃ��̃t���[��
	
public://�����o�[�֐�
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update(float gametime);
	void uninitialize();
	Timer* getTimer() { return timer; }
	TimerFlame* getTimerFlame() { return timerFlame; }
private:
};