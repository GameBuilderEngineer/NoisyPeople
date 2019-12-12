//-----------------------------------------------------------------------------
// �p�[�c�A�j���[�V�������N���X[PartsAnimationManager.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/12
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"


//=============================================================================
//�N���X��`
//=============================================================================
class PartsAnimation: public Base
{
private:
	float animationTime;	// �A�j���[�V��������
	int keyFrame;			// �L�[�t���[��
	int numKeyFrame;		// �L�[�t���[���̐�

public:
	PartsAnimation();
	~PartsAnimation();
	void initialize();
	void update(float t);
};
