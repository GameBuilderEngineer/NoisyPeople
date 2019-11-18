//==================================
//�^�C�}�[�t���[���N���X����[TimerFlame.h]
//����@�W��
//==================================
#pragma once

//==================================
//�C���N���[�h
//==================================
#include "BasicUI.h"
#include "VirtualController.h"

//==================================
//���O���
//==================================
namespace timerFlameNS
{
	const int WIDTH_TIMERFLAME = (1920 * WINDOW_WIDTH / 1920);	//�^�C�}�[�t���[���̉���
	const int HEIGHT_TIMERFLAME = (169 * WINDOW_HEIGHT / 1080);	//�^�C�}�[�t���[���̏c��
	const D3DXVECTOR3 POSITION_TIMERFLAME = D3DXVECTOR3(960 * WINDOW_WIDTH / 1920, 84 * WINDOW_HEIGHT / 1080, 0);	//�^�C�}�[�t���[���̕\���ʒu(��)
}

//==================================
//�N���X��`
//==================================
class TimerFlame :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * timerFlame;	//�X�v���C�g

public://�����o�[�֐�
	TimerFlame();			//�R���X�g���N�^
	~TimerFlame();			//�f�X�g���N�^
	void initialize();		//������
	void render();			//�`��

};