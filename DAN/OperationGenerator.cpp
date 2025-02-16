//-----------------------------------------------------------------------------
// メタAIイベント実行クラス [OperationGenerator.cpp]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "OperationGenerator.h"
#include "NavigationMesh.h"
#include "Sound.h"
using namespace aiNS;


//=============================================================================
// 初期化
//=============================================================================
void OperationGenerator::initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster,
	Player* _player, EnemyManager* _enemyManager, TreeManager* _treeManager,
	ItemManager* _itemManager, TelopManager* _telopManager, MarkerRenderer* marker)
{
	data			= _data;
	gameMaster		= _gameMaster;
	player			= _player;
	enemyManager	= _enemyManager;
	treeManager		= _treeManager;
	itemManager		= _itemManager;
	telopManager	= _telopManager;
	markerRenderer = marker;
	wasBossEntried	= false;
	ZeroMemory(wasTelopDisplayed, sizeof(bool) * 3);
	bossEntryTime	= 1000.0f;
}


//=============================================================================
// エネミー動的作成
//=============================================================================
void OperationGenerator::spawnEnemy(enemyNS::ENEMYSET _enemySet)
{
	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	p->isGeneratedBySpawnEvent = true;
	enemyManager->createEnemy(p);
}


//=============================================================================
// エネミーリスポーン
//=============================================================================
void OperationGenerator::respawnEnemy(int _enemyID)
{
	enemyNS::EnemyData* p = enemyManager->findEnemyData(_enemyID);
	p->setUp();
	enemyManager->createEnemy(p);
}


//=============================================================================
// エネミーデジタルツリー襲撃
//=============================================================================
void OperationGenerator::enemyAttaksTree(enemyNS::ENEMYSET _enemySet, Tree* _attackTarget)
{
	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	p->targetTree = _attackTarget;
	enemyManager->createEnemy(p);

	// マーカー
	markerRenderer->attackedTree = &p->targetTree->center;
}


//=============================================================================
// 巨大環境破壊ロボが登場
//=============================================================================
void OperationGenerator::bossEntry(enemyNS::ENEMYSET _enemySet)
{
	wasBossEntried = true;
	bossEntryTime = gameMaster->getGameTime();
	
	// サイレン鳴らす
	PLAY_PARAMETERS playParameter= { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_BossSpawning, false, NULL, false, NULL };
	SoundInterface::SE->playSound(&playParameter);

	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	p->isGeneratedBySpawnEvent = true;
	enemyManager->createEnemy(p);

	// マーカー
	markerRenderer->bossEnemyPosition = &enemyManager->findEnemy(p->enemyID)->center;

	telopManager->playOrder(telopManagerNS::BOSS_ENTRY);
	telopManager->playOrder(telopManagerNS::BOSS_ENTRY2);
}

void OperationGenerator::updateBossEvent()
{
	if (wasBossEntried == false) { return; }
	float time = gameMaster->getGameTime();

	if (wasTelopDisplayed[0] == false)
	{
		wasTelopDisplayed[0] = true;
	}
	if (time < bossEntryTime - 4.0f && wasTelopDisplayed[1] == false)
	{
		wasTelopDisplayed[1] = true;
	}
}


//=============================================================================
// エネミーツリー襲撃マーカーを消す
//=============================================================================
void OperationGenerator::destroyMarkerEnemyAttaksTree()
{
	markerRenderer->attackedTree = NULL;
}