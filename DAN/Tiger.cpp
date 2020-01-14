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
		parts[i] = new EnemyParts;
		parts[i]->position = PARTS_OFFSET_POS[i];
		parts[i]->setEnemy(this);
	}
	parts[BODY]->setRenderer(EnemyManager::tigerBodyRenderer);
	parts[GUN]->setRenderer(EnemyManager::tigerGunRenderer);
	parts[LEG_L]->setRenderer(EnemyManager::tigerLegLRenderer);
	parts[LEG_R]->setRenderer(EnemyManager::tigerLegRRenderer);

	// �A�j���[�V�����}�l�[�W����������
	animationManager = new TigerAnimationManager(PARTS_MAX, this, (Object**)&parts[0]);

	// �o���b�g�}�l�[�W�����쐬
	bulletManager = new TigerBulletManager(player);

	// ��p�I���Ɍ���t����
	tacticsPersonality = rand() % 4;
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


#define TIGER_SOUND_MAX  (40)
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
#ifdef _DEBUG
	if (enemyData->enemyID == debugEnemyID)
	{
		
		if (input->wasKeyPressed('N'))
		{
		}
	}
#endif
	Enemy::postprocess(frameTime);

	// �p�[�c�A�j���[�V�����̍X�V	
	animationManager->update(frameTime);

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
			if (distance < TIGER_SOUND_MAX)
			{
				volume = (TIGER_SOUND_MAX - distance) / TIGER_SOUND_MAX;
			}
			// �{�����[������
			SoundInterface::S3D->SetVolume(tmpPlayParmeters, volume);
		}
	}

	// �o���b�g�̍X�V
	bulletManager->update(frameTime);
}


//=============================================================================
// �ǐՃX�e�[�g
//=============================================================================
void::Tiger::chase(float frameTime)
{
	Enemy::chase(frameTime);

	float distance = between2VectorLength(position, *movingTarget);
	if (distance < 7.0f && canAttack)
	{
		attack();
	}
	else
	{
		// ���Ԃ��J�E���g
		tacticsTime += frameTime;

		if (tacticsTime < 3.0f + tacticsPersonality)
		{
			// �ړ�
			move(frameTime);
		}
		else if (tacticsTime >= 3.0f + tacticsPersonality && tacticsTime < 6.0f + tacticsPersonality)
		{
			animationManager->inactivateAll();
			animationManager->activate(animationManager->getAnimation(tigerAnimNS::SHOT));
			shot();
			moveDirection = player[chasingPlayer].position - position;
		}
		else if (tacticsTime >= 6.0f + tacticsPersonality)
		{
			tacticsTime = 0.0f;
			animationManager->inactivateAll();
		}
	}
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
	Enemy::rest(frameTime);
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
void Tiger::shot()
{
	D3DXVECTOR3 muzzlePosition;			// �e���|�W�V����
	D3DXVECTOR3 connectionPosition;		// �ڑ����|�W�V����

	D3DXVec3TransformCoord(&muzzlePosition, &MUZZLE_POSITION, &parts[GUN]->matrixWorld);
	D3DXVec3TransformCoord(&connectionPosition, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &parts[GUN]->matrixWorld);

	Ray temp;
	//temp.start = connectionPosition;
	//temp.direction = muzzlePosition - connectionPosition;
	temp.start = muzzlePosition;
	temp.direction = player[chasingPlayer].position - muzzlePosition;

	temp.distance = 0.0f;
	temp.rayIntersect(attractorMesh, *attractorMatrix);
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
EnemyParts* Tiger::getParts(int type)
{
	return parts[type];
}

TigerBulletManager* Tiger::getBulletMangaer()
{
	return bulletManager;
}

//=============================================================================
// Setter
//=============================================================================
