//-----------------------------------------------------------------------------
// ���^AI�C�x���g�����N���X [EventMaker.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "OperationGenerator.h"

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
class EventMaker :public Base
{
private:
	aiNS::AnalyticalData* data;			// ��̓f�[�^
	OperationGenerator* opeGenerator;	// �C�x���g���s�I�u�W�F�N�g
	Fuzzy fuzzy;						// �t�@�W�[�_���I�u�W�F�N�g
	GameMaster* gameMaster;				// �Q�[���Ǘ��I�u�W�F�N�g
	Player* player;						// �v���C���[
	EnemyManager* enemyManager;			// �G�l�~�[�Ǘ��I�u�W�F�N�g
	TreeManager* treeManager;			// �c���[�Ǘ��I�u�W�F�N�g
	ItemManager* itemManager;			// �A�C�e���Ǘ��I�u�W�F�N�g
	TelopManager* telopManager;			// �e���b�v�Ǘ��I�u�W�F�N�g

	bool isBossEntried;					// �{�X�o��ς�

public:
	void initialize(aiNS::AnalyticalData* data, OperationGenerator* _opeGenerator,
		GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
		TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager);
	void uninitialize();
	void update();

	// �G�l�~�[���I�쐬�C�x���g�̍쐬�iSPAWN_ENEMY_AROUND_PLAYER)
	void makeEventSpawningEnemyAroundPlayer(int playerType);
	// �G�l�~�[���X�|�[���C�x���g�̍쐬�iRESPAWN_ENEMY)
	void makeEventRespawnEnemy();
	// �G�l�~�[�f�W�^���c���[�P���C�x���g�̍쐬�iENEMY_ATTACKS_TREE�j
	void makeEventEnemyAttaksTree();
	// ������j�󃍃{�o��C�x���g�̍쐬(BOSS_ENTRY)
	int makeEventBossEntry();
	// �p���[�A�b�v�A�C�e���o��C�x���g�̍쐬(POWER_UP_ITEM)
	void makeEventPowerUpItem();
	// ���I�쐬����G�l�~�[�̃p�����[�^�����߂�
	enemyNS::ENEMY_TYPE decideSpawnEnemyType();
	// �P���C�x���g�Ώۃf�W�^���c���[��I�肷��
	int decideAttackTargetTree();
	// ������j�󃍃{�̏ꏊ�����߂�
	D3DXVECTOR3 decideBossEntryPoint();
	// ����W����ɐڒn���W���쐬����
	D3DXVECTOR3 createGroundedPositionFromPivot(D3DXVECTOR3 pivot, float distance, float reduction);
	// XZ���ʏ�̃x�N�g��(����1.0)�������_���ɍ쐬����
	D3DXVECTOR3 createRandomDirectionXZ();
};
