//============================
//[ResultUI.cpp]
//����W��
//============================

//============================
//�C���N���[�h
//============================
#include "ResultUI.h"
#include "Sound.h"

//============================
//�yusing�錾�z
//============================
using namespace resultUiNS;

//============================
//�R���X�g���N�^
//============================
ResultUI::ResultUI()
{
	resultBG = new ResultBG;
	uiCharacter01 = new UIcharacter;//�v���C���[1
	uiCharacter02 = new UIcharacter;;//�v���C���[2
	uiRank = new UIrank;
}

//============================
//�f�X�g���N�^
//============================
ResultUI::~ResultUI()
{
	delete resultBG;
	delete uiCharacter01;
	delete uiCharacter02;
	delete uiRank;
}

//============================
//������
//============================
void ResultUI::initialize()
{

	//BGM�Đ��t���O��true�ŏ�����
	playedBGM = true;

	//�t�F�C�Y�ڍs���Ԃ̏�����
	time = 0;

	//�t�F�C�Y�̏�����(���t�F�C�Y��)
	resultPhase = PHASE_01;

	//�S�̗Ή���
	//greenigPersent = 0;

	//�Ή��{��
	greeningNum01 = 0;
	greeningNum02 = 0;

	//���j��
	defeat01 = 0;
	defeat02 = 0;

	//�����N�̊m��(��)
	score01 = 0;//�S�̗Ή���+�Ή��{������P�O+���j��(���j
	score02 = 0;//�S�̗Ή���+�Ή��{������P�O+���j��(���j

	rank01 = 0;
	rank02 = 0;

	rankType = 0;

	//����UI�̏�����
	uiCharacter01->initialize(PLAYER_01);//�v���C���[1
	uiCharacter02->initialize(PLAYER_02);//�v���C���[2

	//�e�N�X�`��UI�̏�����
	uiTexture.initialize();

	//�����N�̏�����
	uiRank->initialize();
	

	//�����̎�ޕ�����������
	for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX ; i++)
	{
		uiNumber[i].initialize(i);

	}

	//���U���g�w�i�̏�����
	resultBG->initialize();
}

//============================
//�`��
//�������߂�����k��������
//============================
void ResultUI::render(bool texFlag)
{
	if (texFlag == true)
	{

		rank01 = decisionRank(greenigPersent, greeningNum01, defeat01);
		rank02 = decisionRank(greenigPersent, greeningNum02, defeat02);
		//resultBG->render();					//���U���g�w�i�̕`��
		uiTexture.render(resultPhase);		//�e�N�X�`���̕`��
		uiCharacter01->render(resultPhase);	//�v���C���[�P�̕����`��
		uiCharacter02->render(resultPhase);	//�v���C���[2�̕����`��

		//�S�̗Ή����̕`��
		if (resultPhase == PHASE_02)
		{
			uiNumber[uiNumberNS::GREENIG_PERSENT].render();
		}

		if (resultPhase == PHASE_03)
		{
			uiNumber[uiNumberNS::GREENIG_PERSENT].render();
		}
		//�����\���̓t�F�C�Y�S����`��
		if (resultPhase == PHASE_04)
		{
			//�����̕\��
			for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
			{
				uiNumber[i].render();
			}
		}
		//�����N�\���t�F�[�Y
		if (resultPhase == PHASE_05)
		{
			//�����N�`��
			uiRank->render(rank01, rank02);
			//�����̕\��
			for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
			{
				uiNumber[i].render();
			
			}
		}
	}
}

//============================
//�X�V
//�t���[���^�C�����������Ĉ�莞�Ԍo�߂Ńt�F�C�Y���X�V
//============================
void ResultUI::update(float flameTime)
{
	time += flameTime;

	//�t�F�C�Y�̍X�V
	if (time > PHASE_TIME)
	{
		resultPhase = PHASE_02;
		uiNumber[uiNumberNS::GREENIG_PERSENT].update(greenigPersent);
	}
	if (time > 5.0f)
	{
		resultPhase= PHASE_03;
	}
	if (time > 10.0f)
	{
		resultPhase = PHASE_04;
		//����
		int score[uiNumberNS::GREENIG_PERSENT] = { greeningNum01,greeningNum02,defeat01,defeat02 };
		for (int i = 0; i < uiNumberNS::GREENIG_PERSENT; i++)
		{
			uiNumber[i].update(score[i]);
		}
		
	}
	if (time > 12.0f)
	{
		resultPhase = PHASE_05;
		//�����N
		uiRank->update(rank01, rank02);
		//�Đ�
		if (playedBGM)
		{
			decidionBGM();
		}
	}
	if (time > 14.0f && gameMaster->wasFinishVoicePlayed[gameMasterNS::PLAYER_1P] == false && score > 70)
	{
		PLAY_PARAMETERS voiceFinish = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, S3D_LIST::Voice_Male_Finish, false, NULL, true, gameMasterNS::PLAYER_1P };
		SoundInterface::S3D->playSound(&voiceFinish);
		gameMaster->wasFinishVoicePlayed[gameMasterNS::PLAYER_1P] = true;

	}
	if (time > 16.0f && gameMaster->wasFinishVoicePlayed[gameMasterNS::PLAYER_2P] == false && score > 70)
	{
		PLAY_PARAMETERS voiceFinish = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, S3D_LIST::Voice_Female_Finish, false, NULL, true, gameMasterNS::PLAYER_2P };
		SoundInterface::S3D->playSound(&voiceFinish);
		gameMaster->wasFinishVoicePlayed[gameMasterNS::PLAYER_2P] = true;
	}

	uiCharacter01->update(resultPhase,PLAYER_01);	//�v���C���[�P�̕����X�V
	uiCharacter02->update(resultPhase,PLAYER_02);	//�v���C���[�P�̕����X�V
	uiTexture.update(resultPhase);		//�e�N�X�`���̍X�V
	
}

//============================
//�I������
//============================
void ResultUI::uninitialize()
{

	uiTexture.uninitialize();

	//�����̎�ޕ������I������
	for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
	{
		uiNumber[i].uninitialize();
	}
}

//=============================
//�炷BGM�����肵�炷����
//=============================
void ResultUI::decidionBGM()
{
	//�ǂ��炩��70�𒴂��Ă����Ȃ�N���A
	if (greenigPersent >=70)
	{
		PLAY_PARAMETERS playParameters = { 0 };//�����ɍĐ���������
		//�Đ�����Ȃ̎w��T�E���hID,���[�v,�X�s�[�hNULL�ł��Ȃ�,��{false,��{NULL,�t�B���^�[���g�����g��Ȃ���
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Clear, true,1.0f,false,NULL};//BGM�̐ݒ�
		 //�Đ�
		SoundInterface::BGM->playSound(&playParameters);
	}
	//�ǂ����70�ȉ��Ȃ玸�s
	else if(greenigPersent<70)
	{
		PLAY_PARAMETERS playParameters = { 0 };//�����ɍĐ���������
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Failed, true,1.0f,false,NULL};//BGM�̐ݒ�
		SoundInterface::BGM->playSound(&playParameters);//�Đ�
	}
	//1�x�Đ�������t���O���t�H���X
	playedBGM = false;
}

//========================================
//�����N�v�Z�֐�
//�����F�S�̗Ή����A�Ή��{���A���j��
//========================================
int ResultUI::decisionRank(int greenigPersent, int greeningNum, int defeat)
{
	//�����N�̊m��(��)
	 score = greenigPersent + (greeningNum / 10 )+ defeat;//�S�̗Ή���+�Ή��{������P�O+���j��(���j
	 rankType =0;
	//**********************************************
	//70�ȉ���FAILED,70�ȏ�W�O�ȉ�CLEARE         //
	//80�ȏ�90�ȉ�GREAT,90�ȏ��EXCELLENT         //
	//**********************************************

 //�v���C���[�P�̃����N�̊m�� �����|�C���g
  if (score < 70)
   {
   	rankType = uiRankNS::UIRANK_TYPE::FAILED;
	return rankType;
   }
   else if (score >= 70 && score < 80)
   {
   	rankType = uiRankNS::UIRANK_TYPE::CLEARE;
	return rankType;
   }
   else if (score >= 70 && score < 90)
   {
   	rankType = uiRankNS::UIRANK_TYPE::GREAT;
	return rankType;
   }
   else if(score >= 90)
   {
   	rankType = uiRankNS::UIRANK_TYPE::EXCELLENT;
	return rankType;
   }
	
}