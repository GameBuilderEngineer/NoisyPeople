//-----------------------------------------------------------------------------
// �c���[���N���X [Tree.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
// �X�V�� : 2019/11/12 �y���� ���z
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
	type = ObjectType::TREE;
	treeData = _treeData;
	this->position = treeData.initialPosition;
	leaf.position = treeData.initialPosition;

	// treeData.initialDirection����ɂ��������]��

	// treeData.size����ɂ����g��k��

	// ���a�擾
	BoundingSphere bs;
	bs.initialize(this->getPosition(), staticMeshNS::reference(staticMeshNS::A_TRUNK)->numBytesPerVertex);
	//���f�������a�������Ƃ��Đݒ�
	height = bs.getRadius();

	//���a��ݒ�
	radius = height / 10;

	// �d�͕t�^
	this->onGravity = true;

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
	if (this->onGravity)
	{
		D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
		this->setGravity(gravityDirection, 9.8f);
		this->speed += this->acceleration * frameTime;
		this->position += this->speed * frameTime;
		grounding();
	}

	// �I�u�W�F�N�g�̃A�b�v�f�[�g
	leaf.update();
	this->Object::update();
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
Object* Tree::getTrunk() { return this; }
int Tree::getNumOfTree(){ return numOfTree; }
TreeData* Tree::getTreeData() { return &treeData; }
float Tree::getHight() { return height; }

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
	gravityRay.update(this->position + D3DXVECTOR3(0,height,0), gravityDirection);

	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		if (height + 0.1f >= gravityRay.distance)
		{// �G�l�~�[�͒n��ɐڒn���Ă���

			// �d�͐؂�
			this->onGravity = false;
			this->setGravity(D3DXVECTOR3(0,0,0), 0);
		}
	}
}
#endif
