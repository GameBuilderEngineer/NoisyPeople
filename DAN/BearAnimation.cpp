//-----------------------------------------------------------------------------
// BEAR�A�j���[�V����[BearAnimation.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/15
//-----------------------------------------------------------------------------
#include "Bear.h"
#include "BearAnimation.h"
using namespace::bearNS;
using namespace::bearAnimNS;


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
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_ARM_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_ARM_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
};
static const D3DXVECTOR3 DEFAULT_ANIMATION_WAIST[] =
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
	rot[ARM_L] = updateRotation(DEFAULT_ANIMATION_ARM_L);
	rot[ARM_R] = updateRotation(DEFAULT_ANIMATION_ARM_R);
	rot[WAIST] = updateRotation(DEFAULT_ANIMATION_WAIST);
	rot[LEG_L] = updateRotation(DEFAULT_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(DEFAULT_ANIMATION_LEG_R);
}


//=============================================================================
// �ړ��A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 MOVE_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, -0.01f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.01f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_ARM_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
	D3DXVECTOR3(0.0f, 0.2f, -0.5f),
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
	D3DXVECTOR3(0.0f, -0.2f, -0.5f),
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_ARM_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
	D3DXVECTOR3(0.0f, 0.2f, 0.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
	D3DXVECTOR3(0.0f, -0.2f, 0.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_WAIST[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, -0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.2f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),

};
static const D3DXVECTOR3 MOVE_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 MOVE_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.5f, 0.0f, 0.0f),
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
	rot[ARM_L] = updateRotation(MOVE_ANIMATION_ARM_L);
	rot[ARM_R] = updateRotation(MOVE_ANIMATION_ARM_R);
	rot[WAIST] = updateRotation(MOVE_ANIMATION_WAIST);
	rot[LEG_L] = updateRotation(MOVE_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(MOVE_ANIMATION_LEG_R);
}


//=============================================================================
// �U���A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 ATTACK1_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, -0.01f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
	D3DXVECTOR3(0.2f, 0.0f, 0.01f),
	D3DXVECTOR3(0.2f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK1_ANIMATION_ARM_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
	D3DXVECTOR3(0.0f, 0.0f, -0.6f),
	D3DXVECTOR3(0.0f, 0.0f, -0.5f),
};
static const D3DXVECTOR3 ATTACK1_ANIMATION_ARM_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
	D3DXVECTOR3(0.0f, 0.0f, 0.6f),
	D3DXVECTOR3(0.0f, 0.0f, 0.5f),
};
static const D3DXVECTOR3 ATTACK1_ANIMATION_WAIST[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, -0.05f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.005f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),

};
static const D3DXVECTOR3 ATTACK1_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.25f),
	D3DXVECTOR3(0.5f, 0.0f, 0.25f),
	D3DXVECTOR3(0.0f, 0.0f, 0.25f),
	D3DXVECTOR3(-0.5f, 0.0f, 0.25f),
	D3DXVECTOR3(0.0f, 0.0f, 0.25f),
};
static const D3DXVECTOR3 ATTACK1_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -0.25f),
	D3DXVECTOR3(-0.5f, 0.0f, -0.25),
	D3DXVECTOR3(0.0f, 0.0f, -0.25),
	D3DXVECTOR3(0.5f, 0.0f, -0.25),
	D3DXVECTOR3(0.0f, 0.0f, -0.25),
};

// �R���X�g���N�^
Attack1Animation::Attack1Animation(DWORD _flag) :PartsAnimation(_flag)
{
	numKeyFrame = sizeof(ATTACK1_ANIMATION_BODY) / sizeof(D3DXVECTOR3);
}

// �X�V
void Attack1Animation::update(D3DXVECTOR3* rot, Object** parts, float t)
{
	PartsAnimation::updateTimeAndKeyFrame(t);
	rot[BODY] = updateRotation(ATTACK1_ANIMATION_BODY);
	rot[ARM_L] = updateRotation(ATTACK1_ANIMATION_ARM_L);
	rot[ARM_R] = updateRotation(ATTACK1_ANIMATION_ARM_R);
	rot[WAIST] = updateRotation(ATTACK1_ANIMATION_WAIST);
	rot[LEG_L] = updateRotation(ATTACK1_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(ATTACK1_ANIMATION_LEG_R);
}


//=============================================================================
// ���S�A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 DEAD_ANIMATION_BODY[] =
{
	D3DXVECTOR3(-0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.3f, 0.2f, 0.0f),
	D3DXVECTOR3(-0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.3f, -0.2f, 0.0f),
	D3DXVECTOR3(-0.3f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_ARM_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_ARM_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	D3DXVECTOR3(0.0f, 0.0f, 1.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_WAIST[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.1f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -0.1f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_LEG_L[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_LEG_R[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
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
	rot[ARM_L] = updateRotation(DEAD_ANIMATION_ARM_L);
	rot[ARM_R] = updateRotation(DEAD_ANIMATION_ARM_R);
	rot[WAIST] = updateRotation(DEAD_ANIMATION_WAIST);
	rot[LEG_L] = updateRotation(DEAD_ANIMATION_LEG_L);
	rot[LEG_R] = updateRotation(DEAD_ANIMATION_LEG_R);
}


//=============================================================================
#pragma endregion


#pragma region [Manager]
///////////////////////////////////////////////////////////////////////////////
/////////////////////////  �A�j���[�V�����}�l�[�W���[ /////////////////////////
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
// �R���X�g���N�^
//=============================================================================
BearAnimationManager::BearAnimationManager(int partsMax, Object* parent, Object** parts)
	: PartsAnimationManager(partsMax, parent, parts)
{
	animation = new PartsAnimation*[ANIMATION_TYPE_MAX];

	// �A�j���[�V�������쐬
	animation[DEFAULT] = new DefaultAnimation(ANIMATION_FLAG[DEFAULT]);
	animation[MOVE] = new MoveAnimation(ANIMATION_FLAG[MOVE]);
	animation[ATTACK1] = new Attack1Animation(ANIMATION_FLAG[ATTACK1]);
	animation[ATTACK2] = NULL;
	animation[ATTACK3] = NULL;
	animation[DEAD] = new DeadAnimation(ANIMATION_FLAG[DEAD]);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
BearAnimationManager::~BearAnimationManager()
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
void BearAnimationManager::update(float frameTime)
{
	// ��]�̍X�V
	for (int i = 0; i < ANIMATION_TYPE_MAX; i++)
	{
		if (animation[i] == NULL) { continue; }

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
void BearAnimationManager::culcPartsMatrix()
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

		// �q�p�[�c�̃��[���h�}�g���N�X�ɐe�p�[�c�̉�]�}�g���N�X���|����
		if (i == ARM_L || i == ARM_R)
		{
			D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[BODY]->matrixRotation);
		}
		else if (i == LEG_L || i == LEG_R)
		{
			D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[WAIST]->matrixRotation);
		}

		// �ʒu�}�g���N�X�����[���h�}�g���N�X�Ɋ|����
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parts[i]->matrixPosition);

		// �p�[�c�̃��[���h�}�g���N�X�ɐe�I�u�W�F�N�g�̃��[���h�}�g���N�X���|����
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parent->matrixWorld);
	}
}


//=============================================================================
// �f�t�H���g�A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void BearAnimationManager::switchDefault()
{
	// �U���A�j���[�V�������I�����Ă��Ȃ��ƍĐ�����Ȃ�
	if (animation[ATTACK1]->wasPlayedToEnd)
	{
		inactivate(animation[ATTACK1]);
		activate(animation[DEFAULT]);
	}
}


//=============================================================================
// �ړ��A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void BearAnimationManager::switchMove()
{
	// �U���A�j���[�V�������I�����Ă��Ȃ��ƍĐ�����Ȃ�
	if (animation[ATTACK1]->wasPlayedToEnd)
	{
		inactivate(animation[ATTACK1]);
		activate(animation[MOVE]);
	}
}



//=============================================================================
// �U���A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void BearAnimationManager::switchAttack()
{
	inactivateAll();
	activate(animation[ATTACK1]);
}


//=============================================================================
// ���S�A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void BearAnimationManager::switchDead()
{
	inactivateAll();
	activate(animation[DEAD]);
}
#pragma endregion