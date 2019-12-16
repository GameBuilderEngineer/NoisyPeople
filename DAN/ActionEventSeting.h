//=============================================================================
//
// �A�j���[�V�����w�b�_�[ [D3DXAnimation.h]
// Author : HAL���� ���q� 
//
//=============================================================================
#ifndef _ACTIONEVENTSETING_H_
#define _ACTIONEVENTSETING_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define Keydata_Max (50)
// �f�X�g���N�^
#define SAFE_FREE(object)			{if(object){free (object);			(object) = NULL;}}

#include "D3DXAnimation.h"

enum CallbackKeyType
{
	NoEvent = 0,
	MovePAttackStart,
	MovePAttackEnd,
	MovePJumpFireStart,
	MovePJumpFireEnd,
	MovePRollStart,
	MovePRollEnd,
	MovePDeath,
	MovePIsDamage,
	MovePIsDamageEnd,
	MoveP1AttackStart,
	MoveP1AttackEnd,
	MoveP1JumpFireStart,
	MoveP1JumpFireEnd,
	MoveP1RollStart,
	MoveP1RollEnd,
	MoveP1Death,
	MoveP1IsDamage,
	MoveP1IsDamageEnd,
	MovePMoveVoice,
	MoveP1MoveVoice,
	ResetTime,				// �A�j���[�V�����̃��[�J�����Ԃ����Z�b�g 
	MotionEnd,				// ���[�V�����I��
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	float	Time;			// Keyframe�̎���
	int		KeyType;		// Keyframe�̎��
}KEYDATA;


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct
{
	int				Keydata_No = 0;
	KEYDATA			Keydata[Keydata_Max];
}ACTIONEVENTSETING;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �A�j���[�V�������f�C�x���g�̃L�[��ݒu����
HRESULT SetupCallbackKeys(KEYDATA *Keydata, LPCSTR SetName, int CallbacksNum, const char *Owner);
// �L�[�f�[�^��ݒu����
void AddKeydata(float Time, int KeyType);

ACTIONEVENTSETING *GetAESPAdr(void);

// �A�j���[�V�����R�[���o�b�N�\����
struct AnimCallBackHandler : public ID3DXAnimationCallbackHandler
{
	// �����o�[�ϐ�
	D3DXANIMATION		*AnimPointer;

	// �����o�[�֐�
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);
};



#endif
