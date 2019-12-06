//=============================================================================
//
// ���f������ [model.cpp]
// Author : 
//
//=============================================================================
#include "model.h"
#include "input.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_CAR			"data/MODEL/airplane000.x"						// �ǂݍ��ރ��f����
#define	MODEL_CAR2			"data/MODEL/ball.x"								// �ǂݍ��ރ��f����
#define TEXTURE_FILENAME	"data/TEXTURE/�J�[�r�[.dds"						// �ǂݍ��ރe�N�X�`��
#define TEXTURE_FILENAME2	"data/TEXTURE/starfy.dds"						// �ǂݍ��ރe�N�X�`��

#define	POS_CAMERA_P_X		(0.0f)											// �J�������_�̏����ʒu(X���W)
#define	POS_CAMERA_P_Y		(10.0f)											// �J�������_�̏����ʒu(Y���W)
#define	POS_CAMERA_P_Z		(-10.0f)										// �J�������_�̏����ʒu(Z���W)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z			(1.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(1000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE			(5.0f)											// �ړ���
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)								// ��]��

#define	NUM_LIGHT			(3)												// ���C�g�̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void InitLight(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureModel;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pD3DTextureModel2;	// �e�N�X�`���ւ̃|�C���^2�i�C���^�[�t�F�[�X�j
LPD3DXMESH			g_pD3DXMeshModel;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatModel;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatModel;			// �}�e���A�����̐�

LPD3DXMESH			g_pD3DXMeshMask;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatMask;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatMask;			// �}�e���A�����̐�


D3DXVECTOR3			g_posModel;				// ���f���̈ʒu
D3DXVECTOR3			g_rotModel;				// ���f���̌���(��])
D3DXVECTOR3			g_sclModel;				// ���f���̑傫��(�X�P�[��)

D3DXVECTOR3			g_posMask;				// ���f���̈ʒu
D3DXVECTOR3			g_rotMask;				// ���f���̌���(��])
D3DXVECTOR3			g_sclMask;				// ���f���̑傫��(�X�P�[��)


D3DXVECTOR3			g_posCameraP;			// �J�����̎��_
D3DXVECTOR3			g_posCameraR;			// �J�����̒����_
D3DXVECTOR3			g_vecCameraU;			// �J�����̏�����x�N�g��

D3DXMATRIX			g_mtxView;				// �r���[�}�g���b�N�X
D3DXMATRIX			g_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX			g_mtxWorld;				// ���[���h�}�g���b�N�X

D3DLIGHT9			g_aLight[NUM_LIGHT];	// ���C�g���


//=============================================================================
// ����������
//=============================================================================
HRESULT InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posModel = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclModel = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// ���X�P�[����������Ɩ@�����Ԃ�����̂Ń��C�g���@�\���Ȃ��Ȃ�

	g_posMask = D3DXVECTOR3(0.0f,11.0f, 0.0f);
	g_rotMask = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclMask = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// ���X�P�[����������Ɩ@�����Ԃ�����̂Ń��C�g���@�\���Ȃ��Ȃ�


	g_pD3DTextureModel = NULL;
	g_pD3DXMeshModel = NULL;
	g_pD3DXBuffMatModel = NULL;
	g_nNumMatModel = 0;

	// X�t�@�C���̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(MODEL_CAR2,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
								D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
								pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
								NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
								&g_pD3DXBuffMatModel,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
								NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
								&g_nNumMatModel,		// D3DXMATERIAL�\���̂̐�
								&g_pD3DXMeshModel)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}
	LPD3DXMESH			g_pD3DXMeshMask;		// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		g_pD3DXBuffMatMask;	// �}�e���A�����ւ̃|�C���^
	DWORD				g_nNumMatMask;			// �}�e���A�����̐�

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL_CAR,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pD3DXBuffMatMask,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMatMask,			// D3DXMATERIAL�\���̂̐�
		&g_pD3DXMeshMask)))		// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}


#if 1
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,								// �t�@�C���̖��O
		&g_pD3DTextureModel);							// �ǂݍ��ރ������[

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME2,								// �t�@�C���̖��O
		&g_pD3DTextureModel2);							// �ǂݍ��ރ������[
#endif

	// ��U�K�v�Ȃ�����]�v�ȏ�����؂�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// �X�e�[�W0�̃J���[
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);// �F���|���Z���Ă�������
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	// �|���S���̒��_�̐F��ς���̂����C�g�̂��߁A�|���S���̐F�������Őݒ肵�Ă��Ȃ��ƃ��C�g�ɂȂ�Ȃ�

	// �X�e�[�W0�̃����ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	// �X�e�[�W1�̃J���[
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);// �F���|���Z���Ă�������
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	// �|���S���̒��_�̐F��ς���̂����C�g�̂��߁A�|���S���̐F�������Őݒ肵�Ă��Ȃ��ƃ��C�g�ɂȂ�Ȃ�

	// �X�e�[�W1�̃����ݒ�
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitModel(void)
{
	if(g_pD3DTextureModel != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureModel->Release();
		g_pD3DTextureModel = NULL;
	}

	if(g_pD3DXMeshModel != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshModel->Release();
		g_pD3DXMeshModel = NULL;
	}

	if(g_pD3DXBuffMatModel != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatModel->Release();
		g_pD3DXBuffMatModel = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{
	g_rotModel.y += D3DXToRadian( 0.1f );

	g_posCameraR = g_posModel;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;



	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_mtxView, 
						&g_posCameraP,		// �J�����̎��_
						&g_posCameraR,		// �J�����̒����_
						&g_vecCameraU);		// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
								VIEW_ANGLE,			// �r���[���ʂ̎���p
								VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
								VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_sclModel.x, g_sclModel.y, g_sclModel.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posModel.x, g_posModel.y, g_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	//
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureModel);

		// �`��
		g_pD3DXMeshModel->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}


//=============================================================================
// �}�X�N�̕`��
//=============================================================================
void DrawMask(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_mtxView,
		&g_posCameraP,		// �J�����̎��_
		&g_posCameraR,		// �J�����̒����_
		&g_vecCameraU);		// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_sclMask.x, g_sclMask.y, g_sclMask.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMask.y, g_rotMask.x, g_rotMask.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posMask.x, g_posMask.y, g_posMask.z);
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	//
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureModel);

		// �`��
		g_pD3DXMeshModel->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}


//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	g_posCameraP = D3DXVECTOR3(POS_CAMERA_P_X, POS_CAMERA_P_Y, POS_CAMERA_P_Z);
	g_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_vecCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}


//=============================================================================
// ���C�g�̏���������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;

	/* 0 */
	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ���C�g0�̃^�C�v�̐ݒ�
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g0�̊g�U���̐ݒ�
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

	// ���C�g0�̊����̐ݒ�
	g_aLight[0].Ambient = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

	// ���C�g0�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.5f, -0.2f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ���C�g0�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &g_aLight[0]);

	// ���C�g0���g�p�g�p��Ԃ�
	pDevice->LightEnable(0, TRUE);


	/* 1 */
	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// ���C�g1�̃^�C�v�̐ݒ�
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g1�̊g�U���̐ݒ�
	g_aLight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ���C�g1�̊����̐ݒ�
	g_aLight[1].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    // ���C�g1�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.0f, -1.0f, 0.8f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ���C�g1�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(1, &g_aLight[1]);

	// ���C�g1�̐ݒ�
	pDevice->LightEnable(1, TRUE);


	/* 2 */
	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

	// ���C�g2�̃^�C�v�̐ݒ�
	g_aLight[2].Type = D3DLIGHT_POINT;

	// ���C�g2�̊g�U���̐ݒ�
	g_aLight[2].Diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g2�̊����̐ݒ�
	g_aLight[2].Ambient = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g�Q�̃|�W�V������ݒ�
	g_aLight[2].Position = D3DXVECTOR3(20.0f,20.0f, 0.0f);

	// ���C�g2�̕����̐ݒ�
	vecDir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	g_aLight[2].Attenuation0 = 1.0f;	// Attenuation = �����@0, 1, 2�Ō����̊|��������Ⴄ�H�@0����Ԃ��ꂢ�B
	g_aLight[2].Range = 30.0f;			// �_�����̏ꍇ���ꂪ���C�g�̋����ɂȂ�͗l�@�y���ꂪ�Ȃ��ƌ���Ȃ��I�z
	g_aLight[2].Falloff = 5.0f;			// �悭�킩���

	// ���C�g2�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(2, &g_aLight[2]);

	// ���C�g2�̐ݒ�
	pDevice->LightEnable(2, TRUE);


	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

