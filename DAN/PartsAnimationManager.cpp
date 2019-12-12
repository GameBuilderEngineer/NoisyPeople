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
	parts = new Object*[partsMax];
	for (int i = 0; i < partsMax; i++)
	{
		parts[i] = _parts[i];
	}
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
PartsAnimationManager::~PartsAnimationManager()
{
	SAFE_DELETE_ARRAY(parts);
}


//=============================================================================
// ������
//=============================================================================
void PartsAnimationManager::initialize()
{
	flagState = 0;
}


//=============================================================================
// �p�[�c�̃��[���h�}�g���N�X���Z�o
//=============================================================================
void PartsAnimationManager::culcPartsWorldMatrix()
{
	for (int i = 0; i < partsMax; i++)
	{
		D3DXMatrixMultiply(&parts[i]->matrixWorld, &parts[i]->matrixWorld, &parent->matrixWorld);
	}
}


//=============================================================================
// �A�j���[�V�����N��
//=============================================================================
void PartsAnimationManager::activate(DWORD flag)
{
	flagState |= flag;
}


//=============================================================================
// �A�j���[�V������~
//=============================================================================
void PartsAnimationManager::inactivate(DWORD flag)
{
	flagState &= ~flag;
}

