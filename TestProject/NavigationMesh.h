//-----------------------------------------------------------------------------
// �i�r�Q�[�V�������b�V�������N���X [NavigationMesh.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/5
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshObject.h"


//=============================================================================
// ���O���
//=============================================================================
namespace enemyNS
{

}


//=============================================================================
//�N���X��`
//=============================================================================
class NavigationMesh: public StaticMeshObject
{
private:
	StaticMesh* staticMesh;						// �i�r�Q�[�V�����p���b�V��
	LPDIRECT3DVERTEXBUFFER9	vtxBuffer;			// ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	indexBuffer;		// �C���f�b�N�X�o�b�t�@
	DWORD *adjacency;							// �אڐ��o�b�t�@

	D3DINDEXBUFFER_DESC desc;



public:
	//NavigationMesh();
	//~NavigationMesh();

	void initialize();
	void uninitialize();
	void update();

#ifdef _DEBUG
	void debugRender();
#endif // _DEBUG

};
