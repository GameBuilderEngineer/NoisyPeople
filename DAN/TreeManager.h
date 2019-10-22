//-----------------------------------------------------------------------------
// �c���[�Ǘ��N���X [Tree.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "Tree.h"


//=============================================================================
// ���O���
//=============================================================================
namespace treeNS
{	
}


//=============================================================================
//�N���X��`
//=============================================================================
class TreeManager
{
private:
	std::vector<Tree*> treeList;				// �c���[�|�C���^���X�g
	StaticMeshObject* tree1Renderer;
	StaticMeshObject* leaf1Renderer;
	StaticMeshObject* tree2Renderer;
	StaticMeshObject* leaf2Renderer;
	StaticMeshObject* tree3Renderer;
	StaticMeshObject* leaf3Renderer;

	int nextID;									// ����c���[���sID


public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createTree(treeNS::TreeData* treeData);
	void destroyTree(int _id);
	void destroyAllTree();
	void outputGUI();
};
