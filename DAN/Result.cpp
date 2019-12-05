//===================================================================================================================================
//�yResult.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/12/05
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Result.h"
#include "Sound.h"
#include "UtilityFunction.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace resultNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Result::Result(void)
{
	// ���̃V�[��( ���U���g )
	sceneName = ("Scene -Result-");

	// ���̃V�[��( �^�C�g�� )
	nextScene = SceneList::TITLE;




}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Result::~Result(void)
{
	// �T�E���h�̒�~
	SoundInterface::BGM->uninitSoundStop();
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Result::initialize()
{

	//�e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//camera
	camera = new Camera;
	//�J�����̐ݒ�
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setTarget(&testField->position);
	camera->setTargetX(&testField->getAxisX()->direction);
	camera->setTargetY(&testField->getAxisY()->direction);
	camera->setTargetZ(&testField->getAxisZ()->direction);
	camera->setRelative(CAMERA_RELATIVE_QUATERNION);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI / 180) * 90);
	camera->setLimitRotationTop(0.1f);
	camera->setLimitRotationBottom(0.1f);

	//�G�t�F�N�V�A�[�̐ݒ�
	effekseerNS::setProjectionMatrix(0,
		camera->fieldOfView,
		(float)camera->windowWidth,
		(float)camera->windowHeight,
		camera->nearZ,
		camera->farZ);

	//���U���gUI�̏�����
	resultUI.initialize();

}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Result::uninitialize(void)
{
	SAFE_DELETE(camera);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(testField);

	resultUI.uninitialize();
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Result::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//UI�̍X�V����
	resultUI.update(frameTime);

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[


	//���U���g�t�F�C�Y��5�̎��̂� Enter�܂��́Z�{�^���Ń^�C�g����
	if (resultUI.resultPhase == resultUiNS::PHASE_05&&
		input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// �T�E���h�̍Đ�
		//sound->play(soundNS::TYPE::SE_DECISION, soundNS::METHOD::PLAY);
		// �V�[���J��
		changeScene(nextScene);
	}



	//�J�����̍X�V
	{
		float rate = (sinf(sceneTimer*6)/2.0f) + 0.5f;
		float fov = UtilityFunction::lerp((D3DX_PI / 180) * 70, (D3DX_PI / 180) * 90, rate);
		camera->setFieldOfView(fov);
		camera->rotation(D3DXVECTOR3(0, 1, 0), CAMERA_SPEED*frameTime);
		camera->update();
	}
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Result::render()
{
	//�`��Ώۂ��E�B���h�E�S�̂ɐؑ�
	direct3D9->changeViewportFullWindow();

	//�J�����̕`�揀��
	camera->renderReady();

	//3D�`��
	render3D(*camera);

	//�G�t�F�N�V�A�[�̕`��
	effekseerNS::setCameraMatrix(
		0,
		camera->position,
		camera->gazePosition,
		camera->upVector);
	effekseerNS::render(0);

	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h������
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	//UI
	renderUI();
}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
void Result::render3D(Camera currentCamera)
{
	//�e�X�g�t�B�[���h�̕`��
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

}

//===================================================================================================================================
//�y2D[UI]�`��z
//===================================================================================================================================
void Result::renderUI()
{
	// ���U���gUI
	resultUI.render();
}

//===================================================================================================================================
//�y�Փˏ����z
//===================================================================================================================================
void Result::collisions(void)
{
	// None
}

//===================================================================================================================================
//�yAI�����z
//===================================================================================================================================
void Result::AI(void)
{
	// None
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Result::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
