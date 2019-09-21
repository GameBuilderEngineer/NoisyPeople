//===================================================================================================================================
//�yGame.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
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
	sceneName = "Scene -Game-";

	nextScene = SceneList::RESULT;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Game::~Game()
{
	// �T�E���h�̒�~
	//sound->stop(soundNS::TYPE::BGM_GAME);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Game::initialize() {

	// �T�E���h�̍Đ�
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//player
	//player[0] = new Player;
	//player[1] = new Player;

	//camera
	//camera = new Camera[NUM_PLAYER];
	//for (int i = 0; i < NUM_PLAYER; i++)
	//{
	//	camera[i].initialize(WINDOW_WIDTH/2, WINDOW_HEIGHT);
	//	camera[i].setTarget(player[i]->getPosition());
	//	camera[i].setTargetX(&player[i]->getAxisX()->direction);
	//	camera[i].setTargetY(&player[i]->getAxisY()->direction);
	//	camera[i].setTargetZ(&player[i]->getAxisZ()->direction);
	//	camera[i].setRelative(CAMERA_RELATIVE_QUATERNION[i]);
	//	camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
	//	camera[i].setRelativeGaze(D3DXVECTOR3(0, 10, 0));
	//	camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
	//	camera[i].setFieldOfView(D3DX_PI / 2.5);
	//}

	//light
	//light = new Light;
	//light->initialize(direct3D9);

	//for (int i = 0; i < NUM_PLAYER; i++)
	//{//�v���C���[�̏�����
		//player[i]->initialize(i, gameMaster->getPlayerInfomation()[i].modelType, direct3D9->device, staticMeshLoader, textureLoader, shaderLoader);
		//player[i]->setInput(input);			//���̓N���X�̃Z�b�g
		//player[i]->setAnimationModel(animationLoader->getAnimationModel(i, gameMaster->getPlayerInfomation()[i].modelType));
		//player[i]->setCamera(&camera[i]);	//�J�����̃Z�b�g
		//player[i]->setSound(sound);			//�T�E���h�̃Z�b�g
		//player[i]->configurationGravity(field.getPosition(),field.getRadius());	//�d�͂��쐬
		//player[i]->animationPlayer->setAnimationConfiguration(animationPlayerNS::SCENE_TYPE::GAME);
		//player[i]->animationPlayer->resetAnimation();
	//}

	//�e�L�X�g�̏�����
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//�|�C���g�X�v���C�g
	//pointSprite.initilaize(direct3D9->device);

	//�C���X�^���X�v���[��
	//plane.createPositionSpherical(direct3D9->device, 3000, 250.0f);
	//plane.initialize(direct3D9->device, *shaderLoader->getEffect(shaderNS::INSTANCE_BILLBOARD), *textureLoader->getTexture(textureLoaderNS::BACKGROUND_DUST));

	//xFile�Ǎ�mesh�̃C���X�^���V���O�`��̃e�X�g
	//D3DXVECTOR3 positionList[] =
	//{
	//	D3DXVECTOR3(100,50,50),
	//	D3DXVECTOR3(-100,-100,-50),
	//	D3DXVECTOR3(100,-50,10),
	//	D3DXVECTOR3(100,50,10),
	//	D3DXVECTOR3(100,0,100),
	//	D3DXVECTOR3(0,100,0),
	//	D3DXVECTOR3(-100,100,100),
	//	D3DXVECTOR3(-100,-100,100),
	//	D3DXVECTOR3(-100,100,0),
	//	D3DXVECTOR3(-100,100,-100),
	//	D3DXVECTOR3(100,50,50),
	//	D3DXVECTOR3(-100,-100,-50),
	//	D3DXVECTOR3(100,-50,10),
	//	D3DXVECTOR3(100,50,10),
	//	D3DXVECTOR3(100,0,100),
	//	D3DXVECTOR3(0,100,0),
	//	D3DXVECTOR3(-100,100,100),
	//	D3DXVECTOR3(-100,-100,100),
	//	D3DXVECTOR3(-100,100,0),
	//	D3DXVECTOR3(-100,100,-100),
	//};
	//testObject.initialize(direct3D9->device, &staticMeshLoader->staticMesh[staticMeshNS::STAR_REGULAR_POLYHEDRON_X10], &D3DXVECTOR3(0, 0, 0));
	//testObject.setNumOfRender(direct3D9->device, 20, positionList);
	//testObject.activation();

	//D3DXVECTOR3 cubeList[NUM_CUBE];
	//for (int i = 0; i < NUM_CUBE; i++)
	//{
	//	cubeList[i] = D3DXVECTOR3((float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500), (float)(rand() % 1000 - 500));
	//}
	//testCube.initialize(direct3D9->device, &staticMeshLoader->staticMesh[staticMeshNS::CUBE], &D3DXVECTOR3(0, 0, 0));
	//testCube.setNumOfRender(direct3D9->device, NUM_CUBE, cubeList);
	//testCube.activation();

	//�Q�[���}�X�^�[
	//gameMaster->gameStart();//�Q�[���J�n������
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Game::uninitialize() {
	//SAFE_DELETE(light);
	//SAFE_DELETE_ARRAY(camera);
	//SAFE_DELETE(player[0])
	//SAFE_DELETE(player[1])
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Game::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//�Q�[�����J�n�����ꍇ�|�[�Y���L��
	//if (gameMaster->whetherAlreadyStart())
	//{
	//	if (uiPause.getRenderFlag())
	//	{
	//		if (input->wasKeyPressed('P') ||
	//			input->getController()[PLAYER1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
	//			input->getController()[PLAYER2]->wasButton(virtualControllerNS::SPECIAL_MAIN)
	//			)// �|�[�Y����
	//		{
	//			// �T�E���h�̍Đ�
	//			sound->play(soundNS::TYPE::SE_PAUSE, soundNS::METHOD::PLAY);
	//			uiPause.setRenderFlag(false);
	//		}
	//	}
	//	else if (!uiPause.getRenderFlag())
	//	{
	//		if (input->wasKeyPressed('P') ||
	//			input->getController()[PLAYER1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
	//			input->getController()[PLAYER2]->wasButton(virtualControllerNS::SPECIAL_MAIN)
	//			)// �|�[�Y����
	//		{
	//			// �T�E���h�̍Đ�
	//			sound->play(soundNS::TYPE::SE_PAUSE, soundNS::METHOD::PLAY);
	//			uiPause.setRenderFlag(true);
	//		}
	//	}
	//	if (uiPause.getRenderFlag())return;// �|�[�Y���Ă���X�V���Ȃ�
	//}

	//�y���������z
	//�t���[�����Ԃ���10FPS���̎��̎��Ԃ�蒷���ꍇ�́A���������i�X�V���Ȃ��j
	//���t���[�����Ԃɏ������Ă��鏈��������ɋ@�\���Ȃ�����
	if (frameTime > 0.10)return;

	//�y�Q�[���}�X�^�[�̍X�V�z
	//gameMaster->update(frameTime);
	//�Q�[���I�[�o�[������
	//if (gameMaster->whetherGameOver())
	//{//�V�[���ؑ�
	//	for (int i = 0; i < playerNS::NUM_PLAYER; i++)
	//	{
	//		gameMaster->setWage(i, player[i]->getWage());
	//	}
	//	changeScene(SceneList::RESULT);
	//}

	//�y�v���C���[�̍X�V�z
	//for (int i = 0; i < NUM_PLAYER; i++)
	//{
	//	player[i]->update(frameTime);
	//}

	//D3DXCOLOR* colorList = new D3DXCOLOR[3000];
	//float sinValue = (sinf(gameMaster->getGameTime() * 4)+1.0f)/2.0f;
	//float cosValue = cosf(gameMaster->getGameTime() * 4);
	//for (int i = 0; i < 3000; i++)
	//{
	//	colorList[i] = D3DXCOLOR(sinValue,sinValue,sinValue,sinValue);
	//}
	//plane.setColorBuffer(direct3D9->device, 3000, colorList);
	//SAFE_DELETE_ARRAY(colorList);
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Game::render() {

	//1P�J�����E�E�B���h�E
	//device->SetTransform(D3DTS_VIEW, &camera[PLAYER1].view);
	//device->SetTransform(D3DTS_PROJECTION, &camera[PLAYER1].projection);
	direct3D9->changeViewport1PWindow();
	//render3D(camera[PLAYER1]);

	//2P�J�����E�E�B���h�E
	//device->SetTransform(D3DTS_VIEW, &camera[PLAYER2].view);
	//device->SetTransform(D3DTS_PROJECTION, &camera[PLAYER2].projection);
	direct3D9->changeViewport2PWindow();
	//render3D(camera[PLAYER2]);

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();

}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
//void Game::render3D(Camera currentCamera) {

	//�i�T���v���j�|�C���g�X�v���C�g�̕`��
	//pointSprite.render(direct3D9->device, currentCamera.position);

	//�i�T���v���j�v���[���̕`��(�C���X�^���V���O)
	//plane.render(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);

	//xFileStaticMesh�e�X�g�`��
	//testObject.multipleRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position,
	//	*shaderLoader->getEffect(shaderNS::INSTANCE_STATIC_MESH));
	//testCube.multipleRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position,
	//	*shaderLoader->getEffect(shaderNS::INSTANCE_STATIC_MESH));


	//for (int i = 0; i < NUM_PLAYER; i++)
	//{
	//	// �v���C���[�̕`��
	//	player[i]->toonRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position, *shaderLoader->getEffect(shaderNS::TOON), *textureLoader->getTexture(textureLoaderNS::TOON_SHADE), *textureLoader->getTexture(textureLoaderNS::TOON_OUT_LINE));
	//	// �v���C���[�̑��̃I�u�W�F�N�g�̕`��
	//	player[i]->otherRender(direct3D9->device, currentCamera.view, currentCamera.projection, currentCamera.position);
	//}
//}

//===================================================================================================================================
//�yUI/2D�`��z
//===================================================================================================================================
void Game::renderUI() {

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//===================================================================================================================================
//�y�Փ˔��菈���z
//===================================================================================================================================
void Game::collisions() 
{

}

//===================================================================================================================================
//�yAI�����z
//===================================================================================================================================
void Game::AI() {
#ifdef USING_AI

#endif// USING_AI
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Game::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
