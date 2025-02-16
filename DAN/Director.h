//===================================================================================================================================
//【Director.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/17
// [更新日]2019/10/19
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "MemoryViewer.h"
#include "Window.h"
#include "Direct3D9.h"
#include "ImguiManager.h"
#include "Input.h"
#include "VirtualController.h"
#include "AbstractScene.h"
#include "TextureLoader.h"
#include "StaticMeshLoader.h"
#include "ShaderLoader.h"
#include "GameMaster.h"
#include "Fader.h"
#include "EffekseerManager.h"
#include "SerialCommunication.h"
#include "Sound.h"

//===================================================================================================================================
//【列挙型定数】
//===================================================================================================================================
enum
{
	VARIABLE_FPS,	//可変フレームレート
	FIXED_FPS,		//固定フレームレート
};

//===================================================================================================================================
//【管理クラス】
//[用途]アプリケーション全体の管理を行うクラス
//===================================================================================================================================
class Director : public Base
{
public:
	//Data
#ifdef _DEBUG
	MemoryViewer*		memory;
	ImguiManager*		imgui;					//imguiマネージャークラス
	bool				onGUI;					//GUI表示フラグ
#endif // _DEBUG

	HINSTANCE				instance;				//インスタンス
	Window*					window;					//ウィンドウクラス・ハンドル
	HWND					wnd;					//ウィンドウハンドル
	Direct3D9*				d3d;					//DirectX9クラス・デバイス
	Input*					input;					//Inputクラス
	AbstractScene*			scene;					//抽象シーンクラス
	TextureLoader*			textureLoader;			//テクスチャ読込クラス
	StaticMeshLoader*		staticMeshLoader;		//スタティックメッシュ読込クラス
	ShaderLoader*			shaderLoader;			//シェーダー読込クラス
	SoundInterface*			soundInterface;			//サウンドのインターフェースクラス
	Fader*					fader;					//フェーダー(イン・アウト)クラス
	EffekseerManager*		effekseerManager[3];	//エフェクシアーマネージャー
	GameMaster*				gameMaster;				//ゲーム運営クラス
	SerialCommunication*	serialCommunication;	//シリアル通信クラス
	std::string*			currentSceneName;		//現在のシーンの名前
	int						fpsMode;				//フレームレート状態変数
	int						fixedFps;				//固定フレームレート値
	LARGE_INTEGER			timeStart;				// パフォーマンスカウンターの開始値
	LARGE_INTEGER			timeEnd;				// パフォーマンスカウンターの終了値
	LARGE_INTEGER			timerFreq;				// パフォーマンスカウンターの周波数
	float					frameTime;				//フレーム時間
	float					sleepRenderTime;		//描画待機時間
	bool					hiddenCursor;			//カーソル非表示フラグ
	bool					lockCursor;				//カーソル位置固定フラグ

	//Method
	Director();
	~Director();
	HRESULT initialize();						//初期化
	void run(HINSTANCE);						//エントリポイントにおける呼び出し関数
	void mainLoop();							//メッセージループ内ループ処理関数
	void update();								//更新
	void createGUI();							//GUIウィンドウの作成
	void render();								//描画
	void setFrameTime();						//frameTimeの設定
	void displayFPS();							//FPS表示
	void fixFPS();								//固定FPS補正
	void changeNextScene();						//シーンの切り替え
};
