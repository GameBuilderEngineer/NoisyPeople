//==================================
//�^�C�}�[�t���[���N���X����[TimerFlame.h]
//����@�W��
//==================================
#pragma once

//==================================
//�C���N���[�h
//==================================
#include "BasicUI.h"

//==================================
//���O���
//==================================
namespace timerFlameNS
{
	const int WIDTH_TIMERFLAME = (1920);	//�^�C�}�[�t���[���̉���
	const int HEIGHT_TIMERFLAME = (169);	//�^�C�}�[�t���[���̏c��
	const D3DXVECTOR3 POSITION_TIMERFLAME = D3DXVECTOR3(960, 84, 0);	//�^�C�}�[�t���[���̕\���ʒu(��)
	const D3DXVECTOR3 ROTATION_TIMERFLAME = D3DXVECTOR3(0, 0, 0);		//��]
	const D3DCOLOR COLOR_TIMERFLAME = D3DCOLOR_RGBA(255, 255, 255, 255);//�F
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