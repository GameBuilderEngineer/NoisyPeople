//-----------------------------------------------------------------------------
// �i�r�Q�[�V�������b�V�������N���X [NavigationMesh.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/5
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "VertexAccessor.h"
#include "StaticMeshObject.h"


//=============================================================================
// ���O���
//=============================================================================
namespace navigationMeshNS
{
	struct VertexNaviMesh
	{
	};

}

struct Index3
{
	DWORD index[3];
};


//=============================================================================
//�N���X��`
//=============================================================================
class NavigationMesh: public StaticMeshObject
{
private:
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;		// ���_�o�b�t�@

	LPD3DXMESH mesh;							// ���b�V��
	BYTE* vtx;									// ���_���
	WORD* index;								// (���_)�C���f�b�N�X���
	DWORD *adjacency;							// �אڐ����
	

	VertexAccessor vtxAccessor;					// ���_�A�N�Z�X
	DWORD numVertices;
	DWORD stride;
	DWORD numFaces;

public:
	NavigationMesh(StaticMesh* staticMesh);
	void initialize();
	void uninitialize();
	void update();

	// �T�����s�O�Ɍ��ݒn�|���S���ƖړI�n�|���S���𒲐�����t���O
	enum SEARCH_FLAG
	{
		STANDARD,	// �w����W�ɍł��߂��|���S��
		GRAVITY		// �w����W����d�͕����Ƀ��C�L���X�g���ďՓ˂����|���S��
	};

	// �o�H�T��
	void searchRoute(D3DXVECTOR3 from, D3DXVECTOR3 dest, SEARCH_FLAG flag);

	//BYTE* getVertexFromIndex(BYTE* pVtx, WORD index);

	//WORD getIndexFromPolygon();
	//BYTE* getVertexFromPolygon(DWORD polyIndex);
	//Index3 getAdjacencyFromPolygon(DWORD polyIndex);

#ifdef _DEBUG
	void debugRender();
	void outputGUI();
#endif // _DEBUG
};
