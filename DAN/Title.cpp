//===================================================================================================================================
//�yTitle.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/30
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
	nextScene = SceneList::GAME;

	//�V�[���̍X�V
	SoundInterface::SwitchAudioBuffer(SceneList::TITLE);	

	//�Đ��p�����[�^
	memset(playParameters, 0, sizeof(playParameters));
	XAUDIO2_FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.1f, 1.5f };
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,TITLE_SE_LIST::TITLE_SE_01, false,NULL,true, filterParameters };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,TITLE_SE_LIST::TITLE_SE_02, false,NULL,true, filterParameters };
	playParameters[2] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, TITLE_BGM_LIST::TITLE_BGM_01, true,1.0f,true, filterParameters };

	//�Đ�
	SoundInterface::playSound(playParameters[0]);
	SoundInterface::playSound(playParameters[1]);
	SoundInterface::playSound(playParameters[2]);
}

//============================================================================================================================================
//�y�f�X�g���N�^�z
//============================================================================================================================================
Title::~Title(void)
{
	// �T�E���h�̒�~
	SoundInterface::stopSound(playParameters[0]);
	SoundInterface::stopSound(playParameters[1]);
	SoundInterface::stopSound(playParameters[2]);
}

//============================================================================================================================================
//�y�������z
//============================================================================================================================================
void Title::initialize()
{
	// �T�E���h�̍Đ�
	//sound->play(soundNS::TYPE::BGM_TITLE, soundNS::METHOD::LOOP);

	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setRelative(D3DXQUATERNION(0.0f, 20.0f, -40.0f, 0.0f));
	camera->setRelativeGaze(D3DXVECTOR3(0, 0, 0));
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI) / 18 * 10);

	// Light
	light = new Light;
	light->initialize();

	// �^�C�g��UI�̏�����
	//uiTitle.initialize(_direct3D9->device, _textureLoader, selectStateMemory);

	//�C���X�^���V���O�r���{�[�h�e�X�g
	plane.initialize(
		*shaderNS::reference(shaderNS::INSTANCE_BILLBOARD),
		*textureNS::reference(textureNS::LIGHT_001)
	);

}

//============================================================================================================================================
//�y�I�������z
//============================================================================================================================================
void Title::uninitialize(void)
{
	// ���C�g
	SAFE_DELETE(light);

	// �J����
	SAFE_DELETE(camera);

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

	plane.update(frameTime);

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
	camera->update();
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
	camera->renderReady();
	direct3D9->changeViewportFullWindow();

	// 3D
	render3D(*camera);

	// 2D
	render2D();

}

//============================================================================================================================================
//�y3D�`��z
//============================================================================================================================================
void Title::render3D(Camera _currentCamera)
{
	// �V�[���G�t�F�N�g�̕`��
	//sceneEffect.render(_direct3D9->device, _currentCamera.view, _currentCamera.projection, _currentCamera.position);

	// �v���[��( �C���X�^���V���O )
	plane.render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);

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

}

//============================================================================================================================================
//�y2D�`��z
//============================================================================================================================================
void Title::render2D()
{
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �^�C�g��UI
	//uiTitle.render(_device);

	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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
	ImGui::Text("node:%d", plane.getList().nodeNum);

}
#endif // _DEBUG