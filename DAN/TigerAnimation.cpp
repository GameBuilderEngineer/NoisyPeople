//-----------------------------------------------------------------------------
// TIGER�A�j���[�V����[TigerAnimation.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/12
//-----------------------------------------------------------------------------
#include "Tiger.h"
#include "TigerAnimation.h"
using namespace::tigerNS;
using namespace::tigerAnimNS;


#pragma region [Animation]
///////////////////////////////////////////////////////////////////////////////
////////////////////////////// �A�j���[�V���� /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// ��]�͐���̗��֐���D�悵�O���[�o���ϐ��Ƃ��ėp�ӂ���
// .h��.cpp�̉������Ȃ��Ȃ�R�[�h��������₷���Ȃ邽��

//=============================================================================
// �f�t�H���g�A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 DEFAULT_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// �R���X�g���N�^
DefaultAnimation::DefaultAnimation(DWORD _flag):PartsAnimation(_flag)
{
	numKeyFrame = sizeof(DEFAULT_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// �X�V
void DefaultAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(DEFAULT_ANIMATION_BODY);
	rot[GUN] = updateRotation(DEFAULT_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(DEFAULT_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(DEFAULT_ANIMATION_LEG_R);
}


//=============================================================================
// �ړ��A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 MOVE_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.03f, 0.01f, -0.05f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.03f, -0.01f, 0.05f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

static const D3DXVECTOR3 MOVE_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.01f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.01f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

static const D3DXVECTOR3 MOVE_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.6f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.45f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.45f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.6f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// �R���X�g���N�^
MoveAnimation::MoveAnimation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(MOVE_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// �X�V
void MoveAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(MOVE_ANIMATION_BODY);
	rot[GUN] = updateRotation(MOVE_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(MOVE_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(MOVE_ANIMATION_LEG_R);
}


//=============================================================================
// �U���A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 ATTACK_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.02f, 0.0f, 0.0f),
	D3DXVECTOR3(0.02f, 0.0f, 0.0f),
	D3DXVECTOR3(0.02f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// �R���X�g���N�^
AttackAnimation::AttackAnimation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(ATTACK_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// �X�V
void AttackAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(ATTACK_ANIMATION_BODY);
	rot[GUN] = updateRotation(ATTACK_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(ATTACK_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(ATTACK_ANIMATION_LEG_R);

}


//=============================================================================
// ���S�A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 DEAD_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.01f, 0.01f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.01f, -0.01f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.03f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, -0.03f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// �R���X�g���N�^
DeadAnimation::DeadAnimation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(DEAD_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// �X�V
void DeadAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(DEAD_ANIMATION_BODY);
	rot[GUN] = updateRotation(DEAD_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(DEAD_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(DEAD_ANIMATION_LEG_R);
}


//=============================================================================
// �V���b�g�A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 SHOT_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.1f, 0.0f, 0.0f),
	D3DXVECTOR3(0.1f, 0.0f, 0.0f),
	D3DXVECTOR3(0.1f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 SHOT_ANIMATION_GUN[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 SHOT_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 SHOT_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

// �R���X�g���N�^
ShotAnimation::ShotAnimation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(SHOT_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// �X�V
void ShotAnimation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(SHOT_ANIMATION_BODY);
	rot[GUN] = updateRotation(SHOT_ANIMATION_GUN);
	rot[LEG_L] = updateRotation(SHOT_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(SHOT_ANIMATION_LEG_R);
}
#pragma endregion


#pragma region [Manager]
///////////////////////////////////////////////////////////////////////////////
/////////////////////////  �A�j���[�V�����}�l�[�W���[ /////////////////////////
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
// �R���X�g���N�^
//=============================================================================
TigerAnimationManager::TigerAnimationManager(int partsMax, Object* parent, Object** parts)
	: PartsAnimationManager(partsMax, parent, parts)
{
	animation = new PartsAnimation*[ANIMATION_TYPE_MAX];

	// �A�j���[�V�������쐬
	animation[DEFAULT] = new DefaultAnimation(ANIMATION_FLAG[DEFAULT]);
	animation[MOVE] = new MoveAnimation(ANIMATION_FLAG[MOVE]);
	animation[ATTACK] = new AttackAnimation(ANIMATION_FLAG[ATTACK]);
	animation[DEAD] = new DeadAnimation(ANIMATION_FLAG[DEAD]);
	animation[SHOT] = new ShotAnimation(ANIMATION_FLAG[SHOT]);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
TigerAnimationManager::~TigerAnimationManager()
{
	for (int i = 0; i < ANIMATION_TYPE_MAX; i++)
	{
		SAFE_DELETE(animation[i]);
	}
	SAFE_DELETE_ARRAY(animation);
}


//=============================================================================
// �X�V����
//=============================================================================
void TigerAnimationManager::update(float frameTime)
{
	// ��]�̍X�V
	for (int i = 0; i < ANIMATION_TYPE_MAX; i++)
	{
		if (flagState & animation[i]->flag)
		{
			animation[i]->update(rot, parts, frameTime * ANIMATION_SPEED[i]);
		}
	}

	// �s��v�Z
	culcPartsMatrix();
}


//=============================================================================
// �p�[�c�̃}�g���N�X���v�Z����
// Object�N���X��update()�ł̓N�I�[�^�j�I���x�[�X�ł̉�]�v�Z���s���Ă��邽��,
// ��]�����Ɋ������. update()�̑�p�Ƃ��Ă��̊֐��Ń��[���h�ϊ����s��.
// �܂��e�I�u�W�F�N�g��e�p�[�c�̃��[���h�ϊ����q�p�[�c�ɔ��f�����Ă���.
//=============================================================================
void TigerAnimationManager::culcPartsMatrix()
{
	for (int i = 0; i < partsMax; i++)
	{
		// �p�[�c�̃��[���h�}�g���N�X������������
		D3DXMatrixIdentity(&parts[i]->matrixWorld);
		// ��]�x�N�g�������]�}�g���N�X���쐬����
		D3DXMatrixRotationYawPitchRoll(&parts[i]->matrixRotation, rot[i].y, rot[i].x, rot[i].z);
		// �ʒu����ʒu�}�g���N�X���쐬����
		D3DXMatrixTranslation(&parts[i]->matrixPosition, parts[i]->position.x, parts[i]->position.y, parts[i]->position.z);
		// ��]�}�g���N�X�����[���h�}�g���N�X�Ɋ|����
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[i]->matrixRotation);
		// �ʒu�}�g���N�X�����[���h�}�g���N�X�Ɋ|����
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[i]->matrixPosition);

		if (i == BODY)
		{
			// �p�[�c�̃��[���h�}�g���N�X�ɐe�I�u�W�F�N�g�̃��[���h�}�g���N�X���|����
			D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parent->matrixWorld);
		}
		else
		{
			// �q�p�[�c�̃��[���h�}�g���N�X�ɐe�p�[�c�̃}�g���N�X���|����
			D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[0]->matrixWorld);
		}
	}
}


//=============================================================================
// �f�t�H���g�A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void TigerAnimationManager::switchDefault()
{
	// �U���A�j���[�V�������I�����Ă��Ȃ��ƍĐ�����Ȃ�
	if (animation[ATTACK]->wasPlayedToEnd)
	{
		inactivate(animation[ATTACK]);
		activate(animation[DEFAULT]);
	}
}


//=============================================================================
// �ړ��A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void TigerAnimationManager::switchMove()
{
	// �U���A�j���[�V�������I�����Ă��Ȃ��ƍĐ�����Ȃ�
	if (animation[ATTACK]->wasPlayedToEnd)
	{
		inactivate(animation[ATTACK]);
		activate(animation[MOVE]);
	}
}



//=============================================================================
// �U���A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void TigerAnimationManager::switchAttack()
{
	inactivateAll();
	activate(animation[ATTACK]);
}


//=============================================================================
// ���S�A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void TigerAnimationManager::switchDead()
{
	inactivateAll();
	activate(animation[DEAD]);
}
#pragma endregion