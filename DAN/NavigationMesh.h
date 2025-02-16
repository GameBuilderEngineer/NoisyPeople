//-----------------------------------------------------------------------------
// ナビゲーションメッシュ処理クラス [NavigationMesh.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/5
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include <vector>
#include "Object.h"
#include "StaticMeshRenderer.h"
#include "VertexAccessor.h"
#include "MeshData.h"
#include "AStar.h"
#include "PathFollowing.h"
#include <cassert>


//=============================================================================
// 名前空間
//=============================================================================
namespace navigationMeshNS
{
	enum NAVIRESULT
	{
		NAVI_OK						= 0,		
		CURRENT_NOT_ON_MESH			= -1,
		DESTINATION_NOT_ON_MESH		= -2,
		IMPASSE						= -3,
		UNKNOWN_ERROR				= -99
	};


#ifdef _DEBUG
	const float FLOATING_HEIGHT = 0.0f;
#endif
}

//=============================================================================
//クラス定義
//=============================================================================
class NavigationMesh: public Object
{
private:
	StaticMesh* staticMesh;						// メッシュ情報
	VertexAccessor vtxAccessor;					// 頂点アクセスオブジェクト
	MeshData meshData;							// メッシュデータオブジェクト
	AStar aStar;								// パス検索オブジェクト
	PathFollowing pathFollowing;				// パスフォローイングオブジェクト
	StaticMeshRenderer* renderer;				// 描画オブジェクト
	D3DXVECTOR3 gravityDirection;				// 重力方向
	static NavigationMesh* pointer;				// 取得用ポインタ

	int cntEdge;

public:
	NavigationMesh(StaticMesh* staticMesh);
	~NavigationMesh();
	void initialize();
	void uninitialize();
	void update();
	// 経路探索
	navigationMeshNS::NAVIRESULT pathSearch(LinkedList<meshDataNS::Index2>** edgeList,
		DWORD* faceIndex, D3DXVECTOR3 from, D3DXVECTOR3 dest);
	// ステアリング（移動ベクトルの作成）
	navigationMeshNS::NAVIRESULT steering(D3DXVECTOR3* out, DWORD* faceIndex,
		D3DXVECTOR3 from, D3DXVECTOR3 dest, LinkedList<meshDataNS::Index2>** edgeList);
	// 接地チェック
	bool isHitGrounding(float* distance, DWORD* faceIndex, D3DXVECTOR3 _position);
	// Getter
	static NavigationMesh* getNaviMesh()
	{
		assert(pointer != NULL);
		return pointer;
	}
	D3DXVECTOR3 getGravityDirection()
	{
		return gravityDirection;
	}

	MeshData* getMeshData()
	{
		return &meshData;
	}

#ifdef _DEBUG
	LinkedList<meshDataNS::Index2>** debugEdgeList = NULL;
	void dumpEdgeList(LinkedList<meshDataNS::Index2>* _edgeList);
	void debugRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void debugRenderMesh(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void debugRenderEdge(LinkedList<meshDataNS::Index2>** edgeList);
	void changeColor();
	void affectToEdgeVertex(LinkedList<meshDataNS::Index2>** edgeList);
	void outputGUI();
#endif // _DEBUG
};