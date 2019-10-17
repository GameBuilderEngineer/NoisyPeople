//-----------------------------------------------------------------------------
// ���^AI�N���X [AIDirector.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once
#include "Sensor.h"
#include "EventMaker.h"
#include "OperationGenerator.h"


//=============================================================================
// ���O���
//=============================================================================
namespace aiNS
{
	// �v���C���[��̓f�[�^
	struct PlayerAnalyticalData
	{

	};
	typedef PlayerAnalyticalData PLAYERAD;

	// �G�l�~�[��̓f�[�^
	struct EnemyAnalyticalData
	{

	};
	typedef EnemyAnalyticalData ENEMYAD;

	// �c���[��̓f�[�^
	struct TreeAnalyticalData
	{
		int numBeingAttackedTree;			// �P������Ă���؂̐�

	};
	typedef TreeAnalyticalData TREEAD;

	// �A�C�e����̓f�[�^
	struct ItemAnalyticalData
	{

	};
	typedef ItemAnalyticalData ITEMAD;

	// �C�x���g���X�g
	enum EVENT_LIST
	{
		RESPAWN_ENEMY_AROUND_PLAYER,
		RESPAWN_ITEM,
		ENEMY_ATTACKS_TREE,
	};
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

public:
	void run();							// ���s
	void initialize();					// ������
	void outputGUI();					// ImGui�ɕ\��
};
