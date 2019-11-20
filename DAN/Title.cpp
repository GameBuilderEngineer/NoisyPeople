//===================================================================================================================================
//�yTitle.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/17
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
	nextScene = SceneList::TUTORIAL;

	//�Đ��p�����[�^
	PLAY_PARAMETERS playParameters = { 0 };//�����ɍĐ���������
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.1f, 1.5f };//�t�B���^�[�̐ݒ�
	//�Đ�����Ȃ̎w��T�E���hID,���[�v,�X�s�[�hNULL�ł��Ȃ�,��{false,��{NULL,�t�B���^�[���g�����g��Ȃ���
	playParameters= { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Title, true,1.0f,false,NULL,true, filterParameters };//BGM�̐ݒ�

	//�Đ�
	SoundInterface::BGM->playSound(&playParameters);

	//������
	tmpVolume = 1.0f;
}

//============================================================================================================================================
//�y�f�X�g���N�^�z
//============================================================================================================================================
Title::~Title(void)
{
	// �T�E���h�̒�~
	SoundInterface::BGM->uninitSoundStop();
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

	//�G�t�F�N�V�A�[�̐ݒ�
	effekseerNS::setProjectionMatrix(
		camera->fieldOfView,
		camera->windowWidth,
		camera->windowHeight,
		camera->nearZ,
		camera->farZ);

	//WaveBall
	waveBall = new WaveBall;

	// Light
	light = new Light;
	light->initialize();

	//�^�C�g��UI�̏�����
	titleUI.initialize();

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect = new TestEffect();
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

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	SAFE_DELETE(testEffect);

	//WaveBall
	SAFE_DELETE(waveBall);

	// �^�C�g��UI
	titleUI.uninitialize();

}

//============================================================================================================================================
//�y�X�V�z
//============================================================================================================================================
void Title::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect->update(frameTime);

	// �J����
	//camera[0].setUpVector(player[PLAYER_TYPE::PLAYER_1].getAxisY()->direction);
	//camera[0].update();

	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.updateTitle();

	//�o�[�̈ړ�
	//if(input->wasKeyPressed()

	//�G�t�F�N�g�̍Đ�
	if (input->wasKeyPressed('1'))
	{
		effekseerNS::play(new effekseerNS::Instance);
	}
	//�~���[�g
	if (input->isKeyDown('M'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(0.0f);
		SoundInterface::BGM->setEndPointVoiceVolume(0.0f);
		SoundInterface::S3D->setEndPointVoiceVolume(0.0f);
		waveBall->setVolume(0.0f);
		waveBall->setOnCol(false);
	}
	//�~���[�g����
	if (input->isKeyDown('N'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(1.0f);
		SoundInterface::BGM->setEndPointVoiceVolume(1.0f);
		SoundInterface::S3D->setEndPointVoiceVolume(1.0f);
		waveBall->setVolume(1.0f);
		waveBall->setOnCol(true);
	}
	//10%(test)
	if (input->isKeyDown('B'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(0.1f);
		SoundInterface::BGM->setEndPointVoiceVolume(0.1f);
		SoundInterface::S3D->setEndPointVoiceVolume(0.1f);
		waveBall->setVolume(0.1f);
		waveBall->setOnCol(true);
	}

	// �^�C�g��UI
	titleUI.update(input);

	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{

		updateInput();
		changeScene(nextScene);
	}

	//�J����
	camera->update();
}

//============================================================================================================================================
//�y���͂̍X�V�z
//============================================================================================================================================
void Title::updateInput(void)
{
	switch (titleUI.getSelectState())
	{
	case titleUiNS::TUTORIAL:
		selectStateMemory = titleUiNS::TUTORIAL;
		nextScene = (SceneList::TUTORIAL);
		changeScene(nextScene);
		break;
	case titleUiNS::GAME:
		selectStateMemory = titleUiNS::GAME;
		nextScene = (SceneList::GAME);
		changeScene(nextScene);
		break;
	case titleUiNS::CREDIT:
		selectStateMemory = titleUiNS::CREDIT;
		nextScene = (SceneList::CREDIT);
		changeScene(nextScene);
		break;
	case titleUiNS::EXIT:
		PostQuitMessage(NULL);
		break;
	
	default:
		break;
	}
}

//============================================================================================================================================
//�y�`��z
//============================================================================================================================================
void Title::render()
{
	// �J�����E�E�B���h�E
	camera->renderReady();
	direct3D9->changeViewportFullWindow();

	//�G�t�F�N�V�A�[�e�X�g�`��
	effekseerNS::setCameraMatrix(camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render();

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
	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect->render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);

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
	titleUI.render();

#if _DEBUG
	//WaveBall
	waveBall->draw();
#endif

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
	bool createScene = false;
	float backUpTmpVolume = tmpVolume;

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("node:%d", testEffect->getList().nodeNum);
	ImGui::Checkbox("Create Scene", &createScene);
	ImGui::SliderFloat("volume control", &tmpVolume, 0.0f, 1.0f);

	if (backUpTmpVolume != tmpVolume)
	{
		backUpTmpVolume = tmpVolume;
		waveBall->setVolume(tmpVolume);
	}

	//�c�[���p�V�[��
	if (createScene)
	{
		selectStateMemory = titleUiNS::CREATE;
		nextScene = (SceneList::CREATE);
		changeScene(nextScene);
	}
}
#endif // _DEBUG