//-----------------------------------------------------------------------------
// ���^AI�p�Z���T�[�N���X [Sensor.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "Sensor.h"
using namespace aiNS;


//=============================================================================
// ������
//=============================================================================
void Sensor::initialize(aiNS::PLAYERAD* _playerAD, aiNS::ENEMYAD* _enemyAD,
	aiNS::TREEAD* _treeAD, aiNS::ITEMAD* _itemAD, GameMaster* _gameMaster, Player* _player,
	EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager)
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
}


//=============================================================================
// �X�V����
//=============================================================================
void Sensor::update()
{
	// �e�Z���T�[�ŏ����擾
	playerSensor();
	enemySensor();
	treeSensor();
	itemSensor();

	// �G�l�~�[���X�|�[�����ׂ������
	if (gameMaster->getGameTime() - enemyAD->lastSpawnTime > MANDATORY_SPAWN_INTERVAL)
	{
		float tmp1 = fuzzy.reverseGrade((float)enemyAD->numChase, 0.0f, 4.0f);
		float tmp2 = fuzzy.reverseGrade((float)enemyAD->numKilledRecently, 0.0f, 6.0f);
		enemyAD->shouldSpawnFuzzily = fuzzy.AND(tmp1, tmp2);
	}
}


//=============================================================================
// �v���C���[�Z���T�[
//=============================================================================
void Sensor::playerSensor()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		player[i].getPosition();
		player[i].getMatrixWorld();
		player[i].getAxisZ()->direction;
		player[i].getState();
		player[i].getHp();
		player[i].getPower();
	}
}


//=============================================================================
// �G�l�~�[�Z���T�[
//=============================================================================
void Sensor::enemySensor()
{
	enemyAD->numChase = 0;
	enemyAD->numKilled = 0;
	enemyAD->numKilledRecently = 0;

	for (int i = 0; i < enemyManager->getEnemyDataList()->nodeNum; i++)
	{
		enemyNS::EnemyData* enemyData = enemyManager->getEnemyDataList()->getValue(i);

		if (enemyData->state == stateMachineNS::CHASE)
		{// �ǐՃX�e�[�g�ɓ����Ă���G�l�~�[�̐�
			enemyAD->numChase++;
		}
		if (enemyData->isAlive == false)
		{
			// �|���ꂽ�G�l�~�[�̐�
			enemyAD->numKilled++;

			if (enemyData->deadTime - gameMaster->getGameTime() < RECENT_SECOND)
			{// �ŋߓ|���ꂽ�G�l�~�[�̐�
				enemyAD->numKilledRecently++;
			}
		}
	}
}


//=============================================================================
// �c���[�Z���T�[
//=============================================================================
void Sensor::treeSensor()
{

}


//=============================================================================
// �A�C�e���Z���T�[
//=============================================================================
void Sensor::itemSensor()
{

}