//==================================
// ��bUI�N���X[UImanager.cpp]
// ����@�W��
//==================================

//==================================
// �C���N���[�h
//==================================
#include "BasicUI.h"

//==================================
// [using�錾]
//==================================
using namespace basicUiNS;

//==================================
// �R���X�g���N�^
//==================================
BasicUI::BasicUI()
{
	//�e���l�̏�����
	position = D3DXVECTOR3(0, 0, 0);			//�\���ʒu
	color = D3DCOLOR_RGBA(255,255, 255, 255);	//�F
	widthSize = 0;								//����
	heightSize = 0;								//����
	//�e�N�X�`�����W
	uvCoord01 = D3DXVECTOR2(0.0, 0.0);
	uvCoord02 = D3DXVECTOR2(1.0, 0.0);
	uvCoord03 = D3DXVECTOR2(0.0, 1.0);
	uvCoord04 = D3DXVECTOR2(1.0, 1.0);
}

//==================================
//�f�X�g���N�^
//==================================
BasicUI::~BasicUI()
{

}

//==================================
//���W�̕ύX
//�����F�ړ��̎��
//==================================
void BasicUI::changePostion(int movetype)
{
	switch (movetype)
	{
	case slideRight:
		position.x += SLIDE_SPEED;
		break;

	case slideLeft:
		position.x -= SLIDE_SPEED;
		break;

	case slideUp:
		position.y -= SLIDE_SPEED;
		break;

	case slideDown:
		position.y += SLIDE_SPEED;
		break;

	default:
		break;
	}
}

//=================================
//�����x�̑���
//=================================
void BasicUI::increaseAlpha()
{
	alphaValue += CHANGE_ALPHA_SPEED;
	color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
}

//=================================
//�����x�̌���
//=================================
void BasicUI::decreaseAlpha()
{
	alphaValue -= CHANGE_ALPHA_SPEED;
	color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
}

//=================================
//���T�C�Y�̊g��
//=================================
void BasicUI::expansionWidthSize()
{
	widthSize += CHANGE_SIZE_SPEED;
}

//=================================
//�c�T�C�Y�̕ύX
//=================================
void BasicUI::expansionHeightSize()
{
	widthSize -= CHANGE_SIZE_SPEED;
}

//=================================
//���T�C�Y�̊g��
//=================================
void BasicUI::reductionWidthSize()
{
	heightSize += CHANGE_SIZE_SPEED;
}

//=================================
//�c�T�C�Y�̏k��
//=================================
void BasicUI::reductionHeightSize()
{
	heightSize -= CHANGE_SIZE_SPEED;
}

