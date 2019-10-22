//===================================================================================================================================
//�yEnemyTools.h�z
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
#include "Enemy.h"
#include "EnemyManager.h"

//===================================================================================================================================
//�y�}�N���z
//===================================================================================================================================
#define ENEMY_CHUNK_ID		(4)
#define ENMY_CHUNK			("ENMY")
#define EFMT_CHUNK			("EFMT")
#define ENEMY_FILE_PATH		("enemy.enemy")

//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct	//ENMY�`�����N
{
	char chunkId[ENEMY_CHUNK_ID];	//�`�����NID
	short size;				//�Ȍ�̃T�C�Y(�t�@�C���̃T�C�Y - 8byte)
	short enemyMax;			//�G�̐�
}ENEMY_ENMY;

typedef struct	//EFMT�`�����N
{
	char chunkId[ENEMY_CHUNK_ID];	//�`�����NID
	short size;				//���̃`�����N�̃T�C�Y
	short enemyId;			//�G��ID
	short enemyType;		//�G�̎��
	short enemyState;		//�G�̏��
	float posX;				//�G�̐����ʒu(X)
	float posY;				//�G�̐����ʒu(Y)
	float posZ;				//�G�̐����ʒu(Z)
	float dirX;				//�G�̌���(X)
	float dirY;				//�G�̌���(Y)
	float dirZ;				//�G�̌���(Z)
}ENEMY_EFMT;

typedef struct	//ENEMY�t�@�C���\����
{
	ENEMY_ENMY enmy;			//ENMY�`�����N	
	ENEMY_EFMT *efmt;		//EFMT�`�����N
}ENEMY_FILE;

//===================================================================================================================================
//�y�G�l�~�[�c�[���z
//�G�l�~�[�c�[���p�N���X
//===================================================================================================================================
class ENEMY_TOOLS : public Base
{
public:
	ENEMY_TOOLS();
	~ENEMY_TOOLS();

	//�ϐ�
	bool resetEnemy;	//enemyManager��relocateEnemyAccordingToFile�֐��Ƃ���邽�߂̕ϐ�

	//�֐�
	enemyNS::ENEMYSET GetEnemySet(short enemyId);
	int GetEnemyMax(void);
	void SetEnemy(short enemyId, short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

#ifdef _DEBUG
	//�֐�
	void outputEnemyToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	////�`��p
	//void initialize();
	//void update();
	//void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	//void generate(D3DXVECTOR3 position);
#endif

private:

	//�ϐ�
	ENEMY_FILE enemyFile;					//�G�l�~�[�\����
	int EnemyListboxCurrent;				//�G�l�~�[�I��p(���X�g�{�b�N�X)
	int EnemyListboxType;					//�G�l�~�[�̎��(���X�g�{�b�N�X)
	int EnemyListboxState;					//�G�l�~�[�̏��(���X�g�{�b�N�X)

#ifdef _DEBUG
	////�`��p
	//StaticMeshObject* renderer;
	//bool needUpdate;
#endif

//�֐�
	void OutputEnemyFile(void);				//�G�l�~�[�t�@�C���̏����o������
	void CreatNewEnemyFile(void);			//�G�l�~�[�t�@�C���̐V�K�쐬
	void AddEnemyFormat(short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);
	void DeleteEnemyFormat(short enemyId);	//�G�l�~�[�̃t�H�[�}�b�g�\���̂�����
	void UpdateEfmt(int oldEnemyMax);		//�G�l�~�[�̃t�H�[�}�b�g�\���̂𐮗�
	//void SpinEnemy(short enemyId);
	void SetEnemyType(short enemyId, short enemyType);
	void SetEnemyState(short enemyId, short enemyState);
	void SetEnemyPos(short enemyId,const D3DXVECTOR3 pos);
	void SetEnemyDir(short enemyId, const D3DXVECTOR3 dir);

};