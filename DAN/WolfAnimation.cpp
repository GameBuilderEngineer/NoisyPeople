//-----------------------------------------------------------------------------
// WOLF�A�j���[�V����[WolfAnimation.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/15
//-----------------------------------------------------------------------------
#include "Wolf.h"
#include "WolfAnimation.h"
using namespace::wolfNS;
using namespace::wolfAnimNS;


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
static const D3DXVECTOR3 DEFAULT_ANIMATION_ARM[] =
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
	rot[ARM] = updateRotation(DEFAULT_ANIMATION_ARM);
}


//=============================================================================
// �ړ��A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 MOVE_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.01f, 0.05f, 0.01f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.01f, -0.05f, -0.01f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

static const D3DXVECTOR3 MOVE_ANIMATION_ARM[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.05f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-0.05f, 0.0f, 0.0f),
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
	rot[ARM] = updateRotation(MOVE_ANIMATION_ARM);
}


//=============================================================================
// �U���A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 ATTACK_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(0.3f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};
static const D3DXVECTOR3 ATTACK_ANIMATION_ARM[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
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
	rot[ARM] = updateRotation(ATTACK_ANIMATION_ARM);
}


//=============================================================================
// ���S�A�j���[�V����
//=============================================================================
static const D3DXVECTOR3 DEAD_ANIMATION_BODY[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.01f, 1.54f, 0.0f),
	D3DXVECTOR3(0.0f, 3.14f, 0.0f),
	D3DXVECTOR3(-0.01f, 4.68f, 0.0f),
	D3DXVECTOR3(0.0f, 6.28f, 0.0f),
};
static const D3DXVECTOR3 DEAD_ANIMATION_ARM[] =
{
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-1.2f, 0.0f, 0.1f),
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-1.3f, 0.0f, -0.1f),
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
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
	rot[ARM] = updateRotation(DEAD_ANIMATION_ARM);
}
#pragma endregion


#pragma region [Manager]
///////////////////////////////////////////////////////////////////////////////
/////////////////////////  �A�j���[�V�����}�l�[�W���[ /////////////////////////
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
// �R���X�g���N�^
//=============================================================================
WolfAnimationManager::WolfAnimationManager(int partsMax, Object* parent, Object** parts)
	: PartsAnimationManager(partsMax, parent, parts)
{
	animation = new PartsAnimation*[ANIMATION_TYPE_MAX];

	// �A�j���[�V�������쐬
	animation[DEFAULT] = new DefaultAnimation(ANIMATION_FLAG[DEFAULT]);
	animation[MOVE] = new MoveAnimation(ANIMATION_FLAG[MOVE]);
	animation[ATTACK] = new AttackAnimation(ANIMATION_FLAG[ATTACK]);
	animation[DEAD] = new DeadAnimation(ANIMATION_FLAG[DEAD]);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
WolfAnimationManager::~WolfAnimationManager()
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
void WolfAnimationManager::update(float frameTime)
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
void WolfAnimationManager::culcPartsMatrix()
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
void WolfAnimationManager::switchDefault()
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
void WolfAnimationManager::switchMove()
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
void WolfAnimationManager::switchAttack()
{
	inactivateAll();
	activate(animation[ATTACK]);
}


//=============================================================================
// ���S�A�j���[�V�����ɐ؂�ւ�
//=============================================================================
void WolfAnimationManager::switchDead()
{
	inactivateAll();
	activate(animation[DEAD]);
}
#pragma endregion