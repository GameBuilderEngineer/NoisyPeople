//===================================================================================================================================
//【Object.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#include "Ray.h"
#include "StaticMeshLoader.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "LinearTreeCell.h"
#include "ObjectTypeList.h"
#include "DebugBox.h"
#include "BoundingSphere.h"


//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace objectNS {
	enum FILLMODE
	{
		SOLID		= (int)D3DFILL_SOLID,
		WIREFRAME	= (int)D3DFILL_WIREFRAME,
		POINT		= (int)D3DFILL_POINT,
	};

	const int AXIS_X	= 0x00000001;
	const int AXIS_RX	= 0x00000002;
	const int AXIS_Y	= 0x00000004;
	const int AXIS_RY	= 0x00000008;
	const int AXIS_Z	= 0x00000010;
	const int AXIS_RZ	= 0x00000020;

	void resetCounter();
}

//===================================================================================================================================
//【オブジェクトクラス】
//===================================================================================================================================
class Object:public Base
{
public:
	//Data
	//ステータス変数
	static int			objectCounter;			//オブジェクトカウンター：IDの割当に使用
	int					id;						//ID
	D3DXVECTOR3			position;				//位置
	D3DXVECTOR3			center;					//中心位置
	D3DXQUATERNION		quaternion;				//回転
	D3DXVECTOR3			scale;					//スケール
	float				radius;					//衝突半径
	D3DXVECTOR3			size;					//衝突サイズ(幅/高さ/奥行き)
	float				alpha;					//透過値
	D3DXCOLOR			color;					//カラー

	//移動系変数
	D3DXVECTOR3			speed;					//速度
	D3DXVECTOR3			acceleration;			//加速度
	D3DXVECTOR3			gravity;				//重力


	//各種フラグ
	bool				onGravity;				//重力有効化フラグ
	bool				onActive;				//アクティブ化フラグ

	//方向6軸
	Ray					axisX;					//x軸
	Ray					axisY;					//y軸
	Ray					axisZ;					//z軸
	Ray					reverseAxisX;			//-x軸
	Ray					reverseAxisY;			//-y軸
	Ray					reverseAxisZ;			//-z軸

	//重力Ray
	Ray					gravityRay;				//重力Ray

	//行列（位置・回転・スケール・ワールド）
	D3DXMATRIX			matrixPosition;			//位置行列
	D3DXMATRIX			matrixRotation;			//回転行列
	D3DXMATRIX			matrixScale;			//スケール行列
	D3DXMATRIX			matrixWorld;			//ワールド行列
	D3DXMATRIX			matrixCenter;			//中心座標行列

	//タイマー
	float				existenceTimer;			//存在時間

	//オブジェクトツリーへ所属するためのインターフェースクラス
	ObjectTree<Object>	treeCell;				//木空間

	//衝突判定インスタンス
	BoundingSphere* sphere;
	DebugBox* box;

public:
	//Method
	Object();
	virtual ~Object();
	
	//基本関数
	HRESULT initialize(D3DXVECTOR3* _position);
	void update();

	//デバッグ関係関数
	void debugRender();				//デバッグ用ユーティリティレンダー
	virtual void outputGUI();			//ImGUIへObjectの情報を出力

	//getter
	D3DXMATRIX*				getMatrixWorld();
	D3DXVECTOR3*			getPosition();
	D3DXQUATERNION			getQuaternion();
	float					getRadius();
	D3DXVECTOR3				getSpeed();
	D3DXVECTOR3				getAcceleration();
	D3DXVECTOR3				getGravity();
	Ray*					getAxisX();
	Ray*					getAxisY();
	Ray*					getAxisZ();
	Ray*					getReverseAxisX();
	Ray*					getReverseAxisY();
	Ray*					getReverseAxisZ();
	Ray*					getGravityRay();
	bool					getActive();
	float					getRight();
	float					getLeft();
	float					getTop();
	float					getBottom();
	float					getFront();
	float					getBack();
	D3DXVECTOR3				getMin();
	D3DXVECTOR3				getMax();

	//setter
	void					setSpeed(D3DXVECTOR3 _speed);
	void					addSpeed(D3DXVECTOR3 add);
	void					setPosition(D3DXVECTOR3 _position);
	void					setQuaternion(D3DXQUATERNION _quaternion);
	void					setAlpha(float value);							//α値の設定
	void					setGravity(D3DXVECTOR3 source, float power);	//重力の設定
	void					activation();									//アクティブ化
	void					inActivation();									//非アクティブ化
	void					setRadius(float value);
	void					setSize(D3DXVECTOR3 value);

	//姿勢制御
	void postureControl(D3DXVECTOR3 currentDirection, D3DXVECTOR3 nextDirection,float t)
	{
		Base::postureControl(&quaternion,currentDirection, nextDirection, t);
	}

	//任意軸線型補間回転
	void anyAxisRotationSlerp(D3DXVECTOR3 axis, float radian,float t)
	{
		Base::anyAxisRotationSlerp(&quaternion, axis, radian, t);
	}
	
	//任意軸回転
	void anyAxisRotation(D3DXVECTOR3 axis, float degree)
	{
		Base::anyAxisRotation(&quaternion, axis, degree);
	}
};

