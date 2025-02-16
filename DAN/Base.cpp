//===================================================================================================================================
//【Base.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/17
// [更新日]2019/10/24
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Sound.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
char rootPath[1124] = { 0 };

//===================================================================================================================================
//【関数定義】
//===================================================================================================================================

//-----------------------------------------------------------------------------------------------------------------------------------
//ルートパスの初期化関数
//[用途]ルートパス変数へルートパスへ保存する
//-----------------------------------------------------------------------------------------------------------------------------------
void initializeDirectory(char* root)
{
	strcpy(rootPath, root);//パス(root)をrootPathへコピー
}

//-----------------------------------------------------------------------------------------------------------------------------------
//ルートディレクトリへのセット関数			
//[用途]ルートディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setRootDirectory()
{
	SetCurrentDirectory(rootPath);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//データディレクトリへのセット関数	
//[用途]セーブデータやテキストデータを格納するデータディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setDataDirectory()
{
	char tmporary[512] = { 0 };
	strcpy(tmporary, rootPath);
	strcat(tmporary, "\\01 Data File");
	SetCurrentDirectory(tmporary);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//テクスチャディレクトリへのセット関数
//[用途]テクスチャを格納するテクスチャディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setTextureDirectory()
{
	char tmporary[512] = { 0 };
	strcpy(tmporary, rootPath);
	strcat(tmporary, "\\02 Texture File");
	SetCurrentDirectory(tmporary);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//シェーダーディレクトリへのセット関数		
//[用途]シェーダーファイルを格納するシェーダーディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setShaderDirectory()
{
	char tmporary[512] = { 0 };
	strcpy(tmporary, rootPath);
	strcat(tmporary, "\\03 Shader File");
	SetCurrentDirectory(tmporary);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//サウンドディレクトリへのセット関数		
//[用途]サウンドデータを格納するサウンドディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setSoundDirectory(int endpointVoiceId)
{
	char tmporary[512] = { 0 };
	strcpy(tmporary, rootPath);
	if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_SE)
	{
		strcat(tmporary, "\\04 Sound File\\SE");
	}
	else if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_BGM)
	{
		strcat(tmporary, "\\04 Sound File\\BGM");
	}
	else if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_S3D)
	{
		strcat(tmporary, "\\04 Sound File\\3D");
	}
	else
	{
		strcat(tmporary, "\\04 Sound File");
	}
	SetCurrentDirectory(tmporary);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//ツールディレクトリへのセット関数	
//[用途]ツールデータを格納するツールディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setToolsDirectory()
{
	char tmporary[512] = { 0 };
	strcpy(tmporary, rootPath);
	strcat(tmporary, "\\05 Tools File");
	SetCurrentDirectory(tmporary);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//エフェクシアーディレクトリへのセット関数		
//[用途]エフェクシアーファイルを格納するエフェクシアーディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setEffekseerDirectory()
{
	char tmporary[512] = { 0 };
	strcpy(tmporary, rootPath);
	strcat(tmporary, "\\06 Effekseer File");
	SetCurrentDirectory(tmporary);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//モデルディレクトリへのセット関数
//[用途]3Dモデルデータを格納するモデルディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setModelDirectory()
{
	char tmporary[512] = { 0 };
	strcpy(tmporary, rootPath);
	strcat(tmporary, "\\07 Model File");
	SetCurrentDirectory(tmporary);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//データディレクトリへのセット関数	
//[用途]セーブデータやテキストデータを格納するデータディレクトリへカレントディレクトリを切り替える
//-----------------------------------------------------------------------------------------------------------------------------------
void setSomethingDirectory(const char* something)
{
	char tmporary[512] = { 0 };
	strcpy(tmporary, rootPath);
	strcat(tmporary, something);
	SetCurrentDirectory(tmporary);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//姿勢制御
//-----------------------------------------------------------------------------------------------------------------------------------
void Base::postureControl(D3DXQUATERNION* quaternion, D3DXVECTOR3 currentDirection, D3DXVECTOR3 nextDirection, float t)
{
	D3DXVECTOR3 controlAxis;
	//法線とキャラクターとの外積を求める（平面に直立するための回転軸となる）
	D3DXVec3Cross(&controlAxis, &currentDirection, &nextDirection);
	D3DXVec3Normalize(&controlAxis, &controlAxis);
	float rotationRadian;
	if (formedRadianAngle(&rotationRadian, currentDirection, nextDirection))
	{
		anyAxisRotationSlerp(quaternion, controlAxis, rotationRadian, t);
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
//任意軸線型補間回転
//-----------------------------------------------------------------------------------------------------------------------------------
void Base::anyAxisRotationSlerp(D3DXQUATERNION* quaternion, D3DXVECTOR3 axis, float radian, float t)
{
	D3DXQUATERNION deltaRotation(0, 0, 0, 1);
	D3DXQuaternionRotationAxis(&deltaRotation, &axis, radian);
	deltaRotation = *quaternion * deltaRotation;
	D3DXQuaternionSlerp(quaternion, quaternion, &deltaRotation, t);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//任意軸回転
//-----------------------------------------------------------------------------------------------------------------------------------
void Base::anyAxisRotation(D3DXQUATERNION* quaternion, D3DXVECTOR3 axis, float degree)
{
	D3DXQUATERNION rotationQ(0, 0, 0, 1);
	float radian = D3DXToRadian(degree);
	D3DXQuaternionRotationAxis(&rotationQ, &axis, radian);

	//共役*回転対象*回転クォータニオン
	*quaternion *= rotationQ;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//任意軸回転
//-----------------------------------------------------------------------------------------------------------------------------------
void Base::anyAxisRotation(D3DXVECTOR3* vector,D3DXVECTOR3 axis, float degree)
{
	//ベクトルをクォータニオンに置き換える
	D3DXQUATERNION vectorQuaternion;
	vectorQuaternion.x = vector->x;
	vectorQuaternion.y = vector->y;
	vectorQuaternion.z = vector->z;
	vectorQuaternion.w = 1.0f;

	D3DXQUATERNION conjugateQ;
	D3DXQUATERNION rotationQ(0, 0, 0, 1);
	float radian = D3DXToRadian(degree);
	D3DXQuaternionRotationAxis(&rotationQ, &axis, radian);
	D3DXQuaternionConjugate(&conjugateQ, &rotationQ);
	
	D3DXQUATERNION temporaryQ;
	//共役*回転対象*回転クォータニオン
	temporaryQ = conjugateQ * vectorQuaternion * rotationQ;

	*vector = (D3DXVECTOR3)temporaryQ;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//二つのベクトルのなす角を計算
//          v1・v2
//cosΘ = -------------
//         |v1|*|v2|
//-----------------------------------------------------------------------------------------------------------------------------------
bool Base::formedRadianAngle(float* out, D3DXVECTOR3 v1, D3DXVECTOR3 v2) {
	float dot = D3DXVec3Dot(&v1, &v2);
	if (isnan(dot))return false;

	float v1Length = D3DXVec3Length(&v1);
	float v2Length = D3DXVec3Length(&v2);
	if (v1Length == 0 || v2Length == 0)return false;

	*out = acosf(dot / (v1Length*v2Length));
	if (isnan(*out))return false;
	return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//スリップ関数(入射ベクトルLのベクトルNに反するベクトル成分を打ち消す。）
//-----------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 Base::slip(D3DXVECTOR3 L, D3DXVECTOR3 N)
{
	D3DXVECTOR3 S; //滑りベクトル（滑る方向）

	//滑りベクトル S=L-(N * L)/(|N|^2)*N
	S = L - ((D3DXVec3Dot(&N, &L)) / (powf(D3DXVec3Length(&N), 2)))*N;

	return S;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//２つの位置ベクトル間の距離を計算する
//-----------------------------------------------------------------------------------------------------------------------------------
float Base::between2VectorLength(D3DXVECTOR3 position1, D3DXVECTOR3 position2)
{
	return D3DXVec3Length(&(position1 - position2));
}

//-----------------------------------------------------------------------------------------------------------------------------------
//２つの位置ベクトル間の距離の計算に加え、位置１から位置２に向かう（1→2）ベクトルを作成する(ノルム1)
//-----------------------------------------------------------------------------------------------------------------------------------
float Base::between2VectorDirection(D3DXVECTOR3* out, D3DXVECTOR3 position1, D3DXVECTOR3 position2)
{
	D3DXVECTOR3 direction = position2 - position1;
	D3DXVec3Normalize(out, &direction);
	return D3DXVec3Length(&direction);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//頂点バッファへのコピー
//-----------------------------------------------------------------------------------------------------------------------------------
void Base::copyVertexBuffer(unsigned size, void *sorce, IDirect3DVertexBuffer9 *buffer) {
	void *p = 0;
	buffer->Lock(0, 0, &p, 0);
	memcpy(p, sorce, size);
	buffer->Unlock();
}

//-----------------------------------------------------------------------------------------------------------------------------------
//ある点との線分上で最も近い点を求める
//-----------------------------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 Base::nearestPointOnLine(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXVECTOR3 point)
{
	//始点から終点への線分ベクトルを求める
	D3DXVECTOR3 line = end - start;
	//線分の大きさを求める
	float length = D3DXVec3Length(&line);
	//線分の大きさが0であれば最も近い点は始点及び終点とを戻すことで成立する（0除算防止）
	if (length == 0)return start;

	//始点から点へのベクトルを求める
	D3DXVECTOR3 toPoint = point - start;
	//上記二つのベクトルの内積を求める
	float dot = D3DXVec3Dot(&line, &toPoint);
	//線分の始点から、点から線分へ垂直に下した交点との距離を求める(内積/線分の大きさ）
	float distance = dot / length;
	//始点から交点との距離を判断して、最も近い点を戻す
	if (distance <= 0)
	{
		return start;
	}
	else if (distance >= D3DXVec3Length(&line)) {
		return end;
	}
	else
	{
		D3DXVECTOR3 result;
		D3DXVec3Normalize(&result, &line);
		result = (result * distance) + start;
		return result;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
//ある点から平面までの距離を求める
//-----------------------------------------------------------------------------------------------------------------------------------
float Base::betweenDistancePointAndPlane(D3DXPLANE plane, D3DXVECTOR3 point)
{
	D3DXVECTOR3 normal;
	D3DXVec3Normalize(&normal,&D3DXVECTOR3(plane.a,plane.b,plane.c));
	D3DXPLANE tmp = D3DXPLANE(normal.x, normal.y, normal.z, plane.d);
	return D3DXPlaneDotCoord(&tmp, &point);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//２直線の最短距離
//-----------------------------------------------------------------------------------------------------------------------------------
float Base::between2LineDistance(Line line1, Line line2)
{
	float distance;
	
	D3DXVECTOR3 v1;between2VectorDirection(&v1, line1.start, line1.end);
	D3DXVECTOR3 v2;between2VectorDirection(&v2, line2.start, line2.end);
	D3DXVECTOR3 v12 = line2.start - line1.start;
	D3DXVECTOR3 normal;D3DXVec3Cross(&normal, &v1, &v2);D3DXVec3Normalize(&normal, &normal);
	//距離 = |N・v12|/|N|;
	distance = D3DXVec3Dot(&normal, &v12);

	if (distance < 0)distance *= -1;

	return distance;
}



