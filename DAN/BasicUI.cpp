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
	rotation = D3DXVECTOR3(0, 0, 0);			//��]
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
//������
//==================================
void BasicUI::initialize(Sprite *sprite, LPDIRECT3DTEXTURE9 *fileName)
{
	sprite->initialize(
		*fileName					//�e�N�X�`��
		, SpriteNS::CENTER			//���S
		, widthSize					//����
		, heightSize				//�c��
		, position					//�\���ʒu
		, rotation 					//��]
		, color);					//�F
}

//==================================
//�\���ʒu�̑��
//==================================
void BasicUI::assingPosition(D3DXVECTOR3 pos)
{
	position = pos;
}

//==================================
//��]�̑��
//==================================
void BasicUI::assingRotation(D3DXVECTOR3 rot)
{
	rotation = rot;
}

//==================================
//�F�̑��
//==================================
void BasicUI::assingColor(D3DCOLOR col)
{
	color = col;
}

//==================================
//�T�C�Y�̑��
//==================================
void BasicUI::assingSize(int widh, int height)
{
	widthSize = widh;
	heightSize = height;
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
	if (alphaValue >= 255)
	{
		alphaValue += CHANGE_ALPHA_SPEED;
		color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
	}
}

//=================================
//�����x�̌���
//=================================
void BasicUI::decreaseAlpha()
{
	if (alphaValue >= 0)
	{
		alphaValue -= CHANGE_ALPHA_SPEED;
		color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
	}
}

//=================================
//���T�C�Y�̊g��
//����:�T�C�Y�̕ω����x
//=================================
void BasicUI::expansionWidthSize(int speed)
{
	widthSize += speed;
}

//=================================
//�c�T�C�Y�̕ύX
//����:�T�C�Y�̕ω����x
//=================================
void BasicUI::expansionHeightSize(int speed)
{
	widthSize -= speed;
}

//=================================
//���T�C�Y�̊g��
//����:�T�C�Y�̕ω����x
//=================================
void BasicUI::reductionWidthSize(int speed)
{
	heightSize += speed;
}

//=================================
//�c�T�C�Y�̏k��
//����:�T�C�Y�̕ω����x
//=================================
void BasicUI::reductionHeightSize(int speed)
{
	heightSize -= speed;
}

