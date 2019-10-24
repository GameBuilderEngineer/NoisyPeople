//===================================================================================================================================
//�yItemTools.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/26
// [�X�V��]2019/09/26
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y���C�u�����̃��[�h�z
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"
#include "Player.h"
#include "Item.h"

//===================================================================================================================================
//�y�}�N���z
//===================================================================================================================================
#define ITEM_CHUNK_ID		(4)
#define ITEM_CHUNK			("ITEM")
#define IFMT_CHUNK			("IFMT")
#define ITEM_FILE_PATH		("item.item")
#define ITEM_GUI_ID			(2)

//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct	//ITEM�`�����N
{
	char chunkId[ITEM_CHUNK_ID];	//�`�����NID
	short size;				//�T�C�Y
	short itemMax;			//�A�C�e���̐�
}ITEM_ITEM;

typedef struct	//IFMT�`�����N
{
	char chunkId[ITEM_CHUNK_ID];	//�`�����NID
	short size;				//���̃`�����N�̃T�C�Y
	short itemId;			//�A�C�e����ID
	short itemType;			//�A�C�e���̎��
	float posX;				//�A�C�e���̐����ʒu(X)
	float posY;				//�A�C�e���̐����ʒu(Y)
	float posZ;				//�A�C�e���̐����ʒu(Z)
	float dirX;				//�A�C�e���̌���(X)
	float dirY;				//�A�C�e���̌���(Y)
	float dirZ;				//�A�C�e���̌���(Z)
}ITEM_IFMT;

typedef struct	//ITEM�t�@�C���\����
{
	ITEM_ITEM item;			//ITEM�`�����N	
	ITEM_IFMT *ifmt;		//IFMT�`�����N
}ITEM_FILE;

//===================================================================================================================================
//�y�A�C�e���c�[���z
//�A�C�e���c�[���p�N���X
//===================================================================================================================================
class ITEM_TOOLS : public Base
{
public:
	ITEM_TOOLS();
	~ITEM_TOOLS();

	//�t�@�C��
	int GetItemMax(void);									//�A�C�e���̐����擾
	itemNS::ITEMSET GetItemSet(short itemId);				//�A�C�e���Z�b�g�\���̂��擾

#ifdef _DEBUG
	//�ϐ�	
	BoundingSphere *bodyCollide;							//�����蔻��
	Object** object;										//�I�u�W�F�N�g

	//GUI
	void outputItemToolsGUI(int GUIid,						//GUI
		const D3DXVECTOR3 pos,const D3DXVECTOR3 dir);

	//�����_���[
	void initRender();										//�����_���[�̏�����
	void initObject();										//�I�u�W�F�N�g�̏�����
	void update();											//�X�V
	void render(D3DXMATRIX view,							//�`��
		D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generate(Object *object, short enemyType,			//�쐬
		D3DXVECTOR3 position);
#endif

private:

	//�ϐ�
	ITEM_FILE itemFile;										//�A�C�e���\����

	//�A�C�e���t�@�C��
	void OutputItemFile(void);								//�A�C�e���t�@�C���̏����o������
	void CreatNewItemFile(void);							//�A�C�e���t�@�C���̐V�K�쐬

#ifdef _DEBUG
	//�ϐ�	
	StaticMeshRenderer** renderer;							//�����_���[
	bool needUpdate;										//�X�V�t���O
	int ItemListboxCurrent;									//�A�C�e���I��p(���X�g�{�b�N�X)
	int ItemListboxType;									//�A�C�e���̎��(���X�g�{�b�N�X)

	//�����_���[
	void ResetRenderer(void);								//�����_���[�����Z�b�g
	int  GetStaticMeshNo(short itemType);					//���b�V��ID���擾

	//�A�C�e���t�H�[�}�b�g�\����
	void UpdateIfmt(int oldItemMax);						//�A�C�e���̃t�H�[�}�b�g�\���̂𐮗�
	void DeleteItemFormat(short itemId);					//�A�C�e���̃t�H�[�}�b�g�\���̂�����
	void AddItemFormat(short itemType,
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//�A�C�e���̐ݒu
	void SetItemType(short itemId, short itemType);			//�A�C�e���̎�ނ�ݒ�	
	void SetItemPos(short itemId, const D3DXVECTOR3 pos);	//�A�C�e���̏�Ԃ�ݒ�
	void SetItemDir(short itemId, const D3DXVECTOR3 dir);	//�A�C�e���̈ʒu��ݒ�
	void SetItem(short itemId, short itemType,				//�A�C�e���̌�����ݒ�
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);		//�A�C�e���̐ݒu

#endif
};