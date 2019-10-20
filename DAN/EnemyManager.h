//-----------------------------------------------------------------------------
// �G�l�~�[�Ǘ��N���X [Enemy.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "EnemyTools.h"
#include "Enemy.h"


//=============================================================================
// ���O���
//=============================================================================
namespace enemyNS
{
	// ���������̓f�[�^���X�g�ɑ��݂����Q�[���r������쐬�����G�l�~�[(�ǉ��G�l�~�[)
	// ���̐������z���Ď��O�Ɋm�ۂ��Ă����|�C���^�̗]���
	const int DYNAMIC_SPAWN_MAX = 20;

	const int ENEMY_OBJECT_MAX = 30;	// �����ɑ��݂��Ă悢�G�l�~�[�̍ő吔
}


//=============================================================================
//�N���X��`
//=============================================================================
class EnemyManager
{
private:
	std::vector<enemyNS::EnemyData> enemyDataList;		// �G�l�~�[�f�[�^���X�g
	std::vector<Enemy*> enemyList;						// �G�l�~�[�|�C���^���X�g
	StaticMeshObject* wolfRenderer;
	StaticMeshObject* tigerRenderer;
	StaticMeshObject* bearRenderer;
	int nextID;

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createEnemy(enemyNS::EnemyData* enemyData);
	void destroyEnemy(int _id);
	void destroyAllEnemy();
	int issueNewID();
	void outputGUI();
};
