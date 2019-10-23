//-----------------------------------------------------------------------------
// �c���[�Ǘ��N���X [Tree.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshObject.h"
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
	StaticMeshObject* aTrunkRenderer;			// A���f�����`��I�u�W�F�N�g
	StaticMeshObject* aLeafRenderer;			// A���f���t�`��I�u�W�F�N�g
	StaticMeshObject* bTrunkRenderer;			// B���f�����`��I�u�W�F�N�g
	StaticMeshObject* bLeafRenderer;			// B���f���t�`��I�u�W�F�N�g
	StaticMeshObject* cTrunkRenderer;			// C���f�����`��I�u�W�F�N�g
	StaticMeshObject* cLeafRenderer;			// C���f���t�`��I�u�W�F�N�g
	int nextID;									// ����c���[���sID

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createTree(treeNS::TreeData treeData);
	void createLeaf(Object* leaf, int _model);
	void destroyLeaf(Object* leaf, int _model);
	void destroyAllTree();
	void outputGUI();
};
