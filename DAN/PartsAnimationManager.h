//-----------------------------------------------------------------------------
// �p�[�c�A�j���[�V�����}�l�[�W�� [PartsAnimationManager.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/12
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "PartsAnimation.h"

//=============================================================================
//�N���X��`
//=============================================================================
class PartsAnimationManager: public Base
{
protected:
	int partsMax;
	DWORD flagState;
	Object* parent;
	Object** parts;

public:
	PartsAnimationManager(int _partsMax, Object* _parent, Object** _parts);
	~PartsAnimationManager();
	// ������
	void initialize();
	// �X�V
	virtual void update(float frameTime) = 0;
	// �p�[�c�̃��[���h�}�g���N�X���Z�o
	void culcPartsWorldMatrix();
	// �A�j���[�V�����N��
	void activate(DWORD flag);
	// �A�j���[�V������~
	void inactivate(DWORD flag);
};
