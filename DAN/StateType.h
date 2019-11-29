//-----------------------------------------------------------------------------
// �X�e�[�g��� [StateType.h]
// Author�FGP12A332 32 �����a�P
// �쐬���F2019/11/3
//-----------------------------------------------------------------------------
#pragma once
#include "State.h"


//=============================================================================
// �ǐ�
//=============================================================================
class ChaseState: public stateMachineNS::State
{
private:
	ChaseState() { number = stateMachineNS::CHASE; }

public:
	static State* getInstance(void)
	{
		static ChaseState instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};


//=============================================================================
// �x��
//=============================================================================
class PatrolState : public stateMachineNS::State
{
private:
	PatrolState() { number = stateMachineNS::PATROL; }

public:
	static State* getInstance(void)
	{
		static PatrolState instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};


//=============================================================================
// �x�e
//=============================================================================
class RestState : public stateMachineNS::State
{
private:
	RestState() { number = stateMachineNS::REST; }

public:
	static State* getInstance(void)
	{
		static RestState instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};


//=============================================================================
// �c���[�U��
//=============================================================================
class AttackTree : public stateMachineNS::State
{
private:
	AttackTree() { number = stateMachineNS::REST; }

public:
	static State* getInstance(void)
	{
		static AttackTree instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};


//=============================================================================
// ���S
//=============================================================================
class DieState : public stateMachineNS::State
{
private:
	DieState() { number = stateMachineNS::DIE; }

public:
	static State* getInstance(void)
	{
		static DieState instance;
		return &instance;
	}
	stateMachineNS::State* transition(stateMachineNS::TransitionTimeChecker* checker, Enemy* enemy);
};
