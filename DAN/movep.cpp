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
#include "Player.h"
#include "Input.h"


//�v���C���[
MOVEP MoveP;
using namespace playerNS;
bool initializedMoveP = false;
float wi = 0.0f;

//=============================================================================
// �v���C���[�̏���������
//=============================================================================
HRESULT InitMoveP(D3DXVECTOR3 Rot, D3DXVECTOR3 Scl, bool FirstInit)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	//�����ݒ�
	MoveP.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MoveP.Rot = Rot;
	MoveP.Quaternion = D3DXQUATERNION(0, 0, 0, 1);
	MoveP.Scl = Scl;
	MoveP.ActionSpeed = 1.25f;
	MoveP.IsDie = false;
	MoveP.IsDeath = false;


	// ���߂ď�����
	//if (FirstInit == true)
	if(!initializedMoveP)
	{
		//�K�v 1
		MoveP.pAllocateHier = new CAllocateHierarchy();

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
			//case MoveP_Idle:
			//	MoveP.Animation->AnimData[Set_No] = { "Idle",			NULL, 0.3f,	0.0f };
			//	break;
			//case MoveP_Run:
			//	MoveP.Animation->AnimData[Set_No] = { "Runing",		NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_FireIdle:
			//	MoveP.Animation->AnimData[Set_No] = { "FireIdle",		NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_FireRun:
			//	MoveP.Animation->AnimData[Set_No] = { "FireRun",	NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_Jump:
			//	MoveP.Animation->AnimData[Set_No] = { "SmallJump",		NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_JumpFire:
			//	MoveP.Animation->AnimData[Set_No] = { "JumpFire",		NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_Die:
			//	MoveP.Animation->AnimData[Set_No] = { "Death",	NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_WalkRight:
			//	MoveP.Animation->AnimData[Set_No] = { "WalkRight",	NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_WalkLeft:
			//	MoveP.Animation->AnimData[Set_No] = { "WalkLeft",	NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_WalkBackwards:
			//	MoveP.Animation->AnimData[Set_No] = { "WalkBackwards",	NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_WalkBackward:
			//	MoveP.Animation->AnimData[Set_No] = { "WalkBackward",	NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_WalkStrafeLeft:
			//	MoveP.Animation->AnimData[Set_No] = { "WalkStrafeLeft",	NULL, 0.1f,	0.0f };
			//	break;
			//case MoveP_WalkStrafeRight:
			//	MoveP.Animation->AnimData[Set_No] = { "WalkStrafeRight",	NULL, 0.1f,	0.0f };
			//	break;



			case MoveP_Idle:
				MoveP.Animation->AnimData[Set_No] = { "Idle",			NULL, 0.3f,	0.0f };
				break;
			case MoveP_Run:
				MoveP.Animation->AnimData[Set_No] = { "Run",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_FireIdle:
				MoveP.Animation->AnimData[Set_No] = { "FireIdle",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_FireRun:
				MoveP.Animation->AnimData[Set_No] = { "FireRun",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_WalkLeft:
				MoveP.Animation->AnimData[Set_No] = { "FireLeft",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_WalkRight:
				MoveP.Animation->AnimData[Set_No] = { "FireRight",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_WalkBackwards:
				MoveP.Animation->AnimData[Set_No] = { "FireBack",	NULL, 0.1f,	0.0f };
				break;
			case MoveP_JumpFire:
				MoveP.Animation->AnimData[Set_No] = { "FireJump",		NULL, 0.1f,	0.0f };
				break;
			case MoveP_Die:
				MoveP.Animation->AnimData[Set_No] = { "Death",	NULL, 0.1f,	0.0f };
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
		ChangeAnimation(MoveP.Animation, MoveP_Idle, 1.0f, false);

		initializedMoveP = true;
	}

	MoveP.Animation->MotionEnd = true;

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
	Input *input = getInput();
	const BYTE BUTTON_JUMP = virtualControllerNS::B;
	const BYTE BUTTON_ATTACK = virtualControllerNS::R1;

	AnimCallBackHandler CallBackHandler;

	CallBackHandler.AnimPointer = MoveP.Animation;

	if (input->getMouseLButton() || input->getController()[gameMasterNS::PLAYER_1P]->isButton(BUTTON_ATTACK))
	{
		MoveP.IsChange = true;
	}
	else
	{
		MoveP.IsChange = false;
	}

	if ((input->getMouseRButtonTrigger() || input->getController()[gameMasterNS::PLAYER_1P]->wasButton(BUTTON_JUMP)) && !MoveP.IsJump && MoveP.IsGround )
	{
		MoveP.IsJump = true;
		wi = 0.24f;
	}


	//�ړ�����
	if (!MoveP.IsChange)
	{
		MoveP.IsChangeRun = false;
		MoveP.Animation->NextAnimID = MoveP_Idle;

		if (MoveP.IsGround)
		{

			if (input->isKeyDown('W') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().y < 0.0f)
			{
				MoveP.IsRun = true;
				MoveP.Animation->NextAnimID = MoveP_Run;
			}
			else if (input->isKeyDown('S') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().y > 0.0f)
			{
				MoveP.IsRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkBackwards;
			}
			else if (input->isKeyDown('A') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().x < 0.0f)
			{
				MoveP.IsRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkLeft;
			}
			else if (input->isKeyDown('D') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().x > 0.0f)
			{
				MoveP.IsRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkRight;
			}
			else
			{
				MoveP.IsRun = false;
			}
		}
		//if (MoveP.IsRun)
		//{
		//	MoveP.Animation->NextAnimID = MoveP_Run;
		//}

	}
	if (MoveP.IsChange)
	{
		MoveP.IsRun = false;

		MoveP.Animation->NextAnimID = MoveP_FireIdle;

		if (MoveP.IsGround)
		{

			if (input->isKeyDown('W') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().y < 0.0f)
			{
				MoveP.IsChangeRun = true;
				MoveP.Animation->NextAnimID = MoveP_FireRun;
			}
			else if (input->isKeyDown('S') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().y > 0.0f)
			{
				MoveP.IsChangeRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkBackwards;
			}
			else if (input->isKeyDown('A') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().x < 0.0f)
			{
				MoveP.IsChangeRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkLeft;
			}
			else if (input->isKeyDown('D') || input->getController()[gameMasterNS::PLAYER_1P]->getLeftStick().x > 0.0f)
			{
				MoveP.IsChangeRun = true;
				MoveP.Animation->NextAnimID = MoveP_WalkRight;
			}
			else
			{
				MoveP.IsChangeRun = false;
			}
		}
	}

	if (MoveP.IsJump)
	{
		MoveP.Animation->NextAnimID = MoveP_JumpFire;
	}

	//if (input->isKeyDown('Q'))
	//{
	//	MoveP.Animation->NextAnimID = MoveP_Die;
	//	MoveP.IsDie = true;
	//}

	if (MoveP.IsDeath)
	{
		MoveP.Animation->AnimController->SetTrackPosition(0, 3.8f);
	}

	if (MoveP.IsJumpEnd)
	{
		if (MoveP.IsGround)
		{
			MoveP.IsJumpEnd = false;
			MoveP.IsJump = false;
			MoveP.IsFireJump = false;
			MoveP.Animation->MotionEnd = true;
		}
		if (!MoveP.IsGround)
		{
			MoveP.Animation->AnimController->SetTrackPosition(0, wi);
		}
	}

	if (MoveP.GroundDistance > 0 && MoveP.GroundDistance < 2.5f)
	{
		wi += 0.01f;
	}


	MovePAnimeCur();
	MovePAnimeNext();

	//�A�j���I������Ȃ�A�A�j���̓`�F���W�ł��܂�
	if (MoveP.Animation->MotionEnd)
	{
		MoveP.AnimeChange = true;
	}

	MoveP.RHand = GetBoneMatrix(MoveP.Animation, "right_hand");
	MoveP.RHandPos = D3DXVECTOR3(MoveP.RHand._41, MoveP.RHand._42, MoveP.RHand._43);
	// �A�j���[�V�������X�V 
	//�K������Ă��������A�����Ȃ��ƁA���悪�����Ȃ�
	UpdateAnimation(MoveP.Animation, f_TimeDelta * MoveP.ActionSpeed);

	//MoveP.fTimeDelta = f_TimeDelta;
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

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, MoveP.Scl.x, MoveP.Scl.y, MoveP.Scl.z);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxScl);

	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, MoveP.Rot.y, MoveP.Rot.x, MoveP.Rot.z);
	//D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxRot);


	D3DXMatrixRotationQuaternion(&mtxRot, &MoveP.Quaternion);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxRot);


	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, MoveP.Pos.x, MoveP.Pos.y, MoveP.Pos.z);
	D3DXMatrixMultiply(&MoveP.WorldMatrix, &MoveP.WorldMatrix, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &MoveP.WorldMatrix);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetTexture(0, NULL);

	// �A�j���[�V������`�悷��
	DrawAnimation(MoveP.Animation, &MoveP.WorldMatrix, false);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

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
	if (MoveP.AnimeChange)
	{
		switch (MoveP.Animation->NextAnimID)
		{
		case MoveP_Idle:
			ChangeAnimation(MoveP.Animation, MoveP_Idle, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_Idle;
			MoveP.Animation->MotionEnd = true;	//�����ݒ肵�Ȃ��ƁA�҂̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP_Run:
			ChangeAnimation(MoveP.Animation, MoveP_Run, 0.8f, false);
			MoveP.Animation->NextAnimID = MoveP_Idle;
			MoveP.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP_FireIdle:
			ChangeAnimation(MoveP.Animation, MoveP_FireIdle, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP_FireRun:
			ChangeAnimation(MoveP.Animation, MoveP_FireRun, 0.8f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP_JumpFire:
			ChangeAnimation(MoveP.Animation, MoveP_JumpFire, 0.5f, false);
			if (MoveP.IsChange)
			{
				MoveP.Animation->NextAnimID = MoveP_FireRun;
			}
			if (!MoveP.IsChange)
			{
				MoveP.Animation->NextAnimID = MoveP_Idle;
			}
			break;
		case MoveP_Die:
			ChangeAnimation(MoveP.Animation, MoveP_Die, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_Idle;
			break;
		case MoveP_WalkRight:
			ChangeAnimation(MoveP.Animation, MoveP_WalkRight, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP_WalkLeft:
			ChangeAnimation(MoveP.Animation, MoveP_WalkLeft, 1.0f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		case MoveP_WalkBackwards:
			ChangeAnimation(MoveP.Animation, MoveP_WalkBackwards, 1.5f, false);
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.Animation->MotionEnd = true;//�����ݒ肵�Ȃ��ƁA����̓��슮�����Ȃ��ƁA�ʂ̓��쑱���Ȃ�
			break;
		default:
			break;
		}

		MoveP.AnimeChange = false;

	}
}

//=============================================================================
// �v���C���[�̌��݂̃A�j���ݒ�
//=============================================================================
void MovePAnimeCur(void)
{
	Input *input = getInput();

	switch (MoveP.Animation->CurrentAnimID)
	{
	case MoveP_Idle:
		break;
	case MoveP_Run:
		if (!MoveP.IsRun)
		{
			MoveP.Animation->NextAnimID = MoveP_Idle;
			MoveP.AnimeChange = true;
		}
		break;
	case MoveP_FireIdle:
		break;
	case MoveP_FireRun:
		if (!MoveP.IsChangeRun)
		{
			MoveP.Animation->NextAnimID = MoveP_FireIdle;
			MoveP.AnimeChange = true;
		}
		break;
	case MoveP_JumpFire:
		break;
	case MoveP_Die:
		MoveP.Animation->NextAnimID = MoveP_Idle;
		break;
	default:
		break;
	}
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
		case MoveP_Idle:
			break;
		case MoveP_Run:
			AddKeydata(0.33f, MovePMoveVoice);
			AddKeydata(0.68f, MovePMoveVoice);
			AddKeydata(0.95f, MotionEnd);
			break;
		case MoveP_FireIdle:
			break;
		case MoveP_FireRun:
			AddKeydata(0.95f, MotionEnd);
			break;
		case MoveP_JumpFire:
			AddKeydata(0.01f, MovePJumpFireStart);
			AddKeydata(0.3f, MovePJumpFireEnd);
			break;
		case MoveP_Die:
			AddKeydata(0.95f, MovePDeath);
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

