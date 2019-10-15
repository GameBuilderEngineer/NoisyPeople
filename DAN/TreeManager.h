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
	const int INITIAL_RESERVE = 500;			// �c���[�|�C���^�̏����m�ې�
}


//=============================================================================
//�N���X��`
//=============================================================================
class TreeManager
{
private:
	std::vector<treeNS::TreeData> treeDataList;	// �c���[�f�[�^���X�g
	std::vector<Tree*> treeList;				// �c���[�|�C���^���X�g

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createTree(treeNS::TreeData* treeData);
	void destroyTree(int _id);
	void outputGUI();
};
