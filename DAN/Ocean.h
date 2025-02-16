//===================================================================================================================================
//【Ocean.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/07
// [更新日]2019/11/07
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Object.h"
#include "StaticMeshRenderer.h"

//===================================================================================================================================
//【オーシャンクラス】
//===================================================================================================================================
class Ocean :public Base
{
private:
	Object*								object;
	LPDIRECT3DVERTEXDECLARATION9		declaration;			//頂点宣言
	LPDIRECT3DDEVICE9					device;
	LPD3DXEFFECT						effect;
	StaticMesh*							staticMesh;

	D3DXMATRIX*							worldMatrix;			//ワールドマトリックス配列
	LPDIRECT3DVERTEXBUFFER9				matrixBuffer;			//ワールドマトリックスバッファ

	LPDIRECT3DTEXTURE9					bumpTexture;
	D3DXVECTOR2							waveMove;				//波の移動位置
	float								height;					//波の高さ
	float								deltaHeight;			//波の増減値
	D3DXMATRIX							worldInverseTranspose;	//ワールド逆転置行列


	bool needUpdate;

public:
	Ocean();
	~Ocean();
	void initialize();
	void update();
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);

};

