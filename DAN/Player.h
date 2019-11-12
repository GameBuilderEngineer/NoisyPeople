//===================================================================================================================================
//�yPlayer.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/24
// [�X�V��]2019/11/05
//===================================================================================================================================
#pragma once

//----------------------------
// �v���C���[�̕����p�����[�^
//----------------------------
// �ړ��ō����x				6m/s
// �Î~�W�����v�����ő�l	2m
// �Î~�W�����v�؋󎞊� 	0.8s�i�W�����v���J�n���Ă��璅�n����܂ł̎��ԁj
// �ړ��W�����v�Œ��򋗗�	4.8m(6 * 0.8)


//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "StaticMeshRenderer.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"
#include "GameMaster.h"
#include "Bullet.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace playerNS{

	enum PLAYER_TYPE{
		PLAYER1,
		PLAYER2,
		TITLE_PLAYER,
		NUM_PLAYER
	};

	struct OperationKeyTable
	{
		BYTE front;
		BYTE back;
		BYTE right;
		BYTE left;
		BYTE reset;
		BYTE dash;
		BYTE jump;
		BYTE vision;
		BYTE skyVision;
		BYTE reverseCameraX;
		BYTE reverseCameraY;
	};

	const OperationKeyTable KEY_TABLE[gameMasterNS::PLAYER_NUM] = {
		//1P
		{
			'W',					//FRONT
			'S',					//BACK
			'D',					//RIGHT
			'A',					//LEFT
			'R',					//RESET
			VK_ESCAPE,				//DASH:�p�~
			VK_ESCAPE,				//JUMP:�E�N���b�N
			VK_LSHIFT,				//VISION
			VK_SPACE,				//SKY_VISION
			VK_F7,					//ReverseCameraAxisX
			VK_F8,					//ReverseCameraAxisY
		},
		//2P
		{
			VK_UP,					//FRONT
			VK_DOWN,				//BACK
			VK_RIGHT,				//RIGHT
			VK_LEFT,				//LEFT
			'R',					//RESET
			VK_ESCAPE,				//DASH
			VK_ESCAPE,				//JUMP
			VK_RSHIFT,				//VISION
			VK_BACK,				//SKY_VISION
			VK_F9,					//ReverseCameraAxisX
			VK_F10,					//ReverseCameraAxisY
		}
	};

	const OperationKeyTable NON_CONTOROL = {
		VK_ESCAPE,		//FRONT
		VK_ESCAPE,		//BACK
		VK_ESCAPE,		//RIGHT
		VK_ESCAPE,		//LEFT
		VK_ESCAPE,		//RESET
		VK_ESCAPE,		//DASH
		VK_ESCAPE,		//JUMP
		VK_ESCAPE,		//ReverseCameraAxisX
		VK_ESCAPE,		//ReverseCameraAxisY
	};

	const BYTE BUTTON_VISION		= virtualControllerNS::B;
	const BYTE BUTTON_SKY_VISION	= virtualControllerNS::X;
	const BYTE BUTTON_JUMP			= virtualControllerNS::L1;
	const BYTE BUTTON_BULLET		= virtualControllerNS::R1;
	const BYTE BUTTON_PASUE			= virtualControllerNS::SPECIAL_MAIN;


	enum STATE {
		NORMAL,
		VISION,
		SKY_VISION,
		SHIFT,
		STATE_NUM
	};

	const D3DXVECTOR3 START_POSITION[gameMasterNS::PLAYER_NUM] =
	{
		D3DXVECTOR3(-115, 40, 0),		//1P
		D3DXVECTOR3(-115, 30, 30)		//2P
	};

	// StatusParameter
	const int	MAX_HP						= 100;									// �g�o�ő�l
	const int	FULL_POWER					= 100;									// �d�̓Q�[�W��{
	const int	MAX_POWER					= FULL_POWER * 3;						// �d�̓Q�[�W�ő�l
	const int	MIN_POWER					= 0;									// �d�̓Q�[�W�Œ�l
	const float INVINCIBLE_TIME				= 3.0f;									// ���G����


	//CameraParameter
	const D3DXVECTOR3 CAMERA_GAZE			= D3DXVECTOR3(1.0f,2.0f,0.0f);

	// Physics
	const float MOVE_ACC					= 27.0f;								// �ړ������x
	const float MOVE_ACC_WHEN_NOT_GROUND	= MOVE_ACC / 8.5f;						// �󒆈ړ������x
	const float STOP_SPEED					= 0.5f;									// �ړ���~���x
	const float MAX_SPEED					= 6.0f;									// �ړ���~���x
	const float FALL_SPEED_MAX				= 60.0f;								// �����ō����x
	const float MOVE_FRICTION				= 0.93f;								// �n�ʖ��C�W��
	const float WALL_FRICTION				= 0.98;									// �ǂ��薀�C�W��
	const float GROUND_FRICTION				= 0.25;									// ���n���C�W��
	const float GRAVITY_FORCE				= 9.8f;									// �d��
	const float JUMP_SPEED					= 6.0f;									// �W�����v����
	const float JUMP_CONTROL_SPEED			= 1.0f;									// �W�����v�����R���g�[�����x
	const float DASH_MAGNIFICATION			= 2.0f;									// �_�b�V���{��
	// Another
	const float DIFFERENCE_FIELD			= 0.05f;								// �t�B�[���h�␳����
	const float CAMERA_SPEED				= 1.0f;									// �J�����̑���
	
	//Shooting
	const float MAX_DISTANCE				= 100.0f;								//�ő�Ə�����

	//DigitalShift
	const float SHIFT_TIME				= 2.0f;									//�f�W�^���V�t�g����

}



//===================================================================================================================================
//�y���z�X�e�[�g�N���X�z
//===================================================================================================================================
class State:public Base
{
public:
	float	frameTime;
	int		type;
public:
	virtual void start()					= 0;
	virtual void update(float frameTime)	= 0;
	virtual void operation()				= 0;
	virtual void physics()					= 0;
	virtual void end()						= 0;
	virtual State* transition()				= 0;
};

//===================================================================================================================================
//�y�v���C���[�N���X�z
//===================================================================================================================================
class Player : public Object
{
private:
	//�v���C���e�[�u��
	PlayerTable					infomation;						//�v���C���[���

	//�X�e�[�^�X
	playerNS::OperationKeyTable	keyTable;						//����Key�e�[�u��
	State*						state;							//��ԃN���X
	int							hp;								// HP
	int							power;							// �d��

	//�^�C�}�[
	float						frameTime;						//�t���[���^�C��
	float						invincibleTimer;				//���G����

	//����
	float						reverseValueXAxis;				//����X��
	float						reverseValueYAxis;				//����Y��
	bool						isExecutingMoveOperation;		// �ړ����쒆�t���O
	bool						onJump;							// �W�����v�t���O
	bool						jumping;						// �W�����v���t���O

	// UI�p���씻��t���O
	bool						isShotAble;
	bool						isJumpAble;
	bool						isVisionAble;
	bool						isSkyVisionAble;
	bool						isShiftAble;

	// �Փ�
	BoundingSphere				bodyCollide;					// ���R���W����
	float						difference;						//�t�B�[���h�␳����
	bool						onGround;						//�ڒn����
	bool						onGroundBefore;					// ���O�t���[���̐ڒn����
	D3DXVECTOR3					groundNormal;					// �ڒn�ʖ@��

	// ����
	float						friction;						// ���C�W��
	Ray							ray;							// ���C
	LPD3DXMESH					attractorMesh;					//�d�́i���́j�������b�V��
	D3DXMATRIX*					attractorMatrix;				//�d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X

	// �ėp
	LPDIRECT3DDEVICE9			device;							// Direct3D�f�o�C�X
	Input*						input;							// ���͌n
	Camera*						camera;							// ���삷��J�����ւ̃|�C���^
	D3DXVECTOR3					cameraGaze;						//�J���������ʒu
	D3DXVECTOR3					cameraGazeRelative;				//�J�����������Έʒu
	D3DXVECTOR3					centralPosition;				// ���S���W
	D3DXMATRIX					centralMatrixWorld;				// ���S���W���[���h�}�g���N�X

	//�V���[�e�B���O�A�N�V����
	Ray							aimingRay;						//�Ə����C�i�J��������̃��C�j
	Ray							shootingRay;					//�_�����C�i�v���C���[����̃��C�j
	D3DXVECTOR3					launchPosition;					//���ˈʒu
	D3DXVECTOR3					aimingPosition;					//�Ə��ʒu(�J�������C�����j
	D3DXVECTOR3					shootingPosition;				//�Փˈʒu(�v���C���[���C�����j
	float						collideDistance;				//�Ǝˋ���
	BulletManager*				bulletManager;					//�o���b�g�}�l�[�W���[

	//�f�W�^���A�N�V����
	bool						isShifting;						//�f�W�^���V�t�g���t���O
	float						shiftTimer;						//�f�W�^���V�t�g�^�C�}�[
	Line						shiftLine;						//�f�W�^���V�t�g���C��
	Ray							shiftRay;						//�f�W�^���V�t�g���C

public:
	Player();
	~Player();

	// ��{����
	virtual void initialize(PlayerTable info);
	virtual void update(float frameTime);
	void otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// �Փ�
	void grounding();											// �ڒn����
	void wallScratch();											// �ǂ��菈��

	// ����
	void configurationGravityWithRay(D3DXVECTOR3* attractorPosition,
		LPD3DXMESH attractorMesh, D3DXMATRIX* attractorMatrix);
	void physicalBehavior();									// ��������
	void updatePhysics(float frameTime);						// �����̍X�V
	void insetCorrection();										//�߂荞�ݕ␳

	//����
	void moveOperation();										// �ړ�����
	void jumpOperation();										// �W�����v����
	void controlCamera(float frameTime);						// �J��������

	// �A�N�V����
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);//�ړ�
	void jump();												//�W�����v
	float dash();
	void shot();												//�e��ł�
	void digitalShift();										//�f�W�^���V�t�g

	// ���̑�
	virtual void outputGUI() override;							// ImGUI
	void reset();												// ���Z�b�g

	//setter
	void setCamera(Camera* _camera);							//����ΏۃJ�����̃Z�b�g
	void addpower(int add);										//�d�͉��Z
	void pullpower(int pull);									//�d�͌��Z
	void setInfomation(PlayerTable info);						//�v���C���[���̃Z�b�g
	void damage(int _damage);									//�_���[�W����

	//getter
	int getHp();												// HP�̎擾
	int getPower();												// �d�͂̎擾
	int getState();
	bool canShot();
	bool canJump();
	bool canDoVision();
	bool canDoSkyVision();
	bool canShift();
	BoundingSphere* getBodyCollide();							//���R���W�����̎擾
	PlayerTable*	getInfomation();							//�v���C���[���擾
	D3DXVECTOR3*	getCameraGaze();							//�J���������|�W�V����
	D3DXVECTOR3*	getAiming();								//�Ə��|�C���g�̎擾
	Bullet*			getBullet(int i);							//���˒��̒e�ւ̃|�C���^
	int				getShootingNum();							//���˒��̒e��

	D3DXVECTOR3* getCentralPosition();							//���S���W�̎擾
	bool getWhetherExecutingMoveOpe();							//�ړ����쒆���擾
	bool getWhetherShot() { return false;/*��*/ }				//�������Ăق����i�V���b�g�A�N�V�����������擾�j
	bool getOnGround();											//�ڒn���Ă��邩�擾
	D3DXVECTOR3* getGroundNormal();								//�ڒn�ʖ@�����擾
	D3DXMATRIX* getcentralMatrixWorld();

};


//�ʏ���
class NormalState:public State
{
private:
	Player* player;
public:
	NormalState(Player* player);
	virtual void update(float frameTime);
	virtual void start();
	virtual void operation();
	virtual void physics();
	virtual State* transition();
	virtual void end();
};

