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
	PLAY_PARAMETERS playParameters[2];//�����ɍĐ���������
	memset(playParameters, 0, sizeof(playParameters));//
	//�Đ�����Ȃ̎w��T�E���hID,���[�v,�X�s�[�hNULL�ł��Ȃ�,��{false,��{NULL,�t�B���^�[���g�����g��Ȃ���
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Cancel, false,1.0f,false,NULL };//SE�̐ݒ�
	playParameters[1]= { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Title, true,1.0f,false,NULL };//BGM�̐ݒ�
	//�Đ�
	SoundInterface::BGM->playSound(&playParameters[1]);
	

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
	
	//�^�[�Q�b�g�I�u�W�F�N�g
	target = new Object;
	target->initialize(&D3DXVECTOR3(-34.0f, 160.0f, 20));		//�^�[�Q�b�g�̏����ʒu�ݒ�
	

	//
	

	//�����t�H�g�O���t
	stateCamera = CAMERA0;

	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//camera->setGaze(D3DXVECTOR3(0, 100, 0));
	//camera->setRelative(D3DXQUATERNION(0.0f, 20.0f, -40.0f, 0.0f));	//�����̒l��
	camera->setRelative(D3DXQUATERNION(0.0f, 0.0f, 20.0f, 0.0f));   //���^�[�Q�b�g�̏����ʒu�ɑ������`�ɂȂ��Ă���
	camera->setTarget(&target->position);

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

	//�X�J�C�h�[��
	sky = new Sky();

	//WaveBall
	waveBall = new WaveBall;

	// Light
	light = new Light;
	light->initialize();

	//�^�C�g��UI�̏�����
	titleUI.initialize();

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect = new TestEffect();
	
	//�^�C�g���t�B�[���h�i�e�X�g�j
	titleField = new Object();
	titleFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	titleFieldRenderer->registerObject(titleField);
	titleField->initialize(&D3DXVECTOR3(0, 0, 0));

	 cameraAxisZ = D3DXVECTOR3(0, 0, 0);
	 fixedAxisZ = D3DXVECTOR3(0, 0, 0);
	 cameraAxisX = D3DXVECTOR3(0, 0, 0);
	 cameraAxisY = D3DXVECTOR3(0, 0, 0);
	 fixedAxisX = D3DXVECTOR3(0, 0, 0);
	

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

	//�^�C�g���t�B�[���h
	SAFE_DELETE(titleField);
	SAFE_DELETE(titleFieldRenderer);

	//�^�[�Q�b�g�I�u�W�F�N�g
	SAFE_DELETE(target);
	
	//�X�J�C�t�B�[���h
	SAFE_DELETE(sky);

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
	
	//�^�C�g���t�B�[���h�i�e�X�g�j
	titleField->update();	//�I�u�W�F�N�g
	titleFieldRenderer->update();

	//�X�J�C�t�B�[���h�̍X�V
	sky->update();

	target->update();

	// �J����
	//camera[0].setUpVector(player[PLAYER_TYPE::PLAYER_1].getAxisY()->direction);
	//camera[0].update();

	//player[PLAYER_TYPE::PLAYER_1].animationPlayer.updateTitle();

	//�o�[�̈ړ�
	//if(input->wasKeyPressed()


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
	
	D3DXQUATERNION cameraQ;

	switch (stateCamera)
	{
	case CAMERA0:
		if (sceneTimer > 0.0f)
		{
			startPos = target->position;	//���[�v�n�_
			moveTime = 6.0f;				//�I�_�܂ł̎���
			moveTimer = moveTime;			//�ړ��^�C�}�[
			moveTime2 = 3.0f;
			moveTimer2 = moveTime2;
			stateCamera++;
		}
		break;
	case CAMERA1:
		if (moveTimer > 0)
		{
			moveTimer -= frameTime;
			/*D3DXVec3Lerp(&P0_1, &startPos, &D3DXVECTOR3(-165.0f, 75.0f, -122.0), 1.0f - moveTimer / moveTime);
			D3DXVec3Lerp(&P1_2, &D3DXVECTOR3(-165.0f, 75.0f, -122.0), &D3DXVECTOR3(-34.0f, 160.0f, -135.0f), 1.0f - moveTimer / moveTime);
			D3DXVec3Lerp(&target->position, &P0_1, &P1_2, 1.0f - moveTimer / moveTime);*/
			D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-34.0f, 160.0f, -135.0f), 1.0f - moveTimer / moveTime);
			
			if (target->position.z < -135.0f)
			{
				target->position.z = -135.0f;
				startPos = target->position;
				moveTime = 3.0f;
				moveTimer = moveTime;
				degreeTimer = 2.0f;
				stateCamera++;
			}
		}
		break;
	case CAMERA2:
		if (moveTimer > 0)
		{
			moveTimer -= frameTime;
			degreeTimer -= frameTime;
			D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-65.0f, 75.0f, -122.0), 1.0f - moveTimer / moveTime);
			rateY = degreeTimer / 2.0f;
			rateX = degreeTimer / 2.0f;
			degreeY = UtilityFunction::lerp(0, D3DXToRadian(100.0f), 1.0f - rateY);
			degreeX = UtilityFunction::lerp(0, D3DXToRadian(45.0f), 1.0f - rateX);
			if (degreeTimer > 0.0f)
			{
				camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
				camera->rotation(fixedAxisX, degreeX);
			}
			if (moveTimer <= 0)
			{
				target->position = D3DXVECTOR3(-65.0f, 75.0f, -122.0);
				startPos = target->position;
				moveTime = 3.0f;
				moveTimer = moveTime;
				degreeTimer = 3.0f;
				stateCamera++;
			}

		}
		break;
		
	case CAMERA3:
		if (moveTime > 0.0f)
		{
			moveTimer -= frameTime;
			rate = moveTimer / moveTime;
			degreeTimer -= frameTime;

			target->position = BezierCurve(startPos, D3DXVECTOR3(-200.0f, 63.0f, -176.0), D3DXVECTOR3(-185.0f, 40.0f, 45.0f), rate);
			
			rateY = degreeTimer / 3.0f;
			rateX = degreeTimer / 3.0f;
			degreeY = UtilityFunction::lerp(0, D3DXToRadian(45.0f), 1.0f - rateY);
			degreeX = UtilityFunction::lerp(0, D3DXToRadian(-45.0f), 1.0f - rateX);
			if (degreeTimer > 0)
			{
				camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
				camera->rotation(fixedAxisX, degreeX);
			}
			//D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-150.0f, 63.0f, -76.0), 1.0f - moveTimer / moveTime);

			/*D3DXVec3Lerp(&P0_1, &startPos, &D3DXVECTOR3(-150.0f, 63.0f, -76.0), 1.0f - moveTimer / moveTime);
			D3DXVec3Lerp(&P1_2, &D3DXVECTOR3(-150.0f, 63.0f, -76.0), &D3DXVECTOR3(-185.0f, 40.0f, 45.0f), 1.0f - moveTimer / moveTime);
			D3DXVec3Lerp(&target->position, &P0_1, &P1_2, 1.0f - moveTimer / moveTime);*/
		}
		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(-300.0f, 60.0f, 37.0f);
			startPos = target->position;
			moveTime = 12.0f;
			moveTimer = moveTime;
			degreeTimer = 6.0f;
			degreeTime = degreeTimer;

			stateCamera++;
		}

		break; 
	case CAMERA4:
		moveTimer -= frameTime;
		degreeTimer -= frameTime;
		rate = moveTimer / moveTime;
		//D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-290.0f, 156.0f, 265.0f), 1.0f - moveTimer / moveTime);
		//target->position = BezierCurve(startPos, D3DXVECTOR3(-500.0f, 96.0f, 228.0f), D3DXVECTOR3(-290.0f, 156.0f, 265.0f), rate);
	
		rateY = degreeTimer / 6.0f;
		rateX = degreeTimer / 6.0f;
		degreeY = UtilityFunction::lerp(0, D3DXToRadian(100.0f), 1.0f - rateY);
		degreeX = UtilityFunction::lerp(0, D3DXToRadian(45.0f), 1.0f - rateX);
		if (degreeTimer > 0)
		{
			camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
			camera->rotation(fixedAxisX, degreeX);
		}
		if (moveTimer < 0)
		{
			/*target->position = D3DXVECTOR3(0.0f, 50.0f, 0.0);
			startPos = target->position;*/
			stateCamera++;
		}
		break;

	case CAMERA5:

		//camera->rotation(camera->upVector, 0.3f);
		break;
		
	default:
		break;
	}


	//Base::anyAxisRotationSlerp(&cameraQ,D3DXVECTOR3(13.2f, 6.0f, -13.0f),);


	

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
	effekseerNS::setCameraMatrix(0,camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render(0);

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
	
	//�^�C�g���t�B�[���h�i�e�X�g�j
	titleFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), _currentCamera.view, _currentCamera.projection, _currentCamera.position);

	//�X�J�C�t�B�[���h�̕`��
	sky->render(_currentCamera.view, _currentCamera.projection, _currentCamera.position);

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

D3DXVECTOR3 Title::BezierCurve(D3DXVECTOR3 startpoint, D3DXVECTOR3 curvepoint, D3DXVECTOR3 endpoint, float rate)
{
	D3DXVECTOR3 P0_1;
	D3DXVECTOR3 P1_2;
	D3DXVECTOR3 result;
	
	D3DXVec3Lerp(&P0_1, &startpoint, &curvepoint, 1.0f - rate);
	D3DXVec3Lerp(&P1_2, &curvepoint, &endpoint, 1.0f - rate);
	D3DXVec3Lerp(&result, &P0_1, &P1_2, 1.0f - rate);

	return result;
}


//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Title::createGUI()
{
	bool createScene = false;
	float backUpTmpVolume = tmpVolume;
	float limitTop = 1000;
	float limitBottom = -1000;
	camera->outputGUI();
	target->outputGUI();

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("node:%d", testEffect->getList().nodeNum);
	ImGui::Checkbox("Create Scene", &createScene);
	ImGui::SliderFloat("volume control", &tmpVolume, 0.0f, 1.0f);
	ImGui::Text("CameraState %d", stateCamera);

	if (backUpTmpVolume != tmpVolume)
	{
		backUpTmpVolume = tmpVolume;
		waveBall->setVolume(tmpVolume);
	}

	ImGui::Text("controller1 LStick(%.02f,%.02f)", 
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().x,
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().y);
	ImGui::Text("controller1 LStickTrigger(%.02f,%.02f)", 
		input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().x,
		input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().y);

	//�c�[���p�V�[��
	if (createScene)
	{
		selectStateMemory = titleUiNS::CREATE;
		nextScene = (SceneList::CREATE);
		changeScene(nextScene);
	}
}
#endif // _DEBUG