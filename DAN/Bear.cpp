//-----------------------------------------------------------------------------
// ベアークラス [Bear.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/21
//-----------------------------------------------------------------------------
#include "Bear.h"
#include "EnemyManager.h"
#include "BearAnimation.h"
#include "TelopManager.h"
using namespace enemyNS;
using namespace stateMachineNS;
using namespace bearNS;


//=============================================================================
// コンストラクタ
//=============================================================================
Bear::Bear(ConstructionPackage constructionPackage) : Enemy(constructionPackage)
{
	// サイズを設定後にオブジェクト⇒姿勢制御の順で初期化
	setSize(D3DXVECTOR3(1.0f, 2.7f, 1.0f) * 14.0f);
	Object::initialize(&position);
	enemyData->defaultDirection = slip(enemyData->defaultDirection, axisY.direction);
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);

	{	// オブジェクトタイプと衝突対象を初期化する
		// 通常のエネミーと異なりパーツだけを衝突対象
		using namespace ObjectType;
		treeCell.type = ENEMY_BEAR;
		treeCell.target = PLAYER | ENEMY | TREE;
	}

	// パーツの作成
	for (int i = 0; i < PARTS_MAX; i++)
	{
		parts[i] = new EnemyParts;
		parts[i]->position = PARTS_OFFSET_POS[i];
		parts[i]->setEnemy(this);
		parts[i]->wasEffectPlayed = false;
		{// オブジェクトタイプと衝突対象の指定
			using namespace ObjectType;
			parts[i]->treeCell.type = ENEMY_PARTS;
			parts[i]->treeCell.target = BULLET;
		}
	}
	parts[BODY]->setRenderer(EnemyManager::bearBodyRenderer);
	parts[ARM_L]->setRenderer(EnemyManager::bearArmLRenderer);
	parts[ARM_R]->setRenderer(EnemyManager::bearArmRRenderer);
	parts[WAIST]->setRenderer(EnemyManager::bearWaistRenderer);
	parts[LEG_L]->setRenderer(EnemyManager::bearLegLRenderer);
	parts[LEG_R]->setRenderer(EnemyManager::bearLegRRenderer);

	parts[BODY]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[ARM_L]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[ARM_R]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[WAIST]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[LEG_L]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[LEG_R]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));

	// パーツに耐久度を設定
	parts[BODY]->durability = 999;
	parts[ARM_L]->durability = 150;
	parts[ARM_R]->durability = 150;
	parts[WAIST]->durability = 999;
	parts[LEG_L]->durability = 999;
	parts[LEG_R]->durability = 999;

	// ゲージ
	gauge = EnemyManager::bearGauge;

	// アニメーションマネージャを初期化
	animationManager = new BearAnimationManager(PARTS_MAX, this, (Object**)&parts[0]);

	// 枯れ木化実行前に設定
	wasDeadAroundStarted = false;
}


//=============================================================================
// デストラクタ
//=============================================================================
Bear::~Bear()
{
	// パーツの破棄
	for (int i = 0; i < PARTS_MAX; i++)
	{
		SAFE_DELETE(parts[i]);
	}
	// アニメーションマネージャの破棄
	SAFE_DELETE(animationManager);
}

#define BEAR_SOUND_MAX (150)
//=============================================================================
// 更新処理
//=============================================================================
void Bear::update(float frameTime)
{
	Enemy::preprocess(frameTime);
	switch (enemyData->state)
	{
	case CHASE:			chase(frameTime);		break;
	case PATROL:		patrol(frameTime);		break;
	case REST:			rest(frameTime);		break;
	case ATTACK_TREE:	attackTree(frameTime);	break;
	case DIE:			die(frameTime);			break;
	}
	Enemy::postprocess(frameTime);

	// パーツアニメーションの更新
	animationManager->update(frameTime);

	// パーツの更新
	cntDestroyParts = 0;
	for (int i = 0; i < PARTS_MAX; i++)
	{
		// 接合部座標の更新
		D3DXVec3TransformCoord(&parts[i]->jointPosition, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &parts[i]->matrixWorld);

		// パーツ破壊チェック
		parts[i]->checkDurability(frameTime);
		if (parts[i]->durability == 0) { cntDestroyParts++; }
	}

	// ゲージの更新
	gauge->getInstance(BearGaugeNS::HP)->update(frameTime, &matrixWorld, enemyData->hp);
	gauge->getInstance(BearGaugeNS::LEFT_ARM)->update(frameTime, &parts[ARM_L]->matrixWorld, parts[ARM_L]->durability);
	gauge->getInstance(BearGaugeNS::RIGHT_ARM)->update(frameTime, &parts[ARM_R]->matrixWorld, parts[ARM_R]->durability);

	// 3Dサウンド（移動音）の再生
	if (animationManager->canPlayMoveSound)
	{
		animationManager->canPlayMoveSound = false;

		PLAY_PARAMETERS tmpPlayParmeters =
			getPlayParameters(FOOT_STEPS_SE, enemyData->type);

		// ここで再生
		for (int i = 0; i < 2; i++)
		{
			tmpPlayParmeters.playerID = i;
			SoundInterface::S3D->playSound(&tmpPlayParmeters);
			// ボリューム
			float distance = D3DXVec3Length(&(position - player[i].position));
			float volume = 0.0f;
			if (distance < BEAR_SOUND_MAX)
			{
				volume = (BEAR_SOUND_MAX - distance) / BEAR_SOUND_MAX;
			}
			// ボリューム調整
			SoundInterface::S3D->SetVolume(tmpPlayParmeters, volume);
		}
	}

	// 枯れ木化の更新
	updateDeadArea(frameTime);
}


//=============================================================================
// 追跡ステート
//=============================================================================
void::Bear::chase(float frameTime)
{
	float distance = between2VectorLength(position, *movingTarget);

	if (distance < 18.0f && canAttack)
	{
		attack();
	}
	Enemy::chase(frameTime);

	// 移動
	move(frameTime);
}


//=============================================================================
// 警戒ステート
//=============================================================================
void::Bear::patrol(float frameTime)
{
	if (onGround && isArraved || isDestinationLost)
	{
		setDebugDestination();		// デバッグ用目的地を設定
		isDestinationLost = false;	// 目的地はロストしていない
		searchPath();
	}
	Enemy::patrol(frameTime);
}


//=============================================================================
// 休憩ステート
//=============================================================================
void::Bear::rest(float frameTime)
{
	Enemy::rest(frameTime);
}


//=============================================================================
// ツリー攻撃ステート
//=============================================================================
void Bear::attackTree(float frameTime)
{
	Enemy::attackTree(frameTime);
}


//=============================================================================
// 死亡ステート
//=============================================================================
void::Bear::die(float frameTime)
{
	if (cntTimeDie > DIE_STATE_RENDERING_TIME)
	{
		// 描画解除
		EnemyManager::bearBodyRenderer->unRegisterObjectByID(parts[BODY]->id);
		EnemyManager::bearArmLRenderer->unRegisterObjectByID(parts[ARM_L]->id);
		EnemyManager::bearArmRRenderer->unRegisterObjectByID(parts[ARM_R]->id);
		EnemyManager::bearWaistRenderer->unRegisterObjectByID(parts[WAIST]->id);
		EnemyManager::bearLegLRenderer->unRegisterObjectByID(parts[LEG_L]->id);
		EnemyManager::bearLegRRenderer->unRegisterObjectByID(parts[LEG_R]->id);
		// 描画リスト更新
		EnemyManager::bearBodyRenderer->updateAccessList();
		EnemyManager::bearArmLRenderer->updateAccessList();
		EnemyManager::bearArmRRenderer->updateAccessList();
		EnemyManager::bearWaistRenderer->updateAccessList();
		EnemyManager::bearLegLRenderer->updateAccessList();
		EnemyManager::bearLegRRenderer->updateAccessList();
	}
	Enemy::die(frameTime);
}


//=============================================================================
// 周辺の枯木化
//=============================================================================
void Bear::deadAround()
{
	aroundDeadTimer = 0.0f;
	isMakingTreeDead = true;
	wasTelopDisplayed = false;

	// DeadAreaの初期化
	deadArea.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * AROUND_DEAD_RANGE;
	deadArea.initialize(&position);
	deadArea.setRadius(1.0f);
	deadArea.mode = GreeningAreaNS::DEAD_MODE;

	// エフェクトの再生
	GreeningAreaNS::DeadingEffect* deadingEffect
		= new GreeningAreaNS::DeadingEffect(&deadArea.position, &deadArea.scale);
	effekseerNS::play(0, deadingEffect);
}


//=============================================================================
// 枯れ木エリアの更新
//=============================================================================
void Bear::updateDeadArea(float frameTime)
{
	if (isMakingTreeDead == false) { return; }

	if (aroundDeadTimer < AROUND_DEAD_TIME)
	{
		aroundDeadTimer += frameTime;

		// 枯木時間終了
		if (aroundDeadTimer > AROUND_DEAD_TIME / 2 && wasTelopDisplayed == false)
		{// タイミングを適当に合わせているだけ
			PLAY_PARAMETERS playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Green_Down};
			SoundInterface::SE->playSound(&playParameters);	//SE再生
			TelopManager* telopManager = TelopManager::get();
			telopManager->playOrder(telopManagerNS::WITHER_BOSS);
			wasTelopDisplayed = true;
		}

		if (aroundDeadTimer > AROUND_DEAD_TIME)
		{
			aroundDeadTimer = AROUND_DEAD_TIME;	// タイマー停止
			isMakingTreeDead = false;
			deadArea.treeCell.remove();			// 衝突空間から離脱
		}
	}

	float rate = aroundDeadTimer / AROUND_DEAD_TIME;
	setDeadArea(UtilityFunction::lerp(1.0f, AROUND_DEAD_RANGE, rate));
}


//=============================================================================
// Getter
//=============================================================================
EnemyParts* Bear::getParts(int type)
{
	return parts[type];
}

GreeningArea* Bear::getDeadArea()
{
	return &deadArea;
}

bool Bear::getIsMakingTreeDead()
{
	return isMakingTreeDead;
}


//=============================================================================
// Setter
//=============================================================================
