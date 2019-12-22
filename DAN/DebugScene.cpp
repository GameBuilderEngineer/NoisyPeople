#include "DebugScene.h"
#include "CollisionManager.h"

using namespace debugNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
DebugScene::DebugScene()
{
	//���`�W���؋�ԕ����Ǘ��N���X
	linear8TreeManager = new Linear8TreeManager<Object>;
	linear8TreeManager->initialize(5, D3DXVECTOR3(-3000, -2000, -3000), D3DXVECTOR3(3000, 3000, 3000));

	//�I�u�W�F�N�g�J�E���^�[�̃��Z�b�g
	objectNS::resetCounter();

	sceneName = "Scene -Debug-";
	nextScene = SceneList::SPLASH;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
DebugScene::~DebugScene()
{

}


//=============================================================================
// ������
//=============================================================================
void DebugScene::initialize()
{
	// �e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	// �v���C���[
	player = new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer = new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer = new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

	// �c���[
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	treeManager->setGameMaster(gameMaster);

	// �c���[���c�[���������ɐݒu����
	treeManager->createUsingTool();

	// camera
	camera = new Camera[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//�J�����̐ݒ�
		camera[i].initialize(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getCameraGaze());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(CAMERA_RELATIVE_GAZE);
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		camera[i].setFieldOfView((D3DX_PI / 180) * 90);
		camera[i].setLimitRotationTop(0.1f);
		camera[i].setLimitRotationBottom(0.3f);
		camera[i].updateOrtho();

		// �v���C���[�̐ݒ�
		PlayerTable infomation;
		switch (i)
		{
		case gameMasterNS::PLAYER_1P:
			infomation.playerType = gameMasterNS::PLAYER_1P;
			infomation.modelType = gameMasterNS::MODEL_FEMALE;
			player[i].initialize(infomation);
			break;
		case gameMasterNS::PLAYER_2P:
			infomation.playerType = gameMasterNS::PLAYER_2P;
			infomation.modelType = gameMasterNS::MODEL_FEMALE;
			player[i].initialize(infomation);
			break;
		}

		// �J�����|�C���^�̃Z�b�g
		player[i].setCamera(&camera[i]);

		//���f���̐ݒ�
		switch (player[i].getInfomation()->modelType)
		{
		case gameMasterNS::MODEL_MALE:
			maleRenderer->registerObject(&player[i]);
			break;
		case gameMasterNS::MODEL_FEMALE:
			femaleRenderer->registerObject(&player[i]);
			break;
		}

		//�G�t�F�N�V�A�[�̐ݒ�
		effekseerNS::setProjectionMatrix(i + 1,
			camera[i].fieldOfView,
			(float)camera[i].windowWidth,
			(float)camera[i].windowHeight,
			camera[i].nearZ,
			camera[i].farZ);
	}

	// �G�t�F�N�V�A�[�̐ݒ�
	effekseerNS::setProjectionMatrix(0,
		camera[0].fieldOfView,
		(float)camera[0].windowWidth,
		(float)camera[0].windowHeight,
		camera[0].nearZ,
		camera[0].farZ);

	// light
	light = new Light;
	light->initialize();

	// �Q�[���J�n������
	gameMaster->startGame();
	gameMaster->setTreeNum(treeManager->getTreeNum());
}


//=============================================================================
// �I��
//=============================================================================
void DebugScene::uninitialize()
{
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE_ARRAY(player);
	SAFE_DELETE(maleRenderer);
	SAFE_DELETE(femaleRenderer);
	SAFE_DELETE(treeManager);

}


//=============================================================================
// �X�V
//=============================================================================
void DebugScene::update(float _frameTime)
{

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[

	//�v���C���[�̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		player[i].update(frameTime);
	}

	maleRenderer->update();					//�����_���[
	femaleRenderer->update();				//�����_���[

	// �c���[�̍X�V
	treeManager->update(frameTime);

	//�J�����̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		camera[i].update();
	}
}


//=============================================================================
// �`��
//=============================================================================
void DebugScene::render()
{
	direct3D9->changeViewportFullWindow();
	render3D(&camera[gameMasterNS::PLAYER_1P]);
}


//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
void DebugScene::render3D(Camera* currentCamera) {

	//�e�X�g�t�B�[���h�̕`��
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	//// �v���C���[�̕`��
	//maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	//// �c���[�̕`��
	//if (player[nowRenderingWindow].getState() == playerNS::STATE::VISION ||
	//	player[nowRenderingWindow].getState() == playerNS::STATE::SKY_VISION)
	//{
	//	treeManager->switchingVisionView(nowRenderingWindow);
	//}
	//else {
	//	treeManager->switchingNormalView(nowRenderingWindow);
	//}
	//treeManager->render(currentCamera);
}


//===================================================================================================================================
//�y���`�W���؋�ԕ����}�l�[�W���[�ēo�^�����z
//===================================================================================================================================
void DebugScene::tree8Reregister(Object* tmp)
{
	//��x���X�g����O���
	tmp->treeCell.remove();
	//�ēo�^
	linear8TreeManager->registerObject(&tmp->getMin(), &tmp->getMax(), &tmp->treeCell);
}


//=============================================================================
// �Փ�
//=============================================================================
void DebugScene::collisions()
{
	//8���؂ւ̍ēo�^
	//�v���C���[�̓o�^
	tree8Reregister(&player[gameMasterNS::PLAYER_1P]);//1P�̓o�^
	tree8Reregister(&player[gameMasterNS::PLAYER_2P]);//2P�̓o�^

	//�e�̓o�^
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		for (int num = 0; num < player[i].getShootingNum(); num++)
			tree8Reregister(player[i].getBullet(num));
	}

	//�؂̓o�^
	for (int i = 0; i < treeManager->getTreeList().size(); i++)
	{
		tree8Reregister(treeManager->getTreeList()[i]);
		if (treeManager->getTreeList()[i]->getTreeData()->type == treeNS::DIGITAL_TREE
			&&treeManager->getTreeList()[i]->isAroundGreening())
		{
			//�Ή��G���A�I�u�W�F�N�g�̓o�^
			tree8Reregister(treeManager->getTreeList()[i]->getGreeningArea());
		}
	}

	//�ՓˑΉ����X�g���擾
	collisionNum = linear8TreeManager->getAllCollisionList(&collisionList);
	collisionNum /= 2;//2�Ŋ���̂̓y�A�ɂȂ��Ă��邽��

	//�Փ˔���
	Object** root = collisionList->getRoot();
	Object* tmp1 = NULL;
	Object* tmp2 = NULL;
	for (int i = 0; i < collisionNum; i++)
	{
		tmp1 = root[i * 2];
		tmp2 = root[i * 2 + 1];
		CollisionManager::collision(tmp1, tmp2);//�Փˏ���
	}

	//�J�����ƃt�B�[���h
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		LPD3DXMESH mesh = testFieldRenderer->getStaticMesh()->mesh;
		D3DXMATRIX matrix = testField->matrixWorld;
		//�J�����̂߂荞�ݕ␳
		camera[i].insetCorrection(mesh, matrix);
	}

	//�v���C���[�ƃt�B�[���h
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		LPD3DXMESH mesh = testFieldRenderer->getStaticMesh()->mesh;
		D3DXMATRIX matrix = testField->matrixWorld;
		//�n�ʕ����␳����
		player[i].grounding(mesh, matrix);
		//�ǂ��菈��
		player[i].insetCorrection(objectNS::AXIS_X, player[i].size.x / 2, mesh, matrix);
		player[i].insetCorrection(objectNS::AXIS_RX, player[i].size.x / 2, mesh, matrix);
		player[i].insetCorrection(objectNS::AXIS_Z, player[i].size.z / 2, mesh, matrix);
		player[i].insetCorrection(objectNS::AXIS_RZ, player[i].size.z / 2, mesh, matrix);
		//�Ə����C�X�V/�p���X�V/�_�����C�X�V
		player[i].updateAiming(mesh, matrix);
		player[i].updatePostureByAiming();
		player[i].updateShooting(mesh, matrix);
	}
}


//=============================================================================
// AI
//=============================================================================
void DebugScene::AI()
{

}


#ifdef _DEBUG
void DebugScene::createGUI()
{
	treeManager->outputGUI();		//�c���[�}�l�[�W���[

}
#endif
