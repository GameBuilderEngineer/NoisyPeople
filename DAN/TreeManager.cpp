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

	// �`��I�u�W�F�N�g�̍쐬
	//�f�W�^���c���[
	aDTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_TRUNK));
	aDLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::A_LEAF));
	bDTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_TRUNK));
	bDLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::B_LEAF));
	cDTrunkRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	cDLeafRenderer	= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DEAD_TREE));

	//�A�i���O�c���[(�t)
	//���ߏ�����L���ɂ���
	aLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//�f�W�^���c���[(�t)
	//���ߏ�����L���ɂ���
	aDLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bDLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cDLeafRenderer->setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//�f�W�^���c���[�G�t�F�N�g
	digitalTreeEffect = new DigitalTreeEffect();
	playedDigitalTreeEffect[gameMasterNS::PLAYER_1P] = false;
	playedDigitalTreeEffect[gameMasterNS::PLAYER_2P] = false;
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
	//�A�i���O�c���[
	SAFE_DELETE(cLeafRenderer);
	SAFE_DELETE(cTrunkRenderer);
	SAFE_DELETE(bLeafRenderer);
	SAFE_DELETE(bTrunkRenderer);
	SAFE_DELETE(aLeafRenderer);
	SAFE_DELETE(aTrunkRenderer);
	//�f�W�^���c���[
	SAFE_DELETE(cDLeafRenderer);
	SAFE_DELETE(cDTrunkRenderer);
	SAFE_DELETE(bDLeafRenderer);
	SAFE_DELETE(bDTrunkRenderer);
	SAFE_DELETE(aDLeafRenderer);
	SAFE_DELETE(aDTrunkRenderer);

	//�f�W�^���c���[�G�t�F�N�g
	SAFE_DELETE(digitalTreeEffect);
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
		//��ԑJ�ڂ��s�����Ƃ��Ă���ꍇ
		if (tree->getTransState())
		{
			//�����_���[�̐ؑւ��s��
			needSwap = true;
		}

		//�X�V
		tree->update(frameTime);

		//�����_���[�̐ؑ�
		if (needSwap)
		{
			swapDA(tree,beforeType);
		}

		//�Ή����Ă���؂��J�E���g
		if (treeList[i]->getTreeData()->greenState == treeNS::GREEN)
		{
			greeningTreeNum++;
		}

		//�f�W�^���c���[�̎��͂ɃG�t�F�N�g�𔭐�
		//if (treeList[i]->getTreeData()->type == treeNS::DIGITAL_TREE)
		//{
		//	//�G�t�F�N�g�̐���
		//	digitalTreeEffect->generateInstance(treeList[i]->position);
		//}
	}



	//�Ή����̍X�V
	if (treeList.size() != 0) {
		greeningRate = (float)greeningTreeNum / (float)treeList.size();
	}


	//�����_���[�̍X�V
	aTrunkRenderer->update();
	aLeafRenderer->update();
	bTrunkRenderer->update();
	bLeafRenderer->update();
	cTrunkRenderer->update();
	cLeafRenderer->update();

	//�����_���[�̍X�V
	aDTrunkRenderer->update();
	aDLeafRenderer->update();
	bDTrunkRenderer->update();
	bDLeafRenderer->update();
	cDTrunkRenderer->update();
	cDLeafRenderer->update();

	//�f�W�^���c���[�G�t�F�N�g�̍X�V
	digitalTreeEffect->update(frameTime);

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

	//�A�i���O�c���[
	aTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	aLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);

	//�f�W�^���c���[
	aDTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	aDLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bDTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bDLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cDTrunkRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	cDLeafRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);

	//�f�W�^���c���[�G�t�F�N�g�̕`��
	digitalTreeEffect->render(view, projection, cameraPosition);
}

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
			digitalTreeEffect->playStandardEffect(&tree->position);
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
			digitalTreeEffect->playStandardEffect(&tree->position);
		}
	}
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
	aTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	aLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	bLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	cLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//�f�W�^���c���[
	aDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	aDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	bDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	cDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//�f�W�^���c���[�G�t�F�N�g�̔�\��
	//for (size_t i = 0; i < treeList.size(); i++)
	//{
	//	Tree* tree = treeList[i];
	//	if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
	//	{
	//		//�{���́A�߂��ꍇ�̂݃f�W�^���c���[�G�t�F�N�g��\������
	//
	//		//�ʏ�`�掞�͕`�悵�Ȃ�
	//		tree->switchingShownDigitalEffect(true, playerNo);
	//
	//
	//	}
	//}
};

//=============================================================================
//�y�r�W�����r���[�ɐ؂�ւ���z
//=============================================================================
void TreeManager::switchingVisionView(int playerNo)
{

	//�A�i���O�c���[�̓��ߒl�̐ݒ�
	aTrunkRenderer->setAlpha(0.3f);
	aLeafRenderer->setAlpha(0.3f);
	bTrunkRenderer->setAlpha(0.3f);
	bLeafRenderer->setAlpha(0.3f);
	cTrunkRenderer->setAlpha(0.3f);
	cLeafRenderer->setAlpha(0.3f);
	//�A�i���O�c���[
	aTrunkRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	aLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bTrunkRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cTrunkRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cLeafRenderer->		setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//�f�W�^���c���[
	aDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	aDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	bDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	bDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);
	cDTrunkRenderer->	setRenderPass(staticMeshRendererNS::LAMBERT_PASS);
	cDLeafRenderer->	setRenderPass(staticMeshRendererNS::TRANSPARENT_PASS);

	//�f�W�^���c���[�G�t�F�N�g�̕\��
	//for (size_t i = 0; i < treeList.size(); i++)
	//{
	//	Tree* tree = treeList[i];
	//	if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
	//	{
	//		if (tree->getSelected(playerNo))
	//		{
	//			//�I������Ă���ꍇ�́A���C�g��\�����Ȃ�
	//			tree->switchingShownDigitalEffect(true,playerNo);
	//		}
	//		else {
	//			//�I������Ă��Ȃ���΃f�W�^���c���[�G�t�F�N�g��\������
	//			tree->switchingShownDigitalEffect(false,playerNo);//false�ōőO�ʃ��C�g
	//		}
	//	}
	//}

};

//=============================================================================
//�y���C���[�t���[���`��ɐ؂�ւ���z
//=============================================================================
void TreeManager::changeWireFrame()
{
	aTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	aLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	bTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	bLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	cTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	cLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	aTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	aLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	bTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	bLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);
	cTrunkRenderer->setFillMode(staticMeshRendererNS::WIREFRAME);
	cLeafRenderer->	setFillMode(staticMeshRendererNS::WIREFRAME);

};

//=============================================================================
//�y�\���b�h�`��ɐ؂�ւ���z
//=============================================================================
void TreeManager::changeSolid()
{
	aTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	aLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	bTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	bLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	cTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	cLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	aTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	aLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	bTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	bLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
	cTrunkRenderer->setFillMode(staticMeshRendererNS::SOLID);
	cLeafRenderer->	setFillMode(staticMeshRendererNS::SOLID);
};

//=============================================================================
// �c���[�I�u�W�F�N�g�̍쐬
//=============================================================================
void TreeManager::createTree(TreeData treeData)
{
	Tree* tree = new Tree(treeData);	// �c���[�쐬
	
	//�A�i���O�c���[�����_���[�֓o�^
	registerAnalog(tree);

	tree->setAttractor(attractorMesh, attractorMatrix);
	treeList.push_back(tree);
}

//=============================================================================
//�y(�A�i���O<->�f�W�^��)�����_���[�̌����z
//=============================================================================
void TreeManager::swapDA(Tree* tree,int beforeType)
{
	if (tree->getTreeData()->type == beforeType)
	{
		switch (tree->getTreeData()->type)
		{
		case ANALOG_TREE:
			unRegisterAnalog(tree);
			registerAnalog(tree);
			break;
		case DIGITAL_TREE:
			unRegisterDigital(tree);
			registerDigital(tree);
			break;
		}
		return;
	}

	switch (tree->getTreeData()->type)
	{
	case ANALOG_TREE:
		registerAnalog(tree);
		unRegisterDigital(tree);
		break;
	case DIGITAL_TREE:
		registerDigital(tree);
		unRegisterAnalog(tree);
		break;
	}
}


//=============================================================================
//�y�f�W�^���c���[�`��o�^�z
//=============================================================================
void TreeManager::registerDigital(Tree* tree)
{
	//���̓o�^
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aDTrunkRenderer->registerObject(tree);break;
	case B_MODEL:bDTrunkRenderer->registerObject(tree);break;
	case C_MODEL:cDTrunkRenderer->registerObject(tree);break;
	}

	//�t�̓o�^
	tree->getLeaf()->onActive = true;
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aDLeafRenderer->registerObject(tree->getLeaf());break;
	case B_MODEL:bDLeafRenderer->registerObject(tree->getLeaf());break;
	case C_MODEL:cDLeafRenderer->registerObject(tree->getLeaf());break;
	}
}

//=============================================================================
//�y�f�W�^���c���[�`������z
//=============================================================================
void TreeManager::unRegisterDigital(Tree* tree)
{
	//���̉���
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aDTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case B_MODEL:bDTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case C_MODEL:cDTrunkRenderer->unRegisterObjectByID(tree->id); break;
	}

	//�t�̉���
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aDLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	case B_MODEL:bDLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	case C_MODEL:cDLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	}
}

//=============================================================================
//�y�A�i���O�c���[�`��o�^�z
//=============================================================================
void TreeManager::registerAnalog(Tree* tree)
{
	//���̓o�^
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aTrunkRenderer->registerObject(tree);break;
	case B_MODEL:bTrunkRenderer->registerObject(tree);break;
	case C_MODEL:cTrunkRenderer->registerObject(tree);break;
	}

	//�t�̓o�^
	if (tree->getTreeData()->greenState == GREEN) {
		tree->getLeaf()->onActive = true;
		switch (tree->getTreeData()->model)
		{
		case A_MODEL:aLeafRenderer->registerObject(tree->getLeaf());break;
		case B_MODEL:bLeafRenderer->registerObject(tree->getLeaf());break;
		case C_MODEL:cLeafRenderer->registerObject(tree->getLeaf());break;
		}
	}

}

//=============================================================================
//�y�A�i���O�c���[�`������z
//=============================================================================
void TreeManager::unRegisterAnalog(Tree* tree)
{
	//���̉���
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case B_MODEL:bTrunkRenderer->unRegisterObjectByID(tree->id); break;
	case C_MODEL:cTrunkRenderer->unRegisterObjectByID(tree->id); break;
	}

	//�t�̉���
	switch (tree->getTreeData()->model)
	{
	case A_MODEL:aLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	case B_MODEL:bLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
	case C_MODEL:cLeafRenderer->unRegisterObjectByID(tree->getLeaf()->id); break;
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
// �S�c���[�I�u�W�F�N�g�̔j��
//=============================================================================
void TreeManager::destroyAllTree()
{
	// �`��S����
	//�A�i���O
	aLeafRenderer->allUnRegister();
	aTrunkRenderer->allUnRegister();
	bLeafRenderer->allUnRegister();
	bTrunkRenderer->allUnRegister();
	cLeafRenderer->allUnRegister();
	cTrunkRenderer->allUnRegister();

	//�f�W�^��
	aDLeafRenderer->allUnRegister();
	aDTrunkRenderer->allUnRegister();
	bDLeafRenderer->allUnRegister();
	bDTrunkRenderer->allUnRegister();
	cDLeafRenderer->allUnRegister();
	cDTrunkRenderer->allUnRegister();

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

		ImGui::Text("numOfTree:%d", Tree::getNumOfTree());
		ImGui::Text("greeningRate:%.02f%", greeningRate*100.0f);
		
		ImGui::Text("AnalogTrunkA[Num:%d]",	aTrunkRenderer->getObjectNum());
		ImGui::Text("AnalogTrunkB[Num:%d]",	bTrunkRenderer->getObjectNum());
		ImGui::Text("AnalogTrunkC[Num:%d]",	cTrunkRenderer->getObjectNum());
		ImGui::Text("AnalogLeafA[Num:%d]",	aLeafRenderer->getObjectNum());
		ImGui::Text("AnalogLeafB[Num:%d]",	bLeafRenderer->getObjectNum());
		ImGui::Text("AnalogLeafC[Num:%d]",	cLeafRenderer->getObjectNum());

		ImGui::Text("DigitalTrunkA[Num:%d]", aDTrunkRenderer->getObjectNum());
		ImGui::Text("DigitalTrunkB[Num:%d]", bDTrunkRenderer->getObjectNum());
		ImGui::Text("DigitalTrunkC[Num:%d]", cDTrunkRenderer->getObjectNum());
		ImGui::Text("DigitalLeafA[Num:%d]", aDLeafRenderer->getObjectNum());
		ImGui::Text("DigitalLeafB[Num:%d]", bDLeafRenderer->getObjectNum());
		ImGui::Text("DigitalLeafC[Num:%d]", cDLeafRenderer->getObjectNum());

		bTrunkRenderer->outputGUI();

	}
#endif
}


//=============================================================================
// Getter
//=============================================================================
std::vector<Tree*>& TreeManager::getTreeList(){ return treeList; }
float TreeManager::getGreeningRate() { return greeningRate; }
