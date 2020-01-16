//-----------------------------------------------------------------------------
// ���b�V���f�[�^�N���X [MeshData.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/20
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshLoader.h"
#include "VertexAccessor.h"


//=============================================================================
// ���O���
//=============================================================================
namespace meshDataNS
{
	// �|���S���ʃm�[�h�p���_�\����
	struct VertexForFaceNode
	{
		BYTE* data;						// ���_�f�[�^
		WORD index;						// ���_�C���f�b�N�X
		D3DXVECTOR3	position;			// ���_���W
	};

	// �|���S���ʍ\����
	struct Face
	{
		VertexForFaceNode vtx[3];		// ���_�\���̔z��
		D3DXVECTOR3 nor;				// �@��
		DWORD adjacency[3];				// �אڃ|���S���ʃC���f�b�N�X�z��
	};

	// �C���f�b�N�X2�\����
	struct Index2
	{
		WORD index[2];					// �C���f�b�N�X�z��
	};
}


//=============================================================================
//�N���X��`
//=============================================================================
class MeshData
{
private:
	StaticMesh* staticMesh;				// ���b�V�����
	VertexAccessor*	vtxAccessor;		// ���_�A�N�Z�X�I�u�W�F�N�g
	BYTE* vtxArray;						// ���_���z��
	WORD* indexArray;					// ���_�C���f�b�N�X�z��
	DWORD* adjacencyArray;				// �אڃ|���S���C���f�b�N�X�z��
	meshDataNS::Face* faceArray;		// �|���S���ʔz��
	DWORD numVertices;					// ���_��
	DWORD stride;						// 1���_������̃o�C�g��
	DWORD numFaces;						// 3�p�`�|���S���ʂ̐�
	DWORD fvf;							// FVF(�_��Ȓ��_�t�H�[�}�b�g)


public:
	// ������
	void initialize(StaticMesh* _staticMesh, VertexAccessor* _vtxAccessor);
	// �I������
	void uninitialize();
	// ���_�C���f�b�N�X���璸�_���擾
	BYTE* getVertexPointerFromVertexIndex(WORD number);
	BYTE* getVertexPointerFromVertexIndex(WORD number, BYTE* vtx);
	// �ʃC���f�b�N�X���璸�_�C���f�b�N�X���擾
	WORD getVertexIndexFromFaceIndex(DWORD number, int selecter);
	WORD getVertexIndexFromFaceIndex(DWORD number, int selecter, WORD* vertexIndex);
	// �ʃC���f�b�N�X����אږʃC���f�b�N�X���擾
	DWORD getAdjacentFaceIndexFromFaceIndex(DWORD number, int selecter);
	DWORD getAdjacentFaceIndexFromFaceIndex(DWORD number, int selecter, DWORD* adjacency);
	// 2�̖ʃC���f�b�N�X���狤�ʂ̒��_�C���f�b�N�X�����o��
	meshDataNS::Index2 getSharingVertexIndexFromTwoFace(DWORD number1, DWORD number2);

	// Getter
	StaticMesh* getStaticMesh();		// ���b�V�������擾
	BYTE* getVtxArray();				// ���_���z����擾
	WORD* getIndexArray();				// ���_�C���f�b�N�X�z����擾
	DWORD* getAdjacencyArray();			// �אڃ|���S���C���f�b�N�X�z����擾
	meshDataNS::Face* getFaceArray();	// �ʔz����擾
	DWORD getNumVertices();				// ���_�����擾
	DWORD getStride();					// 1���_������̃o�C�g�����擾
	DWORD getNumFaces();				// 3�p�`�|���S���̖ʂ̐����擾
	DWORD getFVF();						// ���_�t�H�[�}�b�g���擾
};