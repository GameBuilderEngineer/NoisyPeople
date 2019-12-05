//===================================================================================================================================
//�yGameMaster.h�z
// [�쐬��] HAL����GP12A332 11 ���� ��
// [�쐬��] 2019/09/20
// [�X�V��] 2019/10/24
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshLoader.h"
#include "LinkedList.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace gameMasterNS {

	enum PLAYER_TYPE
	{
		PLAYER_1P,
		PLAYER_2P,
		TYPE_NUM
	};

	const int MODEL_MALE		= staticMeshNS::YAMADA_ROBOT;
	const int MODEL_FEMALE		= staticMeshNS::YAMADA_ROBOT2;
	const int MODEL_TYPE_MAX = 2;

	enum RESULT_LIST
	{
		CLEAR,
		FAILED,
		RESULT_NUM,
	};

	enum PLAYER_RANK
	{
		RANK_S,
		RANK_A,
		RANK_B,
		RANK_C,
		RANK_NUM
	};



	const float GAME_TIME				= 60.0f * 4.0f;			//4��
	const float COUNT_DOWN_TIME			= 3.0f;					//3�b

	const int	PLAYER_NUM				= 2;

	const int	ACHIEVEMENT_GREENING_RATE_10	= 0x00000001;//�Ή���10%
	const int	ACHIEVEMENT_GREENING_RATE_30	= 0x00000002;//�Ή���30%
	const int	ACHIEVEMENT_GREENING_RATE_50	= 0x00000004;//�Ή���50%
	const int	PASSING_REMAINING_ONE_MINUTE	= 0x00000008;//�c�莞��1��


	enum EVENT_TYPE
	{
		TO_DEAD,				//�͖؂֖߂�
		TO_GREEN_WITH_DIGITAL,	//�Ή�(�f�W�^��)
		TO_GREEN_WITH_ANALOG,	//�Ή�(�A�i���O)
	};
}

//===================================================================================================================================
//�y�\���̒�`�z
//===================================================================================================================================
//�v���C���[���
struct PlayerTable
{
	int	playerType;		//1P|2P
	int	modelType;		//�v���C���[�̃��f���^�C�v
	int greeningTree;	//�Ή������{��
	int sedationEnemy;	//���É������G�l�~�[
};
//�c���[���
struct TreeTable
{
	int							id;					//�c���[ID
	gameMasterNS::EVENT_TYPE	eventType;			//ANALOG|DIGITAL
	int							modelType;			//���f���^�C�v
	int							player;				//�Ή������v���C���[
	float						eventTime;			//�Ή����ꂽ����
	D3DXVECTOR3					position;			//�ʒu
	D3DXQUATERNION				rotation;			//��]
	D3DXVECTOR3					scale;				//�X�P�[��
	
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
	//�Q�[��
	float					gameTimer;										//�Q�[������
	float					countDownTimer;									//�J�E���g�_�E������
	bool					pause;											//�|�[�Y
	bool					gameTimerStop;									//�Q�[���^�C�}�[�X�g�b�v

	//�؊֌W���
	int						treeNum;										//�͖؁E�Ή��؂̑��v
	LinkedList<TreeTable>	treeTableList;									//�ϊ����ԁF�Ή����ꂽ����
	
	//�v���C���[
	PlayerTable				playerInformation[gameMasterNS::PLAYER_NUM];	//�v���C���[���
	int						progress;										//�B����


public:
	//��{����
	GameMaster();
	~GameMaster();
	void initialize();
	void update(float frameTime);

	//�Q�[��
	void startGame();												//�Q�[���J�n�֐�
	void updateGameTime(float frameTime);							//�Q�[�����Ԃ̍X�V
	bool paused();													//�|�[�Y����
	bool playActionRamaining1Min();									//�c��1���o�ߎ��̃A�N�V����

	//�؂̐ݒ�֐�
	void readyTreeTable();											//�ϊ����ԕϐ�����������
	void discardConversionOrder();									//�ϊ����ԕϐ���j������
	void recordTreeTable(TreeTable treeTable);						//�Ή������؂̖{�����L�^


	//setter
	void setConversionOrder(int* newValue);
	void setProgress(int achievement);

	//getter
	PlayerTable*	getPlayerInfomation();
	float			getGameTime();
	int				getProgress();									//�B���󋵎擾
	bool			whetherAchieved(int achievement);

#ifdef _DEBUG
	bool			showGUI;										//GUI�̉��t���O
	void			createGUI();									//GUI�̍쐬
	bool*			getShowGUI() { return &showGUI; };				//GUI�̉��t���Ogetter
#endif
};