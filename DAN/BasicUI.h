//================================
// ��bUI�N���X�̒�`[BasicUI.h]
// ����@�W��
//================================
#pragma once

//================================
// �C���N���[�h
//================================
#include"TextureLoader.h"
#include "Sprite.h"

//================================
// ���O���
//================================
namespace basicUiNS
{
	//�ړ��̎��
	enum MOVE_TYPE
	{
		slideRight,
		slideLeft,
		slideUp,
		slideDown,
	};

	const float SLIDE_SPEED = 20.0f;		//�X���C�h�C���̑��x
	const int	CHANGE_SIZE_SPEED = 20;		//�T�C�Y�ύX�̑��x
	const int   CHANGE_ALPHA_SPEED = 10;		//���l�̕ύX���x
}

//================================
//�N���X��`
//================================
class BasicUI
{
public://�����o�[�ϐ�
	D3DXVECTOR3 position;	//�\���ʒu
	D3DCOLOR	color;		//�F
	int widthSize;			//���̃T�C�Y
	int heightSize;			//�����T�C�Y
	float alphaValue;		//���l
	// uv���W�p�̕ϐ�
	D3DXVECTOR2 uvCoord01;
	D3DXVECTOR2 uvCoord02;
	D3DXVECTOR2 uvCoord03;
	D3DXVECTOR2 uvCoord04;

private:
public://�����o�[�֐�
	BasicUI();				//�R���X�g���N�^
	~BasicUI();				//�f�X�g���N�^
	void	changePostion(int movetype);//�ʒu�ύX	
	void	increaseAlpha();//�����x����
	void	decreaseAlpha();//�����x����
	void	expansionWidthSize();//���T�C�Y�̊g��
	void	expansionHeightSize();//�c�T�C�Y�̊g��
	void	reductionWidthSize();//���T�C�Y�̏k��
	void	reductionHeightSize();//�c�T�C�Y�̏k��

private:
};