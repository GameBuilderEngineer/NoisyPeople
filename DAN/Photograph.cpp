//===================================================================================================================================
//�yPhotograph.cpp�z
// [�쐬��]HAL����GP12A332 21 �V�� ���m
// [�쐬��]2019/12/26
// [�X�V��]2019/12/26
//===================================================================================================================================
#ifdef _DEBUG
//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Photograph.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace photographNS;


//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Photograph::Photograph()
{
	sceneName = "Scene -Photograph-";
	nextScene = SceneList::TITLE;

	//�G�l�~�[�c�[��
	enemyTools = new ENEMY_TOOLS;

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Photograph::~Photograph()
{
	//�G�l�~�[�c�[��
	SAFE_DELETE(enemyTools);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Photograph::initialize()
{
	//���I�u�W�F�N�g
	tmpObject = new TmpObject;

	//�^�[�Q�b�g
	target = new Object;
	target->initialize(&D3DXVECTOR3(0, 0, 0));

	//�B�e�J����
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setTarget(tmpObject->getPositionYeah());
	camera->setTargetX(&tmpObject->getAxisX()->direction);
	camera->setTargetY(&tmpObject->getAxisY()->direction);
	camera->setTargetZ(&tmpObject->getAxisZ()->direction);
	camera->setRelative(D3DXQUATERNION(0.0f, 60.0f, -15.0f, 0.0f));
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setGazeDistance(10.0f);
	camera->setRelativeGaze(D3DXVECTOR3(0, 0, 0));
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI) / 18 * 9);
	camera->setLimitRotationTop(0.1f);
	camera->setLimitRotationBottom(0.1f);

	//���ՃJ����
	topView = new Camera;
	topView->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	topView->setTarget(tmpObject->getPositionYeah());
	topView->setTargetX(&tmpObject->getAxisX()->direction);
	topView->setTargetY(&tmpObject->getAxisY()->direction);
	topView->setTargetZ(&tmpObject->getAxisZ()->direction);
	topView->setRelative(D3DXQUATERNION(1.0f, 100.0f, 0.0f, 0.0f));
	topView->setGazeDistance(100.0f);
	topView->setGaze(D3DXVECTOR3(0, 0, 0));
	topView->setRelativeGaze(D3DXVECTOR3(0, 0, 0));
	topView->setUpVector(D3DXVECTOR3(0, 1, 0));
	topView->setFieldOfView((D3DX_PI / 18) * 9);
	onTopView = false;

	//���C�g
	light = new Light;
	light->initialize();

	//�e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//�v���C���[�̏�����
	tmpObject->initialize(inputNS::DINPUT_1P, 0);
	tmpObject->setCamera(camera);	//�J�����|�C���^�̃Z�b�g

	tmpObject->configurationGravityWithRay(testField->getPosition(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());	//�d�͂�ݒ�
	tmpObjRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::WOLF));
	tmpObjRenderer->registerObject(tmpObject);

	//�B�e�J�����m�F�p
	cameraObject = new Object;
	cameraObject->initialize(&tmpObject->position);
	cameraObjectRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::WOLF));
	cameraObjectRenderer->registerObject(cameraObject);
	
	cameraAxisZ = D3DXVECTOR3(0, 0, 0);
	fixedAxisZ = D3DXVECTOR3(0, 0, 0);
	cameraAxisX = D3DXVECTOR3(0, 0, 0);
	cameraAxisY = D3DXVECTOR3(0, 0, 0);
	fixedAxisX = D3DXVECTOR3(0, 0, 0);

	PointToolsListboxType = POINT_TYPE::POINT_NONE;

}


//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Photograph::uninitialize()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(topView);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(testField);
	SAFE_DELETE(tmpObjRenderer);
	SAFE_DELETE(light);
	SAFE_DELETE(target);
	SAFE_DELETE(cameraObject);
	SAFE_DELETE(cameraObjectRenderer);
}

//===================================================================================================================================
//�y�X�V�����z
//===================================================================================================================================
void Photograph::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//���������΍�
	if (frameTime > 0.10)return;

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();
	testFieldRenderer->update();

	//�c�[���̍X�V
	enemyTools->update();

	D3DXVECTOR3 difference;
	float normal;
	if (bezier)
	{
		photoTimer -= frameTime;
		rate = photoTimer / photoTime;
		cameraObject->position = setBezier(startPoint, curvePoint, endPoint, rate);
		difference = endPoint - cameraObject->position;
		normal = D3DXVec3Length(&difference);
		if (normal <= 1.0f && normal >= -1.0f)
		{
			camera->position = startPoint;
			photoTimer = photoTime;
		}
	}

	//�����I�u�W�F�N�g�ƃJ�����̓�_�ԃx�N�g���i�J����Z���x�N�g���j
	cameraAxisZ = camera->getAxisZ();
	fixedAxisZ = Base::slip(cameraAxisZ, camera->upVector);	//�J�����̌X���ɑΉ�
	//�J����X���x�N�g��
	D3DXVec3Cross(&cameraAxisX, &camera->upVector, &cameraAxisZ);
	//�J����Y���x�N�g���i������̍X�V�j
	D3DXVECTOR3 Y = D3DXVECTOR3(0, 0, 0);
	D3DXVec3Cross(&Y, &fixedAxisZ, &cameraAxisX);
	D3DXVec3Cross(&cameraAxisY, &cameraAxisZ, &cameraAxisX);
	D3DXVec3Cross(&fixedAxisX, &cameraAxisY, &cameraAxisZ);

	//�^�[�Q�b�g�I�u�W�F�N�g�̍X�V
	tmpObject->update(frameTime);
	LPD3DXMESH mesh = testFieldRenderer->getStaticMesh()->mesh;
	D3DXMATRIX matrix = testField->matrixWorld;
	
	if (input->wasKeyPressed('G'))
	{
		tmpObject->grounding(mesh, matrix);
	}
	tmpObjRenderer->update();
	//���ՃJ�����̍X�V
	topView->update();
	
	//�^�[�Q�b�g�J�����̍X�V
	camera->update();

	//�J�����̍X�V
	tmpObjRenderer->update();

	//�J�����m�F�pobj
	cameraObject->update();
	cameraObjectRenderer->update();

	//Enter�܂��́Z�{�^���Ń��U���g��
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		//�V�[���J��
		changeScene(nextScene);
	}

}

//===================================================================================================================================
//�y�`�揈���z
//===================================================================================================================================
void Photograph::render()
{
	//�J�����E�E�B���h�E
	direct3D9->changeViewportFullWindow();

	if (input->wasKeyPressed('Z'))
	{
		onTopView = !onTopView;
	}

	if (onTopView)
	{
		topView->renderReady();
		render3D(*topView);
		effekseerNS::setCameraMatrix(0, topView->position, topView->gazePosition, topView->upVector);
		effekseerNS::render(0);
	}
	else
	{
		camera->renderReady();
		render3D(*camera);
		effekseerNS::setCameraMatrix(0, camera->position, camera->gazePosition, camera->upVector);
		effekseerNS::render(0);

	}

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();
}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
void Photograph::render3D(Camera currentCamera)
{
	//�^�C�g���t�B�[���h�i�e�X�g�j
	testField->setAlpha(0.1f);
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	// �v���C���[�̕`��
	tmpObjRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	tmpObject->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�B�e�J�����m�F�p�I�u�W�F�N�g�̕`��
	cameraObjectRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	//viewCamera->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�c�[���̕`��
	enemyTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);

}


//===================================================================================================================================
//�yUI/2D�`��z
//===================================================================================================================================
void Photograph::renderUI()
{
	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


}

//===================================================================================================================================
//�y�Փ˔��菈���z
//===================================================================================================================================
void Photograph::collisions()
{

}

//===================================================================================================================================
//�yAI�����z
//===================================================================================================================================
void Photograph::AI() 
{

}




//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
void Photograph::createGUI()
{
	float limitTop = 1000;
	float limitBottom = -1000;

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat3("StartPoint ", startPoint, limitBottom, limitTop);
	ImGui::SliderFloat3("EndPoint", endPoint, limitBottom, limitTop);
	ImGui::SliderFloat3("CurvePoint", curvePoint, limitBottom, limitTop);
	testField->outputGUI();			//�e�X�g�t�B�[���h
	camera->outputGUI();			//�J����
	tmpObject->outputGUI();			//�v���C���[
	cameraObject->outputGUI();		//�J�����^�[�Q�b�g
	toolsGUI();
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
void Photograph::toolsGUI()
{
	bool runFlag = false;
	bool deleteFlag = false;

	//�J�\�ȃt�B�[���h
	if (ImGui::CollapsingHeader("Tool"))
	{
		if (ImGui::TreeNode("TreeNode A"))
		{
			if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
				ImGui::Text("PressKeyboard A");
			ImGui::TreePop();
		}

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("TreeNode B"))
		{
			ImGui::Text("TreeNode B is open from the Beginning");
			ImGui::TreePop();
		}

		const char* listboxPointType[] = { "NONE", "START", "END", "CURVE" };
		ImGui::ListBox("Point Type", &PointToolsListboxType, listboxPointType, POINT_TYPE::POINT_MAX);


		outputStartPointGUI(PointToolsListboxType, tmpObject->getPosition());
		outputEndPointGUI(PointToolsListboxType, tmpObject->getPosition());
		outputCurvePointGUI(PointToolsListboxType, tmpObject->getPosition());

		int backupMeshId = meshId;
		switch (PointToolsListboxType)
		{
		case POINT_TYPE::POINT_NONE:
			break;
		case POINT_TYPE::POINT_START:
			break;
		case POINT_TYPE::POINT_END:
			
			break;
		}
		
		ImGui::Checkbox("RUN", &runFlag);
		ImGui::Checkbox("Delete", &deleteFlag);

		if (runFlag)
		{
			bezier = true;
		}

	}
}

//===================================================================================================================================
//�yImGUI�ւ̏o��(StartPoint)�z
//===================================================================================================================================
void Photograph::outputStartPointGUI(int GUIid,  D3DXVECTOR3* pos)
{
	if (GUIid == START_GUI_ID)
	{

		bool createFlag = false;
		bool deleteFlag = false;

		ImGui::Checkbox("Create Start_P", &createFlag);
		ImGui::Checkbox("Delete Start_P", &deleteFlag);

		if (createFlag)
		{
			startEffect = new photographNS::StartEffect(0, pos);
			effekseerNS::play(0, startEffect);
			startPoint = startEffect->position;
		}
		if (deleteFlag)
		{
			effekseerNS::stop(0, startEffect);
			startPoint = startEffect->position;
		}

	}
}
//===================================================================================================================================
//�yImGUI�ւ̏o��(EndPoint)�z
//===================================================================================================================================
void Photograph::outputEndPointGUI(int GUIid, D3DXVECTOR3* pos)
{
	if (GUIid == END_GUI_ID)
	{

		bool createFlag = false;
		bool deleteFlag = false;

		ImGui::Checkbox("Create End_P", &createFlag);
		ImGui::Checkbox("Delete End_P", &deleteFlag);

		if (createFlag)
		{
			endEffect = new photographNS::EndEffect(0, pos);
			effekseerNS::play(0, endEffect);
			endPoint = endEffect->position;
		}
		if (deleteFlag)
		{
			effekseerNS::stop(0, endEffect);
			endPoint = endEffect->position;
		}
	}
}

//===================================================================================================================================
//�yImGUI�ւ̏o��(CurvePoint)�z
//===================================================================================================================================
void Photograph::outputCurvePointGUI(int GUIid, D3DXVECTOR3* pos)
{
	if (GUIid == CURVE_GUI_ID)
	{
		bool createFlag = false;
		bool deleteFlag = false;

		ImGui::Checkbox("Create Curve_P", &createFlag);
		ImGui::Checkbox("Delete Curve_P", &deleteFlag);

		if (createFlag)
		{
			curveEffect = new photographNS::CurveEffect(0, pos);
			effekseerNS::play(0, curveEffect);
			curvePoint = curveEffect->position;
		}
		if (deleteFlag)
		{
			effekseerNS::stop(0, curveEffect);
			curvePoint = curveEffect->position;
		}
	}
}
void Photograph::addCurvePoint(D3DXVECTOR3* pos)
{

}


D3DXVECTOR3 Photograph::setBezier(D3DXVECTOR3 startpoint, D3DXVECTOR3 curvepoint, D3DXVECTOR3 endpoint, float rate)
{
	D3DXVECTOR3 P0_1;
	D3DXVECTOR3 P1_2;
	D3DXVECTOR3 result;

	D3DXVec3Lerp(&P0_1, &startpoint, &curvepoint, 1.0f - rate);
	D3DXVec3Lerp(&P1_2, &curvepoint, &endpoint, 1.0f - rate);
	D3DXVec3Lerp(&result, &P0_1, &P1_2, 1.0f - rate);

	return result;
}

#endif
