//===================================================================================================================================
//�yDirector.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/17
// [�X�V��]2019/10/24
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Director.h"
#include "Splash.h"
#include "Title.h"
#include "Tutorial.h"
#include "Credit.h"
#include "Game.h"
#include "Result.h"
#include "Create.h"
#include "SE.h"
#include "LinearTreeCell.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Director::Director() {
	ZeroMemory(this, sizeof(Director));
	fpsMode = FIXED_FPS;
	fixedFps = FRAME_RATE;
#ifdef _DEBUG
	onGUI = true;
	memory = new MemoryViewer;
#else
	hiddenCursor = true;
	lockCursor = true;
	ShowCursor(FALSE);
#endif // _DEBUG
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Director::~Director() {
	SAFE_DELETE(window);
	SAFE_DELETE(d3d);
#ifdef _DEBUG
	SAFE_DELETE(imgui);
	SAFE_DELETE(memory);
#else
	ShowCursor(TRUE);
#endif // _DEBUG
	SAFE_DELETE(input);
	SAFE_DELETE(scene);
	SAFE_DELETE(textureLoader);
	SAFE_DELETE(staticMeshLoader);
	SAFE_DELETE(shaderLoader);
	SAFE_DELETE(soundInterface);
	SAFE_DELETE(textManager);
	SAFE_DELETE(fader);
	SAFE_DELETE(effekseerManager);
	SAFE_DELETE(gameMaster);
	//SAFE_DELETE(animationLoader);
	//thread_a->join();
	//SAFE_DELETE(thread_a);

	// COM�̏I������
	CoUninitialize();

}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
HRESULT Director::initialize() {
	//window
	window = new Window;
	if (!window)return E_FAIL;
	MFAIL(window->initialize(instance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME), "�E�B���h�E�쐬���s");
	wnd = window->wnd;

	//direct3D9
	d3d = new Direct3D9;
	if (d3d == NULL)
	{
		MSG("Direct3D�̏��������s");
		return E_FAIL;
	}
	MFAIL(d3d->initialize(wnd), "Direct3D���������s");

#ifdef _DEBUG
	//thread_a = new std::thread(threadA);
	SetForegroundWindow(wnd);
	imgui = new ImguiManager(wnd);
#endif // _DEBUG

	//�G�t�F�N�V�A�[
	effekseerManager = new EffekseerManager();
	effekseerManager->initialize();

	//sound
	soundInterface = new SoundInterface();

	//input
	input = new Input();
	input->initialize(instance, window->wnd, true);
	window->setInput();

	//textureLoader
	textureLoader = new TextureLoader;
	textureLoader->load(getDevice());

	//���b�V���ǂݍ���
	// X�t�@�C�����烁�b�V�������[�h����	
	//StaticMesh
	staticMeshLoader = new StaticMeshLoader;
	staticMeshLoader->load(getDevice());

	//HierarchyMesh
	//setVisualDirectory();
	//SKINMESH
	//setVisualDirectory();

	//�V�F�[�_�[�Ǎ�
	//Shader
	shaderLoader = new ShaderLoader;
	shaderLoader->load(getDevice());

	//�e�L�X�g�f�[�^�Ǎ�
	textManager = new TextManager();
	textManager->initialize();

	//�Q�[���Ǘ��N���X
	gameMaster = new GameMaster();

	//�A�j���[�V�����Ǎ��N���X
	//animationLoader = new AnimationLoader();
	//animationLoader->initialize(d3d->device);

	//scene
	scene = new Splash();
	scene->setGameMaster(gameMaster);
	scene->initialize();

	//fader
	fader = new Fader();

#pragma region LinearTreeCellTest
#ifdef 0

	//�I�u�W�F�N�g�̏�����
	const int testNum = 10;
	Object object[testNum];
	ObjectTree<Object>* objectTreeArray[testNum];//Object����ObjectTree�|�C���^�z��
	//�I�u�W�F�N�g�̓o�^
	for (int i = 0; i < testNum; i++)
	{
		//�I�u�W�F�N�g�c���[�ɓo�^
		ObjectTree<Object>* objectTree = new ObjectTree<Object>;
		objectTree->object = &object[i]; //�o�^
		objectTreeArray[i] = objectTree;
	}
	int partitionLevel = 4;
	//���`4���؃}�l�[�W���[
	//��Ԕ͈͂�X=-60�`720; Y=-60�`1200; Z=-60�`720�ɐݒ�
	Linear4TreeManager<Object> l4Tree;
	if (!l4Tree.initialize(
		partitionLevel,
		-60, -60,
		720, 720))
	{
		MSG("���`�S���؋�Ԃ̏������Ɏ��s���܂����B");
	}
	//���[�v���ꎞ�ϐ�
	DWORD collisionNum;
	vector<Object*> collisionVector;	//�ՓˑΏۃ��X�g
	//�����[�v
	do {
		for (int i = 0; i < testNum; i++)
		{
			Object* tmp = objectTreeArray[i]->object;
			objectTreeArray[i]->remove();//��x���X�g����O���
			//�ēo�^
			float top		= tmp->position.y + tmp->radius;
			float bottom	= tmp->position.y - tmp->radius;
			float right		= tmp->position.x + tmp->radius;
			float left		= tmp->position.x - tmp->radius;
			l4Tree.registObject(left, top, right, bottom, objectTreeArray[i]);
		}
		//�ՓˑΉ����X�g���擾
		collisionNum = l4Tree.getAllCollisionList(collisionVector);

		//�Փ˔���
		DWORD c;
		collisionNum /= 2;//2�Ŋ���̂̓y�A�ɂȂ��Ă���̂�
		for (c = 0; c < collisionNum; c++)
		{
			//�Փ˔��菈��
			//collision(collisionVector[c*2],collisionVector[c*2+1]);
		}



	} while (true);

#endif // TRUE
#pragma endregion


#pragma region MemoryTest
	//����������e�X�g
	//scene->uninitialize();
	//SAFE_DELETE(scene);
	//int i = 1;
	//while (i >= 0)
	//{
	//	scene = new Title();
	//	scene->initialize(d3d,input,sound,textureLoader,staticMeshLoader,shaderLoader,textManager);
	//	scene->uninitialize();
	//	SAFE_DELETE(scene);
	//	i--;
	//}
	//
	//scene = new Splash();
	//scene->initialize(d3d,input,sound,textureLoader,staticMeshLoader,shaderLoader,textManager);
#pragma endregion

	// ������\�^�C�}�[�̏��������݂�
	if (QueryPerformanceFrequency(&timerFreq) == false)
	{
		MSG("������\�^�C�}�[�̎擾���s���܂����B");
	}
	// �J�n���Ԃ��擾
	QueryPerformanceCounter(&timeStart);	

	// COM�̏�����
	CoInitializeEx(NULL, NULL);

	return S_OK;
}


//===================================================================================================================================
//�y��{�����z
// [�p�r]�����̏����̒�`
//===================================================================================================================================
void Director::run(HINSTANCE _instance) {
	char directory[512];
	GetCurrentDirectory(sizeof(directory), directory);
	initializeDirectory(directory);

	instance = _instance;
	if (FAILED(initialize()))
	{
		return;
	}
	ShowWindow(wnd, SW_SHOW);
	UpdateWindow(wnd);


	// ���b�Z�[�W���[�v
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mainLoop();
		}
	}

}

//===================================================================================================================================
//�y���C�����[�v�֐��z
// [�p�r]��{�I�Ƀ��[�v������e
//===================================================================================================================================
void Director::mainLoop() {
	//���Z�b�g
	if (input->wasKeyPressed(VK_F5))
	{
		scene->changeScene(SceneList::SPLASH);
		changeNextScene();
	}

	//�V�[���ؑփt���O�̊m�F
	if (scene->checkChangeOrder())
		changeNextScene();

	setFrameTime();				//�t���[�����Ԃ̏���������
	update();					//���C���X�V����
	render();					//���C���`�揈��
	fixFPS();					//�Œ�FPS����
	displayFPS();				//window�l�[����FPS�\��

	//���͂��N���A:���ׂẴL�[�`�F�b�N���s��ꂽ�ケ����Ăяo��
	input->clear(inputNS::MOUSE | inputNS::KEYS_PRESSED);
}

//===================================================================================================================================
//�y�X�V�����z
// [�p�r]�A�v���S�̂̍X�V����
//===================================================================================================================================
void Director::update() {
	input->update(window->windowActivate);
#ifdef _DEBUG
	memory->update();
	imgui->beginFrame();
	imgui->beginImGui("DirectorGUI");
	createGUI();
	imgui->endImGui();
#else
	if (input->wasKeyPressed(VK_F1))
	{
		hiddenCursor = !hiddenCursor;
		if (hiddenCursor) {
			ShowCursor(FALSE);
		}
		else {
			ShowCursor(TRUE);
		}
	}
	if (input->wasKeyPressed(VK_F2))
		lockCursor = !lockCursor;
	if (lockCursor)
	{
		if (input->getMouseRawX() != 0 || input->getMouseRawY() != 0)
		{
			SetCursorPos((int)window->getCenter().x, (int)window->getCenter().y);
		}
	}
#endif // _DEBUG
	effekseerManager->update();
	scene->update(frameTime);
	scene->collisions();
	scene->AI();
	soundInterface->UpdateSound();

#ifdef _DEBUG
	if (*scene->getShowGUI())
	{
		imgui->beginImGui(scene->getSceneName()->c_str());
		scene->createGUI();
		imgui->endImGui();
	}
	if (*gameMaster->getShowGUI())
	{
		imgui->beginImGui("GameMasterGUI");
		gameMaster->createGUI();
		imgui->endImGui();
	}
#endif // _DEBUG
}

//===================================================================================================================================
//�yGUI�E�B���h�E�̍쐬�z
// [�p�r]GUI�E�B���h�E�̍쐬����
//===================================================================================================================================
void Director::createGUI()
{
#ifdef _DEBUG
	if (!onGUI)return;
	ImGui::SliderInt("fpsMode", &fpsMode, VARIABLE_FPS, FIXED_FPS);
	ImGui::SliderInt("fixedFPS", &fixedFps, MIN_FRAME_RATE, MAX_FRAME_RATE);
	ImGui::Checkbox("SceneGUI", scene->getShowGUI());	
	ImGui::Checkbox("gameMasterGUI", gameMaster->getShowGUI());	
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("CPU %.2f ��", memory->getCpuUsege());
	ImGui::Text("MEMORY %d kb", memory->getMemoryUsege());
	ImGui::Text("PHYS_MEMORY %d kb", memory->getPhysMemorys());
#endif // _DEBUG
}

//===================================================================================================================================
//�y�`�揈���z
// [�p�r]�A�v���S�̂̕`�揈��
//===================================================================================================================================
void Director::render() {

#ifndef _DEBUG
	//�`��X�L�b�v
	//sleepRenderTime += frameTime;
	//if (sleepRenderTime < 1.000f / ((float)fixedFps * 5 / 6))return;
	//sleepRenderTime = 0.0f;
#endif // !_DEBUG

#ifdef _DEBUG
	//Debug
	d3d->clear(imgui->getClearColor());
	if (SUCCEEDED(d3d->beginScene()))
	{
		scene->render();
		imgui->render();
		d3d->endScene();
	}
	d3d->present(); 
#else
	//Release
	d3d->clear();
	if (SUCCEEDED(d3d->beginScene()))
	{
		scene->render();
		effekseerManager->render();
		d3d->endScene();
	}
	d3d->present(); 
#endif // _DEBUG
}

//===================================================================================================================================
//�y�t���[�����Ԃ̃Z�b�g�z
// [�p�r]�t���[�����Ԃ̏����ݒ菈��
//===================================================================================================================================
void Director::setFrameTime()
{
	// �Ō�̃t���[������̌o�ߎ��Ԃ��v�Z�AframeTime�ɕۑ�
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
	timeStart = timeEnd;
}

//===================================================================================================================================
//�y�t���[�����[�g�Œ菈���z
// [�p�r]FPS��萔�ɂ��w�肵�������ɌŒ肷�鏈��
// fpsMode�ɉ����ČŒ�t���[�����[�g�������Ȃ��σt���[�����[�g�ɐ؂�ւ��
//===================================================================================================================================
void Director::fixFPS() {
	if (fpsMode != FIXED_FPS)return;
	static INT Frames = 0, FPS = 0;
	static LARGE_INTEGER Frq = { 0 }, PreviousTime = { 0 }, CurrentTime = { 0 };
	DOUBLE Time = 0;
	char sz[11] = { 0 };

	while (Time <= (double)1000/(double)fixedFps)//1000ms / 60frame=16.6666 
	{
		QueryPerformanceFrequency(&Frq);

		QueryPerformanceCounter(&CurrentTime);
		Time = (DOUBLE)(CurrentTime.QuadPart - PreviousTime.QuadPart);

		Time *= (DOUBLE)1000.0 / (DOUBLE)Frq.QuadPart;

		//�����Ɏ��t���[���܂ł̑ҋ@���ɂ��������������L�q
		if (lockCursor)
		{
			if (input->getMouseRawX() != 0 || input->getMouseRawY() != 0)
				SetCursorPos((int)window->getCenter().x, (int)window->getCenter().y);
		}
	}
	PreviousTime = CurrentTime;
}

//===================================================================================================================================
//�yFPS�\�������z
// [�p�r]FPS���E�B���h�E�l�[���ɕ\�����鏈��
//===================================================================================================================================
void Director::displayFPS() {
	//FPS�v�Z�\��
	static LARGE_INTEGER time;
	LARGE_INTEGER time2, frq;
	double elaptime = 0;
	static int frame = 0;
	QueryPerformanceFrequency(&frq);
	double microsec = 1000000 / (double)frq.QuadPart;

	frame++;

	QueryPerformanceCounter(&time2);
	elaptime = (time2.QuadPart - time.QuadPart)*microsec;

	if (elaptime > 1000000)
	{
		QueryPerformanceCounter(&time);

		char str[50];
		char name[200] = { 0 };
		GetClassNameA(wnd, name, sizeof(name));
		sprintf(str, " fps=%d", frame);
		strcat(name, str);
		SetWindowTextA(wnd, name);
		frame = 0;
	}
}

//===================================================================================================================================
//�y�V�[���J�ڏ����z
// [�p�r]�V�[���N���X���J�ڏ�ԂɂȂ����ꍇ�ɃV�[����J�ڂ����鏈��
//===================================================================================================================================
void Director::changeNextScene() {
	int nextScene = scene->checkNextScene();		//���̃V�[��ID���擾	
	effekseerNS::stop();							//�S�G�t�F�N�g��~
	scene->uninitialize();							//�V�[���I������
	SAFE_DELETE(scene);								// �V�[���̍폜
	switch (nextScene)								// �w�肳�ꂽ�V�[���֑J��
	{
	case SceneList::SPLASH:					scene = new Splash();	break;
	case SceneList::TITLE:					scene = new Title();	break;
	case SceneList::TUTORIAL:				scene = new Tutorial(); break;
	case SceneList::CREDIT:					scene = new Credit();	break;
	case SceneList::GAME:					scene = new Game();		break;
	case SceneList::RESULT:					scene = new Result();	break;
	case SceneList::CREATE:					scene = new Create();	break;
	case SceneList::NONE_SCENE:				break;
	}
	scene->setGameMaster(gameMaster);				//�Q�[���Ǘ������V�[���փZ�b�g
	scene->initialize();							//�V�[������������
	currentSceneName = scene->getSceneName();		//���݃V�[�����̎擾
}

//void threadA()
//{
//	while (roop)
//	{
//	}
//}