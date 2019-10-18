//===================================================================================================================================
//�yGameMaster.h�z
// [�쐬��] HAL����GP12A332 11 ���� ��
// [�쐬��] 2019/09/20
// [�X�V��] 2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
//#include "StaticMeshLoader.h"
//#include "Player.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace gameMasterNS {
	const float GAME_TIME = 60.0f * 4.0f;	//4��
	const float COUNT_DOWN_TIME = 3.0f;		//3�b

	enum PLAYER_TYPE
	{
		PLAYER_CHARACTER,
		NON_PLAYER_CHARACTER,
		TYPE_NUM
	};

	enum RESULT_LIST
	{
		WIN_1P,
		WIN_2P,
		DRAW,
	};

	enum MODEL_TYPE
	{
		ADAM,
		EVE,
		MODEL_TYPE_MAX
	};

	const int MODEL_ADAM = MODEL_TYPE::ADAM;
	const int MODEL_EVE = MODEL_TYPE::EVE;

	const int COUNT_DOWN = 3;
	const float COUNT_INTERVAL = 1.0f;
	const float START_INTERVAL = 0.5f;
	const float DISPLAY_START_TIME = 1.0f;
	const float DISPLAY_FINISH_TIME = 1.0f;
}
//===================================================================================================================================
//�y�\���̒�`�z
//===================================================================================================================================
//�v���C���[���
struct playerTable
{
	int playerType;		//PC|NPC
	int modelType;		//�v���C���[�̃��f���^�C�v
	int wage;			//�`���M��
};

//===================================================================================================================================
//�y�Q�[���Ǘ��N���X�z
// [�p�r]	���̃N���X�́A�V�[�����܂����ŏ��̋��n�����s���B
//			�X�V��ݒ�́A�e�V�[���ōs���B
//===================================================================================================================================
class GameMaster :public Base
{
private:
	//Data
	float gameTimer;										//�Q�[������
	float countDownTimer;									//�J�E���g�_�E������
	float startInterval;									//�J�n�J�E���g�_�E���C���^�[�o��
	float displayStartTimer;								//�X�^�[�g�A�i�E���X�\������
	float displayFinishTimer;								//�t�B�j�b�V���A�i�E���X�\������
	bool alreadyStart;										//�X�^�[�g��
	bool alreadyFinish;										//�t�B�j�b�V����
	int count;												//�Q�[���J�E���g
	//playerTable playerInformation[playerNS::NUM_PLAYER];	//�v���C���[���
	//int chingin[playerNS::NUM_PLAYER];						//	�`���M��
	bool countFlag;
	bool startFinishFlag;
public:
	//Method
	GameMaster();
	~GameMaster();
	void initialize();
	void update(float frameTime);
	void updateGameTime(float frameTime);					//�Q�[�����Ԃ̍X�V
	void gameStartCount(float frameTime);					//�Q�[���J�n���̃J�E���g�_�E��
	void gameFinishCount(float frameTime);					//�Q�[���I�����̃J�E���g�_�E��
	void setCountDown();									//�J�E���g�_�E���̃Z�b�g
	void gameStart();										//�Q�[���J�n����

	//setter
	//void setPlayerCharacter(int playerNo, int playerType, int modelType);	//�L�����N�^�[�����Z�b�g

	//getter
	//playerTable* getPlayerInfomation();
	float getGameTime();
	int getCount();
	bool getCountFlag();
	bool whetherGameOver();
	bool whetherAlreadyStart();
	bool whetherAlreadyFinish();
	bool whetherCountFinish();
	bool displayStart();		//�X�^�[�g�\�L�\������
	bool displayFinish();		//�t�B�j�b�V���\�L�\������

};