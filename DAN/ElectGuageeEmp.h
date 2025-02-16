//======================================
//電力ゲージ(空)クラス[ElectGuageEmp.h]
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
namespace electGuageEmpNS
{
	const int WIDTH_HP_GUAGE_EMP = (512 * WINDOW_WIDTH / 1920);	//緑化率の横幅
	const int HEIGHT_HP_GUAGE_EMP = (32 * WINDOW_HEIGHT / 1080);//緑化率の縦幅
	const D3DXVECTOR3 POSITION_HP_GUAGE_EMP = D3DXVECTOR3(348 * WINDOW_WIDTH / 1920, 90 * WINDOW_HEIGHT / 1080, 0);	//緑化率の表示位置(仮)
	const D3DXVECTOR3 POSITION_HP_GUAGE_EMP_P2 = D3DXVECTOR3(1550 * WINDOW_WIDTH / 1920, 90 * WINDOW_HEIGHT / 1080, 0);	//緑化率の表示位置(仮)
}

//======================================
//クラス定義
//======================================
class ElectGuageEmp :public BasicUI
{
public://メンバー変数
	Sprite * electGuageEmp;
private:
public://メンバー関数
	ElectGuageEmp();
	~ElectGuageEmp();
	void initialize(int playerType);
	void render();
	void update();
};

