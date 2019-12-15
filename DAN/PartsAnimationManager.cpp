//-----------------------------------------------------------------------------
// �p�[�c�A�j���[�V�����}�l�[�W�� [PartsAnimationManager.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/12
//-----------------------------------------------------------------------------
#include "PartsAnimationManager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
PartsAnimationManager::PartsAnimationManager(int _partsMax, Object* _parent, Object** _parts)
{
	partsMax = _partsMax;
	parent = _parent;

	// �p�[�c���̔z����m��
	parts = new Object*[partsMax];
	for (int i = 0; i < partsMax; i++)
	{
		parts[i] = _parts[i];
	}
	rot = new D3DXVECTOR3[partsMax];
	for (int i = 0; i < partsMax; i++)
	{
		rot[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// �t���O��������
	flagState = 0;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
PartsAnimationManager::~PartsAnimationManager()
{
	SAFE_DELETE_ARRAY(parts);
	SAFE_DELETE_ARRAY(rot);
}


//=============================================================================
// �A�j���[�V�����N��
//=============================================================================
void PartsAnimationManager::activate(PartsAnimation* animation)
{
	flagState |= animation->flag;
	animation->wasPlayedToEnd = false;
}


//=============================================================================
// �A�j���[�V������~
//=============================================================================
void PartsAnimationManager::inactivate(PartsAnimation* animation)
{
	flagState &= ~animation->flag;
	animation->wasPlayedToEnd = true;
}


//=============================================================================
// �A�j���[�V�����S��~
//=============================================================================
void PartsAnimationManager::inactivateAll()
{
	flagState = 0;
	for (int i = 0; i < animatinMax; i++)
	{
		animation[i]->wasPlayedToEnd = true;
	}
}


PartsAnimation* PartsAnimationManager::getAnimation(int type)
{
	return animation[type];
}