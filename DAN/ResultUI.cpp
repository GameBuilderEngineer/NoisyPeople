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
}

//============================
//�f�X�g���N�^
//============================
ResultUI::~ResultUI()
{
	delete resultBG;
	delete uiCharacter01;
	delete uiCharacter02;
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
	greenigPersent = 0;

	//�Ή��{��
	greeningNum01 = 0;
	greeningNum02 = 0;

	//���j��
	defeat01 = 0;
	defeat02 = 0;

	//�����N�̊m��(��)
	score01 = greenigPersent + greeningNum01/10 + defeat01;//�S�̗Ή���+�Ή��{������P�O+���j��(���j
	score02 = greenigPersent + greeningNum02/10 + defeat02;//�S�̗Ή���+�Ή��{������P�O+���j��(���j

	//**********************************************
	//70�ȉ���FAILED,70�ȏ�W�O�ȉ�CLEARE         //
	//80�ȏ�90�ȉ�GREAT,90�ȏ��EXCELLENT         //
	//**********************************************
	//�v���C���[�P�̃����N�̊m��
	if (score01 < 70)
	{
		rank01 = uiRankNS::UIRANK_TYPE::FAILED;
	}
	else if (score01 >= 70 && score01 < 80)
	{
		rank01 = uiRankNS::UIRANK_TYPE::CLEARE;
	}
	else if (score01 >= 70 && score01 < 90)
	{
		rank01 = uiRankNS::UIRANK_TYPE::GREAT;
	}
	else if(score01 >= 90)
	{
		rank01 = uiRankNS::UIRANK_TYPE::EXCELLENT;
	}
	
	//�v���C���[2�̃����N�̊m��
	if (score02 < 70)
	{
		rank02 = uiRankNS::UIRANK_TYPE::FAILED;
	}
	else if (score02 >= 70 && score02 < 80)
	{
		rank02 = uiRankNS::UIRANK_TYPE::CLEARE;
	}
	else if (score02 >= 70 && score02 < 90)
	{
		rank02 = uiRankNS::UIRANK_TYPE::GREAT;
	}
	else if (score02 >= 90)
	{
		rank02 = uiRankNS::UIRANK_TYPE::EXCELLENT;
	}

	//����UI�̏�����
	uiCharacter01->initialize(PLAYER_01);//�v���C���[1
	uiCharacter02->initialize(PLAYER_02);//�v���C���[2

	//�e�N�X�`��UI�̏�����
	uiTexture.initialize();
	
	//�����NUI�̏�����
	uiRank01.initialize(rank01, PLAYER_01);	//�v���C���[�P
	uiRank02.initialize(rank02, PLAYER_02);	//�v���C���[2

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
void ResultUI::render()
{
	resultBG->render();					//���U���g�w�i�̕`��
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
	//�����N�\��
	if (resultPhase == PHASE_05)
	{
		uiRank01.render(rank01);		//�v���C���[�P�̃����N�`��
		uiRank02.render(rank02);		//�v���C���[�Q�̃����N�`��
										//�����̕\��
		for (int i = 0; i < uiNumberNS::NUMBER_TYPE_MAX; i++)
		{
			uiNumber[i].render();
			
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
	if (time > 2.0f)
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
		uiRank01.update(rank01);
		uiRank02.update(rank02);
		//�Đ�
		if (playedBGM)
		{
			decidionBGM();
		}
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
	uiRank01.uninitialize(rank01);
	uiRank02.uninitialize(rank02);

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
	if (score01 >=70 || score02 >= 70)
	{
		PLAY_PARAMETERS playParameters = { 0 };//�����ɍĐ���������
		//�Đ�����Ȃ̎w��T�E���hID,���[�v,�X�s�[�hNULL�ł��Ȃ�,��{false,��{NULL,�t�B���^�[���g�����g��Ȃ���
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Clear, true,1.0f,false,NULL};//BGM�̐ݒ�
		 //�Đ�
		SoundInterface::BGM->playSound(&playParameters);
	}
	//�ǂ����70�ȉ��Ȃ玸�s
	else if(score01<70 && score02<70)
	{
		PLAY_PARAMETERS playParameters = { 0 };//�����ɍĐ���������
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_BGM, BGM_LIST::BGM_Failed, true,1.0f,false,NULL};//BGM�̐ݒ�
		SoundInterface::BGM->playSound(&playParameters);//�Đ�
	}
	//1�x�Đ�������t���O���t�H���X
	playedBGM = false;
}