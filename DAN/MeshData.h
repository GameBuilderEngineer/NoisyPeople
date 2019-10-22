//-----------------------------------------------------------------------------
// ���b�V���f�[�^�N���X [MeshData.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/20
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshLoader.h"


//=============================================================================
//�N���X��`
//=============================================================================
class MeshData
{
private:
	StaticMesh* staticMesh;				// ���b�V�����
	BYTE* vtxArray;						// ���_���z��
	WORD* indexArray;					// ���_�C���f�b�N�X�z��
	DWORD* adjacencyArray;				// �אڃ|���S���C���f�b�N�X�z��
	DWORD numVertices;					// ���_��
	DWORD stride;						// 1���_������̃o�C�g��
	DWORD numFaces;						// 3�p�`�|���S���ʂ̐�

public:
	// ������
	void initialize(StaticMesh* _staticMesh);
	// �I������
	void uninitialize();
	// ���_�C���f�b�N�X���璸�_���擾
	BYTE* getVertexPointerFromVertexIndex(BYTE* vtx, WORD number);
	// �ʃC���f�b�N�X���璸�_�C���f�b�N�X���擾
	WORD* getVertexIndexPointerFromFaceIndex(WORD* vertexIndex, DWORD number);
	// �ʃC���f�b�N�X���璸�_���擾
	BYTE* getVertexPointerFromFaceIndex(BYTE* vtx, WORD* vertexIndex, DWORD number);
	// �ʃC���f�b�N�X����אږʃC���f�b�N�X���擾
	DWORD* getAdjacentFaceIndexPointerFromFaceIndex(DWORD* adjacency, DWORD number);

	// Getter
	StaticMesh* getStaticMesh();		// ���b�V�������擾
	BYTE* getVtxArray();				// ���_���z����擾
	WORD* getIndexArray();				// ���_�C���f�b�N�X�z����擾
	DWORD* getAdjacencyArray();			// �אڃ|���S���C���f�b�N�X�z����擾
	DWORD getNumVertices();				// ���_�����擾
	DWORD getStride();					// 1���_������̃o�C�g�����擾
	DWORD getNumFaces();				// 3�p�`�|���S���̖ʂ̐����擾
};