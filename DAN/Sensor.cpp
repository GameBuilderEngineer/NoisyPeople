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
void Sensor::initialize(aiNS::AnalyticalData* _data, GameMaster* _gameMaster, Player* _player,
	EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager)
{
	data = _data;
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
}


//=============================================================================
// �v���C���[�Z���T�[
//=============================================================================
void Sensor::playerSensor()
{
	D3DXVECTOR3 vecBetweenPlayers =
		*player[gameMasterNS::PLAYER_1P].getPosition() - *player[gameMasterNS::PLAYER_2P].getPosition();

	// �v���C���[���m�̋���	
	data->lengthBetweenTwoPlayers = D3DXVec3Length(&vecBetweenPlayers);

	// �v���C���[�̃R���f�B�V����
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		// HP�̎c�ʂ�0.0�`1.0�ɕϊ�
		float fuzzyHp = fuzzy.grade((float)player[i].getHp(), 0, playerNS::MAX_HP);

		if (fuzzyHp > 0.7f)
		{
			data->playerCondition[i] = PLAYER_CONDITION::GOOD;
		}
		else if(fuzzyHp > 0.4f)
		{
			data->playerCondition[i] = PLAYER_CONDITION::NORMAL;
		}
		else if(fuzzyHp > 0.2f)
		{
			// �d�͂̎c�ʂ�0.0�`1.0�ɕϊ�
			float fuzzyPower = fuzzy.grade((float)player[i].getPower(), 0, playerNS::MAX_POWER);

			if (fuzzyPower > 0.33f)
			{
				data->playerCondition[i] = PLAYER_CONDITION::NORMAL;
			}
			else
			{
				data->playerCondition[i] = PLAYER_CONDITION::BAD;
			}
		}
		else
		{
			data->playerCondition[i] = PLAYER_CONDITION::BAD;
		}
	}
}


//=============================================================================
// �G�l�~�[�Z���T�[
//=============================================================================
void Sensor::enemySensor()
{
	data->numChase = 0;
	data->numChasingPlayer[gameMasterNS::PLAYER_1P] = 0;
	data->numChasingPlayer[gameMasterNS::PLAYER_2P] = 0;
	data->numKilled = 0;
	data->numKilledRecently = 0;

	for (int i = 0; i < enemyManager->getEnemyDataList()->nodeNum; i++)
	{
		enemyNS::EnemyData* enemyData = enemyManager->getEnemyDataList()->getValue(i);

		if (enemyData->isObjectExists && enemyData->state == stateMachineNS::CHASE)
		{// �ǐՃX�e�[�g�ɓ����Ă���G�l�~�[�̐�
			data->numChase++;
			if (enemyManager->findEnemy(enemyData->enemyID)->getChasingPlayer() == gameMasterNS::PLAYER_1P)
			{
				data->numChasingPlayer[gameMasterNS::PLAYER_1P]++;
			}
			else
			{
				data->numChasingPlayer[gameMasterNS::PLAYER_2P]++;
			}
		}
		if (enemyData->isAlive == false)
		{
			// �|���ꂽ�G�l�~�[�̐�
			data->numKilled++;

			if (enemyData->deadTime - gameMaster->getGameTime() < RECENT_SECOND)
			{// �ŋߓ|���ꂽ�G�l�~�[�̐�
				data->numKilledRecently++;
			}
		}
	}
}


//=============================================================================
// �c���[�Z���T�[
//=============================================================================
void Sensor::treeSensor()
{
	data->numDigital = 0;
	data->numGreen = 0;
	data->numBeingAttackedTree = 0;

	treeNS::TreeData* treeData;
	for (size_t i = 0; i < treeManager->getTreeList().size(); i++)
	{
		treeData = treeManager->getTreeList()[i]->getTreeData();
		if (treeData->type == treeNS::DIGITAL_TREE)
		{
			data->numDigital++;
		}
		if (treeData->greenState == treeNS::GREEN)
		{
			data->numGreen++;
		}
		if (treeData->isAttaked)
		{
			data->numBeingAttackedTree++;
		}
	}
}


//=============================================================================
// �A�C�e���Z���T�[
//=============================================================================
void Sensor::itemSensor()
{

}