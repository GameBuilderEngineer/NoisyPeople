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
	aTrunkRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_PLAYSTATION));
	aLeafRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	bTrunkRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	bLeafRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	cTrunkRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	cLeafRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DEAD_TREE));

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

	// ���̕`����Z�b�g
	switch (treeData.model)
	{
	case A_MODEL:
		aTrunkRenderer->registerObject(tree->getTrunk());
		break;

	case B_MODEL:
		bTrunkRenderer->registerObject(tree->getTrunk());
		break;

	case C_MODEL:
		cTrunkRenderer->registerObject(tree->getTrunk());
		break;
	}

	// �t�̕`����Z�b�g
	if (treeData.geenState != GREEN) return;
	switch (treeData.model)
	{
	case A_MODEL:
		aLeafRenderer->registerObject(tree->getLeaf());
		break;

	case B_MODEL:
		bLeafRenderer->registerObject(tree->getLeaf());
		break;

	case C_MODEL:
		cLeafRenderer->registerObject(tree->getLeaf());
		break;
	}

	treeList.push_back(tree);
}


//=============================================================================
// ���[�t�`��o�^
//=============================================================================
void TreeManager::registerLeafRendering(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:
		aLeafRenderer->registerObject(leaf);
		break;

	case B_MODEL:
		bLeafRenderer->registerObject(leaf);
		break;

	case C_MODEL:
		cLeafRenderer->registerObject(leaf);
		break;
	}
}


//=============================================================================
// ���[�t�`�����
//=============================================================================
void TreeManager::unRegisterLeafRendering(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:
		aLeafRenderer->unRegisterObjectByID(leaf->id);
		break;

	case B_MODEL:
		aLeafRenderer->unRegisterObjectByID(leaf->id);
		break;

	case C_MODEL:
		aLeafRenderer->unRegisterObjectByID(leaf->id);
		break;
	}
}


//=============================================================================
// �S�c���[�I�u�W�F�N�g�̔j��
//=============================================================================
void TreeManager::destroyAllTree()
{
	// �`��S����
	aLeafRenderer->allUnRegister();
	aTrunkRenderer->allUnRegister();
	bLeafRenderer->allUnRegister();
	bTrunkRenderer->allUnRegister();
	cLeafRenderer->allUnRegister();
	cTrunkRenderer->allUnRegister();

	for (int i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}
	treeList.clear();
}


//=============================================================================
// �c���[ID�𔭍s����
//=============================================================================
int TreeManager::issueNewTreeID()
{
	int ans = nextID;
	nextID++;
	return ans;
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


//=============================================================================
// Getter
//=============================================================================
std::vector<Tree*>& TreeManager::getTreeList(){ return treeList; }
