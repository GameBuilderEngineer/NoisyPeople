//-----------------------------------------------------------------------------
// ���^AI�C�x���g���s�N���X [OperationGenerator.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "TelopManager.h"
#pragma once

//=============================================================================
// ���O���
//=============================================================================
namespace aiNS
{
	// �O���錾
	struct	PlayerAnalyticalData;
	typedef PlayerAnalyticalData PLAYERAD;
	struct	EnemyAnalyticalData;
	typedef EnemyAnalyticalData	ENEMYAD;
	struct	TreeAnalyticalData;
	typedef TreeAnalyticalData TREEAD;
	struct	ItemAnalyticalData;
	typedef ItemAnalyticalData ITEMAD;
}


//=============================================================================
//�N���X��`
//=============================================================================
class OperationGenerator
{
private:
	aiNS::PLAYERAD* playerAD;			// �v���C���[��̓f�[�^
	aiNS::ENEMYAD* enemyAD;				// �G�l�~�[��̓f�[�^
	aiNS::TREEAD* treeAD;				// �c���[��̓f�[�^
	aiNS::ITEMAD* itemAD;				// �A�C�e����̓f�[�^

	GameMaster* gameMaster;				// �Q�[���Ǘ��I�u�W�F�N�g
	Player* player;						// �v���C���[
	EnemyManager* enemyManager;			// �G�l�~�[�Ǘ��I�u�W�F�N�g
	TreeManager* treeManager;			// �c���[�Ǘ��I�u�W�F�N�g
	ItemManager* itemManager;			// �A�C�e���Ǘ��I�u�W�F�N�g
	TelopManager* telopManager;			// �e���b�v�Ǘ��I�u�W�F�N�g
	
public:
	void initialize(aiNS::PLAYERAD* _playerAD, aiNS::ENEMYAD* _enemyAD,
		aiNS::TREEAD* _treeAD, aiNS::ITEMAD* _itemAD, GameMaster* _gameMaster,
		Player* _player, EnemyManager* _enemyManager, TreeManager* _treeManager,
		ItemManager* _itemManager, TelopManager* _telopManager);

	// �G�l�~�[���I�쐬�iSPAWN_ENEMY_AROUND_PLAYER)
	void spawnEnemyAroundPlayer(enemyNS::ENEMYSET _enemySet);
	// �G�l�~�[���X�|�[��(RESPAWN_ENEMY)
	void respawnEnemy(int _enemyID);
	// �G�l�~�[�f�W�^���c���[�P���iENEMY_ATTACKS_TREE�j
	void enemyAttaksTree();
};
