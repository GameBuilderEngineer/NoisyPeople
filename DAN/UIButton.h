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
	const D3DXVECTOR3 POSITION_BUTTON = D3DXVECTOR3(100, 900, 0);	//�{�^��UI�̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_BUTTON2 = D3DXVECTOR3(1800, 900, 0);	//�{�^��UI�̕\���ʒu(��)
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 100);
	const float ALPHA_MAX = 255.0f;
	const float ALPHA_MINIMAM = 100.0f;
}

//==================================
//�N���X��`
//==================================
class UiButton :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * button[uiButtonNS::VISION];	//�X�v���C�g
	float	alpha;							//���l

public://�����o�[�֐�
	UiButton();					//�R���X�g���N�^
	~UiButton();				//�f�X�g���N�^
	void initialize();			//������
	void renderP1();			//P1�`��
	void renderP2();			//P2�`��
	void update(bool flag, int buttontype);	//�X�V
	void  changeAlpha(bool flag, int buttontype);//���l�̕ύX
	

}; 
