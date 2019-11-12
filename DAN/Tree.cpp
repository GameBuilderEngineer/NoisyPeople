//-----------------------------------------------------------------------------
// �c���[���N���X [Tree.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
#include "Tree.h"
#include "ImguiManager.h"
using namespace treeNS;


int Tree::numOfTree = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Tree::Tree(treeNS::TreeData _treeData)
{
	treeData = _treeData;
	trunk.position = treeData.initialPosition;
	leaf.position = treeData.initialPosition;

	// treeData.initialDirection����ɂ��������]��

	// treeData.size����ɂ����g��k��

	// ���a�擾
	BoundingSphere bs;
	bs.initialize(trunk.getPosition(), staticMeshNS::reference(staticMeshNS::A_TRUNK)->numBytesPerVertex);
	radius = bs.getRadius();

	// �d�͕t�^
	trunk.onGravity = true;

	//// �X�P�[���ύX
	//trunk.scale *= 0.4f;
	//leaf.scale *= 0.4f;

	numOfTree++;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Tree::~Tree()
{
	numOfTree--;
}


//=============================================================================
// �X�V����
//=============================================================================
void Tree::update(float frameTime)
{
	// �d��
	if (trunk.onGravity)
	{
		D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
		trunk.setGravity(gravityDirection, 9.8f);
		trunk.speed += trunk.acceleration * frameTime;
		trunk.position += trunk.speed * frameTime;
		grounding();

	}

	// �I�u�W�F�N�g�̃A�b�v�f�[�g
	leaf.update();
	trunk.update();
}


//=============================================================================
// �d�͔������b�V���i�ڒn���b�V���j�̐ݒ�
//=============================================================================
void Tree::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}


//=============================================================================
// Getter
//=============================================================================
Object* Tree::getLeaf() { return &leaf; }
Object* Tree::getTrunk() { return &trunk; }
int Tree::getNumOfTree(){ return numOfTree; }
TreeData* Tree::getTreeData() { return &treeData; }


//=============================================================================
// Setter
//=============================================================================
void Tree::setDataToTree(TreeData _treeData) { treeData = _treeData; }


#ifdef _DEBUG
//=============================================================================
// �ڒn����
//=============================================================================
void Tree::grounding()
{
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	Ray gravityRay;
	gravityRay.update(trunk.position + D3DXVECTOR3(0,radius,0), gravityDirection);

	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (radius + 0.1f >= gravityRay.distance)
		{// �G�l�~�[�͒n��ɐڒn���Ă���

			// �d�͐؂�
			trunk.onGravity = false;
			trunk.setGravity(D3DXVECTOR3(0,0,0), 0);
		}
	}
}
#endif
