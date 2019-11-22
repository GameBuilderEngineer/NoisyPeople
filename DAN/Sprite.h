//=============================================================================
// �X�v���C�g���� [Sprite.h]
//
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�� : 2019/6/8
//=============================================================================
#pragma once
#include <d3dx9.h>
#include "Base.h"

namespace SpriteNS
{
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

	const  int	NUM_VERTEX = 4;		// ���_��
	const  int	NUM_POLYGON = 2;	// �|���S����

	// ���_�i��]���j
	enum PIVOT
	{
		CENTER,						// ���S
		TOP_LEFT,					// ���㒸�_
		TOP_CENTER,					// �e���b�v�\���p���_
		TOP_RIGHT,					// �����A��]���邱�Ƃ͍l�����ĂȂ��ł�
		BOTTOM_LEFT,				// �������_�i���Ή��j
		BOTTOM_RIGHT,				// �E�����_�i���Ή��j
		PIVOT_MAX
	};

	// �Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\����
	struct Vertex2D
	{
		D3DXVECTOR3		vtx;		// ���_���W
		float			rhw;		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		D3DCOLOR		diffuse;	// ���ˌ�
		D3DXVECTOR2		uv;			// �e�N�X�`�����W
	};
}


//*****************************************************************************
// �X�v���C�g�����N���X
//*****************************************************************************
class Sprite 
{
private:
	//------
	// Data
	//------
	SpriteNS::Vertex2D	vertexWk[SpriteNS::NUM_VERTEX]; // ���_���i�[���[�N
	LPDIRECT3DTEXTURE9	texture;						// �e�N�X�`���ւ̃|�C���^
	int					pivot;							// ���_�i��]���j
	int					width;							// ����
	int					height;							// ����

	D3DXVECTOR3			position;						// �ʒu
	D3DXVECTOR3			rotation;						// ��]
	float				centralAngle;					// ���S���W����S���_�ւ̊p�x
	float				radius;							// ���a
	float				initialRadius;					// ���̔��a

	bool				onReadFile;						//���̃N���X���Ńt�@�C������e�N�X�`����ǂݍ��񂾂�

public:
	//--------
	// Method
	//--------
	// Regular use
	Sprite();
	~Sprite();
	void initialize(const char* fileName,										// �����������i�e�N�X�`�����t�@�C������ǂݍ��ށj
		int _pivot, int _width, int _height, D3DXVECTOR3 position,
		D3DXVECTOR3 rotation, D3DCOLOR color);
	void initialize(LPDIRECT3DTEXTURE9 _texture,								// �����������i�e�N�X�`�����|�C���^�œn���j
		int _pivot, int _width, int _height, D3DXVECTOR3 _position,
		D3DXVECTOR3 _rotation, D3DCOLOR color);
	void render();										// �`��

	// Function to set parameters
	HRESULT setTextureFromFile(const char* fileName);							// �e�N�X�`�����t�@�C������ǂݍ���
	void setTexturePointer(LPDIRECT3DTEXTURE9 _texture);						// �e�N�X�`�����|�C���^�ŃZ�b�g����
	void setUVCoord(D3DXVECTOR2 v0, D3DXVECTOR2 v1,
		D3DXVECTOR2 v2, D3DXVECTOR2 v3);										// UV���W��ݒ�
	void setSize(int sizeX, int sizeY);											// �T�C�Y��ݒ�
	void setVertex();															// ���_���W��ݒ�
	void setColor(D3DCOLOR v0, D3DCOLOR v1, D3DCOLOR v2, D3DCOLOR v3);			// ���_�J���[�𒸓_�ʂɐݒ�
	void setColorAllVertex(D3DCOLOR color);										// ���_�J���[���ꊇ�ݒ�
	void setAlpha(int v0, int v1, int v2, int v3);								// ���l�𒸓_�ʂɐݒ�
	void setAlphaAllVertex(int _alpha);											// ���l���ꊇ�ݒ�
	void setPosition(D3DXVECTOR3 _position);									// ���W��ݒ�
	void setRotation(D3DXVECTOR3 _rotation);									// ��]��ݒ�
	void setPivot(int _pivot);													// ��]����ݒ�
	void setRadius(float _radius);												// ���a��ݒ�i��]���S���Ȃ璸�_���W�ɔ��f�j

	// Getter
	SpriteNS::Vertex2D*	getVertexPointer();			// ���_���̃|�C���^���擾
	D3DXVECTOR3				getPosition();				// ���W�擾
	D3DXVECTOR3*			getPositionPointer();		// ���W�̃|�C���^���擾
	D3DXVECTOR3				getRotation();				// ��]���擾
	D3DXVECTOR3*			getRotationPointer();		// ��]�̃|�C���^���擾
	float					getInitialRadius();			// ���a�������l���擾
	LPDIRECT3DTEXTURE9		getTexture();				// �e�N�X�`�����擾
};
