//-----------------------------------------------------------------------------
// �^�C�K�[�N���X [Tiger.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#include "Tiger.h"
using namespace enemyNS;
using namespace stateMachineNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Tiger::Tiger(ConstructionPackage constructionPackage) : Enemy(constructionPackage)
{
	// �T�C�Y��ݒ��ɃI�u�W�F�N�g�ˎp������̏��ŏ�����
	setSize(D3DXVECTOR3(1.0f, 2.9, 1.0f));
	Object::initialize(&position);
	enemyData->defaultDirection = slip(enemyData->defaultDirection, axisY.direction);
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Tiger::~Tiger()
{

}


//=============================================================================
// �X�V����
//=============================================================================
void Tiger::update(float frameTime)
{
	Enemy::preprocess(frameTime);
	switch (enemyData->state)
	{
	case CHASE:  chase(frameTime);  break;
	case PATROL: patrol(frameTime); break;
	case REST:   rest(frameTime);   break;
	case DIE:    die(frameTime);    break;
	}
	Enemy::postprocess(frameTime);
}


//=============================================================================
// �ǐՃX�e�[�g
//=============================================================================
void::Tiger::chase(float frameTime)
{

	float distance = between2VectorLength(position, *movingTarget);

	if (distance < 7.0f && canAttack)
	{
		attack();
	}
	Enemy::chase(frameTime);
}


//=============================================================================
// �x���X�e�[�g
//=============================================================================
void::Tiger::patrol(float frameTime)
{
	if (onGround && isArraved || isDestinationLost)
	{
		setDebugDestination();		// �f�o�b�O�p�ړI�n��ݒ�
		isDestinationLost = false;	// �ړI�n�̓��X�g���Ă��Ȃ�
		searchPath();
	}
	Enemy::patrol(frameTime);
}


//=============================================================================
// �x�e�X�e�[�g
//=============================================================================
void::Tiger::rest(float frameTime)
{

}


//=============================================================================
// �c���[�U���X�e�[�g
//=============================================================================
void Tiger::attackTree(float frameTime)
{
	Enemy::attackTree(frameTime);
}


//=============================================================================
// ���S�X�e�[�g
//=============================================================================
void::Tiger::die(float frameTime)
{
	Enemy::die(frameTime);
}


//=============================================================================
// Getter
//=============================================================================


//=============================================================================
// Setter
//=============================================================================
