//-----------------------------------------------------------------------------
// �c���[�Ǘ��N���X [Tree.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
// �X�V�� : 2019/11/12 �y���� ���z
//-----------------------------------------------------------------------------
#include "TreeManager.h"
#include "ImguiManager.h"
using namespace treeNS;


//=============================================================================
// ������
//=============================================================================
void TreeManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;			// ���񔭍sID��0�ɏ�����

	// �ڒn�t�B�[���h���Z�b�g
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// �`��I�u�W�F�N�g�̍쐬
	aTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_TRUNK));
	aLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_LEAF));
	bTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_TRUNK));
	bLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_LEAF));
	cTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	cLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DEAD_TREE));

	//�t�̃��C�g��؂�
	aLeafRenderer->disableLight();
	bLeafRenderer->disableLight();
	cLeafRenderer->disableLight();
	//���ߏ�����L���ɂ���
	aLeafRenderer->enableTransparent();
	bLeafRenderer->enableTransparent();
	cLeafRenderer->enableTransparent();

#if 0	// �c���[�c�[���̃f�[�^��ǂݍ���

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
	SAFE_DELETE(cLeafRenderer);
	SAFE_DELETE(cTrunkRenderer);
	SAFE_DELETE(bLeafRenderer);
	SAFE_DELETE(bTrunkRenderer);
	SAFE_DELETE(aLeafRenderer);
	SAFE_DELETE(aTrunkRenderer);
}


//=============================================================================
// �X�V����
//=============================================================================
void TreeManager::update(float frameTime)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->update(frameTime);

		//�t�̕`���Ԃ�؂�ւ���
		if (treeList[i]->getLeaf()->onActive) 
		{//�`�悳��Ă���ꍇ
			//�͖؏��
			if (treeList[i]->getTreeData()->greenState == treeNS::DEAD)
			{
				treeList[i]->getLeaf()->onActive = false;//�A�N�e�B�u��
				//�����_���[�֓o�^
				unRegisterLeafRendering(
					treeList[i]->getLeaf(),				//�t�I�u�W�F�N�g
					treeList[i]->getTreeData()->model);	//���f�����
			}
		}
		else 
		{//�`�悳��Ă��Ȃ��ꍇ
			//�Ή����
			if (treeList[i]->getTreeData()->greenState == treeNS::GREEN)
			{
				treeList[i]->getLeaf()->onActive = true;//�A�N�e�B�u��
				//�����_���[�֓o�^
				registerLeafRendering(
					treeList[i]->getLeaf(),				//�t�I�u�W�F�N�g
					treeList[i]->getTreeData()->model);	//���f�����
				
			}
		}
	}

	//�����_���[�̍X�V
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
#ifdef _DEBUG
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->render();
	}
#endif // _DEBUG

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
	if (treeData.greenState == GREEN)
	{
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
	}
	tree->setAttractor(attractorMesh, attractorMatrix);
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
