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
// ������
//=============================================================================
void AIDirector::initialize(GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
	TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager)
{
	gameMaster = _gameMaster;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;

	sensor.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager);
	eventMaker.initialize(&data, &opeGenerator, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	opeGenerator.initialize(&data, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	frameCnt = 0;
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