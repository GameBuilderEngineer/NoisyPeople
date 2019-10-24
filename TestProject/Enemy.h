//-----------------------------------------------------------------------------
// �G�l�~�[���N���X [Enemy.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshObject.h"


//=============================================================================
// ���O���
//=============================================================================
namespace enemyNS
{
	struct EnemyData
	{
		int type;
		int state;
		int position;
	};

}


//=============================================================================
//�N���X��`
//=============================================================================
class Enemy: public StaticMeshObject
{
private:
	int id;							// ���ʔԍ��i0..*�j
	int type;						// �G�l�~�[�^�C�v
	static int cntID;				// ���ʔԍ��J�E���^
	static int numOfEnemy;			// �G�l�~�[�̑���

public:
	Enemy();
	~Enemy();
	virtual void update();
	virtual void render();

	void setEnemyData(enemyNS::EnemyData enemyData);


	// Getter
	int getID();
	int getType();
	static int getNumOfEnemy();
};
