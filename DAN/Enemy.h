//-----------------------------------------------------------------------------
// �G�l�~�[���N���X [Enemy.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"
#include "Player.h"
#include "NavigationMesh.h"
#include "StateMachine.h"
#include "Sound.h"
#include "SoundBase.h"


//=============================================================================
// �r���h�X�C�b�`
//=============================================================================
#if 1	// �Z���T�[��`��
#define RENDER_SENSOR
#endif


//=============================================================================
// ���O���
//=============================================================================
#pragma region [Name Space]
namespace enemyNS
{
	// �G�l�~�[�̎��
	enum ENEMY_TYPE
	{
		WOLF,		// code name "WOLF"
		TIGER,		// code name "TIGER"
		BEAR,		// code name "BEAR"
		TYPE_MAX,
	};

	// �G�l�~�[�̍ő�HP�e�[�u��
	const int ENEMY_HP_MAX[TYPE_MAX] =
	{
		40,		// WOLF
		40,		// TIGER
		40,		// BEAR
	};

	// �G�l�~�[�̃Z���T�[�X�V�p�x�e�[�u���ix�b��1�x�X�V�j
	const float SENSOR_UPDATE_INTERVAL[TYPE_MAX] =
	{
		0.3f,		// WOLF
		0.2f,		// TIGER
		0.1f,		// BEAR
	};

	// �G�l�~�[�̎��͋����e�[�u��
	const float VISIBLE_DISTANCE[TYPE_MAX] =
	{
		40.0f,		// WOLF
		40.0f,		// TIGER
		40.0f,		// BEAR
	};

	// �G�l�~�[�̐�������p�e�[�u���iZ�� - �����Б��j
	const float HORIZONTAL_HALF_VIEWING_ANGLE[TYPE_MAX] =
	{
		0.55f,		// WOLF
		0.55f,		// TIGER
		0.55f,		// BEAR
	};

	// �G�l�~�[�̐�������p�e�[�u���iZ�� - �����Б��j
	const float VERTICAL_HALF_VIEWING_ANGLE[TYPE_MAX] =
	{
		0.5f,		// WOLF
		0.5f,		// TIGER
		0.5f,		// BEAR
	};

	// �G�l�~�[�̒��o�����e�[�u��
	const float NOTICEABLE_DISTANCE_PLAYER[TYPE_MAX] =
	{
		3.5f,		// WOLF
		3.5f,		// TIGER
		3.5f,		// BEAR
	};
	const float SHOT_SOUND_SCALE = 4.5f;// �V���b�g�������{��

	// �G�l�~�[�̍U���̓e�[�u��
	const int ATTACK_DAMAGE[TYPE_MAX] =
	{
		5,			// WOLF
		10,			// TIGER
		15,			// BEAR
	};

	// �G�l�~�[�̍U���ړ����x�e�[�u��
	const float ATTACK_SPEED[TYPE_MAX] =
	{
		15.0f,		// WOLF
		15.0f,		// TIGER
		15.0f,		// BEAR
	};

	// �G�l�~�[�̍U���ړ����ԃe�[�u��
	const float ATTACK_TIME[TYPE_MAX] =
	{
		0.3f,		// WOLF
		0.3f,		// TIGER
		0.3f,		// BEAR
	};

	// �G�l�~�[�̍U���Ԋu�e�[�u��
	const float ATTACK_INTERVAL[TYPE_MAX] =
	{
		3.0f,		// WOLF
		3.0f,		// TIGER
		3.0f,		// BEAR
	};

	// �G�l�~�[�̃v���C���[�L�����ԃe�[�u��
	const float PLAYER_MEMORIZED_TIME[TYPE_MAX] =
	{
		4.0f,		// WOLF
		4.0f,		// TIGER
		4.0f,		// BEAR
	};

	// �G�l�~�[�̈ړ������x�e�[�u��
	const float MOVE_ACC[TYPE_MAX] =
	{
		20.0f,		// WOLF
		20.0f,		// TIGER
		20.0f,		// BEAR
	};

	// Physics Constant
	const float AIR_MOVE_ACC_MAGNIFICATION = 0.12f;		// �󒆈ړ������x�{��
	const float STOP_SPEED = 0.5f;						// �ړ���~���x
	const float FALL_SPEED_MAX = 60.0f;					// �����ō����x
	const float MOVE_FRICTION = 0.90f;					// �n�ʖ��C�W��
	const float WALL_FRICTION = 0.98f;					// �ǂ��薀�C�W��
	const float GROUND_FRICTION = 0.25f;				// ���n���C�W��
	const float ATTACKED_FRICTION = 0.2f;				// �U���㖀�C�W��
	const float GRAVITY_FORCE = 9.8f * 2;				// �d��

	// AI Constant
	const float ARRIVAL_DISTANCE = 0.5f;				// ��������
	const float MOVE_LIMIT_TIME = 10.0f;				// �ړ����~�b�g����
	const float PATH_SEARCH_INTERVAL_WHEN_CHASE = 0.3f;	// �ǐՃX�e�[�g���̃p�X�����Ԋu

	// Other Constant
	const float DIFFERENCE_FIELD = 0.05f;				// �t�B�[���h�␳����
	const float JUMP_SPEED = 6.0f;						// �W�����v����
	const float JUMP_CONTROL_SPEED = 1.0f;				// �W�����v�����R���g�[�����x
	const float DASH_MAGNIFICATION = 2.0f;				// �_�b�V���{��
	const float CAMERA_SPEED = 1.0f;					// �J�����̑���

	//-----------------------------------------------------------------
	// EnemyInitialSettingData�N���X�̓G�l�~�[�����X�e�[�^�X��ێ�����
	// �V�K�G�l�~�[�쐬��G�l�~�[�z�u�c�[���Ƃ̃f�[�^�����Ɏg�p����
	//-----------------------------------------------------------------
	typedef struct EnemyInitialSettingData
	{
		int enemyID;					// ���ʔԍ�(0..*)
		int type;						// �G�l�~�[���
		int defaultState;				// �����X�e�[�g	
		D3DXVECTOR3 defaultPosition;	// �������W
		D3DXVECTOR3 defaultDirection;	// �������ʕ���
	} ENEMYSET;
 
	//--------------------------------------------------------------------------
	// EnemyData�N���X�̓G�l�~�[�ŗL�̃X�e�[�^�X��ێ�����
	// Enemy�N���X�I�u�W�F�N�g��EnemyData�̃|�C���^����ăX�e�[�^�X���Q�Ƃ���
	//--------------------------------------------------------------------------
	struct EnemyData
	{
		int enemyID;					// ���ʔԍ�(0..*)
		int type;						// �G�l�~�[���
		int state;						// �X�e�[�g
		int defaultState;				// �����X�e�[�g	
		D3DXVECTOR3 position;			// ���W
		D3DXVECTOR3 defaultPosition;	// �������W
		D3DXVECTOR3 direction;			// ���ʕ���
		D3DXVECTOR3 defaultDirection;	// �������ʕ���
		int hp;							// HP
		float deadTime;					// ���ގ���
		bool isAlive;					// �����t���O

		void zeroClear() { ZeroMemory(this, sizeof(EnemyData)); }
		void setUp()
		{
			state = defaultState;
			position = defaultPosition;
			direction = defaultDirection;
			hp = type >= 0 && type < TYPE_MAX ? ENEMY_HP_MAX[type] : 0;
			deadTime = 10000.0;
			isAlive = true;
		}
	};

	// �R���X�g���N�^�����ɓn���������p�f�[�^�Z�b�g
	struct ConstructionPackage
	{
		StaticMesh* staticMesh;
		EnemyData* enemyData;
		Player* player;
		LPD3DXMESH	attractorMesh;
		D3DXMATRIX*	attractorMatrix;
	};

#if _DEBUG
	struct OperationKeyTable
	{
		BYTE front;
		BYTE back;
		BYTE right;
		BYTE left;
		BYTE jump;
		BYTE reverseCameraX;
		BYTE reverseCameraY;
	};

	const OperationKeyTable KEY_TABLE_1P =
	{
		'W',		// FRONT
		'S',		// BACK
		'D',		// RIGHT
		'A',		// LEFT
		VK_SPACE,	// JUMP
		VK_F11,		// ReverseCameraAxisX
		VK_F12,		// ReverseCameraAxisY
	};
#endif //_DEBUG
}
#pragma endregion


//=============================================================================
// �N���X��`
//=============================================================================
#pragma region [Class]
class Enemy: public Object
{
protected:
	enemyNS::EnemyData* enemyData;					// �G�l�~�[�f�[�^
	static int numOfEnemy;							// �G�l�~�[�̑���
	StaticMesh* staticMesh;							// ���b�V�����
	Player* player;									// �v���C���[
	LPD3DXMESH	attractorMesh;						// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;					// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X
	StateMachine stateMachine;						// �X�e�[�g�}�V��

	// �u���b�N�{�[�h
	bool isNoticingPlayer[gameMasterNS::PLAYER_NUM];// �v���C���[�F���t���O
	float noticedTimePlayer[gameMasterNS::PLAYER_NUM];// �v���C���[�F������
	D3DXVECTOR3* movingTarget;						// �ړ��^�[�Q�b�g
	Player* attackTarget;							// �U���Ώۃv���C���[
	D3DXVECTOR3 attentionDirection;					// ���ڕ���
	bool isPayingNewAttention;						// �V�K���ڃt���O

	// �Z���T�[
	bool canSense;									// �Z���T�[���s�\�t���O
	float cntSensorInterval;						// �Z���T�[�Ď��s�Ԋu�J�E���^
	bool isSensingPlayer[gameMasterNS::PLAYER_NUM];	// �v���C���[���m�t���O
	float distanceToPlayer[gameMasterNS::PLAYER_NUM];// �v���C���[�Ƃ̋���

	// �o�H�T��
	bool canSearch;									// �o�H�T�����s�\�t���O
	float cntPathSearchInterval;					// �o�H�T���Ď��s�Ԋu�J�E���^
	NavigationMesh* naviMesh;						// �i�r���b�V��
	LinkedList<meshDataNS::Index2>* edgeList;		// �G�b�W���X�g
	DWORD naviFaceIndex;							// �p�X�������̃i�r���b�V���ݒu�ʃC���f�b�N�X

	// �U��
	bool canAttack;									// �U���\�t���O
	float cntAttackInterval;						// �U���Ď��s�Ԋu�J�E���^
	bool isAttacking;								// �U�����t���O
	float attackTime;								// �U�������ԁi�b�j
	D3DXVECTOR3 attackDirection;					// �U�������x�N�g��

	// �ړ�
	D3DXVECTOR3 destination;						// �ړI�n
	D3DXVECTOR3 moveDirection;						// �ړ��x�N�g��
	bool isArraved;									// �����t���O
	float movingTime;								// �v���ړ�����
	bool isDestinationLost;							// �ړI�n����������
	bool onJump;									// �W�����v�t���O
	bool jumping;									// �W�����v���t���O

	// �R���W�����ƕ�������
	Ray	ray;										// ���C
	BoundingSphere sphereCollider;					// �o�E���f�B���O�X�t�B�A
	float difference;								// �t�B�[���h�␳����
	bool onGround;									// �ڒn����
	bool onGroundBefore;							// ���O�t���[���̐ڒn����
	D3DXVECTOR3	groundNormal;						// �ڒn�ʖ@��
	bool isHitPlayer;								// �v���C���[�ƐڐG���Ă���
	float friction;									// ���C�W��

	// �T�E���h
	PLAY_PARAMETERS playParameters;
	PLAY_PARAMETERS footStepsSetting;

	//------------------
	// �A�N�V�����N�G��
	//------------------
	// �Z���T�[
	bool sensor();
	// �o�H�T��
	void searchPath();
	// �U��
	void attack();
	// �ړ�
	void move(float frameTime);
	//------------------
	// �A�N�V�����̎���
	//------------------
	// ���o�Z���T�[
	bool eyeSensor(int playerType);
	// ���o�Z���T�[
	bool earSensor(int playerType);
	// �X�e�A�����O
	void steering();
	// �U����
	void attacking(float frameTime);
	//----------------
	// ���㏈���̎���
	//----------------
	// �ڒn����
	void grounding();
	// �ǂ���
	void wallScratch();
	// ��������
	void physicalBehavior();
	// �����̍X�V
	void updatePhysics(float frameTime);
	// �p���̍X�V
	void updatePosture(float frameTime);
	// ��������
	void checkArrival();
	// �L���̍X�V
	void updataBlackBoard(float frameTime);
	//--------------
	// �X�e�[�g����
	//--------------
	// �ǐՃX�e�[�g
	virtual void chase(float frameTime);
	// �x���X�e�[�g
	virtual void patrol(float frameTime);
	// �x�e�X�e�[�g
	virtual void rest(float frameTime);
	// �c���[�U���X�e�[�g
	virtual void attackTree(float frameTime);
	// ���S�X�e�[�g
	virtual void die(float frameTime);
	//--------
	// ���̑�
	//--------
	// �K�؂ȃv���C���[��ǐՃ^�[�Q�b�g�ɐݒ肷��
	void setPlayerChaseTarget();
	// �f�o�b�O�p�ړI�n�ݒ�
	void setDebugDestination();
	// �����̏���
	void footsteps(D3DXVECTOR3 playerPos, int playerID);

public:
	//----------
	// ��{����
	//----------
	Enemy(enemyNS::ConstructionPackage constructionPackage);
	~Enemy();
	// �X�V����
	virtual void update(float frameTime);
	// ���O����
	virtual void preprocess(float frameTime);
	// ���㏈��
	virtual void postprocess(float frameTime);
	//----------------------
	// �X�e�[�g�؂�ւ�����
	//----------------------
	// �ǐՃX�e�[�g�̏���
	void prepareChase();
	// �x���X�e�[�g�̏���
	void preparePatrol();
	// �x�e�X�e�[�g�̏���
	void prepareRest();
	// �c���[�U���X�e�[�g�̏���
	void prepareAttackTree();
	// ���S�X�e�[�g�̏���
	void prepareDie();
	//-----------------
	// Getter & Setter
	//-----------------
	int getEnemyID();
	static int getNumOfEnemy();
	enemyNS::EnemyData* getEnemyData();
	BoundingSphere* getSphereCollider();
	LPD3DXMESH getMesh();
	bool getNoticedOfPlayer(int playerType);
	bool getIsAttacking();

	// �G�l�~�[�̃I�u�W�F�N�g�̐���������
	static void resetNumOfEnemy();
	// �_���[�W����
	void damage(int _damage);
	// �U������Ԃ��~�߂�
	void stopAttacking();
	// �v���C���[�ɋC�Â�
	void setAttention(D3DXVECTOR3 setting);
	// �ړ��^�[�Q�b�g��ݒ�
	void setMovingTarget(D3DXVECTOR3* _target);

#ifdef _DEBUG
	//-----------
	// Debug Use
	//-----------
	LPDIRECT3DDEVICE9 device;			// Direct3D�f�o�C�X
	Camera*	camera;						// ���삷��J�����ւ̃|�C���^
	Input* input;						// ���͏���
	enemyNS::OperationKeyTable keyTable;// ����L�[�e�[�u��
	float reverseValueXAxis;			// ����X��
	float reverseValueYAxis;			// ����Y��
	static int debugEnemyID;			// �f�o�b�O����G�l�~�[��ID
	Ray eyeAngleRay[4];					// ���o���C�\���p
	BoundingSphere hearingSphere[2];	// ���o�����\���p

	// �f�o�b�O����ݒ�
	void setDebugEnvironment();
	// ����ΏۃJ�����̃Z�b�g
	void setCamera(Camera* _camera) { camera = _camera; }
	// �J��������
	void controlCamera(float frameTime);
	// �ړ�����
	void moveOperation();
	// �ړ�
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);
	// �f�o�b�O�Z���T�[
	void debugSensor();
	// ImGUI�ւ̏o��
	void outputGUI();
#endif//_DEBUG
};
#pragma endregion
