//==================================
//�L�����A�C�R���N���X[CharaIcon.h]
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
namespace charaIconNS
{
	const int WIDTH_ICON = (123);	//�L�����A�C�R���̉���
	const int HEIGHT_ICON = (123);	//�L�����A�C�R���̏c��
	const D3DXVECTOR3 POSITION_ICON = D3DXVECTOR3(85, 96, 0);	//�L�����A�C�R���̕\���ʒu(��)
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
	void initialize();		//������
	void render();			//�`��

};
