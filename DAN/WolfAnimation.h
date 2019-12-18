//-----------------------------------------------------------------------------
// WOLF�A�j���[�V����[WolfAnimation.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/15
//-----------------------------------------------------------------------------
#pragma once
#include "PartsAnimation.h"
#include "PartsAnimationManager.h"

//=============================================================================
// ���O���
//=============================================================================
namespace wolfAnimNS
{
	// �A�j���[�V�����̎��
	enum ANIMATION_TYPE
	{
		DEFAULT,
		MOVE,
		ATTACK,
		DEAD,
		ANIMATION_TYPE_MAX
	};

	// �A�j���[�V�����̃r�b�g���Z�t���O
	const DWORD ANIMATION_FLAG[ANIMATION_TYPE_MAX] =
	{
		0x00000001, // DEFAULT
		0x00000002,	// MOVE
		0x00000004,	// ATTACK
		0x00000008,	// DEAD
	};

	// �A�j���[�V�����̃X�s�[�h
	// �L�[�t���[�����u1�i�ނ̂�1�b�v������X�s�[�h�ɑ΂��Ă̔{��. ex)3.0f ���@�u1�i�ނ̂�0.33�b�v
	// �܂�1�̃A�j���[�V�����S�̂̒�����(�L�[�t���[���̐� - 1) * (1.0f / ���̔{��)�ŋ��߂���.
	const float ANIMATION_SPEED[ANIMATION_TYPE_MAX] =
	{
		1.0f,		// DEFAULT
		12.0f,		// MOVE
		7.0f,		// ATTACK
		6.0f,		// DEAD
	};

	//----------------------
	// �A�j���[�V�����N���X
	//----------------------
	// �f�t�H���g
	class DefaultAnimation : public PartsAnimation
	{
	public:
		DefaultAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};

	// �ړ�
	class MoveAnimation : public PartsAnimation
	{
	public:
		MoveAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};

	// �U��
	class AttackAnimation : public PartsAnimation
	{
	public:
		AttackAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};

	// ���S
	class DeadAnimation : public PartsAnimation
	{
	public:
		DeadAnimation(DWORD _flag);
		void update(D3DXVECTOR3* rot, Object** parts, float t);
	};
}


//=============================================================================
// �A�j���[�V�����}�l�[�W���[�N���X
//=============================================================================
class WolfAnimationManager: public PartsAnimationManager
{
public:
	// �R���X�g���N�^
	WolfAnimationManager(int partsMax, Object* parent, Object** parts);
	// �f�X�g���N�^
	~WolfAnimationManager();
	// �X�V
	void update(float frameTime) override;
	// �p�[�c�̃}�g���N�X���v�Z����
	void culcPartsMatrix() override;
	// �f�t�H���g�A�j���[�V�����ɐ؂�ւ�
	void switchDefault() override;
	// �ړ��A�j���[�V�����ɐ؂�ւ�
	void switchMove() override;
	// �U���A�j���[�V�����ɐ؂�ւ�
	void switchAttack() override;
	// ���S�A�j���[�V�����ɐ؂�ւ�
	void switchDead() override;
};
