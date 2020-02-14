//=============================================================================
//
// �A�j������ [AllocateHierarchy.h]
// Author : HAL���� ���q�
//
//=============================================================================
#pragma once

#include <d3dx9.h>
#include <stdio.h>
#include "Base.h"


struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};


struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;            // �e�N�X�`�����

	// SkinMesh info
	LPD3DXMESH           pOrigMesh;             // �I���W�i�����b�V��

	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD                BoneNum;					 // �{�[���̐�
	DWORD                BoneWeightNum;               // �P�̒��_�ɉe�����y�ڂ��d�݂̐�
	LPD3DXBUFFER         pBoneCombinationBuf;	 // �{�[�����̃o�b�t�@
	D3DXMATRIX**         ppBoneMatrixPtrs;      // �S�{�[���̃}�g���b�N�X�̃|�C���^�̔z��
	D3DXMATRIX*          pBoneOffsetMatrices;   // �{�[���̃I�t�Z�b�g�s��
	DWORD                NumPaletteEntries;     // �p���b�g�T�C�Y
	bool                 UseSoftwareVP;        

	D3DXMATRIXA16*			g_pBoneMatrices;

};


class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR              Name, 
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD                     NumMaterials, 
		CONST DWORD *             pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);    
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};




void DrawFrame( IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame );
void DrawMeshContainer( IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );
//HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot );
void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
