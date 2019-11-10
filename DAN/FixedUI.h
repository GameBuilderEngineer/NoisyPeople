//=================================
// �Œ肳��Ă���UI[FixedUI]
// ����W��
//=================================
#pragma once
//=================================
// �C���N���[�h
//=================================
#include "Timer.h"
#include "TimerFlame.h"
#include "UIButton.h"

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
	bool	     buttonFlag;    //�{�^��on off�p�t���O
public://�����o�[�֐�
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update();
	void uninitialize();
private:
};