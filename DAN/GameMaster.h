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

	const float OPENING_TIME			= 5.0f;					//5�b
	const float GAME_TIME				= 60.0f * 1.0f;			//4��
	const float COUNT_DOWN_TIME			= 3.0f;					//3�b
	const float ENDING_TIME				= 3.0f;					//3�b

	const int	PLAYER_NUM				= 2;

	const int	ACHIEVEMENT_GREENING_RATE_10	= 0x00000001;//�Ή���10%
	const int	ACHIEVEMENT_GREENING_RATE_30	= 0x00000002;//�Ή���30%
	const int	ACHIEVEMENT_GREENING_RATE_50	= 0x00000004;//�Ή���50%
	const int	PASSING_REMAINING_ONE_MINUTE	= 0x00000008;//�c�莞��1��
	const int	PASSING_GAME_OPENING			= 0x00000010;//OP�I��
	const int	PASSING_COUNT_DOWN_THREE		= 0x00000020;//3�J�E���g�_�E��
	const int	PASSING_COUNT_DOWN_TWO			= 0x00000040;//2�J�E���g�_�E��
	const int	PASSING_COUNT_DOWN_ONE			= 0x00000080;//1�J�E���g�_�E��
	const int	PASSING_GAME_START				= 0x00000100;//�Q�[���X�^�[�g
	const int	PASSING_REMAINING_10			= 0x00000200;//�c��10�`��
	const int	PASSING_REMAINING_9				= 0x00000400;//�c��9�`��
	const int	PASSING_REMAINING_8				= 0x00000800;//�c��8�`��
	const int	PASSING_REMAINING_7				= 0x00001000;//�c��7�`��
	const int	PASSING_REMAINING_6				= 0x00002000;//�c��6�`��
	const int	PASSING_REMAINING_5				= 0x00004000;//�c��5�`��
	const int	PASSING_REMAINING_4				= 0x00008000;//�c��4�`��
	const int	PASSING_REMAINING_3				= 0x00010000;//�c��3�`��
	const int	PASSING_REMAINING_2				= 0x00020000;//�c��2�`��
	const int	PASSING_REMAINING_1				= 0x00040000;//�c��1�`��
	const int	PASSING_GAME_FINISH				= 0x00080000;//�Q�[���I��
	const int	PASSING_GAME_ENDING				= 0x00100000;//ED�I��
	

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
	bool						playBacked;			//���U���g�Đ�(����:true)
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
	float					openingTimer;									//�I�[�v�j���O����
	float					gameTimer;										//�Q�[������
	float					countDownTimer;									//�J�E���g�_�E������
	bool					pause;											//�|�[�Y
	bool					gameTimerStop;									//�Q�[���^�C�}�[�X�g�b�v

	//�؊֌W���
	int						treeNum;										//�͖؁E�Ή��؂̑��v
	LinkedList<TreeTable>	treeTableList;									//�ϊ����ԁF�Ή����ꂽ����
	
	//�G�l�~�[���ސ�
	int						killEnemyNum[gameMasterNS::PLAYER_NUM];			//���ރG�l�~�[��

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

	void updateTimer(float frameTime);								//�e�^�C�}�[�X�V
	void updateOpeningTime(float frameTime);						//�Q�[�����Ԃ̍X�V
	void updateGameTime(float frameTime);							//�Q�[�����Ԃ̍X�V
	void updateStartCountDown(float frameTime);						//�Q�[���J�n�J�E���g�_�E���̍X�V
	void updateFinishCountDown(float frameTime);						//�Q�[���J�n�J�E���g�_�E���̍X�V
	bool paused();													//�|�[�Y����
	bool playActionRamaining1Min();									//�c��1���o�ߎ��̃A�N�V����
	bool playActionStartCount(int countNum);						//�J�n�J�E���g3���̃A�N�V����
	bool playActionFinishCount(int countNum);						//�I���J�E���g3���̃A�N�V����

	//�؂̐ݒ�֐�
	void setTreeNum(int num);										//�c���[�̐���ۑ�����
	void discardConversionOrder();									//�ϊ����ԕϐ���j������
	void recordTreeTable(TreeTable treeTable);						//�Ή������؂̖{�����L�^
	int	 getGreeningRate();											//�Ή����̎擾
	int  getGreeningTreeNum(int playerNo);							//�Ή��{��
	int  getKillEnemyNum(int playerNo);								//���ރG�l�~�[��
	
	//�Đ��C�x���g�̎擾
	//���擾�������X�g�͕K��SAFE_DELETE_ARRAY���邱��
	//TreeTable* out �F�|�C���^��������ƃ��X�g���i�[�����B
	//float		 time�F�o�ߎ��Ԃ�������
	int getEventList(TreeTable** out,float time);

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