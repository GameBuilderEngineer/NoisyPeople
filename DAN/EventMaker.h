//-----------------------------------------------------------------------------
// ���^AI�C�x���g�����N���X [GameMaker.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "OperationGenerator.h"
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


	// �G�l�~�[���I�쐬�C�x���g�̍쐬�iSPAWN_ENEMY_AROUND_PLAYER)
	const float FUZZY_VALUE_SHOULD_SPAWN = 0.8f;
}


//=============================================================================
//�N���X��`
//=============================================================================
class EventMaker
{
private:
	aiNS::PLAYERAD* playerAD;
	aiNS::ENEMYAD* enemyAD;
	aiNS::TREEAD* treeAD;
	aiNS::ITEMAD* itemAD;

	OperationGenerator* opeGenerator;

	GameMaster* gameMaster;				// �Q�[���Ǘ��I�u�W�F�N�g
	Player* player;						// �v���C���[
	EnemyManager* enemyManager;			// �G�l�~�[�Ǘ��I�u�W�F�N�g
	TreeManager* treeManager;			// �c���[�Ǘ��I�u�W�F�N�g
	ItemManager* itemManager;			// �A�C�e���Ǘ��I�u�W�F�N�g
	TelopManager* telopManager;			// �e���b�v�Ǘ��I�u�W�F�N�g

public:
	void initialize(aiNS::PLAYERAD* _playerAD, aiNS::ENEMYAD* _enemyAD,
		aiNS::TREEAD* _treeAD, aiNS::ITEMAD* _itemAD, OperationGenerator* _opeGenerator, 
		GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
		TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager);

	void uninitialize();
	void update();
	void makeEventSpawningEnemyAroundPlayer();
};
