//===================================================================================================================================
//【Camera.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/12/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Window.h"
#include "Direct3D9.h"
#include "Ray.h"
#include "Object.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace cameraNS {
	const float LIMIT_BOTTOM_Y = -3.0f;
	const float LIMIT_TOP_Y = 7.0f;
	const float INIT_NEAR_Z = 0.1f;
	const float INIT_FAR_Z = 30000.0f;

	enum {
		ROTATION_TOP_LIMIT		= 0x00000001,
		ROTATION_BOTTOM_LIMIT	= 0x00000002,
	};

	const int ALL_LIMIT = ROTATION_TOP_LIMIT | ROTATION_BOTTOM_LIMIT;
}

//===================================================================================================================================
//【カメラクラス】
//===================================================================================================================================
class Camera : public Base
{
private:
public:
	//Data
	Direct3D9* d3d;

	float				fieldOfView;				//視野角
	float				aspect;						//画面縦横比
	float				nearZ;						//視認近距離
	float				farZ;						//視認遠距離
	DWORD				windowWidth;
	DWORD				windowHeight;
	bool				onGaze;						//=true 注視ありモード
	int					limit;						//制限フラグ
	float				limitValueRotaionTop;		//高さ制限
	float				limitValueRotaionBottom;	//高さ制限
	D3DXVECTOR3			position;					//カメラ位置
	D3DXVECTOR3			gazePosition;				//注視
	D3DXVECTOR3			relativeGaze;				//注視相対値
	D3DXVECTOR3			upVector;					//上方ベクトル
	D3DXQUATERNION		posture;					//姿勢クォータニオン
	D3DXMATRIX			world;						//姿勢制御用行列
	D3DXQUATERNION		relativeQuaternion;			//ターゲットに対する相対位置ベクトル
	float				relativeDistance;			//相対位置ベクトル距離
	float				betweenGaze;				//相対位置ベクトル固定距離
	D3DXVECTOR3*		target;						//注視ターゲット位置ポインタ
	D3DXVECTOR3*		targetX;					//注視ターゲットX方向ポインタ
	D3DXVECTOR3*		targetY;					//注視ターゲットY方向ポインタ
	D3DXVECTOR3*		targetZ;					//注視ターゲットZ方向ポインタ
	D3DXMATRIX			view;						//ビューマトリックス
	D3DXMATRIX			projection;					//プロジェクションマトリックス
	D3DXMATRIX			ortho;						//正射影行列

	//Method
	Camera();
	~Camera();

	//基本関数
	HRESULT initialize(DWORD _windowWidth, DWORD _windowHeight);
	void update();
	void renderReady();
	virtual void outputGUI();

	//衝突補正
	bool insetCorrection(LPD3DXMESH mesh, D3DXMATRIX matrix);	//めり込み補正
	bool sphereCollide(D3DXVECTOR3 position,float radius);		//球判定
	bool rayCollide(LPD3DXMESH mesh, D3DXMATRIX matrix);		//レイ判定

	//制限[limit]
	void enableLimit(int limitParameter);				//制限の有効化
	void disableLimit(int limitParameter);				//制限の無効化
	void setLimitRotationTop(float value);				//回転上方向制限
	void setLimitRotationBottom(float value);			//回転下方向制限

	//操作[operation]
	void rotation(D3DXVECTOR3 axis, float angle);
	void lockOn(D3DXVECTOR3 lockOnTarget,float frameTime);

	//タイトル操作 ターゲットに追従
	void GetViewMatrix(D3DXMATRIX* viewOut, D3DXMATRIX* world);

	//注視点間距離の補正
	void setGazeDistance(float newValue);
	//正射影行列を更新する
	void updateOrtho();

	//getter
	D3DXVECTOR3 getDirectionX() { 
		D3DXVECTOR3 result;
		D3DXVec3Normalize(&result, &(gazePosition- position));
		D3DXVec3Cross(&result, &upVector,&result);
		D3DXVec3Normalize(&result, &result);
		return result;}
	D3DXVECTOR3 getDirectionY() {
		return upVector;}
	D3DXVECTOR3 getDirectionZ() {
		D3DXVECTOR3 result;
		D3DXVec3Normalize(&result, &(gazePosition- position));
		return result;}
	D3DXVECTOR3 getHorizontalAxis();
	D3DXVECTOR3 getAxisZ();
	D3DXVECTOR3 getAxisY();

	//setter
	void setAspect(DWORD _windowWidth, DWORD _windowHeight);
	void setFieldOfView(float value);
	void setPosition(D3DXVECTOR3 _position) { position = _position; };
	void setGaze(D3DXVECTOR3 _gazePosition) { gazePosition = _gazePosition; };
	void setUpVector(D3DXVECTOR3 _upVector) { upVector = _upVector; };
	void setRelative(D3DXQUATERNION _relativeQuaternion) {relativeQuaternion = _relativeQuaternion; };
	void setRelative(D3DXVECTOR3 relativeVector) {
		relativeQuaternion.x = relativeVector.x;
		relativeQuaternion.y = relativeVector.y;
		relativeQuaternion.z = relativeVector.z;
		relativeQuaternion.w = 1.0f;};
	void setRelativeLength(float length) { relativeDistance = length; }
	void setTarget(D3DXVECTOR3* _target) { target = _target; }
	void setTargetX(D3DXVECTOR3* targetAxisX) { targetX = targetAxisX; }
	void setTargetY(D3DXVECTOR3* targetAxisY) { targetY = targetAxisY; }
	void setTargetZ(D3DXVECTOR3* targetAxisZ) { targetZ = targetAxisZ; }
	void setRelativeGaze(D3DXVECTOR3 _relativeGaze) { relativeGaze = _relativeGaze;}
	HRESULT setViewProjection();
	void setNearZ(float value);
	void setFarZ(float value);

};