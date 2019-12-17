//=============================================================================
//
// �L�����N�^�A�j������ [MoveP1.cpp]
// Author : HAL���� ���q�
// Type : C
//=============================================================================

#include "movep1.h"
#include <vector>
#include "camera.h"
#include "Direct3D9.h"
#include "ActionEventSeting.h"
#include "Player.h"
#include "Input.h"


//�v���C���[
MOVEP1 MoveP1;
using namespace playerNS;

float wi1 = 0.0f;

//=============================================================================
// �v���C���[�̏���������
//=============================================================================
HRESULT InitMoveP1(D3DXVECTOR3 Rot, D3DXVECTOR3 Scl, bool FirstInit)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//�K�v 1
	MoveP1.pAllocateHier = new CAllocateHierarchy();
	//�K�v 1


	//�����ݒ�
	MoveP1.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MoveP1.Rot = Rot;
	MoveP1.Quaternion = D3DXQUATERNION(0, 0, 0, 1);
	MoveP1.Scl = Scl;
	MoveP1.ActionSpeed = 1.25f;


	// ���߂ď�����
	if (FirstInit == true)
	{
		// �A�j���[�V�����I�u�W�F�N�g�����
		MoveP1.Animation = (D3DXANIMATION*)calloc(1, sizeof(D3DXANIMATION));
		MoveP1.Animation->Owner = "MoveP1";

		// xfile��ǂݍ���
		if (FAILED(Load_xFile(MoveP1.Animation, MOVEP1_MODEL, "MoveP1")))
		{
			return E_FAIL;
		}

		// �A�j���[�V�����Z�b�g�̖��O�Ɛݒu����R�[���o�b�N�̐��Ȃǂ̎���
		// ���Ԃ�x file�ƈꏏ�ɂ��Ȃ��Ƃ����Ȃ�
		for (int Set_No = 0; Set_No < MoveP1.Animation->AnimeSetNum; Set_No++)
		{
			switch (Set_No)
			{
				//������̓L�����N�^�[�̑S���A�j���𓱓�����ꏊ
				//���f�������l�m�F��������
				//.X����AnimationSet �̌��̖��O�͓���̖��O�ł�
				//������m�F���āA�Ґ�����
				//									   SetName		AnimSet ShiftTime CurWeightTime
			case MoveP1_Idle:
				MoveP1.Animation->AnimData[Set_No] = { "Idle",			NULL, 0.3f,	0.0f };
				break;
			case MoveP1_Run:
				MoveP1.Animation->AnimData[Set_No] = { "Run",		NULL, 0.1f,	0.0f };
				break;
			case MoveP1_FireIdle:
				MoveP1.Animation->AnimData[Set_No] = { "FireIdle",		NULL, 0.1f,	0.0f };
				break;
			case MoveP1_FireRun:
				MoveP1.Animation->AnimData[Set_No] = { "FireRun",	NULL, 0.1f,	0.0f };
				break;
			case MoveP1_WalkRight:
				MoveP1.Animation->AnimData[Set_No] = { "FireRight",	NULL, 0.1f,	0.0f };
				break;
			case MoveP1_WalkLeft:
				MoveP1.Animation->AnimData[Set_No] = { "FireLeft",	NULL, 0.1f,	0.0f };
				break;
			case MoveP1_WalkBackwards:
				MoveP1.Animation->AnimData[Set_No] = { "FireBack",	NULL, 0.1f,	0.0f };
				break;
			case MoveP1_JumpFire:
				MoveP1.Animation->AnimData[Set_No] = { "FireJump",		NULL, 0.1f,	0.0f };
				break;
			case MoveP1_Die:
				MoveP1.Animation->AnimData[Set_No] = { "Death",	NULL, 0.1f,	0.0f };
				break;
			default:
				break;
			}
		}

		// AnimationCallback��ݒu����
		if (FAILED(InitCallbackKeys_MoveP1()))
		{
			return E_FAIL;
		}


		for (int Set_No = 0; Set_No < MoveP1.Animation->AnimeSetNum; Set_No++)
		{
			// AnimationSet������������
			if (FAILED(InitAnimation(MoveP1.Animation, Set_No)))
			{
				return E_FAIL;
			}
		}

		//�ŏ��̃A�j���ݒ�
		MoveP1.Animation->CurrentAnimID = -1;
		ChangeAnimation(MoveP1.Animation, MoveP1_Idle, 1.0f, false);

	}

	MoveP1.Animation->MotionEnd = true;

	return S_OK;

}

//=============================================================================
// �v���C���[�̏I������
//=============================================================================
void UninitMoveP1(void)
{
	// �A�j���[�V�����������[�X
	UninitAnimation(MoveP1.Animation);
	return;

}

//=============================================================================
// �v���C���[�̍X�V����
//=============================================================================
void UpdateMoveP1(float f_TimeDelta)
{
	Input *input = getInput();
	const BYTE BUTTON_JUMP = virtualControllerNS::B;
	const BYTE BUTTON_ATTACK = virtualControllerNS::R1;

	if (input->getMouseLButton() || input->getController()[gameMasterNS::PLAYER_2P]->isButton(BUTTON_ATTACK))
	{
		MoveP1.IsChange = true;
	}
	else
	{
		MoveP1.IsChange = false;
	}

	if ((input->getMouseRButtonTrigger() || input->getController()[gameMasterNS::PLAYER_2P]->wasButton(BUTTON_JUMP)) && !MoveP1.IsJump && MoveP1.IsGround )
	{
		MoveP1.IsJump = true;
		wi1 = 0.24f;
	}


	//�ړ�����
	if (!MoveP1.IsChange)
	{
		MoveP1.IsChangeRun = false;
		MoveP1.Animation->NextAnimID = MoveP1_Idle;

		if (MoveP1.IsGround)
		{

			if (input->isKeyDown(VK_UP) || input->getController()[gameMasterNS::PLAYER_2P]->getLeftStick().y < 0.0f)
			{
				MoveP1.IsRun = true;
				MoveP1.Animation->NextAnimID = MoveP1_Run;
			}
			else if (input->isKeyDown(VK_DOWN) || input->getController()[gameMasterNS::PLAYER_2P]->getLeftStick().y > 0.0f)
			{
				MoveP1.IsRun = true;
				MoveP1.Animation->NextAnimID = MoveP1_WalkBackwards;
			}
			else if (input->isKeyDown(VK_LEFT) || input->getController()[gameMasterNS::PLAYER_2P]->getLeftStick().x < 0.0f)
			{
				MoveP1.IsRun = true;
				MoveP1.Animation->NextAnimID = MoveP1_WalkLeft;
			}
			else if (input->isKeyDown(VK_RIGHT) || input->getController()[gameMasterNS::PLAYER_2P]->getLeftStick().x > 0.0f)
			{
				MoveP1.IsRun = true;
				MoveP1.Animation->NextAnimID = MoveP1_WalkRight;
			}
			else
			{
				MoveP1.IsRun = false;
			}
		}
	}
	if (MoveP1.IsChange)
	{
		MoveP1.IsRun = false;
		MoveP1.Animation->NextAnimID = MoveP1_FireIdle;

		if (MoveP1.IsGround)
		{

			if (input->isKeyDown(VK_UP) || input->getController()[gameMasterNS::PLAYER_2P]->getLeftStick().y < 0.0f)
			{
				MoveP1.IsChangeRun = true;
				MoveP1.Animation->NextAnimID = MoveP1_FireRun;
			}
			else if (input->isKeyDown(VK_DOWN) || input->getController()[gameMasterNS::PLAYER_2P]->getLeftStick().y > 0.0f)
			{
				MoveP1.IsChangeRun = true;
				MoveP1.Animation->NextAnimID = MoveP1_WalkBackwards;
			}
			else if (input->isKeyDown(VK_LEFT) || input->getController()[gameMasterNS::PLAYER_2P]->getLeftStick().x < 0.0f)
			{
				MoveP1.IsChangeRun = true;
				MoveP1.Animation->NextAnimID = MoveP1_WalkLeft;
			}
			else if (input->isKeyDown(VK_RIGHT) || input->getController()[gameMasterNS::PLAYER_2P]->getLeftStick().x > 0.0f)
			{
				MoveP1.IsChangeRun = true;
				MoveP1.Animation->NextAnimID = MoveP1_WalkRight;
			}
			else
			{
				MoveP1.IsChangeRun = false;
			}
		}
	}

	if (MoveP1.IsJump)
	{
		MoveP1.Animation->NextAnimID = MoveP1_JumpFire;
	}

	//if (input->isKeyDown('Q'))
	//{
	//	MoveP1.Animation->NextAnimID = MoveP1_Die;
	//	MoveP1.IsDie = true;
	//}

	if (MoveP1.IsDeath)
	{
		MoveP1.Animation->AnimController->SetTrackPosition(0, 3.8f);
	}

	if (MoveP1.IsJumpEnd)
	{
		if (MoveP1.IsGround)
		{
			MoveP1.IsJumpEnd = false;
			MoveP1.IsJump = false;
			MoveP1.IsFireJump = false;
			MoveP1.Animation->MotionEnd = true;
		}
		if (!MoveP1.IsGround)
		{
			MoveP1.Animation->AnimController->SetTrackPosition(0, wi1);
		}
	}

	if (MoveP1.GroundDistance > 0 && MoveP1.GroundDistance < 2.5f)
	{
		wi1 += 0.01f;
	}



	MoveP1AnimeCur();
	MoveP1AnimeNext();

	//�A�j���I������Ȃ�A�A�j���̓`�F���W�ł��܂�
	if (MoveP1.Animation->MotionEnd)
	{
		MoveP1.AnimeChange = true;
	}

	// �A�j���[�V�������X�V 
	//�K������Ă��������A�����Ȃ��ƁA���悪�����Ȃ�
	UpdateAnimation(MoveP1.Animation, f_TimeDelta * MoveP1.ActionSpeed);

}

//=============================================================================
// �v���C���[�̕`��ݒ�
//=============================================================================
void DrawMoveP1()
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();


	//         �傫���@��]�@�@�ړ��@�@�@�@�@�J�v�Z������@�@���̃J�v�Z���@�@�@�@�@���̃J�v�Z��
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, CapsuleMatrix, CapsuleMatrix_Shield, CapsuleMatrix_Sword;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MoveP1.WorldMatrix);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, MoveP1.Scl.x, MoveP1.Scl.y, MoveP1.Scl.z);
	D3DXMatrixMultiply(&MoveP1.WorldMatrix, &MoveP1.WorldMatrix, &mtxScl);

	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, MoveP1.Rot.y, MoveP1.Rot.x, MoveP1.Rot.z);
	//D3DXMatrixMultiply(&MoveP1.WorldMatrix, &MoveP1.WorldMatrix, &mtxRot);


	D3DXMatrixRotationQuaternion(&mtxRot, &MoveP1.Quaternion);
	D3DXMatrixMultiply(&MoveP1.WorldMatrix, &MoveP1.WorldMatrix, &mtxRot);


	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, MoveP1.Pos.x, MoveP1.Pos.y, MoveP1.Pos.z);
	D3DXMatrixMultiply(&MoveP1.WorldMatrix, &MoveP1.WorldMatrix, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &MoveP1.WorldMatrix);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetTexture(0, NULL);

	// �A�j���[�V������`�悷��
	DrawAnimation(MoveP1.Animation, &MoveP1.WorldMatrix, false);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// �v���C���[�̈ړ��ݒ�
//=============================================================================
void MoveP1Move(void)
{

	return;
}

//=============================================================================
// �v���C���[�̎��̃A�j���ݒ�
//=============================================================================
void MoveP1AnimeNext(void)
{
	if (MoveP1.AnimeChange)
	{
		switch (MoveP1.Animation->NextAnimID)
		{
		case MoveP1_Idle:
			ChangeAnimation(MoveP1.Animation, MoveP1_Idle, 1.0f, false);
			MoveP1.Animation->NextAnimID = MoveP1_Idle;
			MoveP1.Animation->MotionEnd = true;	//�����ݒ肵�Ȃ��ƁA�҂̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP1_Run:
			ChangeAnimation(MoveP1.Animation, MoveP1_Run, 0.8f, false);
			MoveP1.Animation->NextAnimID = MoveP1_Idle;
			MoveP1.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP1_FireIdle:
			ChangeAnimation(MoveP1.Animation, MoveP1_FireIdle, 1.0f, false);
			MoveP1.Animation->NextAnimID = MoveP1_FireIdle;
			MoveP1.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP1_FireRun:
			ChangeAnimation(MoveP1.Animation, MoveP1_FireRun, 0.8f, false);
			MoveP1.Animation->NextAnimID = MoveP1_FireIdle;
			MoveP1.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP1_JumpFire:
			ChangeAnimation(MoveP1.Animation, MoveP1_JumpFire, 0.5f, false);
			if (MoveP1.IsChange)
			{
				MoveP1.Animation->NextAnimID = MoveP1_FireRun;
			}
			if (!MoveP1.IsChange)
			{
				MoveP1.Animation->NextAnimID = MoveP1_Idle;
			}
			break;
		case MoveP1_Die:
			ChangeAnimation(MoveP1.Animation, MoveP1_Die, 1.0f, false);
			MoveP1.Animation->NextAnimID = MoveP1_Idle;
			break;
		case MoveP1_WalkRight:
			ChangeAnimation(MoveP1.Animation, MoveP1_WalkRight, 1.0f, false);
			MoveP1.Animation->NextAnimID = MoveP1_FireIdle;
			MoveP1.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP1_WalkLeft:
			ChangeAnimation(MoveP1.Animation, MoveP1_WalkLeft, 1.0f, false);
			MoveP1.Animation->NextAnimID = MoveP1_FireIdle;
			MoveP1.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP1_WalkBackwards:
			ChangeAnimation(MoveP1.Animation, MoveP1_WalkBackwards, 1.0f, false);
			MoveP1.Animation->NextAnimID = MoveP1_FireIdle;
			MoveP1.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		default:
			break;
		}

		MoveP1.AnimeChange = false;

	}
}

//=============================================================================
// �v���C���[�̌��݂̃A�j���ݒ�
//=============================================================================
void MoveP1AnimeCur(void)
{
	Input *input = getInput();

	switch (MoveP1.Animation->CurrentAnimID)
	{
	case MoveP1_Idle:
		break;
	case MoveP1_Run:
		if (!MoveP1.IsRun)
		{
			MoveP1.Animation->NextAnimID = MoveP1_Idle;
			MoveP1.AnimeChange = true;
		}
		break;
	case MoveP1_FireIdle:
		break;
	case MoveP1_FireRun:
		if (!MoveP1.IsChangeRun)
		{
			MoveP1.Animation->NextAnimID = MoveP1_FireIdle;
			MoveP1.AnimeChange = true;
		}
		break;
	case MoveP1_JumpFire:
		break;
	case MoveP1_Die:
		MoveP1.Animation->NextAnimID = MoveP1_Idle;
		break;
	default:
		break;
	}
}

//=============================================================================
// �v���C���[�̃|�C���^���l������
//=============================================================================
MOVEP1 *GetMoveP1Adr(void)
{
	return &MoveP1;
}

//=============================================================================
// CallbackKeys��������(�v���C���[)
//=============================================================================
HRESULT InitCallbackKeys_MoveP1(void)
{
	ACTIONEVENTSETING *AES = GetAESPAdr();

	// �ݒu����L�[�̐�
	int SetNum = MoveP1.Animation->AnimeSetNum;
	float AnimLoopTime = 0.0f;

	for (int Set_No = 0; Set_No < SetNum; Set_No++)
	{
		// �L�[�f�[�^���N���A����
		memset(&AES->Keydata[0], 0, sizeof(KEYDATA) * Keydata_Max);
		AES->Keydata_No = 0;

		switch (Set_No)
		{
		case MoveP1_Idle:
			break;
		case MoveP1_Run:
			AddKeydata(0.33f, MoveP1MoveVoice);
			AddKeydata(0.68f, MoveP1MoveVoice);
			AddKeydata(0.95f, MotionEnd);
			break;
		case MoveP1_FireIdle:
			break;
		case MoveP1_FireRun:
			AddKeydata(0.95f, MotionEnd);
			break;
		case MoveP1_JumpFire:
			AddKeydata(0.01f, MoveP1JumpFireStart);
			AddKeydata(0.3f, MoveP1JumpFireEnd);
			break;
		case MoveP1_Die:
			AddKeydata(0.95f, MoveP1Death);
			break;
		default:
			continue;
		}

		if (FAILED(
			SetupCallbackKeys(&AES->Keydata[0], MoveP1.Animation->AnimData[Set_No].SetName,
				AES->Keydata_No, "MoveP1")))
		{
			return E_FAIL;
		}

	}


	return S_OK;
}


