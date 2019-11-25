//==================================
//�{�^���t���[���N���X����[ButtonFlame.h]
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
namespace buttonFrameNS
{
	const int WIDTH_BUTTONFLAME = (185 * WINDOW_WIDTH / 1920);	//�{�^���t���[���̉���
	const int HEIGHT_BUTTONFLAME = (255 * WINDOW_HEIGHT / 1080);	//�{�^���t���[���̏c��
	const D3DXVECTOR3 POSITION_BUTTONFLAME01 = D3DXVECTOR3(100 * WINDOW_WIDTH / 1920, 870 * WINDOW_HEIGHT / 1080, 0);	//�{�^���t���[���̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_BUTTONFLAME02 = D3DXVECTOR3(1800 * WINDOW_WIDTH / 1920, 870 * WINDOW_HEIGHT / 1080, 0);	//�{�^���t���[���̕\���ʒu(��)
}

//==================================
//�N���X��`
//==================================
class ButtonFrame :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * buttonFrame;	//�X�v���C�g

public://�����o�[�֐�
	ButtonFrame();			//�R���X�g���N�^
	~ButtonFrame();			//�f�X�g���N�^
	void initialize(int type);		//������
	void render();			//�`��

};
