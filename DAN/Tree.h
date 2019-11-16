//-----------------------------------------------------------------------------
// �c���[���N���X [Tree.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
// �X�V�� : 2019/11/12 �y���� ���z
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "GreeningArea.h"

//=============================================================================
// ���O���
//=============================================================================
namespace treeNS
{
	// �c���[���
	enum TREE_TYPE
	{
		ANALOG_TREE,
		DIGITAL_TREE,
		TREE_TYPE_MAX
	};

	// �Ή����
	enum GREEN_STATE
	{
		GREEN,
		DEAD,
		GREEN_STATE_MAX
	};

	// �T�C�Y
	enum TREE_SIZE
	{
		STANDARD,
		LARGE,
		VERY_LARGE,
		TREE_SIZE_MAX
	};

	// ���f��
	enum TREE_MODEL
	{
		A_MODEL,
		B_MODEL,
		C_MODEL,
		TREE_MAX
	};

	//�c���[�f�[�^
	typedef struct TreeData
	{
		int treeID;						// ���ʔԍ�
		int model	;					// ���f��ID
		TREE_TYPE type;					// �c���[�^�C�v
		GREEN_STATE greenState;			// �Ή����
		TREE_SIZE size;					// �؂̃T�C�Y
		int hp;							// HP
		bool isAttaked;					// �G�l�~�[���U����
		int digitalHp;					// A/D�ϊ���G�l�~�[�����HP
		D3DXVECTOR3 initialPosition;	// �������W
		D3DXVECTOR3 initialDirection;	// �������p
		void zeroClear() { ZeroMemory(this, sizeof(TreeData)); }
	} TREESET;

	//�萔
	const int	MAX_HP					= 100;		//�ő�HP
	const float	AROUND_GREEN_TIME		= 5.0f;		//���ӂւ̗Ή�����
	const float	AROUND_GREEN_RANGE_S	= 50.0f;	//���ӂւ̗Ή��͈�
	const float	AROUND_GREEN_RANGE_L	= 100.0f;	//���ӂւ̗Ή��͈�
	const float	AROUND_GREEN_RANGE_V	= 200.0f;	//���ӂւ̗Ή��͈�


	//=============================================================================
	//�y���ۏ�ԃN���X�z
	//=============================================================================
	class AbstractState
	{
	public:
		float	frameTime;
		int		type;
	public:
		virtual void start() = 0;
		virtual void update(float frameTime) = 0;
		virtual void end() = 0;
		virtual AbstractState* transition() = 0;
	};
}


//=============================================================================
//�y�c���[�N���X�z
//=============================================================================
class Tree:public Object
{
private:
	treeNS::TreeData		treeData;			// �c���[�f�[�^
	Object					leaf;				// �t�I�u�W�F�N�g
	GreeningArea			greeningArea;		// �Ή��͈�
	LPD3DXMESH				attractorMesh;		// �d�́i���́j�������b�V��
	D3DXMATRIX*				attractorMatrix;	// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X
	static int				numOfTree;			// �c���[�I�u�W�F�N�g�̑���
	treeNS::AbstractState*	state;				//��ԃN���X
	bool					onTransState;		//��ԑJ�ڊJ�n�t���O
	bool					nowAroundGreening;	//���ݎ��͂�Ή���
public:
	Tree(treeNS::TreeData _treeData);
	~Tree();
	//��{����
	void update(float frameTime);
	void render();

	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void grounding();					// �ڒn����
	void greeningAround();				//���ӂ̗Ή�(�f�W�^������)
	void transState();					//��ԑJ��

	// Getter
	Object* getLeaf();
	Object* getTrunk();
	Object* getGreeningArea();
	static int getNumOfTree();			// �c���[�̐����擾
	treeNS::TreeData* getTreeData();	// �c���[�f�[�^���擾
	LPD3DXMESH getMesh();				// �Փ˗p���b�V���ւ̎Q��
	bool isAroundGreening();			//���͂֗Ή�����

	// Setter
	void setDataToTree(treeNS::TreeData _treeData);	
	void addHp(int value);							//�f�W�^��������HP�̑���
	void setGreeningArea(float value);				//�Ή��G���A�̃X�P�[����ݒ�
	void disableAroundGreening();					//���͂ւ̗Ή����I��

};

//=============================================================================
//�y���O��ԁz
//=============================================================================
namespace treeNS
{
	//=============================================================================
	//�y�f�W�^����ԃN���X�z
	//=============================================================================
	class DigitalState :public AbstractState
	{
	private:
		Tree* tree;
		float aroundGreenTimer;//���͂ւ̗Ή��^�C�}�[
		float aroundGreenRange;
	public:
		DigitalState(Tree* target);
		~DigitalState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

	//=============================================================================
	//�y�A�i���O��ԃN���X�z
	//=============================================================================
	class AnalogState :public AbstractState
	{
	private:
		Tree* tree;
	public:
		AnalogState(Tree* target);
		~AnalogState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

	//=============================================================================
	//�y�Ή���ԃN���X�z
	//=============================================================================
	class GreenState :public AbstractState
	{
	private:
		Tree* tree;
	public:
		GreenState(Tree* target);
		~GreenState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

}