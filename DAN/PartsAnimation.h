//-----------------------------------------------------------------------------
// �p�[�c�A�j���[�V�������N���X[PartsAnimation.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/12
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include "Object.h"


//=============================================================================
//�N���X��`
//=============================================================================
class PartsAnimation: public Base
{
public:
	float animationTime;	// �A�j���[�V��������
	int keyFrame;			// �L�[�t���[��
	int numKeyFrame;		// �L�[�t���[���̐�
	DWORD flag;				// ���g�̃A�j���[�V�����t���O
	bool wasPlayedToEnd;	// �Ō�܂ōĐ����ꂽ

	// �R���X�g���N�^
	PartsAnimation(DWORD _flag);
	// �f�X�g���N�^
	~PartsAnimation();
	// ������
	void initialize();
	// �X�V
	virtual void update(D3DXVECTOR3* rot, Object** parts, float t) = 0;
	// �A�j���[�V�������ԂƃL�[�t���[���̍X�V
	void updateTimeAndKeyFrame(float t);
	// ��]�̍X�V
	D3DXVECTOR3 updateRotation(const D3DXVECTOR3* rotation);
	// ���݂̃L�[�t���[�����擾
	int getKeyFrame();
	// �L�[�t���[���̐����擾
	int getNumKeyFrame();
};
