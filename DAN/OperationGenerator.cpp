//-----------------------------------------------------------------------------
// ���^AI�C�x���g���s�N���X [OperationGenerator.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "OperationGenerator.h"

using namespace aiNS;


//=============================================================================
// ������
//=============================================================================
void OperationGenerator::initialize(PLAYERAD* _playerAD, ENEMYAD* _enemyAD, TREEAD* _treeAD, ITEMAD* _itemAD,
	GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager, TreeManager* _treeManager,
	ItemManager* _itemManager, TelopManager* _telopManager)
{
	playerAD = _playerAD;
	enemyAD = _enemyAD;
	treeAD = _treeAD;
	itemAD = _itemAD;

	gameMaster = _gameMaster;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;
}


//=============================================================================
// �G�l�~�[���I�쐬�iSPAWN_ENEMY_AROUND_PLAYER)
//=============================================================================
void OperationGenerator::spawnEnemyAroundPlayer(enemyNS::ENEMYSET _enemySet)
{
	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	enemyManager->createEnemy(p);
}


//=============================================================================
// �G�l�~�[���X�|�[��(RESPAWN_ENEMY)
//=============================================================================
void OperationGenerator::respawnEnemy(int _enemyID)
{
	enemyNS::EnemyData* p = enemyManager->findEnemyData(_enemyID);
	p->setUp();
	enemyManager->createEnemy(p);
}


//=============================================================================
// �G�l�~�[�f�W�^���c���[�P���iENEMY_ATTACKS_TREE�j
//=============================================================================
void OperationGenerator::enemyAttaksTree()
{

}
