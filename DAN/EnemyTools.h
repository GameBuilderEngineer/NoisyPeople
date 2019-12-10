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
#include "StaticMeshRenderer.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Input.h"

//===================================================================================================================================
//�y�}�N���z
//===================================================================================================================================
#define ENEMY_CHUNK_ID		(4)
#define ENMY_CHUNK			("ENMY")
#define EFMT_CHUNK			("EFMT")
#define ENEMY_FILE_PATH		("enemy.enemy")
#define ENEMY_GUI_ID		(1)

//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct	//ENMY�`�����N
{
	char chunkId[ENEMY_CHUNK_ID];	//�`�����NID
	short size;				//�T�C�Y
	short enemyMax;			//�G�̐�
}ENEMY_ENMY;

typedef struct	//EFMT�`�����N
{
	char	chunkId[ENEMY_CHUNK_ID];	//�`�����NID
	short	size;						//���̃`�����N�̃T�C�Y
	short	enemyId;					//�G��ID
	short	enemyType;					//�G�̎��
	short	enemyState;					//�G�̏��
	float	posX;						//�G�̐����ʒu(X)
	float	posY;						//�G�̐����ʒu(Y)
	float	posZ;						//�G�̐����ʒu(Z)
	float	dirX;						//�G�̌���(X)
	float	dirY;						//�G�̌���(Y)
	float	dirZ;						//�G�̌���(Z)
	short	rootSize;					//���[�g�̗��p��
	D3DXVECTOR3	root[8];				//���[�g
}ENEMY_EFMT;

typedef struct	//ENEMY�t�@�C���\����
{
	ENEMY_ENMY enmy;		//ENMY�`�����N	
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

	//�t�@�C��
	int GetEnemyMax(void);									//�G�l�~�[�̐����擾
	enemyNS::ENEMYSET GetEnemySet(short enemyId);			//�G�l�~�[�Z�b�g�\���̂��擾

#ifdef _DEBUG
	//�ϐ�	
	int EnemyListboxType;									//�G�l�~�[�̎��(���X�g�{�b�N�X)
	BoundingSphere *bodyCollide;							//�����蔻��
	Object** object;										//�I�u�W�F�N�g
	int Model[enemyNS::TYPE_MAX];

	//GUI
	void outputEnemyToolsGUI(int GUIid,						//GUI
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);		
	void outputRootToolGUI(int enemyId, D3DXVECTOR3 pos);	//���[�g�c�[��

	//�����_���[
	void initRender();										//�����_���[�̏�����
	void initObject();										//�I�u�W�F�N�g�̏�����
	void update();											//�X�V
	void render(D3DXMATRIX view,							//�`��
		D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);			
	void generate(Object *object, short enemyType,			//�쐬
		D3DXVECTOR3 position, D3DXVECTOR3 dir);
	void collideDraw(int ID, bool use);
#endif

private:
	//�ϐ�	
	ENEMY_FILE enemyFile;									//�G�l�~�[�\����
	Input *input;

	//�G�l�~�[�t�@�C��
	void OutputEnemyFile(void);								//�G�l�~�[�t�@�C���̏����o������
	void CreatNewEnemyFile(void);							//�G�l�~�[�t�@�C���̐V�K�쐬

#ifdef _DEBUG
	//�ϐ�	
	StaticMeshRenderer** renderer;							//�����_���[
	bool rootEdit;											//���[�g�̕ҏW�t���O
	bool needUpdate;										//�X�V�t���O
	int EnemyListboxRootCurrent;							//�G�l�~�[�I��p(���X�g�{�b�N�X)
	int EnemyListboxCurrent;								//�G�l�~�[�I��p(���X�g�{�b�N�X)
	int EnemyListboxState;									//�G�l�~�[�̏��(���X�g�{�b�N�X)

	//�����_���[
	void ResetRenderer(void);								//�����_���[�����Z�b�g

	//�G�l�~�[�̐ݒu
	void SetEnemyType(short enemyId, short enemyType);		//�G�l�~�[�̎�ނ�ݒ�	
	void SetEnemyState(short enemyId, short enemyState);	//�G�l�~�[�̏�Ԃ�ݒ�
	void SetEnemyPos(short enemyId, const D3DXVECTOR3 pos);	//�G�l�~�[�̈ʒu��ݒ�
	void SetEnemyDir(short enemyId, const D3DXVECTOR3 dir);	//�G�l�~�[�̌�����ݒ�
	void SetEnemy(short enemyId, short enemyType,			//�G�l�~�[�̐ݒu
		short enemyState, const D3DXVECTOR3 pos,
		const D3DXVECTOR3 dir);

	//�G�l�~�[�t�H�[�}�b�g�\����
	void UpdateEfmt(int oldEnemyMax);						//�G�l�~�[�t�H�[�}�b�g�̐���
	void DeleteEnemyFormat(short enemyId);					//�G�l�~�[�t�H�[�}�b�g�̍폜
	void AddEnemyFormat(short enemyType,					//�G�l�~�[�t�H�[�}�b�g�̒ǉ�
		short enemyState, const D3DXVECTOR3 pos,
		const D3DXVECTOR3 dir);
#endif		
};