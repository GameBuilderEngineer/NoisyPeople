//-----------------------------------------------------------------------------
// ���^AI�p�Z���T�[�N���X [Sensor.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Fuzzy.h"
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
class Sensor
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

	Fuzzy fuzzy;				
	void playerSensor();				// �v���C���[�Z���T�[
	void enemySensor();					// �G�l�~�[�Z���T�[
	void treeSensor();					// �c���[�Z���T�[
	void itemSensor();					// �A�C�e���Z���T�[

public:
	// ������
	void initialize(aiNS::PLAYERAD* _playerAD, aiNS::ENEMYAD* _enemyAD,
		aiNS::TREEAD* _treeAD, aiNS::ITEMAD* _itemAD, GameMaster* _gameMaster, Player* _player,
		EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager);
	// �X�V����
	void update();
};
