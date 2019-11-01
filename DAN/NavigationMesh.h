//-----------------------------------------------------------------------------
// �i�r�Q�[�V�������b�V�������N���X [NavigationMesh.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/5
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include <vector>
#include "Object.h"
#include "StaticMeshRenderer.h"
#include "VertexAccessor.h"
#include "MeshData.h"
#include "AStar.h"
#include "PathFollowing.h"


//=============================================================================
// ���O���
//=============================================================================
namespace navigationMeshNS
{
	enum NAVIRESULT
	{
		NAVI_OK						= 0,		
		CURRENT_NOT_ON_MESH			= -1,
		DESTINATION_NOT_ON_MESH		= -2,
		IMPASSE						= -3,
		UNKNOWN_ERROR				= -99
	};


#ifdef _DEBUG
	const float FLOATING_HEIGHT = 0.0f;
#endif
}

//=============================================================================
//�N���X��`
//=============================================================================
class NavigationMesh: public Object
{
private:
	StaticMesh* staticMesh;						// ���b�V�����
	VertexAccessor vtxAccessor;					// ���_�A�N�Z�X�I�u�W�F�N�g
	MeshData meshData;							// ���b�V���f�[�^�I�u�W�F�N�g
	AStar aStar;								// �p�X�����I�u�W�F�N�g
	PathFollowing pathFollowing;				// �p�X�t�H���[�C���O�I�u�W�F�N�g
	StaticMeshRenderer* renderer;				// �`��I�u�W�F�N�g
	D3DXVECTOR3 gravityDirection;				// �d�͕���
	static NavigationMesh* pointer;				// �擾�p�|�C���^
	
	// pathSearch()���s���ɍX�V�������
	D3DXVECTOR3 from;							// �J�n���W
	D3DXVECTOR3 dest;							// �ړI�n���W
	DWORD startIndex;							// �J�n�|���S���ʃC���f�b�N�X
	DWORD destIndex;							// �ړI�n�ʃ|���S���C���f�b�N�X
	LinkedList<meshDataNS::Index2>* edgeList;	// �G�b�W���X�g

public:
	NavigationMesh(StaticMesh* staticMesh);
	void initialize();
	void uninitialize();
	void update();
	// �o�H�T��
	navigationMeshNS::NAVIRESULT pathSearch(LinkedList<meshDataNS::Index2>** pOut,
		DWORD* faceIndex, D3DXVECTOR3 _from, D3DXVECTOR3 _dest);
	// �X�e�A�����O�i�ړ��x�N�g���̍쐬�j
	navigationMeshNS::NAVIRESULT steering(D3DXVECTOR3* out, DWORD* faceIndex,
		D3DXVECTOR3 _position, LinkedList<meshDataNS::Index2>* _edgeList);
	// Getter
	static NavigationMesh* getNaviMesh() { return pointer; }

#ifdef _DEBUG
	void dumpEdgeList();
	void debugRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void debugRenderMesh(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void debugRenderEdge(LinkedList<meshDataNS::Index2>* _edgeList);
	void changeColor();
	void affectToEdgeVertex();
	void outputGUI();
#endif // _DEBUG
};