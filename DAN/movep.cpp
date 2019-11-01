//=============================================================================
//
// �L�����N�^�A�j������ [MoveP.cpp]
// Author : HAL���� ���q�
// Type : C
//=============================================================================

#include "movep.h"
#include <vector>
#include "camera.h"
#include "Direct3D9.h"
#include "ActionEventSeting.h"

//�v���C���[
MOVEP MoveP;

//=============================================================================
// �v���C���[�̏���������
//=============================================================================
HRESULT InitMoveP(D3DXVECTOR3 Rot, D3DXVECTOR3 Scl, bool FirstInit)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//�K�v 1
	MoveP.pAllocateHier = new CAllocateHierarchy();
	//�K�v 1

	//�����ݒ�
	MoveP.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MoveP.Rot = Rot;
	MoveP.Scl = Scl;
	MoveP.ActionSpeed = 1.25f;



	// ���߂ď�����
	if (FirstInit == true)
	{
		// �A�j���[�V�����I�u�W�F�N�g�����
		MoveP.Animation = (D3DXANIMATION*)calloc(1, sizeof(D3DXANIMATION));
		MoveP.Animation->Owner = "MoveP";

		// xfile��ǂݍ���
		if (FAILED(Load_xFile(MoveP.Animation, MOVEP_MODEL, "MoveP")))
		{
			return E_FAIL;
		}

		// �A�j���[�V�����Z�b�g�̖��O�Ɛݒu����R�[���o�b�N�̐��Ȃǂ̎���
		// ���Ԃ�x file�ƈꏏ�ɂ��Ȃ��Ƃ����Ȃ�
		for (int Set_No = 0; Set_No < MoveP.Animation->AnimeSetNum; Set_No++)
		{
			switch (Set_No)
			{
				//������̓L�����N�^�[�̑S���A�j���𓱓�����ꏊ
				//���f�������l�m�F��������
				//.X����AnimationSet �̌��̖��O�͓���̖��O�ł�
				//������m�F���āA�Ґ�����
				//									   SetName		AnimSet ShiftTime CurWeightTime
			case MoveP_Idle:
				MoveP.Animation->AnimData[Set_No] = { "Idle",			NULL, 0.3f,	0.0f };
				break;
			case MoveP_Attack:
				MoveP.Animation->AnimData[Set_No] = { "Attack",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_Defend:
				MoveP.Animation->AnimData[Set_No] = { "Defend",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_Die:
				MoveP.Animation->AnimData[Set_No] = { "Die",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_Run:
				MoveP.Animation->AnimData[Set_No] = { "Run",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_Walk:
				MoveP.Animation->AnimData[Set_No] = { "walk",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_NoDefendImpact:
				MoveP.Animation->AnimData[Set_No] = { "NoDefendImpact",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_DefendImpact:
				MoveP.Animation->AnimData[Set_No] = { "DefendImpact",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_Roll:
				MoveP.Animation->AnimData[Set_No] = { "Roll",	NULL, 0.1f,	0.0f };
				break;
			default:
				break;
			}
		}

		// AnimationCallback��ݒu����
		if (FAILED(InitCallbackKeys_MoveP()))
		{
			return E_FAIL;
		}


		for (int Set_No = 0; Set_No < MoveP.Animation->AnimeSetNum; Set_No++)
		{
			// AnimationSet������������
			if (FAILED(InitAnimation(MoveP.Animation, Set_No)))
			{
				return E_FAIL;
			}
		}

		//�ŏ��̃A�j���ݒ�
		MoveP.Animation->CurrentAnimID = -1;
		ChangeAnimation(MoveP.Animation, MoveP_Attack, 1.0f, false);

	}

	return S_OK;

}

//=============================================================================
// �v���C���[�̏I������
//=============================================================================
void UninitMoveP(void)
{
	// �A�j���[�V�����������[�X
	UninitAnimation(MoveP.Animation);
	return;

}

//=============================================================================
// �v���C���[�̍X�V����
//=============================================================================
void UpdateMoveP(float f_TimeDelta)
{

	// �A�j���[�V�������X�V 
	//�K������Ă��������A�����Ȃ��ƁA���悪�����Ȃ�
	UpdateAnimation(MoveP.Animation, TimePerFrame * MoveP.ActionSpeed);

	MoveP.fTimeDelta = f_TimeDelta;
}

//=============================================================================
// �v���C���[�̕`��ݒ�
//=============================================================================
void DrawMoveP()
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//         �傫���@��]�@�@�ړ��@�@�@�@�@�J�v�Z������@�@���̃J�v�Z���@�@�@�@�@���̃J�v�Z��
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, CapsuleMatrix, CapsuleMatrix_Shield, CapsuleMatrix_Sword;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MoveP.WorldMatrix);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, MoveP.Scl.x, MoveP.Scl.y, MoveP.Scl.z);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, MoveP.Rot.y, MoveP.Rot.x, MoveP.Rot.z);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, MoveP.Pos.x, MoveP.Pos.y, MoveP.Pos.z);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &MoveP.WorldMatrix);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �A�j���[�V������`�悷��
	DrawAnimation(MoveP.Animation, &MoveP.WorldMatrix, false);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// �v���C���[�̈ړ��ݒ�
//=============================================================================
void MovePMove(void)
{
	//float fDiffRotY;
	//DIJOYSTATE2 Dijs = GetDijs();

	//if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
	//{
	//	if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
	//	{// ���O�ړ�
	//		if (GetKeyboardPress(DIK_LSHIFT) || ((Dijs.lX > -500 && Dijs.lY > -500) && Dijs.lX && Dijs.lY))
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y - D3DX_PI * 0.25f) * MoveP.WalkSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y - D3DX_PI * 0.25f) * MoveP.WalkSpeedPl;
	//			MoveP.IsWalk = true;
	//			MoveP.IsRun = false;
	//		}
	//		else
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y - D3DX_PI * 0.25f) * MoveP.RunSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y - D3DX_PI * 0.25f) * MoveP.RunSpeedPl;
	//			MoveP.IsRun = true;
	//			MoveP.IsWalk = false;
	//		}
	//		MoveP.DestAngle.y = Camera->Rot.y - D3DX_PI * 0.25f;
	//	}
	//	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//	{// ����ړ�
	//		if (GetKeyboardPress(DIK_LSHIFT) || ((Dijs.lX > -500 && Dijs.lY < 500) && Dijs.lX && Dijs.lY))
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y - D3DX_PI * 0.75f) * MoveP.WalkSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y - D3DX_PI * 0.75f) * MoveP.WalkSpeedPl;
	//			MoveP.IsWalk = true;
	//			MoveP.IsRun = false;
	//		}
	//		else
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y - D3DX_PI * 0.75f) * MoveP.RunSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y - D3DX_PI * 0.75f) * MoveP.RunSpeedPl;
	//			MoveP.IsRun = true;
	//			MoveP.IsWalk = false;
	//		}
	//		MoveP.DestAngle.y = Camera->Rot.y - D3DX_PI * 0.75f;
	//	}
	//	else
	//	{// ���ړ�
	//		if (GetKeyboardPress(DIK_LSHIFT) || (Dijs.lX > -500 && Dijs.lX ))
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y - D3DX_PI * 0.50f) * MoveP.WalkSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y - D3DX_PI * 0.50f) * MoveP.WalkSpeedPl;
	//			MoveP.IsWalk = true;
	//			MoveP.IsRun = false;
	//		}
	//		else
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y - D3DX_PI * 0.50f) * MoveP.RunSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y - D3DX_PI * 0.50f) * MoveP.RunSpeedPl;
	//			MoveP.IsRun = true;
	//			MoveP.IsWalk = false;
	//		}
	//		MoveP.DestAngle.y = Camera->Rot.y - D3DX_PI * 0.50f;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
	//{
	//	if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
	//	{// �E�O�ړ�
	//		if (GetKeyboardPress(DIK_LSHIFT) || ((Dijs.lX < 500 && Dijs.lY > -500) && Dijs.lX && Dijs.lY))
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y + D3DX_PI * 0.25f) * MoveP.WalkSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y + D3DX_PI * 0.25f) * MoveP.WalkSpeedPl;
	//			MoveP.IsWalk = true;
	//			MoveP.IsRun = false;
	//		}
	//		else
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y + D3DX_PI * 0.25f) * MoveP.RunSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y + D3DX_PI * 0.25f) * MoveP.RunSpeedPl;
	//			MoveP.IsRun = true;
	//			MoveP.IsWalk = false;
	//		}

	//		MoveP.DestAngle.y = Camera->Rot.y + D3DX_PI * 0.25f;
	//	}
	//	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//	{// �E��ړ�
	//		if (GetKeyboardPress(DIK_LSHIFT) || ((Dijs.lX < 500 && Dijs.lY < 500) && Dijs.lX && Dijs.lY))
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y + D3DX_PI * 0.75f) * MoveP.WalkSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y + D3DX_PI * 0.75f) * MoveP.WalkSpeedPl;
	//			MoveP.IsWalk = true;
	//			MoveP.IsRun = false;
	//		}
	//		else
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y + D3DX_PI * 0.75f) * MoveP.RunSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y + D3DX_PI * 0.75f) * MoveP.RunSpeedPl;
	//			MoveP.IsRun = true;
	//			MoveP.IsWalk = false;
	//		}

	//		MoveP.DestAngle.y = Camera->Rot.y + D3DX_PI * 0.75f;
	//	}
	//	else
	//	{// �E�ړ�
	//		if (GetKeyboardPress(DIK_LSHIFT) || (Dijs.lX < 500 && Dijs.lX))
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y + D3DX_PI * 0.50f) * MoveP.WalkSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y + D3DX_PI * 0.50f) * MoveP.WalkSpeedPl;
	//			MoveP.IsWalk = true;
	//			MoveP.IsRun = false;
	//		}
	//		else
	//		{
	//			MoveP.Move.x -= sinf(Camera->Rot.y + D3DX_PI * 0.50f) * MoveP.RunSpeedPl;
	//			MoveP.Move.z -= cosf(Camera->Rot.y + D3DX_PI * 0.50f) * MoveP.RunSpeedPl;
	//			MoveP.IsRun = true;
	//			MoveP.IsWalk = false;
	//		}
	//		MoveP.DestAngle.y = Camera->Rot.y + D3DX_PI * 0.50f;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
	//{// �O�ړ�
	//	if (GetKeyboardPress(DIK_LSHIFT) ||  (Dijs.lY > -500 && Dijs.lY))
	//	{
	//		MoveP.Move.x -= sinf(Camera->Rot.y) * MoveP.WalkSpeedPl;
	//		MoveP.Move.z -= cosf(Camera->Rot.y) * MoveP.WalkSpeedPl;
	//		MoveP.IsWalk = true;
	//		MoveP.IsRun = false;
	//	}
	//	else
	//	{
	//		MoveP.Move.x -= sinf(Camera->Rot.y) * MoveP.RunSpeedPl;
	//		MoveP.Move.z -= cosf(Camera->Rot.y) * MoveP.RunSpeedPl;
	//		MoveP.IsRun = true;
	//		MoveP.IsWalk = false;
	//	}
	//	MoveP.DestAngle.y = Camera->Rot.y;
	//}
	//else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//{// ��ړ�
	//	if (GetKeyboardPress(DIK_LSHIFT) || (Dijs.lY < 500 && Dijs.lY))
	//	{
	//		MoveP.Move.x -= sinf(D3DX_PI + Camera->Rot.y) * MoveP.WalkSpeedPl;
	//		MoveP.Move.z -= cosf(D3DX_PI + Camera->Rot.y) * MoveP.WalkSpeedPl;
	//		MoveP.IsWalk = true;
	//		MoveP.IsRun = false;
	//	}
	//	else
	//	{
	//		MoveP.Move.x -= sinf(D3DX_PI + Camera->Rot.y) * MoveP.RunSpeedPl;
	//		MoveP.Move.z -= cosf(D3DX_PI + Camera->Rot.y) * MoveP.RunSpeedPl;
	//		MoveP.IsRun = true;
	//		MoveP.IsWalk = false;
	//	}
	//	MoveP.DestAngle.y = D3DX_PI + Camera->Rot.y;
	//}
	//else
	//{
	//	MoveP.IsRun = false;
	//	MoveP.IsWalk = false;
	//}

	//// �ʒu�ړ�
	//MoveP.Pos.x += MoveP.Move.x;
	//MoveP.Pos.y += MoveP.Move.y;
	//MoveP.Pos.z += MoveP.Move.z;

	//// �ړ��ʂɊ�����������
	//MoveP.Move.x += (0.0f - MoveP.Move.x) * 0.2f;
	//MoveP.Move.y += (0.0f - MoveP.Move.y) * 0.2f;
	//MoveP.Move.z += (0.0f - MoveP.Move.z) * 0.2f;

	//// �ړI�̊p�x�܂ł̍���
	//fDiffRotY = MoveP.DestAngle.y - MoveP.Rot.y ;
	//if (fDiffRotY > D3DX_PI)
	//{
	//	fDiffRotY -= D3DX_PI * 2.0f;
	//}
	//if (fDiffRotY < -D3DX_PI)
	//{
	//	fDiffRotY += D3DX_PI * 2.0f;
	//}
	//// �ړI�̊p�x�܂Ŋ�����������
	//MoveP.Rot.y += fDiffRotY * 0.2f;
	//if (MoveP.Rot.y > D3DX_PI)
	//{
	//	MoveP.Rot.y -= D3DX_PI * 2.0f;
	//}
	//if (MoveP.Rot.y < -D3DX_PI)
	//{
	//	MoveP.Rot.y += D3DX_PI * 2.0f;
	//}

	return;
}

//=============================================================================
// �v���C���[�̎��̃A�j���ݒ�
//=============================================================================
void MovePAnimeNext(void)
{
	//if (MoveP.AnimeChange)
	//{
	//	switch (MoveP.Animation->NextAnimID)
	//	{
	//	case MoveP_Idle:
	//		ChangeAnimation(MoveP.Animation, MoveP_Idle, 1.0f, false);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		MoveP.Animation->MotionEnd = true;	//�����ݒ肵�Ȃ��ƁA�҂̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
	//		break;
	//	case MoveP_Attack:
	//		ChangeAnimation(MoveP.Animation, MoveP_Attack, 1.2f, false);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		break;
	//	case MoveP_Defend:
	//		ChangeAnimation(MoveP.Animation, MoveP_Defend, 1.0f, true);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		MoveP.Animation->MotionEnd = true;//�h��͉������Ă��邾����A�ړ��ȊO�̓���͎g���܂�
	//		break;
	//	case MoveP_Die:
	//		ChangeAnimation(MoveP.Animation, MoveP_Die, 1.0f, false);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		break;
	//	case MoveP_Run:
	//		ChangeAnimation(MoveP.Animation, MoveP_Run, 0.8f, false);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		MoveP.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
	//		break;
	//	case MoveP_Walk:
	//		ChangeAnimation(MoveP.Animation, MoveP_Walk, 0.8f, false);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		MoveP.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA�����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
	//		break;
	//	case MoveP_NoDefendImpact:
	//		ChangeAnimation(MoveP.Animation, MoveP_NoDefendImpact, 1.0f, false);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		break;
	//	case MoveP_DefendImpact:
	//		//���ݎg���Ă��Ȃ�
	//		ChangeAnimation(MoveP.Animation, MoveP_DefendImpact, 1.0f, false);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		break;
	//	case MoveP_Roll:
	//		ChangeAnimation(MoveP.Animation, MoveP_Roll, 1.7f, false);
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		break;
	//	default:
	//		break;
	//	}

	//	MoveP.AnimeChange = false;

	//}
}

//=============================================================================
// �v���C���[�̌��݂̃A�j���ݒ�
//=============================================================================
void MovePAnimeCur(void)
{
	//switch (MoveP.Animation->CurrentAnimID)
	//{
	//case MoveP_Idle:
	//	break;
	//case MoveP_Attack:
	//	if (MoveP.AttackMove && !MoveP.AttackMove1 && !MoveP.AttackMove2)
	//	{
	//		MoveP.Pos.x -= sinf(MoveP.Rot.y);
	//		MoveP.Pos.z -= cosf(MoveP.Rot.y);
	//	}
	//	if (MoveP.AttackMove1 && !MoveP.AttackMove && !MoveP.AttackMove2)
	//	{
	//		MoveP.Pos.x -= sinf(MoveP.Rot.y);
	//		MoveP.Pos.z -= cosf(MoveP.Rot.y);
	//	}
	//	if (MoveP.AttackMove2 && !MoveP.AttackMove1 && !MoveP.AttackMove)
	//	{
	//		MoveP.Pos.x -= sinf(MoveP.Rot.y) * 0.4f;
	//		MoveP.Pos.z -= cosf(MoveP.Rot.y) * 0.4f;
	//	}

	//	if ((IsMouseLeftTriggered() || IsButtonTriggered(0, BUTTON_Z)) && MoveP.IsAttack)
	//	{
	//		MoveP.IsAttack1 = true;
	//		MoveP.IsAttack = false;
	//	}
	//	else if ((IsMouseLeftTriggered() || IsButtonTriggered(0, BUTTON_Z)) && MoveP.IsAttack1 && !MoveP.IsAttack)
	//	{
	//		MoveP.IsAttack2 = true;
	//		MoveP.IsAttack1 = false;
	//	}
	//	break;
	//case MoveP_Defend:
	//	if (IsMouseLeftTriggered() || IsButtonTriggered(0, BUTTON_Z))
	//	{
	//		MoveP.Animation->NextAnimID = MoveP_Attack;
	//	}
	//	break;
	//case MoveP_Die:
	//	//Enemy->FreeMode = true;
	//	MoveP.Animation->NextAnimID = MoveP_Idle;
	//	break;
	//case MoveP_Run:
	//	if (!MoveP.IsRun)
	//	{
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		MoveP.AnimeChange = true;
	//	}
	//	break;
	//case MoveP_Walk:
	//	if (!MoveP.IsWalk)
	//	{
	//		MoveP.Animation->NextAnimID = MoveP_Idle;
	//		MoveP.AnimeChange = true;
	//	}
	//	break;
	//case MoveP_NoDefendImpact:
	//	break;
	//case MoveP_DefendImpact:
	//	break;
	//case MoveP_Roll:
	//	if (MoveP.IsRoll)
	//	{
	//		MoveP.Pos.x -= sinf(MoveP.Rot.y) * 4.0f;
	//		MoveP.Pos.z -= cosf(MoveP.Rot.y) * 4.0f;

	//		MoveP.IsDamage1 = false;
	//		MoveP.IsDamage2 = false;
	//		MoveP.IsDamage3 = false;
	//	}
	//	break;
	//default:
	//	break;
	//}
}

//=============================================================================
// �v���C���[�̃|�C���^���l������
//=============================================================================
MOVEP *GetMovePAdr(void)
{
	return &MoveP;
}

//=============================================================================
// CallbackKeys��������(�v���C���[)
//=============================================================================
HRESULT InitCallbackKeys_MoveP(void)
{
	ACTIONEVENTSETING *AES = GetAESPAdr();

	// �ݒu����L�[�̐�
	int SetNum = MoveP.Animation->AnimeSetNum;
	float AnimLoopTime = 0.0f;

	for (int Set_No = 0; Set_No < SetNum; Set_No++)
	{
		// �L�[�f�[�^���N���A����
		memset(&AES->Keydata[0], 0, sizeof(KEYDATA) * Keydata_Max);
		AES->Keydata_No = 0;

		switch (Set_No)
		{
		case MoveP_Attack:
			AddKeydata(0.9f, MovePAttackEnd);  //�A�j���̎��Ԃɂ���ăC�x���g�𔭐����܂�
			break;
		default:
			continue;
		}

		if (FAILED(
			SetupCallbackKeys(&AES->Keydata[0], MoveP.Animation->AnimData[Set_No].SetName,
				AES->Keydata_No, "MoveP")))
		{
			return E_FAIL;
		}

	}


	return S_OK;
}


