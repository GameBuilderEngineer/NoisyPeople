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
#include "UIButton.h"
#include "ButtonFrame.h"
#include "StatusFrame.h"

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
	UiButton   * buttonP1;		//�{�^��UI
	UiButton   * buttonP2;		//�{�^��UI
	ButtonFrame *buttonFrameP1;//�{�^���̃{�b�N�X
	ButtonFrame *buttonFrameP2;//�{�^���̃{�b�N�X
	StatusFrame *statusFrameP1;//HP�Ƃ��̃t���[��
	StatusFrame *statusFrameP2;//HP�Ƃ��̃t���[��
	//bool	     buttonFlag;    //�{�^��on off�p�t���O
	//int		 buttonType;	//�{�^���^�C�v�Ǘ��p
public://�����o�[�֐�
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update();
	void uninitialize();
private:
};