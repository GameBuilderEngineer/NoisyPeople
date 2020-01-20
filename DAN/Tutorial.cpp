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
#include "CollisionManager.h"

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
	linear8TreeManager->initialize(5, D3DXVECTOR3(-3000, -2000, -3000), D3DXVECTOR3(3000, 3000, 3000));

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

	// �`���[�g���A�����Ԃ̐ݒ�
	tutorialTimer = 60.0f;
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
	testField = new Object;
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::TUTORIAL_FILED));
	testField = new Object();
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));
	
	//player
	player = new Player[gameMasterNS::PLAYER_NUM];

	//camera
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

		//�v���C���[�̐ݒ�
		PlayerTable infomation;
		switch (i)
		{
		case gameMasterNS::PLAYER_1P:
			infomation.playerType = gameMasterNS::PLAYER_1P;
			infomation.modelType = gameMasterNS::MODEL_MALE;
			player[i].initialize(infomation);
			player[i].setPosition(tutorialNS::PLAYER_P1_POSITION);
			player[i].getBulletManager()->setCurrentScene(sceneName);

			break;
		case gameMasterNS::PLAYER_2P:
			infomation.playerType = gameMasterNS::PLAYER_2P;
			infomation.modelType = gameMasterNS::MODEL_FEMALE;
			player[i].initialize(infomation);
			player[i].setPosition(tutorialNS::PLAYER_P2_POSITION);
			player[i].getBulletManager()->setCurrentScene(sceneName);

			break;
		}
		//�J�����|�C���^�̃Z�b�g
		player[i].setCamera(&camera[i]);

		//�i��
		step[i] = tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_1;
		planeStep[i] = tutorialPlaneNS::TUTORIAL_PLANE_ID::PLANE_ENEMY;
		timeCnt[i] = 0;
		shift[i] = false;
		play[i] = false;
	}

	//�A�j���V�����L�����̏�����
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), true);
	InitMoveP1(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), true);

	//�G�t�F�N�V�A�[�̐ݒ�
	effekseerNS::setProjectionMatrix(0,
		camera[0].fieldOfView,
		(float)camera[0].windowWidth,
		(float)camera[0].windowHeight,
		camera[0].nearZ,
		camera[0].farZ);
	effekseerNS::setProjectionMatrix(1,
		camera[0].fieldOfView,
		(float)camera[0].windowWidth,
		(float)camera[0].windowHeight,
		camera[0].nearZ,
		camera[0].farZ);
	effekseerNS::setProjectionMatrix(2,
		camera[1].fieldOfView,
		(float)camera[1].windowWidth,
		(float)camera[1].windowHeight,
		camera[1].nearZ,
		camera[1].farZ);

	//light
	light = new Light;
	light->initialize();

	//�X�J�C�h�[���̏�����
	sky = new Sky();

	// �i�r�Q�[�V����AI�i�i�r�Q�[�V����AI�̓G�l�~�[�֌W�N���X����ɏ���������j
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::TUTORIAL_FILED));
	naviMesh->initialize();

	// �G�l�~�[
	enemyManager = new EnemyManager;
	enemyManager->initialize(sceneName,testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(),gameMaster,player, NULL);

	for (int i = 0; i < 2; i++)
	{
		enemyNS::EnemyData* enemyData;
		enemyNS::ENEMYSET enemySet;
		enemySet.enemyID = enemyManager->issueNewEnemyID();
		enemySet.type = enemyNS::ENEMY_TYPE::TIGER;
		enemySet.defaultPosition = ENEMY_POSTITION[i];
		enemySet.defaultDirection = D3DXVECTOR3(0.001f, 0.0f, -1.0f);
		enemySet.defaultState = stateMachineNS::ENEMY_STATE::REST;

		enemyData = enemyManager->createEnemyData(enemySet);
		enemyManager->createEnemy(enemyData);
		Enemy *tmpEnemy = enemyManager->findEnemy(enemySet.enemyID);
		tmpEnemy->setFieldMatrix(testField->getMatrixWorld());
		tmpEnemy->damage(50, i);
	}

	enemyManager->updateEnemyWithoutTime();	// ���O�X�V
	enemyManager->updatePartsRenderer();	// �����_���[���O�X�V
	enemyManager->setUpdate(true);			// �X�V�����L����

	//�c���[
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	treeManager->setGameMaster(gameMaster);

	const int treeMaxW = 5;
	const int treeMaxH = 2;

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < treeMaxH; i++)
		{
			for (int k = 0; k < treeMaxW; k++)
			{
				treeNS::TreeData treeData;
				treeData.zeroClear();
				treeData.treeID = treeManager->issueNewTreeID();
				treeData.initialDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				treeData.initialPosition = FIRST_TREE_POSTITION[j];
				treeData.initialPosition.x += 5 * i;
				treeData.initialPosition.z += (50 / treeMaxW) * k;
				treeData.greenState = treeNS::GREEN_STATE::DEAD;
				treeData.model = treeNS::TREE_MODEL::B_MODEL;
				treeData.type = treeNS::TREE_TYPE::ANALOG_TREE;
				treeData.size = treeNS::TREE_SIZE::STANDARD;
				treeData.hp = 0;
				treeManager->createTree(treeData);
			}
		}
	}

	for (int j = 0; j < 2; j++)
	{
		// Up Tree
		treeNS::TreeData treeData;
		treeData.zeroClear();
		treeData.treeID = treeManager->issueNewTreeID();
		treeData.initialDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		treeData.initialPosition = MID_TREE_POSTITION[j];
		treeData.greenState = treeNS::GREEN_STATE::GREEN;
		treeData.model = treeNS::TREE_MODEL::B_MODEL;
		treeData.type = treeNS::TREE_TYPE::DIGITAL_TREE;
		treeData.size = treeNS::TREE_SIZE::STANDARD;
		treeData.hp = 100;
		treeManager->createTree(treeData);
	}

	//�f�B�X�v���C�v���[��
	D3DXVECTOR3 enemyPlanePos = ENEMY_POSTITION[0];
	enemyPlanePos.y += 10;	//����
	plane = new TutorialPlane*[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		plane[i] = new TutorialPlane(enemyPlanePos);
	}



	//UI
	tutorialUI = new TutorialUI;

	////�Œ肳�ꂽUI
	//fixedUI = new FixedUI;
	//fixedUI->initialize();

	////�v���C���[1�����UI
	//player1UI = new Player1UI;
	//player1UI->initialize(&player[gameMasterNS::PLAYER_1P]);

	////�v���C���[�Q�����UI
	//player2UI = new Player2UI;
	//player2UI->initialize(&player[gameMasterNS::PLAYER_2P]);

	//���e�B�N��
	reticle = new Reticle();
	reticle->setAimingPosition1(player[gameMasterNS::PLAYER_1P].getAiming());
	reticle->setAimingPosition2(player[gameMasterNS::PLAYER_2P].getAiming());

	//�_���[�WUI
	damageUI = new DamageUI();

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
	SAFE_DELETE(testField);
	SAFE_DELETE(light);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(enemyManager);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(sky);
	SAFE_DELETE(tutorialUI);
	SAFE_DELETE(reticle);
	//SAFE_DELETE(fixedUI);
	//SAFE_DELETE(player1UI);
	//SAFE_DELETE(player2UI);
	SAFE_DELETE(damageUI);

	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		SAFE_DELETE(plane[i]);
	}
	SAFE_DELETE_ARRAY(plane);

	tutorialTex.uninitialize();
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Tutorial::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	// �`���[�g���A���̃^�C�}�[��i�߂�
	if (tutorialTimer != 0.0f)
	{
		tutorialTimer -= frameTime;
	}
	if (tutorialTimer < 0.0f)
	{
		tutorialTimer = 0.0f;
	}

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
	////Back�܂��́~�{�^���őO��
	//else if (input->wasKeyPressed(VK_BACK) ||
	//	input->getController()[PLAYER1]->wasButton(virtualControllerNS::B) ||
	//	input->getController()[PLAYER2]->wasButton(virtualControllerNS::B))

	//{
	//	// �T�E���h�̍Đ�
	//	//sound->play(soundNS::TYPE::SE_CANCEL, soundNS::METHOD::PLAY);

	//	if (tutorialTex.nextPage > 0)
	//	{
	//		// �`���[�g���A����O��
	//		tutorialTex.nextPage--;
	//	}
	//	return;
	//}

	if (tutorialTex.nextPage >= 2)
	{
		//�v���C���[�̍X�V
		for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		{
			player[i].update(frameTime);		//�I�u�W�F�N�g

		}
	}
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//�f�B�X�v���C�v���[��
		plane[i]->update(frameTime, planeStep[i]);
	}

	UpdateMoveP(frameTime);
	//�L�����N�^�[�̏ꏊ�Ɖ�]�̘A�g
	MOVEP *mp = GetMovePAdr();
	mp->Pos = player[gameMasterNS::PLAYER_1P].position;
	D3DXQUATERNION q = player[gameMasterNS::PLAYER_1P].quaternion;
	Base::anyAxisRotation(&q, D3DXVECTOR3(0, 1, 0), 180);
	if (!mp->IsDie)
	{
		mp->Quaternion = q;
	}

	UpdateMoveP1(frameTime);
	//�L�����N�^�[�̏ꏊ�Ɖ�]�̘A�g
	MOVEP1 *mp1 = GetMoveP1Adr();
	mp1->Pos = player[gameMasterNS::PLAYER_2P].position;
	D3DXQUATERNION q1 = player[gameMasterNS::PLAYER_2P].quaternion;
	Base::anyAxisRotation(&q1, D3DXVECTOR3(0, 1, 0), 180);
	if (!mp1->IsDie)
	{
		mp1->Quaternion = q1;
	}

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[

	//�G�l�~�[�̍X�V
	enemyManager->update(frameTime);

	//�c���[�̍X�V
	treeManager->update(frameTime);

	//�X�J�C�h�[���̍X�V
	sky->update();

	//�J�����̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		camera[i].update();
	}

	//�Œ�UI�̍X�V
	//fixedUI->update(tutorialTimer);

	////�v���C���[�����UI�̍X�V
	//player1UI->update(treeManager->getGreeningRate() * 100);
	//player2UI->update(treeManager->getGreeningRate() * 100);

	//���e�B�N���̍X�V
	reticle->update(frameTime);

	//�_���[�WUI�̍X�V
	damageUI->update(frameTime);

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
		//if (player[i].getPosition()->y <= -10)
		//{
		//	flag++;
		//}

		//if (flag == gameMasterNS::PLAYER_NUM)
		//{
		//	changeScene(nextScene);
		//}
	}
	if (play[0] && play[1])
	{
		changeScene(nextScene);
	}

	float distance = 0;
	D3DXVECTOR3 dir = D3DXVECTOR3(0, 0, 0);
	float len = 0;

	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		switch (step[i])
		{
		case tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_1:	//enemy
			if (enemyManager->findEnemy(i) == NULL)
			{
				step[i]++;
				planeStep[i]++;
				PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_TUTORIAL_STEP_CLEAR };
				SoundInterface::SE->playSound(&playParameters);
				D3DXVECTOR3 planePos = treeManager->findTree(7 + (10 * i))->position;
				planePos.y += 15;
				plane[i]->setPos(planePos);
			}
			else
			{
				D3DXVECTOR3 planePos = enemyManager->findEnemy(i)->position;
				planePos.y += 10;
				plane[i]->setPos(planePos);
			}
			break;
		case tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_2: //tree
			if (treeManager->findTree(7 + (10 * i))->getTreeData()->greenState == treeNS::GREEN)
			{
				step[i]++;
				planeStep[i]++;
				PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_TUTORIAL_STEP_CLEAR };
				SoundInterface::SE->playSound(&playParameters);
				for (int k = 0; k < treeManager->getTreeList().size(); k++)
				{
					Tree* tree = treeManager->getTreeList()[k];
					if (tree->getTreeData()->type == treeNS::DIGITAL_TREE
						&& tree->getTreeData()->treeID < i * 10 + 10)
					{
						D3DXVECTOR3 planePos = tree->position;
						planePos.y += 20;
						plane[i]->setPos(planePos);
					}
				}
			}
			break;
		case tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_3:
			if (input->wasKeyPressed(VK_LSHIFT) ||
				input->getController()[i]->wasButton(virtualControllerNS::Y))
			{
				step[i]++;
				planeStep[i]++;
				PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_TUTORIAL_STEP_CLEAR };
				SoundInterface::SE->playSound(&playParameters);
				plane[i]->setPos(plane[i]->getPos() + D3DXVECTOR3(0, 0, 0));
			}
			break;
		case tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_4:
			if (input->wasKeyPressed(VK_SPACE) ||
				input->getController()[i]->wasButton(virtualControllerNS::X))
			{
				step[i]++;
				planeStep[i]++;
				PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_TUTORIAL_STEP_CLEAR };
				SoundInterface::SE->playSound(&playParameters);
				plane[i]->setPos(plane[i]->getPos() + D3DXVECTOR3(0, 80, 0));
			}
			break;

		case tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_5:

			// ���C
			dir = MID_TREE_POSTITION[i] - plane[i]->getPos();
			ray[i].distance = D3DXVec3Length(&dir);
			D3DXVec3Normalize(&dir, &dir);
			ray[i].initialize(plane[i]->getPos(), dir);
			ray[i].color = D3DXCOLOR(255, 0, 0, 255);

			if (player[i].getState() == playerNS::DIGITAL_SHIFT)
			{
				//�f�W�^���V�t�g�����t���O�؊���
				shift[i] = true;
			}
			if (shift[i] == true)
			{
				if (player[i].getState() == playerNS::NORMAL && player[i].center.y > 30.0f)
				{
					step[i]++;
					planeStep[i]++;
					PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_TUTORIAL_CLEAR };
					SoundInterface::SE->playSound(&playParameters);
					plane[i]->setPos(PLANE_POS_5_5[i]);
					timeCnt[i] = clock() + 5000;
				}
			}
			break;

		case tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_END:
			if (clock() > timeCnt[i])
			{
				if (clear55flag == false)
				{
					planeStep[i]++;
				}
				else
				{
					planeStep[i]--;
				}
				plane[i]->setPos(PLANE_POS_FIN[i]);
				clear55flag = clear55flag ? false : true;
				timeCnt[i] = clock() + 5000;
			}

			if (!play[i])
			{
				distance = D3DXVec3Length(&(FIN_POS[i] - *player[i].getPosition()));
				if (distance < 5)
				{
					PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_TUTORIAL_END };
					SoundInterface::SE->playSound(&playParameters);
					play[i] = true;
				}
			}
			break;
		default:
			break;
		}

		if (tutorialUI->getStep(i) != step[i])
		{
			tutorialUI->setStep(i, step[i]);
		}
		if (plane[i]->getPlaneStep() != planeStep[i])
		{
			plane[i]->update(frameTime, planeStep[i]);
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
	nowRenderingWindow = gameMasterNS::PLAYER_1P;
	camera[gameMasterNS::PLAYER_1P].renderReady();
	direct3D9->changeViewport1PWindow();
	render3D(&camera[gameMasterNS::PLAYER_1P]);
	effekseerNS::setCameraMatrix(
		0,
		camera[gameMasterNS::PLAYER_1P].position,
		camera[gameMasterNS::PLAYER_1P].gazePosition,
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render(0);
	effekseerNS::setCameraMatrix(
		1,
		camera[gameMasterNS::PLAYER_1P].position,
		camera[gameMasterNS::PLAYER_1P].gazePosition,
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render(1);

	//2P�J�����E�E�B���h�E�E�G�t�F�N�V�A�[�}�l�[�W���[
	nowRenderingWindow = gameMasterNS::PLAYER_2P;
	camera[gameMasterNS::PLAYER_2P].renderReady();
	direct3D9->changeViewport2PWindow();
	render3D(&camera[gameMasterNS::PLAYER_2P]);
	effekseerNS::setCameraMatrix(
		0,
		camera[gameMasterNS::PLAYER_2P].position,
		camera[gameMasterNS::PLAYER_2P].gazePosition,
		camera[gameMasterNS::PLAYER_2P].upVector);
	effekseerNS::render(0);

	effekseerNS::setCameraMatrix(
		2,
		camera[gameMasterNS::PLAYER_2P].position,
		camera[gameMasterNS::PLAYER_2P].gazePosition,
		camera[gameMasterNS::PLAYER_2P].upVector);
	effekseerNS::render(2);

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();
}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
void Tutorial::render3D(Camera* currentCamera)
{
	//�e�X�g�t�B�[���h�̕`��
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	// �v���C���[�̕`��
	DrawMoveP();
	DrawMoveP1();

	// �v���C���[�̑��̃I�u�W�F�N�g�̕`��
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		player[i].otherRender(currentCamera->view, currentCamera->projection, currentCamera->position);
		if (step[i] == tutorialUINS::TUTORIAL_STEP::TUTORIAL_STEP_5)
		{
			ray[i].render(ray[i].distance);
		}
	}

	//�X�J�C�h�[���̕`��
	sky->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// �G�l�~�[�̕`��
	enemyManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// �c���[�̕`��
	if (player[nowRenderingWindow].getState() == playerNS::STATE::VISION ||
		player[nowRenderingWindow].getState() == playerNS::STATE::SKY_VISION)
	{
		treeManager->switchingVisionView(nowRenderingWindow);
	}
	else {
		treeManager->switchingNormalView(nowRenderingWindow);
	}
	treeManager->render(currentCamera);


	//���e�B�N��3D�`��
	if (player[nowRenderingWindow].getState() == playerNS::STATE::NORMAL)
		reticle->render3D(nowRenderingWindow, currentCamera);

	//�f�B�X�v���C�p�v���[���T���v��
	plane[nowRenderingWindow]->render(currentCamera->view, currentCamera->projection, currentCamera->position);
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

	//�Œ�UI�̕`��
	//fixedUI->getTimerFlame()->render();
	//fixedUI->getTimer()->render();

	//UI
	if (tutorialTex.nextPage >= 2)
	{
		tutorialUI->render();
	}

	////�v���C���[1�����UI�̕`��
	//player1UI->render(gameMaster->getGameTime());

	////�v���C���[2�����UI�̕`��
	//player2UI->render(gameMaster->getGameTime());

	//���e�B�N���̕`��
	reticle->render2D(&player[gameMasterNS::PLAYER_1P]);
	reticle->render2D(&player[gameMasterNS::PLAYER_2P]);

	//�_���[�WUI�̕`��
	damageUI->render(gameMasterNS::PLAYER_1P);
	damageUI->render(gameMasterNS::PLAYER_2P);

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
	//�v���C���[�̓o�^
	tree8Reregister(&player[gameMasterNS::PLAYER_1P]);
	tree8Reregister(&player[gameMasterNS::PLAYER_2P]);

	//�e�̓o�^
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		for (int num = 0; num < player[i].getShootingNum(); num++)
			tree8Reregister(player[i].getBullet(num));
	}
	//�G�̓o�^
	for (int i = 0; i < enemyManager->getEnemyList().size(); i++)
	{
		Enemy* enemy = enemyManager->getEnemyList()[i];
		// �G�l�~�[�{��
		tree8Reregister(enemy);

		// �G�l�~�\�̃o���b�g
		if (enemy->getEnemyData()->type == enemyNS::TIGER)
		{
			Tiger* tiger = (Tiger*)enemy;
			for (int k = 0; k < tiger->getBulletMangaer()->getBulletList()->nodeNum; k++)
			{
				tree8Reregister(*tiger->getBulletMangaer()->getBulletList()->getValue(k));
			}
		}
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

	//�v���C���[�̃_���[�WUI�̏���
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		if (player[i].getDamaged())
		{
			damageUI->damaged(i);
		}
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

	//�r�W����|�X�J�C�r�W������Ԃ̎�
	//�v���C���[[�V�t�g���C]�ƃf�W�^���c���[
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//�V�t�g����̖�����(�����l)
		player[i].disableOperation(playerNS::ENABLE_SHIFT);
		//�I�����C�g��OFF
		player[i].shownSelectLight(false);


		//�I�����ꂽ�f�W�^���c���[
		Tree* selectTree = NULL;
		//���X�g�̎擾
		std::vector<Tree*> list = treeManager->getTreeList();

		//�r�W�������X���[&�I�����C�g�G�t�F�N�g��OFF�ɂ���
		if (player[i].getState() != playerNS::VISION &&
			player[i].getState() != playerNS::SKY_VISION)
		{
			//�I������
			for (int num = 0; num < list.size(); num++)
			{
				Tree* tree = list[num];
				//�f�W�^���c���[�łȂ��ꍇcontinue
				if (tree->getTreeData()->type != treeNS::DIGITAL_TREE)continue;
				//�I������Ă��Ȃ���Ԃɂ���
				tree->switchingSelected(false, i);
			}
			continue;
		}

		//�Փ˔���
		for (int num = 0; num < list.size(); num++)
		{
			Tree* tree = list[num];
			//�f�W�^���c���[�̏ꍇ
			if (tree->getTreeData()->type != treeNS::DIGITAL_TREE)continue;
			//�I������Ă��Ȃ���Ԃɂ���
			tree->switchingSelected(false, i);

			//�J�����O����
			//�J��������p���̏ꍇ
			D3DXVECTOR3 center = tree->center;
			float radius = tree->radius;
			if (!UtilityFunction::culling(
				center, radius, camera[i].view, camera[i].fieldOfView,
				camera[i].nearZ, camera[i].farZ, camera[i].aspect))continue;

			//�V�t�g���C�̍X�V
			Cylinder treeCylinder;
			treeCylinder.centerLine.start = tree->position;
			treeCylinder.centerLine.end = tree->position + tree->getAxisY()->direction*tree->size.y;
			treeCylinder.height = tree->size.y / 2;
			treeCylinder.radius = tree->size.x;
			//�Փˎ�
			if (player[i].collideShiftRay(treeCylinder))
			{
				//�I�����Ƃ��ĕۑ�����B
				selectTree = tree;
			}
		}

		//�I�����ꂽ�f�W�^���c���[�ւ̏������s��
		if (selectTree)
		{
			selectTree->switchingSelected(true, i);
			player[i].shownSelectLight(true);
		}
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
