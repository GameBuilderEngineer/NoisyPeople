//===================================================================================================================================
//�ySplash.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/21
//===================================================================================================================================
#include "Splash.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace splashNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Splash::Splash()
{
	// ���݂̃V�[��(�X�v���b�V��)
	sceneName = "Scene -Splash-";
	// ���̃V�[��(�^�C�g��)
	nextScene = SceneList::TITLE;

	//�V�[���̍X�V
	SoundInterface::SwitchAudioBuffer(SceneList::SPLASH);

	//�Đ��p�����[�^
	memset(playParameters, 0, sizeof(playParameters));
	XAUDIO2_FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.1f, 1.5f };
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SPLASH_SE_LIST::SPLASH_SE_01, false,NULL,true, filterParameters };
	playParameters[1] = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, SPLASH_BGM_LIST::SPLASH_BGM_01, true ,1.0f,true, filterParameters };

	//�Đ�
	SoundInterface::playSound(playParameters[0]);
	SoundInterface::playSound(playParameters[1]);

	//�G�t�F�N�V�A�[�e�X�g
	effekseerNS::setProjectionMatrix(90.0f / 180.0f * 3.14f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 10000.0f);

	enemyTools = new ENEMY_TOOLS;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Splash::~Splash()
{
	//�T�E���h�̒�~
	SoundInterface::stopSound(playParameters[0]);
	SoundInterface::stopSound(playParameters[1]);

	effekseerNS::stop(effekseerNS::TEST0);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Splash::initialize()
{
	// �X�v���b�V��sprite�̍쐬
	splashSprite = new SplashSprite;
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Splash::uninitialize()
{
	//�X�v���b�V���̍폜
	SAFE_DELETE(splashSprite);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Splash::update(float _frameTime)
{
	//�t���[������
	if (_frameTime > 1.0f / (FRAME_RATE / 4))
	{
		stopTimer += _frameTime;					//�����������Ԃ̒~��
		return;
	}
	stopTimer = max(stopTimer-_frameTime,0.0f);		//��~���Ԃ̌���
	sceneTimer += _frameTime;						//�V�[���^�C���̍X�V
	frameTime = _frameTime;							//�t���[���^�C���̕ۑ�

	//�X�v���b�V��2D�X�V
	float rate = sceneTimer / SCENE_TIME;
	splashSprite->update(rate);

	//�G�t�F�N�g�̍Đ�
	if (input->wasKeyPressed('1'))
	{
		effekseerNS::play(effekseerNS::TEST0,D3DXVECTOR3(0,0,0));
	}

	//Enter,Space�܂��́Z�{�^��,Options�Ń^�C�g����
	if (input->wasKeyPressed(VK_RETURN) ||
		input->wasKeyPressed(VK_SPACE) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::SPECIAL_MAIN)
		)
	{
		changeScene(nextScene);
	}

	// �t�F�[�h���I�������^�C�g����
	if(sceneTimer > SCENE_TIME)changeScene(nextScene);
}

//===================================================================================================================================
//�y�`�揈���z
//===================================================================================================================================
void Splash::render()
{
	//�`��Ώۂ��E�B���h�E�S�̂ɐؑ�
	direct3D9->changeViewportFullWindow();

	//�G�t�F�N�V�A�[�e�X�g
	effekseerNS::setCameraMatrix(D3DXVECTOR3(10.0f, 5.0f, 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	effekseerNS::render();

	//UI
	renderUI();

}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
//void Splash::render3D(Camera _currentCamera)
//{
//
//}

//===================================================================================================================================
//�yUI�`��z
//===================================================================================================================================
void Splash::renderUI()
{
	// �X�v���b�V��2D�`��
	splashSprite->render();

}

//===================================================================================================================================
//�y�Փˏ����z
//===================================================================================================================================
void Splash::collisions()
{

}

//===================================================================================================================================
//�yAI�����z
//===================================================================================================================================
void Splash::AI()
{

}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Splash::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("stopTime = %f", stopTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
