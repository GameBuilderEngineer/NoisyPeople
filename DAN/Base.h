//===================================================================================================================================
//【Base.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/17
// [更新日]2019/10/27
//===================================================================================================================================
#define D3D_DEBUG_INFO
#define XAUDIO2_ON		(true)
#define XADUIO2_OFF		(flase)
#define XADUIO2_STATE	(XAUDIO2_ON) 
//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#pragma once
#include <stdio.h>
#include <crtdbg.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <VersionHelpers.h>

#if(XADUIO2_STATE)
#include <xaudio2.h>
#include <x3daudio.h>
#endif
//===================================================================================================================================
//【必要なライブラリファイルのロード】
//===================================================================================================================================
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")

#if(XADUIO2_STATE)
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"x3daudio.lib")
#endif

//===================================================================================================================================
//【警告非表示】
//===================================================================================================================================
#pragma warning(disable:4305)
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#pragma warning(disable:4111)

//===================================================================================================================================
//【マクロ定義】
//===================================================================================================================================
#ifdef _DEBUG
//Debug
#define APP_NAME (LPSTR)"D/A:Nature"
#if TRUE
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#else
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#endif
#else
//Release
#define APP_NAME (LPSTR)"D/A:Nature"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#endif // _DEBUG
//Common
#define MFAIL(code,string) if(FAILED(code)){MessageBox(0,string,"error",MB_OK);return E_FAIL;}
#define MFALSE(code,string) if(!(code)) {MessageBox(0,string,"error",MB_OK);return E_FAIL;}
#define MSG(text) MessageBox(0,text,"message", MB_OK|MB_TOPMOST);

//===================================================================================================================================
//【ポインタ参照項目を安全に処理するための関数テンプレート】
// これらのテンプレートによって定義された関数は、
// 通常の関数呼び出し構文を使用して呼び出すことができます。
// コンパイラは、Tを呼び出すパラメータの型に置き換える関数を作成します。
//===================================================================================================================================
// ポインタ参照されるアイテムを安全に解放
template<typename T>
inline void safeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease //後方互換性のために

// ポインタ参照されるアイテムを安全に削除
template<typename T>
inline void safeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE safeDelete//後方互換性のために

// ポインタ参照される配列を安全に削除
template<typename T>
inline void safeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE_ARRAY safeDeleteArray //後方互換性のために

//===================================================================================================================================
//【定数定義】
//===================================================================================================================================
static const int FRAME_RATE = 60;
static const int MAX_FRAME_RATE = 200;
static const int MIN_FRAME_RATE = 5;

//===================================================================================================================================
//【構造体】
//===================================================================================================================================
struct Vertex3D
{
	D3DXVECTOR3 coord;
};
struct Vertex2D
{
	D3DXVECTOR2 coord;
};
struct UV
{
	D3DXVECTOR2 coord;
};
struct Line
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
};
struct Cylinder
{
	Line		centerLine;
	float		height;
	float		radius;
};

//===================================================================================================================================
//【クラス】
//===================================================================================================================================
class Base
{
public:
	static void postureControl(D3DXQUATERNION* quaternion, D3DXVECTOR3 currentDirection, D3DXVECTOR3 nextDirection, float time);	//姿勢制御
	static void anyAxisRotationSlerp(D3DXQUATERNION* quaternion, D3DXVECTOR3 axis, float radian, float time);						//任意軸線型補間回転
	static void anyAxisRotation(D3DXQUATERNION* quaternion, D3DXVECTOR3 axis, float degree);										//任意軸回転
	static void anyAxisRotation(D3DXVECTOR3* vector, D3DXVECTOR3 axis, float degree);										//任意軸回転
	static bool formedRadianAngle(float* out, D3DXVECTOR3 v1, D3DXVECTOR3 v2);														//二つのベクトルのなす角を計算
	D3DXVECTOR3 slip(D3DXVECTOR3 L, D3DXVECTOR3 N);																					//スリップ関数(入射ベクトルLのベクトルNに反するベクトル成分を打ち消す。）
	static float between2VectorLength(D3DXVECTOR3 position1, D3DXVECTOR3 position2);												//２つの位置ベクトル間の距離を計算する
	static float between2VectorDirection(D3DXVECTOR3* out, D3DXVECTOR3 position1, D3DXVECTOR3 position2);							//２つの位置ベクトル間の距離の計算に加え、位置１から位置２に向かう（1→2）ベクトルを作成する(ノルム1)
	void copyVertexBuffer(unsigned size, void *sorce, IDirect3DVertexBuffer9 *buffer);												//頂点バッファへのコピー
	D3DXVECTOR3 nearestPointOnLine(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXVECTOR3 point);											//ある点との線分上で最も近い点を求める
	static float betweenDistancePointAndPlane(D3DXPLANE plane, D3DXVECTOR3 point);
	static float between2LineDistance(Line line1, Line line2);

};

//===================================================================================================================================
//【プロトタイプ宣言】
//===================================================================================================================================
void initializeDirectory(char* root);			//ルートパスの初期化関数					[用途]ルートパス変数へルートパスへ保存する
void setRootDirectory();						//ルートディレクトリへのセット関数			[用途]ルートディレクトリへカレントディレクトリを切り替える
void setDataDirectory();						//データディレクトリへのセット関数			[用途]セーブデータやテキストデータを格納するデータディレクトリへカレントディレクトリを切り替える
void setTextureDirectory();						//テクスチャディレクトリへのセット関数		[用途]テクスチャを格納するテクスチャディレクトリへカレントディレクトリを切り替える
void setModelDirectory();						//モデルディレクトリへのセット関数			[用途]3Dモデルデータを格納するモデルディレクトリへカレントディレクトリを切り替える
void setShaderDirectory();						//シェーダーディレクトリへのセット関数		[用途]シェーダーファイルを格納するシェーダーディレクトリへカレントディレクトリを切り替える
void setSoundDirectory(int endpointVoiceId);	//サウンドディレクトリへのセット関数		[用途]サウンドデータを格納するサウンドディレクトリへカレントディレクトリを切り替える
void setEffekseerDirectory();					//エフェクシアーディレクトリへのセット関数	[用途]エフェクシアーファイルを格納するシェーダーディレクトリへカレントディレクトリを切り替える
void setToolsDirectory();						//ツールディレクトリへのセット関数			[用途]ツールデータを格納するツールディレクトリへカレントディレクトリを切り替える
void setSomethingDirectory(const char* something);