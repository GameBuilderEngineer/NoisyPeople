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


//=============================================================================
// ���O���
//=============================================================================
namespace aiNS
{
	// �v���C���[��̓f�[�^
	typedef struct PlayerAnalyticalData
	{
		float lengthBetweenTwoPlayers;		// �v���C���[���m�̋���	
		int	numOfDead;						// ���񎀂�ł��邩

	} PLAYERAD;

	// �G�l�~�[��̓f�[�^
	typedef struct EnemyAnalyticalData
	{
		int numChase;						// �ǐՃX�e�[�g�ɓ����Ă���G�l�~�[�̐�
		int numKilled;						// �|���ꂽ�G�l�~�[�̐�
		int numKilledRecently;				// �ŋߓ|���ꂽ�G�l�~�[�̐�
		float lastSpawnTime;				// �Ō�ɃX�|�[����������
		float shouldSpawnFuzzily;			// �X�|�[�����ׂ����d�݂Â�
	} ENEMYAD;

	// �c���[��̓f�[�^
	typedef struct TreeAnalyticalData
	{
		int numDigital;						// �f�W�^���c���[�̐�
		int numGreen;	
		int numBeingAttackedTree;			// �P������Ă���؂̐�

	} TREEAD;

	// �A�C�e����̓f�[�^
	typedef struct ItemAnalyticalData
	{

	} ITEMAD;

	// �C�x���g���X�g
	enum EVENT_LIST
	{
		SPAWN_ENEMY_AROUND_PLAYER,
		RESPAWN_ENEMY,
		ENEMY_ATTACKS_TREE,
		NUM_EVENT_LIST
	};


	// �G�l�~�\�C�x���g�֌W�萔
	const float RECENT_SECOND = 30.0f;				// �ŋ߂ƌ��Ȃ��b���i���߁��b�j
	const float MANDATORY_SPAWN_INTERVAL = 20.0f;	// �X�|�[���̂��߂̍Œ�o�ߊԊu�b

}


//=============================================================================
//�N���X��`
//=============================================================================
// AI�f�B���N�^�[�͂R�̃��W���[���𑀍삵�Q�[�����̃C�x���g�𓝊�����
// �Z���T�[�̓Q�[���I�u�W�F�N�g�̃p�����[�^����W����
// �C�x���g���[�J�[�̓p�����[�^���甭��������C�x���g�����肷��
// �I�y���[�V�����W�F�l���[�^���e�I�u�W�F�N�g�Ǘ��N���X�ɃC�x���g���s���w������
class AIDirector
{
private:
	aiNS::PLAYERAD playerAD;			// �v���C���[��̓f�[�^
	aiNS::ENEMYAD enemyAD;				// �G�l�~�[��̓f�[�^
	aiNS::TREEAD treeAD;				// �c���[��̓f�[�^
	aiNS::ITEMAD itemAD;				// �A�C�e����̓f�[�^

	Sensor sensor;						// �Z���T�[���W���[��
	EventMaker eventMaker;				// �C�x���g�������W���[��
	OperationGenerator opeGenerator;	// �C�x���g���s���W���[��
	int frameCnt;						// �t���[���J�E���^

	GameMaster* gameMaster;				// �Q�[���Ǘ��I�u�W�F�N�g
	Player* player;						// �v���C���[
	EnemyManager* enemyManager;			// �G�l�~�[�Ǘ��I�u�W�F�N�g
	TreeManager* treeManager;			// �c���[�Ǘ��I�u�W�F�N�g
	ItemManager* itemManager;			// �A�C�e���Ǘ��I�u�W�F�N�g
	TelopManager* telopManager;			// �e���b�v�Ǘ��I�u�W�F�N�g

public:
	// ���s
	void run();
	// ������
	void initialize(GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
	TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager);
	// ImGui�ɕ\��
	void outputGUI();
};
