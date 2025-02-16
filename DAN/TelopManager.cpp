//-----------------------------------------------------------------------------
// テロップマネージャークラス [TelopManager.cpp]
// Author : 新里　将士
// 作成開始日 : 2019/10/31
//-----------------------------------------------------------------------------
// 更新日 : 2020/01/16 【菅野 樹】
//-----------------------------------------------------------------------------
#include "TelopManager.h"

using namespace telopManagerNS;

TelopManager* TelopManager::pointer = NULL;

TelopManager::TelopManager()
{
	//サウンド再生
	announceTelop = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_AnnounceTelop,false,NULL,false,NULL };
}


TelopManager::~TelopManager()
{
	uninitialize();
}

//=============================================================================
// 初期化
//=============================================================================
void TelopManager::initialize()
{
	playFlag = false;
	pointer = this;

	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i] = new Telop;
		telop[i]->setManagerFlag(&playFlag);
	}

	//テロップバーの初期化
	telop[TELOP_INFO_BAR]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BAR),
		SpriteNS::CENTER,
		telopNS::WIDTH_BAR,
		telopNS::HEIGHT_BAR,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ0の初期化
	telop[TELOP_TYPE0]->initialize(
		*textureNS::reference(textureNS::UI_INFO_10),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ1の初期化
	telop[TELOP_TYPE1]->initialize(
		*textureNS::reference(textureNS::UI_INFO_30),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ2の初期化
	telop[TELOP_TYPE2]->initialize(
		*textureNS::reference(textureNS::UI_INFO_50),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ3の初期化
	telop[ENEMY_ASSULT]->initialize(
		*textureNS::reference(textureNS::UI_INFO_ASSULT),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// デジタルツリーが破壊される前に撃退せよ！
	telop[ENEMY_ASSULT2]->initialize(
		*textureNS::reference(textureNS::UI_INFO_ASSULT2),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ4の初期化
	telop[TELOP_TYPE4]->initialize(
		*textureNS::reference(textureNS::UI_INFO_LIMIT1),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ5の初期化
	telop[TELOP_TYPE5]->initialize(
		*textureNS::reference(textureNS::UI_INFO_LIMIT2),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//テロップ6の初期化
	telop[TELOP_TYPE6]->initialize(
		*textureNS::reference(textureNS::UI_INFO_MISSION),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// 巨大環境破壊ロボが現れた！
	telop[BOSS_ENTRY]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BOSS),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// 緑化した木が枯らされる前に撃退せよ！
	telop[BOSS_ENTRY2]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BOSS2),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// デジタルツリーが壊れ周辺が枯れ木に戻った
	telop[WITHER]->initialize(
		*textureNS::reference(textureNS::UI_INFO_WITHER),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// 巨大環境破壊ロボに、周辺を枯れ木に戻された！
	telop[WITHER_BOSS]->initialize(
		*textureNS::reference(textureNS::UI_INFO_WITHER2),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// ショット強化アイテムが島に投下された！
	telop[POWER_UP]->initialize(
		*textureNS::reference(textureNS::UI_INFO_POWERUP),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);
}

//=============================================================================
// 終了処理
//=============================================================================
void TelopManager::uninitialize()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		SAFE_DELETE(telop[i]);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void TelopManager::update(float _frameTime)
{
	//再生中でない
	if (!playFlag)
	{
		//再生命令の要求を確認
		if (orderList[orderNum].request)
		{
			//再生
			play(orderList[orderNum].telopType);
		}
	}
	//各テロップの更新
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i]->update(_frameTime);
	}

	//テロップの更新後再生が完了されていて、現在指す命令が要求状態
	if (!playFlag && orderList[orderNum].request)
	{
		//再生されていたテロップ命令を再生済みにする
		orderList[orderNum].played = true;
		//命令リストを一つ進める
		orderNum = (orderNum+1) % 100;//ラップ0〜99
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void TelopManager::render()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i]->render();
	}
}


//=============================================================================
// 再生処理
//=============================================================================
void TelopManager::play(int type)
{
	if (playFlag == false)
	{
		SoundInterface::SE->playSound(&announceTelop);
		telop[type]->playTelop();
		telop[TELOP_INFO_BAR]->playTelopBar();
		playFlag = true;
	}
}

//=============================================================================
// 再生要求処理
//=============================================================================
void TelopManager::playOrder(int type)
{
	orderList[requestNum].request = true;
	orderList[requestNum].telopType = type;
	requestNum++;
	if (requestNum >= 100)
	{
		//配列のリフレッシュ
		for (int i = 0; i < 100; i++)
		{
			if (orderList[i].played)
			{
				orderList[i].played = false;
				orderList[i].request = false;
				orderList[i].telopType = -1;
			}
		}
		requestNum = 0;
	}

}
