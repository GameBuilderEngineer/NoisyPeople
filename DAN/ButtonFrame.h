//==================================
//�{�^���t���[���N���X����[ButtonFlame.h]
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
namespace buttonFrameNS
{
	const int WIDTH_BUTTONFLAME = (185);	//�{�^���t���[���̉���
	const int HEIGHT_BUTTONFLAME = (255);	//�{�^���t���[���̏c��
	const D3DXVECTOR3 POSITION_BUTTONFLAME01 = D3DXVECTOR3(100, 870, 0);	//�{�^���t���[���̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_BUTTONFLAME02 = D3DXVECTOR3(1800, 870, 0);	//�{�^���t���[���̕\���ʒu(��)
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
