//-----------------------------------------------------------------------------
// �E���t�N���X [Wolf.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#include "Wolf.h"
#include "EnemyManager.h"
#include "WolfAnimation.h"
using namespace enemyNS;
using namespace stateMachineNS;
using namespace wolfNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Wolf::Wolf(ConstructionPackage constructionPackage): Enemy(constructionPackage)
{
	// �T�C�Y��ݒ��ɃI�u�W�F�N�g�ˎp������̏��ŏ�����
	//scale *= 2.0f;
	setSize(D3DXVECTOR3(1.0f, 0.87f, 1.0f) * 2.0f);
	Object::initialize(&position);
	enemyData->defaultDirection = slip(enemyData->defaultDirection, axisY.direction);
	postureControl(axisZ.direction, enemyData->defaultDirection, 1);

	// �p�[�c�̍쐬
	for (int i = 0; i < PARTS_MAX; i++)
	{
		parts[i] = new Object;
		parts[i]->position = PARTS_OFFSET_POS[i];
	}

	// �A�j���[�V�����}�l�[�W����������
	animationManager = new WolfAnimationManager(PARTS_MAX, this, &parts[0]);
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
	case CHASE:  chase(frameTime);  break;
	case PATROL: patrol(frameTime); break;
	case REST:   rest(frameTime);   break;
	case DIE:    die(frameTime);    break;
	}
	Enemy::postprocess(frameTime);

	// �p�[�c�A�j���[�V�����̍X�V
	animationManager->update(frameTime);
}


//=============================================================================
// �ǐՃX�e�[�g
//=============================================================================
void::Wolf::chase(float frameTime)
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
void::Wolf::patrol(float frameTime)
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
void::Wolf::rest(float frameTime)
{

}


//=============================================================================
// �c���[�U���X�e�[�g
//=============================================================================
void Wolf::attackTree(float frameTime)
{
	Enemy::attackTree(frameTime);
}


//=============================================================================
// ���S�X�e�[�g
//=============================================================================
void::Wolf::die(float frameTime)
{
	if (cntTimeDie > DIE_STATE_RENDERING_TIME)
	{
		EnemyManager::wolfBodyRenderer->unRegisterObjectByID(parts[BODY]->id);
		EnemyManager::wolfBodyRenderer->updateAccessList();
		EnemyManager::wolfArmRenderer->unRegisterObjectByID(parts[ARM]->id);
		EnemyManager::wolfArmRenderer->updateAccessList();
	}
	Enemy::die(frameTime);
}


//=============================================================================
// Getter
//=============================================================================
Object* Wolf::getParts(int type)
{
	return parts[type];
}


//=============================================================================
// Setter
//=============================================================================
