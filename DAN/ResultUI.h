//============================
// [ResultUI.h]
// ����@�W��
//============================
#pragma once
//============================
//�C���N���[�h
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "UIcharacter.h"
#include "UItex.h"
#include "UIrank.h"
#include "UInumber.h"
#include "ResultBG.h"
#include "Player.h"

//============================
//���O��� �萔
//============================
namespace resultUiNS
{
	
	//���U���g�̃t�F�C�Y
	enum RESULT_PHASE
	{
		PHASE_01,		//���U���g���S�̃X���C�h�C��
		PHASE_02,		//���\��
		PHASE_03,		//�����\��
		PHASE_04,		//�����̃J�E���g�A�b�v
		PHASE_05,		//�����N�̕\��
		PHASE_MAX,		//�t�F�C�Y�̑���
	};

	//�v���C���[�̎��
	enum PLAYER_TYPE
	{
		PLAYER_01,
		PLAYER_02,
		PLAYER_MAX,
	};

}
//============================
//�N���X��`
//============================
class ResultUI
{
public: //�����o�[�ϐ�
	UIcharacter* uiCharacter01;		//�v���C���[�P�p������UI
	UIcharacter* uiCharacter02;		//�v���C���[2�p������UI
	UItexture	uiTexture;			//�e�N�X�`����UI
	UIrank*     uiRank;			//�v���C���[�P�̃����N��UI
	UIrank      uiRank02;			//�v���C���[2�̃����N��UI
	UInumber	uiNumber[uiNumberNS::NUMBER_TYPE_MAX];//����
	ResultBG*   resultBG;			//���U���g�w�i
	Player*		player;				//�v���C���[�̏��
	int			score01;			//�v���C���[�P�̃X�R�A�ۑ��p
	int			score02;			//�v���C���[�Q�̃X�R�A�ۑ��p
	int			greenigPersent;		//�S�̗Ή���
	int         defeat01;			//���j��
	int         defeat02;			//���j��
	int			greeningNum01;		//�Ή��{��
	int			greeningNum02;		//�Ή��{��
	int			resultPhase;		//�t�F�C�Y�Ǘ��p
	float		time;				//�t�F�C�Y�ڍs���鎞��
	int			rank01;				//�v���C���[�P�̃����N�Ǘ��p
	int         rank02;				//�v���C���[2�̃����N�Ǘ��p
	bool		playedBGM;			//BGM�Đ��p�t���O
public: //�����o�[�֐�
	ResultUI();						//�R���X�g���N�^
	~ResultUI();					//�f�X�g���N�^
	void initialize(/*Player *player*/);//������
	void render();					//�`��
	void update(float flameTime);	//�X�V
	void uninitialize();			//�I��
	void decidionBGM();
	int decisionRank(int greening, int greenNum, int defeat);
	
};

