//-----------------------------------------------------------------------------
// ���b�V���f�[�^�N���X [MeshData.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/20
//-----------------------------------------------------------------------------
#include "MeshData.h"
using namespace meshDataNS;

//=============================================================================
// ������
//=============================================================================
void MeshData::initialize(StaticMesh* _staticMesh, VertexAccessor* _vtxAccessor)
{
	staticMesh = _staticMesh;
	vtxAccessor = _vtxAccessor;
	numVertices = staticMesh->mesh->GetNumVertices();
	stride = staticMesh->mesh->GetNumBytesPerVertex();
	numFaces = staticMesh->mesh->GetNumFaces();
	staticMesh->mesh->GetFVF();

	// �z����m��
	vtxArray = new BYTE[numVertices * stride];
	indexArray = new WORD[numFaces * 3];
	adjacencyArray = new DWORD[numFaces * 3];
	faceArray = new Face[getNumFaces()];

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

	// �אڐ�����z��ɃR�s�[����
	staticMesh->mesh->GenerateAdjacency(0, adjacencyArray);

	// �|���S���ʔz��̏��𖄂߂�
	for (int i = 0; i < getNumFaces(); i++)
	{
		for (int vertexCnt = 0; vertexCnt < 3; vertexCnt++)
		{
			// ���_�f�[�^���Z�b�g
			faceArray[i].vtx[vertexCnt].data = getVertexPointerFromVertexIndex(faceArray[i].vtx[vertexCnt].index);
			// ���_�C���f�b�N�X���Z�b�g
			faceArray[i].vtx[vertexCnt].index = getVertexIndexFromFaceIndex(i, vertexCnt);
			// ���_���W���Z�b�g
			D3DXVECTOR3* position = (D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
				getVertexPointerFromVertexIndex(faceArray[i].vtx[vertexCnt].index));
			faceArray[i].vtx[vertexCnt].position = *position;
		}
		for (int faceCnt = 0; faceCnt < 3; faceCnt++)
		{
			// �אڐ��f�[�^���Z�b�g
			faceArray[i].adjacency[faceCnt] = getAdjacentFaceIndexFromFaceIndex(i, faceCnt);
		}
		// �@�����Z�b�g
		D3DXVECTOR3 vec0to1 = faceArray[i].vtx[1].position - faceArray[i].vtx[0].position;
		D3DXVECTOR3 vec0to2 = faceArray[i].vtx[2].position - faceArray[i].vtx[0].position;
		D3DXVec3Cross(&faceArray[i].nor, &vec0to1, &vec0to2);
		D3DXVec3Normalize(&faceArray[i].nor, &faceArray[i].nor);
	}

#if 0	// ���_�o�b�t�@�ɃA�N�Z�X���ă��b�V���𑀍삷��e�X�g
	D3DCOLOR red = D3DCOLOR_RGBA(255, 120, 0, 100);

	//BYTE* pVtx;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		for (int i = 0; i < numVertices; i++)
		{
			D3DCOLOR* pColor = (D3DCOLOR*)vtxAccessor->getPointer(vtxAccess::COLOR, &pVtx[i * stride]);
			*pColor = red;
		}
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", NULL, MB_OK);
		return;
	}
#endif

#if 1	// �z��̒��g���t�@�C���Ƀ_���v
	FILE* fp;
	setDataDirectory();

	// ���_���
	fp = fopen("VertexBufferOfNavMesh.txt", "w");
	fprintf(fp, "���_�o�b�t�@�z��̒��g\n�o�͌��FMeshData.cpp\n");
	for (int i = 0; i < numVertices; i++)
	{
		D3DXVECTOR3* test = (D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION, &vtxArray[i * stride]);
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
		int unko = indexArray[i];
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

	// �|���S���ʔz��
	fp = fopen("FaceArray.txt", "w");
	fprintf(fp, "�|���S���ʔz��̒��g\n�o�͌��FMeshData.cpp\n");
	for (int i = 0; i < getNumFaces(); i++)
	{
		// �ʃC���f�b�N�X
		fprintf(fp, "�y�|���S��Index%d�z\n", i);
		for (int k = 0; k < 3; k++)
		{
			// ���_�C���f�b�N�X�ƒ��_�f�[�^
			fprintf(fp, "���_Ind�F%d�@Pos(%f, %f, %f)\n",
				faceArray[i].vtx[k].index, faceArray[i].vtx[k].position.x,
				faceArray[i].vtx[k].position.y, faceArray[i].vtx[k].position.z);
		}
		// �אږʃC���f�b�N�X
		fprintf(fp, "�אږ�Ind: %d, %d, %d\n",
			faceArray[i].adjacency[0], faceArray[i].adjacency[1], faceArray[i].adjacency[2]);
		// �@���x�N�g��
		fprintf(fp, "�@���x�N�g��(%f, %f, %f)\n\n",
			faceArray[i].nor.x, faceArray[i].nor.y, faceArray[i].nor.z);
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
	SAFE_DELETE_ARRAY(faceArray);
}


//=============================================================================
// ���_�C���f�b�N�X���璸�_���擾(1)
//[����1] DWORD number ���_�C���f�b�N�X���w��
//=============================================================================
BYTE* MeshData::getVertexPointerFromVertexIndex(WORD number)
{
	return &vtxArray[stride * number];
}


//=============================================================================
// ���_�C���f�b�N�X���璸�_���擾(2)
//[����1] DWORD number ���_�C���f�b�N�X���w��
//[����2] BYTE* vtx �T�����钸�_�z����w��
//=============================================================================
BYTE* MeshData::getVertexPointerFromVertexIndex(WORD number, BYTE* vtx)
{
	return &vtx[stride * number];
}


//=============================================================================
// �ʃC���f�b�N�X���璸�_�C���f�b�N�X���擾(1)
//[����1] DWORD number �ʃC���f�b�N�X���w��
//[����2] int selecter (0,1,2)�ŉ��Ԗڂ̒��_���w��
//=============================================================================
WORD MeshData::getVertexIndexFromFaceIndex(DWORD number, int selecter)
{
	return indexArray[number * 3 + selecter];
}


//=============================================================================
// �ʃC���f�b�N�X���璸�_�C���f�b�N�X���擾(2)
//[����1] DWORD number �ʃC���f�b�N�X���w��
//[����2] int selecter (0,1,2)�ŉ��Ԗڂ̒��_���w��
//[����3] WORD* vetexIndex �T�����钸�_�C���f�b�N�X�z����w��
//=============================================================================
WORD MeshData::getVertexIndexFromFaceIndex(DWORD number, int selecter, WORD* vertexIndex)
{
	return indexArray[number * 3 + selecter];
}


//=============================================================================
// �ʃC���f�b�N�X����אږʃC���f�b�N�X���擾(1)
//[����1] DWORD number �ʃC���f�b�N�X���w��
//[����2] int selecter (0,1,2)�ŉ��Ԗڂ̗אږʂ��w��
//=============================================================================
DWORD MeshData::getAdjacentFaceIndexFromFaceIndex(DWORD number, int selecter)
{
	return adjacencyArray[number * 3 + selecter];
}


//=============================================================================
// �ʃC���f�b�N�X����אږʃC���f�b�N�X���擾(2)
//[����1] DWORD number �ʃC���f�b�N�X���w��
//[����2] int selecter (0,1,2)�ŉ��Ԗڂ̗אږʂ��w��
//[����3] WORD* vetexIndex �T������אڐ����z����w��
//=============================================================================
DWORD MeshData::getAdjacentFaceIndexFromFaceIndex(DWORD number, int selecter, DWORD* adjacency)
{
	return adjacency[number * 3 + selecter];
}


//=============================================================================
// 2�̖ʃC���f�b�N�X���狤�ʂ̒��_�C���f�b�N�X�����o��
//[����1] DWORD number1 �ʃC���f�b�N�X���w��
//[����2] DWORD number2 �ʃC���f�b�N�X���w��
//=============================================================================
Index2 MeshData::getSharingVertexIndexFromTwoFace(DWORD number1, DWORD number2)
{
	Index2 ans;
	ans.index[0] = ans.index[1] = -1;	// = 65535
	int cnt = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			if (getVertexIndexFromFaceIndex(number1, i) == getVertexIndexFromFaceIndex(number2, k))
			{
				ans.index[cnt++] = getVertexIndexFromFaceIndex(number1, i);
			}
		}
	}

	return ans;
}


//=============================================================================
// Getter
//=============================================================================
StaticMesh* MeshData::getStaticMesh() { return staticMesh; }
BYTE* MeshData::getVtxArray() { return vtxArray; };
WORD* MeshData::getIndexArray() { return indexArray; };
DWORD* MeshData::getAdjacencyArray() { return adjacencyArray; };
Face* MeshData::getFaceArray() { return faceArray; }
DWORD MeshData::getNumVertices() { return numVertices; };
DWORD MeshData::getStride() { return stride; };
DWORD MeshData::getNumFaces() { return numFaces; };
DWORD MeshData::getFVF() { return fvf; }