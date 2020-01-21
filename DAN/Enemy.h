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
#include "Tree.h"
#include "EnemyChaseMark.h"
#include "PartsAnimationManager.h"


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

	///////////////////////////////////////////////////////////////////////////
	////////////////////////////ENEMY_TYPE���Ƃ̒萔 //////////////////////////
	///////////////////////////////////////////////////////////////////////////
	// �G�l�~�[�̍ő�HP�e�[�u��
	const int ENEMY_HP_MAX[TYPE_MAX] =
	{
		60,			// WOLF
		100,		// TIGER
		450,		// BEAR
	};

	// �G�l�~�[�̃v���C���[�L�����ԃe�[�u��
	const float PLAYER_MEMORIZED_TIME[TYPE_MAX] =
	{
		4.0f,		// WOLF
		8.0f,		// TIGER
		10.0f,		// BEAR
	};

	// �G�l�~�[�̈ړ������x�e�[�u��
	const float MOVE_ACC[TYPE_MAX] =
	{
		15.0f,		// WOLF
		15.0f,		// TIGER
		12.0f,		// BEAR
	};

	//-------------
	//�Z���T�[�֌W
	//-------------
	// �G�l�~�[�̃Z���T�[�X�V�p�x�e�[�u���ix�b��1�x�X�V�j
	const float SENSOR_UPDATE_INTERVAL[TYPE_MAX] =
	{
		0.3f,		// WOLF
		0.3f,		// TIGER
		0.3f,		// BEAR
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
		0.8f,		// BEAR
	};

	// �G�l�~�[�̒��o�����e�[�u��
	const float NOTICEABLE_DISTANCE_PLAYER[TYPE_MAX] =
	{
		3.5f,		// WOLF
		5.5f,		// TIGER
		18.5f,		// BEAR
	};
	const float SHOT_SOUND_SCALE = 4.5f;// �V���b�g�������{��

	//----------
	// �U���֌W
	//----------
	// �G�l�~�[�̍U���̓e�[�u��
	const int ATTACK_DAMAGE[TYPE_MAX] =
	{
		10,			// WOLF
		20,			// TIGER
		30,			// BEAR
	};

	// �G�l�~�[�̍U���ړ����x�e�[�u��
	const float ATTACK_SPEED[TYPE_MAX] =
	{
		15.0f,		// WOLF
		15.0f,		// TIGER
		15.0f,		// BEAR
	};
	const float MULTIPLICATION_TO_MAKE_ATTACK_ACC = 6.5f;//�����x������W��

	// �G�l�~�[�̍U���ړ����ԃe�[�u��
	const float ATTACK_TIME[TYPE_MAX] =
	{
		0.7f,		// WOLF
		0.7f,		// TIGER
		0.7f,		// BEAR
	};

	// �G�l�~�[�̍U���Ԋu�e�[�u��
	const float ATTACK_INTERVAL[TYPE_MAX] =
	{
		8.0f,		// WOLF
		8.0f,		// TIGER
		10.0f,		// BEAR
	};

	//--------
	// ���̑�
	//--------
	// �ǐՃ}�[�N�̑傫��
	const D3DXVECTOR2 MARK_SCALE[TYPE_MAX] =
	{
		D3DXVECTOR2(2.0f, 2.0f),	// WOLF
		D3DXVECTOR2(3.5f, 3.5f),	// TIGER
		D3DXVECTOR2(3.0f, 3.0f),	// BEAR
	};

	// �A�C�e�����o���m���̕���i1/x�j
	const int ITEM_DROP_PROBABILITY_DENOMINATOR[TYPE_MAX] =
	{
		7,			// WOLF
		2,			// TIGER
		1			// BEAR
	};

	// �G�t�F�N�g�T�C�Y
	const float DEATH_EFFECT_SCALE[TYPE_MAX] =
	{
		0.85f,		// WOLF
		1.5f,		// TIGER
		5.0f,		// BEAR
	};

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////�G�l�~�[���ʒ萔 ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////
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
	const float TIME_BEFORE_TREE_COLLISION = 10.0f;		// �c���[�U���X�e�[�g�Ńc���[�ɐG��Ȃ��܂܂ł����鎞��

	// Other Constant
	const float DIFFERENCE_FIELD = 0.05f;				// �t�B�[���h�␳����
	const float JUMP_SPEED = 6.0f;						// �W�����v����
	const float JUMP_CONTROL_SPEED = 1.0f;				// �W�����v�����R���g�[�����x
	const float DASH_MAGNIFICATION = 2.0f;				// �_�b�V���{��
	const float CAMERA_SPEED = 1.0f;					// �J�����̑���
	const int PATOROL_ROUTE_MAX = 8;					// �x���X�e�[�g���̏�����W�ő吔
	const float AUTO_DESTRUCTION_HEIGHT = -100.0f;		// �����j������鍂��
	const float MARK_FLOATING_HEIGHT = 0.1f;			// �ǐՃ}�[�N�̍���
	const float DIE_STATE_TIME = 4.0f;					// ���S�X�e�[�g�̎���
	const float DIE_STATE_RENDERING_TIME = 2.8f;		// ���S�X�e�[�g�̂����`�悪��������

	// 3DSound
	enum SE_3D
	{
		FOOT_STEPS_SE,	// ����
		ATTACK_SE,		// �U����
		DIE_SE,			// ���S��
		SE_3D_MAX
	};

	///////////////////////////////////////////////////////////////////////////
	/////////////////////////////// �N���X��` ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------
	// EnemyInitialSettingData�N���X�̓G�l�~�[�����X�e�[�^�X��ێ�����
	// �V�K�G�l�~�[�쐬��G�l�~�[�z�u�c�[���Ƃ̃f�[�^�����Ɏg�p����
	//-----------------------------------------------------------------
	typedef struct EnemyInitialSettingData
	{
		int enemyID;									// ���ʔԍ�(0..*)
		int type;										// �G�l�~�[���
		int defaultState;								// �����X�e�[�g	
		D3DXVECTOR3 defaultPosition;					// �������W
		D3DXVECTOR3 defaultDirection;					// �������ʕ���
		int numRoute;									// �x���X�e�[�g���̏�����W�̐�
		D3DXVECTOR3 patrolRoute[PATOROL_ROUTE_MAX];		// �x���X�e�[�g���̏�����W
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
		int numRoute;					// �x���X�e�[�g���̏�����W�̐�
		D3DXVECTOR3* patrolRoute;		// �x���X�e�[�g���̏�����W
		Tree* targetTree;				// �U���Ώۃc���[
		int hp;							// HP
		float deadTime;					// ���ގ���
		bool isAlive;					// �����t���O
		bool isGeneratedBySpawnEvent;	// �C�x���g�ɂ�鐶���t���O(�f�[�^�쐬��I�u�W�F�N�g�쐬�O��true�ɐݒ�)
		bool isObjectExists;			// �I�u�W�F�N�g���݃t���O�i�I�u�W�F�N�g�쐬�j���ɍ��킹�Đݒ�j				
		bool wasAutoDetroy;				// �����폜�t���O

		//~EnemyData() { SAFE_DELETE_ARRAY(patrolRoute); }
		void zeroClear() { ZeroMemory(this, sizeof(EnemyData)); }
		void setUp()
		{
			state = defaultState;
			position = defaultPosition;
			direction = defaultDirection;
			hp = type >= 0 && type < TYPE_MAX ? ENEMY_HP_MAX[type] : 0;
			deadTime = 10000.0;
			isGeneratedBySpawnEvent = false;
			isAlive = true;
		}
	};

	//----------------------------------------------
	// �R���X�g���N�^�����ɓn���������p�f�[�^�Z�b�g
	//----------------------------------------------
	struct ConstructionPackage
	{
		StaticMesh* staticMesh;
		EnemyData* enemyData;
		GameMaster* gameMaster;
		Player* player;
		LPD3DXMESH	attractorMesh;
		D3DXMATRIX*	attractorMatrix;
		//EnemyChaseMark* markRenderer;
		//StaticMeshRenderer* tigerBulletRender;
	};

	//----------------------
	// ���S�G�t�F�N�g�N���X
	//----------------------
	class DeathEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3 * syncPosition;
		DeathEffect(D3DXVECTOR3* sync, int _effectNo)
		{
			syncPosition = sync;
			effectNo = _effectNo;
		}
		virtual void update()
		{
			position = *syncPosition;
			Instance::update();
		};
	};

	//----------------------------------------------------------------
	// EnemyParts�N���X�̓G�l�~�[�p�[�c�A�j���̊e�p�[�c�ł���
	// �G�l�~�[�ł��鎩�����g�̃|�C���^�ƃ��b�V�����̃|�C���^������
	//----------------------------------------------------------------
	class EnemyParts: public Object
	{
	private:
		Enemy* enemy;					// �G�l�~�[
		StaticMeshRenderer* renderer;	// �`��I�u�W�F�N�g
		float cntDestroyTime = 0.0f;	// �p�[�c���j�󂳂�鎞�Ԃ��J�E���g

	public:
		int durability;					// �ϋv�x
		D3DXVECTOR3 jointPosition;		// �ڍ������W�i= �I�t�Z�b�g * �p�[�c�̃��[���h�}�g���N�X�j
		bool wasEffectPlayed;			// �G�t�F�N�g�Đ����ꂽ��

		Enemy* getEnemy() { return enemy; }
		void setEnemy(Enemy* setting) { enemy = setting; }
		StaticMeshRenderer* getRenderer() { return renderer; }
		void setRenderer(StaticMeshRenderer* setting) { renderer = setting; }

		// �ϋv�x�ւ̃_���[�W����
		void damage(int value)
		{
			durability -= value;

			if (durability < 0)
			{
				durability = 0;
			}
		}

		// �ϋv�x�`�F�b�N
		void checkDurability(float frameTime)
		{
			if (durability != 0) return;

			if (wasEffectPlayed == false)
			{
				// �G�t�F�N�g
				DeathEffect* effect = new DeathEffect(&jointPosition, effekseerNS::ENEMY_DEATH);
				effect->scale *= 5.0f;//�G�t�F�N�g�T�C�Y
				effekseerNS::play(0, effect);
				wasEffectPlayed = true;
			}

			cntDestroyTime += frameTime;
			if (cntDestroyTime > 2.0f)
			{
				// �`��ƏՓ˔��肩���菜���i�C���X�^���X�j�����̂̓G�l�~�[�̃f�X�g���N�^�j
				renderer->unRegisterObjectByID(this->id);
				treeCell.remove();
			}
		}
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
// Enemy�N���X��`
//=============================================================================
#pragma region [Class]
class Enemy: public Object
{
protected:
#ifdef _DEBUG
public:
#endif
	enemyNS::EnemyData* enemyData;					// �G�l�~�[�f�[�^
	static int numOfEnemy;							// �G�l�~�[�̑���
	StaticMesh* staticMesh;							// ���b�V�����
	GameMaster* gameMaster;							// �Q�[���Ǘ��I�u�W�F�N�g
	Player* player;									// �v���C���[
	LPD3DXMESH	attractorMesh;						// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;					// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X
	StateMachine stateMachine;						// �X�e�[�g�}�V��
	PartsAnimationManager* animationManager;		// �A�j���[�V�����}�l�[�W��

	// �u���b�N�{�[�h
	bool isNoticingPlayer[gameMasterNS::PLAYER_NUM];// �v���C���[�F���t���O
	float noticedTimePlayer[gameMasterNS::PLAYER_NUM];// �v���C���[�F������
	int chasingPlayer;								// �ǐՃv���C���[
	D3DXVECTOR3* movingTarget;						// �ړ��^�[�Q�b�g
	Object* attackTarget;							// �U���Ώہi�v���C���[,�c���[�j
	D3DXVECTOR3 attentionDirection;					// ���ڕ���
	bool isPayingNewAttention;						// �V�K���ڃt���O
	int nextIndexOfRoute;							// �x���X�e�[�g���̎��̏�����W�i�����O�o�b�t�@�Y�����j
	float tacticsTime;								// ��p���ԁiAI�̐擪�s������ɔėp�I�Ɏg�p�j
	int tacticsPersonality;							// ��p��

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
	bool wasHittingTree;							// �c���[���U������			
	float treeAttackTime;							// �c���[�U������
	bool canDamageTree;								// �c���[�Ƀ_���[�W�����邩
	float cntBeforeTreeCollision;					// �c���[�U���X�e�[�g�Ńc���[�ɐڐG����܂ł̎���
	int cntDestroyParts;							// �j�󂳂ꂽ�p�[�c�̐�

	// �ړ�
	D3DXVECTOR3 destination;						// �ړI�n
	D3DXVECTOR3 moveDirection;						// �ړ��x�N�g��
	D3DXQUATERNION	moveMotion;						// �ړ����[�V����
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

	// �ǐՃ}�[�N
	EnemyChaseMark* markRenderer;					// �ǐՃ}�[�N�����_���[
	enemyChaseMarkNS::StandardChaseMark* markFront;	// �ǐՃ}�[�N�|�C���^
	enemyChaseMarkNS::StandardChaseMark* markBack;	// �ǐՃ}�[�N�|�C���^
	D3DXVECTOR3 markDirection;						// ���ǐՃ}�[�N�̕��p

	// State
	float cntTimeDie;								// ���S�X�e�[�g���ԃJ�E���^

	// �T�E���h
	PLAY_PARAMETERS playParameters;

	// �G�t�F�N�g
	enemyNS::DeathEffect* deathEffect;				// ���S�G�t�F�N�g

	//�G�l�~�[�����j�����v���C���[
	int playerNo;

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
	void steering(float frameTime);
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
	//----------
	// �T�E���h
	//----------
	// �T�E���h�v���C�p�����[�^�̎擾
	PLAY_PARAMETERS getPlayParameters(enemyNS::SE_3D soundType, int enemyType);
	// �����̏���
	void footsteps(D3DXVECTOR3 playerPos, int playerID);
	//--------
	// ���̑�
	//--------
	void reset();
	// �����j�����s�����m�F
	void checkAutoDestruction();
	// �K�؂ȃv���C���[��ǐՃ^�[�Q�b�g�ɐݒ肷��
	void setPlayerChaseTarget();
	// �f�o�b�O�p�ړI�n�ݒ�
	void setDebugDestination();
	// ����H�����O�o�b�t�@�̍X�V
	void updatePatrolRoute();
	// �ǐՃ}�[�N�̍쐬
	void createMark();
	// �ǐՃ}�[�N�̔j��
	void deleteMark();

public:
	//----------
	// ��{����
	//----------
	Enemy(enemyNS::ConstructionPackage constructionPackage);
	~Enemy();
	// �X�V����
	virtual void update(float frameTime) = 0;
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
	Enemy* getAdr() { return this; }
	int getEnemyID();
	static int getNumOfEnemy();
	enemyNS::EnemyData* getEnemyData();
	BoundingSphere* getSphereCollider();
	LPD3DXMESH getMesh();
	bool getNoticedOfPlayer(int playerType);
	bool getIsAttacking();
	int getChasingPlayer();
	int getPlayerNo();
	bool getIsPayingNewAttention();
	bool getCanDamageTree();
	int getCntDestroyParts();
	// �G�l�~�[�̃I�u�W�F�N�g�̐���������
	static void resetNumOfEnemy();
	// �_���[�W����
	void damage(int _damage,int playerNo);
	// �U������Ԃ��~�߂�
	void stopAttacking();
	// �v���C���[�ɋC�Â�
	void setAttention(D3DXVECTOR3 setting);
	// �ړ��^�[�Q�b�g��ݒ�
	void setMovingTarget(D3DXVECTOR3* _target);
	// �U���^�[�Q�b�g��ݒ�
	void setAttackTarget(Object* _target);
	// �|���ꂽ�v���C���[��ݒ�
	void setPlayerNo(int playerNo);
	// �c���[�ɍU�����q�b�g�������ݒ�
	void setTreeHit(bool setting);
	// �c���[�ɍU�����q�b�g�������ݒ�
	void setCanDamageTree(bool setting);
	// �t�B�[���h���[���h�}�g���N�X��ݒ�
	void setFieldMatrix(D3DXMATRIX *matrix);
	
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
	DWORD faceNumber;					// �f�o�b�O�p�ʔԍ�

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
