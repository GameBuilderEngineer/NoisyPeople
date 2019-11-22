//===================================================================================================================================
//�yPlayer.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/24
// [�X�V��]2019/11/13
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
#include "Sound.h"

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
		BYTE cancelVision;
		BYTE skyVision;
		BYTE cancelSkyVision;
		BYTE digitalShift;
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
			VK_LSHIFT,				//CANCEL_VISION
			VK_SPACE,				//SKY_VISION
			VK_SPACE,				//CANCEL_SKY_VISION
			VK_ESCAPE,				//DIGITAL_SHIFT:���N���b�N
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
			VK_ESCAPE,				//JUMP:�E�N���b�N
			VK_RSHIFT,				//VISION
			VK_RSHIFT,				//CANCEL_VISION
			VK_BACK,				//SKY_VISION
			VK_BACK,				//CANCEL_SKY_VISION
			VK_ESCAPE,				//DIGITAL_SHIFT:���N���b�N
			VK_F9,					//ReverseCameraAxisX
			VK_F10,					//ReverseCameraAxisY
		}
	};

	const OperationKeyTable NON_CONTOROL = {
		VK_ESCAPE,					//FRONT
		VK_ESCAPE,					//BACK
		VK_ESCAPE,					//RIGHT
		VK_ESCAPE,					//LEFT
		VK_ESCAPE,					//RESET
		VK_ESCAPE,					//DASH
		VK_ESCAPE,					//JUMP:�E�N���b�N
		VK_ESCAPE,					//VISION
		VK_ESCAPE,					//CANCEL_VISION
		VK_ESCAPE,					//SKY_VISION
		VK_ESCAPE,					//CANCEL_SKY_VISION
		VK_ESCAPE,					//DIGITAL_SHIFT:���N���b�N
		VK_ESCAPE,					//ReverseCameraAxisX
		VK_ESCAPE,					//ReverseCameraAxisY
	};

	const BYTE BUTTON_VISION		= virtualControllerNS::Y;
	const BYTE BUTTON_SKY_VISION	= virtualControllerNS::X;
	const BYTE BUTTON_DIGITA_SHIFT	= virtualControllerNS::R1;
	const BYTE BUTTON_JUMP			= virtualControllerNS::B;
	const BYTE BUTTON_BULLET		= virtualControllerNS::R1;
	const BYTE BUTTON_PASUE			= virtualControllerNS::SPECIAL_MAIN;

	enum STATE {
		NONE,
		NORMAL,
		VISION,
		SKY_VISION,
		DIGITAL_SHIFT,
		STATE_NUM
	};

	const D3DXVECTOR3 START_POSITION[gameMasterNS::PLAYER_NUM] =
	{
		D3DXVECTOR3(-10, 20, -220),			//1P
		D3DXVECTOR3(10, 20, -220)			//2P
	};

	// StatusParameter
	const int	MAX_HP						= 100;									// �g�o�ő�l
	const int	FULL_POWER					= 100;									// �d�̓Q�[�W��{
	const int	MAX_POWER					= FULL_POWER * 3;						// �d�̓Q�[�W�ő�l
	const int	MIN_POWER					= 0;									// �d�̓Q�[�W�Œ�l
	const float INVINCIBLE_TIME				= 3.0f;									// ���G����


	//CameraParameter
	const D3DXVECTOR3 CAMERA_GAZE			= D3DXVECTOR3(1.0f,1.5f,0.0f);

	// Physics
	const float MOVE_ACC					= 27.0f;								// �ړ������x
	const float MOVE_ACC_WHEN_NOT_GROUND	= MOVE_ACC / 8.5f;						// �󒆈ړ������x
	const float STOP_SPEED					= 0.5f;									// �ړ���~���x
	const float MAX_SPEED					= 6.0f;									// �ړ���~���x
	const float MAX_SLIP					= 10.0f;									// �ړ���~���x
	const float FALL_SPEED_MAX				= 60.0f;								// �����ō����x
	const float MOVE_FRICTION				= 1.3f;								// �n�ʖ��C�W��
	const float WALL_FRICTION				= 0.98;									// �ǂ��薀�C�W��
	const float GROUND_FRICTION				= 0.25;									// ���n���C�W��
	const float GRAVITY_FORCE				= 9.8f;									// �d��
	const float JUMP_SPEED					= 6.0f;									// �W�����v����
	const float JUMP_TIME					= 0.3f;									// �W�����v����
	const float JUMP_CONTROL_SPEED			= 1.0f;									// �W�����v�����R���g�[�����x
	const float DASH_MAGNIFICATION			= 2.0f;									// �_�b�V���{��
	// Another
	const float DIFFERENCE_FIELD			= 0.1f;								// �t�B�[���h�␳����
	const float CAMERA_SPEED				= 1.0f;									// �J�����̑���
	
	//Shooting
	const float MAX_DISTANCE				= 100.0f;								//�ő�Ə�����

	//EnableOperation
	const int	DISABLE_OPERATION			= 0x00000000;
	const int	ENABLE_SHOT					= 0x00000001;
	const int	ENABLE_JUMP					= 0x00000002;
	const int	ENABLE_VISION				= 0x00000004;
	const int	ENABLE_CANCEL_VISION		= 0x00000008;
	const int	ENABLE_SKY_VISION			= 0x00000010;
	const int	ENABLE_CANCEL_SKY_VISION	= 0x00000020;
	const int	ENABLE_SHIFT				= 0x00000040;
	const int	ALL_OPERATION				= 0xffffffff;
}



//===================================================================================================================================
//�y���z�X�e�[�g�N���X�z
//===================================================================================================================================
class AbstractState:public Base
{
public:
	float			frameTime;
	float			stateTimer;
	int				type;
	int				nextType;
	bool			onTrans;
	std::string		stateName;
public:
	AbstractState() {
		frameTime	= 0.0f;
		stateTimer	= 0.0f;
		type		= playerNS::STATE::NONE;
		nextType	= playerNS::STATE::NONE;
		onTrans		= false;
		stateName	= "none";
	}
	virtual void start()					= 0;
	virtual void update(float frameTime)	= 0;
	virtual void operation()				= 0;
	virtual void physics()					= 0;
	virtual void controlCamera()			= 0;
	virtual void end()						= 0;
	virtual AbstractState* transition()		= 0;
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
	AbstractState*				state;							//��ԃN���X
	int							hp;								// HP
	int							power;							// �d��
	int							validOperation;					//�L���ȑ���r�b�g�t���O



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

	//�V���[�e�B���O�A�N�V����
	Ray							aimingRay;						//�Ə����C�i�J��������̃��C�j
	Ray							shootingRay;					//�_�����C�i�v���C���[����̃��C�j
	D3DXVECTOR3					launchPosition;					//���ˈʒu
	D3DXVECTOR3					aimingPosition;					//�Ə��ʒu(�J�������C�����j
	D3DXVECTOR3					shootingPosition;				//�Փˈʒu(�v���C���[���C�����j
	float						collideDistance;				//�Ǝˋ���
	BulletManager*				bulletManager;					//�o���b�g�}�l�[�W���[

	//�f�W�^���A�N�V����
	Line						shiftLine;						//�f�W�^���V�t�g���C��
	Ray							shiftRay;						//�f�W�^���V�t�g���C
	bool						enableShift;					//�V�t�g���\


	//�Đ��p�����[�^
	PLAY_PARAMETERS shiftStartSE;
	PLAY_PARAMETERS shiftFinishSE;
	PLAY_PARAMETERS visionSE;
	PLAY_PARAMETERS visionStartSE;
	PLAY_PARAMETERS visionFinishSE;
	PLAY_PARAMETERS skyVisionStartSE;
	PLAY_PARAMETERS skyVisionFinishSE;

	//debug
	float dot;

public:
	Player();
	~Player();

	// ��{����
	virtual void initialize(PlayerTable info);
	virtual void update(float frameTime);
	void otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//��ԑJ��
	void transState(int next);											//��ԑJ��

	// �Փ�
	void grounding();											// �ڒn����
	void wallScratch();											// �ǂ��菈��

	bool grounding(LPD3DXMESH mesh, D3DXMATRIX matrix);
	
	//�߂荞�ݕ␳
	//Ray ray�F�␳�������C�i�擾�����@�������ɕ␳�����j
	//float distance�F�␳����
	//LPD3DXMESH mesh�F�ՓˑΏۃ��b�V��
	//D3DXMATRIX matrix�F�ՓˑΏۍs��
	bool insetCorrection(Ray ray, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix);
	bool insetCorrection(int axisID, float distance, LPD3DXMESH mesh, D3DXMATRIX matrix);


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
	float dash();												//�_�b�V��
	bool shot();												//�e��ł�

	//�f�W�^���A�N�V����
	bool digitalShift();										//�f�W�^���V�t�g
	bool executionDigitalShift();								//�f�W�^���V�t�g���s
	bool skyVision();											//�X�J�C�r�W����
	bool vision();												//�r�W����
	bool cancelSkyVision();										//�X�J�C�r�W����
	bool cancelVision();										//�r�W����
	void collideShiftRay(LPD3DXMESH mesh, D3DXMATRIX matrix);	//�V�t�g���C�Ƃ̏Փ�


	//�V���[�e�B���O
	void updateAiming();										//�Ə��������X�V����
	void updatePostureByAiming();								//�_�������֎p����������
	void updateShooting();										//�_���ʒu�̍X�V

	// ���̑�
	virtual void outputGUI() override;							// ImGUI
	void reset();												// ���Z�b�g

	//setter
	void	setCamera(Camera* _camera);							//����ΏۃJ�����̃Z�b�g
	void	addpower(int add);									//�d�͉��Z
	void	pullpower(int pull);								//�d�͌��Z
	void	setInfomation(PlayerTable info);					//�v���C���[���̃Z�b�g
	void	damage(int _damage);								//�_���[�W����
	void	setValidOperation(int value);						//�L������̐ݒ�
	void	enableOperation(int value);							//�����L���ɂ���
	void	disableOperation(int value);						//����𖳌��ɂ���
	

	//getter
	int getHp();												// HP�̎擾
	int getPower();												// �d�͂̎擾
	int getState();
	bool whetherValidOperation(int operation);					//�L������̊m�F
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
	LPD3DXMESH		getMesh();									//���C�Փ˗p���b�V��
	bool getWhetherExecutingMoveOpe();							//�ړ����쒆���擾
	bool getWhetherShot() { return false;/*��*/ }				//�������Ăق����i�V���b�g�A�N�V�����������擾�j
	bool getOnGround();											//�ڒn���Ă��邩�擾
	D3DXVECTOR3* getGroundNormal();								//�ڒn�ʖ@�����擾

};
