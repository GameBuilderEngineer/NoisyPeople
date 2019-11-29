//===================================================================================================================================
//�yCreate.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/18
//===================================================================================================================================
#ifdef _DEBUG
//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Create.h"
#include "Game.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace gameNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Create::Create()
{
	sceneName = "Scene -Create-";

	nextScene = SceneList::RESULT;

	//�G�l�~�[�c�[��
	enemyTools = new ENEMY_TOOLS;
	//�A�C�e���c�[��
	itemTools = new ITEM_TOOLS;
	//�c���[�c�[��
	treeTools = new TREE_TOOLS;

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Create::~Create()
{
	//�G�l�~�[�c�[��
	SAFE_DELETE(enemyTools);
	//�A�C�e���c�[��
	SAFE_DELETE(itemTools);
	//�c���[�c�[��
	SAFE_DELETE(treeTools);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Create::initialize() {

	//tmpObj
	tmpObject = new TmpObject;

	//camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setTarget(tmpObject->getPositionYeah());
	camera->setTargetX(&tmpObject->getAxisX()->direction);
	camera->setTargetY(&tmpObject->getAxisY()->direction);
	camera->setTargetZ(&tmpObject->getAxisZ()->direction);
	camera->setRelative(CAMERA_RELATIVE_QUATERNION);
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setRelativeGaze(CAMERA_RELATIVE_GAZE);
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI / 18) * 9);
	camera->setLimitRotationTop(0.3f);
	camera->setLimitRotationBottom(0.7f);

	//light
	light = new Light;
	light->initialize();

	//�e�X�g�t�B�[���h
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_V2));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//�v���C���[�̏�����
	tmpObject->initialize(inputNS::DINPUT_1P, 0);
	tmpObject->setCamera(camera);	//�J�����|�C���^�̃Z�b�g
	tmpObject->configurationGravityWithRay(testField->getPosition(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());	//�d�͂�ݒ�
	tmpObjRenderer = new StaticMeshRenderer *[tmpObjNS::TMPOBJ_LIST::TMPOBJ_MAX];
	for (int i = 0; i < tmpObjNS::TMPOBJ_LIST::TMPOBJ_MAX; i++)
	{
		int staticMeshNo = 0;
		switch (i)
		{
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_PLAYER:
			staticMeshNo = staticMeshNS::YAMADA_ROBOT2;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_WOLF:
			staticMeshNo = staticMeshNS::SAMPLE_REDBULL;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_TIGER:
			staticMeshNo = staticMeshNS::STAR_REGULAR_POLYHEDRON;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_BEAR:
			staticMeshNo = staticMeshNS::DICE;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_BATTERY:
			staticMeshNo = staticMeshNS::SAMPLE_SCISSORS;
			break;
		case tmpObjNS::TMPOBJ_LIST::TMPOBJ_EXAMPLE:
			staticMeshNo = staticMeshNS::YAMADA_ROBOT2;
			break;
		default:
			break;
		}
		tmpObjRenderer[i] = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNo));
		tmpObjRenderer[i]->registerObject(tmpObject);
	}

	//�͖؂̏�����
	deadTree = new DeadTree();
	//��A�̏�����
	treeA = new TreeTypeA();
	//��B�̏�����
	treeB = new TreeTypeB();
	//�΂̏�����
	stone = new Stone();

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect = new TestEffect();

	//�c�[��
	ToolsListboxType = TOOLS_TYPE::TOOLS_NONE;
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void Create::uninitialize() {
	SAFE_DELETE(camera);
	SAFE_DELETE(light);
	SAFE_DELETE(testFieldRenderer);

	//�����҂�
	for (int i = 0; i < tmpObjNS::TMPOBJ_LIST::TMPOBJ_MAX; i++)
	{
		SAFE_DELETE(tmpObjRenderer[i]);
	}
	ES_SAFE_DELETE_ARRAY(tmpObjRenderer);

	SAFE_DELETE(deadTree);
	SAFE_DELETE(treeA);
	SAFE_DELETE(treeB);
	SAFE_DELETE(stone);
	SAFE_DELETE(testEffect);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Create::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//�y���������z
	//�t���[�����Ԃ���10FPS���̎��̎��Ԃ�蒷���ꍇ�́A���������i�X�V���Ȃ��j
	//���t���[�����Ԃɏ������Ă��鏈��������ɋ@�\���Ȃ�����
	if (frameTime > 0.10)return;

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect->update(frameTime);

	//�e�X�g�t�B�[���h�̍X�V
	testField->update();
	testFieldRenderer->update();

	//�c�[���̍X�V
	enemyTools->update();
	itemTools->update();
	treeTools->update();

	//�v���C���[�̍X�V
	tmpObject->update(frameTime);

	// �A�C�e���̍X�V
	if (input->wasKeyPressed('0'))
	{
		itemNS::ItemData unko = { 0, itemNS::BATTERY, *tmpObject->getPosition() };
	}
	if (input->wasKeyPressed('9'))
	{
	}

	////�J�����̍X�V
	//camera->update();
	for (int i = 0; i < tmpObjNS::TMPOBJ_LIST::TMPOBJ_MAX; i++)
	{
		tmpObjRenderer[i]->update();
	}

	//�͖؂̍X�V
	deadTree->update();
	//��A�̍X�V
	treeA->update();
	//��B�̍X�V
	treeB->update();
	//�΂̍X�V
	stone->update();

	//�J�����̍X�V
	camera->update();

	// Enter�܂��́Z�{�^���Ń��U���g��
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// �V�[���J��
		changeScene(nextScene);
	}

}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Create::render() {

	//�J�����E�E�B���h�E
	camera->renderReady();
	direct3D9->changeViewportFullWindow();
	render3D(*camera);
	effekseerNS::setCameraMatrix(camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render();

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();

}

//===================================================================================================================================
//�y3D�`��z
//===================================================================================================================================
void Create::render3D(Camera currentCamera) {

	//�e�X�g�t�B�[���h�̕`��
	testField->setAlpha(0.1f);
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	//�G�t�F�N�g�i�C���X�^���V���O�j�e�X�g
	testEffect->render(currentCamera.view, currentCamera.projection, currentCamera.position);

	// �v���C���[�̕`��
	tmpObjRenderer[getBufferID(meshId)]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// �v���C���[�̑��̃I�u�W�F�N�g�̕`��
	tmpObject->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//�͖؂̕`��
	deadTree->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//��A�̕`��
	treeA->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//��B�̕`��
	treeB->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//�΂̕`��
	stone->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	//�c�[���̕`��
	enemyTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	itemTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	treeTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
}

//===================================================================================================================================
//�yUI/2D�`��z
//===================================================================================================================================
void Create::renderUI() {

	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);						// ���u�����h���s��
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);				// ���\�[�X�J���[�̎w��
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);			// ���f�X�e�B�l�[�V�����J���[�̎w��

	//device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//===================================================================================================================================
//�y�Փ˔��菈���z
//===================================================================================================================================
void Create::collisions()
{
	
}

//===================================================================================================================================
//�yAI�����z
//===================================================================================================================================
void Create::AI() {
}

//===================================================================================================================================
//�yGUI�쐬�����z
//===================================================================================================================================
void Create::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("node:%d", testEffect->getList().nodeNum);
	testField->outputGUI();			//�e�X�g�t�B�[���h
	camera->outputGUI();			//�J����
	tmpObject->outputGUI();			//�v���C���[
	toolsGUI();						//�c�[��
}

//===================================================================================================================================
//�y�c�[����GUI�z
//===================================================================================================================================
void Create::toolsGUI()
{
	if (!ImGui::CollapsingHeader("Tools"))
	{
		collideGUI();	//�����蔻��

		//�c�[���̎��
		const char* listboxEnemyType[] = { "NONE","ENEMY", "ITEM", "TREE" ,"MAP OBJECT" };
		ImGui::ListBox("Tools Type", &ToolsListboxType, listboxEnemyType, TOOLS_TYPE::TOOLS_MAX);

		enemyTools->outputEnemyToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);
		itemTools->outputItemToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);
		treeTools->outputTreeToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);

		int backupMeshId = meshId;
		switch (ToolsListboxType)
		{
		case TOOLS_TYPE::TOOLS_NONE:
			break;
		case TOOLS_TYPE::TOOLS_ENEMY:
			meshId = enemyTools->GetStaticMeshID(enemyTools->EnemyListboxType);
			break;
		case TOOLS_TYPE::TOOLS_ITEM:
			meshId = itemTools->GetStaticMeshID(itemTools->ItemListboxType);
			break;
		case TOOLS_TYPE::TOOLS_TREE:
			break;
		case TOOLS_TYPE::TOOLS_MAP_OBJECT:
			break;
		default:
			break;
		}
		if (backupMeshId != meshId)
		{
			tmpObject->resetMesh(meshId);
		}
	}
}

//===================================================================================================================================
//�y�����蔻���GUI�z
//===================================================================================================================================
void Create::collideGUI()
{
	//�����蔻��
	ImGui::Text("Enemy:");
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		if (enemyTools->bodyCollide[i].collide(tmpObject->getBodyCollide()->getCenter(),
			tmpObject->getRadius(), *enemyTools->object[i]->getMatrixWorld(), *tmpObject->getMatrixWorld()))
		{
			ImGui::Text("%d", i);
		}
	}
	ImGui::Text("Item:");
	for (int i = 0; i < itemTools->GetItemMax(); i++)
	{
		if (itemTools->bodyCollide[i].collide(tmpObject->getBodyCollide()->getCenter(),
			tmpObject->getRadius(), *itemTools->object[i]->getMatrixWorld(), *tmpObject->getMatrixWorld()))
		{
			ImGui::Text("%d", i);
		}
	}
}

//===================================================================================================================================
//�y�o�b�t�@ID���擾 meshID->bufferID�z
//===================================================================================================================================
int Create::getBufferID(int meshID)
{
	switch (meshID)
	{
	case staticMeshNS::YAMADA_ROBOT2:
		return  tmpObjNS::TMPOBJ_LIST::TMPOBJ_EXAMPLE;
		break;
	case staticMeshNS::SAMPLE_REDBULL:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_WOLF;
		break;
	case staticMeshNS::STAR_REGULAR_POLYHEDRON:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_TIGER;
		break;
	case staticMeshNS::DICE:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_BEAR;
		break;
	case staticMeshNS::SAMPLE_SCISSORS:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_BATTERY;
		break;
	//case staticMeshNS::YAMADA_ROBOT2:
	//	return tmpObjNS::TMPOBJ_LIST::TMPOBJ_EXAMPLE;
	//	break;
	default:
		return tmpObjNS::TMPOBJ_LIST::TMPOBJ_PLAYER;
		break;
	}

	return tmpObjNS::TMPOBJ_LIST::TMPOBJ_PLAYER;
}
#endif
