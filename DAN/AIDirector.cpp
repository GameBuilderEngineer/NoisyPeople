//-----------------------------------------------------------------------------
// ���^AI�N���X [AIDirector.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EnemyManager.h"
#include "ImguiManager.h"
using namespace aiNS;

// Static�����o�ϐ�
AIDirector* AIDirector::pointer = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
AIDirector::AIDirector()
{
	pointer = this;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
AIDirector::~AIDirector()
{
	uninitialize();
}


//=============================================================================
// ������
//=============================================================================
void AIDirector::initialize(GameMaster* _gameMaster, LPD3DXMESH _fieldMesh, Player* _player,
	EnemyManager* _enemyManager, TreeManager* _treeManager, ItemManager* _itemManager,
	TelopManager* _telopManager, MarkerRenderer* marker)
{
	gameMaster = _gameMaster;
	fieldMesh = _fieldMesh;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;
	markerRenderer = marker;

	sensor.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager);
	eventMaker.initialize(&data, &opeGenerator, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	opeGenerator.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager, markerRenderer);
	frameCnt = 0;

	// ��̓f�[�^�̏�����
	data.lastSpawnTime[gameMasterNS::PLAYER_1P] =
	data.lastSpawnTime[gameMasterNS::PLAYER_2P] = gameMasterNS::GAME_TIME;
	data.treeDistanceFromPlayer[gameMasterNS::PLAYER_1P] = new float[treeManager->getTreeList().size()];
	data.treeDistanceFromPlayer[gameMasterNS::PLAYER_2P] = new float[treeManager->getTreeList().size()];
	data.lastTimeEnemyAttaksTree = gameMasterNS::GAME_TIME;
	data.ajustTimeEnemyAttaksTree = 0.0f;
	data.wasBossEntried = false;
	data.attackedTree = NULL;

	BoundingSphere temp;
	temp.initialize(NULL, fieldMesh);
	data.fieldRadius = temp.getRadius();
}


//=============================================================================
// �I������
//=============================================================================
void AIDirector::uninitialize()
{
	SAFE_DELETE_ARRAY(data.treeDistanceFromPlayer[gameMasterNS::PLAYER_1P]);
	SAFE_DELETE_ARRAY(data.treeDistanceFromPlayer[gameMasterNS::PLAYER_2P]);
}


//=============================================================================
// ���s
//=============================================================================
void AIDirector::run()
{
	if (frameCnt % 2 == 0)
	{
		sensor.update();
		eventMaker.update();
	}
	// �����\�z�ł�1�t���[�����ɏ����𕪂������ł������A
	// �c���[�U���C�x���g�̃t���O�����̎x��ɂȂ邱�Ƃ���
	// 1�t���[����2�̏������܂Ƃ߂邱�Ƃɂ���
	//else// frameCnt % 2 == 1
	//{
	//}

	frameCnt++;

	//opeGenerator.updateBossEvent();
}


//=============================================================================
// ImGui�ɏo��
//=============================================================================
void AIDirector::outputGUI()
{
#ifdef _DEBUG
#endif
}