//===================================================================================================================================
//�yTutorial.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Tutorial.h"

//===================================================================================================================================
// �yusing�錾�z
//===================================================================================================================================
using namespace tutorialNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Tutorial::Tutorial()
{
	//���`�W���؋�ԕ����Ǘ��N���X
	linear8TreeManager = new Linear8TreeManager<Object>;
	linear8TreeManager->initialize(4, D3DXVECTOR3(-1000, -500, -1000), D3DXVECTOR3(1000, 1000, 1000));

	//�I�u�W�F�N�g�J�E���^�[�̃��Z�b�g
	objectNS::resetCounter();

	// ���݂̃V�[��(�`���[�g���A��)
	sceneName = "Scene -Tutorial-";
	// ���̃V�[��(�Q�[��)
	nextScene = SceneList::GAME;

	//�Đ��p�����[�^
	PLAY_PARAMETERS playParameters[2];
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Decision, false,NULL,false,NULL,true, filterParameters };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Tutorial, true,1.0f,false,NULL,true, filterParameters };

	//�Đ�
	SoundInterface::SE->playSound(&playParameters[0]);
	SoundInterface::BGM->playSound(&playParameters[1]);

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Tutorial::~Tutorial()
{
	// �T�E���h�̒�~
	SoundInterface::BGM->uninitSoundStop();
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Tutorial::initialize()
{
	//�e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::TUTORIAL_FILED));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//player
	player = new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer = new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer = new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

	//camera
	camera = new Camera[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//�J�����̐ݒ�
		camera[i].initialize(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getPosition());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(CAMERA_RELATIVE_GAZE);
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		camera[i].setFieldOfView((D3DX_PI / 180) * 91);
		camera[i].setLimitRotationTop(0.3f);
		camera[i].setLimitRotationBottom(0.7f);

		//�v���C���[�̐ݒ�
		PlayerTable infomation;
		switch (i)
		{
		case gameMasterNS::PLAYER_1P:
			infomation.playerType = gameMasterNS::PLAYER_1P;
			infomation.modelType = gameMasterNS::MODEL_FEMALE;
			player[i].initialize(infomation);
			player[i].setPosition(tutorialNS::PLAYER_P1_POSITION);
			break;
		case gameMasterNS::PLAYER_2P:
			infomation.playerType = gameMasterNS::PLAYER_2P;
			infomation.modelType = gameMasterNS::MODEL_FEMALE;
			player[i].initialize(infomation);
			player[i].setPosition(tutorialNS::PLAYER_P2_POSITION);
			break;
		}
		//�J�����|�C���^�̃Z�b�g
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

		//�i��
		step[i] = tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_1;
		planeStep[i] = tutorialPlaneNS::TUTORIAL_PLANE_ID::PLANE_ENEMY;
	}

	//�G�t�F�N�V�A�[�̐ݒ�
	effekseerNS::setProjectionMatrix(
		camera[0].fieldOfView,
		(float)camera[0].windowWidth,
		(float)camera[0].windowHeight,
		camera[0].nearZ,
		camera[0].farZ);

	//light
	light = new Light;
	light->initialize();

	//�X�J�C�h�[���̏�����
	sky = new Sky();

	//�A�j���V�����L�����̏�����
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), true);

	// �i�r���b�V���̏�����
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	naviMesh->initialize();

	// �G�l�~�[
	enemyManager = new EnemyManager;
	enemyManager->initialize(sceneName,testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(),gameMaster,player);
	enemyNS::ENEMYSET enemySet = { 0 };
	enemySet.defaultDirection = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	enemySet.defaultPosition = ENEMY_POSTITION;
	enemySet.defaultState = stateMachineNS::ENEMY_STATE::REST;
	enemySet.type = enemyNS::ENEMY_TYPE::TIGER;
	enemySet.enemyID = 0;
	enemyData = enemyManager->createEnemyData(enemySet);
	enemyData->isAlive = true;
	enemyData->hp = 100;
	enemyManager->createEnemy(enemyData);

	//�c���[
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	const int treeMax = 20;
	treeNS::TreeData treeData[treeMax] = { 0 };
	for (int i = 0; i < treeMax; i++)
	{
		treeData[i].treeID = treeManager->issueNewTreeID();
		treeData[i].initialDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		treeData[i].initialPosition = FIRST_TREE_POSTITION;
		treeData[i].initialPosition.z += (65 / treeMax) * i;
		treeData[i].greenState = treeNS::GREEN_STATE::DEAD;
		treeData[i].model = treeNS::TREE_MODEL::B_MODEL;
		treeData[i].type = treeNS::TREE_TYPE::ANALOG_TREE;
		treeData[i].size = treeNS::TREE_SIZE::STANDARD;
		treeData[i].hp = 100;
		treeManager->createTree(treeData[i]);
	}

	//�f�B�X�v���C�v���[��
	D3DXVECTOR3 enemyPlanePos = enemySet.defaultPosition;
	enemyPlanePos.y += 10;	//����
	plane = new TutorialPlane*[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		plane[i] = new TutorialPlane(enemyPlanePos);
	}

	//UI
	tutorialUI = new TutorialUI;

	// �T�E���h�̍Đ�
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//�e�L�X�g�̏�����
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//�^�C�}�[
	timer = new Timer;
	timer->initialize();

	// �`���[�g���A��2D������
	tutorialTex.initialize();

#ifdef _DEBUG
	//enemyManager->setDebugEnvironment(camera, &player[gameMasterNS::PLAYER_1P]);
	//playerSelect = gameMasterNS::PLAYER_1P;
#endif
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Tutorial::uninitialize()
{
	SAFE_DELETE(linear8TreeManager);
	SAFE_DELETE_ARRAY(player);
	SAFE_DELETE_ARRAY(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(maleRenderer);
	SAFE_DELETE(enemyManager);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(femaleRenderer);
	SAFE_DELETE(sky);
	SAFE_DELETE(timer);
	SAFE_DELETE(tutorialUI);

	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		SAFE_DELETE(plane[i]);
	}
	SAFE_DELETE_ARRAY(plane);

	UninitMoveP();

	tutorialTex.uninitialize();
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Tutorial::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//Enter�܂��́Z�{�^���Ŏ���
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[PLAYER1]->wasButton(virtualControllerNS::A) ||
		input->getController()[PLAYER2]->wasButton(virtualControllerNS::A))
	{
		// �T�E���h�̍Đ�
		//sound->play(soundNS::TYPE::SE_PAPER, soundNS::METHOD::PLAY);

		//�`���[�g���A��������
		tutorialTex.nextPage++;

		return;
	}
	//Back�܂��́~�{�^���őO��
	else if (input->wasKeyPressed(VK_BACK) ||
		input->getController()[PLAYER1]->wasButton(virtualControllerNS::B) ||
		input->getController()[PLAYER2]->wasButton(virtualControllerNS::B))

	{
		// �T�E���h�̍Đ�
		//sound->play(soundNS::TYPE::SE_CANCEL, soundNS::METHOD::PLAY);

		if (tutorialTex.nextPage > 0)
		{
			// �`���[�g���A����O��
			tutorialTex.nextPage--;
		}
		return;
	}

	if (tutorialTex.nextPage >= 2)
	{
		//�v���C���[�̍X�V
		for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		{
			//�I�u�W�F�N�g
			player[i].update(frameTime);

			//�f�B�X�v���C�v���[��
			plane[i]->update(frameTime, planeStep[i]);
		}
	}

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[


	maleRenderer->update();				//�����_���[
	femaleRenderer->update();			//�����_���[

	// �G�l�~�[�̍X�V
	enemyManager->update(frameTime);
	//for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	//	enemyManager->footsteps(*player[i].getPosition(), i);		//����

	//�c���[�̍X�V
	treeManager->update(frameTime);

	UpdateMoveP(0.01f);

	//�L�����N�^�[�̏ꏊ�Ɖ�]�̘A�g
	MOVEP *mp = GetMovePAdr();
	mp->Pos = player->position;
	D3DXQUATERNION q = player->quaternion;
	Base::anyAxisRotation(&q, D3DXVECTOR3(0, 1, 0), 180);
	mp->Quaternion = q;

	//�X�J�C�h�[���̍X�V
	sky->update();

	//�d�͌����i�d�͉񕜊m�F�p�j
	player->pullpower(1);

	//�J�����̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		camera[i].update();

	//�^�C�}�[�̍X�V
	//timer->update();

	//Delete�܂��͓��ꃁ�C���{�^���Ń^�C�g����
	if (input->wasKeyPressed(VK_DELETE) ||
		input->getController()[PLAYER1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[PLAYER2]->wasButton(virtualControllerNS::SPECIAL_MAIN))
	{
		changeScene(nextScene);
	}

	//�V�[���J��
	int flag = 0;
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		if (player[i].getPosition()->y <= -10)
		{
			flag++;
		}

		if (flag == gameMasterNS::PLAYER_NUM)
		{
			changeScene(nextScene);
		}
	}

	// �`���[�g���A��2D�X�V
	tutorialTex.update();
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Tutorial::render()
{
	//1P�J�����E�E�B���h�E�E�G�t�F�N�V�A�[�}�l�[�W���[
	camera[gameMasterNS::PLAYER_1P].renderReady();
	direct3D9->changeViewport1PWindow();
	render3D(camera[gameMasterNS::PLAYER_1P], gameMasterNS::PLAYER_1P);
	effekseerNS::setCameraMatrix(
		camera[gameMasterNS::PLAYER_1P].position,
		camera[gameMasterNS::PLAYER_1P].gazePosition,
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render();

	//2P�J�����E�E�B���h�E�E�G�t�F�N�V�A�[�}�l�[�W���[
	camera[gameMasterNS::PLAYER_2P].renderReady();
	direct3D9->changeViewport2PWindow();
	render3D(camera[gameMasterNS::PLAYER_2P], gameMasterNS::PLAYER_2P);
	effekseerNS::setCameraMatrix(
		camera[gameMasterNS::PLAYER_2P].position,
		camera[gameMasterNS::PLAYER_2P].gazePosition,
		camera[gameMasterNS::PLAYER_2P].upVector);
	effekseerNS::render();

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();
}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
void Tutorial::render3D(Camera currentCamera, int playerID)
{
	//�e�X�g�t�B�[���h�̕`��
//testField->setAlpha(0.1f); 
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	// �v���C���[�̕`��
	maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	femaleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// �v���C���[�̑��̃I�u�W�F�N�g�̕`��
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�X�J�C�h�[���̕`��
	sky->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	DrawMoveP();

	// �G�l�~�[�̕`��
	enemyManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�c���[�̕`��
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�f�B�X�v���C�p�v���[���T���v��
	plane[playerID]->render(currentCamera.view, currentCamera.projection, currentCamera.position);

#if _DEBUG
	//4���؋�ԕ����̃��C���`��
	//linear4TreeManager->render();
	//8���؋�ԕ����̃��C���`��
	linear8TreeManager->render();
	Ray ray;
	ray.color = D3DXCOLOR(255, 255, 0, 255);
	Object** root = collisionList->getRoot();
	Object* tmp1 = NULL;
	Object* tmp2 = NULL;
	D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0);
	float length = 0;
	for (int i = 0; i < collisionNum; i++)
	{
		tmp1 = root[i * 2];
		tmp2 = root[i * 2 + 1];

		length = Base::between2VectorDirection(&direction, tmp1->position, tmp2->position);
		ray.initialize(tmp1->position, direction);
		ray.render(length);
	}
#endif

}

//===================================================================================================================================
//�y2D[UI]�`��z
//===================================================================================================================================
void Tutorial::renderUI()
{
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//UI
	tutorialUI->render();

	//�^�C�}�[�̕`��
	timer->render();

	//�`���[�g���A��2D�`��
	tutorialTex.render();
}

//===================================================================================================================================
//�y���`�W���؋�ԕ����}�l�[�W���[�ēo�^�����z
//===================================================================================================================================
void Tutorial::tree8Reregister(Object* tmp)
{
	//��x���X�g����O���
	tmp->treeCell.remove();
	//�ēo�^
	linear8TreeManager->registerObject(&tmp->getMin(), &tmp->getMax(), &tmp->treeCell);
}

//===================================================================================================================================
//�y�Փˏ����z
//===================================================================================================================================
void Tutorial::collisions()
{
	//�ēo�^
	tree8Reregister(&player[gameMasterNS::PLAYER_1P]);
	tree8Reregister(&player[gameMasterNS::PLAYER_2P]);

	//�ՓˑΉ����X�g���擾
	collisionNum = linear8TreeManager->getAllCollisionList(&collisionList);
	collisionNum /= 2;//2�Ŋ���̂̓y�A�ɂȂ��Ă��邽��

		//�v���C���[�ƃt�B�[���h
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//�n�ʕ����␳����
		player[i].grounding(testFieldRenderer->getStaticMesh()->mesh, testField->matrixWorld);
		//�ǂ��菈��
		player[i].insetCorrection(objectNS::AXIS_X, player[i].size.x / 2, testFieldRenderer->getStaticMesh()->mesh, testField->matrixWorld);
		player[i].insetCorrection(objectNS::AXIS_RX, player[i].size.x / 2, testFieldRenderer->getStaticMesh()->mesh, testField->matrixWorld);
		player[i].insetCorrection(objectNS::AXIS_Z, player[i].size.z / 2, testFieldRenderer->getStaticMesh()->mesh, testField->matrixWorld);
		player[i].insetCorrection(objectNS::AXIS_RZ, player[i].size.z / 2, testFieldRenderer->getStaticMesh()->mesh, testField->matrixWorld);
	}


}

//===================================================================================================================================
//�yAI�����z
//===================================================================================================================================
void Tutorial::AI()
{
	//none
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Tutorial::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	player->outputGUI();

	ImGui::SliderInt("PLAYER", &playerSelect, gameMasterNS::PLAYER_1P, gameMasterNS::PLAYER_2P, "");

	ImGui::SliderInt("STEP", &step[playerSelect], tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_1, tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_END, "");
	if (tutorialUI->getStep(playerSelect) != step[playerSelect])
	{
		tutorialUI->setStep(playerSelect, step[playerSelect]);
	}

	ImGui::SliderInt("PLANE STEP", &planeStep[playerSelect], tutorialPlaneNS::TUTORIAL_PLANE_ID::PLANE_ENEMY, tutorialPlaneNS::TUTORIAL_PLANE_ID::PLANE_CLEAR, "");
	if (plane[playerSelect]->getPlaneStep() != planeStep[playerSelect])
	{
		plane[playerSelect]->update(frameTime, planeStep[playerSelect]);
	}
}
#endif // _DEBUG
