//===================================================================================================================================
//�yDisplay.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/12/10
// [�X�V��]2019/12/10
//===================================================================================================================================

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//�I��ցF�܂��͂�����ǂ�Ł`�I
//�T�[�o�[		�F�����̃��j�^�[�����A�v���N�����f�B�X�v���C���[�h��I����display�V�[�����X�V�B
//�N���C�A���g	�F�Q�[���v���C�����A�v���N�����ɃQ�[�����[�h��I�����Asplash�V�[������J�n���A�T�[�o�[�֏��𑗐M��������B
//��L�ɂ��ċt�̕����ǂ��ꍇ�́A����ł������ł��B
//
//�y�ʐM�m�F�B���ڕW�z
//�@�ʐM�̊m��		�F�ʐM���ł����Ԃł��邱�Ƃ��m�F���邽�߂ɁA�V�[���J�ڂ��s���B
//�A�ʐM�̂���m�F	�F�ǂ�قǃ��O�����邩���F���邽�߂ɁA�^�C�}�[�̓����e�X�g���s���B
//�B�ʐM�ʂ̊m�F	�F�ǂꂭ�炢�̃f�[�^�𑗂�邩�e�X�g����B
//�C���ۂɖ؂𐶂₷�F�Q�[���̐i�s�ɍ��킹�āA���ۂɖ؂��͂₷�B
//
//�y��΂ɕK�v�ȋ@�\�z
//�@IP�A�h���X�̓��͋@�\
//�A�Q�[���v���C���́A��΂ɒʐM�󋵂̉e�����󂯂Ȃ�
//�@����j�ʐM���ł��Ă��Ȃ��Ă��Q�[���̓v���C�ł���B
//�@����j�ʐM���ؒf����Ă��Q�[���̓v���C�ł���B
//�@����j�ʐM���r������J�n����Ă��Q�[���̓v���C�ł���B�ȂǂȂ�
//�B�Q�[���v���C���ɐڑ����ł���悤�ɂ���B���A�v���P�[�V�����N���������łȂ��A�v���C�����ڑ��ł���悤�ɂ���B
//�C�ʐM�v���C�̃G���[���N�����ꍇ�ɁA�Q�[����W���Ȃ��B
//�D�A�v���P�[�V�����J�n���ɁA���[�h�I�����ł���B
//
//�v�����ŏ����Ă��ŁA��΂ł͂Ȃ����A�^��_����_������ꍇ�͕K�����k���Ă��������B
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Display.h"
#include "UtilityFunction.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace DisplayNS;


//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Display::Display()
{
	// ���̃V�[��( ���U���g )
	sceneName = ("Scene -Display-");

	// ���̃V�[��( �^�C�g�� )
	nextScene = SceneList::SPLASH;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Display::~Display()
{
}


//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Display::initialize()
{
	syncTimer = 0.0f;
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

	networkServer = new NETWORK_INTERFACE;
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Display::uninitialize(void)
{
	SAFE_DELETE(treeManager);
	SAFE_DELETE(camera);
	SAFE_DELETE(testFieldRenderer);
	SAFE_DELETE(testField);
	SAFE_DELETE(networkServer);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Display::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	PACKAGE *package = networkServer->updata();
	
	//�����^�C�}�[�F�Q�[���v���C�i�N���C�A���g�j���X�V
	if (package->networkTester == true)
	{
		syncTimer = package->timer;
	}

	if (package->num != 0)
	{
		//�p�b�P�[�W���̃C�x���g���Ăяo��
		for (int i = 0; i < package->num; i++)
		{
			//�C�x���g�Ώۂ̃c���[
			Tree* selectTree = NULL;
			//�C�x���g�Ώۂ̃c���[����������
			for (int num = 0; num < treeManager->getTreeList().size(); num++)
			{
				Tree* tree = treeManager->getTreeList()[i];
				if (tree->getTreeData()->treeID == package->treeTable[i].id)
				{
					selectTree = tree;
					num = treeManager->getTreeList().size();//�����I��
				}
			}
		
			//�C�x���g�̑Ώۃc���[�����݂��Ȃ��ꍇ�̓X���[
			if (selectTree == NULL)continue;

			//�C�x���g�ʂɃA�N�V��������
			switch (package->treeTable[i].eventType)
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
		//�p�b�P�[�W���̃C�x���g���������
		free(package->treeTable);
	}

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();			//�I�u�W�F�N�g
	testFieldRenderer->update();	//�����_���[


	//�c���[�}�l�[�W���[�̍X�V
	treeManager->update(frameTime);



	//if (input->wasKeyPressed(VK_RETURN) ||
	//	input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
	//	input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	
	//���
	//�ʐM�v���O������tarnsition��true�ɂ��ăV�[���J�ڂ��s���B
	bool transition = false;
	
	if(transition)
	{
		// �V�[���J��
		changeScene(nextScene);
	}

	//�J�����̍X�V
	{
		float rate = (sinf(sceneTimer * 6) / 2.0f) + 0.5f;
		float fov = UtilityFunction::lerp((D3DX_PI / 180) * 70, (D3DX_PI / 180) * 90, 0.5);
		camera->setFieldOfView(fov);
		camera->rotation(D3DXVECTOR3(0, 1, 0), CAMERA_SPEED*frameTime);
		camera->update();
	}
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Display::render()
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
	//renderUI();
}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
void Display::render3D(Camera currentCamera)
{
	//�e�X�g�t�B�[���h�̕`��
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	//�c���[�̕`��
	treeManager->render(currentCamera.view, currentCamera.projection, currentCamera.position);

}

//===================================================================================================================================
//�y2D[UI]�`��z
//===================================================================================================================================
//void Display::renderUI()
//{
//	// ���U���gUI
//	resultUI.render();
//}

//===================================================================================================================================
//�y�Փˏ����z
//===================================================================================================================================
void Display::collisions(void)
{
	// None
}

//===================================================================================================================================
//�yAI�����z
//===================================================================================================================================
void Display::AI(void)
{
	// None
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
#ifdef _DEBUG
void Display::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("syncTimer = %f", syncTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
#endif // _DEBUG
