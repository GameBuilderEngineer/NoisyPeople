//===================================================================================================================================
//�yGame.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/04
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

	//player
	player = new Player;

	//camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setTarget(player->getPosition());
	camera->setTargetX(&player->getAxisX()->direction);
	camera->setTargetY(&player->getAxisY()->direction);
	camera->setTargetZ(&player->getAxisZ()->direction);
	camera->setRelative(CAMERA_RELATIVE_QUATERNION);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setRelativeGaze(CAMERA_RELATIVE_GAZE);
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView( (D3DX_PI/18) * 9 );

	//light
	light = new Light;
	light->initialize();

	//�e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::FIELD));
	testFieldRenderer->generateObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//�v���C���[�̏�����
	player->initialize(inputNS::DINPUT_1P, 0);
	player->setCamera(camera);	//�J�����|�C���^�̃Z�b�g
	playerRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON));
	playerRenderer->generateObject(player);
	player->configurationGravityWithRay(testField->getPosition(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());	//�d�͂�ݒ�

	//�΂̏�����
	stone = new Stone();

	// �T�E���h�̍Đ�
	//sound->play(soundNS::TYPE::BGM_GAME, soundNS::METHOD::LOOP);

	//�e�L�X�g�̏�����
	//text.initialize(direct3D9->device,10,10, 0xff00ff00);
	//text2.initialize(direct3D9->device,11,11, 0xff0000ff);

	//�|�C���g�X�v���C�g
	//pointSprite.initilaize(direct3D9->device);

	//�C���X�^���V���O�r���{�[�h�e�X�g
	instancingBillboardTest.initialize(
		*shaderNS::reference(shaderNS::INSTANCE_BILLBOARD),
		*textureNS::reference(textureNS::LIGHT_001)
	);

}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Game::uninitialize() {
	SAFE_DELETE(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testField);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(player);
	SAFE_DELETE(playerRenderer);
	SAFE_DELETE(stone);
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
	if (frameTime > 0.10)return;

	//�C���X�^���V���O�r���{�[�h�e�X�g
	//instancingBillboardTest.update(frameTime);

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();
	testFieldRenderer->update();

	//�v���C���[�̍X�V
	player->update(frameTime);
	playerRenderer->update();

	//�΂̍X�V
	stone->update();

	//�J�����̍X�V
	camera->update();

	//3D�T�E���h
	//�v���C���[�̈ʒu�ƌ���	
	//sound->updateSound(*player->getPosition(), player->getAxisZ()->direction);
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Game::render() {

	//1P�J�����E�E�B���h�E
	camera->renderReady();
	direct3D9->changeViewportFullWindow();
	render3D(*camera);


	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();

}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
void Game::render3D(Camera currentCamera) {

	//�e�X�g�t�B�[���h�̕`��
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	//�i�T���v���j�v���[���̕`��(�C���X�^���V���O)
	instancingBillboardTest.render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// �v���C���[�̕`��
	playerRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// �v���C���[�̑��̃I�u�W�F�N�g�̕`��
	player->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�΂̕`��
	stone->render(currentCamera.view, currentCamera.projection, currentCamera.position);
}

//===================================================================================================================================
//�yUI/2D�`��z
//===================================================================================================================================
void Game::renderUI() {

	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	//device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ���e�X�g�𖳌���
	//device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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
	ImGui::Text("node:%d", instancingBillboardTest.getList().nodeNum);

	player->outputGUI();				//�v���C���[
	testField->outputGUI();			//�e�X�g�t�B�[���h
	camera->outputGUI();			//�J����

}
#endif // _DEBUG
