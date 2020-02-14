#include "Equipment.h"



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
SWORD		PlayerSword;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEquipment(bool FirstInit)
{
	MOVEP *MoveP = GetMovePAdr();

	LPDIRECT3DDEVICE9 Device = getDevice();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	PlayerSword.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerSword.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerSword.Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	// ���߂ď�����
	if (FirstInit == true)
	{
		PlayerSword.Texture = NULL;
		PlayerSword.MaterialBuffer = NULL;
		PlayerSword.Mesh = NULL;
		PlayerSword.MaterialNum = 0;


		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(Model_MovePSword,	// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,							// ���b�V���̍쐬�I�v�V�������w��
			Device,										// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,										// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&PlayerSword.MaterialBuffer,				// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,										// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&PlayerSword.MaterialNum,					// D3DXMATERIAL�\���̂̐�
			&PlayerSword.Mesh)))						// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		{
			return E_FAIL;
		}


		// ���f���p�̃e�N�X�`���̃�������Ԃ�z��
		PlayerSword.Texture = (LPDIRECT3DTEXTURE9*)calloc(PlayerSword.MaterialNum, sizeof(LPDIRECT3DTEXTURE9));
		if (PlayerSword.Texture == NULL)
		{
			MessageBox(0, "Alloc PlayerSword Texture Memory Failed�I", "Error", 0);
			return E_FAIL;
		}

		// �e�N�X�`����ǂݍ���
		if (FAILED(SafeLoadModelTexture(PlayerSword.Texture, PlayerSword.MaterialBuffer, PlayerSword.MaterialNum, "PlayerSword")))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEquipment(void)
{
	int i = 0;

	// �e�N�X�`���̊J��
	for (i = 0; i < (int)PlayerSword.MaterialNum; i++)
	{
		SAFE_RELEASE(PlayerSword.Texture[i]);
	}
	// �}�e���A���̊J��
	SAFE_RELEASE(PlayerSword.MaterialBuffer);

	// ���b�V���̊J��
	SAFE_RELEASE(PlayerSword.Mesh);


	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEquipment(void)
{

	MOVEP *MoveP = GetMovePAdr();
	D3DXMATRIX PlayerSwordMatrix;

	PlayerSwordMatrix = GetBoneMatrix(MoveP->Animation, "mixamorig_LeftHand");

	PlayerSword.Pos.x = PlayerSwordMatrix._41 ;
	PlayerSword.Pos.y = PlayerSwordMatrix._42  ;
	PlayerSword.Pos.z = PlayerSwordMatrix._43 ;

	return;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEquipment(void)
{
	LPDIRECT3DDEVICE9 Device = getDevice();
	D3DXMATRIX ScaleMatrix, RotMatrix, TransMatrix;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 DefaultMat;
	D3DXMATRIX PlayerSwordMatrix;
	MOVEP *MoveP1 = GetMovePAdr();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&PlayerSword.WorldMatrix);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&ScaleMatrix, PlayerSword.Scale.x, PlayerSword.Scale.y, PlayerSword.Scale.z);
	D3DXMatrixMultiply(&PlayerSword.WorldMatrix, &PlayerSword.WorldMatrix, &ScaleMatrix);

	D3DXMatrixRotationQuaternion(&RotMatrix, &PlayerSword.Quaternion);
	D3DXMatrixMultiply(&PlayerSword.WorldMatrix, &PlayerSword.WorldMatrix, &RotMatrix);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&TransMatrix, PlayerSword.Pos.x, PlayerSword.Pos.y, PlayerSword.Pos.z);
	D3DXMatrixMultiply(&PlayerSword.WorldMatrix, &PlayerSword.WorldMatrix, &TransMatrix);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &PlayerSword.WorldMatrix);

	// ���݂̃}�e���A�����擾
	Device->GetMaterial(&DefaultMat);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)PlayerSword.MaterialBuffer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)PlayerSword.MaterialNum; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		Device->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		Device->SetTexture(0, PlayerSword.Texture[nCntMat]);

		// �`��
		PlayerSword.Mesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	Device->SetMaterial(&DefaultMat);

	return;
}

//=============================================================================
// �|�C���^���擾����
//=============================================================================
SWORD *GetSword(const char* Owner)
{
	if (strcmp(Owner, "MoveP1") == 0)
	{
		return &PlayerSword;
	}
	else
	{
		MessageBox(0, "Can't Find Sword Owner", "Error", 0);
		return NULL;
	}
}

//=============================================================================
// ���f���̃e�N�X�`����ǂݍ��ފ֐�
//=============================================================================
HRESULT SafeLoadModelTexture(LPDIRECT3DTEXTURE9	*Texture, LPD3DXBUFFER MaterialBufferconst, DWORD MaterialNum, const char* ErrorSrc)
{
	LPDIRECT3DDEVICE9 Device = getDevice();

	char Message[256];
	char TextureName[256];

	D3DXMATERIAL *pMtrls = (D3DXMATERIAL*)MaterialBufferconst->GetBufferPointer();

	for (int i = 0; i < (int)MaterialNum; i++)
	{
		Texture[i] = NULL;

		ZeroMemory(TextureName, sizeof(TextureName));
		sprintf_s(TextureName, "data/Model/%s", pMtrls[i].pTextureFilename);

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(Device,		// �f�o�C�X�ւ̃|�C���^
			TextureName,							// �t�@�C���̖��O
			&Texture[i]);							// �ǂݍ��ރ������[

	}

	return S_OK;
}
