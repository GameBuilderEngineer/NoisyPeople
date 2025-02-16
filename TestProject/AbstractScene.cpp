//===================================================================================================================================
//【AbstractScene.cpp】
// [作成者] HAL東京GP12A332 11 菅野 樹
// [作成日] 2019/09/19
// [更新日] 2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace SceneList;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
AbstractScene::AbstractScene() {
	nextScene = NONE_SCENE;
	sceneName = "none";
	sceneTimer = 0.0f;
	stopTimer = 0.0f;
	onChange = false;
	device = getDevice();
	direct3D9 = getDirect3D9();
	input = getInput();
	camera = NULL;
	light = NULL;
//	gameMaster = new GameMaster;
#ifdef _DEBUG
	showGUI = true;
#endif // _DEBUG
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
AbstractScene::~AbstractScene() {
//	SAFE_DELETE(gameMaster);
}

//===================================================================================================================================
//【ゲーム管理情報の引継ぎ】
//[用途] 外部へゲーム管理情報を出力する
//[引数] 
//GameMaster* destination：コピー先
//[戻値]
//なし
//===================================================================================================================================
//void AbstractScene::copyGameMaster(GameMaster* destination)
//{
//	*destination = *gameMaster;
//}

//===================================================================================================================================
//【ゲーム管理情報の受け取り(setter)】
//[用途] 外部からゲーム管理情報を取得する
//[引数] 
//GameMaster* destination：コピー先
//[戻値]
//なし
//===================================================================================================================================
//void AbstractScene::setGameMaster(GameMaster* resorce)
//{
//	*gameMaster = *resorce;
//}

//===================================================================================================================================
//【アニメーションローダーのセット】
//===================================================================================================================================
//void AbstractScene::setAnimationLoader(AnimationLoader* animationLoader)
//{
//	this->animationLoader = animationLoader;
//}