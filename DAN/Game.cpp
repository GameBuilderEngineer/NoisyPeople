//===================================================================================================================================
//�yGame.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/11/19
//===================================================================================================================================

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ���Ή���Ɍ����Ă��������iby �����j
// �v���C���[�̏����ʒu�́A�v���C���[�Ԃŏd���Ȃ������_����4��������I�肳��܂��B
// �@��̕���(-43, 8, -236), �A���̖�(-265, 54, 26), �B�k�̕���(142, 13, 246), �C���̋�����(311, 13, -65)
// �ɒB�R�����𐳖ʂɁA�ey���W���10m��ɃX�|�[�����܂��B���X�|�[���̂��߂ɂ��̉�̏����ʒu���L�^���Ă���
// �K�v������܂��B
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Game.h"
#include "CollisionManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace gameNS;


//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Game::Game()
{
	//���`�S���؋�ԕ����Ǘ��N���X
	//linear4TreeManager = new Linear4TreeManager<Object>;
	//linear4TreeManager->initialize(5, -1000, 1000, 1000, -1000);	
	//���`�W���؋�ԕ����Ǘ��N���X
	linear8TreeManager = new Linear8TreeManager<Object>;
	linear8TreeManager->initialize(5, D3DXVECTOR3(-3000,-2000,-3000),D3DXVECTOR3(3000,3000,3000));	

	//�I�u�W�F�N�g�J�E���^�[�̃��Z�b�g
	objectNS::resetCounter();		

	sceneName = "Scene -Game-";

	nextScene = SceneList::RESULT;

	//�Đ��p�����[�^
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Game, true,1.1f,false,NULL };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,  SE_LIST::SE_CountDown, false,NULL,false,NULL };
	playParameters[2] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,  SE_LIST::SE_StartGame, false,NULL,false,NULL };
	playParameters[3] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,  SE_LIST::SE_TimeUp, false,NULL,false,NULL };
	playParameters[4] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Opening, false,1.0f,false,NULL };
	playParameters[5] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_HurryUp, false,1.0f,false,NULL };
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Game::~Game()
{
	// �T�E���h�̒�~
	SoundInterface::BGM->uninitSoundStop();
}

//#define SAMPLE_NAVI	// �r���h�X�C�b�`�@����define���ӂ͋߁X�ŏ����܂�����U�c���Ă����Ă��炦��Ə�����܂�����
//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Game::initialize() {

	//�e�X�g�t�B�[���h
	testField = new Object();
#ifdef SAMPLE_NAVI
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
#else
	//testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_NAVIMESH));
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
#endif
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//player
	player				= new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer		= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer		= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

	//OP�J�����^�[�Q�b�g�I�u�W�F�N�g
	target = new Object;
	target->initialize(&D3DXVECTOR3(247.0f, 38.0f, 181.0f));		//�^�[�Q�b�g�̏����ʒu�ݒ�

	//�I�[�v�j���O�J����
	stateCamera = CAMERA0;		//�����t�H�g�O���t
	cameraOP = new Camera;
	//�J�����̐ݒ�
	cameraOP->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	cameraOP->setRelative(D3DXVECTOR3(0.0f,0.0f,1.0f));
	cameraOP->setTarget(&target->position);
	cameraOP->setGazeDistance(20.0f);
	//cameraOP->setGaze(D3DXVECTOR3(0, 0, 0));
	cameraOP->setUpVector(D3DXVECTOR3(0, 1, 0));
	cameraOP->setFieldOfView((D3DX_PI) / 18 * 10);;
	cameraOP->setViewProjection();
	cameraOP->setLimitRotationTop(0.1f);
	cameraOP->setLimitRotationBottom(0.1f);
	cameraOP->updateOrtho();
	//�J��������ϐ�
	cameraAxisZ = D3DXVECTOR3(0, 0, 0);
	fixedAxisZ = D3DXVECTOR3(0, 0, 0);
	cameraAxisX = D3DXVECTOR3(0, 0, 0);
	cameraAxisY = D3DXVECTOR3(0, 0, 0);
	fixedAxisX = D3DXVECTOR3(0, 0, 0);

	//�I�[�v�j���O�J����
	cameraBoss = new Camera;
	//�J�����̐ݒ�
	cameraBoss->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	cameraBoss->setTarget(&testField->position);
	cameraBoss->setRelative(D3DXVECTOR3(1.5f,3.0f,-3.0f));
	cameraBoss->setGazeDistance(500.0f);
	cameraBoss->setGaze(D3DXVECTOR3(0, 0, 0));
	cameraBoss->setUpVector(D3DXVECTOR3(0, 1, 0));
	cameraBoss->setFieldOfView((D3DX_PI / 180) * 90);
	cameraBoss->setLimitRotationTop(0.1f);
	cameraBoss->setLimitRotationBottom(0.1f);
	cameraBoss->updateOrtho();



	//camera
	camera = new Camera[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//�J�����̐ݒ�
		camera[i].initialize(WINDOW_WIDTH / 2,  WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getCameraGaze());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(CAMERA_RELATIVE_GAZE);
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		camera[i].setFieldOfView( (D3DX_PI/180) * 90);
		camera[i].setLimitRotationTop(0.1f);
		camera[i].setLimitRotationBottom(0.3f);
		camera[i].updateOrtho();
		
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
			infomation.playerType	= gameMasterNS::PLAYER_2P;
			infomation.modelType	= gameMasterNS::MODEL_FEMALE;
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

		//�G�t�F�N�V�A�[�̐ݒ�
		effekseerNS::setProjectionMatrix(i+1,
			camera[i].fieldOfView,
			(float)camera[i].windowWidth,
			(float)camera[i].windowHeight,
			camera[i].nearZ,
			camera[i].farZ);
	}

	//�G�t�F�N�V�A�[�̐ݒ�
	effekseerNS::setProjectionMatrix(0,
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
	//�C�ʂ̏�����
	ocean = new Ocean();

	//�A�j���V�����L�����̏�����
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), true);
	InitMoveP1(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.003f, 0.003f, 0.003f), true);
	//InitEquipment(TRUE);

	// �T�E���h�̍Đ�
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//�e�L�X�g�̏�����
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	//testEffect = new TestEffect();

	//�f�B�X�v���C�p�v���[���T���v��
	//samplePlane = new TestPlane();
	//�J�����L��
	//ad = new Advertisement();

	// �i�r�Q�[�V����AI�i�i�r�Q�[�V����AI�̓G�l�~�[�֌W�N���X����ɏ���������j
#ifdef SAMPLE_NAVI
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
#else
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_NAVIMESH));
#endif
	naviMesh->initialize();

	// �G�l�~�[
	enemyManager = new EnemyManager;
	enemyManager->initialize(*getSceneName(),testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld(), gameMaster, player);
	
	// �c���[
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	treeManager->setGameMaster(gameMaster);

	// �A�C�e��
	itemManager = new ItemManager;
	itemManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// ��
	windManager = new WindManager;
	windManager->initialize(player);

	// �}�b�v�I�u�W�F�N�g
	mapObjectManager = new MapObjectManager;
	mapObjectManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());


	//�e���b�v�}�l�[�W���[
	telopManager = new TelopManager;
	telopManager->initialize();

	//�Œ肳�ꂽUI
	fixedUI = new FixedUI;
	fixedUI->initialize();

	//�v���C���[1�����UI
	player1UI = new Player1UI;
	player1UI->initialize(&player[gameMasterNS::PLAYER_1P]);

	//�v���C���[�Q�����UI
	player2UI = new Player2UI;
	player2UI->initialize(&player[gameMasterNS::PLAYER_2P]);

	//�J�E���gUI
	countUI = new CountUI();

	//���e�B�N��
	reticle = new Reticle();
	reticle->setAimingPosition1(player[gameMasterNS::PLAYER_1P].getAiming());
	reticle->setAimingPosition2(player[gameMasterNS::PLAYER_2P].getAiming());

	//Sprite����
	spriteGauge = new SpriteGauge;
	spriteGauge->initialize();

	//Network
	networkClient = new NETWORK_CLIENT;

	//OP�A�i�E���X
	announcement = new Announcement;

	//�}�[�J�[
	markerRenderer = new MarkerRenderer;
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		markerRenderer->playerPosition[i] = &player[i].center;
	}

	//�_���[�WUI
	damageUI = new DamageUI();


#pragma region Memory Test
	////�������e�X�g

	//treeManager->uninitialize();
	//SAFE_DELETE(treeManager);

	//int i = 100000;
	//while (i >= 0)
	//{
	//	/*testFieldRenderer2 = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	//	testFieldRenderer2->registerObject(testField);
	//	testFieldRenderer2->update();
	//	SAFE_DELETE(testFieldRenderer2);*/
	//	treeManager = new TreeManager();
	//	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	//	//treeManager->update(1.0/60.0);
	//	treeManager->uninitialize();

	//	SAFE_DELETE(treeManager);
	//	i--;
	//}
	//i++;
	//treeManager = new TreeManager();
	//treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
#pragma endregion


#ifdef _DEBUG
	// �f�o�b�O�G�l�~�[���[�h�ɂ��邽�߂̏���
	enemyManager->setDebugEnvironment(camera, &player[gameMasterNS::PLAYER_1P]);
#endif // _DEBUG

	// �c���[���c�[���������ɐݒu����
	treeManager->createUsingTool();

	// ���^AI�i���^AI�̓c���[�̐����m�肵����ɏ���������j5
	aiDirector = new AIDirector;
	aiDirector->initialize(gameMaster, testFieldRenderer->getStaticMesh()->mesh,
		player, enemyManager, treeManager, itemManager, telopManager);
	
	//�Q�[���J�n������
	gameMaster->startGame();
	gameMaster->setTreeNum(treeManager->getTreeNum());
	SoundInterface::BGM->playSound(&playParameters[4]);	//�I�[�v�j���OBGM�Đ�
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Game::uninitialize() {

	//SAFE_DELETE(linear4TreeManager);
	SAFE_DELETE(linear8TreeManager);
	SAFE_DELETE_ARRAY(player);
	SAFE_DELETE_ARRAY(camera);
	SAFE_DELETE(cameraOP);
	SAFE_DELETE(cameraBoss);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(maleRenderer);
	SAFE_DELETE(femaleRenderer);
	SAFE_DELETE(sky);
	SAFE_DELETE(ocean);
	//SAFE_DELETE(testEffect);
	//SAFE_DELETE(samplePlane);
	SAFE_DELETE(enemyManager);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(itemManager);
	SAFE_DELETE(windManager);
	SAFE_DELETE(mapObjectManager);
	/*SAFE_DELETE(telop);*/
	SAFE_DELETE(telopManager);
	SAFE_DELETE(aiDirector);
	SAFE_DELETE(spriteGauge);
	SAFE_DELETE(reticle);
	SAFE_DELETE(fixedUI);
	SAFE_DELETE(player1UI);
	SAFE_DELETE(player2UI);
	SAFE_DELETE(countUI);
	//SAFE_DELETE(ad);
	SAFE_DELETE(networkClient);
	SAFE_DELETE(announcement);
	//�^�[�Q�b�g�I�u�W�F�N�g
	SAFE_DELETE(target); 
	SAFE_DELETE(damageUI);
	//UninitMoveP();
	//UninitMoveP1();
	//UninitEquipment();
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Game::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//gameMaster����������|�[�Y�Ɋւ��Ȃ��X�V
	gameMaster->update(frameTime);

	//�|�[�Y���Ȃ烊�^�[��
	if (gameMaster->paused())return;

	//�y���������z
	//�t���[�����Ԃ�10FPS���̎��Ԃ�蒷���ꍇ�́A���������i�X�V���Ȃ��j
	//���t���[�����Ԃɏ������Ă��鏈��������ɋ@�\���Ȃ�����
	if (frameTime > 10.0f/60.0f)return;

	//�I�[�v�j���O���Ԃ̍X�V
	gameMaster->updateOpeningTime(frameTime);

	//�J�E���g�_�E���̍X�V
	gameMaster->updateStartCountDown(frameTime);	//�J�n�J�E���g
	gameMaster->updateFinishCountDown(frameTime);	//�I���J�E���g

	//�J�n�J�E���g�_�E��
	if (gameMaster->playActionStartCount(3))
	{
		countUI->startCount(3);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionStartCount(2))
	{
		countUI->startCount(2);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionStartCount(1))
	{
		countUI->startCount(1);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionStartCount(0))
	{
		countUI->startCount(0);									//�Q�[���J�n
		SoundInterface::SE->playSound(&playParameters[2]);		//�J�n�T�E���h
		SoundInterface::BGM->playSound(&playParameters[0]);		//BGM�Đ�
		enemyManager->setUpdate(true);							//�G�l�~�[�X�V�J�n
	}
	
	//�Q�[���^�C���̍X�V
	gameMaster->updateGameTime(frameTime);

	//�I���J�E���g�_�E��
	if (gameMaster->playActionFinishCount(10))
	{
		countUI->finishCount(10);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(9))
	{
		countUI->finishCount(9);
		SoundInterface::SE->playSound(&playParameters[1]);
	}

	if (gameMaster->playActionFinishCount(8))
	{
		countUI->finishCount(8);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(7))
	{
		countUI->finishCount(7);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(6))
	{
		countUI->finishCount(6);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(5))
	{
		countUI->finishCount(5);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(4))
	{
		countUI->finishCount(4);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(3))
	{
		countUI->finishCount(3);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(2))
	{
		countUI->finishCount(2);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(1))
	{
		countUI->finishCount(1);
		SoundInterface::SE->playSound(&playParameters[1]);
	}
	if (gameMaster->playActionFinishCount(0))
	{
		countUI->finishCount(0);	//�Q�[���I��
		SoundInterface::SE->playSound(&playParameters[3]);	// �^�C���A�b�v�T�E���h
	}

	//�G���f�B���O���Ԃ̍X�V
	gameMaster->updateEndingTime(frameTime);

	//�Q�[���J�n���{�C�X
	if (gameMaster->getGameTime() < gameMasterNS::GAME_TIME && gameMaster->wasStartVoicePlayed[gameMasterNS::PLAYER_1P] == false)
	{
		PLAY_PARAMETERS voiceStart = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, S3D_LIST::Voice_Male_Start, false, NULL, true, gameMasterNS::PLAYER_1P };
		SoundInterface::S3D->playSound(&voiceStart);
		gameMaster->wasStartVoicePlayed[gameMasterNS::PLAYER_1P] = true;
	}
	else if (gameMaster->getGameTime() < gameMasterNS::GAME_TIME -2.0f && gameMaster->wasStartVoicePlayed[gameMasterNS::PLAYER_2P] == false)
	{
		PLAY_PARAMETERS voiceStart = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, S3D_LIST::Voice_Female_Start, false, NULL, true, gameMasterNS::PLAYER_2P };
		SoundInterface::S3D->playSound(&voiceStart);
		gameMaster->wasStartVoicePlayed[gameMasterNS::PLAYER_2P] = true;
	}

	//OP�J�����̃^�[�Q�b�g�̍X�V
	target->update();

	//�����I�u�W�F�N�g�ƃJ�����̓�_�ԃx�N�g���i�J����Z���x�N�g���j
	cameraAxisZ = cameraOP->getAxisZ();
	fixedAxisZ = Base::slip(cameraAxisZ, cameraOP->upVector);	//�J�����̌X���ɑΉ�
	//�J����X���x�N�g��
	D3DXVec3Cross(&cameraAxisX, &cameraOP->upVector, &cameraAxisZ);
	//�J����Y���x�N�g���i������̍X�V�j
	D3DXVECTOR3 Y = D3DXVECTOR3(0, 0, 0);
	D3DXVec3Cross(&Y, &fixedAxisZ, &cameraAxisX);
	D3DXVec3Cross(&cameraAxisY, &cameraAxisZ, &cameraAxisX);
	D3DXVec3Cross(&fixedAxisX, &cameraAxisY, &cameraAxisZ);

	//switch (stateCamera)
	//{
	//case CAMERA0:
	//	if (sceneTimer > 0.0f)
	//	{
	//		startPos = target->position;	//���[�v�n�_
	//		moveTime = 2.0f;				//�I�_�܂ł̎���
	//		moveTimer = moveTime;			//�ړ��^�C�}�[
	//		degreeTime = 2.0f;
	//		degreeTimer = degreeTime;
	//		stateCamera++;
	//		
	//		cameraOP->rotation(D3DXVECTOR3(0, 1, 0), 90.0f);
	//		cameraOP->rotation(fixedAxisX, -55.0f);
	//		tmpCameraQ = cameraOP->relativeQuaternion;
	//		//�J�����̑��Έʒu���ꎞ�ۑ�
	//	}
	//	break;
	//case CAMERA1:
	//	
	//		moveTimer -= frameTime;
	//		degreeTimer -= frameTime;
	//		
	//		rate = moveTimer / moveTime;
	//		rateX = degreeTimer / degreeTime;

	//		D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(247.0f, 122.0f, 181.0f), 1.0f - rate);
	//		degreeX = UtilityFunction::lerp(0, 40, 1.0f - rateX);
	//		
	//		if (degreeTimer > 0)
	//		{
	//			//�O�̃J�����̑��Έʒu�ɕ␳����
	//			cameraOP->relativeQuaternion = tmpCameraQ;
	//			cameraOP->rotation(fixedAxisX, degreeX);
	//		}

	//		if (moveTimer <= 0)
	//		{
	//			target->position = D3DXVECTOR3(105.0f, 73.0f, -250.0);
	//			startPos = target->position;
	//			moveTime = 2.0f;
	//			moveTimer = moveTime;
	//			degreeTime = 2.0f;
	//			degreeTimer = degreeTime;
	//			stateCamera = CAMERA9;
	//			//stateCamera++;
	//			//�J�����̑��Έʒu���ꎞ�ۑ�
	//			cameraOP->rotation(D3DXVECTOR3(0, 1, 0), 120.0f);
	//			cameraOP->rotation(fixedAxisX, -40.0f);
	//			tmpCameraQ = cameraOP->relativeQuaternion;
	//		}
	//		
	//	
	//	break;
	//case CAMERA2:
	//
	//		//moveTimer -= frameTime;
	//		//degreeTimer -= frameTime;
	//		//rate = moveTimer / moveTime;
	//		//rateY = degreeTimer / degreeTime;

	//		////D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-64.0f, 73.0f, -232.0f), 1.0f - rate);
	//		////degreeY = UtilityFunction::lerp(0, 20.0f, 1.0f - rateY);
	//		////target->position = Title::BezierCurve(startPos, D3DXVECTOR3(-250.0f, 63.0f, -200.0), D3DXVECTOR3(-185.0f, 40.0f, 80.0f), rate);
	//		//if (degreeTimer > 0)
	//		//{
	//		//	//�O�̃J�����̑��Έʒu�ɕ␳����
	//		//	cameraOP->relativeQuaternion = tmpCameraQ;
	//		//	//cameraOP->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
	//		//}

	//		//if (moveTimer <= 0)
	//		//{
	//		//	/*target->position = D3DXVECTOR3(-305.0f, 75.0f, 45.0f);
	//		//	startPos = target->position;
	//		//	moveTime = 6.0f;
	//		//	moveTimer = moveTime;
	//		//	degreeTimer = 6.0f;
	//		//	degreeTime = degreeTimer;*/
	//		//	stateCamera = CAMERA9;
	//		//	//stateCamera++;
	//		//	//���X�e�[�g�p�Ɋp�x����
	//		//	//cameraOP->rotation(D3DXVECTOR3(0, 1, 0), -15.0f);
	//		//	//cameraOP->rotation(fixedAxisX, 35.0f);
	//		//	////�J�����̑��Έʒu���ꎞ�ۑ�
	//		//	tmpCameraQ = cameraOP->relativeQuaternion;
	//		//}

	//	
	//	
	//	break;


	//case CAMERA3:
	//	//moveTimer -= frameTime;
	//	//degreeTimer -= frameTime;
	//	//rate = moveTimer / moveTime;

	//	//target->position = Title::BezierCurve(startPos, D3DXVECTOR3(-430.0f, 96.0f, 200.0f), D3DXVECTOR3(-290.0f, 156.0f, 265.0f), rate);

	//	//rateY = degreeTimer / degreeTime;
	//	//rateX = degreeTimer / degreeTime;
	//	//degreeY = UtilityFunction::lerp(0, 140.0f, 1.0f - rateY);
	//	//degreeX = UtilityFunction::lerp(0, 20.0f, 1.0f - rateX);
	//	//if (degreeTimer > 0)
	//	//{
	//	//	//�O�̃J�����̑��Έʒu�ɕ␳����
	//	//	cameraOP->relativeQuaternion = tmpCameraQ;
	//	//	cameraOP->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
	//	//	cameraOP->rotation(fixedAxisX, degreeX);
	//	//}
	//	//if (moveTimer <= 0)
	//	//{
	//	//	target->position = D3DXVECTOR3(-65.0f, 75.0f, 225.0f);
	//	//	startPos = target->position;
	//	//	moveTime = 6.0f;
	//	//	moveTimer = moveTime;
	//	//	degreeTimer = 6.0f;
	//	//	degreeTime = degreeTimer;
	//	//	stateCamera++;
	//	//	//���X�e�[�g�p�Ɋp�x����
	//	//	cameraOP->rotation(fixedAxisX, -25.0f);
	//	//	//�J�����̑��Έʒu���ꎞ�ۑ�
	//	//	tmpCameraQ = cameraOP->relativeQuaternion;
	//	//}
	//	break;

	//case CAMERA4:
	//	//moveTimer -= frameTime;
	//	//degreeTimer -= frameTime;
	//	//rate = moveTimer / moveTime;

	//	//target->position = Title::BezierCurve(startPos, D3DXVECTOR3(150.0f, 75.0f, 350.0f), D3DXVECTOR3(290.0f, 75.0f, 150.0f), rate);

	//	//rateY = degreeTimer / degreeTime;
	//	//degreeY = UtilityFunction::lerp(0, 120.0f, 1.0f - rateY);
	//	//if (degreeTimer > 0)
	//	//{
	//	//	cameraOP->relativeQuaternion = tmpCameraQ;
	//	//	cameraOP->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
	//	//}

	//	//if (moveTimer <= 0)
	//	//{
	//	//	target->position = D3DXVECTOR3(185.0f, 67.0f, 11.0f);
	//	//	startPos = target->position;
	//	//	moveTime = 6.0f;
	//	//	moveTimer = moveTime;
	//	//	degreeTimer = 6.0f;
	//	//	degreeTime = degreeTimer;
	//	//	stateCamera++;
	//	//	//���X�e�[�g�p�Ɋp�x����
	//	//	cameraOP->rotation(D3DXVECTOR3(0, 1, 0), -130.0f);
	//	//	cameraOP->rotation(fixedAxisX, -25.0f);
	//	//	//�J�����̑��Έʒu���ꎞ�ۑ�
	//	//	tmpCameraQ = cameraOP->relativeQuaternion;
	//	//}

	//	break;


	//case CAMERA5:
	//	//moveTimer -= frameTime;
	//	//degreeTimer -= frameTime;
	//	//rate = moveTimer / moveTime;

	//	//target->position = Title::BezierCurve(startPos, D3DXVECTOR3(221.0f, 230.0f, -49.0f), D3DXVECTOR3(257.0f, 183.0f, -110.0f), rate);

	//	//rateY = degreeTimer / degreeTime;
	//	//rateX = degreeTimer / degreeTime;
	//	//degreeY = UtilityFunction::lerp(0, 140.0f, 1.0f - rateY);

	//	//if (degreeTimer > 0)
	//	//{
	//	//	cameraOP->relativeQuaternion = tmpCameraQ;
	//	//	cameraOP->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
	//	//	//cameraOP->rotation(fixedAxisX, degreeX);
	//	//}

	//	//if (moveTimer <= 0)
	//	//{
	//	//	target->position = D3DXVECTOR3(-50.0f, 115.0f, -300.0f);
	//	//	startPos = target->position;
	//	//	moveTime = 5.0f;
	//	//	moveTimer = moveTime;
	//	//	degreeTimer = 4.0f;
	//	//	degreeTime = degreeTimer;
	//	//	stateCamera++;
	//	//	//���X�e�[�g�p�Ɋp�x����
	//	//	cameraOP->rotation(D3DXVECTOR3(0, 1, 0), 75.0f);
	//	//	//�J�����̑��Έʒu���ꎞ�ۑ�
	//	//	tmpCameraQ = cameraOP->relativeQuaternion;
	//	//}

	//	break;

	//case CAMERA6:
	//	//moveTimer -= frameTime;
	//	//degreeTimer -= frameTime;
	//	//rate = moveTimer / moveTime;

	//	//rateX = degreeTimer / degreeTime;
	//	//degreeX = UtilityFunction::lerp(0, -80.0f, 1.0f - rateX);

	//	//BezierCurveS1 = Title::BezierCurve(startPos, D3DXVECTOR3(-43.0f, -50.0f, -200.0f), D3DXVECTOR3(-43.0f, 250.0f, 350.0f), rate);
	//	//BezierCurveS2 = Title::BezierCurve(startPos, D3DXVECTOR3(-43.0f, 300.0f, 150.0f), D3DXVECTOR3(-43.0f, 250.0f, 350.0f), rate);
	//	//D3DXVec3Lerp(&target->position, &BezierCurveS1, &BezierCurveS2, 1.0f - rate);
	//	//if (degreeTimer > 0)
	//	//{
	//	//	cameraOP->relativeQuaternion = tmpCameraQ;
	//	//	cameraOP->rotation(fixedAxisX, degreeX);
	//	//}

	//	//if (moveTimer <= 0)
	//	//{
	//	//	target->position = D3DXVECTOR3(-23.0f, 250.0f, 350.0f);
	//	//	startPos = target->position;
	//	//	moveTime = 3.0f;
	//	//	moveTimer = moveTime;
	//	//	degreeTimer = 3.0f;
	//	//	degreeTime = degreeTimer;
	//	//	stateCamera++;
	//	//	//���X�e�[�g�p�Ɋp�x����

	//	//	//�J�����̑��Έʒu���ꎞ�ۑ�
	//	//	tmpCameraQ = cameraOP->relativeQuaternion;
	//	//}


	//	break;
	//case CAMERA7:
	//	//moveTimer -= frameTime;
	//	//degreeTimer -= frameTime;
	//	//rate = moveTimer / moveTime;
	//	//
	//	//targetDistance = D3DXVECTOR3(0, 100.0f, 0) - startPos;
	//	//distance = D3DXVec3Length(&targetDistance);
	//	////measurement = false;

	//	//rateY = degreeTimer / degreeTime;
	//	//rateX = degreeTimer / degreeTime;
	//	//degreeY = UtilityFunction::lerp(0, 180.0f, 1.0f - rateY);
	//	//degreeX = UtilityFunction::lerp(0, 75.0f, 1.0f - rateX);

	//	//D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(0, 100.0f, 0), 1.0f - rate);

	//	//moveDistance = UtilityFunction::lerp(0, distance, 1.0 - rate);

	//	//cameraOP->setGazeDistance(moveDistance);
	//	//if (degreeTimer > 0)
	//	//{
	//	//	//�O�̃J�����̑��Έʒu�ɕ␳����
	//	//	cameraOP->relativeQuaternion = tmpCameraQ;
	//	//	cameraOP->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
	//	//	cameraOP->rotation(fixedAxisX, degreeX);

	//	//}

	//	//if (moveTimer <= 0)
	//	//{
	//	//	target->position = D3DXVECTOR3(0, 100.0f, 0);
	//	//	startPos = target->position;
	//	//	moveTime = 6.0f;
	//	//	moveTimer = moveTime;
	//	//	degreeTimer = 30.0f;
	//	//	degreeTime = degreeTimer;
	//	//	stateCamera++;
	//	//	//�J�����̑��Έʒu���ꎞ�ۑ�
	//	//	tmpCameraQ = cameraOP->relativeQuaternion;
	//	//}

	//	break;
	//case CAMERA8:
	//	//moveTimer -= frameTime;
	//	//degreeTimer -= frameTime;
	//	//rate = moveTimer / moveTime;
	//	//rateY = degreeTimer / degreeTime;

	//	//degreeY = UtilityFunction::lerp(0, 900.0f, 1.0 - rateY);
	//	//if (degreeTimer > 0)
	//	//{
	//	//	cameraOP->relativeQuaternion = tmpCameraQ;
	//	//	cameraOP->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
	//	//}
	//	//else if (degreeTimer <= 0)
	//	//{
	//	//	stateCamera = CAMERA0;
	//	//	target->initialize(&D3DXVECTOR3(-34.0f, 180.0f, 100));		//�^�[�Q�b�g�̏����ʒu�ݒ�
	//	//	cameraOP->setRelative(D3DXQUATERNION(0.0f, 0.0f, 1.0f, 0.0f));   //���^�[�Q�b�g�̏����ʒu�ɑ������`�ɂȂ��Ă���
	//	//	cameraOP->setGazeDistance(20.0f);
	//	//	//stateCamera++;
	//	//}

	//	break;
	//case CAMERA9:

	//	//stateCamera = CAMERA0;
	//	//target->initialize(&D3DXVECTOR3(-34.0f, 160.0f, 20));		//�^�[�Q�b�g�̏����ʒu�ݒ�
	//	//cameraOP->setRelative(D3DXQUATERNION(0.0f, 0.0f, 20.0f, 0.0f));   //���^�[�Q�b�g�̏����ʒu�ɑ������`�ɂȂ��Ă���
	//	/*moveTimer -= frameTime;
	//	degreeTimer -= frameTime;
	//	rate = moveTimer / moveTime;

	//	D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(0, 0, 0), 1.0f - rate);
	//	if (degreeTimer <= 0)
	//	{
	//		stateCamera++;
	//	}*/
	//	break;
	//default:
	//	break;
	//}


	////Base::anyAxisRotationSlerp(&cameraQ,D3DXVECTOR3(13.2f, 6.0f, -13.0f),);

	////�J�����ړ�
	//if (input->isKeyDown('W'))
	//{
	//	fixedAxisZ *= 1.0f;
	//	target->position += fixedAxisZ;
	//}
	//if (input->isKeyDown('S'))
	//{
	//	fixedAxisZ *= -1.0f;
	//	target->position += fixedAxisZ;
	//}
	//if (input->isKeyDown('A'))
	//{
	//	cameraAxisX *= -1.0f;
	//	target->position += cameraAxisX;
	//}
	//if (input->isKeyDown('D'))
	//{
	//	cameraAxisX *= 1.0f;
	//	target->position += cameraAxisX;
	//}
	//if (input->isKeyDown('Q'))
	//{
	//	Y *= 1.0f;
	//	target->position += cameraOP->upVector;
	//}
	//if (input->isKeyDown('E'))
	//{
	//	Y *= 1.0f;
	//	target->position -= cameraOP->upVector;

	//}

	////�J������]
	////cameraOP->rotation(D3DXVECTOR3(0, -1, 0), degree);
	////Y��
	//if (input->isKeyDown(VK_RIGHT))
	//{
	//	
	//	cameraOP->rotation(cameraOP->upVector, inputDegree);
	//	//target->quaternion.y += 5.0f;
	//}
	//if (input->isKeyDown(VK_LEFT))
	//{
	//	cameraOP->rotation(-cameraOP->upVector, inputDegree);
	//	//target->quaternion.y -= 5.0f;
	//}
	////X��
	//if (input->isKeyDown(VK_UP))
	//{
	//	cameraOP->rotation(-fixedAxisX, inputDegree);
	//}
	//if (input->isKeyDown(VK_DOWN))
	//{
	//	cameraOP->rotation(fixedAxisX, inputDegree);
	//}
	////�Y�[��
	//if (input->isKeyDown('Z'))
	//{
	//	cameraOP->relativeQuaternion -= cameraOP->relativeQuaternion * 0.05f;
	//}
	//if (input->isKeyDown('X'))
	//{
	//	cameraOP->relativeQuaternion += cameraOP->relativeQuaternion * 0.05f;
	//}

	//if (input->wasKeyPressed('P'))
	//{
	//	getFader()->setShader(faderNS::NORMAL);
	//	getFader()->start();
	//}

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[

	//�v���C���[�̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].update(frameTime);		//�I�u�W�F�N�g
	maleRenderer->update();					//�����_���[
	femaleRenderer->update();				//�����_���[

	// �G�l�~�[�̍X�V
	enemyManager->update(frameTime);

#ifdef CHEAT_PRESENTATION
	// �v���[���p�`�[�g�@�\
	if (input->wasKeyPressed('6') || 
		input->getController()[gameMasterNS::PLAYER_1P]->wasButton(virtualControllerNS::SPECIAL_SUB)||
		input->getController()[gameMasterNS::PLAYER_2P]->wasButton(virtualControllerNS::SPECIAL_SUB))
	{
		aiDirector->eventMaker.makeEventBossEntry();
		markerRenderer->bossEnemyPosition = &treeManager->getTreeList()[1]->position;/*�����ɏP���c���[�̈ʒu���|�C���^����*/
		//markerRenderer->bossEnemyPosition = &enemyManager->getEnemyList()[enemyManager->getNextID()-1]->position;
		//markerRenderer->bossEnemyPosition = /*�����Ƀ{�X�̈ʒu���|�C���^����*/
		//�I���S����NULL����
	}
	if (input->wasKeyPressed('7'))
	{
		markerRenderer->attackedTree = &treeManager->getTreeList()[0]->position;/*�����ɏP���c���[�̈ʒu���|�C���^����*/
		//markerRenderer->attackedTree = /*�����Ƀ{�X�̈ʒu���|�C���^����*/
		//�I�P���I������NULL����
	}
#endif

	// �c���[�̍X�V
	treeManager->update(frameTime);

	// �A�C�e���̍X�V
	itemManager->update(frameTime);

	// ���̍X�V
	windManager->update(frameTime);

	// �}�b�v�I�u�W�F�N�g�̍X�V
	mapObjectManager->update(frameTime);

	UpdateMoveP(frameTime);
	//�L�����N�^�[�̏ꏊ�Ɖ�]�̘A�g
	MOVEP *mp = GetMovePAdr();
	mp->Pos = player[gameMasterNS::PLAYER_1P].position;
	D3DXQUATERNION q = player[gameMasterNS::PLAYER_1P].quaternion;
	Base::anyAxisRotation(&q,D3DXVECTOR3(0,1,0),180);
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
	//UpdateEquipment();
	//SWORD *Gun = GetSword("MoveP1");
	//D3DXQUATERNION q2 = player[gameMasterNS::PLAYER_1P].quaternion;
	//Base::anyAxisRotation(&q2, D3DXVECTOR3(0, 1, 0), 270);
	//Gun->Quaternion = q2;


	//�G�t�F�N�V�A�[�̃e�X�g
#pragma region EffekseerTest
	//�G�t�F�N�g�̍Đ�
	if (input->wasKeyPressed('1'))
	{
		effekseerNS::Instance* instance = new effekseerNS::Instance();
		instance->position = *player[gameMasterNS::PLAYER_1P].getPosition();
		effekseerNS::play(0,instance);
	}
	if (input->wasKeyPressed('2'))
	{
		class Fire :public effekseerNS::Instance
		{
		public:
			D3DXVECTOR3 * syncPosition;
			Fire() {
				effectNo = effekseerNS::BLOW;
				deltaRadian = D3DXVECTOR3(0, 0.3, 0);
			}
			virtual void update() {
				position = *syncPosition;
				Instance::update();
			};
		};
		Fire* instance = new Fire;
		instance->position = *player[gameMasterNS::PLAYER_1P].getPosition();
		instance->syncPosition = player[gameMasterNS::PLAYER_1P].getPosition();
		effekseerNS::play(0,instance);
	}
	//�G�t�F�N�g�̈ꎞ��~�F�Đ�
	if (input->wasKeyPressed('3'))
	{
		effekseerNS::pause(0,false);
	}
	//�G�t�F�N�g�̈ꎞ��~�F��~
	if (input->isKeyDown('4'))
	{
		effekseerNS::pause(0,true);
	}
	//�G�t�F�N�g�̒�~
	if (input->wasKeyPressed('G'))
	{
		//effekseerNS::stop((*getEffekseerManager()->instanceList->getValue(0))->handle);
	}
#pragma endregion


	//�e���b�v�}�l�[�W���[�̍X�V
	telopManager->update(frameTime);
	//�e���b�v�����t���O
	//�Ή���10%
	if (treeManager->getGreeningRate() >= 0.1 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_10 ))
	{
		telopManager->play(telopManagerNS::TELOP_TYPE0);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_10);
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_10);
	}
	//�Ή���30%
	if (treeManager->getGreeningRate() >= 0.3 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_30))
	{
		telopManager->play(telopManagerNS::TELOP_TYPE1);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_30);
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_30);
	}
	//�Ή���50%
	if (treeManager->getGreeningRate() >= 0.5 &&
		!gameMaster->whetherAchieved(gameMasterNS::ACHIEVEMENT_GREENING_RATE_50))
	{
		telopManager->play(telopManagerNS::TELOP_TYPE2);
		gameMaster->setProgress(gameMasterNS::ACHIEVEMENT_GREENING_RATE_50);
		SerialCommunicationNS::send(SerialCommunicationNS::GREENING_50);
	}
	

	//�X�J�C�h�[���̍X�V
	sky->update();
	//�C�ʂ̍X�V
	ocean->update();

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	//testEffect->update(frameTime);

	//�f�B�X�v���C�p�v���[���T���v��
	//samplePlane->update(frameTime);
	// �J�����L��
	//ad->update(frameTime);
	
	//�J�����̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		camera[i].update();
	}

	//�I�[�v�j���O�J�����̍X�V
	if (!gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_OPENING))
	{
		cameraOP->update();
	}
	//�{�X�J�����̍X�V
	//cameraBoss->update();


	//�Œ�UI�̍X�V
	fixedUI->update(gameMaster->getGameTime());

	//�v���C���[�����UI�̍X�V
	player1UI->update(treeManager->getGreeningRate()*100);
	player2UI->update(treeManager->getGreeningRate()*100);

	//�J�E���gUI�̍X�V
	countUI->update(frameTime);

	//���e�B�N���̍X�V
	reticle->update(frameTime);

	//OP�A�i�E���X
	announcement->update(frameTime);

	//�}�[�J�[�̍X�V
	markerRenderer->update(frameTime);

	//�_���[�WUI�̍X�V
	damageUI->update(frameTime);

	//�G���f�B���O���I��������V�[���J��
	if(gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_ENDING))
	{
		// �V�[���J��
		changeScene(nextScene);
	}

	//�c�莞�ԂP��
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_TELOP_ACTIVITY_LIMIT))
	{
		telopManager->play(telopManagerNS::TELOP_TYPE4);	// �������E�`
	}
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_TELOP_CANT_SENSE_GREEN))
	{
		telopManager->play(telopManagerNS::TELOP_TYPE5);	// �Ή�����������Ȃ��Ȃ��Ă��܂����`
	}
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_SE_HURRY_UP))
	{
		SoundInterface::SE->playSound(&playParameters[5]);	// �W���O��
		SoundInterface::BGM->stopSound(playParameters[0]);
	}
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_BGM_TEMP_REPLAY))
	{
		SoundInterface::BGM->playSound(&playParameters[0]);
	}
	if (gameMaster->playActionRamaining1Min(gameMasterNS::PASSING_SE_BGM_SPEED_UP))
	{
		SoundInterface::BGM->SetSpeed();					// BGM����
	}

	//�f�B�X�v���CPC�֑��M
	networkClient->send(gameMaster->getGameTime());

	//�t�F�[�_�[�e�X�g
	if (input->wasKeyPressed('P'))
	{
		getFader()->setShader(faderNS::NORMAL);
		getFader()->start();
	}

#ifdef _DEBUG
	if (input->wasKeyPressed('1'))
	{
		changeScene(SceneList::RESULT);
	}
	test();
#endif
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Game::render() 
{
	if (!gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_OPENING))
	{
		//OP��
		nowRenderingWindow = gameMasterNS::PLAYER_1P;
		direct3D9->changeViewportFullWindow();
		cameraOP->renderReady();
		render3D(cameraOP);
		renderUI();
		return;
	}
	//�{�X�J����
	//cameraBoss->renderReady();

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
void Game::render3D(Camera* currentCamera) {

	//�X�J�C�h�[���̕`��
	sky->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//�e�X�g�t�B�[���h�̕`��
	if (player[nowRenderingWindow].getState() == playerNS::STATE::VISION ||
		player[nowRenderingWindow].getState() == playerNS::STATE::SKY_VISION)
	{
		testFieldRenderer->setStaticMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_FACE_BLACK));
	}
	else {
		testFieldRenderer->setStaticMesh(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_FACE));
	}
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);


	// �v���C���[�̕`��
	//maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);
	//femaleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera->view, currentCamera->projection, currentCamera->position);

	// �v���C���[�̑��̃I�u�W�F�N�g�̕`��
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].otherRender(currentCamera->view, currentCamera->projection, currentCamera->position);

	//�A�j���[�V�������f���̕`��
	DrawMoveP();
	DrawMoveP1();
	//DrawEquipment();

	//�C�ʂ̕`��
	//ocean->render(currentCamera->view, currentCamera->projection, currentCamera->position);

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

	// �A�C�e���̕`��
	itemManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// ���̕`��
	windManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// �}�b�v�I�u�W�F�N�g�̕`��
	mapObjectManager->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	//testEffect->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//�f�B�X�v���C�p�v���[���T���v��
	//samplePlane->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	// �J�����L��
	//ad->render(currentCamera->view, currentCamera->projection, currentCamera->position);

	//3DUI
	if (gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_OPENING))
	{
		//���e�B�N��3D�`��
		if(player[nowRenderingWindow].getState() == playerNS::STATE::NORMAL)
			reticle->render3D(nowRenderingWindow,currentCamera);

		//�}�[�J�[�̕`��(2D/3D)�����Ƃ��`��
		markerRenderer->render(nowRenderingWindow, currentCamera);

	}

#if _DEBUG
	//4���؋�ԕ����̃��C���`��
	//linear4TreeManager->render();
	//8���؋�ԕ����̃��C���`��
	//linear8TreeManager->render();
	//Ray ray;
	//ray.color = D3DXCOLOR(150, 150, 0, 255);
	//Object** root = collisionList->getRoot();
	//Object* tmp1 = NULL;
	//Object* tmp2 = NULL;
	//D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0);
	//float length = 0;
	//for (int i = 0; i < collisionNum; i++)
	//{
	//	tmp1 = root[i * 2];
	//	tmp2 = root[i * 2 + 1];
	//	length = Base::between2VectorDirection(&direction, tmp1->position, tmp2->position);
	//	ray.initialize(tmp1->position, direction);
	//	ray.render(length);
	//}
#endif

#ifdef _DEBUG
#if 0	// �i�r�Q�[�V�������b�V���̃f�o�b�O�`��
	naviMesh->debugRender(currentCamera->view, currentCamera->projection, currentCamera->position);
#endif
#endif //_DEBUG
}

//===================================================================================================================================
//�yUI/2D�`��z
//===================================================================================================================================
void Game::renderUI() 
{
	//OP��UI
	if (!gameMaster->whetherAchieved(gameMasterNS::PASSING_GAME_OPENING))
	{
		announcement->render();
		return;
	}


	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	//device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//Sprite����
	//spriteGauge->render();
	//telop->render();
	
	// �e���b�v�}�l�[�W���[�̕`��
	telopManager->render();	

	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//�Œ�UI�̕`��
	fixedUI->render();

	//�v���C���[1�����UI�̕`��
	player1UI->render(gameMaster->getGameTime());

	//�v���C���[2�����UI�̕`��
	player2UI->render(gameMaster->getGameTime());

	//�J�E���gUI�̕`��
	countUI->render();

	//���e�B�N���̕`��
	reticle->render2D(&player[gameMasterNS::PLAYER_1P]);
	reticle->render2D(&player[gameMasterNS::PLAYER_2P]);

	//�_���[�WUI�̕`��
	damageUI->render(gameMasterNS::PLAYER_1P);
	damageUI->render(gameMasterNS::PLAYER_2P);

}

//===================================================================================================================================
//�y���`�S���؋�ԕ����}�l�[�W���[�ēo�^�����z
//===================================================================================================================================
//void Game::tree4Reregister(Object* tmp)
//{
//	//��x���X�g����O���
//	tmp->treeCell.remove();
//	//�ēo�^
//	linear4TreeManager->registerObject(
//		tmp->getLeft(), tmp->getFront(), tmp->getRight(), tmp->getBack(), &tmp->treeCell);
//
//}

//===================================================================================================================================
//�y���`�W���؋�ԕ����}�l�[�W���[�ēo�^�����z
//===================================================================================================================================
void Game::tree8Reregister(Object* tmp)
{
	//��x���X�g����O���
	tmp->treeCell.remove();
	//�ēo�^
	linear8TreeManager->registerObject( &tmp->getMin(),&tmp->getMax(), &tmp->treeCell);
}

//===================================================================================================================================
//�y�Փ˔��菈���z
//===================================================================================================================================
void Game::collisions()
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
	// �}�b�v�I�u�W�F�N�g�̓o�^
	for (size_t i = 0; i < mapObjectManager->getMapObjectList().size(); i++)
	{
		tree8Reregister(mapObjectManager->getMapObjectList()[i]);
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
	for (int i = 0; i < gameMasterNS::PLAYER_NUM;i++)
	{
		if (player[i].getDamaged())
		{
			damageUI->damaged(i);
		}
	}

	// ���Ƃ̓����蔻��
	windManager->windCollision(player);


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
		player[i].grounding(mesh,matrix);
		//�ǂ��菈��
		player[i].insetCorrection(objectNS::AXIS_X, player[i].size.x / 2,mesh,matrix);
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
			treeCylinder.centerLine.start	= tree->position;
			treeCylinder.centerLine.end		= tree->position + tree->getAxisY()->direction*tree->size.y;
			treeCylinder.height				= tree->size.y/2;
			treeCylinder.radius				= tree->size.x;
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

	

	// �v���C���[�ƃA�C�e��
	std::vector<Item*> itemList = itemManager->getItemList();
	for (size_t i = 0; i < itemList.size(); i++)
	{	
		for (int j = 0; j < gameMasterNS::PLAYER_NUM; j++)
		{
			if (itemList[i]->sphereCollider.collide(player[j].getBodyCollide()->getCenter(),
				player[j].getRadius(), *itemList[i]->getMatrixWorld(), *player[j].getMatrixWorld()))
			{
				player[j].addpower(batteryNS::RECOVERY_POWER);	//�d�͉��Z
				//FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
				PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Getlem, false ,NULL,false,NULL};
				SoundInterface::SE->playSound(&playParameters);	//SE�Đ�
				//itemManager->destroyAllItem();					//�f���[�g(���͑S����)
				itemManager->destroyItem(itemList[i]->getItemData()->itemID);					//�f���[�g(���͑S����)
			}
		}
	}
}

//===================================================================================================================================
//�yAI�����z
//===================================================================================================================================
void Game::AI() {
	aiDirector->run();		// ���^AI���s
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Game::createGUI()
{
	Object** root = collisionList->getRoot();
	Object* tmp1;
	Object* tmp2;
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("count = %d", count);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::Text("node:%d", testEffect->getList().nodeNum);
	ImGui::Text("collisionNum:%d", collisionNum);
	if (ImGui::CollapsingHeader("CollisionList"))
	{
		for (int i = 0; i < collisionNum; i++)
		{
			tmp1 = root[i * 2];
			tmp2 = root[i * 2 +1];
			ImGui::Text("ID(%d):position(%.02f,%.02f,%.02f)<->ID(%d):position(%.02f,%.02f,%.02f)",
				tmp1->id, tmp1->position.x,tmp1->position.y,tmp1->position.z,
				tmp2->id, tmp2->position.x,tmp2->position.y,tmp2->position.z);
		}
	}
	networkClient->outputGUI();
	player->outputGUI();			//�v���C���[
	enemyManager->outputGUI();		//�G�l�~�[
	itemManager->outputGUI();		//�A�C�e���}�l�[�W��
	treeManager->outputGUI();		//�c���[�}�l�[�W���[
	testField->outputGUI();			//�e�X�g�t�B�[���h
	camera->outputGUI();			//�J����
	cameraOP->outputGUI();			//�J����
	naviMesh->outputGUI();			//�i�r�Q�[�V����AI
}
#endif // _DEBUG


//===================================================================================================================================
//�y�Ȃ񂩂��낢��e�X�g����Ƃ���z
//===================================================================================================================================
#ifdef _DEBUG
void Game::test()
{
	// �A�C�e���}�l�[�W���̃e�X�g
	if (input->wasKeyPressed('0'))
	{
		itemNS::ItemData unko = { itemManager->issueNewItemID(), itemNS::BATTERY, *player->getPosition() };
		itemManager->createItem(unko);
	}
	// 3D���f���\���m�F�p�i�A�C�e���̍X�V�j
	if (input->wasKeyPressed('N'))
	{
		itemNS::ItemData abc = { 1, itemNS::EXAMPLE, *player->getPosition() };
		itemManager->createItem(abc);
	}

	if (input->wasKeyPressed('9'))
	{
		itemManager->destroyAllItem();
		//itemManager->destroyItem(3);
	}

	// �f�o�b�O�G�l�~�[��'7','8'�g�p��
	//// �}�b�v�I�u�W�F�N�g�}�l�[�W���[�̃e�X�g
	//if (input->wasKeyPressed('7'))	// �쐬
	//{
	//	mapObjectNS::MapObjectData mapObjectData;
	//	mapObjectData.zeroClear();
	//	mapObjectData.mapObjectID = mapObjectManager->issueNewMapObjectID();
	//	mapObjectData.type = mapObjectNS::STONE_01;
	//	mapObjectData.defaultPosition = *player->getPosition();
	//	mapObjectManager->createMapObject(mapObjectData);
	//}


	if (input->wasKeyPressed('6'))
	{
		//aiDirector->eventMaker.makeEventSpawningEnemyAroundPlayer(0);

		aiDirector->eventMaker.makeEventBossEntry();
	}

	//if (input->wasKeyPressed('6'))
	//{
	//	aiDirector->eventMaker.makeEventSpawningEnemyAroundPlayer(0);

	//	//aiDirector->eventMaker.makeEventEnemyAttaksTree();
	//}
	if (input->wasKeyPressed('Z'))
	{
		player->position = D3DXVECTOR3(20, 5, 0);
	}

	// �c���[�}�l�[�W���̃e�X�g
	if (input->wasKeyPressed('5'))	// �쐬
	{
		treeNS::TreeData treeData;
		treeData.treeID = treeManager->issueNewTreeID();
		treeData.hp = 100;
		treeData.type = treeNS::ANALOG_TREE;
		treeData.size = treeNS::STANDARD;
		treeData.greenState = treeNS::DEAD;
		treeData.model = treeNS::B_MODEL;
		treeData.initialPosition = *player->getPosition();
		treeManager->createTree(treeData);
	}
	if (input->wasKeyPressed('4'))	// ���[�t3�Ԃ̂ݕ`���؂�
	{
		treeManager->unRegisterLeafRendering(treeManager->getTreeList()[3]->getLeaf(),
			treeManager->getTreeList()[3]->getTreeData()->model);
	}	
	if (input->wasKeyPressed('3'))	// �S�j��
	{
		treeManager->destroyAllTree();
	}

	//�d�͊֌WUI�̃e�X�g
	//�d�͌����i�d�͉񕜊m�F�p�j
	if (input->isKeyDown('U'))
	{
		player[gameMasterNS::PLAYER_1P].pullpower(1);
	}
	//�d�͑����i�Q�[�W�A�^���N�e�X�g�p�j
	if (input->isKeyDown('I'))
	{
		player[gameMasterNS::PLAYER_1P].addpower(1);
	}
	//�d�͌����i�d�͉񕜊m�F�p�j
	if (input->isKeyDown('O'))
	{
		player[gameMasterNS::PLAYER_2P].pullpower(1);
	}
	//�d�͑����i�Q�[�W�A�^���N�e�X�g�p�j
	if (input->isKeyDown('P'))
	{
		player[gameMasterNS::PLAYER_2P].addpower(1);
	}
}
#endif // _DEBUG
