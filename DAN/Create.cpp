//===================================================================================================================================
//【Create.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/20
// [更新日]2019/10/18
//===================================================================================================================================
#ifdef _DEBUG
//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Create.h"
#include "Game.h"
#include "networkClient.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace gameNS;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Create::Create()
{
	sceneName = "Scene -Create-";

	nextScene = SceneList::TITLE;

	//エネミーツール
	enemyTools = new ENEMY_TOOLS;
	//アイテムツール
	itemTools = new ITEM_TOOLS;
	//ツリーツール
	treeTools = new TREE_TOOLS;
	//マップオブジェクトツール
	mapObjTools = new MPOJ_TOOLS;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Create::~Create()
{
	//エネミーツール
	SAFE_DELETE(enemyTools);
	//アイテムツール
	SAFE_DELETE(itemTools);
	//ツリーツール
	SAFE_DELETE(treeTools);
	//マップオブジェクトツール
	SAFE_DELETE(mapObjTools);
}

//===================================================================================================================================
//【初期化】
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
	camera->setRelative(D3DXQUATERNION(0.0f, 60.0f, -15.5f, 0.0f));
	camera->setGaze(D3DXVECTOR3(0, 0, 0));
	camera->setGazeDistance(10.0f);
	camera->setRelativeGaze(CAMERA_RELATIVE_GAZE);
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI / 18) * 9);
	camera->setLimitRotationTop(0.3f);
	camera->setLimitRotationBottom(0.7f);
	
	//TopViewCamera
	topView = new Camera;
	topView->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	topView->setTarget(tmpObject->getPositionYeah());
	topView->setTargetX(&tmpObject->getAxisX()->direction);
	topView->setTargetY(&tmpObject->getAxisY()->direction);
	topView->setTargetZ(&tmpObject->getAxisZ()->direction);
	topView->setRelative(D3DXQUATERNION(1.0f, 100.0f, 0.0f, 0.0f));
	topView->setGazeDistance(100.0f);
	topView->setGaze(D3DXVECTOR3(0, 0, 0));
	topView->setRelativeGaze(CAMERA_RELATIVE_GAZE);
	topView->setUpVector(D3DXVECTOR3(0, 1, 0));
	topView->setFieldOfView((D3DX_PI / 18) * 9);
	onTopView = false;

	//light
	light = new Light;
	light->initialize();

	//テストフィールド
	testField = new Object();
	testFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL));
	testFieldRenderer->registerObject(testField);
	testField->initialize(&D3DXVECTOR3(0, 0, 0));

	//プレイヤーの初期化
	tmpObject->initialize(inputNS::DINPUT_1P, 0);
	tmpObject->setCamera(camera);	//カメラポインタのセット

	tmpObject->configurationGravityWithRay(testField->getPosition(), testFieldRenderer->getStaticMesh()->mesh, testField->getMatrixWorld());	//重力を設定
	tmpObjRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::WOLF));
	tmpObjRenderer->registerObject(tmpObject);

	//ツール
	ToolsListboxType = TOOLS_TYPE::TOOLS_NONE;
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void Create::uninitialize() {
	SAFE_DELETE(camera);
	SAFE_DELETE(topView);
	SAFE_DELETE(light);
	SAFE_DELETE(testFieldRenderer);

	SAFE_DELETE(tmpObjRenderer);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Create::update(float _frameTime) {

	sceneTimer += _frameTime;
	frameTime = _frameTime;

	//【処理落ち】
	//フレーム時間が約10FPS時の時の時間より長い場合は、処理落ち（更新しない）
	//※フレーム時間に準拠している処理が正常に機能しないため
	if (frameTime > 0.10)return;

	//テストフィールドの更新
	testField->update();
	testFieldRenderer->update();

	//ツールの更新
	enemyTools->update();
	itemTools->update();
	treeTools->update();
	mapObjTools->update();

	//プレイヤーの更新
	tmpObject->update(frameTime);
	LPD3DXMESH mesh = testFieldRenderer->getStaticMesh()->mesh;
	D3DXMATRIX matrix = testField->matrixWorld;

	if (input->wasKeyPressed('G'))
	{
		tmpObject->grounding(mesh, matrix);
	}


	//カメラの更新
	tmpObjRenderer->update();

	//カメラの更新
	camera->update();

	//俯瞰カメラの更新
	topView->update();

	// Enterまたは〇ボタンでリザルトへ
	if (input->wasKeyPressed(VK_RETURN) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A))
	{
		// シーン遷移
		changeScene(nextScene);
	}
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Create::render() {

	//カメラ・ウィンドウ
	direct3D9->changeViewportFullWindow();

	if (input->wasKeyPressed('Z'))
	{
		onTopView = !onTopView;
	}

	if (onTopView)
	{
		topView->renderReady();
		render3D(*topView);
	}
	else {
		camera->renderReady();
		render3D(*camera);
	}
	
	effekseerNS::setCameraMatrix(0,camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render(0);

	//UI
	direct3D9->changeViewportFullWindow();
	renderUI();
}

//===================================================================================================================================
//【3D描画】
//===================================================================================================================================
void Create::render3D(Camera currentCamera) {

	//テストフィールドの描画
	testField->setAlpha(0.1f);
	testFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);

	// プレイヤーの描画
	tmpObjRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), currentCamera.view, currentCamera.projection, currentCamera.position);
	// プレイヤーの他のオブジェクトの描画
	tmpObject->otherRender(currentCamera.view, currentCamera.projection, currentCamera.position);

	//ツールの描画
	enemyTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	itemTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	treeTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
	mapObjTools->render(currentCamera.view, currentCamera.projection, currentCamera.position);
}

//===================================================================================================================================
//【UI/2D描画】
//===================================================================================================================================
void Create::renderUI() {

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//===================================================================================================================================
//【衝突判定処理】
//===================================================================================================================================
void Create::collisions()
{
	
}

//===================================================================================================================================
//【AI処理】
//===================================================================================================================================
void Create::AI() {
}

//===================================================================================================================================
//【GUI作成処理】
//===================================================================================================================================
void Create::createGUI()
{
	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	testField->outputGUI();			//テストフィールド
	camera->outputGUI();			//カメラ
	tmpObject->outputGUI();			//プレイヤー
	toolsGUI();						//ツール
}

//===================================================================================================================================
//【ツールのGUI】
//===================================================================================================================================
void Create::toolsGUI()
{
	if (!ImGui::CollapsingHeader("Tools"))
	{
		collideGUI();	//当たり判定

		//ツールの種類
		const char* listboxEnemyType[] = { "NONE","ENEMY", "ITEM", "TREE" ,"MAP OBJECT" };
		ImGui::ListBox("Tools Type", &ToolsListboxType, listboxEnemyType, TOOLS_TYPE::TOOLS_MAX);

		enemyTools->outputEnemyToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);
		mapObjTools->outputMpojToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);
		itemTools->outputItemToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);
		treeTools->outputTreeToolsGUI(ToolsListboxType, *tmpObject->getPosition(), tmpObject->getAxisZ()->direction);

		int backupMeshId = meshId;
		switch (ToolsListboxType)
		{
		case TOOLS_TYPE::TOOLS_NONE:
			break;
		case TOOLS_TYPE::TOOLS_ENEMY:
			meshId = enemyTools->Model[enemyTools->EnemyListboxType];
			break;
		case TOOLS_TYPE::TOOLS_ITEM:
			meshId = itemTools->Model[itemTools->ItemListboxType];
			break;
		case TOOLS_TYPE::TOOLS_TREE:
			meshId = treeTools->Model[treeTools->TreeListboxType];
			break;
		case TOOLS_TYPE::TOOLS_MAP_OBJECT:
			meshId = mapObjTools->Model[mapObjTools->MpojListboxType];
			break;
		default:
			break;
		}
		if (backupMeshId != meshId)
		{
			tmpObjRenderer->setStaticMesh(staticMeshNS::reference(meshId));
			tmpObject->resetMesh(meshId);
		}
	}
}

//===================================================================================================================================
//【当たり判定のGUI】
//===================================================================================================================================
void Create::collideGUI()
{
	bool tmpButton = false;
	ImGui::Checkbox("Test Network", &tmpButton);

	//当たり判定
	ImGui::Text("Enemy:");
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		if (enemyTools->bodyCollide[i].collide(tmpObject->getBodyCollide()->getCenter(),
			tmpObject->getRadius(), *enemyTools->object[i]->getMatrixWorld(), *tmpObject->getMatrixWorld()))
		{
			ImGui::Text("%d", i);
			enemyTools->collideDraw(i, true);
			break;
		}
		else
		{
			enemyTools->collideDraw(i, false);
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
	ImGui::Text("Tree:");
	for (int i = 0; i < treeTools->GetTreeMax(); i++)
	{
		if (treeTools->bodyCollide[i].collide(tmpObject->getBodyCollide()->getCenter(),
			tmpObject->getRadius(), *treeTools->object[i]->getMatrixWorld(), *tmpObject->getMatrixWorld()))
		{
			ImGui::Text("%d", i);
		}
	}
	ImGui::Text("MapObj:");
	for (int i = 0; i < mapObjTools->GetMpojMax(); i++)
	{
		if (mapObjTools->bodyCollide[i].collide(tmpObject->getBodyCollide()->getCenter(),
			tmpObject->getRadius(), *mapObjTools->object[i]->getMatrixWorld(), *tmpObject->getMatrixWorld()))
		{
			ImGui::Text("%d", i);
		}
	}
}
#endif
