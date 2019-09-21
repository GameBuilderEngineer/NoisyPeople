//===================================================================================================================================
//�ySplash.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#include "Splash.h"
//#include "Sound.h"

//===================================================================================================================================
// �yusing�錾�z
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
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Splash::~Splash()
{

}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Splash::initialize()
{
	// �T�E���h�̍Đ�
	//sound->play(soundNS::TYPE::BGM_SPLASH, soundNS::METHOD::PLAY);

	// Camera
	//camera = new Camera[PLAYER_TYPE::PLAYER_TYPE_MAX];

	//for (int i = 0; i < PLAYER_TYPE::PLAYER_TYPE_MAX; i++)
	//{
		//camera[i].initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
		//camera[i].setTarget(player[i].getPosition());
		//camera[i].setTargetX(&player[i].getAxisX()->direction);
		//camera[i].setTargetY(&player[i].getAxisY()->direction);
		//camera[i].setTargetZ(&player[i].getAxisZ()->direction);
		//camera[i].setRelative(CAMERA_RELATIVE_QUATERNION[0]);
		//camera[i].setGaze(D3DXVECTOR3(0, 0, 0));
		//camera[i].setRelativeGaze(D3DXVECTOR3(0, 0, 0));
		//camera[i].setUpVector(D3DXVECTOR3(0, 1, 0));
		//camera[i].setFieldOfView(D3DX_PI / 2.5);
	//}

	// Light
	//light = new Light;
	//light->initialize(direct3D9);

	// �X�v���b�V��2D������
	//splash2D.initialize(direct3D9->device,0, _textureLoader);
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Splash::uninitialize()
{
	// �T�E���h�̒�~
	//sound->stop(soundNS::TYPE::BGM_SPLASH);

	// �J����
	//SAFE_DELETE_ARRAY(camera);

	// ���C�g
	//SAFE_DELETE(light);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Splash::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	// �J�����X�V
	//camera->update();

	// �X�v���b�V��2D�X�V
	//splash2D.update();

	//Enter,Space�܂��́Z�{�^��,Options�Ń^�C�g����
	if (input->wasKeyPressed(VK_RETURN) ||
		input->wasKeyPressed(VK_SPACE) ||
		input->getController()[PLAYER_TYPE::PLAYER_1]->wasButton(virtualControllerNS::A) ||
		input->getController()[PLAYER_TYPE::PLAYER_2]->wasButton(virtualControllerNS::A) ||
		input->getController()[PLAYER_TYPE::PLAYER_1]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[PLAYER_TYPE::PLAYER_2]->wasButton(virtualControllerNS::SPECIAL_MAIN)
		)changeScene(nextScene);


	// �t�F�[�h���I�������^�C�g����
	//if (splash2D.gotitle)changeScene(nextScene);
}

//===================================================================================================================================
//�y�`�揈���z
//===================================================================================================================================
void Splash::render()
{
	//1P�J�����E�E�B���h�E
	//device->SetTransform(D3DTS_VIEW, &camera[PLAYER_TYPE::PLAYER_1].view);
	//device->SetTransform(D3DTS_PROJECTION, &camera[PLAYER_TYPE::PLAYER_1].projection);
	direct3D9->changeViewportFullWindow();

	//render3D(camera[PLAYER_TYPE::PLAYER_1]);

	// ���u�����h������
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// ���f�X�e�B�l�[�V�����J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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
	// ���e�X�g��L����
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �X�v���b�V��2D�`��
	//splash2D.render(device);

	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
