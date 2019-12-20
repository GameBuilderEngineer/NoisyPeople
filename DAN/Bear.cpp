//-----------------------------------------------------------------------------
// �x�A�[�N���X [Bear.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#include "Bear.h"
#include "EnemyManager.h"
#include "BearAnimation.h"
using namespace enemyNS;
using namespace stateMachineNS;
using namespace bearNS;


//=============================================================================
// �R���X�g���N�^
//=============================================================================
Bear::Bear(ConstructionPackage constructionPackage) : Enemy(constructionPackage)
{
	// �T�C�Y��ݒ��ɃI�u�W�F�N�g�ˎp������̏��ŏ�����
	setSize(D3DXVECTOR3(1.0f, 2.7f, 1.0f) * 14.0f);
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
	animationManager = new BearAnimationManager(PARTS_MAX, this, &parts[0]);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Bear::~Bear()
{

}


//=============================================================================
// �X�V����
//=============================================================================
void Bear::update(float frameTime)
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

	// 3D�T�E���h�i�ړ����j�̍Đ�
	if (animationManager->canPlayMoveSound)
	{
		animationManager->canPlayMoveSound = false;

		// �����ōĐ�
		// �v���C���[�̍��W�͎擾�ł��Ă���
		player[gameMasterNS::PLAYER_1P].position;
		player[gameMasterNS::PLAYER_2P].position;
		// ���낵���B
	}
}


//=============================================================================
// �ǐՃX�e�[�g
//=============================================================================
void::Bear::chase(float frameTime)
{
	float distance = between2VectorLength(position, *movingTarget);

	if (distance < 18.0f && canAttack)
	{
		attack();
	}
	Enemy::chase(frameTime);
}


//=============================================================================
// �x���X�e�[�g
//=============================================================================
void::Bear::patrol(float frameTime)
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
void::Bear::rest(float frameTime)
{
	Enemy::rest(frameTime);
}


//=============================================================================
// �c���[�U���X�e�[�g
//=============================================================================
void Bear::attackTree(float frameTime)
{
	Enemy::attackTree(frameTime);
}


//=============================================================================
// ���S�X�e�[�g
//=============================================================================
void::Bear::die(float frameTime)
{
	if (cntTimeDie > DIE_STATE_RENDERING_TIME)
	{
		// �`�����
		EnemyManager::bearBodyRenderer->unRegisterObjectByID(parts[BODY]->id);
		EnemyManager::bearArmLRenderer->unRegisterObjectByID(parts[ARM_L]->id);
		EnemyManager::bearArmRRenderer->unRegisterObjectByID(parts[ARM_R]->id);
		EnemyManager::bearWaistRenderer->unRegisterObjectByID(parts[WAIST]->id);
		EnemyManager::bearLegLRenderer->unRegisterObjectByID(parts[LEG_L]->id);
		EnemyManager::bearLegRRenderer->unRegisterObjectByID(parts[LEG_R]->id);
		// �`�惊�X�g�X�V
		EnemyManager::bearBodyRenderer->updateAccessList();
		EnemyManager::bearArmLRenderer->updateAccessList();
		EnemyManager::bearArmRRenderer->updateAccessList();
		EnemyManager::bearWaistRenderer->updateAccessList();
		EnemyManager::bearLegLRenderer->updateAccessList();
		EnemyManager::bearLegRRenderer->updateAccessList();
	}
	Enemy::die(frameTime);
}


//=============================================================================
// Getter
//=============================================================================
Object* Bear::getParts(int type)
{
	return parts[type];
}

//=============================================================================
// Setter
//=============================================================================
