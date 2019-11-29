//===================================================================================================================================
//�yTreeTools.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/28
// [�X�V��]2019/10/28
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y���C�u�����̃��[�h�z
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"
#include "Player.h"
#include "Tree.h"

//===================================================================================================================================
//�y�}�N���z
//===================================================================================================================================
#define TREE_CHUNK_ID		(4)
#define TREE_CHUNK			("TREE")
#define TFMT_CHUNK			("TFMT")
#define TREE_FILE_PATH		("tree.tree")
#define TREE_GUI_ID			(3)

//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct	//TREE�`�����N
{
	char chunkId[TREE_CHUNK_ID];	//�`�����NID
	short size;				//�T�C�Y
	short treeMax;			//�c���[�̐�
}TREE_TREE;

typedef struct	//TFMT�`�����N
{
	char chunkId[TREE_CHUNK_ID];	//�`�����NID
	short size;				//���̃`�����N�̃T�C�Y
	short treeId;			//�c���[��ID
	short treeType;			//�c���[�̎��
	short treeState;		//�c���[�̏��
	short treeSize;			//�c���[�̃T�C�Y
	short treeModel;		//�c���[�̃��f��
	float posX;				//�c���[�̐����ʒu(X)
	float posY;				//�c���[�̐����ʒu(Y)
	float posZ;				//�c���[�̐����ʒu(Z)
	float dirX;				//�c���[�̌���(X)
	float dirY;				//�c���[�̌���(Y)
	float dirZ;				//�c���[�̌���(Z)
}TREE_TFMT;

typedef struct	//TREE�t�@�C���\����
{
	TREE_TREE tree;			//TREE�`�����N	
	TREE_TFMT *tfmt;		//TFMT�`�����N
}TREE_FILE;

//===================================================================================================================================
//�y�c���[�c�[���z
//�c���[�c�[���p�N���X
//===================================================================================================================================
class TREE_TOOLS : public Base
{
public:
	TREE_TOOLS();
	~TREE_TOOLS();

	//�t�@�C��
	int GetTreeMax(void);									//�c���[�̐����擾
	treeNS::TREESET GetTreeSet(short treeId);				//�c���[�Z�b�g�\���̂��擾

#ifdef _DEBUG
	//�ϐ�
	int TreeListboxType;									//�c���[�̎��(���X�g�{�b�N�X)
	int TreeListboxState;									//�c���[�̏��(���X�g�{�b�N�X)
	int TreeListboxSize;									//�c���[�̃T�C�Y(���X�g�{�b�N�X)
	int TreeListboxModel;									//�c���[�̃��f��(���X�g�{�b�N�X)
	BoundingSphere *bodyCollide;							//�����蔻��
	Object** object;										//�I�u�W�F�N�g

	//GUI
	void outputTreeToolsGUI(int GUIid,						//GUI
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//�����_���[
	int  GetStaticTrunkMeshID(short treeType);				//���b�V��ID���擾(��)
	int  GetStaticLeafMeshID(short treeState);				//���b�V��ID���擾(�t)
	void initRender();										//�����_���[�̏�����
	void initObject();										//�I�u�W�F�N�g�̏�����
	void update();											//�X�V
	void render(D3DXMATRIX view,							//�`��
		D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateTrunk(Object *object, short treeType,		//�쐬
		 D3DXVECTOR3 position, D3DXVECTOR3 dir);
	void generateLeaf(Object *object, short treeState,		//�쐬
		D3DXVECTOR3 position, D3DXVECTOR3 dir);
#endif

private:

	//�ϐ�
	TREE_FILE treeFile;										//�c���[�\����

	//�c���[�t�@�C��
	void OutputTreeFile(void);								//�c���[�t�@�C���̏����o������
	void CreatNewTreeFile(void);							//�c���[�t�@�C���̐V�K�쐬

#ifdef _DEBUG
	//�ϐ�	
	StaticMeshRenderer** trunkRenderer;						//�����_���[
	StaticMeshRenderer** leafRenderer;						//�����_���[
	bool needUpdate;										//�X�V�t���O
	int TreeListboxCurrent;									//�c���[�I��p(���X�g�{�b�N�X)

	//�����_���[
	void ResetRenderer(void);								//�����_���[�����Z�b�g

	//�c���[�t�H�[�}�b�g�\����
	void UpdateTfmt(int oldTreeMax);						//�c���[�̃t�H�[�}�b�g�\���̂𐮗�
	void DeleteTreeFormat(short treeId);					//�c���[�̃t�H�[�}�b�g�\���̂�����
	void AddTreeFormat(short treeType,
		short treeState, short treeSize, short treeModel,
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//�c���[�̐ݒu
	void SetTreeType(short treeId, short treeType);			//�c���[�̎�ނ�ݒ�
	void SetTreeState(short treeId, short treeState);		//�c���[�̏�Ԃ�ݒ�
	void SetTreeSize(short treeId, short treeSize);			//�c���[�̃T�C�Y��ݒ�
	void SetTreeModel(short treeId, short treeModel);		//�c���[�̃��f����ݒ�
	void SetTreePos(short treeId, const D3DXVECTOR3 pos);	//�c���[�̈ʒu��ݒ�
	void SetTreeDir(short treeId, const D3DXVECTOR3 dir);	//�c���[�̌�����ݒ�
	void SetTree(short treeId, short treeType,				//�c���[�̐ݒu
		short treeState, short treeSize, short treeModel,
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

#endif
};