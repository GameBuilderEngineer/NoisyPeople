//-----------------------------------------------------------------------------
// ツリー基底クラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
// 更新日 : 2019/12/01 【菅野 樹】
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "GreeningArea.h"
#include "EffekseerManager.h"
#include "GameMaster.h"
#include "Marker.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace treeNS
{
	// ツリー種別
	enum TREE_TYPE
	{
		ANALOG_TREE,
		DIGITAL_TREE,
		TREE_TYPE_MAX
	};

	// 緑化状態
	enum GREEN_STATE
	{
		GREEN,
		DEAD,
		GREEN_STATE_MAX
	};

	// サイズ
	enum TREE_SIZE
	{
		STANDARD,
		LARGE,
		VERY_LARGE,
		TREE_SIZE_MAX
	};

	// モデル
	enum TREE_MODEL
	{
		A_MODEL,
		B_MODEL,
		C_MODEL,
		TREE_MAX
	};

	//ツリーデータ
	typedef struct TreeData
	{
		int treeID;						//識別番号
		int model;						//モデルID
		TREE_TYPE type;					//ANALOG|DIGITAL
		GREEN_STATE greenState;			//緑化状態
		TREE_SIZE size;					//木のサイズ
		int hp;							//HP
		bool isAttaked;					//エネミーが攻撃中
		D3DXVECTOR3 initialPosition;	//初期座標
		D3DXVECTOR3 initialDirection;	//初期方角
		void zeroClear() { ZeroMemory(this, sizeof(TreeData)); }
	} TREESET;

	//定数
	const int	MAX_HP					= 100;		//最大HP
	const float	AROUND_GREEN_TIME		= 2.0f;		//周辺への緑化時間
	const float	AROUND_DEAD_TIME		= 2.0f;		//周辺への枯木時間
	const float	AROUND_GREEN_RANGE_S	= 35.0f;	//周辺への緑化範囲
	const float	AROUND_GREEN_RANGE_L	= 70.0f;	//周辺への緑化範囲
	const float	AROUND_GREEN_RANGE_V	= 120.0f;	//周辺への緑化範囲


	//=============================================================================
	//【抽象状態クラス】
	//=============================================================================
	class AbstractState
	{
	public:
		float	frameTime;
		int		type;
	public:
		virtual void start() = 0;
		virtual void update(float frameTime) = 0;
		virtual void end() = 0;
		virtual AbstractState* transition() = 0;
	};

	//=============================================================================
	//【デジタルツリー標準エフェクト】
	//=============================================================================
	class DigitTree :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3 * syncPosition;
		DigitTree(int no,D3DXVECTOR3* sync){
			syncPosition = sync;
			scale = D3DXVECTOR3(1.0f,30.0f,1.0f);
			managerNo = no;
			effectNo = effekseerNS::DIGIT_TREE;
		}
		virtual void update() {
			position = *syncPosition;
			Instance::update();
		};
	};
	//=============================================================================
	//【デジタル前面エフェクト】
	//=============================================================================
	class DigitFront :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3 * syncPosition;
		DigitFront(D3DXVECTOR3* sync) {
			syncPosition = sync;
			effectNo = effekseerNS::DIGIT_TREE_RAID;
		}
		virtual void update() {
			position = *syncPosition;
			Instance::update();
		};
	};
}


//=============================================================================
//【ツリークラス】
//=============================================================================
class Tree:public Object
{
private:
	treeNS::TreeData				treeData;										// ツリーデータ
	Object							leaf;											// 葉オブジェクト
	GreeningArea					greeningArea;									// 緑化範囲

	LPD3DXMESH						attractorMesh;									// 重力（引力）発生メッシュ
	D3DXMATRIX*						attractorMatrix;								// 重力（引力）発生オブジェクトマトリックス
	static int						numOfTree;										// ツリーオブジェクトの総数
	treeNS::AbstractState*			state;											//状態クラス
	bool							onTransState;									//状態遷移開始フラグ
	bool							nowAroundGreening;								//現在周囲を緑化中
	treeNS::DigitTree*				digitalEffect[gameMasterNS::PLAYER_NUM];		//デジタルシフトエフェクト
	treeNS::DigitFront*				frontDigitalEffect[gameMasterNS::PLAYER_NUM];	//デジタルシフトエフェクト
	bool							selectShift[gameMasterNS::PLAYER_NUM];			//シフト先として選択されている
public:
	int								playerNo;
	bool							culling;
	bool							beforeDigital;
public:
	Tree(treeNS::TreeData _treeData);
	~Tree();
	//基本処理
	void update(float frameTime);
	void render();

	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void grounding();																// 接地処理
	void greeningAround();															//周辺の緑化(デジタル化時)
	void deadAround();																//周辺の枯木化(枯木化時)
	void transState();																//状態遷移
	void playDigitalEffect();														//デジタルエフェクトの再生
	void stopDigitalEffect();														//デジタルエフェクトの停止
	void switchingShownDigitalEffect(bool shown,int playerNo);						//デジタルエフェクトの表示/非表示
	void switchingSelected(bool selected,int playerNo);


	// Getter
	Object* getLeaf();
	Object* getTrunk();
	Object* getGreeningArea();
	static int getNumOfTree();			// ツリーの数を取得
	static void resetNumOfTree();		// ツリーの数をリセット

	treeNS::TreeData* getTreeData();	// ツリーデータを取得
	LPD3DXMESH getMesh();				// 衝突用メッシュへの参照
	bool isAroundGreening();			//周囲へ緑化中か
	bool getTransState();				//状態遷移状態
	bool getSelected(int playerNo);		//選択状況

	// Setter
	void setDataToTree(treeNS::TreeData _treeData);	
	void addHp(int value,int playerNo);				//デジタル化するHPの増加
	void reduceHp(int value);						//デジタル化するHPの減少
	void setGreeningArea(float value);				//緑化エリアのスケールを設定
	void disableAroundGreening();					//周囲への緑化を終了
};

//=============================================================================
//【名前空間】
//=============================================================================
namespace treeNS
{
	//=============================================================================
	//【デジタル状態クラス】
	//=============================================================================
	class DigitalState :public AbstractState
	{
	private:
		Tree* tree;
		float aroundGreenTimer;//周囲への緑化タイマー
		float aroundGreenRange;
	public:
		DigitalState(Tree* target);
		~DigitalState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

	//=============================================================================
	//【アナログ状態クラス】
	//=============================================================================
	class AnalogState :public AbstractState
	{
	private:
		Tree* tree;
		float aroundDeadTimer;//周囲への枯木タイマー
		float aroundDeadRange;
	public:
		AnalogState(Tree* target);
		~AnalogState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

	//=============================================================================
	//【緑化状態クラス】
	//=============================================================================
	class GreenState :public AbstractState
	{
	private:
		Tree* tree;
	public:
		GreenState(Tree* target);
		~GreenState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

}