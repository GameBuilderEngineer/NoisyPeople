//-----------------------------------------------------------------------------
// �G�l�~�[�Ǘ��N���X [Enemy.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <string>
#include <vector>
#include "Base.h"
#include "Camera.h"
#include "Player.h"
#include "StaticMeshRenderer.h"
#include "LinkedList.h"
#include "EnemyTools.h"
#include "Enemy.h"
#include "Wolf.h"
#include "Tiger.h"
#include "Bear.h"

#include "Sound.h"
#include "SoundBase.h"


//=============================================================================
// ���O���
//=============================================================================
namespace enemyNS
{
	// �����ɑ��݂��Ă悢�G�l�~�[�̍ő吔
	const int ENEMY_OBJECT_MAX = 30;
}


//=============================================================================
//�N���X��`
//=============================================================================
class EnemyManager
{
private:
	LinkedList<enemyNS::EnemyData> enemyDataList;		// �G�l�~�[�f�[�^���X�g
	std::vector<Enemy*> enemyList;						// �G�l�~�[�|�C���^���X�g
	StaticMeshRenderer* wolfRenderer;					// �`��I�u�W�F�N�g
	StaticMeshRenderer* tigerRenderer;					// �`��I�u�W�F�N�g
	StaticMeshRenderer* bearRenderer;					// �`��I�u�W�F�N�g
	int nextID;											// ����G�l�~�[���sID
	LPD3DXMESH	attractorMesh;							// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;						// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X
	Player* player;										// �v���C���[

public:
	void initialize(std::string _sceneName, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix, Player* _player);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	enemyNS::EnemyData* createEnemyData(enemyNS::ENEMYSET enemySetting);
	void createEnemy(enemyNS::EnemyData* enemyData);
	void destroyEnemyData(int _enemyID);
	void destroyAllEnemyData();
	void destroyEnemy(int _enemyID);
	void destroyEnemy(Enemy* enemy);
	void destroyAllEnemy();
	void assertDestructionOrder();
	enemyNS::EnemyData* findEnemyData(int enemyID);
	int issueNewEnemyID();
	void outputGUI();
	void relocateEnemyAccordingToFile();

	// Getter
	LinkedList<enemyNS::EnemyData>* getEnemyDataList();
	std::vector<Enemy*>& getEnemyList();
	int getNextID();

#ifdef _DEBUG
	Camera* camera;
	void setDebugEnvironment(Camera* _camera, Player* _player)
	{
		camera = _camera;
		player = _player;
	}
#endif
};
