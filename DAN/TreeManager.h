//-----------------------------------------------------------------------------
// ツリー管理クラス [TreeManager.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/4
//-----------------------------------------------------------------------------
// 更新日 : 2019/11/12 【菅野 樹】
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshRenderer.h"
#include "Tree.h"
#include "AnalogTree.h"
#include "DigitalTree.h"
#include "TreeEffect.h"
#include "GameMaster.h"
#include "networkClient.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace treeManagerNS
{	
}


//=============================================================================
//クラス定義
//=============================================================================
class TreeManager
{
private:
	std::vector<Tree*> treeList;				// ツリーポインタリスト
	//レンダラー：アナログツリー
	StaticMeshRenderer* aTrunkRenderer;			// Aモデル幹描画オブジェクト
	StaticMeshRenderer* aLeafRenderer;			// Aモデル葉描画オブジェクト
	StaticMeshRenderer* bTrunkRenderer;			// Bモデル幹描画オブジェクト
	StaticMeshRenderer* bLeafRenderer;			// Bモデル葉描画オブジェクト
	StaticMeshRenderer* cTrunkRenderer;			// Cモデル幹描画オブジェクト
	StaticMeshRenderer* cLeafRenderer;			// Cモデル葉描画オブジェクト

	//エフェクト
	TreeEffect*	treeEffect;						//デジタルツリーエフェクト

	int nextID;									// 次回ツリー発行ID
	LPD3DXMESH	attractorMesh;					// 重力（引力）発生メッシュ
	D3DXMATRIX*	attractorMatrix;				// 重力（引力）発生オブジェクトマトリックス

	int treeNum;
	int greeningTreeNum;						//緑化本数
	float greeningRate;							//緑化率
	bool playedDigitalTreeEffect[gameMasterNS::PLAYER_NUM];

	GameMaster*			gameMaster;				//ゲームマスターへのポインタ

public:
	~TreeManager();
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(Camera* camera);

	//ツリーの生成
	void createTree(treeNS::TreeData treeData);
	void createUsingTool();
	//全てのツリーの削除
	void destroyAllTree();

	//全ての描画登録/解除
	void allRegister();
	void allUnRegister();
	//幹の描画登録/解除
	void registerTrunk(Tree* tree);
	void unRegisterTrunk(Tree* tree);
	//葉の描画登録/解除
	void registerLeafRendering(Object* leaf, int _model);
	void unRegisterLeafRendering(Object* leaf, int _model);
	//見た目の切替
	void switchingVisionView(int playerNo);
	void switchingNormalView(int playerNo);

	//デジタルツリーエフェクト
	void playDigitalTreeEffect(int playerNo);
	void stopDigitalTreeEffect(int playerNo);

	//ツリーIDを発行する
	int	issueNewTreeID();

	// ツリーを探す
	Tree* findTree(int _treeID);

	void outputGUI();


	//Setter
	void setGameMaster(GameMaster* gameMaster);

	// Getter
	std::vector<Tree*>& getTreeList();
	float getGreeningRate();
	int getTreeNum();
};
