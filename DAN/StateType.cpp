//-----------------------------------------------------------------------------
// �X�e�[�g��� [StateType.h]
// Author�FGP12A332 32 �����a�P
// �쐬���F2019/11/3
//-----------------------------------------------------------------------------
#include "StateType.h"
#include "Enemy.h"	// �C���N���[�h���Ȃ��ƑO���錾�݂̂̂��߃����o�ɃA�N�Z�X�ł��Ȃ�
using namespace stateMachineNS;

//=============================================================================
// �ǐՏ�Ԃ���̑J��
//=============================================================================
State* ChaseState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	if (enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_1P) == false &&
		enemy->getNoticedOfPlayer(gameMasterNS::PLAYER_2P) == false &&
		checker[PATROL].executable)
	{
		return PatrolState::getInstance();
	}

	return this;
}


//=============================================================================
// �x����Ԃ���̑J��
//=============================================================================
State* PatrolState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		if (checker[CHASE].executable && enemy->getNoticedOfPlayer(i))
		{
			return ChaseState::getInstance();
		}
	}

	return this;
}


//=============================================================================
// �x�e��Ԃ���̑J��
//=============================================================================
State* RestState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	return this;
}


//=============================================================================
// �c���[�U����Ԃ�̑J��
//=============================================================================
State* AttackTree::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	return this;
}


//=============================================================================
// ���S��Ԃ���̑J��
//=============================================================================
State* DieState::transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy)
{
	return this;
}
