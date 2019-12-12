//===================================================================================================================================
//�yFinale.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/12/12
// [�X�V��]2019/12/12
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Finale.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace FinaleNS;

//============================================================================================================================================
//�y�O���[�o���ϐ��z
//============================================================================================================================================

//============================================================================================================================================
//�y�R���X�g���N�^�z
//============================================================================================================================================
Finale::Finale()
{
	//���̃V�[��(�t�B�i�[��)
	sceneName = ("Scene -Finale-");
	nextScene = SceneList::TITLE;
}

//============================================================================================================================================
//�y�f�X�g���N�^�z
//============================================================================================================================================
Finale::~Finale()
{
}

//============================================================================================================================================
//�y�������z
//============================================================================================================================================
void Finale::initialize()
{

	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//camera->setGaze(D3DXVECTOR3(0, 100, 0));
	//camera->setRelative(D3DXQUATERNION(0.0f, 20.0f, -40.0f, 0.0f));	//�����̒l��
	camera->setRelative(D3DXQUATERNION(0.0f, 0.0f, 20.0f, 0.0f));   //���^�[�Q�b�g�̏����ʒu�ɑ������`�ɂȂ��Ă���
	//camera->setTarget(&target->position);

	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI) / 18 * 10);
	camera->setViewProjection();


	//�G�t�F�N�V�A�[�̐ݒ�
	effekseerNS::setProjectionMatrix(0,
		camera->fieldOfView,
		camera->windowWidth,
		camera->windowHeight,
		camera->nearZ,
		camera->farZ);

	// Light
	light = new Light;
	light->initialize();

	//�^�C�g���t�B�[���h�i�e�X�g�j
	//titleField = new Object();
	//titleFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	//titleFieldRenderer->registerObject(titleField);
	//titleField->initialize(&D3DXVECTOR3(0, 0, 0));

}

//============================================================================================================================================
//�y�I�������z
//============================================================================================================================================
void Finale::uninitialize(void)
{
	// ���C�g
	SAFE_DELETE(light);

	// �J����
	SAFE_DELETE(camera);

	//�^�C�g���t�B�[���h
	//SAFE_DELETE(titleField);
	//SAFE_DELETE(titleFieldRenderer);

}

//============================================================================================================================================
//�y�X�V�z
//============================================================================================================================================
void Finale::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//�^�C�g���t�B�[���h�i�e�X�g�j
	//titleField->update();	//�I�u�W�F�N�g
	//titleFieldRenderer->update();

	// �J����
	//camera[0].setUpVector(player[PLAYER_TYPE::PLAYER_1].getAxisY()->direction);
	//camera[0].update();

	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.updateFinale();

	//�o�[�̈ړ�
	//if(input->wasKeyPressed()


	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		changeScene(nextScene);
	}

	

	//Base::anyAxisRotationSlerp(&cameraQ,D3DXVECTOR3(13.2f, 6.0f, -13.0f),);




	////�J�����ړ�
	//if (input->isKeyDown('W'))
	//{
	//	fixedAxisZ *= 1.0f;
	//	target->position += fixedAxisZ;
	//}
	//if (input->isKeyDown('S'))
	//{
	//	fixedAxisZ *= -1.0f;
	//	target->position += fixedAxisZ;
	//}
	//if (input->isKeyDown('A'))
	//{
	//	cameraAxisX *= -1.0f;
	//	target->position += cameraAxisX;
	//}
	//if (input->isKeyDown('D'))
	//{
	//	cameraAxisX *= 1.0f;
	//	target->position += cameraAxisX;
	//}
	//if (input->isKeyDown('Q'))
	//{
	//	Y *= 1.0f;
	//	target->position += camera->upVector;
	//}
	//if (input->isKeyDown('E'))
	//{
	//	Y *= 1.0f;
	//	target->position -= camera->upVector;

	//}

	////�J������]
	////camera->rotation(D3DXVECTOR3(0, -1, 0), degree);
	////Y��
	//if (input->isKeyDown(VK_RIGHT))
	//{
	//	
	//	camera->rotation(camera->upVector, inputDegree);
	//	//target->quaternion.y += 5.0f;
	//}
	//if (input->isKeyDown(VK_LEFT))
	//{
	//	camera->rotation(-camera->upVector, inputDegree);
	//	//target->quaternion.y -= 5.0f;
	//}
	////X��
	//if (input->isKeyDown(VK_UP))
	//{
	//	camera->rotation(-fixedAxisX, inputDegree);
	//}
	//if (input->isKeyDown(VK_DOWN))
	//{
	//	camera->rotation(fixedAxisX, inputDegree);
	//}
	////�Y�[��
	//if (input->isKeyDown('Z'))
	//{
	//	camera->relativeQuaternion -= camera->relativeQuaternion * 0.05f;
	//}
	//if (input->isKeyDown('X'))
	//{
	//	camera->relativeQuaternion += camera->relativeQuaternion * 0.05f;
	//}



	//�J����
	camera->update();

}

//============================================================================================================================================
//�y�`��z
//============================================================================================================================================
void Finale::render()
{
	// �J�����E�E�B���h�E
	camera->renderReady();
	direct3D9->changeViewportFullWindow();

	//�G�t�F�N�V�A�[�e�X�g�`��
	effekseerNS::setCameraMatrix(0, camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render(0);

	// 3D
	render3D(*camera);

	// 2D
	render2D();

}

//============================================================================================================================================
//�y3D�`��z
//============================================================================================================================================
void Finale::render3D(Camera _currentCamera)
{

	//�^�C�g���t�B�[���h�i�e�X�g�j
	//titleFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), _currentCamera.view, _currentCamera.projection, _currentCamera.position);



}

//============================================================================================================================================
//�y2D�`��z
//============================================================================================================================================
void Finale::render2D()
{
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================================================================================
//�y�Փˏ����z
//============================================================================================================================================
void Finale::collisions(void)
{
	// None
}

//============================================================================================================================================
//�yAI�����z
//============================================================================================================================================
void Finale::AI(void)
{
	// None
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Finale::createGUI()
{
	bool createScene = false;
	float limitTop = 1000;
	float limitBottom = -1000;
	camera->outputGUI();

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Create Scene", &createScene);

	ImGui::Text("controller1 LStick(%.02f,%.02f)",
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().x,
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().y);
	ImGui::Text("controller1 LStickTrigger(%.02f,%.02f)",
		input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().x,
		input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().y);

}
#endif // _DEBUG