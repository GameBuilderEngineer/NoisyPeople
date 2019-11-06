//===================================================================================================================================
//�yGame.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/11/01
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Game.h"

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
	linear8TreeManager->initialize(4, D3DXVECTOR3(-1000,-500,-1000),D3DXVECTOR3(1000,1000,1000));	

	//�I�u�W�F�N�g�J�E���^�[�̃��Z�b�g
	objectNS::resetCounter();		

	sceneName = "Scene -Game-";

	nextScene = SceneList::RESULT;

	//�V�[���̍X�V
	SoundInterface::SwitchAudioBuffer(SceneList::GAME);

	//�Đ��p�����[�^
	PLAY_PARAMETERS playParameters[3];
	memset(playParameters, 0, sizeof(playParameters));
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_01, false ,NULL,false,NULL, true, filterParameters };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_02, false ,NULL,false,NULL,true, filterParameters };
	playParameters[2] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, GAME_BGM_LIST::GAME_BGM_01, true,1.0f,false,NULL,true, filterParameters };

	//�Đ�
	SoundInterface::SE->playSound(&playParameters[0]);
	SoundInterface::SE->playSound(&playParameters[1]);
	SoundInterface::BGM->playSound(&playParameters[2]);
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Game::~Game()
{
	SoundInterface::SE->uninitSoundStop();
	SoundInterface::BGM->uninitSoundStop();
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Game::initialize() {

	//�e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::YAMADA_TEST_ZONE));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//player
	player				= new Player[gameMasterNS::PLAYER_NUM];
	maleRenderer		= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_MALE));
	femaleRenderer		= new StaticMeshRenderer(staticMeshNS::reference(gameMasterNS::MODEL_FEMALE));

	//camera
	camera = new Camera[gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		//�J�����̐ݒ�
		camera[i].initialize(WINDOW_WIDTH/2, WINDOW_HEIGHT);
		camera[i].setTarget(player[i].getCameraGaze());
		camera[i].setTargetX(&player[i].getAxisX()->direction);
		camera[i].setTargetY(&player[i].getAxisY()->direction);
		camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		camera[i].setRelative(CAMERA_RELATIVE_QUATERNION);
		camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		camera[i].setRelativeGaze(CAMERA_RELATIVE_GAZE);
		camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		camera[i].setFieldOfView( (D3DX_PI/180) * 80 );
		
		//�v���C���[�̐ݒ�
		PlayerTable infomation;
		switch (i)
		{
		case gameMasterNS::PLAYER_1P:
			infomation.playerType	= gameMasterNS::PLAYER_1P;
			infomation.modelType	= gameMasterNS::MODEL_FEMALE;
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

		//�d�͂�ݒ�
		player[i].configurationGravityWithRay(
			testField->getPosition(), 
			testFieldRenderer->getStaticMesh()->mesh, 
			testField->getMatrixWorld());	
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


	//�͖؂̏�����
	deadTree = new DeadTree();
	//��A�̏�����
	treeA = new TreeTypeA();
	//��B�̏�����
	treeB = new TreeTypeB();
	//�΂̏�����
	stone = new Stone();
	//�X�J�C�h�[���̏�����
	sky = new Sky();

	//�A�j���V�����L�����̏�����
	InitMoveP(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.04f, 0.04f, 0.04f), true);


	// �T�E���h�̍Đ�
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//�e�L�X�g�̏�����
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect = new TestEffect();

	//�f�B�X�v���C�p�v���[���T���v��
	samplePlane = new TestPlane();

	// �G�l�~�[
	enemyManager = new EnemyManager;
	enemyManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// �c���[
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// �A�C�e��
	itemManager = new ItemManager;
	itemManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());

	// �e���b�v
	telop = new Telop;
	telop->initialize();

	// AI
	aiDirector = new AIDirector;
	aiDirector->initialize();
	naviMesh = new NavigationMesh(staticMeshNS::reference(staticMeshNS::SAMPLE_NAVMESH));
	naviMesh->initialize();

	//�^�C�}�[
	timer = new Timer;
	timer->initialize();

	//Sprite����
	spriteGauge = new SpriteGauge;
	spriteGauge->initialize();

#ifdef _DEBUG
	enemyManager->setDebugEnvironment(camera, &player[gameMasterNS::PLAYER_1P]);
#endif
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Game::uninitialize() {
	//�T�E���h
	enemyManager->uninitializeSound();

	//SAFE_DELETE(linear4TreeManager);
	SAFE_DELETE(linear8TreeManager);
	SAFE_DELETE_ARRAY(player);
	SAFE_DELETE_ARRAY(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(maleRenderer);
	SAFE_DELETE(femaleRenderer);
	SAFE_DELETE(deadTree);
	SAFE_DELETE(treeA);
	SAFE_DELETE(treeB);
	SAFE_DELETE(stone);
	SAFE_DELETE(sky);
	SAFE_DELETE(testEffect);
	SAFE_DELETE(samplePlane);
	SAFE_DELETE(enemyManager);
	SAFE_DELETE(treeManager);
	SAFE_DELETE(itemManager);
	SAFE_DELETE(telop);
	SAFE_DELETE(aiDirector);
	SAFE_DELETE(timer);
	SAFE_DELETE(spriteGauge);

	UninitMoveP();

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Game::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//�y���������z
	//�t���[�����Ԃ���10FPS���̎��̎��Ԃ�蒷���ꍇ�́A���������i�X�V���Ȃ��j
	//���t���[�����Ԃɏ������Ă��鏈��������ɋ@�\���Ȃ�����
	//if (frameTime > 0.10)return;


	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[

	//�v���C���[�̍X�V
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		player[i].update(frameTime);		//�I�u�W�F�N�g
	maleRenderer->update();				//�����_���[
	femaleRenderer->update();			//�����_���[

	// �G�l�~�[�̍X�V
	enemyManager->update(frameTime);
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
		enemyManager->footsteps(*player[i].getPosition(), i);		//����

	// �c���[�̍X�V
	treeManager->update(frameTime);

	// �A�C�e���̍X�V
	itemManager->update(frameTime);

	UpdateMoveP(0.01f);

	//�G�t�F�N�V�A�[�̃e�X�g
#pragma region EffekseerTest
	//�G�t�F�N�g�̍Đ�
	if (input->wasKeyPressed('1'))
	{
		effekseerNS::Instance* instance = new effekseerNS::Instance();
		instance->position = *player[gameMasterNS::PLAYER_1P].getPosition();
		effekseerNS::play(instance);
	}
	if (input->wasKeyPressed('2'))
	{
		class Fire :public effekseerNS::Instance
		{
		public:
			D3DXVECTOR3* syncPosition;
			Fire() { 
				effectNo = effekseerNS::TEST0;
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
		effekseerNS::play(instance);
	}
	//�G�t�F�N�g�̈ꎞ��~�F�Đ�
	if (input->wasKeyPressed('3'))
	{
		effekseerNS::pause(false);
	}
	//�G�t�F�N�g�̈ꎞ��~�F��~
	if (input->isKeyDown('4'))
	{
		effekseerNS::pause(true);
	}
	//�G�t�F�N�g�̒�~
	if (input->wasKeyPressed('G'))
	{
		//effekseerNS::stop((*getEffekseerManager()->instanceList->getValue(0))->handle);
	}
#pragma endregion


	// �e���b�v�̍X�V
	//float rate = frameTime / SCENE_TIME;
	telop->update(frameTime);
	
	//�͖؂̍X�V
	deadTree->update();
	//��A�̍X�V
	treeA->update();
	//��B�̍X�V
	treeB->update();
	//�΂̍X�V
	stone->update();
	//�X�J�C�h�[���̍X�V
	sky->update();

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect->update(frameTime);

	//�f�B�X�v���C�p�v���[���T���v��
	samplePlane->update(frameTime);

	//�d�͌����i�d�͉񕜊m�F�p�j
	player->pullpower(1);

	//�J�����̍X�V
	for(int i = 0;i<gameMasterNS::PLAYER_NUM;i++)
		camera[i].update();

	//�^�C�}�[�̍X�V
	timer->update();

	// Enter�܂��́Z�{�^���Ń��U���g��
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// �T�E���h�̍Đ�
		//sound->play(soundNS::TYPE::SE_DECISION, soundNS::METHOD::PLAY);
		// �V�[���J��
		changeScene(nextScene);
	}

#ifdef _DEBUG
	test();
#endif
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Game::render() {	
		
	//1P�J�����E�E�B���h�E�E�G�t�F�N�V�A�[�}�l�[�W���[
	camera[gameMasterNS::PLAYER_1P].renderReady();
	direct3D9->changeViewport1PWindow();
	render3D(camera[gameMasterNS::PLAYER_1P]);
	effekseerNS::setCameraMatrix(
		camera[gameMasterNS::PLAYER_1P].position, 
		camera[gameMasterNS::PLAYER_1P].gazePosition, 
		camera[gameMasterNS::PLAYER_1P].upVector);
	effekseerNS::render();

	//2P�J�����E�E�B���h�E�E�G�t�F�N�V�A�[�}�l�[�W���[
	camera[gameMasterNS::PLAYER_2P].renderReady();
	direct3D9->changeViewport2PWindow();
	render3D(camera[gameMasterNS::PLAYER_2P]);
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
void Game::render3D(Camera currentCamera) {

	//�e�X�g�t�B�[���h�̕`��
	//testField->setAlpha(0.1f); 
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	// �v���C���[�̕`��
	maleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	femaleRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// �v���C���[�̑��̃I�u�W�F�N�g�̕`��
	for(int i = 0;i<gameMasterNS::PLAYER_NUM;i++)
		player[i].otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�؂̕`��
	deadTree->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//��A�̕`��
	treeA->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//��B�̕`��
	treeB->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//�΂̕`��
	stone->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//�X�J�C�h�[���̕`��
	sky->render(currentCamera.view, currentCamera.projection, currentCamera.position);


	
	DrawMoveP();

	// �G�l�~�[�̕`��
	enemyManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// �c���[�̕`��
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// �A�C�e���̕`��
	itemManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	//testEffect->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�f�B�X�v���C�p�v���[���T���v��
	samplePlane->render(currentCamera.view, currentCamera.projection, currentCamera.position);

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

#ifdef _DEBUG
#if 1	// �i�r�Q�[�V�������b�V���̃f�o�b�O�`��
	naviMesh->debugRender(currentCamera.view, currentCamera.projection, currentCamera.position);
#endif
#endif //_DEBUG
}

//===================================================================================================================================
//�yUI/2D�`��z
//===================================================================================================================================
void Game::renderUI() {

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	//device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//Sprite����
	//spriteGauge->render();
	//telop->render();
	
	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	telop->render();	// �e���b�v�̕`��

	//�^�C�}�[�̕`��
	timer->render();

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
	//�ēo�^
	tree8Reregister(&player[gameMasterNS::PLAYER_1P]);
	tree8Reregister(&player[gameMasterNS::PLAYER_2P]);
	for (int i = 0; i < stone->num; i++)
	{
		tree8Reregister(&stone->object[i]);
	}

	//�ՓˑΉ����X�g���擾
	collisionNum = linear8TreeManager->getAllCollisionList(&collisionList);
	collisionNum /= 2;//2�Ŋ���̂̓y�A�ɂȂ��Ă��邽��

	// �v���C���[�ƃA�C�e��
	std::vector<Item*> itemList = itemManager->getItemList();
	for (size_t i = 0; i < itemList.size(); i++)
	{	
		for (int j = 0; j < gameMasterNS::PLAYER_NUM; j++)
		{
			if (itemList[i]->sphereCollider.collide(player[j].getBodyCollide()->getCenter(),
				player[j].getRadius(), *itemList[i]->getMatrixWorld(), *player[j].getMatrixWorld()))
			{
				player[j].addSpeed(D3DXVECTOR3(0, 10, 0));
				player[j].addpower(batteryNS::RECOVERY_POWER);	//�d�͉��Z
				FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
				PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, GAME_SE_LIST::GAME_SE_01, false ,NULL,false,NULL,true, filterParameters };
				SoundInterface::SE->playSound(&playParameters);	//SE�Đ�
				itemManager->destroyAllItem();					//�f���[�g(���͑S����)
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
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("node:%d", testEffect->getList().nodeNum);
	if (ImGui::CollapsingHeader("CollisionList"))
	{
		ImGui::Text("collisionNum:%d", collisionNum);
		for (int i = 0; i < collisionNum; i++)
		{
			tmp1 = root[i * 2];
			tmp2 = root[i * 2 +1];
			ImGui::Text("ID(%d):position(%.02f,%.02f,%.02f)<->ID(%d):position(%.02f,%.02f,%.02f)",
				tmp1->id, tmp1->position.x,tmp1->position.y,tmp1->position.z,
				tmp2->id, tmp2->position.x,tmp2->position.y,tmp2->position.z);
		}
	}

	player->outputGUI();			//�v���C���[
	enemyManager->outputGUI();		//�G�l�~�[
	itemManager->outputGUI();		// �A�C�e���}�l�[�W��
	testField->outputGUI();			//�e�X�g�t�B�[���h
	camera->outputGUI();			//�J����
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
	if (input->wasKeyPressed('P'))
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

	// �c���[�}�l�[�W���̃e�X�g
	if (input->wasKeyPressed('5'))	// �쐬
	{
		treeNS::TreeData treeData;
		treeData.geenState = treeNS::GREEN;
		treeData.model = treeNS::A_MODEL;
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
}
#endif // _DEBUG
