//-----------------------------------------------------------------------------
// �X�e�[�g��� [StateType.h]
// Author�FGP12A332 32 �����a�P
// �쐬���F2019/11/3
//-----------------------------------------------------------------------------
#include "StateMachine.h"
#include "StateType.h"
using namespace stateMachineNS;

//=============================================================================
// �ǐՏ�Ԃ���̑J��
//=============================================================================
State* ChaseState::transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType)
{
	if (checker[PATROL].executable)
	{ 
		return PatrolState::getInstance();
	}

	return this;
}


//=============================================================================
// �x����Ԃ���̑J��
//=============================================================================
State* PatrolState::transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType)
{
	if (checker[CHASE].executable)
	{
		return ChaseState::getInstance();
	}

	return this;
}


//=============================================================================
// �x�e��Ԃ���̑J��
//=============================================================================
State* RestState::transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType)
{
	return this;
}


//=============================================================================
// ���S��Ԃ���̑J��
//=============================================================================
State* DieState::transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType)
{
	return this;
}
