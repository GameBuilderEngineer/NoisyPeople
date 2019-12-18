//=============================================================================
//
// �L�����N�^�A�j������ [movep.h]
// Author : HAL���� ���q�
//
//=============================================================================
#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "AllocateHierarchy.h"										
#include <vector>
#include "D3DXAnimation.h"


#define	MOVEP_MODEL		"07 Model File/male.X"

#define TimePerFrame		(0.0167f)				// 1 / 60�b

enum MovePAction
{
	MoveP_Idle,
	MoveP_Run,
	MoveP_FireIdle,
	MoveP_FireRun,
	MoveP_Jump,
	MoveP_JumpFire,
	MoveP_Die,
	MoveP_WalkRight,
	MoveP_WalkLeft,
	MoveP_WalkBackwards,
	MoveP_WalkBackward,
	MoveP_WalkStrafeLeft,
	MoveP_WalkStrafeRight,
	MoveP_Max
};


typedef struct
{
	CAllocateHierarchy*		pAllocateHier = NULL;

	D3DXMATRIX			WorldMatrix;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3			Pos;				// ���W
	D3DXVECTOR3			Scl;				// �傫��(�X�P�[��)
	D3DXVECTOR3			Rot;				// ��]
	D3DXQUATERNION		Quaternion;				// ��]
	D3DXANIMATION		*Animation;			// �A�j���[�V����
	float				ActionSpeed;		// �S�̃A�N�V�����̑��x
	float				fTimeDelta = 0.0f;
	D3DXMATRIX			RHand;
	D3DXVECTOR3			RHandPos;
D3DXVECTOR3			PrePos;				// 1�t���C���O�̍��W
D3DXVECTOR3			CenterPos;			// �v���C���[�̒��S���W
D3DXVECTOR3			NextPos;			// ���̍��W
D3DXVECTOR3			Move;				// �ړ���
D3DXVECTOR3			DestAngle;			// ����
float				MoveY;				// �ڕW�p�x
float				GroundDistance;		// �n�ʂƂ̋���
float				facing;				// �ڕW�p�x
float				facingTarget;		// ���̃A�j���[�V�����̖ڕW�p�x
int					HP;					// �v���C���[��HP
float				RunSpeedPl;			
float				WalkSpeedPl;

//���f�p�ϐ�
bool				IsRun = false;
bool				IsIdle = true;
bool				IsWalk = false;
bool				IsRoll = false;
bool				IsJump = false;
bool				IsFireJump = false;
bool				IsJumpEnd = false;
bool				IsGround = false;
bool				IsDefend = false;
bool				AnimeChange = true;
bool				IsChange = false;
bool				IsChangeRun = false;


//���S�̓��씻�f
bool				IsDie = false;
//���S�����㔻�f
bool				IsDeath = false;
//�U���O�i�̔��f
bool				IsAttack = false;
bool				IsAttack1 = false;
bool				IsAttack2 = false;
//�U���O�i�̈ړ����f
bool				AttackMove = false;
bool				AttackMove1 = false;
bool				AttackMove2 = false;
//�U���O�i�̍U�����f
bool				IsDamage1 = false;
bool				flag1 = false;
bool				IsDamage2 = false;
bool				flag2 = false;
bool				IsDamage3 = false;
bool				flag3 = false;
bool				IsDefendDamage = false;
bool				IsNoDefendDamage = false;

//���ݎg�p���Ă��Ȃ�
bool				IsLock = false;

}MOVEP;


HRESULT InitMoveP(D3DXVECTOR3 Rot, D3DXVECTOR3 Scl, bool FirstInit);
void UpdateMoveP(float f_TimeDelta);
void DrawMoveP();
void UninitMoveP(void);

void MovePMove(void);
void MovePAnimeCur(void);
void MovePAnimeNext(void);


MOVEP *GetMovePAdr(void);
// �A�j���[�V�������f�C�x���g��Keyframes��ݒu����
HRESULT InitCallbackKeys_MoveP(void);

