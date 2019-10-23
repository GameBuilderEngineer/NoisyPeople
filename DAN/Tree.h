//-----------------------------------------------------------------------------
// �c���[���N���X [Tree.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshRenderer.h"
#include "BoundingSphere.h"


//=============================================================================
// ���O���
//=============================================================================
namespace treeNS
{
	enum TREE_TYPE
	{
		ANALOG_TREE,
		DIGITAL_TREE
	};

	enum GREEN_STATE
	{
		GREEN,
		DEAD
	};

	enum TREE_SIZE
	{
		STANDARD,
		LARGE,
		VERY_LARGE
	};

	struct TreeData
	{
		int id;							// ���ʔԍ�
		TREE_TYPE treeType;				// �c���[�^�C�v
		GREEN_STATE geenState;			// �Ή����
		TREE_SIZE treeSize;				// �؂̃T�C�Y
		int hp;							// HP
		int initialPosition;			// �������W
		void initialize() { ZeroMemory(this, sizeof(TreeData)); }
	};
}


//=============================================================================
//�N���X��`
//=======================================================================T======
class Tree: public StaticMeshRenderer
{
private:
	treeNS::TreeData* treeData;


	// Static
	// ���T�C�Y�̐��ÓI�����o�ł����Ď����̃T�C�Y�̂��̂��Q�Ƃ���΂���
	BoundingSphere greeningArea;		// �Ή��͈�
	static int numOfTree;				// �c���[�I�u�W�F�N�g�̑���

public:
	Tree();
	~Tree();
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// Getter
	static int getNumOfTree();			// �c���[�̐����擾
	treeNS::TreeData* getTreeData();	// �c���[�f�[�^���擾

	// Setter
	void setDataToTree(treeNS::TreeData* _treeData);
};
