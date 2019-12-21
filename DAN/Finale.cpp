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
	//�^�[�Q�b�g�I�u�W�F�N�g
	target = new Object;
	//target->initialize(&D3DXVECTOR3(0.0f, 100.0f, -150));		//�^�[�Q�b�g�̏����ʒu�ݒ�
	target->initialize(&D3DXVECTOR3(0.0f, 20.0f, -300));		//�^�[�Q�b�g�̏����ʒu�ݒ�
	
	//�����t�H�g�O���t
	stateCamera = CAMERA0;
	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//camera->setGaze(D3DXVECTOR3(0, 100, 0));
	//camera->setRelative(D3DXQUATERNION(0.0f, 20.0f, -40.0f, 0.0f));	//�����̒l��
	camera->setRelative(D3DXQUATERNION(0.0f, 0.0f, -1.0f, 0.0f));   //���^�[�Q�b�g�̏����ʒu�ɑ������`�ɂȂ��Ă���
	camera->setTarget(&target->position);
	camera->setGazeDistance(500.0f);
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

	//�t�B�i�[���t�B�[���h�i�e�X�g�j
	finaleField = new Object();
	finaleFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	finaleFieldRenderer->registerObject(finaleField);
	finaleField->initialize(&D3DXVECTOR3(0, 0, 0));

	//�X�J�C�h�[��
	sky = new Sky();

	cameraAxisZ = D3DXVECTOR3(0, 0, 0);
	fixedAxisZ = D3DXVECTOR3(0, 0, 0);
	cameraAxisX = D3DXVECTOR3(0, 0, 0);
	cameraAxisY = D3DXVECTOR3(0, 0, 0);
	fixedAxisX = D3DXVECTOR3(0, 0, 0);
	cameraRelativeQuaternion = CAMERA_RELATIVE_QUATERNION;

	// �c���[�̏�����
	treeManager = new TreeManager();
	treeManager->initialize(finaleFieldRenderer->getStaticMesh()->mesh, finaleField->getMatrixWorld());
	// �c���[���c�[���������ɐݒu����
	treeManager->createUsingTool();
	// �񃔃B�W�����̕`��
	treeManager->switchingNormalView(0);
	for (int i = 0; i < treeManager->getTreeList().size(); i++)
	{
		treeManager->getTreeList()[i]->transState();
	}
	int unko = treeManager->getTreeNum();

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
	SAFE_DELETE(finaleField);
	SAFE_DELETE(finaleFieldRenderer);

	//�X�J�C�h�[��
	SAFE_DELETE(sky);

	//�^�[�Q�b�g�I�u�W�F�N�g
	SAFE_DELETE(target);

	// �c���[�}�l�[�W���[
	SAFE_DELETE(treeManager);
}

//============================================================================================================================================
//�y�X�V�z
//============================================================================================================================================
void Finale::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//�^�C�g���t�B�[���h�i�e�X�g�j
	finaleField->update();	//�I�u�W�F�N�g
	finaleFieldRenderer->update();

	//�X�J�C�t�B�[���h�̍X�V
	sky->update();

	target->update();

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

	// �c���[�̍X�V
	treeManager->update(frameTime);

	switch (stateCamera)
	{
	case CAMERA0:
		if (sceneTimer > 0.0f)
		{
			startPos = target->position;	//���[�v�n�_
			moveTime = 3.0f;				//�I�_�܂ł̎���
			moveTimer = moveTime;			//�ړ��^�C�}�[
			stateCamera++;
			//�J�����̑��Έʒu���ꎞ�ۑ�
			tmpCameraQ = camera->relativeQuaternion;
		}
		break;
	case CAMERA1:

		if (moveTimer > 0.0f)
		{
			moveTimer -= frameTime;
			rate = moveTimer / moveTime;

			distance = UtilityFunction::lerp(500, 20, 1.0f - rate);
			camera->setGazeDistance(distance);
			//D3DXVec3Lerp(&target->position, &D3DXVECTOR3(0.0f, 100.0f, -150), &D3DXVECTOR3(0.0f, 20.0f, -300), 1.0f - rate);
			if (moveTimer <= 0)
			{
				target->position = D3DXVECTOR3(0.0f, 20.0f, -300);
				startPos = target->position;	//���[�v�n�_
				moveTime = 10.0f;				//�I�_�܂ł̎���
				moveTimer = moveTime;			//�ړ��^�C�}�[
				degreeTime = 10.0;				//��]���ԁiY���j
				degreeTimer = degreeTime;		//��]�^�C�}�[�iY���j
				degreeTimeX = 2.0;				//��]���ԁiX���j
				degreeTimerX = degreeTimeX;		//��]�^�C�}�[�iX���j
				stateCamera++;
				//�J�����̑��Έʒu���ꎞ�ۑ�
				tmpCameraQ = camera->relativeQuaternion;
			}

		}
		break;
	case CAMERA2:
		if (moveTimer > 0)
		{
			moveTimer -= frameTime;
			degreeTimer -= frameTime;
			rate = moveTimer / moveTime;

			rateY = degreeTimer / degreeTime;
			degreeY = UtilityFunction::lerp(0, 380, 1.0 - rateY);
			
			//D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-34.0f, 180.0f, -135.0f), 1.0f - rate);
			BezierPoint1 = Title::BezierCurve(startPos, D3DXVECTOR3(-1500, 800, -150), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);//�u��
			BezierPoint2 = Title::BezierCurve(startPos, D3DXVECTOR3(-4000, -2700, 0), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);//����
			D3DXVec3Lerp(&Curve1, &BezierPoint1, &BezierPoint2, 1.0f - rate);
			
			BezierPoint3 = Title::BezierCurve(startPos, D3DXVECTOR3(80, 300, 2000), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);//����
			D3DXVec3Lerp(&Curve2, &Curve1, &BezierPoint3, 1.0f - rate);
			
			BezierPoint4 = Title::BezierCurve(startPos, D3DXVECTOR3(200, 400, 2000), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);//���l��
			D3DXVec3Lerp(&Curve3, &Curve2, &BezierPoint4, 1.0f - rate);

			BezierPoint5 = Title::BezierCurve(startPos, D3DXVECTOR3(0, -300, 2300), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);
			D3DXVec3Lerp(&Curve4, &Curve3, &BezierPoint5, 1.0 - rate);

			BezierPoint6 = Title::BezierCurve(startPos, D3DXVECTOR3(1300, 200, -300), D3DXVECTOR3(-70.0f, 150.0f, -370), rate);
			D3DXVec3Lerp(&target->position, &Curve4, &BezierPoint6, 1.0 - rate);

			if (degreeTimer > 0)
			{
				//�O�̃J�����̑��Έʒu�ɕ␳����
				camera->relativeQuaternion = tmpCameraQ;
				camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
				if (moveTimer < 1.0f)
				{
					degreeTimerX -= frameTime;
					rateX = degreeTimerX / degreeTimeX;
					degreeX = UtilityFunction::lerp(0, 20, 1.0 - rateX);

					camera->rotation(fixedAxisX, degreeX);
				}
			}
			if (moveTimer <= 0)
			{
				target->position = D3DXVECTOR3(-70.0f, 150.0f, -370);
				startPos = target->position;
				moveTime = 5.0f;//�ҋ@
				moveTimer = moveTime;
				//degreeTimer = 6.0f;
				//degreeTime = degreeTimer;
				////�J�����̑��Έʒu���ꎞ�ۑ�
				//camera->rotation(D3DXVECTOR3(0, 1, 0), 55.0f);
				//camera->rotation(fixedAxisX, 45.0f);
				//tmpCameraQ = camera->relativeQuaternion;
				stateCamera++;


			}
		}
		break;
	case CAMERA3:
		if (moveTimer > 0)
		{
			moveTimer -= frameTime;
			if (moveTimer <= 0)
			{
				changeScene(nextScene);
			}


		}


		break;
	default:
		break;
	}



	//�J�����ړ�
	if (input->isKeyDown('W'))
	{
		fixedAxisZ *= 1.0f;
		target->position += fixedAxisZ;
	}
	if (input->isKeyDown('S'))
	{
		fixedAxisZ *= -1.0f;
		target->position += fixedAxisZ;
	}
	if (input->isKeyDown('A'))
	{
		cameraAxisX *= -1.0f;
		target->position += cameraAxisX;
	}
	if (input->isKeyDown('D'))
	{
		cameraAxisX *= 1.0f;
		target->position += cameraAxisX;
	}
	if (input->isKeyDown('Q'))
	{
		Y *= 1.0f;
		target->position += camera->upVector;
	}
	if (input->isKeyDown('E'))
	{
		Y *= 1.0f;
		target->position -= camera->upVector;

	}

	//�J������]
	//camera->rotation(D3DXVECTOR3(0, -1, 0), degree);
	//Y��
	if (input->isKeyDown(VK_RIGHT))
	{
		
		camera->rotation(camera->upVector, inputDegree);
		//target->quaternion.y += 5.0f;
	}
	if (input->isKeyDown(VK_LEFT))
	{
		camera->rotation(-camera->upVector, inputDegree);
		//target->quaternion.y -= 5.0f;
	}
	//X��
	if (input->isKeyDown(VK_UP))
	{
		camera->rotation(-fixedAxisX, inputDegree);
	}
	if (input->isKeyDown(VK_DOWN))
	{
		camera->rotation(fixedAxisX, inputDegree);
	}
	//�Y�[��
	if (input->isKeyDown('Z'))
	{
		camera->relativeQuaternion -= camera->relativeQuaternion * 0.05f;
	}
	if (input->isKeyDown('X'))
	{
		camera->relativeQuaternion += camera->relativeQuaternion * 0.05f;
	}



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

	//�t�B�i�[���t�B�[���h�i�e�X�g�j
	finaleFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), _currentCamera.view, _currentCamera.projection, _currentCamera.position);

	//�X�J�C�t�B�[���h�̕`��
	sky->render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);

	//�c���[�̕`��
	treeManager->render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);
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