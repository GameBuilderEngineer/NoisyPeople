//-----------------------------------------------------------------------------
// �G�l�~�[���N���X [Enemy.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshObject.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"


//=============================================================================
// ���O���
//=============================================================================
namespace enemyNS
{
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

	struct EnemyData
	{
		int id;							// ���ʔԍ�(0..*)
		int enemyType;					// �G�l�~�[���
		int enemyState;					// �G�l�~�[�X�e�[�g
		int initialPosition;			// �������W
		bool isDead;					// ���S�������ǂ���
		void initialize() { ZeroMemory(this, sizeof(EnemyData)); }
	};


	const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(-35, 10, 0);
	//const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(0, 0, 0);

	const float MOVE_SPEED_MAX = 5.67f;		// �b��5.67m
	//const float MOVE_SPEED_MAX = 50.67f;		// �b��5.67m
	const float MOVE_FRICTION = 0.99f;
	const float MOVE_ACC = MOVE_SPEED_MAX / MOVE_FRICTION;

	const float GRAVITY_FORCE = 9.8f * 0.5;	// �d��
	const float DIFFERENCE_FIELD = 0.2f;	// �t�B�[���h�␳����
	const float CAMERA_SPEED = 1.0f;		// �J�����̑���
}


//=============================================================================
//�N���X��`
//=============================================================================
class Enemy: public Object
{
private:
	enemyNS::EnemyData* enemyData;		// �G�l�~�[�f�[�^
	static int numOfEnemy;				// �G�l�~�[�̑���

	BoundingSphere sphereCollider;		// �o�E���f�B���O�X�t�B�A
	 

	// ��������
	LPD3DXMESH	attractorMesh;			// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;		// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X
	float difference;					// �t�B�[���h�␳����
	bool onGround;						// �ڒn����
	bool onGroundBefore;				// ���O�t���[���̐ڒn����
	float friction;

	//// �o�H�T��
	//std::vector<D3DXVECTOR3> routeStack;// �o�H�X�^�b�N


#ifdef _DEBUG
	// �f�o�b�O�p
	LPDIRECT3DDEVICE9 device;			// Direct3D�f�o�C�X
	Camera*	camera;						// ���삷��J�����ւ̃|�C���^
	Input* input;						// ���͏���
	enemyNS::OperationKeyTable keyTable;// ����L�[�e�[�u��
	float reverseValueXAxis;			// ����X��
	float reverseValueYAxis;			// ����Y��
#endif

public:
	Enemy();
	~Enemy();
	virtual void update(float frameTime);
	//virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	void groundingWork();
	void updatePhysicalBehavior();
	void updatePhysics(float frameTime);
	void configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH attractorMesh, D3DXMATRIX* attractorMatrix);
	void outputGUI();

	// Debug
	void setDebugEnvironment();								// �f�o�b�O����ݒ�
	void controlCamera(float frameTime);					// �J��������
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);	//�ړ�
	void moveOperation();									// �ړ�����
	bool isGoingMoveOperation;

	// Getter
	static int getNumOfEnemy();								// �G�l�~�[�̐����擾
	enemyNS::EnemyData* getEnemyData();						// �G�l�~�[�f�[�^���擾

	//setter
	void setDataToEnemy(enemyNS::EnemyData* _enemyData);	// �G�l�~�[�f�[�^���G�l�~�[�ɐݒ�
	void setCamera(Camera* _camera);						// ����ΏۃJ�����̃Z�b�g

};
