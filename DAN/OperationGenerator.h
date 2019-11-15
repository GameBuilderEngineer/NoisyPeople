//-----------------------------------------------------------------------------
// ���^AI�C�x���g���s�N���X [OperationGenerator.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "TelopManager.h"

//=============================================================================
// ���O���
//=============================================================================
namespace aiNS
{
	// �O���錾
	struct AnalyticalData;
}


//=============================================================================
//�N���X��`
//=============================================================================
class OperationGenerator
{
private:
	aiNS::AnalyticalData* data;			// ��̓f�[�^

	GameMaster* gameMaster;				// �Q�[���Ǘ��I�u�W�F�N�g
	Player* player;						// �v���C���[
	EnemyManager* enemyManager;			// �G�l�~�[�Ǘ��I�u�W�F�N�g
	TreeManager* treeManager;			// �c���[�Ǘ��I�u�W�F�N�g
	ItemManager* itemManager;			// �A�C�e���Ǘ��I�u�W�F�N�g
	TelopManager* telopManager;			// �e���b�v�Ǘ��I�u�W�F�N�g
	
public:
	// ������
	void initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster,
		Player* _player, EnemyManager* _enemyManager, TreeManager* _treeManager,
		ItemManager* _itemManager, TelopManager* _telopManager);

	// �G�l�~�[���I�쐬�iSPAWN_ENEMY_AROUND_PLAYER)
	void spawnEnemyAroundPlayer(enemyNS::ENEMYSET _enemySet);
	// �G�l�~�[���X�|�[��(RESPAWN_ENEMY)
	void respawnEnemy(int _enemyID);
	// �G�l�~�[�f�W�^���c���[�P���iENEMY_ATTACKS_TREE�j
	void enemyAttaksTree();
};
