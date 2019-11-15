//=============================================================================
//
// �A�j���[�V�������� [D3DXAnimation.cpp]
// Author : HAL���� ���q�
//
//=============================================================================

#include "ActionEventSeting.h"
#include "movep.h"



ACTIONEVENTSETING ActionEventSeting;

//=============================================================================
// �L�[�f�[�^��ݒu����
//=============================================================================
void AddKeydata(float Time, int KeyType)
{
	if (ActionEventSeting.Keydata_No > Keydata_Max)
	{
		MessageBox(0, "Keydata_Max is too samll", "Error", 0);
		return;
	}
	else
	{
		ActionEventSeting.Keydata[ActionEventSeting.Keydata_No] = { Time,KeyType };
		ActionEventSeting.Keydata_No++;
	}

	return;
}


//=============================================================================
// �A�j���[�V�������f�C�x���g�̃L�[��ݒu����
//=============================================================================
HRESULT SetupCallbackKeys(KEYDATA *Keydata, LPCSTR SetName, int CallbacksNum, const char *Owner)
{
	D3DXANIMATION *Animation = NULL;
	// Keyframe��ݒu����\���AnimationSet
	ID3DXKeyframedAnimationSet* AnimSetTemp = NULL;
	// Keyframe��ݒu�����V����AnimationSet
	ID3DXCompressedAnimationSet* CompressedAnimSet = NULL;
	// ���k���ꂽ����
	ID3DXBuffer* CompressedInfo = NULL;
	// �G���[���b�Z�[�W	
	char Message[256] = { NULL };
	double Ticks = 0.0;
	double Period = 0.0;
	// �ݒu����Callback Keys
	D3DXKEY_CALLBACK *Keys = NULL;


	//Sample of SetupCallbackKeys
	MOVEP *MoveP = GetMovePAdr();
	if (strcmp(Owner, "MoveP") == 0)
	{
		Animation = MoveP->Animation;
		if (FAILED(Animation->AnimController->GetAnimationSetByName(SetName, (ID3DXAnimationSet**)&AnimSetTemp)))
		{
			sprintf(Message, "Setup Callbacks in %s AnimationSet Failed�I", SetName);
			goto FunctionExit;

		}
	}
	else
	{
		sprintf(Message, "Can't find this AnimationSet Owner");
		goto FunctionExit;
	}


	// ���k������n��
	AnimSetTemp->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, 0, &CompressedInfo);

	// 1�b�̃L�[�t���C���̐���Ԃ�(1�b = 4800)
	Ticks = AnimSetTemp->GetSourceTicksPerSecond();
	// �A�j���[�V�����̃��[�v����
	Period = AnimSetTemp->GetPeriod();

	// ��������z��
	Keys = (D3DXKEY_CALLBACK*)calloc(CallbacksNum, sizeof(D3DXKEY_CALLBACK));
	if (Keys == NULL)
	{
		sprintf(Message, "Alloc Keys in %s AnimationSet Failed�I", SetName);
		goto FunctionExit;
	}

	// Callback Keys��ݒu����
	for (int i = 0; i < CallbacksNum; i++)
	{
		if (Keydata[i].KeyType == NoEvent)
		{
			sprintf(Message, "There is NoEvent Keydata in %s AnimationSet�I", SetName);
			goto FunctionExit;
		}
		Keys[i].Time = (float)(Period * Ticks * Keydata[i].Time);
		Keys[i].pCallbackData = (void*)Keydata[i].KeyType;
	}

	// ���k����A�j���[�V�����Z�b�g���쐬����
	D3DXCreateCompressedAnimationSet(AnimSetTemp->GetName(),
		AnimSetTemp->GetSourceTicksPerSecond(),
		AnimSetTemp->GetPlaybackType(), CompressedInfo, CallbacksNum, Keys, &CompressedAnimSet);

	// �Â��A�j���[�V�����Z�b�g(���k����Ȃ�)���폜����
	Animation->AnimController->UnregisterAnimationSet(AnimSetTemp);

	// �V�����A�j���[�V�����Z�b�g(���k���ꂽ)��ǉ�����
	Animation->AnimController->RegisterAnimationSet(CompressedAnimSet);

	// �I������
FunctionExit:

	SAFE_RELEASE(CompressedInfo);
	SAFE_RELEASE(AnimSetTemp);
	SAFE_RELEASE(CompressedAnimSet);
	SAFE_FREE(Keys);


	if (Message[0] == NULL)
	{
		return S_OK;
	}
	else
	{
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}
}


//=============================================================================
// �A�j���Đ��̒��f�C�x���g
//=============================================================================
HRESULT AnimCallBackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	static float PreActionSpeed = 0.0f;
	int KeyType = (int)pCallbackData;


	//event�̐ݒu
	MOVEP *MoveP = GetMovePAdr();
	switch (KeyType)
	{
	case MovePAttackEnd:
		//MoveP->Pos.x += 1.0f;
			break;
	default:
		break;
	}

	return D3D_OK;
}

//=============================================================================
// �v���C���[�̃|�C���^���l������
//=============================================================================
ACTIONEVENTSETING *GetAESPAdr(void)
{
	return &ActionEventSeting;
}
