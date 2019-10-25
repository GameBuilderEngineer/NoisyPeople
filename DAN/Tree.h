//-----------------------------------------------------------------------------
// �c���[���N���X [Tree.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"


//=============================================================================
// ���O���
//=============================================================================
namespace treeNS
{
	// �c���[���
	enum TREE_TYPE
	{
		ANALOG_TREE,
		DIGITAL_TREE
	};

	// �Ή����
	enum GREEN_STATE
	{
		GREEN,
		DEAD
	};

	// �T�C�Y
	enum TREE_SIZE
	{
		STANDARD,
		LARGE,
		VERY_LARGE
	};

	// ���f��
	enum TREE_MODEL
	{
		A_MODEL,
		B_MODEL,
		C_MODEL,
		TREE_MAX
	};


	typedef struct TreeData
	{
		int treeID;						// ���ʔԍ�
		int model	;					// ���f��ID
		TREE_TYPE type;					// �c���[�^�C�v
		GREEN_STATE geenState;			// �Ή����
		TREE_SIZE size;					// �؂̃T�C�Y
		int hp;							// HP
		D3DXVECTOR3 initialPosition;	// �������W
		D3DXVECTOR3 initialDirection;	// �������p
		void zeroClear() { ZeroMemory(this, sizeof(TreeData)); }
	} TREESET;
}

//=============================================================================
//�N���X��`
//=======================================================================T======
class Tree
{
private:
	treeNS::TreeData treeData;			// �c���[�f�[�^
	Object leaf;						// �t�I�u�W�F�N�g
	Object trunk;						// ���I�u�W�F�N�g
	BoundingSphere greeningArea;		// �Ή��͈�
	LPD3DXMESH	attractorMesh;			// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;		// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X
	static int numOfTree;				// �c���[�I�u�W�F�N�g�̑���

public:
	Tree(treeNS::TreeData _treeData);
	~Tree();
	void update(float frameTime);
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);

	// Getter
	Object* getLeaf();
	Object* getTrunk();
	static int getNumOfTree();			// �c���[�̐����擾
	treeNS::TreeData* getTreeData();	// �c���[�f�[�^���擾

	// Setter
	void setDataToTree(treeNS::TreeData _treeData);
};