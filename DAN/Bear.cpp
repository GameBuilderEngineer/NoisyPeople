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

	{	// �I�u�W�F�N�g�^�C�v�ƏՓˑΏۂ�����������
		// �ʏ�̃G�l�~�[�ƈقȂ�p�[�c�������ՓˑΏ�
		using namespace ObjectType;
		treeCell.type = ENEMY_BEAR;
		treeCell.target = PLAYER | ENEMY | TREE;
	}

	// �p�[�c�̍쐬
	for (int i = 0; i < PARTS_MAX; i++)
	{
		parts[i] = new EnemyParts;
		parts[i]->position = PARTS_OFFSET_POS[i];
		parts[i]->setEnemy(this);
		parts[i]->wasEffectPlayed = false;
		{// �I�u�W�F�N�g�^�C�v�ƏՓˑΏۂ̎w��
			using namespace ObjectType;
			parts[i]->treeCell.type = ENEMY_PARTS;
			parts[i]->treeCell.target = BULLET;
		}
	}
	parts[BODY]->setRenderer(EnemyManager::bearBodyRenderer);
	parts[ARM_L]->setRenderer(EnemyManager::bearArmLRenderer);
	parts[ARM_R]->setRenderer(EnemyManager::bearArmRRenderer);
	parts[WAIST]->setRenderer(EnemyManager::bearWaistRenderer);
	parts[LEG_L]->setRenderer(EnemyManager::bearLegLRenderer);
	parts[LEG_R]->setRenderer(EnemyManager::bearLegRRenderer);

	parts[BODY]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[ARM_L]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[ARM_R]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[WAIST]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[LEG_L]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	parts[LEG_R]->setSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));

	// �p�[�c�ɑϋv�x��ݒ�
	parts[BODY]->durability = 999;
	parts[ARM_L]->durability = 50;
	parts[ARM_R]->durability = 50;
	parts[WAIST]->durability = 999;
	parts[LEG_L]->durability = 999;
	parts[LEG_R]->durability = 999;

	// �A�j���[�V�����}�l�[�W����������
	animationManager = new BearAnimationManager(PARTS_MAX, this, (Object**)&parts[0]);

	// �͂�؉����s�O�ɐݒ�
	wasDeadAroundStarted = false;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Bear::~Bear()
{
	// �p�[�c�̔j��
	for (int i = 0; i < PARTS_MAX; i++)
	{
		SAFE_DELETE(parts[i]);
	}
	// �A�j���[�V�����}�l�[�W���̔j��
	SAFE_DELETE(animationManager);
}

#define BEAR_SOUND_MAX (150)
//=============================================================================
// �X�V����
//=============================================================================
void Bear::update(float frameTime)
{
	Enemy::preprocess(frameTime);
	switch (enemyData->state)
	{
	case CHASE:			chase(frameTime);		break;
	case PATROL:		patrol(frameTime);		break;
	case REST:			rest(frameTime);		break;
	case ATTACK_TREE:	attackTree(frameTime);	break;
	case DIE:			die(frameTime);			break;
	}
	Enemy::postprocess(frameTime);

	// �p�[�c�A�j���[�V�����̍X�V
	animationManager->update(frameTime);

	for (int i = 0; i < PARTS_MAX; i++)
	{
		// �ڍ������W�̍X�V
		D3DXVec3TransformCoord(&parts[i]->jointPosition, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &parts[i]->matrixWorld);

		// �p�[�c�j��`�F�b�N
		parts[i]->checkDurability();
	}

	// 3D�T�E���h�i�ړ����j�̍Đ�
	if (animationManager->canPlayMoveSound)
	{
		animationManager->canPlayMoveSound = false;

		PLAY_PARAMETERS tmpPlayParmeters =
			getPlayParameters(FOOT_STEPS_SE, enemyData->type);

		// �����ōĐ�
		for (int i = 0; i < 2; i++)
		{
			tmpPlayParmeters.playerID = i;
			SoundInterface::S3D->playSound(&tmpPlayParmeters);
			// �{�����[��
			float distance = D3DXVec3Length(&(position - player[i].position));
			float volume = 0.0f;
			if (distance < BEAR_SOUND_MAX)
			{
				volume = (BEAR_SOUND_MAX - distance) / BEAR_SOUND_MAX;
			}
			// �{�����[������
			SoundInterface::S3D->SetVolume(tmpPlayParmeters, volume);
		}
	}

	// �͂�؉��̍X�V
	updateDeadArea(frameTime);
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

	// �ړ�
	move(frameTime);
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
// ���ӂ͖̌؉�
//=============================================================================
void Bear::deadAround()
{
	aroundDeadTimer = 0.0f;
	isMakingTreeDead = true;

	deadArea.position = position;
	deadArea.initialize(&position);
	deadArea.setSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	deadArea.setRadius(1.0f);
	deadArea.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * AROUND_DEAD_RANGE;
	deadArea.mode = GreeningAreaNS::DEAD_MODE;
	//�G�t�F�N�g�̍Đ�
	GreeningAreaNS::DeadingEffect* deadingEffect
		= new GreeningAreaNS::DeadingEffect(&deadArea.position, &deadArea.scale);
	effekseerNS::play(0, deadingEffect);
}


//=============================================================================
// �͂�؃G���A�̍X�V
//=============================================================================
void Bear::updateDeadArea(float frameTime)
{
	deadArea.update(frameTime);

	if (isMakingTreeDead != false) { return; }

	if (aroundDeadTimer < AROUND_DEAD_TIME)
	{
		aroundDeadTimer += frameTime;
		// �͖؎��ԏI��
		if (aroundDeadTimer > AROUND_DEAD_TIME)
		{
			aroundDeadTimer = AROUND_DEAD_TIME;	// �^�C�}�[��~
			isMakingTreeDead = false;
		}
	}

	float rate = aroundDeadTimer / AROUND_DEAD_TIME;
	setDeadArea(UtilityFunction::lerp(1.0f, AROUND_DEAD_RANGE, rate));
}


//=============================================================================
// Getter
//=============================================================================
EnemyParts* Bear::getParts(int type)
{
	return parts[type];
}

GreeningArea* Bear::getDeadArea()
{
	return &deadArea;
}

bool Bear::getIsMakingTreeDead()
{
	return isMakingTreeDead;
}


//=============================================================================
// Setter
//=============================================================================
