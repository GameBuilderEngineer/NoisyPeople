//==================================
//�L�����A�C�R���N���X[CharaIcon.h]
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
namespace charaIconNS
{
	const int WIDTH_ICON = (123 * WINDOW_WIDTH / 1920);	//�L�����A�C�R���̉���
	const int HEIGHT_ICON = (123 * WINDOW_HEIGHT / 1080);	//�L�����A�C�R���̏c��
	const D3DXVECTOR3 POSITION_ICON = D3DXVECTOR3(85 * WINDOW_WIDTH / 1920, 96 * WINDOW_HEIGHT / 1080, 0);	//�L�����A�C�R���̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_ICON_P2 = D3DXVECTOR3(1840 * WINDOW_WIDTH / 1920, 96 * WINDOW_HEIGHT / 1080, 0);	//�L�����A�C�R���̕\���ʒu(��)
}

//==================================
//�N���X��`
//==================================
class CharaIcon :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * charaIcon;	//�X�v���C�g

public://�����o�[�֐�
	CharaIcon();			//�R���X�g���N�^
	~CharaIcon();			//�f�X�g���N�^
	void initialize(int playerType);//������
	void render();			//�`��

};
