//-----------------------------------------------------------------------------
// �X�e�[�g��� [StateType.h]
// Author�FGP12A332 32 �����a�P
// �쐬���F2019/11/3
//-----------------------------------------------------------------------------
#pragma once
#include "State.h"
#include "StateMachine.h"



//=============================================================================
// �ǐ�
//=============================================================================
class ChaseState: public State
{
private:
	ChaseState() { number = stateMachineNS::CHASE; }

public:
	static State* getInstance(void)
	{
		static ChaseState instance;
		return &instance;
	}
	State* transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType);
};


//=============================================================================
// �x��
//=============================================================================
class PatrolState : public State
{
private:
	PatrolState() { number = stateMachineNS::PATROL; }

public:
	static State* getInstance(void)
	{
		static PatrolState instance;
		return &instance;
	}
	State* transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType);
};


//=============================================================================
// �x�e
//=============================================================================
class RestState : public State
{
private:
	RestState() { number = stateMachineNS::REST; }

public:
	static State* getInstance(void)
	{
		static RestState instance;
		return &instance;
	}
	State* transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType);
};


//=============================================================================
// ���S
//=============================================================================
class DieState : public State
{
private:
	DieState() { number = stateMachineNS::DIE; }

public:
	static State* getInstance(void)
	{
		static DieState instance;
		return &instance;
	}
	State* transition(stateMachineNS::TransitionTimeChecker* checker, int enemyType);
};
