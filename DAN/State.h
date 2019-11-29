//-----------------------------------------------------------------------------
// �X�e�[�g [StateMachine.h]
// Author�FGP12A332 32 �����a�P
// �쐬���F2019/7/20
//-----------------------------------------------------------------------------
#pragma once
#include "StateMachine.h"


namespace stateMachineNS
{
	class State
	{
	protected:
		int number;									// �X�e�[�g�ԍ�

	public:
		int getNumber(void) { return number; }		// �X�e�[�g�ԍ����擾
													// �X�e�[�g�J��
		virtual State* transition(stateMachineNS::TransitionTimeChecker* chekcer, Enemy* enemy) = 0;
	};
}
