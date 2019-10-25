//-----------------------------------------------------------------------------
// �i�r�Q�[�V�������b�V�������N���X [NavigationMesh.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/5
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "Object.h"
#include "VertexAccessor.h"
#include "MeshData.h"
#include "AStar.h"


//=============================================================================
// ���O���
//=============================================================================
namespace navigationMeshNS
{
	struct Edge
	{
		D3DXVECTOR3 v0;		
		D3DXVECTOR3 v1;
	};

}


//=============================================================================
//�N���X��`
//=============================================================================
class NavigationMesh: public Object
{
private:
	StaticMesh* staticMesh;						// ���b�V�����
	MeshData meshData;							// ���b�V���f�[�^�I�u�W�F�N�g
	VertexAccessor vtxAccessor;					// ���_�A�N�Z�X�I�u�W�F�N�g
	AStar aStar;								// �p�X�����I�u�W�F�N�g


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
	void pathSearch(D3DXVECTOR3 from, D3DXVECTOR3 dest, SEARCH_FLAG flag);

	// �X�e�A�����O
	void steering();


#ifdef _DEBUG
	void debugRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void outputGUI();
#endif // _DEBUG
};
