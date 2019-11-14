//-----------------------------------------------------------------------------
// �X�e�[�g�}�V��[StateMachine.h]
// Author�FGP12A332 32 �����a�P
// �쐬���F2019/11/3
//-----------------------------------------------------------------------------
#pragma once

namespace stateMachineNS
{
	// �X�e�[�g�̎��
	enum ENEMY_STATE
	{
		CHASE,		// �ǐՃX�e�[�g
		PATROL,		// �x���X�e�[�g
		REST,		// �x�e�X�e�[�g
		ATTACK_TREE,// �c���[�U���X�e�[�g
		DIE,			// ���S�X�e�[�g
		STATE_MAX,	// �X�e�[�g�̐�
	};

	// �J�ڎ��ԃ`�F�b�N�Ώ�
	const bool WHETHER_IS_CHECK_TARGET[STATE_MAX] =
	{
		true,		// �ǐՃX�e�[�g
		true,		// �x���X�e�[�g
		true,		// �x�e�X�e�[�g
		false,		// ���S�X�e�[�g
	};

	// �J�ڂɕK�v�Ȍo�ߎ���
	const float NECESSARY_ELAPSED_TIME[STATE_MAX] =
	{
		2.0f,		// �ǐՃX�e�[�g
		2.0f,		// �x���X�e�[�g
		5.0f,		// �x�e�X�e�[�g
		0.0f,		// ���S�X�e�[�g
	};

	// �J�ڎ��ԃ`�F�b�N�\����
	struct TransitionTimeChecker
	{
		bool	isCheckTarget;			// �`�F�b�N�Ώۂ�
		float	necesarryElapsedTime;	// �K�v�o�ߎ��ԁi�Q�Ǝ��ԁj
		float	elapsedTime;			// �o�ߎ���
		bool	executable;				// ���s�\
	};
}

// �O���錾
class State;

//=============================================================================
// �N���X��`
//=============================================================================
class StateMachine
{
private:
	State*		current;				// ���݂̃X�e�[�g
	State*		old;					// ��O�̃X�e�[�g

	// �J�ڎ��ԃ`�F�b�N�I�u�W�F�N�g
	stateMachineNS::TransitionTimeChecker	checker[stateMachineNS::STATE_MAX];

public:
	StateMachine();
	// ������
	void initialize(int stateNumber);
	// �X�e�[�g�}�V���̎��s
	int run(float frameTime, int enemyType);
};
