//-----------------------------------------------------------------------------
// �c���[�Ǘ��N���X [TreeManager.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
// �X�V�� : 2019/12/22 �y���� ���z
//-----------------------------------------------------------------------------

//=============================================================================
//�y�C���N���[�h�z
//=============================================================================
#include "TreeManager.h"
#include "ImguiManager.h"
#include "TreeTools.h"
#include "UtilityFunction.h"

//=============================================================================
//�yusing�錾�z
//=============================================================================
using namespace treeNS;

//=============================================================================
//�y�������z
//=============================================================================
void TreeManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;			// ���񔭍sID��0�ɏ�����
	Tree::resetNumOfTree();//�c���[�̃J�E���g�����Z�b�g
	// �ڒn�t�B�[���h���Z�b�g
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// �`��I�u�W�F�N�g�̍쐬
	aTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_TRUNK));
	aLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_LEAF));
	bTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_TRUNK));
	bLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_LEAF));
	cTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::C_TRUNK));
	cLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::C_LEAF));

	//�A�i���O�c���[(�t)
	//���ߏ�����L���ɂ���
	aLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//�f�W�^���c���[�G�t�F�N�g
	treeEffect = new TreeEffect();
	playedDigitalTreeEffect[gameMasterNS::PLAYER_1P] = false;
	playedDigitalTreeEffect[gameMasterNS::PLAYER_2P] = false;

	gameMaster = NULL;

}

//=============================================================================
//�y�f�X�g���N�^�z
//=============================================================================
TreeManager::~TreeManager()
{
	uninitialize();
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
	//�A�i���O�c���[
	SAFE_DELETE(cLeafRenderer);
	SAFE_DELETE(cTrunkRenderer);
	SAFE_DELETE(bLeafRenderer);
	SAFE_DELETE(bTrunkRenderer);
	SAFE_DELETE(aLeafRenderer);
	SAFE_DELETE(aTrunkRenderer);

	//�f�W�^���c���[�G�t�F�N�g
	SAFE_DELETE(treeEffect);
}

//=============================================================================
// �X�V����
//=============================================================================
void TreeManager::update(float frameTime)
{
	//�Ή������{���𐔂��Ȃ���
	greeningTreeNum = 0;


	//�e�c���[�̍X�V
	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];

		bool needSwap = false;
		int beforeType = tree->getTreeData()->type;
		int beforeGreenState = tree->getTreeData()->greenState;
		//��ԑJ�ڂ��s�����Ƃ��Ă���ꍇ
		if (tree->getTransState())
		{
			//�����_���[�̐ؑւ��s��
			needSwap = true;
		}

		//�X�V
		tree->update(frameTime);

		TreeData* data = tree->getTreeData();
		TreeTable in;
		in.id = data->treeID;
		in.position = tree->position;
		in.rotation = tree->quaternion;
		in.scale = tree->scale;
		in.modelType = data->model;
		in.playBacked = false;
		in.onRecord = true;
		//�v���C���[NO�̋L�^
		in.player = tree->playerNo;

		//�Ή����
		in.greenState = data->greenState;
		
		//�l�b�g���[�N�̑��M���֋L�^
		NETWORK_CLIENT::recordTreeTable(in,i);

		//�c���[�̃X�e�[�g�ؑփC�x���g����
		if (needSwap)
		{
			//�C�x���g�̃^�C�v�̎���
			if (beforeGreenState == treeNS::DEAD)
			{
				switch (tree->getTreeData()->type)
				{
				case treeNS::DIGITAL_TREE:
					in.eventType = gameMasterNS::TO_GREEN_WITH_DIGITAL;
					break;
				case treeNS::ANALOG_TREE:
					in.eventType = gameMasterNS::TO_GREEN_WITH_ANALOG;
					break;
				}
			}
			else if (beforeGreenState == treeNS::GREEN){
				in.eventType = gameMasterNS::TO_DEAD;
			}

			//�Q�[���}�X�^�[�֋L�^
			if (gameMaster)
			{
				gameMaster->recordTreeTable(in);
			}
		}



		//�Ή����Ă���؂��J�E���g
		if (treeList[i]->getTreeData()->greenState == treeNS::GREEN)
		{
			greeningTreeNum++;
		}
	}


	//�Ή����̍X�V
	if (treeList.size() != 0) {
		greeningRate = (float)greeningTreeNum / (float)treeList.size();
	}



	//�f�W�^���c���[�G�t�F�N�g�̍X�V
	treeEffect->update(frameTime);

}

//=============================================================================
//�y�`��z
//=============================================================================
void TreeManager::render(Camera* camera)
{
#ifdef _DEBUG
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->render();
	}
#endif // _DEBUG
	
	//�S�`�����
	allUnRegister();

	//�S�`��o�^
	allRegister();

	//�A�i���O�c���[
	aTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	aLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	bTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	bLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	cTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);
	cLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), camera->view, camera->projection, camera->position);

	//�f�W�^���c���[�G�t�F�N�g�̕`��
	treeEffect->render(camera->view, camera->projection, camera->position);
}

//=============================================================================
//�y�c�[���f�[�^���g�p���ăc���[���쐬�z
//=============================================================================
void TreeManager::createUsingTool()
{
	TREE_TOOLS* treeTools = new TREE_TOOLS;
	for (int i = 0; i < treeTools->GetTreeMax(); i++)
	{
		createTree(treeTools->GetTreeSet(i));
	}
	SAFE_DELETE(treeTools);
}

//=============================================================================
//�y�c���[�I�u�W�F�N�g�̍쐬�z
//=============================================================================
void TreeManager::createTree(TreeData treeData)
{
	Tree* tree = new Tree(treeData);	// �c���[�쐬

	//�r���{�[�h�i�}�[�J�[/�W���j�̍쐬
	treeEffect->generateInstance(TreeEffectNS::INSTANCE_MARKER, tree);
	treeEffect->generateInstance(TreeEffectNS::INSTANCE_SIGN, tree);

	tree->setAttractor(attractorMesh, attractorMatrix);
	treeList.push_back(tree);
	treeNum = tree->getNumOfTree();
}

//=============================================================================
//�y�S�c���[�I�u�W�F�N�g�̔j���z
//=============================================================================
void TreeManager::destroyAllTree()
{
	// �`��S����
	allUnRegister();

	//�S�폜
	for (int i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}
	treeList.clear();
}

//=============================================================================
//�y�c���[�`��o�^�z
//=============================================================================
void TreeManager::registerTrunk(Tree* tree)
{
	//���̓o�^
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aTrunkRenderer->registerObject(tree);break;
	case B_MODEL:bTrunkRenderer->registerObject(tree);break;
	case C_MODEL:cTrunkRenderer->registerObject(tree);break;
	}
}

//=============================================================================
//�y�c���[�`������z
//=============================================================================
void TreeManager::unRegisterTrunk(Tree* tree)
{
	//���̉���
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case B_MODEL:bTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case C_MODEL:cTrunkRenderer->unRegisterObjectByID(tree->id); break;
	}
}

//=============================================================================
// ���[�t�`��o�^
//=============================================================================
void TreeManager::registerLeafRendering(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:	aLeafRenderer->registerObject(leaf);	break;
	case B_MODEL:	bLeafRenderer->registerObject(leaf);	break;
	case C_MODEL:	cLeafRenderer->registerObject(leaf);	break;
	}
}

//=============================================================================
// ���[�t�`�����
//=============================================================================
void TreeManager::unRegisterLeafRendering(Object* leaf, int _model)
{
	switch (_model)
	{
	case A_MODEL:	aLeafRenderer->unRegisterObjectByID(leaf->id);	break;
	case B_MODEL:	bLeafRenderer->unRegisterObjectByID(leaf->id);	break;
	case C_MODEL:	cLeafRenderer->unRegisterObjectByID(leaf->id);	break;
	}
}


//=============================================================================
// �S�c���[�̕`��o�^
//=============================================================================
void TreeManager::allRegister()
{
	Tree* tree = NULL;
	TreeData data;
	for (int i = 0; i < treeList.size(); i++)
	{
		tree = treeList[i];
		data = *tree->getTreeData();

		//�J�����O
		if (tree->culling)continue;

		//���̓o�^
		registerTrunk(tree);

		//�t�̓o�^
		switch (data.type)
		{
		case treeNS::ANALOG_TREE:
			//�t�̓o�^
			if(data.greenState == treeNS::GREEN)
				registerLeafRendering(tree, data.model);
			break;
		case treeNS::DIGITAL_TREE:
			//�t�̓o�^
			registerLeafRendering(tree, data.model);
			break;
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
// �S�c���[�̕`�����
//=============================================================================
void TreeManager::allUnRegister()
{
	//�A�i���O
	aLeafRenderer->allUnRegister();
	aTrunkRenderer->allUnRegister();
	bLeafRenderer->allUnRegister();
	bTrunkRenderer->allUnRegister();
	cLeafRenderer->allUnRegister();
	cTrunkRenderer->allUnRegister();

}

//=============================================================================
//�y�ʏ�r���[�ɐ؂�ւ���z
//=============================================================================
void TreeManager::switchingNormalView(int playerNo)
{
	//�A�i���O�c���[�̓��ߒl�̐ݒ�
	aTrunkRenderer->setAlpha(1.0f);
	aLeafRenderer->setAlpha(1.0f);
	bTrunkRenderer->setAlpha(1.0f);
	bLeafRenderer->setAlpha(1.0f);
	cTrunkRenderer->setAlpha(1.0f);
	cLeafRenderer->setAlpha(1.0f);

	//�A�i���O�c���[
	aTrunkRenderer->setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	aLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bTrunkRenderer->setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	bLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cTrunkRenderer->setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	cLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	aTrunkRenderer->enableRender();
	aLeafRenderer->enableRender();
	bTrunkRenderer->enableRender();
	bLeafRenderer->enableRender();
	cTrunkRenderer->enableRender();
	cLeafRenderer->enableRender();


	//�c���[�G�t�F�N�g
	treeEffect->disableRender();

	//�f�W�^���c���[�G�t�F�N�g�̔�\��
	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];
		if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
		{
			//�ʏ�`�掞�͕`�悵�Ȃ�
			tree->switchingShownDigitalEffect(false, playerNo);
		}
	}
};

//=============================================================================
//�y�r�W�����r���[�ɐ؂�ւ���z
//=============================================================================
void TreeManager::switchingVisionView(int playerNo)
{
	//�`�悵�Ȃ�
	aTrunkRenderer->disableRender();
	aLeafRenderer->disableRender();
	bTrunkRenderer->disableRender();
	bLeafRenderer->disableRender();
	cTrunkRenderer->disableRender();
	cLeafRenderer->disableRender();

	//�c���[�G�t�F�N�g
	treeEffect->enableRender();

	//�f�W�^���c���[�G�t�F�N�g�̕\��
	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];
		if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
		{
			if (tree->getSelected(playerNo))
			{
				//�I������Ă���ꍇ�́A���C�g��\�����Ȃ�
				tree->switchingShownDigitalEffect(false, playerNo);
			}
			else {
				//�I������Ă��Ȃ���΃f�W�^���c���[�G�t�F�N�g��\������
				tree->switchingShownDigitalEffect(true, playerNo);
			}
		}
	}

};

//=============================================================================
//�y�f�W�^���c���[�p�̃G�t�F�N�g���Đ�����z
//=============================================================================
void TreeManager::playDigitalTreeEffect(int playerNo)
{
	if (playedDigitalTreeEffect[playerNo] == true)return;

	playedDigitalTreeEffect[playerNo] = true;

	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];
		if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
		{
			treeEffect->playStandardEffect(&tree->position);
		}
	}

}

//=============================================================================
//�y�f�W�^���c���[�p�̃G�t�F�N��~�₷��z
//=============================================================================
void TreeManager::stopDigitalTreeEffect(int playerNo)
{
	if (playedDigitalTreeEffect[playerNo] == false)return;

	playedDigitalTreeEffect[playerNo] = false;

	for (size_t i = 0; i < treeList.size(); i++)
	{
		Tree* tree = treeList[i];
		if (tree->getTreeData()->type = treeNS::DIGITAL_TREE)
		{
			treeEffect->playStandardEffect(&tree->position);
		}
	}
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
// �c���[��T��
//=============================================================================
Tree* TreeManager::findTree(int _treeID)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		if (treeList[i]->getTreeData()->treeID == _treeID)
		{
			return treeList[i];
		}
	}

	return NULL;
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

		ImGui::Text("numOfTree:%d", Tree::getNumOfTree());
		ImGui::Text("greeningRate:%.02f%", greeningRate*100.0f);
		
		ImGui::Text("TrunkA[Num:%d]",	aTrunkRenderer->getObjectNum());
		ImGui::Text("TrunkB[Num:%d]",	bTrunkRenderer->getObjectNum());
		ImGui::Text("TrunkC[Num:%d]",	cTrunkRenderer->getObjectNum());
		ImGui::Text("LeafA[Num:%d]",	aLeafRenderer->getObjectNum());
		ImGui::Text("LeafB[Num:%d]",	bLeafRenderer->getObjectNum());
		ImGui::Text("LeafC[Num:%d]",	cLeafRenderer->getObjectNum());

	}
#endif
}

//=============================================================================
//�ySetter�z
//=============================================================================
void TreeManager::setGameMaster(GameMaster* gameMaster) { this->gameMaster = gameMaster; }

//=============================================================================
//�yGetter�z
//=============================================================================
std::vector<Tree*>& TreeManager::getTreeList(){ return treeList; }
float TreeManager::getGreeningRate() { return greeningRate; }
int TreeManager::getTreeNum() { return treeNum; }