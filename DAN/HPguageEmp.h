//======================================
//HPゲージ(空)クラス[HPguageEmp.h]
//染矢晃介
//======================================
#pragma once

//======================================
//インクルード
//======================================
#include "BasicUI.h"
#include "VirtualController.h"

//======================================
//名前空間
//======================================
namespace hpGuageEmpNS
{
	const int WIDTH_HP_GUAGE_EMP = (500 * WINDOW_WIDTH / 1920);	//タイマーフレームの横幅
	const int HEIGHT_HP_GUAGE_EMP = (30 * WINDOW_HEIGHT / 1080);	//タイマーフレームの縦幅
	const D3DXVECTOR3 POSITION_HP_GUAGE_EMP = D3DXVECTOR3(350 * WINDOW_WIDTH / 1920, 50 * WINDOW_HEIGHT / 1080, 0);	//タイマーフレームの表示位置(仮)
	const D3DXVECTOR3 POSITION_HP_GUAGE_EMP_P2 = D3DXVECTOR3(1575 * WINDOW_WIDTH / 1920, 50 * WINDOW_HEIGHT / 1080, 0);
}

//======================================
//クラス定義
//======================================
class HPguageEmp :public BasicUI
{
public://メンバー変数
	Sprite * hpGuageEmp;
private:
public://メンバー関数
	HPguageEmp();
	~HPguageEmp();
	void initialize(int playerType);
	void render();
	void update();
};
