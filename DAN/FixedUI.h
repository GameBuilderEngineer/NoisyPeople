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
	Timer * timer;
	TimerFlame * timerFlame;	//�^�C�}�[�t���[��

public://�����o�[�֐�
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update();
	void uninitialize();
private:
};