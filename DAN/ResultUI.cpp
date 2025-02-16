//============================
//[ResultUI.cpp]
//染矢晃介
//============================

//============================
//インクルード
//============================
#include "ResultUI.h"
#include "Sound.h"

//============================
//【using宣言】
//============================
using namespace resultUiNS;

//============================
//コンストラクタ
//============================
ResultUI::ResultUI()
{
	resultBG = new ResultBG;
	uiCharacter01 = new UIcharacter;//プレイヤー1
	uiCharacter02 = new UIcharacter;;//プレイヤー2
	uiRank = new UIrank;

	//中込追記
	seParameter = { ENDPOINT_VOICE_LIST::ENDPOINT_SE,  SE_LIST::SE_Score2, false,NULL,false,NULL };
	ZeroMemory(wasSEPlayed, sizeof(bool) * 2);
} 

//============================
//デストラクタ
//============================
ResultUI::~ResultUI()
{
	delete resultBG;
	delete uiCharacter01;
	delete uiCharacter02;
	delete uiRank;
}

//============================
//初期化
//============================
void ResultUI::initialize()
{

	//BGM再生フラグをtrueで初期化
	playedBGM = true;

	//フェイズ移行時間の初期化
	time = 0;

	//フェイズの初期化(第一フェイズで)
	resultPhase = PHASE_01;

	//緑化本数
	greeningNum01 = 0;
	greeningNum02 = 0;

	//撃破数
	defeat01 = 0;
	defeat02 = 0;

	//ランクの確定
	score01 = 0;//全体緑化率+緑化本数割る１０+撃破数
	score02 = 0;//全体緑化率+緑化本数割る１０+撃破数

	rank01 = 0;
	rank02 = 0;

	rankType = 0;

	//文字UIの初期化
	uiCharacter01->initialize(PLAYER_01);//プレイヤー1
	uiCharacter02->initialize(PLAYER_02);//プレイヤー2

	//テクスチャUIの初期化
	uiTexture.initialize();

	//ランクの初期化
	uiRank->initialize();
	

	//数字の種類分だけ初期化
	for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX ; i++)
	{
		uiNumber[i].initialize(i);

	}

	//リザルト背景の初期化
	resultBG->initialize();
}

//============================
//描画
//ここもめっちゃk持ち悪い
//============================
void ResultUI::render(bool texFlag)
{
	if (texFlag == true)
	{

		rank01 = decisionRank(greenigPersent, greeningNum01, defeat01);
		rank02 = decisionRank(greenigPersent, greeningNum02, defeat02);

		uiTexture.render(resultPhase);		//テクスチャの描画
		uiCharacter01->render(resultPhase);	//プレイヤー１の文字描画
		uiCharacter02->render(resultPhase);	//プレイヤー2の文字描画

		//全体緑化率の描画
		if (resultPhase == PHASE_02)
		{
			uiNumber[uiNumberNS::GREENIG_PERSENT].render();
		}

		if (resultPhase == PHASE_03)
		{
			uiNumber[uiNumberNS::GREENIG_PERSENT].render();
		}
		//数字表示はフェイズ４から描画
		if (resultPhase == PHASE_04)
		{
			//数字の表示
			for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
			{
				uiNumber[i].render();
			}
		}
		//ランク表示フェーズ
		if (resultPhase == PHASE_05)
		{
			//ランク描画
			uiRank->render(rank01, rank02);
			//数字の表示
			for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
			{
				uiNumber[i].render();
			
			}
		}
	}
}

//============================
//更新
//フレームタイムをもたせて一定時間経過でフェイズを更新
//============================
void ResultUI::update(float flameTime,bool texStart)
{
	if (texStart==true)
	{
		time += flameTime;

	//フェイズの更新
	if (time > PHASE_TIME)
	{
		resultPhase = PHASE_02;
		uiNumber[uiNumberNS::GREENIG_PERSENT].update(greenigPersent);
	}
	if (time > 5.0f)
	{
		resultPhase= PHASE_03;
	}

	// 中込追記部分（サウンド）
	if (time > 8.2f && wasSEPlayed[0] == false)
	{
		wasSEPlayed[0] = true;
		SoundInterface::SE->playSound(&seParameter);
	}
	if (time > 9.7f && wasSEPlayed[1] == false)
	{
		wasSEPlayed[1] = true;
		SoundInterface::SE->playSound(&seParameter);
	}

	if (time > 10.0f)
	{
		resultPhase = PHASE_04;
		//数字
		int score[uiNumberNS::GREENIG_PERSENT] = { greeningNum01,greeningNum02,defeat01,defeat02 };
		for (int i = 0; i < uiNumberNS::GREENIG_PERSENT; i++)
		{
			uiNumber[i].update(score[i]);
			}
			
		}
		if (time > 12.0f)
		{
			resultPhase = PHASE_05;
			//ランク
			uiRank->update(rank01, rank02);
			//再生
		if (playedBGM)
		{
			decidionBGM();
		}
	}

	// 中込追記部分（ボイス）
	if (time > 14.0f && gameMaster->wasFinishVoicePlayed[gameMasterNS::PLAYER_1P] == false && score > 70)
	{
		PLAY_PARAMETERS voiceFinish = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, S3D_LIST::Voice_Male_Finish, false, NULL, true, gameMasterNS::PLAYER_1P };
		SoundInterface::S3D->playSound(&voiceFinish);
		gameMaster->wasFinishVoicePlayed[gameMasterNS::PLAYER_1P] = true;

	}
	if (time > 16.0f && gameMaster->wasFinishVoicePlayed[gameMasterNS::PLAYER_2P] == false && score > 70)
	{
		PLAY_PARAMETERS voiceFinish = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, S3D_LIST::Voice_Female_Finish, false, NULL, true, gameMasterNS::PLAYER_2P };
		SoundInterface::S3D->playSound(&voiceFinish);
		gameMaster->wasFinishVoicePlayed[gameMasterNS::PLAYER_2P] = true;
	}

		uiCharacter01->update(resultPhase,PLAYER_01,time);	//プレイヤー１の文字更新
		uiCharacter02->update(resultPhase,PLAYER_02,time);	//プレイヤー１の文字更新
		uiTexture.update(resultPhase);		//テクスチャの更新
	}
	
}

//============================
//終了処理
//============================
void ResultUI::uninitialize()
{

	uiTexture.uninitialize();

	//数字の種類分だけ終了処理
	for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
	{
		uiNumber[i].uninitialize();
	}
}

//=============================
//鳴らすBGMを決定し鳴らす処理
//=============================
void ResultUI::decidionBGM()
{
	//どちらかが70を超えていたならクリア
	if (greenigPersent >=70)
	{
		PLAY_PARAMETERS playParameters = { 0 };//同時に再生したい数
		//再生する曲の指定サウンドID,ループ,スピードNULLでしない,基本false,基本NULL,フィルターを使うか使わないか
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Clear, true,1.0f,false,NULL};//BGMの設定
		 //再生
		SoundInterface::BGM->playSound(&playParameters);
	}
	//どちらも70以下なら失敗
	else if(greenigPersent<70)
	{
		PLAY_PARAMETERS playParameters = { 0 };//同時に再生したい数
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Failed, true,1.0f,false,NULL};//BGMの設定
		SoundInterface::BGM->playSound(&playParameters);//再生
	}
	//1度再生したらフラグをフォルス
	playedBGM = false;
}

//========================================
//ランク計算関数
//引数：全体緑化率、緑化本数、撃破数
//========================================
int ResultUI::decisionRank(int greenigPersent, int greeningNum, int defeat)
{
	//ランクの確定(仮)
	 score = greenigPersent + (greeningNum / 10 )+ defeat;//全体緑化率+緑化本数割る１０+撃破数(仮）
	 rankType =0;
	//**********************************************
	//70以下でFAILED,70以上８０以下CLEARE         //
	//80以上90以下GREAT,90以上でEXCELLENT         //
	//**********************************************

 //プレイヤー１のランクの確定 直しポイント
  if (score < 70)
   {
   	rankType = uiRankNS::UIRANK_TYPE::FAILED;
	return rankType;
   }
   else if (score >= 70 && score < 80)
   {
   	rankType = uiRankNS::UIRANK_TYPE::CLEARE;
	return rankType;
   }
   else if (score >= 70 && score < 90)
   {
   	rankType = uiRankNS::UIRANK_TYPE::GREAT;
	return rankType;
   }
   else if(score >= 90)
   {
   	rankType = uiRankNS::UIRANK_TYPE::EXCELLENT;
	return rankType;
   }
	
}