//-----------------------------------------------------------------------------
// ���b�V���f�[�^�N���X [MeshData.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/20
//-----------------------------------------------------------------------------
#include "MeshData.h"
#include "VertexAccessor.h"
using namespace meshDataNS;

//=============================================================================
// ������
//=============================================================================
void MeshData::initialize(StaticMesh* _staticMesh)
{
	staticMesh = _staticMesh;
	numVertices = staticMesh->mesh->GetNumVertices();
	stride = staticMesh->mesh->GetNumBytesPerVertex();
	numFaces = staticMesh->mesh->GetNumFaces();

	// �z����m��
	vtxArray = new BYTE[numVertices * stride];
	indexArray = new WORD[numFaces * 3];
	adjacencyArray = new DWORD[numFaces * 3];

	// ���_�o�b�t�@��z��ɃR�s�[����
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;
	BYTE* pVtx;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		memcpy(vtxArray, pVtx, numVertices * stride);
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", NULL, MB_OK);
		return;
	}

	// �C���f�b�N�X�o�b�t�@��z��ɃR�s�[����
	WORD* pIndex;
	staticMesh->mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);
	memcpy(indexArray, pIndex, sizeof(WORD) * numFaces * 3);
	staticMesh->mesh->UnlockIndexBuffer();

	// �אڐ�����z��ɃR�s�[
	staticMesh->mesh->GenerateAdjacency(0, adjacencyArray);


	////renderer->generateObject(this);

	//position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	//Object::initialize(&position);

	//numVertices = mesh->GetNumVertices();
	//stride = mesh->GetNumBytesPerVertex();
	//numFaces = mesh->GetNumFaces();
	//vtxAccessor.initialize(staticMesh);

	////// ���_�o�b�t�@��z��ɃR�s�[����
	////vtx = new BYTE[numVertices * stride];
	////BYTE* pVtx;
	////mesh->GetVertexBuffer(&vertexBuffer);
	////if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	////{
	////	memcpy(vtx, pVtx, numVertices * stride);
	////	vertexBuffer->Unlock();
	////}
	////else
	////{
	////	MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", NULL, MB_OK);
	////	return;
	////}

	////// �C���f�b�N�X�o�b�t�@��z��ɃR�s�[����
	////index = new WORD[numFaces * 3];
	////WORD* pIndex;
	////mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);
	////memcpy(index, pIndex, sizeof(WORD) * numFaces * 3);
	////mesh->UnlockIndexBuffer();

	////// �אڐ�����z��ɃR�s�[
	////adjacency = new DWORD[numFaces * 3];
	////mesh->GenerateAdjacency(0, adjacency);


	//D3DCOLOR white = D3DCOLOR_RGBA(255, 255, 255, 155);
	//D3DCOLOR red = D3DCOLOR_RGBA(255, 0, 0, 155);

	//BYTE* pVtx;
	//mesh->GetVertexBuffer(&vertexBuffer);
	//if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	//{
	//	bool isRed = false;
	//	for (int i = 0; i < numVertices; i++)
	//	{
	//		D3DCOLOR* pColor = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, &pVtx[i * stride]);
	//		*pColor = red;


	//		//D3DXVECTOR3* pos = (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, &pVtx[i * stride]);
	//		//*pos = *pos +  D3DXVECTOR3(rand() % 3, rand() % 3, rand() % 3);
	//	}
	//	vertexBuffer->Unlock();
	//}
	//else
	//{
	//	MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", NULL, MB_OK);
	//	return;
	//}


#if 1	// �t�@�C���Ƀ_���v
	VertexAccessor vtxAccessor;
	vtxAccessor.initialize(staticMesh);
	FILE* fp;
	setDataDirectory();

	// ���_���
	fp = fopen("VertexBufferOfNavMesh.txt", "w");
	fprintf(fp, "���_�o�b�t�@�z��̒��g\n�o�͌��FMeshData.cpp\n");
	for (int i = 0; i < numVertices; i++)
	{
		D3DXVECTOR3* test = (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, &vtxArray[i * stride]);
		fprintf(fp, "���_%d, %f, %f, %f \n", i, test->x, test->y, test->z);
	}
	fclose(fp);

	// ���_�錾
	fp = fopen("DeclarationOfNavMesh.txt", "w");
	fprintf(fp, "���_�錾�\���̂̒��g\n�o�͌��FMeshData.cpp\n");
	for (int i = 0; i < 65; i++)
	{
		fprintf(fp, "\n[%d]",i);
		fprintf(fp, "Type : %d\n", staticMesh->vertexElement[i].Type);
		fprintf(fp, "Usage : %d\n", staticMesh->vertexElement[i].Usage);
	}
	fclose(fp);

	// �C���f�b�N�X���
	fp = fopen("IndexBufferOfNavMesh.txt", "w");
	fprintf(fp, "�C���f�b�N�X�o�b�t�@�z��̒��g\n�o�͌��FMeshData.cpp\n");
	fprintf(fp, "�|���S���C���f�b�N�X : �|���S�����\�����钸�_�C���f�b�N�X1, 2, 3\n\n");
	for (int i = 0; i < numFaces * 3; i++)
	{
		fprintf(fp, "%d,", indexArray[i]);
		if(i % 3 == 2) fprintf(fp, "\n");
	}
	fclose(fp);

	// �אڐ����
	fp = fopen("AdjacencyOfNavMesh.txt", "w");
	fprintf(fp, "�אڐ����z��̒��g\n�o�͌��FMeshData.cpp\n");
	fprintf(fp, "�|���S���C���f�b�N�X : �אڂ���|���S���C���f�b�N�X1, 2, 3\n\n");
	for (int i = 0, j = 0; i < numFaces * 3; i++)
	{
		if (i % 3 == 0) fprintf(fp, "%4d : ", j++);
		fprintf(fp, "%d,", adjacencyArray[i]);
		if (i % 3 == 2) fprintf(fp, "\n");
	}
	fclose(fp);
#endif
}


//=============================================================================
// �I������
//=============================================================================
void MeshData::uninitialize()
{
	SAFE_DELETE_ARRAY(vtxArray);
	SAFE_DELETE_ARRAY(indexArray);
	SAFE_DELETE_ARRAY(adjacencyArray);
}


//=============================================================================
// ���_�C���f�b�N�X���璸�_���擾
//=============================================================================
BYTE* MeshData::getVertexPointerFromVertexIndex(BYTE* vtx, WORD number)
{
	return &vtx[stride * number];
}


//=============================================================================
// �ʃC���f�b�N�X���璸�_�C���f�b�N�X���擾
//=============================================================================
VertexIndex MeshData::getVertexIndexFromFaceIndex(WORD* vertexIndex, DWORD number)
{
	VertexIndex out;
	out.v[0] = vertexIndex[number * 3];
	out.v[1] = vertexIndex[number * 3 + 1];
	out.v[2] = vertexIndex[number * 3 + 2];
	return out;
}


// ��
//=============================================================================
// �ʃC���f�b�N�X���璸�_���擾
//=============================================================================
BYTE* MeshData::getVertexPointerFromFaceIndex(BYTE* vtx, WORD* vertexIndex, DWORD number)
{
	//WORD* p = getVertexIndexPointerFromFaceIndex(vertexIndex, number);
	//return getVertexPointerFromVertexIndex(vtx, p[0]);
	return NULL;
}


//=============================================================================
// �ʃC���f�b�N�X����אږʃC���f�b�N�X���擾
//=============================================================================
DWORD* MeshData::getAdjacentFaceIndexPointerFromFaceIndex(DWORD* adjacency, DWORD number)
{
	return &adjacency[number * 3];
}


//=============================================================================
// Getter
//=============================================================================
StaticMesh* MeshData::getStaticMesh() { return staticMesh; }
BYTE* MeshData::getVtxArray() { return vtxArray; };
WORD* MeshData::getIndexArray() { return indexArray; };
DWORD* MeshData::getAdjacencyArray() { return adjacencyArray; };
DWORD MeshData::getNumVertices() { return numVertices; };
DWORD MeshData::getStride() { return stride; };
DWORD MeshData::getNumFaces() { return numFaces; };