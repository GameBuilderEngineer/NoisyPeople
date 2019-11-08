//-----------------------------------------------------------------------------
// ���^AI�C�x���g�����N���X [EventMaker.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EventMaker.h"
using namespace aiNS;


//=============================================================================
// ������
//=============================================================================
void EventMaker::initialize(PLAYERAD* _playerAD, ENEMYAD* _enemyAD, TREEAD* _treeAD, ITEMAD* _itemAD,
	OperationGenerator* _opeGenerator, GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
	TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager)
{
	playerAD = _playerAD;
	enemyAD = _enemyAD;
	treeAD = _treeAD;
	itemAD = _itemAD;

	opeGenerator = _opeGenerator;

	gameMaster = _gameMaster;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;
}


//=============================================================================
// �I������
//=============================================================================
void EventMaker::uninitialize()
{
}


//=============================================================================
// �X�V����
//=============================================================================
void EventMaker::update()
{
	// �G�l�~�[���I�쐬�C�x���g�̍쐬
	if (enemyAD->shouldSpawnFuzzily > FUZZY_VALUE_SHOULD_SPAWN)
	{
		makeEventSpawningEnemyAroundPlayer();
	}
}


//=============================================================================
// �G�l�~�[���I�쐬�C�x���g�̍쐬�iSPAWN_ENEMY_AROUND_PLAYER)
//=============================================================================
void EventMaker::makeEventSpawningEnemyAroundPlayer()
{
	enemyNS::ENEMYSET enemySet;
	enemySet.enemyID = enemyManager->issueNewEnemyID();
	enemySet.type = enemyNS::WOLF;
	enemySet.defaultPosition = *player[0].getPosition() + D3DXVECTOR3(rand() % 5, 0.0f, rand() % 5);
	enemySet.defaultDirection = *player[0].getPosition() - enemySet.defaultPosition;
	enemySet.defaultState = stateMachineNS::PATROL;
	opeGenerator->spawnEnemyAroundPlayer(enemySet);
}