//-----------------------------------------------------------------------------
// �i�r�Q�[�V�������b�V�������N���X [NavigationMesh.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/5
//-----------------------------------------------------------------------------
#include "NavigationMesh.h"
#include "StaticMeshLoader.h"


//=============================================================================
// ����������
//=============================================================================
void NavigationMesh::initialize()
{
	staticMesh =  staticMeshNS::getStaticMesh(staticMeshNS::SAMPLE_STATIC_MESH);

	//// ���_�t�H�[�}�b�g���m�F���邽�߂Ɏg�p
	// DWORD fvf = staticMesh->mesh->GetFVF();

	//// ���_�o�b�t�@�̓��e���݂�
	//Vertex3D *vertex;
	//staticMesh->mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vertex);
	//staticMesh->mesh->UnlockVertexBuffer();

	//// �C���f�b�N�X�o�b�t�@�̓��e���݂�
	//WORD *index;
	//staticMesh->mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&index);
	//staticMesh->mesh->UnlockIndexBuffer();
	
	// �אڐ��o�b�t�@��p�ӂ��ăf�[�^���R�s�[
	adjacency = new DWORD[staticMesh->mesh->GetNumFaces() * 3];
	staticMesh->mesh->GenerateAdjacency(0, adjacency);


}


//=============================================================================
// �I������
//=============================================================================
void NavigationMesh::uninitialize()
{
	SAFE_DELETE_ARRAY(adjacency)
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


#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�`��
//=============================================================================
void NavigationMesh::debugRender()
{
}
#endif // _DEBUG

// FVF���킩��Ȃ�
// ���b�V����񖄂߂邱�ƂɌq���邩������Ȃ�
// Astar�ł̓G�b�W�̏����������āA���̃G�b�W�̂ǂ���ړI�n�Ƃ��邩�A�͕ʏ����iAI�����ɏ����Ă���j