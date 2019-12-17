#pragma once

#include "movep.h"
#include "D3DXAnimation.h"
#include "Input.h"
#include "Direct3D9.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �ǂݍ��ރ��f����
#define	Model_MovePSword	"07 Model File/Gun.X"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���\����
typedef struct
{
	D3DXVECTOR3			Pos;				// ���f���̈ʒu
	D3DXVECTOR3			Rot;				// ���f���̌���(��])
	D3DXVECTOR3			Scale;				// ���f���̑傫��(�X�P�[��)
	LPDIRECT3DTEXTURE9	*Texture;			// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			Mesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		MaterialBuffer;		// �}�e���A�����ւ̃|�C���^
	DWORD				MaterialNum;		// �}�e���A�����̐�
	D3DXMATRIX			WorldMatrix;		// ���[���h�}�g���b�N�X
	int					EffectID;			// �G�t�F�N�g�ԍ�
	D3DXQUATERNION		Quaternion;			// ��]

}SWORD;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitEquipment(bool FirstInit);
// �I������
void UninitEquipment(void);
// �X�V����
void UpdateEquipment(void);
// �`�揈��
void DrawEquipment(void);
// ���̃|�C���^���擾����
SWORD *GetSword(const char* Owner);

HRESULT SafeLoadModelTexture(LPDIRECT3DTEXTURE9	*Texture, LPD3DXBUFFER MaterialBufferconst, DWORD MaterialNum, const char* ErrorSrc);

