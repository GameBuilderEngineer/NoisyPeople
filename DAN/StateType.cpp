//-----------------------------------------------------------------------------
// ステート種類 [StateType.h]
// Author：GP12A332 32 中込和輝
// 作成日：2019/11/3
//-----------------------------------------------------------------------------
#include "StateType.h"
#include "Enemy.h"	// インクルードしないと前方宣言のみのためメンバにアクセスできない
using namespace stateMachineNS;

//=============================================================================
// 追跡状態からの遷移
//=============================================================================
State* ChaseState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	if (enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_1P) == false &&
		enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_2P) == false &&
		checker[PATROL].executable)
	{
		enemy->preparePatrol();
		return PatrolState::getInstance();
	}

	if (enemy->getEnemyData()->hp == 0)
	{
		enemy->prepareDie();
		return DieState::getInstance();
	}

	return this;
}


//=============================================================================
// 警戒状態からの遷移
//=============================================================================
State* PatrolState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		if (checker[CHASE].executable && enemy->getNoticedOfPlayer(i))
		{
			enemy->prepareChase();
			return ChaseState::getInstance();
		}
	}

	if (enemy->getEnemyData()->hp == 0)
	{
		enemy->prepareDie();
		return DieState::getInstance();
	}

	return this;
}


//=============================================================================
// 休憩状態からの遷移
//=============================================================================
State* RestState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		if (checker[CHASE].executable && enemy->getNoticedOfPlayer(i))
		{
			enemy->prepareChase();
			return ChaseState::getInstance();
		}
	}

	if (enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_1P) == false &&
		enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_2P) == false &&
		enemy->getIsPayingNewAttention() &&
		checker[PATROL].executable)
	{
		enemy->preparePatrol();
		return PatrolState::getInstance();
	}

	if (enemy->getEnemyData()->hp == 0)
	{
		enemy->prepareDie();
		return DieState::getInstance();
	}

	return this;
}


//=============================================================================
// ツリー攻撃状態からの遷移
//=============================================================================
State* AttackTree::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		if (checker[CHASE].executable && enemy->getNoticedOfPlayer(i))
		{
			enemy->prepareChase();
			return ChaseState::getInstance();
		}
	}

	if (enemy->getEnemyData()->targetTree == NULL &&
		enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_1P) == false &&
		enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_2P) == false &&
		checker[PATROL].executable)
	{
		enemy->preparePatrol();
		return PatrolState::getInstance();
	}

	if (enemy->getEnemyData()->hp == 0)
	{
		enemy->prepareDie();
		return DieState::getInstance();
	}

	return this;
}


//=============================================================================
// 死亡状態からの遷移
//=============================================================================
State* DieState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	return this;
}
