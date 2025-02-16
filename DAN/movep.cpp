//=============================================================================
//
// キャラクタアニメ処理 [MoveP.cpp]
// Author : HAL東京 周子皓
// Type : C
//=============================================================================

#include "movep.h"
#include <vector>
#include "camera.h"
#include "Direct3D9.h"
#include "ActionEventSeting.h"
#include "Player.h"
#include "Input.h"


//プレイヤー
MOVEP MoveP;
using namespace playerNS;
bool initializedMoveP = false;
float wi = 0.0f;

//=============================================================================
// プレイヤーの初期化処理
//=============================================================================
HRESULT InitMoveP(D3DXVECTOR3 Rot, D3DXVECTOR3 Scl, bool FirstInit)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//初期設定
	MoveP.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MoveP.Rot = Rot;
	MoveP.Quaternion = D3DXQUATERNION(0, 0, 0, 1);
	MoveP.Scl = Scl;
	MoveP.ActionSpeed = 1.25f;
	MoveP.IsDie = false;
	MoveP.IsDeath = false;


	// 初めて初期化
	if(!initializedMoveP)
	{
		//必要 1
		MoveP.pAllocateHier = new CAllocateHierarchy();

		// アニメーションオブジェクトを作る
		MoveP.Animation = (D3DXANIMATION*)calloc(1, sizeof(D3DXANIMATION));
		MoveP.Animation->Owner = "MoveP";

		// xfileを読み込む
		if (FAILED(Load_xFile(MoveP.Animation, MOVEP_MODEL, "MoveP")))
		{
			return E_FAIL;
		}

		// アニメーションセットの名前と設置するコールバックの数などの資料
		// 順番はx fileと一緒にしないといけない
		for (int Set_No = 0; Set_No < MoveP.Animation->AnimeSetNum; Set_No++)
		{
			switch (Set_No)
			{
				//こちらはキャラクターの全部アニメを導入する場所
				//モデルを作る人確認ください
				//.X中のAnimationSet の後ろの名前は動作の名前です
				//それを確認して、編成する
				//									   SetName		AnimSet ShiftTime CurWeightTime
			case MoveP_Idle:
				MoveP.Animation->AnimData[Set_No] = { "Idle",			NULL, 0.3f,	0.0f };
				break;
			case MoveP_Run:
				MoveP.Animation->AnimData[Set_No] = { "Run",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_FireIdle:
				MoveP.Animation->AnimData[Set_No] = { "FireIdle",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_FireRun:
				MoveP.Animation->AnimData[Set_No] = { "FireRun",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_WalkLeft:
				MoveP.Animation->AnimData[Set_No] = { "FireLeft",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_WalkRight:
				MoveP.Animation->AnimData[Set_No] = { "FireRight",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_WalkBackwards:
				MoveP.Animation->AnimData[Set_No] = { "FireBack",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_JumpFire:
				MoveP.Animation->AnimData[Set_No] = { "FireJump",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_Die:
				MoveP.Animation->AnimData[Set_No] = { "Death",	NULL, 0.1f,	0.0f };
				break;

			default:
				break;
			}
		}

		// AnimationCallbackを設置する
		if (FAILED(InitCallbackKeys_MoveP()))
		{
			return E_FAIL;
		}


		for (int Set_No = 0; Set_No < MoveP.Animation->AnimeSetNum; Set_No++)
		{
			// AnimationSetを初期化する
			if (FAILED(InitAnimation(MoveP.Animation, Set_No)))
			{
				return E_FAIL;
			}
		}

		//最初のアニメ設定
		MoveP.Animation->CurrentAnimID = -1;
		ChangeAnimation(MoveP.Animation, MoveP_Idle, 1.0f, false);

		initializedMoveP = true;
	}

	MoveP.Animation->MotionEnd = true;

	return S_OK;

}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void UninitMoveP(void)
{
	// アニメーションをリリース
	UninitAnimation(MoveP.Animation);
	return;

}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void UpdateMoveP(float f_TimeDelta)
{
	Input *input = getInput();
	const BYTE BUTTON_JUMP = virtualControllerNS::B;
	const BYTE BUTTON_ATTACK = virtualControllerNS::R1;

	AnimCallBackHandler CallBackHandler;

	CallBackHandler.AnimPointer = MoveP.Animation;

	if (input->getMouseLButton() || input->getController()[gameMasterNS::PLAYER_1P]->isButton(BUTTON_ATTACK))
	{
		MoveP.IsChange = true;
	}
	else
	{
		MoveP.IsChange = false;
	}

	if ((input->getMouseRButtonTrigger() || input->getController()[gameMasterNS::PLAYER_1P]->wasButton(BUTTON_JUMP)) && !MoveP.IsJump && MoveP.IsGround )
	{
		MoveP.IsJump = true;
		wi = 0.24f;
	}


	//移動制限
	if (!MoveP.IsChange)
	{
		MoveP.IsChangeRun = false;
		MoveP.Animation->NextAnimID = MoveP_Idle;

		if (MoveP.IsGround)
		{

			if (input->isKeyDown('W') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().y < 0.0f)
			{
				MoveP.IsRun = true;
				MoveP.Animation->NextAnimID = MoveP_Run;
			}
			else if (input->isKeyDown('S') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().y > 0.0f)
			{
				MoveP.IsRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkBackwards;
			}
			else if (input->isKeyDown('A') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().x < 0.0f)
			{
				MoveP.IsRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkLeft;
			}
			else if (input->isKeyDown('D') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().x > 0.0f)
			{
				MoveP.IsRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkRight;
			}
			else
			{
				MoveP.IsRun = false;
			}
		}
	}

	if (MoveP.IsChange)
	{
		MoveP.IsRun = false;

		MoveP.Animation->NextAnimID = MoveP_FireIdle;

		if (MoveP.IsGround)
		{

			if (input->isKeyDown('W') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().y < 0.0f)
			{
				MoveP.IsChangeRun = true;
				MoveP.Animation->NextAnimID = MoveP_FireRun;
			}
			else if (input->isKeyDown('S') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().y > 0.0f)
			{
				MoveP.IsChangeRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkBackwards;
			}
			else if (input->isKeyDown('A') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().x < 0.0f)
			{
				MoveP.IsChangeRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkLeft;
			}
			else if (input->isKeyDown('D') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().x > 0.0f)
			{
				MoveP.IsChangeRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkRight;
			}
			else
			{
				MoveP.IsChangeRun = false;
			}
		}
	}

	if (MoveP.IsJump)
	{
		MoveP.Animation->NextAnimID = MoveP_JumpFire;
	}

	if (MoveP.IsDeath)
	{
		MoveP.Animation->AnimController->SetTrackPosition(0, 3.8f);
	}

	if (MoveP.IsJumpEnd)
	{
		if (MoveP.IsGround)
		{
			MoveP.IsJumpEnd = false;
			MoveP.IsJump = false;
			MoveP.IsFireJump = false;
			MoveP.Animation->MotionEnd = true;
		}
		if (!MoveP.IsGround)
		{
			MoveP.Animation->AnimController->SetTrackPosition(0, wi);
		}
	}

	if (MoveP.GroundDistance > 0 && MoveP.GroundDistance < 2.5f)
	{
		wi += 0.01f;
	}

	MovePAnimeCur();
	MovePAnimeNext();

	//アニメ終わったなら、アニメはチェンジできます
	if (MoveP.Animation->MotionEnd)
	{
		MoveP.AnimeChange = true;
	}

	MoveP.RHand = GetBoneMatrix(MoveP.Animation, "mixamorig_RightHand");
	MoveP.RHandPos = D3DXVECTOR3(MoveP.RHand._41, MoveP.RHand._42, MoveP.RHand._43);
	MoveP.LHand = GetBoneMatrix(MoveP.Animation, "mixamorig_LeftHand");
	MoveP.LHandPos = D3DXVECTOR3(MoveP.LHand._41, MoveP.LHand._42, MoveP.LHand._43);
	// アニメーションを更新 
	//必ず入れてください、さもないと、動画が動けない
	UpdateAnimation(MoveP.Animation, f_TimeDelta * MoveP.ActionSpeed);

}

//=============================================================================
// プレイヤーの描画設定
//=============================================================================
void DrawMoveP()
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//         大きさ　回転　　移動　　　　　カプセル判定　　盾のカプセル　　　　　剣のカプセル
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, CapsuleMatrix, CapsuleMatrix_Shield, CapsuleMatrix_Sword;
	D3DMATERIAL9 matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&MoveP.WorldMatrix);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, MoveP.Scl.x, MoveP.Scl.y, MoveP.Scl.z);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationQuaternion(&mtxRot, &MoveP.Quaternion);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxRot);
	
	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, MoveP.Pos.x, MoveP.Pos.y, MoveP.Pos.z);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &MoveP.WorldMatrix);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	pDevice->SetTexture(0, NULL);

	// アニメーションを描画する
	DrawAnimation(MoveP.Animation, &MoveP.WorldMatrix, false);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// プレイヤーの移動設定
//=============================================================================
void MovePMove(void)
{

	return;
}

//=============================================================================
// プレイヤーの次のアニメ設定
//=============================================================================
void MovePAnimeNext(void)
{
	if (MoveP.AnimeChange)
	{
		switch (MoveP.Animation->NextAnimID)
		{
		case MoveP_Idle:
			ChangeAnimation(MoveP.Animation, MoveP_Idle, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_Idle;
			MoveP.Animation->MotionEnd = true;	//そう設定しないと、待つの動作完成しないと、別の動作続けない
			break;
		case MoveP_Run:
			ChangeAnimation(MoveP.Animation, MoveP_Run, 0.8f, false);
			MoveP.Animation->NextAnimID = MoveP_Idle;
			MoveP.Animation->MotionEnd = true;//そう設定しないと、走るの動作完成しないと、別の動作続けない
			break;
		case MoveP_FireIdle:
			ChangeAnimation(MoveP.Animation, MoveP_FireIdle, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//そう設定しないと、走るの動作完成しないと、別の動作続けない
			break;
		case MoveP_FireRun:
			ChangeAnimation(MoveP.Animation, MoveP_FireRun, 0.8f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//そう設定しないと、走るの動作完成しないと、別の動作続けない
			break;
		case MoveP_JumpFire:
			ChangeAnimation(MoveP.Animation, MoveP_JumpFire, 0.5f, false);
			if (MoveP.IsChange)
			{
				MoveP.Animation->NextAnimID = MoveP_FireRun;
			}
			if (!MoveP.IsChange)
			{
				MoveP.Animation->NextAnimID = MoveP_Idle;
			}
			break;
		case MoveP_Die:
			ChangeAnimation(MoveP.Animation, MoveP_Die, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_Idle;
			break;
		case MoveP_WalkRight:
			ChangeAnimation(MoveP.Animation, MoveP_WalkRight, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//そう設定しないと、走るの動作完成しないと、別の動作続けない
			break;
		case MoveP_WalkLeft:
			ChangeAnimation(MoveP.Animation, MoveP_WalkLeft, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//そう設定しないと、走るの動作完成しないと、別の動作続けない
			break;
		case MoveP_WalkBackwards:
			ChangeAnimation(MoveP.Animation, MoveP_WalkBackwards, 1.5f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//そう設定しないと、走るの動作完成しないと、別の動作続けない
			break;
		default:
			break;
		}

		MoveP.AnimeChange = false;

	}
}

//=============================================================================
// プレイヤーの現在のアニメ設定
//=============================================================================
void MovePAnimeCur(void)
{
	Input *input = getInput();

	switch (MoveP.Animation->CurrentAnimID)
	{
	case MoveP_Idle:
		break;
	case MoveP_Run:
		if (!MoveP.IsRun)
		{
			MoveP.Animation->NextAnimID = MoveP_Idle;
			MoveP.AnimeChange = true;
		}
		break;
	case MoveP_FireIdle:
		break;
	case MoveP_FireRun:
		if (!MoveP.IsChangeRun)
		{
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.AnimeChange = true;
		}
		break;
	case MoveP_JumpFire:
		break;
	case MoveP_Die:
		MoveP.Animation->NextAnimID = MoveP_Idle;
		break;
	default:
		break;
	}
}

//=============================================================================
// プレイヤーのポインタを獲得する
//=============================================================================
MOVEP *GetMovePAdr(void)
{
	return &MoveP;
}

//=============================================================================
// CallbackKeysを初期化(プレイヤー)
//=============================================================================
HRESULT InitCallbackKeys_MoveP(void)
{
	ACTIONEVENTSETING *AES = GetAESPAdr();

	// 設置するキーの数
	int SetNum = MoveP.Animation->AnimeSetNum;
	float AnimLoopTime = 0.0f;

	for (int Set_No = 0; Set_No < SetNum; Set_No++)
	{
		// キーデータをクリアする
		memset(&AES->Keydata[0], 0, sizeof(KEYDATA) * Keydata_Max);
		AES->Keydata_No = 0;
		float AnimationFlame = 0.0f;

		switch (Set_No)
		{
		case MoveP_Idle:
			break;
		case MoveP_Run:
			AnimLoopTime = 0.7f;
			AddKeydata(0.01f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(0.37f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(0.95f, MotionEnd);
			break;
		case MoveP_FireIdle:
			break;
		case MoveP_FireRun:
			AnimLoopTime = 0.73f;
			AddKeydata(0.01f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(0.37f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(0.95f, MotionEnd);
			break;
		case MoveP_WalkBackwards:
			AnimLoopTime = 1.4f;
			AddKeydata(0.42f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(1.11f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(0.99f, MotionEnd);
			break;
		case MoveP_WalkLeft:
			AnimLoopTime = 1.03f;
			AddKeydata(0.31f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(0.88f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(0.99f, MotionEnd);
			break;
		case MoveP_WalkRight:
			AnimLoopTime = 1.43f;
			AddKeydata(0.60f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(1.29f / AnimLoopTime, MovePMoveVoice);
			AddKeydata(0.99f, MotionEnd);
			break;
		case MoveP_JumpFire:
			AddKeydata(0.01f, MovePJumpFireStart);
			AddKeydata(0.3f, MovePJumpFireEnd);
			break;
		case MoveP_Die:
			AddKeydata(0.95f, MovePDeath);
			break;
		default:
			continue;
		}

		if (FAILED(
			SetupCallbackKeys(&AES->Keydata[0], MoveP.Animation->AnimData[Set_No].SetName,
				AES->Keydata_No, "MoveP")))
		{
			return E_FAIL;
		}

	}


	return S_OK;
}

