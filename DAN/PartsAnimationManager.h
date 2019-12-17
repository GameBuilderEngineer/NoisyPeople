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
	int partsMax;				// �p�[�c�̐�
	Object* parent;				// �e�I�u�W�F�N�g
	Object** parts;				// �p�[�c�̃|�C���^�z��(�p�[�c�̐��ɍ��킹�ē��I�ɍ쐬�����)
	D3DXVECTOR3* rot;			// ��]�x�N�g���z��(�p�[�c�̐��ɍ��킹�ē��I�ɍ쐬�����)
	int animatinMax;			// �A�j���[�V�����̐�
	DWORD flagState;			// �t���O�X�e�[�g(�A�j���[�V�����̎��s���Ǘ�����)
	PartsAnimation** animation;	// �A�j���[�V�����I�u�W�F�N�g�z��i�h���N���X�œ��I�ɍ쐬�̔j���j

public:

	// 3D�T�E���h
	bool wasTimingCame;			// �ړ����Đ��^�C�~���O���R���g���[������t���O
	bool canPlayMoveSound;		// �ړ����Đ��t���O

	//-------------------------------------------------------------------------
	// �R���X�g���N�^�Ƀp�[�c�̐��A�e�I�u�W�F�N�g�̃|�C���^�A�p�[�c�I�u�W�F�N�g
	// �̃|�C���^��n�����ƂŃ}�l�[�W���[������������.
	// �p�[�c�͑S�p�[�c�̃|�C���^�z���p�ӂ��ēn�����Ƃɒ��ӂ���.
	//-------------------------------------------------------------------------
	// �R���X�g���N�^
	PartsAnimationManager(int _partsMax, Object* _parent, Object** _parts);
	// �f�X�g���N�^
	~PartsAnimationManager();
	// �X�V
	virtual void update(float frameTime) = 0;
	// �p�[�c�̃}�g���N�X���v�Z����
	virtual void culcPartsMatrix() = 0;
	// �A�j���[�V�����N��
	virtual void activate(PartsAnimation* animation);
	// �A�j���[�V������~
	virtual void inactivate(PartsAnimation* animation);
	// �A�j���[�V�����S��~
	void inactivateAll();
	// �A�j���[�V�������擾
	PartsAnimation* getAnimation(int type);

	//--------------------------------------------
	// ���ʃA�j���[�V�����͈ȉ��̊֐����R�[������
	//--------------------------------------------
	// �f�t�H���g�A�j���[�V�����ɐ؂�ւ�
	virtual void switchDefault() = 0;
	// �ړ��A�j���[�V�����ɐ؂�ւ�
	virtual void switchMove() = 0;
	// �U���A�j���[�V�����ɐ؂�ւ�
	virtual void switchAttack() = 0;
	// ���S�A�j���[�V�����ɐ؂�ւ�
	virtual void switchDead() = 0;
};
