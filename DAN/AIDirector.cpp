//-----------------------------------------------------------------------------
// ���^AI�N���X [AIDirector.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EnemyManager.h"
#include "ImguiManager.h"
using namespace aiNS;


//=============================================================================
// �R���X�g���N�^
//=============================================================================
AIDirector::AIDirector()
{

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
	TelopManager* _telopManager)
{
	gameMaster = _gameMaster;
	fieldMesh = _fieldMesh;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;

	sensor.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager);
	eventMaker.initialize(&data, &opeGenerator, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	opeGenerator.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	frameCnt = 0;

	// ��̓f�[�^�̏�����
	data.lastSpawnTime[gameMasterNS::PLAYER_1P] =
	data.lastSpawnTime[gameMasterNS::PLAYER_2P] = gameMasterNS::GAME_TIME;
	data.treeDistanceFromPlayer[gameMasterNS::PLAYER_1P] = new float[treeManager->getTreeList().size()];
	data.treeDistanceFromPlayer[gameMasterNS::PLAYER_2P] = new float[treeManager->getTreeList().size()];
	data.lastTimeEnemyAttaksTree = gameMasterNS::GAME_TIME;
	data.lastTimeCheckedWeightEnemyAttacksTree = gameMasterNS::GAME_TIME;
	data.wasBossEntried = false;

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
	}
	else// frameCnt % 2 == 1
	{
		eventMaker.update();
	}

	frameCnt++;
}


//=============================================================================
// ImGui�ɏo��
//=============================================================================
void AIDirector::outputGUI()
{
#ifdef _DEBUG
#endif
}