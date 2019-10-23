//-----------------------------------------------------------------------------
// �c���[�Ǘ��N���X [Tree.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "TreeManager.h"
#include "ImguiManager.h"
using namespace treeNS;


//=============================================================================
// ������
//=============================================================================
void TreeManager::initialize()
{
	nextID = 0;								// ���񔭍sID��0�ɏ�����

	// �`��I�u�W�F�N�g�̍쐬
	aTrunkRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	aLeafRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_PLAYSTATION));
	bTrunkRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	bLeafRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	cTrunkRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	cLeafRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));

#if 0
#endif
}


//=============================================================================
// �I������
//=============================================================================
void TreeManager::uninitialize()
{
	// �S�c���[�I�u�W�F�N�g��j��
	destroyAllTree();

	// �x�N�^�[�̊m�ۃ��������������i�������A���P�[�^�����ɖ߂��j
	std::vector<Tree*> temp;
	treeList.swap(temp);

	// �`��I�u�W�F�N�g�̔j��
	SAFE_DELETE(aTrunkRenderer);
	SAFE_DELETE(aLeafRenderer);
	SAFE_DELETE(bTrunkRenderer);
	SAFE_DELETE(bLeafRenderer);
	SAFE_DELETE(cTrunkRenderer);
	SAFE_DELETE(cLeafRenderer);
}


//=============================================================================
// �X�V����
//=============================================================================
void TreeManager::update(float frameTime)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->update(frameTime);
	}

	aTrunkRenderer->update();
	aLeafRenderer->update();
	bTrunkRenderer->update();
	bLeafRenderer->update();
	cTrunkRenderer->update();
	cLeafRenderer->update();
}


//=============================================================================
// �`�揈��
//=============================================================================
void TreeManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	aTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	aLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// �c���[�I�u�W�F�N�g�̍쐬
//=============================================================================
void TreeManager::createTree(TreeData treeData)
{
	Tree* tree = new Tree(treeData);	// �c���[�쐬

	// �����쐬
	tree->setTrunk(new Object);
	switch (treeData.model)
	{
	case A_MODEL:
		aTrunkRenderer->generateObject(tree->getTrunk());
		break;

	case B_MODEL:
		bTrunkRenderer->generateObject(tree->getTrunk());
		break;

	case C_MODEL:
		cTrunkRenderer->generateObject(tree->getTrunk());
		break;
	}

	// ���[�t�쐬
	if (treeData.geenState == GREEN)
	{
		createLeaf(tree->getLeaf(), treeData.model);
	}

	treeList.push_back(tree);
}


//=============================================================================
// ���[�t�쐬
//=============================================================================
void TreeManager::createLeaf(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:
		leaf = new Object;
		aLeafRenderer->generateObject(leaf);
		break;

	case B_MODEL:
		leaf = new Object;
		bLeafRenderer->generateObject(leaf);
		break;

	case C_MODEL:
		leaf = new Object;
		cLeafRenderer->generateObject(leaf);
		break;
	}
}


//=============================================================================
// ���[�t�j��
//=============================================================================
void TreeManager::destroyLeaf(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:
		aLeafRenderer->deleteObjectByID(leaf->id);
		break;

	case B_MODEL:
		aLeafRenderer->deleteObjectByID(leaf->id);
		break;

	case C_MODEL:
		aLeafRenderer->deleteObjectByID(leaf->id);
		break;
	}
}


//=============================================================================
// �S�c���[�I�u�W�F�N�g�̔j��
//=============================================================================
void TreeManager::destroyAllTree()
{
	aLeafRenderer->allDelete();
	aTrunkRenderer->allDelete();
	bLeafRenderer->allDelete();
	bTrunkRenderer->allDelete();
	cLeafRenderer->allDelete();
	cTrunkRenderer->allDelete();
	
	for (int i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}
	treeList.clear();
}



//=============================================================================
// ImGui�ɏo��
//=============================================================================
void TreeManager::outputGUI()
{
#ifdef _DEBUG

	if (ImGui::CollapsingHeader("TreeInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		//float limitTop = 1000;
		//float limitBottom = -1000;

		ImGui::Text("numOfTree:%d", Tree::getNumOfTree());

	}

#endif
}