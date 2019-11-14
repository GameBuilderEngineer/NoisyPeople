//-----------------------------------------------------------------------------
// �E���t�N���X [Wolf.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#include "Wolf.h"
using namespace enemyNS;
using namespace stateMachineNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Wolf::Wolf(StaticMesh* _staticMesh, EnemyData* _enemyData): Enemy(_staticMesh, _enemyData)
{

}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Wolf::~Wolf()
{

}


//=============================================================================
// �X�V����
//=============================================================================
void Wolf::update(float frameTime)
{
	Enemy::preprocess(frameTime);
	switch (enemyData->state)
	{
	case CHASE:  chase();  break;
	case PATROL: patrol(); break;
	case REST:   rest();   break;
	case DIE:    die();    break;
	}
	Enemy::update(frameTime);
}


//=============================================================================
// �ǐՃX�e�[�g
//=============================================================================
void::Wolf::chase()
{
	if (onGround && isArraved || isDestinationLost)
	{
		// �K�؂ȃv���C���[���ړ��^�[�Q�b�g�ɐݒ肷��
		if (isNoticedPlayer[gameMasterNS::PLAYER_1P] && isNoticedPlayer[gameMasterNS::PLAYER_2P] == false)
		{
			setMovingTarget(&player[gameMasterNS::PLAYER_1P].position);
		}
		else if (isNoticedPlayer[gameMasterNS::PLAYER_1P] == false && isNoticedPlayer[gameMasterNS::PLAYER_2P])
		{
			setMovingTarget(&player[gameMasterNS::PLAYER_2P].position);
		}
		else if (isNoticedPlayer[gameMasterNS::PLAYER_1P] && isNoticedPlayer[gameMasterNS::PLAYER_2P])
		{
			float distance1 = between2VectorLength(position, player[gameMasterNS::PLAYER_1P].position);
			float distance2 = between2VectorLength(position, player[gameMasterNS::PLAYER_2P].position);
			if (distance1 < distance2) { setMovingTarget(&player[gameMasterNS::PLAYER_1P].position); }
			else { setMovingTarget(&player[gameMasterNS::PLAYER_1P].position); }
		}

		isDestinationLost = false;	// �ړI�n�̓��X�g���Ă��Ȃ�
		shouldSearch = true;		// �p�X�T�[�`���s
	}

	setMove(true);
}


//=============================================================================
// �x���X�e�[�g
//=============================================================================
void::Wolf::patrol()
{
	if (onGround && isArraved || isDestinationLost)
	{
		setDebugDestination();		// �f�o�b�O�p�ړI�n��ݒ�
		isDestinationLost = false;	// �ړI�n�̓��X�g���Ă��Ȃ�
		shouldSearch = true;		// �p�X�T�[�`���s
	}

	setMove(true);
}


//=============================================================================
// �x�e�X�e�[�g
//=============================================================================
void::Wolf::rest()
{

}


//=============================================================================
// ���S�X�e�[�g
//=============================================================================
void::Wolf::die()
{

}


//=============================================================================
// Getter
//=============================================================================


//=============================================================================
// Setter
//=============================================================================
