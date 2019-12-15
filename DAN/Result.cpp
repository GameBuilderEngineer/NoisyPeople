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
	nextScene = SceneList::FINALE;

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
	//���U���gUI�̏�����
	resultUI.initialize();
	
	resultUI.greenigPersent = 0;//�S�̗Ή���
	resultUI.greeningNum01 =0;//player1�̗Ή��{��
	resultUI.greeningNum02 = 0;//player2�̗Ή��{��
	resultUI.defeat01 = 0;//player1�̌��j��
	resultUI.defeat02 = 0;//player2�̌��j��

	//�e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
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

	// �c���[
	treeManager = new TreeManager;
	treeManager->initialize(testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());
	treeManager->createUsingTool();
	treeManager->switchingNormalView(gameMasterNS::PLAYER_1P);
	
	//���v���C�^�C�}�[
	playbackTimer = 0.0f;

}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Result::uninitialize(void)
{
	SAFE_DELETE(treeManager);
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

	//���v���C�^�C�}�[
	playbackTimer += frameTime*PLAYBACK_SPEED;

	//UI�̍X�V����
	resultUI.update(frameTime);

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[

	//�Q�[���}�X�^�[����C�x���g���擾���A�Đ�
	if (gameMaster->getEventList(NULL,playbackTimer) > 0)
	{
		TreeTable* eventList = NULL;	//�C�x���g���X�g
		int eventNum;					//�C�x���g��

		//�C�x���g���擾
		eventNum = gameMaster->getEventList(&eventList,playbackTimer);

		//�擾�����C�x���g�����s����
		for (int num = 0; num < eventNum; num++)
		{
			//�C�x���g�Ώۂ̃c���[
			Tree* selectTree = NULL;
			
			//�C�x���g�Ώۂ̃c���[��I��
			for (int i = 0; i < treeManager->getTreeList().size(); i++)
			{
				if (selectTree)continue;//�I���ς݂Ȃ�X���[
				Tree* tree = treeManager->getTreeList()[i];
				//�C�x���g�Ώۃc���[��ID�ɂ�茟��
				if (tree->getTreeData()->treeID == eventList[num].id)
				{
					selectTree = tree;
					i = treeManager->getTreeList().size();//�����I��
				}
			}
			
			//�C�x���g�̑Ώۃc���[�����݂��Ȃ��ꍇ�̓X���[
			if (selectTree == NULL)continue;

			//�C�x���g�ʂɃA�N�V����������
			switch (eventList[num].eventType)
			{
			case gameMasterNS::TO_DEAD:
				selectTree->transState();
				break;
			case gameMasterNS::TO_GREEN_WITH_ANALOG:
				selectTree->transState();
				break;
			case gameMasterNS::TO_GREEN_WITH_DIGITAL:
				selectTree->transState();
				break;
			}

		}

		//�擾�����C�x���g���X�g���폜
		SAFE_DELETE_ARRAY(eventList);
	}
	//�c���[�}�l�[�W���[�̍X�V
	treeManager->update(frameTime);



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
		float fov = UtilityFunction::lerp((D3DX_PI / 180) * 70, (D3DX_PI / 180) * 90, 0.5);
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

	//�c���[�̕`��
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

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
	ImGui::Text("playbackTimer = %f", playbackTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
