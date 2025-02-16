//-----------------------------------------------------------------------------
// テロップマネージャークラス [TelopManager.h]
// Author : 新里　将士
// 作成開始日 : 2019/10/31
//-----------------------------------------------------------------------------
// 更新日 : 2020/01/16 【菅野 樹】
//-----------------------------------------------------------------------------

#pragma once
#include "Telop.h"
#include "Sprite.h"
#include "Sound.h"
#include "SoundBase.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace telopManagerNS
{
	// テロップタイプ
	enum TYPE
	{
		TELOP_INFO_BAR,
		TELOP_TYPE0,	// 緑化率10%
		TELOP_TYPE1,	// 緑化率30%
		TELOP_TYPE2,	// 緑化率50%
		ENEMY_ASSULT,	// デジタルツリー襲撃
		ENEMY_ASSULT2,	// デジタルツリーが破壊される前に撃退せよ！
		TELOP_TYPE4,	// 活動限界〜
		TELOP_TYPE5,	// 〜感知できなくなってしまった
		TELOP_TYPE6,	// 制限時間以内に島の枯れ木を緑化せよ！
		BOSS_ENTRY,		// 巨大環境破壊ロボが現れた！
		BOSS_ENTRY2,	// 緑化した木が枯らされる前に撃退せよ！
		WITHER,			// デジタルツリーが壊れ周辺が枯れ木に戻った！
		WITHER_BOSS,	// 巨大環境破壊ロボに、周辺を枯れ木に戻された！
		POWER_UP,		// ショット強化アイテムが島に投下された！

		MAX_TELOP
	};

	struct OrderItem
	{
		bool request	= false;	//要求確認
		bool played		= false;	//再生が完了したか
		int telopType	= -1;		//再生するテロップタイプ
	};
}

class TelopManager
{
private:
	Telop* telop[telopManagerNS::MAX_TELOP];

	bool playFlag;

	telopManagerNS::OrderItem	orderList[100];//命令リスト
	int							orderNum	= 0;//現在の再生対象ナンバー
	int							requestNum	= 0;//命令要求シーク

	PLAY_PARAMETERS announceTelop;
	static TelopManager* pointer;
public:
	TelopManager();
	~TelopManager();
	static TelopManager* get() { return pointer; }
	void initialize();
	void uninitialize();
	void update(float _frameTime);
	void render();
	void play(int type);

	void playOrder(int type);//再生要求

};

