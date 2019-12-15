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
	pivot = SpriteNS::CENTER;
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
		, pivot						//���S
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
//UV���W�̑��
//==================================
void BasicUI::assingUV(D3DXVECTOR2 uvCoord1, D3DXVECTOR2 uvCoord2, D3DXVECTOR2 uvCoord3, D3DXVECTOR2 uvCoord4)
{
	uvCoord01 = uvCoord1;
	uvCoord02 = uvCoord2;
	uvCoord03 = uvCoord3;
	uvCoord04 = uvCoord4;
}

//==================================
// ���S�ʒu�̑��
//==================================
void BasicUI::changePivot(int pivotType)
{
	switch (pivotType)
	{
	case SpriteNS::TOP_LEFT:
		pivot = SpriteNS::TOP_LEFT;
		break;
	case SpriteNS::TOP_RIGHT:
		pivot = SpriteNS::TOP_RIGHT;
		break;
	default:
		pivot = SpriteNS::TOP_CENTER;
		break;
	}
	
}

//==================================
//���W�̕ύX
//�����F�ړ��̎��
//==================================
void BasicUI::changePostion(Sprite *sprite,int movetype)
{
	switch (movetype)
	{
	case slideRight:
		position.x += SLIDE_SPEED;
		sprite->setPosition(position);
		sprite->setVertex();
		sprite->render();
		break;

	case slideLeft:
		position.x -= SLIDE_SPEED;
		sprite->setPosition(position);
		sprite->setVertex();
		sprite->render();
		break;

	case slideUp:
		position.y -= SLIDE_SPEED;
		sprite->setPosition(position);
		sprite->setVertex();
		sprite->render();
		break;

	case slideDown:
		position.y += SLIDE_SPEED;
		sprite->setPosition(position);
		sprite->setVertex();
		sprite->render();
		break;

	default:
		break;
	}
}

//=================================
//�����x�̕ύX
//=================================
void BasicUI::changeAlpha(Sprite *sprite,float alpha)
{

	alphaValue = alpha;
	color = D3DCOLOR_RGBA(255, 255, 255, (int)alphaValue);
	sprite->setAlphaAllVertex(alphaValue);
	sprite->render();
	
}

//=================================
//�����x�̌���
//=================================
void BasicUI::decreaseAlpha(Sprite *sprite,float alpha)
{
	
}

//=================================
//���T�C�Y�̊g��
//����:�T�C�Y�̕ω����x
//=================================
void BasicUI::expansionWidthSize(Sprite *sprite,int speed)
{
	widthSize += speed;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//=================================
//�c�T�C�Y�̕ύX
//����:�T�C�Y�̕ω����x
//=================================
void BasicUI::expansionHeightSize(Sprite *sprite,int speed)
{
	heightSize -= speed;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//=================================
//���T�C�Y�̊g��
//����:�T�C�Y�̕ω����x
//=================================
void BasicUI::reductionWidthSize(Sprite *sprite,int speed)
{
	widthSize -= speed;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//=================================
//�c�T�C�Y�̏k��
//����:�T�C�Y�̕ω����x
//=================================
void BasicUI::reductionHeightSize(Sprite *sprite,int speed)
{
	heightSize -= speed;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//=================================
//UV���W�̕ω�
//=================================
void BasicUI::changeUV(Sprite *sprite,D3DXVECTOR2 uv01, D3DXVECTOR2 uv02, D3DXVECTOR2 uv03, D3DXVECTOR2 uv04)
{
	
	sprite->setUVCoord(uv01, uv02, uv03, uv04);
	sprite->setVertex();
	sprite->render();
}

//=================================
//���T�C�Y�̕ω�
//=================================
void BasicUI::changeWhidthSize(Sprite *sprite, int size)
{
	widthSize = size;
	sprite->setSize(widthSize, heightSize);
	sprite->setVertex();
	sprite->render();
}

//==================================
//�F�̕ύX
//==================================
void BasicUI::changeColor(Sprite *sprite, D3DCOLOR col)
{
	color = col;
	sprite->setColorAllVertex(color);
	sprite->setVertex();
	sprite->render();
}