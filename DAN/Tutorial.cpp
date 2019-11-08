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
	// ���̃V�[��(�^�C�g��)
	nextScene = SceneList::TITLE;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Tutorial::~Tutorial()
{
	// �T�E���h�̒�~
	//sound->stop(soundNS::TYPE::BGM_TUTORIAL);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Tutorial::initialize()
{
	//�e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
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
		camera[i].setFieldOfView((D3DX_PI / 180) * 90);

		//�v���C���[�̐ݒ�
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

		//�d�͂�ݒ�
		player[i].configurationGravityWithRay(
			testField->getPosition(),
			testFieldRenderer->getStaticMesh()->mesh,
			testField->getMatrixWorld());

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
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.04f, 0.04f, 0.04f), true);

	// �G�l�~�[
	enemyManager = new EnemyManager;
	enemyManager->initialize(sceneName,testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(),player);
	//test(sai[�}�[�W���鎞�ɏ�����])
	enemyNS::ENEMYSET enemySet = { 0 };
	enemySet.defaultDirection = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	enemySet.defaultPosition = D3DXVECTOR3(-3.0f, 1.5f, 24.5f);
	enemySet.defaultState = stateMachineNS::ENEMY_STATE::CHASE;
	enemySet.type = enemyNS::ENEMY_TYPE::WOLF;
	enemySet.enemyID = 0;
	enemyData = enemyManager->createEnemyData(enemySet);
	enemyManager->createEnemy(enemyData);

	//�f�B�X�v���C�v���[��
	D3DXVECTOR3 enemyPlanePos = enemySet.defaultPosition;
	enemyPlanePos.y += 5;
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
	enemyManager->setDebugEnvironment(camera, &player[gameMasterNS::PLAYER_1P]);
	playerSelect = gameMasterNS::PLAYER_1P;
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

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[

	//�v���C���[�̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{		
		//�I�u�W�F�N�g
		player[i].update(frameTime);	

		//�f�B�X�v���C�v���[��
		plane[i]->update(frameTime, planeStep[i]);
	}

	maleRenderer->update();				//�����_���[
	femaleRenderer->update();			//�����_���[

	// �G�l�~�[�̍X�V
	enemyManager->update(frameTime);
	//for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	//	enemyManager->footsteps(*player[i].getPosition(), i);		//����

	UpdateMoveP(0.01f);

	//�X�J�C�h�[���̍X�V
	sky->update();

	//�d�͌����i�d�͉񕜊m�F�p�j
	player->pullpower(1);

	//�J�����̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		camera[i].update();

	//�^�C�}�[�̍X�V
	timer->update();


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
	else if (input->wasKeyPressed(VK_BACK)||
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

	//Delete�܂��͓��ꃁ�C���{�^���Ń^�C�g����
	if (input->wasKeyPressed(VK_DELETE) ||
		input->getController()[PLAYER1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[PLAYER2]->wasButton(virtualControllerNS::SPECIAL_MAIN))
	{
		changeScene(nextScene);
	}

	// �`���[�g���A�����Ō�܂ōs������^�C�g����
	if (tutorialTex.nextPage >= tutorialTex::TUTORIAL_2D_SCENE_MAX)
	{
		//changeScene(nextScene);
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

	ImGui::SliderInt("PLAYER", &playerSelect, gameMasterNS::PLAYER_1P, gameMasterNS::PLAYER_2P, "");

	ImGui::SliderInt("STEP", &step[playerSelect], tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_1, tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_5, "");
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
