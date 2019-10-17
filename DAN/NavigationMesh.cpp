//-----------------------------------------------------------------------------
// �i�r�Q�[�V�������b�V�������N���X [NavigationMesh.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/5
//-----------------------------------------------------------------------------
#include "NavigationMesh.h"
#include "ImguiManager.h"


//=============================================================================
// �R���X�g���N�^
//=============================================================================
NavigationMesh::NavigationMesh(StaticMesh* staticMesh)//:StaticMeshObject(staticMesh)
{
	mesh = staticMesh->mesh;	// �Q�Ƃ̒������k�߂邽�߂Ƀ|�C���^���R�s�[
}


//=============================================================================
// ����������
//=============================================================================
void NavigationMesh::initialize()
{
	numVertices = mesh->GetNumVertices();
	stride = mesh->GetNumBytesPerVertex();
	numFaces = mesh->GetNumFaces();
	//vtxAccessor.initialize(staticMesh);������R�����g�A�E�g

	//// ���_�o�b�t�@��z��ɃR�s�[����
	//vtx = new BYTE[numVertices * stride];
	//BYTE* pVtx;
	//mesh->GetVertexBuffer(&vertexBuffer);
	//if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	//{
	//	memcpy(vtx, pVtx, numVertices * stride);
	//	vertexBuffer->Unlock();
	//}
	//else
	//{
	//	MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", NULL, MB_OK);
	//	return;
	//}

	//// �C���f�b�N�X�o�b�t�@��z��ɃR�s�[����
	//index = new WORD[numFaces * 3];
	//WORD* pIndex;
	//mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);
	//memcpy(index, pIndex, sizeof(WORD) * numFaces * 3);
	//mesh->UnlockIndexBuffer();

	//// �אڐ�����z��ɃR�s�[
	//adjacency = new DWORD[numFaces * 3];
	//mesh->GenerateAdjacency(0, adjacency);


	D3DCOLOR setColor = D3DCOLOR_RGBA(255, 0, 0, 255);

	BYTE* pVtx;
	mesh->GetVertexBuffer(&vertexBuffer);
 	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		for (int i = 0; i < numVertices; i++)
		{
			D3DCOLOR* pColor = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, &pVtx[i * stride]);
			*pColor = setColor;
			D3DXVECTOR3* pos = (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, &pVtx[i * stride]);
			*pos = *pos +  D3DXVECTOR3(rand() % 3, rand() % 3, rand() % 3);
		}
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", NULL, MB_OK);
		return;
	}



#ifdef _DEBUG
	////------------------
	//// �t�@�C���Ƀ_���v
	////------------------
	//FILE* fp;
	//setDataDirectory();

	//// ���_���
	//fp = fopen("VertexBufferOfNavMesh.txt", "w");
	//fprintf(fp, "���_�o�b�t�@�z��̒��g\n�o�͌��FNavigationMesh.cpp\n");
	//for (int i = 0; i < numVertices; i++)
	//{
	//	D3DXVECTOR3* test = (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, &vtx[i * stride]);
	//	fprintf(fp, "%f, %f, %f \n", test->x, test->y, test->z);
	//}
	//fclose(fp);


	//// ���_�錾
	//fp = fopen("DeclarationOfNavMesh.txt", "w");
	//fprintf(fp, "���_�錾�\���̂̒��g\n�o�͌��FNavigationMesh.cpp\n");
	//for (int i = 0; i < 65; i++)
	//{
	//	fprintf(fp, "\n[%d]",i);
	//	fprintf(fp, "Type : %d\n", staticMesh->vertexElement[i].Type);
	//	fprintf(fp, "Usage : %d\n", staticMesh->vertexElement[i].Usage);
	//}
	//fclose(fp);

	//// �C���f�b�N�X���
	//fp = fopen("IndexBufferOfNavMesh.txt", "w");
	//fprintf(fp, "�C���f�b�N�X�o�b�t�@�z��̒��g\n�o�͌��FNavigationMesh.cpp\n");
	//fprintf(fp, "�|���S���C���f�b�N�X : �|���S�����\�����钸�_�C���f�b�N�X1, 2, 3\n\n");
	//for (int i = 0; i < numFaces * 3; i++)
	//{
	//	fprintf(fp, "%d,", index[i]);
	//	if(i % 3 == 2) fprintf(fp, "\n");
	//}
	//fclose(fp);

	//// �אڐ����
	//fp = fopen("AdjacencyOfNavMesh.txt", "w");
	//fprintf(fp, "�אڐ����z��̒��g\n�o�͌��FNavigationMesh.cpp\n");
	//fprintf(fp, "�|���S���C���f�b�N�X : �אڂ���|���S���C���f�b�N�X1, 2, 3\n\n");
	//for (int i = 0, j = 0; i < numFaces * 3; i++)
	//{
	//	if (i % 3 == 0) fprintf(fp, "%4d : ", j++);
	//	fprintf(fp, "%d,", adjacency[i]);
	//	if (i % 3 == 2) fprintf(fp, "\n");
	//}
	//fclose(fp);
#endif
}


//=============================================================================
// �I������
//=============================================================================
void NavigationMesh::uninitialize()
{
	SAFE_DELETE_ARRAY(vtx);
	SAFE_DELETE_ARRAY(index);
	SAFE_DELETE_ARRAY(adjacency);
}


//=============================================================================
// �X�V����
//=============================================================================
void NavigationMesh::update()
{













	// �����̂���|���S���̃C���f�b�N�X���擾

	// �J�n�|���S���ɐݒ�

	// �ړI�n�|���S����ݒ�

	// ��m�[�h�ɊJ�n�|���S����ݒ肷��

	// �I�[�v�����X�g�ɊJ�n�m�[�h��ǉ�����

	// A*----------------------------
	// ******** ��m�[�h���ړI�n�̃m�[�h�ł���ꍇ
	// �o�H����
	// ******** ��m�[�h���ړI�n�̃m�[�h�łȂ��ꍇ
	// ��m�[�h�̎�����I�[�v�����Ă���
	//�i�אڐ��f�[�^�����ƂɁj

	// �o�H�T����A�|���S����ł̋������K�v
}


//=============================================================================
// �o�H�T��
// [�@�\] 2�n�_���Ȃ��o�H�����߂�
// [����] from�F���ݒn���W�@dest�F�ړI�n���W�@flag�F���ݒn/�ړI�n�̌�����@
// STANDARD �������W�ɍł��߂��|���S��
// GRAVITY  �������W����d�͕����Ƀ��C�L���X�g���ďՓ˂����|���S��
//=============================================================================
void NavigationMesh::searchRoute(D3DXVECTOR3 from, D3DXVECTOR3 dest, SEARCH_FLAG flag)
{
	// ���[���h�}�g���N�X�̋t�}�g���N�X�Ŏw�����W�����[�J�����W�ɕϊ�����

	// �d�͕���
}


//
//BYTE* NavigationMesh::getVertexFromIndex(BYTE* pVtx, WORD index)
//{
//	return &pVtx[index];
//}


#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�`��
//=============================================================================
void NavigationMesh::debugRender()
{
}


//=============================================================================
// ImGUI�ւ̏o��
//=============================================================================
void NavigationMesh::outputGUI()
{
	//ImGui::Text(sceneName.c_str());

	if (ImGui::CollapsingHeader("NaviAIInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;

		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));


		ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
		ImGui::Checkbox("onActive", &onActive);											//�A�N�e�B�u���t���O
	}
}
#endif // _DEBUG



// Astar�ł̓G�b�W�̏����������āA���̃G�b�W�̂ǂ���ړI�n�Ƃ��邩�A�͕ʏ����iAI�����ɏ����Ă���j