//=================================
// �Œ肳��Ă���UI[FixedUI]
// ����W��
//=================================
#pragma once

//=================================
// �C���N���[�h
//=================================
#include "Timer.h"
#include "TimerFrame.h"
#include "ButtonUI.h"
#include "ButtonFrame.h"
#include "StatusFrame.h"
#include "Guage.h"
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
	ButtonUI   * buttonP2;		//�{�^��UI
	ButtonFrame *buttonFrameP1;//�{�^���̃{�b�N�X
	ButtonFrame *buttonFrameP2;//�{�^���̃{�b�N�X
	StatusFrame *statusFrameP1;//HP�Ƃ��̃t���[��
	StatusFrame *statusFrameP2;//HP�Ƃ��̃t���[��
	Guage		*guage;		   //�Q�[�W
	
public://�����o�[�֐�
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update(float gametime);
	void uninitialize();
private:
};