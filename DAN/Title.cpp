//===================================================================================================================================
//【Title.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/10/17
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Title.h"
#include "SerialCommunication.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace titleNS;

//============================================================================================================================================
//【グローバル変数】
//============================================================================================================================================

//============================================================================================================================================
//【コンストラクタ】
//============================================================================================================================================
Title::Title(void)
{
	// 今のシーン( タイトル )
	sceneName = ("Scene -Title-");
	nextScene = SceneList::TUTORIAL;

	//再生パラメータ
	PLAY_PARAMETERS playParameters[2];//同時に再生したい数
	memset(playParameters, 0, sizeof(playParameters));//
	//再生する曲の指定サウンドID,ループ,スピードNULLでしない,基本false,基本NULL,フィルターを使うか使わないか
	playParameters[0] = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Cancel, false,1.0f,false,NULL };//SEの設定
	playParameters[1]= { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Title, true,1.0f,false,NULL };//BGMの設定
	//再生
	SoundInterface::BGM->playSound(&playParameters[1]);
	

	//初期化
	tmpVolume = 1.0f;

	//ネットワークの初期化
	networkClient = new NETWORK_CLIENT;
	networkClient->resetDisplay();//ディスプレイシーンをリセットする
	networkClient->send(0.0f);
	

	//ライトOFF
	SerialCommunicationNS::send(SerialCommunicationNS::OFF);
}

//============================================================================================================================================
//【デストラクタ】
//============================================================================================================================================
Title::~Title(void)
{
	//ネットワーククライアントの削除
	SAFE_DELETE(networkClient);
	// サウンドの停止
	SoundInterface::BGM->uninitSoundStop();
}

//============================================================================================================================================
//【初期化】
//============================================================================================================================================
void Title::initialize()
{
	
	//ターゲットオブジェクト
	target = new Object;
	target->initialize(&D3DXVECTOR3(-34.0f, 180.0f, 100));		//ターゲットの初期位置設定
	
	//初期フォトグラフ
	stateCamera = CAMERA0;

	// Camera
	camera = new Camera;
	camera->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera->setRelative(D3DXQUATERNION(0.0f, 0.0f, 1.0f, 0.0f));   //※ターゲットの初期位置に足される形になっている
	camera->setTarget(&target->position);
	camera->setGazeDistance(20.0f);
	camera->setUpVector(D3DXVECTOR3(0, 1, 0));
	camera->setFieldOfView((D3DX_PI) / 18 * 10);
	camera->setViewProjection();	

	//エフェクシアーの設定
	effekseerNS::setProjectionMatrix(0,
		camera->fieldOfView,
		camera->windowWidth,
		camera->windowHeight,
		camera->nearZ,
		camera->farZ);

	//スカイドーム
	sky = new Sky();
	//海面の初期化
	ocean = new Ocean();

	// Light
	light = new Light;
	light->initialize();

	//タイトルUIの初期化
	titleUI.initialize();
	
	//タイトルフィールド（テスト）
	titleField = new Object();
	titleFieldRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::DATE_ISLAND_FINAL_FACE));
	titleFieldRenderer->registerObject(titleField);
	titleField->initialize(&D3DXVECTOR3(0, 0, 0));

	cameraAxisZ = D3DXVECTOR3(0, 0, 0);
	fixedAxisZ = D3DXVECTOR3(0, 0, 0);
	cameraAxisX = D3DXVECTOR3(0, 0, 0);
	cameraAxisY = D3DXVECTOR3(0, 0, 0);
	fixedAxisX = D3DXVECTOR3(0, 0, 0);
	
	 // ツリーの初期化
	 treeManager = new TreeManager();
	 treeManager->initialize(titleFieldRenderer->getStaticMesh()->mesh, titleField->getMatrixWorld());
	 // ツリーをツール情報を元に設置する
	 treeManager->createUsingTool();
	 // 非ヴィジョンの描画
	 treeManager->switchingNormalView(0);
	 for (int i = 0; i < treeManager->getTreeList().size(); i++)
	 {
 		treeManager->getTreeList()[i]->transState();
	 }
	 titleState = TITLE01;//タイトルの状態を０で初期化
}

//============================================================================================================================================
//【終了処理】
//============================================================================================================================================
void Title::uninitialize(void)
{
	// ライト
	SAFE_DELETE(light);

	// カメラ
	SAFE_DELETE(camera);

	//タイトルフィールド
	SAFE_DELETE(titleField);
	SAFE_DELETE(titleFieldRenderer);

	//ターゲットオブジェクト
	SAFE_DELETE(target);
	
	//スカイフィールド
	SAFE_DELETE(sky);
	SAFE_DELETE(ocean);

	// タイトルUI
	titleUI.uninitialize();

	// ツリーマネージャー
	SAFE_DELETE(treeManager);
}

//============================================================================================================================================
//【更新】
//============================================================================================================================================
void Title::update(float _frameTime)
{
	sceneTimer += _frameTime;
	frameTime = _frameTime;

	if (frameTime > 10.0f / 60.0f)return;

	//スカイフィールドの更新
	sky->update();
	//海面の更新
	ocean->update();
	//タイトルフィールド（テスト）
	titleField->update();	//オブジェクト
	titleFieldRenderer->update();


	target->update();

	//ミュート
	if (input->isKeyDown('M'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(0.0f);
		SoundInterface::BGM->setEndPointVoiceVolume(0.0f);
		SoundInterface::S3D->setEndPointVoiceVolume(0.0f);
	}
	//ミュート解除
	if (input->isKeyDown('N'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(1.0f);
		SoundInterface::BGM->setEndPointVoiceVolume(1.0f);
		SoundInterface::S3D->setEndPointVoiceVolume(1.0f);
	}
	//10%(test)
	if (input->isKeyDown('B'))
	{
		SoundInterface::SE->setEndPointVoiceVolume(0.1f);
		SoundInterface::BGM->setEndPointVoiceVolume(0.1f);
		SoundInterface::S3D->setEndPointVoiceVolume(0.1f);
	}

	// タイトルUI
	titleUI.update(input,frameTime);

	//プレゼンテーションモードの切替
	if (input->getController()[inputNS::DINPUT_1P]->isButton(virtualControllerNS::HOME) ||
		input->getController()[inputNS::DINPUT_2P]->isButton(virtualControllerNS::HOME) )
	{
		setModeFlag(MODE_PRESENTATION, true);
	}
	else {
		setModeFlag(MODE_PRESENTATION, false);
	}

	//タイトルシーンの更新
	if (input->wasKeyPressed(VK_RETURN) ||
		input->wasKeyPressed(VK_SPACE) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::A) ||
		input->getController()[inputNS::DINPUT_1P]->wasButton(virtualControllerNS::SPECIAL_MAIN) ||
		input->getController()[inputNS::DINPUT_2P]->wasButton(virtualControllerNS::SPECIAL_MAIN)
		)
	{
		PLAY_PARAMETERS playDesicion = { 0 };
		switch (titleState)
		{
		case TITLE01:
			playDesicion = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Decision, false ,NULL,false,NULL };
			SoundInterface::SE->playSound(&playDesicion);
			break;
		case TITLE02:
			//シーン遷移
			updateInput();
			changeScene(nextScene);
			break;
		}
		titleState++;
	}



	// ツリーの更新
	treeManager->update(frameTime);
	
	//注視オブジェクトとカメラの二点間ベクトル（カメラZ軸ベクトル）
	cameraAxisZ = camera->getAxisZ();
	fixedAxisZ = Base::slip(cameraAxisZ, camera->upVector);	//カメラの傾きに対応
	//カメラX軸ベクトル
	D3DXVec3Cross(&cameraAxisX, &camera->upVector, &cameraAxisZ);
	//カメラY軸ベクトル（上方向の更新）
	D3DXVECTOR3 Y = D3DXVECTOR3(0, 0, 0);
	D3DXVec3Cross(&Y, &fixedAxisZ, &cameraAxisX);
	D3DXVec3Cross(&cameraAxisY, &cameraAxisZ, &cameraAxisX);
	D3DXVec3Cross(&fixedAxisX, &cameraAxisY, &cameraAxisZ);


	switch (stateCamera)
	{
	case CAMERA0:
		if (sceneTimer > 0.0f)
		{
			startPos = target->position;	//ラープ始点
			moveTime = 10.0f;				//終点までの時間
			moveTimer = moveTime;			//移動タイマー
			stateCamera++;
			//カメラの相対位置を一時保存
			tmpCameraQ = camera->relativeQuaternion;
		}
		break;
	case CAMERA1:
		if (moveTimer > 0)
		{
			moveTimer -= frameTime;
			rate = moveTimer / moveTime;

			D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(-34.0f, 180.0f, -135.0f), 1.0f - rate);
			
			//前のカメラの相対位置に補正する
			camera->relativeQuaternion = tmpCameraQ;
			if (moveTimer <= 0)
			{
				target->position = D3DXVECTOR3(-65.0f, 75.0f, -122.0);
				startPos = target->position;
				moveTime = 6.0f;
				moveTimer = moveTime;
				degreeTimer = 6.0f;
				degreeTime = degreeTimer;
				stateCamera++;
				//カメラの相対位置を一時保存
				camera->rotation(D3DXVECTOR3(0, 1, 0), 55.0f);
				camera->rotation(fixedAxisX, 45.0f);
				tmpCameraQ = camera->relativeQuaternion;	
				
				
			}
		}
		break;
	case CAMERA2:
		if (moveTime > 0.0f)
		{
			moveTimer -= frameTime;
			rate = moveTimer / moveTime;
			degreeTimer -= frameTime;

			target->position = BezierCurve(startPos, D3DXVECTOR3(-250.0f, 63.0f, -200.0), D3DXVECTOR3(-185.0f, 40.0f, 80.0f), rate);

			rateY = degreeTimer / degreeTime;
			rateX = degreeTimer / degreeTime;
			degreeY = UtilityFunction::lerp(0, 150.0f, 1.0f - rateY);
			degreeX = UtilityFunction::lerp(0, -45.0f, 1.0f - rateX);
			if (degreeTimer > 0)
			{
				//前のカメラの相対位置に補正する
				camera->relativeQuaternion = tmpCameraQ;
				camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
				camera->rotation(fixedAxisX, degreeX);
			}
			
		}
		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(-305.0f, 75.0f, 45.0f);
			startPos = target->position;
			moveTime = 6.0f;
			moveTimer = moveTime;
			degreeTimer = 6.0f;
			degreeTime = degreeTimer;
			stateCamera++;
			//次ステート用に角度調整
			camera->rotation(D3DXVECTOR3(0, 1, 0), -15.0f);
			camera->rotation(fixedAxisX, 35.0f);
			//カメラの相対位置を一時保存
			tmpCameraQ = camera->relativeQuaternion;
		}
		break;
		
	
	case CAMERA3:
		moveTimer -= frameTime;
		degreeTimer -= frameTime;
		rate = moveTimer / moveTime;
		
		target->position = BezierCurve(startPos, D3DXVECTOR3(-430.0f, 96.0f, 200.0f), D3DXVECTOR3(-290.0f, 156.0f, 265.0f), rate);
	
		rateY = degreeTimer / degreeTime;
		rateX = degreeTimer / degreeTime;
		degreeY = UtilityFunction::lerp(0, 140.0f, 1.0f - rateY);
		degreeX = UtilityFunction::lerp(0, 20.0f, 1.0f - rateX);
		if (degreeTimer > 0)
		{
			//前のカメラの相対位置に補正する
			camera->relativeQuaternion = tmpCameraQ;
			camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
			camera->rotation(fixedAxisX, degreeX);
		}
		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(-65.0f, 75.0f, 225.0f);
			startPos = target->position;
			moveTime = 6.0f;
			moveTimer = moveTime;
			degreeTimer = 6.0f;
			degreeTime = degreeTimer;
			stateCamera++;
			//次ステート用に角度調整
			camera->rotation(fixedAxisX, -25.0f);
			//カメラの相対位置を一時保存
			tmpCameraQ = camera->relativeQuaternion;
		}
		break;

	case CAMERA4:
		moveTimer -= frameTime;
		degreeTimer -= frameTime;
		rate = moveTimer / moveTime;
		
		target->position = BezierCurve(startPos, D3DXVECTOR3(150.0f, 75.0f, 350.0f), D3DXVECTOR3(290.0f, 75.0f, 150.0f), rate);
		
		rateY = degreeTimer / degreeTime;
		degreeY = UtilityFunction::lerp(0, 120.0f, 1.0f - rateY);
		if (degreeTimer > 0)
		{
			camera->relativeQuaternion = tmpCameraQ;
			camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
		}

		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(185.0f, 67.0f, 11.0f);
			startPos = target->position;
			moveTime = 6.0f;
			moveTimer = moveTime;
			degreeTimer = 6.0f;
			degreeTime = degreeTimer;
			stateCamera++;
			//次ステート用に角度調整
			camera->rotation(D3DXVECTOR3(0, 1, 0),-130.0f);
			camera->rotation(fixedAxisX, -25.0f);
			//カメラの相対位置を一時保存
			tmpCameraQ = camera->relativeQuaternion;
		}

		break;

		
	case CAMERA5:
		moveTimer -= frameTime;
		degreeTimer -= frameTime;
		rate = moveTimer / moveTime;

		target->position = BezierCurve(startPos, D3DXVECTOR3(221.0f, 230.0f, -49.0f), D3DXVECTOR3(257.0f, 183.0f, -110.0f), rate);

		rateY = degreeTimer / degreeTime;
		rateX = degreeTimer / degreeTime;
		degreeY = UtilityFunction::lerp(0, 140.0f, 1.0f - rateY);
	
		if (degreeTimer > 0)
		{
			camera->relativeQuaternion = tmpCameraQ;
			camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
		}

		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(-50.0f, 115.0f, -300.0f);
			startPos = target->position;
			moveTime = 5.0f;
			moveTimer = moveTime;
			degreeTimer = 4.0f;
			degreeTime = degreeTimer;
			stateCamera++;
			//次ステート用に角度調整
			camera->rotation(D3DXVECTOR3(0, 1, 0), 75.0f);
			//カメラの相対位置を一時保存
			tmpCameraQ = camera->relativeQuaternion;
		}

		break;

	case CAMERA6:
		moveTimer -= frameTime;
		degreeTimer -= frameTime;
		rate = moveTimer / moveTime;

		rateX = degreeTimer / degreeTime;
		degreeX = UtilityFunction::lerp(0, -80.0f, 1.0f - rateX);

		BezierCurveS1 = BezierCurve(startPos, D3DXVECTOR3(-43.0f, -50.0f, -200.0f), D3DXVECTOR3(-43.0f, 250.0f, 350.0f), rate);
		BezierCurveS2 = BezierCurve(startPos, D3DXVECTOR3(-43.0f, 300.0f, 150.0f), D3DXVECTOR3(-43.0f, 250.0f, 350.0f), rate);
		D3DXVec3Lerp(&target->position, &BezierCurveS1, &BezierCurveS2, 1.0f - rate);
		if (degreeTimer > 0)
		{
			camera->relativeQuaternion = tmpCameraQ;
			camera->rotation(fixedAxisX, degreeX);
		}

		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(-23.0f, 250.0f, 350.0f);
			startPos = target->position;
			moveTime = 3.0f;
			moveTimer = moveTime;
			degreeTimer = 3.0f;
			degreeTime = degreeTimer;
			stateCamera++;
			
			//カメラの相対位置を一時保存
			tmpCameraQ = camera->relativeQuaternion;
		}


		break;
	case CAMERA7:
		moveTimer -= frameTime;
		degreeTimer -= frameTime;
		rate = moveTimer / moveTime;
		if (measurement)
		{
			targetDistance = D3DXVECTOR3(0, 100.0f, 0) - startPos;
			distance = D3DXVec3Length(&targetDistance);
			measurement = false;
		}

		rateY = degreeTimer / degreeTime;
		rateX = degreeTimer / degreeTime;
		degreeY = UtilityFunction::lerp(0, 180.0f, 1.0f - rateY);
		degreeX = UtilityFunction::lerp(0, 75.0f, 1.0f - rateX);

		D3DXVec3Lerp(&target->position, &startPos, &D3DXVECTOR3(0, 100.0f, 0), 1.0f - rate);
		
		moveDistance = UtilityFunction::lerp(0, distance, 1.0 - rate);
		
		camera->setGazeDistance(moveDistance);
		if (degreeTimer > 0)
		{
			//前のカメラの相対位置に補正する
			camera->relativeQuaternion = tmpCameraQ;
			camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
			camera->rotation(fixedAxisX, degreeX);

		}
		
		if (moveTimer <= 0)
		{
			target->position = D3DXVECTOR3(0, 100.0f, 0);
			startPos = target->position;
			moveTime = 6.0f;
			moveTimer = moveTime;
			degreeTimer = 30.0f;
			degreeTime = degreeTimer;
			stateCamera++;
			//カメラの相対位置を一時保存
			tmpCameraQ = camera->relativeQuaternion;
		}
		
		break;
	case CAMERA8:
		moveTimer -= frameTime;
		degreeTimer -= frameTime;
		rate = moveTimer / moveTime;
		rateY = degreeTimer / degreeTime;

		degreeY = UtilityFunction::lerp(0, 900.0f, 1.0 - rateY);
		if (degreeTimer > 0)
		{
			camera->relativeQuaternion = tmpCameraQ;
			camera->rotation(D3DXVECTOR3(0, 1, 0), degreeY);
		}
		else if (degreeTimer <= 0)
		{
			stateCamera = CAMERA0;
			target->initialize(&D3DXVECTOR3(-34.0f, 180.0f, 100));		//ターゲットの初期位置設定
			camera->setRelative(D3DXQUATERNION(0.0f, 0.0f, 1.0f, 0.0f));   //※ターゲットの初期位置に足される形になっている
			camera->setGazeDistance(20.0f);
			//stateCamera++;
		}

		break;
	case CAMERA9:
		break;
	default:
		break;
	}

	//カメラ
	camera->update();
	
}

//============================================================================================================================================
//【入力の更新】
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
//【描画】
//============================================================================================================================================
void Title::render()
{
	// カメラ・ウィンドウ
	camera->renderReady();
	direct3D9->changeViewportFullWindow();

	//エフェクシアーテスト描画
	effekseerNS::setCameraMatrix(0,camera->position, camera->gazePosition, camera->upVector);
	effekseerNS::render(0);

	// 3D
	render3D(camera);

	// 2D
	render2D();

}

//============================================================================================================================================
//【3D描画】
//============================================================================================================================================
void Title::render3D(Camera* _currentCamera)
{
	//タイトルフィールド（テスト）
	titleFieldRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), _currentCamera->view, _currentCamera->projection, _currentCamera->position);

	//スカイフィールドの描画
	sky->render(_currentCamera->view, _currentCamera->projection, _currentCamera->position);
	//海面の描画
	ocean->render(_currentCamera->view, _currentCamera->projection, _currentCamera->position);

	//ツリーの描画
	treeManager->render(_currentCamera);

}

//============================================================================================================================================
//【2D描画】
//============================================================================================================================================
void Title::render2D()
{
	// タイトルUI
	titleUI.render();

	// αテストを無効に
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================================================================================
//【衝突処理】
//============================================================================================================================================
void Title::collisions(void)
{
	// None
}

//============================================================================================================================================
//【AI処理】
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
//【GUI作成処理】
//===================================================================================================================================
#ifdef _DEBUG
void Title::createGUI()
{
	bool createScene = false;
	bool photographScene = false;
	float backUpTmpVolume = tmpVolume;
	float limitTop = 1000;
	float limitBottom = -1000;
	camera->outputGUI();
	target->outputGUI();

	ImGui::Text(sceneName.c_str());
	ImGui::Text("sceneTime = %f", sceneTimer);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Create Scene", &createScene);
	ImGui::Checkbox("Photograph Scene", &photographScene);
	ImGui::SliderFloat("volume control", &tmpVolume, 0.0f, 1.0f);
	ImGui::Text("CameraState %d", stateCamera);

	if (backUpTmpVolume != tmpVolume)
	{
		backUpTmpVolume = tmpVolume;
	}

	ImGui::Text("controller1 LStick(%.02f,%.02f)", 
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().x,
		input->getController()[inputNS::DINPUT_1P]->getLeftStick().y);
	ImGui::Text("controller1 LStickTrigger(%.02f,%.02f)", 
		input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().x,
		input->getController()[inputNS::DINPUT_1P]->getLeftStickTrigger().y);
	
	//ツール用シーン
	if (createScene)
	{
		selectStateMemory = titleUiNS::CREATE;
		nextScene = (SceneList::CREATE);
		changeScene(nextScene);
	}
	//撮影ツール用シーン
	if (photographScene)
	{
		selectStateMemory = titleUiNS::PHOTOGRAPH;
		nextScene = (SceneList::PHOTOGRAPH);
		changeScene(nextScene);
	}



}
#endif // _DEBUG