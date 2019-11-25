//-----------------------------------------------------------------------------
// ���^AI�C�x���g���s�N���X [OperationGenerator.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "OperationGenerator.h"
#include "NavigationMesh.h"
using namespace aiNS;


//=============================================================================
// ������
//=============================================================================
void OperationGenerator::initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster, 
	Player* _player, EnemyManager* _enemyManager, TreeManager* _treeManager,
	ItemManager* _itemManager, TelopManager* _telopManager)
{
	data			= _data;
	gameMaster		= _gameMaster;
	player			= _player;
	enemyManager	= _enemyManager;
	treeManager		= _treeManager;
	itemManager		= _itemManager;
	telopManager	= _telopManager;
}


//=============================================================================
// �G�l�~�[���I�쐬
//=============================================================================
void OperationGenerator::spawnEnemy(enemyNS::ENEMYSET _enemySet)
{
	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	enemyManager->createEnemy(p);
}


//=============================================================================
// �G�l�~�[���X�|�[��
//=============================================================================
void OperationGenerator::respawnEnemy(int _enemyID)
{
	enemyNS::EnemyData* p = enemyManager->findEnemyData(_enemyID);
	p->setUp();
	enemyManager->createEnemy(p);
}


//=============================================================================
// �G�l�~�[�f�W�^���c���[�P��
//=============================================================================
void OperationGenerator::enemyAttaksTree(enemyNS::ENEMYSET _enemySet, Tree* _attackTarget)
{
	telopManager->play(telopManagerNS::TELOP_TYPE3);
	enemyNS::EnemyData* p = enemyManager->createEnemyData(_enemySet);
	p->targetTree = _attackTarget;
	enemyManager->createEnemy(p);

}