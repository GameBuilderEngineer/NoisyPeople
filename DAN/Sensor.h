//-----------------------------------------------------------------------------
// ���^AI�p�Z���T�[�N���X [Sensor.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Fuzzy.h"


//=============================================================================
// ���O���
//=============================================================================
namespace aiNS
{
	// �O���錾
	struct AnalyticalData;

	// enemySensor()
	const float RECENT_SECOND = 30.0f;				// �ŋ߂ƌ��Ȃ��b���i���߁��b�j
	const float MANDATORY_SPAWN_INTERVAL = 20.0f;	// �X�|�[���̂��߂̍Œ�o�ߊԊu�b
}


//=============================================================================
//�N���X��`
//=============================================================================
class Sensor
{
private:
	aiNS::AnalyticalData* data;			// ��̓f�[�^

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
	void initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster, Player* _player,
		EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager);

	// �X�V����
	void update();
};
