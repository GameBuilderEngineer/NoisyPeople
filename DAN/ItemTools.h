////===================================================================================================================================
////�yItemTools.h�z
//// [�쐬��]HAL����GP12A332 16 �� �F��
//// [�쐬��]2019/09/26
//// [�X�V��]2019/09/26
////===================================================================================================================================
//#pragma once
//
////===================================================================================================================================
////�y���C�u�����̃��[�h�z
////===================================================================================================================================
//#include "Base.h"
//#include "StaticMeshObject.h"
//#include "Player.h"
//#include "Item.h"
//
////===================================================================================================================================
////�y�}�N���z
////===================================================================================================================================
//#define ITEM_CHUNK_ID		(4)
//#define ITEM_CHUNK			("ITEM")
//#define IFMT_CHUNK			("IFMT")
//#define ITEM_FILE_PATH		("item.item")
//
////===================================================================================================================================
////�y�\���́z
////===================================================================================================================================
//typedef struct	//ITEM�`�����N
//{
//	char chunkId[ITEM_CHUNK_ID];	//�`�����NID
//	short size;						//�Ȍ�̃T�C�Y(�t�@�C���̃T�C�Y - 8byte)
//	short itemMax;					//�G�̐�
//}ITEM_ITEM;
//
//typedef struct	//IFMT�`�����N
//{
//	char chunkId[ITEM_CHUNK_ID];	//�`�����NID
//	short size;						//���̃`�����N�̃T�C�Y
//	short itemId;					//�A�C�e����ID
//	short itemType;					//�A�C�e���̎��
//	float posX;						//�A�C�e���̐����ʒu(X)
//	float posY;						//�A�C�e���̐����ʒu(Y)
//	float posZ;						//�A�C�e���̐����ʒu(Z)
//	float dirX;						//�A�C�e���̌���(X)
//	float dirY;						//�A�C�e���̌���(Y)
//	float dirZ;						//�A�C�e���̌���(Z)
//}ITEM_IFMT;
//
//typedef struct	//ITEM�t�@�C���\����
//{
//	ITEM_ITEM enmy;			//ITEM�`�����N	
//	ITEM_IFMT *efmt;		//IFMT�`�����N
//}ITEM_FILE;
//
////===================================================================================================================================
////�y�A�C�e���c�[���z
////�A�C�e���c�[���p�N���X
////===================================================================================================================================
//class ITEM_TOOLS : public Base
//{
//public:
//	ITEM_TOOLS();
//	~ITEM_TOOLS();
//
//	//�֐�
//	itemNS::ENEMYSET GetEnemySet(short enemyId);
//	int GetEnemyMax(void);
//	void SetEnemy(short enemyId, short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);
//
//#ifdef _DEBUG
//	//�֐�
//	void outputEnemyToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);
//
//	//�`��p
//	void initialize();
//	void update();
//	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
//	void generate(D3DXVECTOR3 position);
//#endif
//
//private:
//
//	//�ϐ�
//	ENEMY_FILE enemyFile;					//�G�l�~�[�\����
//	int EnemyListboxCurrent;				//�G�l�~�[�I��p(���X�g�{�b�N�X)
//	int EnemyListboxType;					//�G�l�~�[�̎��(���X�g�{�b�N�X)
//	int EnemyListboxState;					//�G�l�~�[�̏��(���X�g�{�b�N�X)
//
//#ifdef _DEBUG
//	//�`��p
//	StaticMeshObject* renderer;
//	bool needUpdate;
//#endif
//
//	//�֐�
//	void OutputEnemyFile(void);				//�G�l�~�[�t�@�C���̏����o������
//	void CreatNewEnemyFile(void);			//�G�l�~�[�t�@�C���̐V�K�쐬
//	void AddEnemyFormat(short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);
//	void DeleteEnemyFormat(short enemyId);	//�G�l�~�[�̃t�H�[�}�b�g�\���̂�����
//	void UpdateEfmt(int oldEnemyMax);		//�G�l�~�[�̃t�H�[�}�b�g�\���̂𐮗�
//	//void SpinEnemy(short enemyId);
//	void SetEnemyType(short enemyId, short enemyType);
//	void SetEnemyState(short enemyId, short enemyState);
//	void SetEnemyPos(short enemyId, const D3DXVECTOR3 pos);
//	void SetEnemyDir(short enemyId, const D3DXVECTOR3 dir);
//
//};