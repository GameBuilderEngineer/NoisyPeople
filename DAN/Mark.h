//==================================
//�}�[�J�[�N���X[Mark.h]
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
namespace markNS
{
	const int WIDTH_MARK = (512 * WINDOW_WIDTH / 1920);	//�L�����A�C�R���̉���
	const int HEIGHT_MARK = (32 * WINDOW_HEIGHT / 1080);	//�L�����A�C�R���̏c��
	const D3DXVECTOR3 POSITION_MARK = D3DXVECTOR3(348 * WINDOW_WIDTH / 1920, 85 * WINDOW_HEIGHT / 1080, 0);	//�L�����A�C�R���̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_MARK_P2 = D3DXVECTOR3(1800 * WINDOW_WIDTH / 1920, 85 * WINDOW_HEIGHT / 1080, 0);	//�L�����A�C�R���̕\���ʒu(��)
}

//==================================
//�N���X��`
//==================================
class Mark :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * mark;	//�X�v���C�g

public://�����o�[�֐�
	Mark();			//�R���X�g���N�^
	~Mark();		//�f�X�g���N�^
	void initialize(int playerType);//������
	void render();			//�`��

};

