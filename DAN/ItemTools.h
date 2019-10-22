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
#include "StaticMeshObject.h"
#include "Player.h"
#include "Item.h"

//===================================================================================================================================
//�y�}�N���z
//===================================================================================================================================
#define ITEM_CHUNK_ID		(4)
#define ITEM_CHUNK			("ITEM")
#define IFMT_CHUNK			("IFMT")
#define ITEM_FILE_PATH		("item.item")

//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct	//ITEM�`�����N
{
	char chunkId[ITEM_CHUNK_ID];	//�`�����NID
	short size;						//�Ȍ�̃T�C�Y(�t�@�C���̃T�C�Y - 8byte)
	short itemMax;					//�G�̐�
}ITEM_ITEM;

typedef struct	//IFMT�`�����N
{
	char chunkId[ITEM_CHUNK_ID];	//�`�����NID
	short size;						//���̃`�����N�̃T�C�Y
	short itemId;					//�A�C�e����ID
	short itemType;					//�A�C�e���̎��
	float posX;						//�A�C�e���̐����ʒu(X)
	float posY;						//�A�C�e���̐����ʒu(Y)
	float posZ;						//�A�C�e���̐����ʒu(Z)
	float dirX;						//�A�C�e���̌���(X)
	float dirY;						//�A�C�e���̌���(Y)
	float dirZ;						//�A�C�e���̌���(Z)
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

	//�ϐ�
	bool resetItem;

	//�֐�
	itemNS::ITEMSET GetItemSet(short itemId);
	int GetItemMax(void);
	void SetItem(short itemId, short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

#ifdef _DEBUG
	//�֐�
	void outputItemToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	////�`��p
	//void initialize();
	//void update();
	//void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	//void generate(D3DXVECTOR3 position);
#endif

private:

	//�ϐ�
	ITEM_FILE itemFile;					//�A�C�e���\����
	int ItemListboxCurrent;				//�A�C�e���I��p(���X�g�{�b�N�X)
	int ItemListboxType;				//�A�C�e���̎��(���X�g�{�b�N�X)

#ifdef _DEBUG
	////�`��p
	//StaticMeshObject* renderer;
	//bool needUpdate;
#endif

	//�֐�
	void SetItemType(short itemId, short itemType);
	void SetItemPos(short itemId, const D3DXVECTOR3 pos);
	void SetItemDir(short itemId, const D3DXVECTOR3 dir);
	void OutputItemFile(void);				//�A�C�e���t�@�C���̏����o������
	void CreatNewItemFile(void);			//�A�C�e���t�@�C���̐V�K�쐬
	void AddItemFormat(short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);
	void DeleteItemFormat(short itemId);	//�A�C�e���̃t�H�[�}�b�g�\���̂�����
	void UpdateIfmt(int oldItemMax);		//�A�C�e���̃t�H�[�}�b�g�\���̂𐮗�

};