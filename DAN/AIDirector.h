//-----------------------------------------------------------------------------
// ���^AI�N���X [AIDirector.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "Sensor.h"
#include "EventMaker.h"
#include "OperationGenerator.h"
#include "GameMaster.h"
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "TelopManager.h"
#include "LinkedList.h"
#include "Marker.h"


//=============================================================================
// ���O���
//=============================================================================
namespace aiNS
{
	// �C�x���g���X�g
	enum EVENT_LIST
	{
		SPAWN_ENEMY_AROUND_PLAYER,		// �v���C���[�̎���ɓ��I�ɃG�l�~�[���X�|�[������
		RESPAWN_ENEMY,					// ��x�|�����G�l�~�[�������ʒu�Ƀ��X�|�[������
		ENEMY_ATTACKS_TREE,				// �G�l�~�[���c���[���P������C�x���g����������
		BOSS_ENTRY,						// ������j�󃍃{���o��
		POWER_UP_ITEM,					// �p���[�A�b�v�A�C�e���o��
		NUM_EVENT_LIST
	};

	//--------
	// Sensor
	//--------
	// �v���C���[
	enum PLAYER_CONDITION
	{
		GOOD,
		NORMAL,
		BAD
	};

	// �G�l�~�[
	const float RECENT_SECOND = 30.0f;				// �ŋ߂ƌ��Ȃ��b���i���߁��b�j�ŋߓ|�����G�l�~�[�̐��W�v�Ɏg�p

	//-------------
	// Event Maker
	//-------------
	// �C�x���g�����̕]���l�萔�i���L�l������΃C�x���g�����s�����j
	const float WEIGHT_SPAWN = 0.6f;				// SPAWN_ENEMY_AROUND_PLAYER
	const float WEIGHT_RESPAWN = 0.7f;				// RESPAWN_ENEMY
	const float WEIGHT_ENEMY_ATTACKS_TREE = 0.5f;	// ENEMY_ATTACKS_TREE

	const float MANDATORY_SPAWN_INTERVAL = 35.0f;				// �G�l�~�[�X�|�[���̂��߂̍Œ�o�ߊԊu�b
	const float MANDATOEY_INTERVAL_ENEMY_ATTAKS_TREE = 20.0f;	// �c���[�P���C�x���g�̍Œ�o�ߊԊu�b

	// �c���[�P���C�x���g�̗�O�ƂȂ�؂�ID
	const int EXCEPTION_TREE_MAX = 3;// ��
	const int EXCEPTION_TREE_ID[EXCEPTION_TREE_MAX] =
	{
		10000,
		10001,
		10002
	};

	// �P���C�x���g�Ώۃf�W�^���c���[
	struct AttackCandidate
	{
		int treeID;
		float distanceToNearPlayer;
		float score;
	};

	//--------------------------------------------------------
	// ���^AI��̓f�[�^
	// Sensor, EventMaker, OperationGenerator�ŋ��L�����p����
	//--------------------------------------------------------
	struct AnalyticalData
	{
		// �v���C���[
		float lengthBetweenTwoPlayers;						// �v���C���[���m�̋���	
		int playerCondition[gameMasterNS::PLAYER_NUM];		// �v���C���[�̃R���f�B�V����

		// �G�l�~�[
		int numChase;										// �ǐՃX�e�[�g�ɓ����Ă���G�l�~�[�̐�
		int numTreeAttackingEnemy;							// 
		int numChasingPlayer[gameMasterNS::PLAYER_NUM];		// �ǐՂ��Ă���v���C���[�̐�
		int numKilled;										// �|���ꂽ�G�l�~�[�̐�
		int numKilledRecently;								// �ŋߓ|���ꂽ�G�l�~�[�̐�
		float lastSpawnTime[gameMasterNS::PLAYER_NUM];		// �Ō�ɃX�|�[����������

		// �c���[
		int numDigital;										// �f�W�^���c���[�̐�
		int numGreen;										// �Ή�����Ă���؂̐�	
		int numBeingAttackedTree;							// �P������Ă���؂̐�
		float* treeDistanceFromPlayer[2];
		Tree* attackedTree;									// �P������Ă����

		// �t�B�[���h
		float fieldRadius;									// �t�B�[���h���a�T�C�Y

		// �C�x���g
		float lastTimeEnemyAttaksTree;						// �Ō�Ƀc���[�P���C�x���g��������������
		float ajustTimeEnemyAttaksTree;						// �c���[�P���C�x���g�̒�������
		bool wasBossEntried;								// ���j�󃍃{���o�ꂵ����
		bool wasPowerUpEntried;								// �p���[�A�b�v�A�C�e�����o�ꂵ����
		float powerUpEntryAdjustTime;						// �p���[�A�b�v�A�C�e�����o�ꂷ�邽�߂̒�������

		// �C�x���g�����̕]���l�i0.0�`1.0�j
		float weightSpawn[gameMasterNS::PLAYER_NUM];		// SPAWN_ENEMY_AROUND_PLAYER
		float weightRespawn;								// RESPAWN_ENEMY
		float weightEnemyAttacksTree;						// ENEMY_ATTACKS_TREE
	};
}


//=============================================================================
// AI�f�B���N�^�[�͂R�̃��W���[���𑀍삵�Q�[�����̃C�x���g�𓝊�����
// *�Z���T�[�̓Q�[���I�u�W�F�N�g�̃p�����[�^����W����
// *�C�x���g���[�J�[�̓p�����[�^���甭��������C�x���g�����肷��
// *�I�y���[�V�����W�F�l���[�^���e�I�u�W�F�N�g�Ǘ��N���X�ɃC�x���g���s���w������
//=============================================================================
class AIDirector
{
private:
	aiNS::AnalyticalData data;			// ��̓f�[�^

	//Sensor sensor;						// �Z���T�[���W���[��
	//EventMaker eventMaker;				// �C�x���g�������W���[��
	//OperationGenerator opeGenerator;	// �C�x���g���s���W���[��
	int frameCnt;						// �t���[���J�E���^


	GameMaster* gameMaster;				// �Q�[���Ǘ��I�u�W�F�N�g
	LPD3DXMESH fieldMesh;				// �t�B�[���h���b�V��
	Player* player;						// �v���C���[
	EnemyManager* enemyManager;			// �G�l�~�[�Ǘ��I�u�W�F�N�g
	TreeManager* treeManager;			// �c���[�Ǘ��I�u�W�F�N�g
	ItemManager* itemManager;			// �A�C�e���Ǘ��I�u�W�F�N�g
	TelopManager* telopManager;			// �e���b�v�Ǘ��I�u�W�F�N�g
	MarkerRenderer*	markerRenderer;		// �}�[�J�[
	static AIDirector* pointer;			// �|�C���^

public:
	//��
	Sensor sensor;						// �Z���T�[���W���[��
	EventMaker eventMaker;				// �C�x���g�������W���[��
	OperationGenerator opeGenerator;	// �C�x���g���s���W���[��

	AIDirector();
	~AIDirector();

	// ������
	void initialize(GameMaster* _gameMaster, LPD3DXMESH _fieldMesh, Player* _player,
		EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager,
		TelopManager* _telopManager, MarkerRenderer* marker);
	// �I������
	void uninitialize();
	// ���s
	void run();
	// ImGui�ɕ\��
	void outputGUI();

	static AIDirector* get() { return pointer; }
};

