//==================================
//�{�^��UI����[ButtonUI.h]
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
namespace uiButtonNS
{
	enum BUTTON_UI_TYPE
	{
		SHOT,
		JUMP,
		VISION,
		SV,
		SHIFT,
		TYPE_MAX,
	};
	const int WIDTH_BUTTON = (80 * WINDOW_WIDTH / 1920);	//�{�^��UI�̉���
	const int HEIGHT_BUTTON = (80 * WINDOW_HEIGHT / 1080);	//�{�^��UI�̏c��
	const D3DXVECTOR3 POSITION_BUTTON = D3DXVECTOR3(80 * WINDOW_WIDTH / 1920, 700 * WINDOW_HEIGHT / 1080, 0);	//�{�^��UI�̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_BUTTON2 = D3DXVECTOR3(1800 * WINDOW_WIDTH / 1920, 700 * WINDOW_HEIGHT / 1080, 0);	//�{�^��UI�̕\���ʒu(��)
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 100);
	const float ALPHA_MAX = 255.0f;
	const float ALPHA_MINIMAM = 100.0f;
}

//==================================
//�N���X��`
//==================================
class ButtonUI :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * button[uiButtonNS::VISION];	//�X�v���C�g
	float	alpha;							//���l

public://�����o�[�֐�
	ButtonUI();					//�R���X�g���N�^
	~ButtonUI();				//�f�X�g���N�^
	void initialize();			//������
	void renderP1();			//P1�`��
	void renderP2();			//P2�`��
	void update(bool flag, int buttontype);	//�X�V
	void  changeAlpha(bool flag, int buttontype);//���l�̕ύX
	void  changeRender(bool visionFlag);

}; 
