//==================================
//�{�^��UI����[TimerFlame.h]
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
namespace uiButtonNS
{
	enum BUTTON_UI_TYPE
	{
		SV,
		SHOT,
		JUMP,
		VISION,
		TYPE_MAX,
	};
	const int WIDTH_BUTTON = (140);	//�{�^��UI�̉���
	const int HEIGHT_BUTTON = (40);	//�{�^��UI�̏c��
	const D3DXVECTOR3 POSITION_BUTTON = D3DXVECTOR3(100, 84, 0);	//�{�^��UI�̕\���ʒu(��)
}

//==================================
//�N���X��`
//==================================
class UiButton :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * button[uiButtonNS::VISION];	//�X�v���C�g

public://�����o�[�֐�
	UiButton();			//�R���X�g���N�^
	~UiButton();			//�f�X�g���N�^
	void initialize();		//������
	void render();			//�`��
	void update();			//�X�V

}; 
