//===================================================================================================================================
//�yTitle.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Title.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace titleNS;

//============================================================================================================================================
//�y�O���[�o���ϐ��z
//============================================================================================================================================
//int selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_GAME_START;	//	�^�C�g�����j���[�I����ԋL��

//============================================================================================================================================
//�y�R���X�g���N�^�z
//============================================================================================================================================
Title::Title(void)
{
	// ���̃V�[��( �^�C�g�� )
	sceneName = ("Scene -Title-");
	nextScene = SceneList::SPLASH;
}

//============================================================================================================================================
//�y�f�X�g���N�^�z
//============================================================================================================================================
Title::~Title(void)
{
	// �T�E���h�̒�~
	//sound->stop(soundNS::TYPE::BGM_TITLE);
}

//============================================================================================================================================
//�y�������z
//============================================================================================================================================
void Title::initialize()
{
	// �T�E���h�̍Đ�
	//sound->play(soundNS::TYPE::BGM_TITLE, soundNS::METHOD::LOOP);

	//// Player
	//player[PLAYER_TYPE::PLAYER_1] = new Player;
	//player[PLAYER_TYPE::PLAYER_2] = new Player;

	// Camera
	//camera = new Camera[PLAYER_TYPE::PLAYER_TYPE_MAX];

	//for (int i = 0; i < PLAYER_TYPE::PLAYER_TYPE_MAX; i++)
	//{
	//	camera[i].initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//	//camera[i].setTarget(player[i].getPosition());
	//	//camera[i].setTargetX(&player[i].getAxisX()->direction);
	//	//camera[i].setTargetY(&player[i].getAxisY()->direction);
	//	//camera[i].setTargetZ(&player[i].getAxisZ()->direction);
	//	camera[i].setRelative(CAMERA_RELATIVE_QUATERNION[0]);
	//	camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
	//	camera[i].setRelativeGaze(D3DXVECTOR3(0, 0, 0));
	//	camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
	//	camera[i].setFieldOfView(D3DX_PI / 2.5);
	//}

	// Light
	//light = new Light;
	//light->initialize(_direct3D9);

	// �v���C���[�̏�����
	//player[PLAYER_TYPE::PLAYER_1].initialize(playerNS::PLAYER_TYPE::PLAYER1, gameMaster->getPlayerInfomation()[PLAYER_TYPE::PLAYER_1].modelType, _direct3D9->device, staticMeshLoader, textureLoader, shaderLoader);
	//player[PLAYER_TYPE::PLAYER_1].setPosition(PLAYER_POSITION[playerNS::PLAYER_TYPE::PLAYER1]);
	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.setAnimationConfiguration(animationPlayerNS::SCENE_TYPE::TITLE);

	// �^�C�g��UI�̏�����
	//uiTitle.initialize(_direct3D9->device, _textureLoader, selectStateMemory);

	// �C���X�^���X�v���[���̏�����
	//plane.createPositionSpherical(_direct3D9->device, 3000, 250.0f);
	//plane.initialize(_direct3D9->device, *shaderLoader->getEffect(shaderNS::INSTANCE_BILLBOARD), *textureLoader->getTexture(textureLoaderNS::BACKGROUND_DUST));

	// �V�[���G�t�F�N�g������
	//sceneEffect.initialize(_direct3D9->device, textureLoader, *shaderLoader->getEffect(shaderNS::INSTANCE_BILLBOARD));
}

//============================================================================================================================================
//�y�I�������z
//============================================================================================================================================
void Title::uninitialize(void)
{
	// �v���C���[
	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.release();

	// ���C�g
	//SAFE_DELETE(light);

	// �J����
	//SAFE_DELETE_ARRAY(camera);

	// �^�C�g��UI
	//uiTitle.release();

}

//============================================================================================================================================
//�y�X�V�z
//============================================================================================================================================
void Title::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	// �J����
	//camera[0].setUpVector(player[PLAYER_TYPE::PLAYER_1].getAxisY()->direction);
	//camera[0].update();

	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.updateTitle();

	// �^�C�g��UI
	//uiTitle.update(input, sound);

	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// �T�E���h�̍Đ�
		//sound->play(soundNS::TYPE::SE_DECISION, soundNS::METHOD::PLAY);
		updateInput();
		changeScene(nextScene);
	}

	// �V�[���G�t�F�N�g�̍X�V
	//sceneEffect.update(_frameTime);

	//for (int i = 0; i < EFFECT_MAX; i++)
	//{
	//	// �V�[���G�t�F�N�g����
	//	sceneEffect.generateSceneEffect(1, D3DXVECTOR3((float)(rand() % 100 - 50), (float)(rand() % 100 - 50), (float)(rand() % 100 - 50)));
	//}
}

//============================================================================================================================================
//�y���͂̍X�V�z
//============================================================================================================================================
void Title::updateInput(void)
{
	//switch (uiTitle.getSelectState())
	//{
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_GAME_START:
	//	selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_GAME_START;
	//	nextScene = (SceneList::SELECT);
	//	changeScene(nextScene);
	//	break;
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_TUTORIAL:
	//	selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_TUTORIAL;
	//	nextScene = (SceneList::TUTORIAL);
	//	changeScene(nextScene);
	//	break;
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_OPERATION:
	//	selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_OPERATION;
	//	nextScene = (SceneList::OPERATION);
	//	changeScene(nextScene);
	//	break;
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_CREDIT:
	//	selectStateMemory = uiTitleNS::TITLE_MENU_TYPE::MENU_CREDIT;
	//	nextScene = (SceneList::CREDIT);
	//	changeScene(nextScene);
	//	break;
	//case uiTitleNS::TITLE_MENU_TYPE::MENU_GAME_EXIT:
	//	PostQuitMessage(NULL);
	//	break;
	//default:
	//	break;
	//}
}

//============================================================================================================================================
//�y�`��z
//============================================================================================================================================
void Title::render()
{
	// �J�����E�E�B���h�E
	//device->SetTransform(D3DTS_VIEW, &camera[PLAYER_TYPE::PLAYER_1].view);
	//device->SetTransform(D3DTS_PROJECTION, &camera[PLAYER_TYPE::PLAYER_1].projection);
	direct3D9->changeViewportFullWindow();

	// 3D
	//render3D(camera[PLAYER_TYPE::PLAYER_1]);

	// ���u�����h������
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// ���f�X�e�B�l�[�V�����J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 2D
	render2D();

}

//============================================================================================================================================
//�y3D�`��z
//============================================================================================================================================
//void Title::render3D(Camera _currentCamera)
//{
	// �V�[���G�t�F�N�g�̕`��
	//sceneEffect.render(_direct3D9->device, _currentCamera.view, _currentCamera.projection, _currentCamera.position);

	// �v���[��( �C���X�^���V���O )
	//plane.render(_direct3D9->device, _currentCamera.view, _currentCamera.projection, _currentCamera.position);

	// �^�C�g���v���C���[�`��
	//player[0].toonRender
	//(
	//	_direct3D9->device,
	//	_currentCamera.view,
	//	_currentCamera.projection,
	//	_currentCamera.position,
	//	*shaderLoader->getEffect(shaderNS::TOON),
	//	*textureLoader->getTexture(textureLoaderNS::TOON_SHADE),
	//	*textureLoader->getTexture(textureLoaderNS::TOON_OUT_LINE));
//}

//============================================================================================================================================
//�y2D�`��z
//============================================================================================================================================
void Title::render2D()
{
	// �^�C�g��UI
	//uiTitle.render(_device);
}

//============================================================================================================================================
//�y�Փˏ����z
//============================================================================================================================================
void Title::collisions(void)
{
	// None
}

//============================================================================================================================================
//�yAI�����z
//============================================================================================================================================
void Title::AI(void)
{
	// None
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Title::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG