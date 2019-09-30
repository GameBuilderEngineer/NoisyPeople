//===================================================================================================================================
//�yPlayer.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/24
// [�X�V��]2019/09/24
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "StaticMeshObject.h"
#include "BoundingSphere.h"
#include "Input.h"
#include "Camera.h"

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
		BYTE reverseCameraX;
		BYTE reverseCameraY;
	};

	const OperationKeyTable KEY_TABLE_1P = {
		'W',		//FRONT
		'S',		//BACK
		'D',		//RIGHT
		'A',		//LEFT
		'R',		//RESET
		VK_LSHIFT,	//DASH
		VK_SPACE,	//JUMP
		VK_F11,		//ReverseCameraAxisX
		VK_F12,		//ReverseCameraAxisY
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

	const BYTE BUTTON_JUMP = virtualControllerNS::B;
	const BYTE BUTTON_BULLET = virtualControllerNS::R1;
	const BYTE BUTTON_CUT = virtualControllerNS::X;
	const BYTE BUTTON_REVIVAL = virtualControllerNS::A;
	const BYTE BUTTON_PILE = virtualControllerNS::L1;

	enum STATE {
		GROUND,
		STATE_NUM
	};

	const int MAX_HP					= 100;		//�g�o�ő�l
	const float SPEED					= 100.0f;	//���x
	const float DASH_MAGNIFICATION		= 2.0f;		//�_�b�V���{��
	const float JUMP_FORCE				= 30.0f;	//�W�����v��
	const float GRAVITY_FORCE			= 9.8f*7.0;	//�d��
	const float DIFFERENCE_FIELD		= 0.2f;		//�t�B�[���h�␳����
	const float INVINCIBLE_TIME			= 3.0f;		//���G����
	const float CAMERA_SPEED			= 1.0f;		//�J�����̑���

	//�v���C���[�̃X�^�[�g�|�W�V����
	const D3DXVECTOR3 START_POSITION =	D3DXVECTOR3(0, 1000, 0);

}

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
class Player : public StaticMeshObject
{
protected:
	//�ėp�f�o�C�X�E�N���X
	LPDIRECT3DDEVICE9 device;
	Input* input;
	int type;
	//Sound* sound;										//�T�E���h�N���X

	//�X�e�[�^�X
	Camera* camera;										//�J�����ւ̃|�C���^
	playerNS::OperationKeyTable keyTable;				//����Key�e�[�u��
	int state;											//��ԕϐ�

	//���R���W����
	BoundingSphere bodyCollide;

	//�d�͏����ɗ��p
	LPD3DXMESH attractorMesh;							//�d�́i���́j�������b�V��
	D3DXMATRIX* attractorMatrix;						//�d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X

	//�^�C�}�[
	float invincibleTimer;								//���G����

	//����֌W
	float reverseValueXAxis;							//����X��
	float reverseValueYAxis;							//����Y��
	bool onJump;										//�W�����v�t���O

	//�ڒn�֌W
	float difference;									//�t�B�[���h�␳����
	bool onGround;										//�ڒn����

	//�T�E���hGUI
	bool onSound;										//�T�E���hGUI�̃t���O
	int volume;											//�{�����[��
	void outputSoundGUI();								//�T�E���hGUI�̒��g
public:

	Player();
	~Player();

	//processing
	virtual void initialize(int playerType, int modelType);
	virtual void update(float frameTime);
	void toonRender(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon,
		LPD3DXEFFECT effect, LPDIRECT3DTEXTURE9 textureShade, LPDIRECT3DTEXTURE9 textureLine);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void otherRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//operation
	void configurationGravityWithRay(D3DXVECTOR3* attractorPosition, LPD3DXMESH attractorMesh, D3DXMATRIX* attractorMatrix);
	void moveOperation();
	void move(D3DXVECTOR2 moveDirection, D3DXVECTOR3 cameraAxisX, D3DXVECTOR3 cameraAxisZ);
	void jump();
	void reset();
	void updateGround(float frameTime, bool onJump);
	void controlCamera(float frameTime);
	virtual void outputGUI() override;

	//setter
	void setCamera(Camera* _camera);

	//getter
	int getState();
	BoundingSphere* getBodyCollide();	//���R���W����

};