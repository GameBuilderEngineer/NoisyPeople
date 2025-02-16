//===================================================================================================================================
//【Credit.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "AbstractScene.h"
//#include "Credit2D.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace creditNS
{
	// プレイヤー人数
	enum {
		PLAYER1,
		PLAYER2,
		NUM_PLAYER,
	};
}

//===================================================================================================================================
//【クレジットシーンクラス】
//===================================================================================================================================
class Credit : public AbstractScene
{
private:
	// クレジット2D
	//Credit2D credit2D;
public:
	Credit();
	~Credit();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	//void render3D();
	void renderUI(LPDIRECT3DDEVICE9 device);

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};
