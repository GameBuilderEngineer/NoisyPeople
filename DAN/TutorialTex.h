//============================
// [TutorialTex.h]
// 染矢　晃介
//============================
#pragma once

//============================
//インクルード
//============================
#include "Sprite.h"
#include "TextureLoader.h"

//============================
//名前空間 定数
//============================
namespace tutorialTex
{
	enum
	{
		TUTORIAL_2D_SCENE01,
		TUTORIAL_2D_SCENE02,
		TUTORIAL_2D_SCENE03_1,
		TUTORIAL_2D_SCENE03_2,
		TUTORIAL_2D_STANDBY1,
		TUTORIAL_2D_STANDBY2,
		TUTORIAL_2D_SCENE_MAX		//2Dのチュートリアルシーンの総数
	};

	const int WIDTH = WINDOW_WIDTH;								//横幅
	const int HEIGHT = WINDOW_HEIGHT;							//縦幅
	const D3DXVECTOR3 POSITION = D3DXVECTOR3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);//位置
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//回転
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//色

	const D3DXVECTOR3 POSITION_1P = D3DXVECTOR3(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, 0);
	const D3DXVECTOR3 POSITION_2P = D3DXVECTOR3(WINDOW_WIDTH / 4 * 3, WINDOW_HEIGHT / 2, 0);
}
//============================
//クラス定義
//============================
class TutorialTex
{
private: //メンバー変数
	Sprite * tutorial1;
	Sprite * tutorial2;
	Sprite * tutorial3_1;
	Sprite * tutorial3_2;
	Sprite * tutorialStandby1;
	Sprite * tutorialStandby2;
	bool renderFlag[4];
public://メンバー変数
	int nextPage;			//チュートリアルのページを管理

public: //メンバー関数
	void initialize();		//初期
	void uninitialize();	//終了
	void render();			//描画
	void update();			//更新
	Sprite* getHalfSlide(int playerNo, int number);
	void setRender(int index, bool setting);
	bool getRenderFlag(int index) {
		return renderFlag[index];
	}
};
