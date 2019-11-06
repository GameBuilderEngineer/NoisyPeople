//-----------------------------------------------------------------------------
// �X�e�[�g�}�V��[StateMachine.h]
// Author�FGP12A332 32 �����a�P
// �쐬���F2019/11/3
//-----------------------------------------------------------------------------
#include "StateType.h"
#include "StateMachine.h"
using namespace stateMachineNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
StateMachine::StateMachine(void)
{
	// �J�ڎ��ԃ`�F�b�N��������
	for (int i = 0; i < STATE_MAX; i++)
	{
		checker[i].isCheckTarget = WHETHER_IS_CHECK_TARGET[i];
		checker[i].necesarryElapsedTime = NECESSARY_ELAPSED_TIME[i];
		checker[i].elapsedTime = 0.0f;
		checker[i].executable = true;
	}
}


//=============================================================================
// ������
//=============================================================================
void StateMachine::initialize(int stateNumber)
{
	switch (stateNumber)
	{
	case CHASE:		current = ChaseState::getInstance();	break;
	case PATROL:	current = PatrolState::getInstance();	break;
	case REST:		current = RestState::getInstance();		break;
	case DIE:		current = DieState::getInstance();		break;
	}
}


//=============================================================================
// ���s
//=============================================================================
int StateMachine::run(float frameTime, int enemyType)
{
	old = current;
	current = old->transition(checker, enemyType);

	// �J�ڑO�̃X�e�[�g�ւ̍đJ�ڂɎ��Ԑ������|����i�J�ڎ��ԃ`�F�b�N�j
	// �Z���ԂŃX�e�[�g���z�������s������܂�Ȃ��Ȃ錻�ۂ�����邽��
	if (current != old && checker[old->getNumber()].isCheckTarget)
	{
        checker[old->getNumber()].elapsedTime = 0.0f;
		checker[old->getNumber()].executable = false;
	}
	for (int i = 0; i < stateMachineNS::STATE_MAX; i++)
	{
		if (checker[old->getNumber()].isCheckTarget == false) continue;
		if (checker[i].executable) continue;

		checker[i].elapsedTime += frameTime;
		if (checker[i].elapsedTime > checker[i].necesarryElapsedTime)
		{
			checker[i].elapsedTime = 0.0f;
			checker[i].executable = true;
		}
	}

	return current->getNumber();
}