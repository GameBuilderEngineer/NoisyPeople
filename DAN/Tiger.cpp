//-----------------------------------------------------------------------------
// �^�C�K�[�N���X [Tiger.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#include "Tiger.h"
#include "EnemyManager.h"
#include "TigerAnimation.h"
using namespace enemyNS;
using namespace stateMachineNS;
using namespace tigerNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Tiger::Tiger(ConstructionPackage constructionPackage) : Enemy(constructionPackage)
{
	// �T�C�Y��ݒ��ɃI�u�W�F�N�g�ˎp������̏��ŏ�����
	setSize(D3DXVECTOR3(2.0f, 2.8, 2.0f));
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
	animationManager = new TigerAnimationManager(PARTS_MAX, this, &parts[0]);

	// �o���b�g�}�l�[�W���Ƀ����_���[��ݒ�
	bulletManager = new TigerBulletManager(EnemyManager::tigerBulletRenderer);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Tiger::~Tiger()
{
	// �p�[�c�̔j��
	for (int i = 0; i < PARTS_MAX; i++)
	{
		SAFE_DELETE(parts[i]);
	}
	// �A�j���[�V�����}�l�[�W���̔j��
	SAFE_DELETE(animationManager);
	// �o���b�g�}�l�[�W���̔j��
	SAFE_DELETE(bulletManager);
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
	//if (enemyData->enemyID == debugEnemyID)
	//{
	//	if (input->wasKeyPressed('N'))
	//	{
	//		shot(&player[0]);
	//	}
	//}
	Enemy::postprocess(frameTime);

	// �p�[�c�A�j���[�V�����̍X�V
	animationManager->update(frameTime);

	// �o���b�g�̍X�V
	muzzlePosition = center;// ����
	bulletManager->update(frameTime);
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
// �V���b�g
//=============================================================================
void Tiger::shot(Player* target)
{
	Ray temp;
	temp.start = muzzlePosition;
	temp.direction = target->center - muzzlePosition;
	D3DXVec3Normalize(&temp.direction, &temp.direction);
	bulletManager->shoot(temp);
}


//=============================================================================
// ���S�X�e�[�g
//=============================================================================
void::Tiger::die(float frameTime)
{
	if (cntTimeDie > DIE_STATE_RENDERING_TIME)
	{
		EnemyManager::tigerBodyRenderer->unRegisterObjectByID(parts[BODY]->id);
		EnemyManager::tigerBodyRenderer->updateAccessList();
		EnemyManager::tigerGunRenderer->unRegisterObjectByID(parts[GUN]->id);
		EnemyManager::tigerGunRenderer->updateAccessList();
		EnemyManager::tigerLegLRenderer->unRegisterObjectByID(parts[LEG_L]->id);
		EnemyManager::tigerLegLRenderer->updateAccessList();
		EnemyManager::tigerLegRRenderer->unRegisterObjectByID(parts[LEG_R]->id);
		EnemyManager::tigerLegRRenderer->updateAccessList();
	}
	Enemy::die(frameTime);
}


//=============================================================================
// Getter
//=============================================================================
Object* Tiger::getParts(int type)
{
	return parts[type];
}


//=============================================================================
// Setter
//=============================================================================
