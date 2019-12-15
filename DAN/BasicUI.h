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

	enum PLAYER_TYPE
	{
		P1,
		P2,
		PLAYER_TYPE_MAX,
	};


	const float SLIDE_SPEED = 20.0f;		//�X���C�h�C���̑��x
	const int	CHANGE_SIZE_SPEED = 20;		//�T�C�Y�ύX�̑��x
	const float   CHANGE_ALPHA_SPEED = 155.0f;		//���l�̕ύX���x
	const float CHANGE_UV_SPEED = 0.1f;	//UV���W�̕ω����x
}

//================================
//�N���X��`
//================================
class BasicUI
{
public://�����o�[�ϐ�
	D3DXVECTOR3 position;	//�\���ʒu
	D3DXVECTOR3 rotation;	//��]
	D3DCOLOR	color;		//�F
	int widthSize;			//���̃T�C�Y
	int heightSize;			//�����T�C�Y
	float alphaValue;		//���l
	int	  pivot;			//���S
	// uv���W�p�̕ϐ�
	D3DXVECTOR2 uvCoord01;
	D3DXVECTOR2 uvCoord02;
	D3DXVECTOR2 uvCoord03;
	D3DXVECTOR2 uvCoord04;

private:
public://�����o�[�֐�
	BasicUI();//�R���X�g���N�^
	~BasicUI();//�f�X�g���N�^
	void    initialize(Sprite *sprite, LPDIRECT3DTEXTURE9 *fileName);//������
	void	assingPosition(D3DXVECTOR3 pos);//�\���ʒu�̑��
	void	assingRotation(D3DXVECTOR3 rot);//��]�̑��
	void	assingColor(D3DCOLOR col);//�F�̑��
	void	assingSize(int widh, int height);//�T�C�Y�̑��
	void    assingUV(D3DXVECTOR2 uvCoord1, D3DXVECTOR2 uvCoord2, D3DXVECTOR2 uvCoord3, D3DXVECTOR2 uvCoord4);//UV���W�̑��
	void	changePivot(int pivotTyp);//���S��ύX
	void	changePostion(Sprite *sprite,int movetype);//�ʒu�ύX	
	void	changeAlpha(Sprite *sprite,float alpha);//�����x�̕ύX
	void	decreaseAlpha(Sprite *sprite,float alpha);//�����x����
	void	expansionWidthSize(Sprite *sprite,int speed);//���T�C�Y�̊g��
	void	expansionHeightSize(Sprite *sprite,int speed);//�c�T�C�Y�̊g��
	void	reductionWidthSize(Sprite *sprite,int speed);//���T�C�Y�̏k��
	void	reductionHeightSize(Sprite *sprite,int speed);//�c�T�C�Y�̏k��
	void    changeUV(Sprite *sprite,D3DXVECTOR2 uv01, D3DXVECTOR2 uv02, D3DXVECTOR2 uv03, D3DXVECTOR2 uv04);//UV���W�̕ω�
	void	changeWhidthSize(Sprite *sprite, int size);//�����̕ω�
	void	changeColor(Sprite *sprite, D3DCOLOR col);//�F�̕ύX
private:
};