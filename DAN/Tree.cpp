//-----------------------------------------------------------------------------
// �c���[���N���X [Tree.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
// �X�V�� : 2019/11/17 �y���� ���z
//-----------------------------------------------------------------------------

//=============================================================================
//�y�C���N���[�h�z
//=============================================================================
#include "Tree.h"
#include "ImguiManager.h"
#include "UtilityFunction.h"

//=============================================================================
//�yusing�錾�z
//=============================================================================
using namespace treeNS;

#pragma region Tree
//=============================================================================
//�y�O���[�o���ϐ��z
//=============================================================================
int Tree::numOfTree = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Tree::Tree(treeNS::TreeData _treeData)
{
	{//�I�u�W�F�N�g�^�C�v�ƏՓˑΏۂ̎w��
		using namespace ObjectType;
		treeCell.type = TREE;
		treeCell.target = PLAYER|ENEMY|BULLET;
	}

	treeData = _treeData;
	this->position = treeData.initialPosition;
	leaf.position = treeData.initialPosition;
	leaf.onActive = false;//�����l�͔�A�N�e�B�u


	// treeData.initialDirection����ɂ��������]��
	//postureControl(axisZ.direction, treeData.initialDirection, 1.0f);

	// treeData.size����ɂ����g��k��
	switch (treeData.size)
	{
	case STANDARD:
		scale = D3DXVECTOR3(1.0f, 0.5f, 1.0f);
		break;
	case LARGE:
		scale = D3DXVECTOR3(5.0f, 1.5f, 5.0f);
		break;
	case VERY_LARGE:
		scale = D3DXVECTOR3(10.0f, 3.0f, 10.0f);
		break;
	}

	leaf.scale = scale;
	
	//�T�C�Y�̐ݒ�
	setSize(D3DXVECTOR3(1.5f, 20.0f, 1.5f));

	//���a�̐ݒ�
	setRadius(size.y);

	// �d�͕t�^
	this->onGravity = true;

	//// �X�P�[���ύX
	//trunk.scale *= 0.4f;
	//leaf.scale *= 0.4f;

	//���݂͗Ή����łȂ�
	nowAroundGreening = false;

	state = new AnalogState(this);
	onTransState = false;
	numOfTree++;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Tree::~Tree()
{
	numOfTree--;
	SAFE_DELETE(state);
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

	//��ԕʍX�V
	state->update(frameTime);

	//��ԑJ��
	if (onTransState)
	{
		AbstractState* tmp = state->transition();
		SAFE_DELETE(state);
		state = tmp;
		onTransState = false;
	}

	// �����������ǋL������
	if (treeData.hp <= 0)
	{
		treeData.isAttaked = false;
		// �A�i���O�c���[�ɖ߂��A
		// �Ή��͈͂��͂�؂ɖ߂��Ă��������I
	}
	
	// �I�u�W�F�N�g�̃A�b�v�f�[�g
	leaf.update();
	this->Object::update();
}

//=============================================================================
//�y���ӂ̗Ή������i�f�W�^�������j�z
//=============================================================================
void Tree::render()
{
#ifdef _DEBUG
	if (treeData.type == treeNS::DIGITAL_TREE) {
		greeningArea.render();
	}
	box->render(matrixCenter);
#endif
}

//=============================================================================
//�y���ӂ̗Ή������i�f�W�^�������j�z
//=============================================================================
void Tree::greeningAround()
{	
	greeningArea.initialize(&center);
	greeningArea.setRadius(1.0f);
	nowAroundGreening = true;
}

//=============================================================================
//�y��ԑJ�ڂ��s���z
//=============================================================================
void Tree::transState()
{	
	onTransState = true;
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
Object* Tree::getGreeningArea() { return &greeningArea; }
int Tree::getNumOfTree(){ return numOfTree; }
TreeData* Tree::getTreeData() { return &treeData; }
LPD3DXMESH Tree::getMesh() {
	using namespace staticMeshNS;
	StaticMesh model;
	switch (treeData.model)
	{
	case treeNS::A_MODEL:	model = *reference(A_TRUNK);		break;
	case treeNS::B_MODEL:	model = *reference(B_TRUNK);		break;
	case treeNS::C_MODEL:	model = *reference(GREEN_TREE_002);	break;
	default:				model = *reference(A_TRUNK);		break;
	}
	return model.mesh;
};

bool Tree::isAroundGreening()		{ return nowAroundGreening; }

//=============================================================================
// Setter
//=============================================================================
void Tree::setDataToTree(TreeData _treeData) { treeData = _treeData; }
void Tree::addHp(int value) { 
	if (treeData.greenState == treeNS::GREEN)return;//�Ή����Ă���Ή��Z���Ȃ�
	treeData.hp += value; 
	if (treeData.hp >=MAX_HP)
	{
		transState();//��ԑJ��
	}
}
//�Ή��G���A�̃X�P�[����ݒ�
void Tree::setGreeningArea(float value)
{
	greeningArea.sphere->setScale(value);
}
//���͂ւ̗Ή����I��
void Tree::disableAroundGreening()	{ 
	nowAroundGreening = false; 
	greeningArea.treeCell.remove();//�Փˋ�Ԃ��痣�E
}


//=============================================================================
// �ڒn����
//=============================================================================
void Tree::grounding()
{
	D3DXVECTOR3 gravityDirection = D3DXVECTOR3(0, -1, 0);
	Ray gravityRay;
	gravityRay.update(this->position, gravityDirection);

	if (gravityRay.rayIntersect(attractorMesh, *attractorMatrix))
	{
		position.y -= gravityRay.distance;
		leaf.position = position;
		// �d�͐؂�
		this->onGravity = false;
		this->setGravity(D3DXVECTOR3(0,0,0), 0);
	}
	else {
		position =
			D3DXVECTOR3(rand() % 400, 150, rand() % 480);
		position -= D3DXVECTOR3(200, 0, 240);
	}
}
#pragma endregion

#pragma region DigitalState
//=============================================================================
//�y�R���X�g���N�^�F�f�W�^����ԁz
//=============================================================================
DigitalState::DigitalState(Tree* target)
{
	tree = target;
	start();
}

//=============================================================================
//�y�f�X�g���N�^�F�f�W�^����ԁz
//=============================================================================
DigitalState::~DigitalState()
{
}

//=============================================================================
//�y�J�n�F�f�W�^����ԁz
//=============================================================================
void DigitalState::start()
{
	aroundGreenTimer = 0.0f;

	switch (tree->getTreeData()->size)
	{
	case treeNS::TREE_SIZE::STANDARD:
		aroundGreenRange = AROUND_GREEN_RANGE_S; break;
	case treeNS::TREE_SIZE::LARGE:break;
		aroundGreenRange = AROUND_GREEN_RANGE_L; break;
	case treeNS::TREE_SIZE::VERY_LARGE:break;
		aroundGreenRange = AROUND_GREEN_RANGE_V; break;
	}

	//���͂ւ̗Ή����J�n����
	tree->greeningAround();
}

//=============================================================================
//�y�I���F�f�W�^����ԁz
//=============================================================================
void DigitalState::end()
{

}

//=============================================================================
//�y�X�V�F�f�W�^����ԁz
//=============================================================================
void DigitalState::update(float frameTime)
{
	if (aroundGreenTimer < AROUND_GREEN_TIME)
	{
		aroundGreenTimer += frameTime;
		//�Ή����ԏI��
		if (aroundGreenTimer > AROUND_GREEN_TIME)
		{
			aroundGreenTimer = AROUND_GREEN_TIME;	//�^�C�}�[��~
			tree->disableAroundGreening();			//���͗Ή��t���O��؂�
		}
	}
	float rate = aroundGreenTimer/AROUND_GREEN_TIME;
	tree->setGreeningArea(UtilityFunction::lerp(1.0f, aroundGreenRange, rate));
}

//=============================================================================
//�y��ԑJ�ځF�f�W�^����ԁz
//=============================================================================
AbstractState* DigitalState::transition()
{
	return new AnalogState(tree);
}

#pragma endregion

#pragma region AnalogState
//=============================================================================
//�y�R���X�g���N�^�F�A�i���O��ԁz
//=============================================================================
AnalogState::AnalogState(Tree* target)
{
	tree = target;
	start();
}

//=============================================================================
//�y�f�X�g���N�^�F�A�i���O��ԁz
//=============================================================================
AnalogState::~AnalogState()
{

}

//=============================================================================
//�y�J�n�F�A�i���O��ԁz
//=============================================================================
void AnalogState::start()
{
	tree->getTreeData()->type = treeNS::ANALOG_TREE;
	tree->getTreeData()->greenState = treeNS::DEAD;
}

//=============================================================================
//�y�I���F�A�i���O��ԁz
//=============================================================================
void AnalogState::end()
{

}

//=============================================================================
//�y�X�V�F�A�i���O��ԁz
//=============================================================================
void AnalogState::update(float frameTime)
{

}

//=============================================================================
//�y��ԑJ�ځF�A�i���O��ԁz
//=============================================================================
AbstractState* AnalogState::transition()
{
	//�Ή�����
	tree->getTreeData()->greenState = treeNS::GREEN;
	//HP�̑����ɂ��Ή�
	if (tree->getTreeData()->hp >= MAX_HP)
	{
		//�f�W�^����
		tree->getTreeData()->type = treeNS::DIGITAL_TREE;
		return new DigitalState(tree);

	}
	else {
		//�A�i���O��Ԃ̂܂ܗΉ�
		tree->getTreeData()->type = treeNS::ANALOG_TREE;
		return new GreenState(tree);
	}
}

#pragma endregion

#pragma region GreenState
//=============================================================================
//�y�R���X�g���N�^�F�Ή���ԁz
//=============================================================================
GreenState::GreenState(Tree* target)
{
	tree = target;
	start();
}

//=============================================================================
//�y�f�X�g���N�^�F�Ή���ԁz
//=============================================================================
GreenState::~GreenState()
{

}

//=============================================================================
//�y�J�n�F�Ή���ԁz
//=============================================================================
void GreenState::start()
{
	//�Ή������A�i���O�c���[
	tree->getTreeData()->greenState = treeNS::GREEN;
	tree->getTreeData()->type = treeNS::ANALOG_TREE;
}

//=============================================================================
//�y�I���F�Ή���ԁz
//=============================================================================
void GreenState::end()
{

}

//=============================================================================
//�y�X�V�F�Ή���ԁz
//=============================================================================
void GreenState::update(float frameTime)
{

}

//=============================================================================
//�y�X�V�F�Ή���ԁz
//=============================================================================
AbstractState* GreenState::transition()
{
	//�ēx�A�i���O��Ԃɖ߂�
	return new AnalogState(tree);
}

#pragma endregion

