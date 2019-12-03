//===================================================================================================================================
//�yTmpObject.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/24
// [�X�V��]2019/10/18
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "StaticMeshRenderer.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace tmpObjNS {

	enum TMPOBJ_LIST
	{
		TMPOBJ_PLAYER,
		TMPOBJ_WOLF,
		TMPOBJ_TIGER,
		TMPOBJ_BEAR,
		TMPOBJ_BATTERY,
		TMPOBJ_EXAMPLE,
		TMPOBJ_MAX
	};

	struct OperationKeyTable
	{
		BYTE front;
		BYTE back;
		BYTE right;
		BYTE left;
		BYTE reset;
		BYTE spin;
		BYTE down;
		BYTE fly;
	};

	const OperationKeyTable KEY_TABLE_1P = {
		'W',					//FRONT
		'S',					//BACK
		'D',					//RIGHT
		'A',					//LEFT
		'R',					//RESET
		'E',					//SPIN
		VK_LSHIFT,				//DASH
		VK_SPACE,				//JUMP
	};

	const OperationKeyTable NON_CONTOROL = {
		VK_ESCAPE,		//FRONT
		VK_ESCAPE,		//BACK
		VK_ESCAPE,		//RIGHT
		VK_ESCAPE,		//LEFT
		VK_ESCAPE,		//RESET
		VK_ESCAPE,		//DASH
		VK_ESCAPE,		//JUMP
	};

	const BYTE BUTTON_JUMP = virtualControllerNS::B;
	const BYTE BUTTON_BULLET = virtualControllerNS::R1;
	const BYTE BUTTON_CUT = virtualControllerNS::X;
	const BYTE BUTTON_REVIVAL = virtualControllerNS::A;
	const BYTE BUTTON_PILE = virtualControllerNS::L1;

	const D3DXVECTOR3 START_POSITION = D3DXVECTOR3(-115, 40, 0);

	// Physics
	const float MOVE_ACC = 27.0f*1.5f;								// �ړ������x
	const float STOP_SPEED = 1.0f;									// �ړ���~���x
	const float FALL_SPEED_MAX = 60.0f;								// �����ō����x
	const float JUMP_SPEED = 6.0f;									// �W�����v����
	const float JUMP_CONTROL_SPEED = 1.0f;							// �W�����v�����R���g�[�����x
	const float DASH_MAGNIFICATION = 2.0f;							// �_�b�V���{��
	// Another
	const float DIFFERENCE_FIELD = 0.05f;							// �t�B�[���h�␳����
	const float CAMERA_SPEED = 1.0f;								// �J�����̑���
}


//===================================================================================================================================
//�y�N���X��`�z
//===================================================================================================================================
class TmpObject : public Object
{
private:
	//�X�e�[�^�X
	int							type;							//�v���C���[�^�C�v
	tmpObjNS::OperationKeyTable	keyTable;						//����Key�e�[�u��
	int							state;							//��ԕϐ�

	//����
	float						reverseValueXAxis;				//����X��
	float						reverseValueYAxis;				//����Y��
	bool							isExecutingMoveOperation;		// �ړ����쒆�t���O
	bool							onFly;							// ��ԃt���O
	bool							jumping;							// �W�����v���t���O

	// �Փ�
	BoundingSphere				bodyCollide;						// ���R���W����
	float						difference;						//�t�B�[���h�␳����
	bool						onGround;						//�ڒn����

	// �ėp
	LPDIRECT3DDEVICE9			device;						// Direct3D�f�o�C�X
	Input*						input;						// ���͌n
	Camera*						camera;						// ���삷��J�����ւ̃|�C���^
	D3DXVECTOR3					centralPosition;				// ���S���W
	D3DXMATRIX					centralMatrixWorld;			// ���S���W���[���h�}�g���N�X

	// ���̑�
	int							staticMeshNo;				// ���b�V����ID
	//int							ItemListboxMesh;				// ���b�V���̎��(���X�g�{�b�N�X)
	
	D3DXVECTOR3					PositionYeah;

public:
	TmpObject();
	~TmpObject();

	// ��{����
	virtual void initialize(int playerType, int modelType);
	virtual void update(float frameTime);
	void otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// ����
	void configurationGravityWithRay(D3DXVECTOR3* attractorPosition,
		LPD3DXMESH attractorMesh, D3DXMATRIX* attractorMatrix);
	void updatePhysics(float frameTime);							// �����̍X�V

	//����
	void moveOperation();											// �ړ�����
	void controlCamera(float frameTime);							// �J��������
	bool grounding(LPD3DXMESH mesh, D3DXMATRIX matrix);				//�ڒn����

	// �A�N�V����
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);//�ړ�

	// ���̑�
	virtual void outputGUI() override;							// ImGUI
	void reset();												// ���Z�b�g
	void resetMesh(int meshId);									// ���b�V���̃��Z�b�g

	//setter
	void setCamera(Camera* _camera);							//����ΏۃJ�����̃Z�b�g

	//getter
	int getState();
	BoundingSphere* getBodyCollide();							//���R���W�����̎擾
	D3DXVECTOR3*	getPositionYeah() { return&PositionYeah; };
};