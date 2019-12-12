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
#include "GameMaster.h"
#include "Sound.h"
#include "SoundBase.h"
#include "EnemyChaseMark.h"


//=============================================================================
// ���O���
//=============================================================================
namespace enemyNS
{
	// �����ɑ��݂��Ă悢�G�l�~�[�̍ő吔
	const int ENEMY_OBJECT_MAX = 30;

	// �I�u�W�F�N�g�쐬��̋ߋ���(2��)
	const float NEAR_DISTANCE2 = 10000.0f;	// 100 * 100

	// �I�u�W�F�N�g�j����̉�����(2��)
	const float FAR_DISTANCE2 = 14400.0f;	// 120 * 120

	// �G�l�~�[�f�[�^���X�g�̃`�F�b�N�p�x
	const float DATA_LIST_CHECK_INTERVAL = 0.5f;
}


//=============================================================================
//�N���X��`
//=============================================================================
class EnemyManager :public Base
{
private:
	LinkedList<enemyNS::EnemyData> enemyDataList;		// �G�l�~�[�f�[�^���X�g
	std::vector<Enemy*> enemyList;						// �G�l�~�[�|�C���^���X�g
	int nextID;											// ����G�l�~�[���sID
	LPD3DXMESH	attractorMesh;							// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;						// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X
	GameMaster* gameMaster;								// �Q�[���Ǘ��I�u�W�F�N�g
	Player* player;										// �v���C���[
	float cntTimeDataList;								// �G�l�~�[�f�[�^���X�g�̃`�F�b�N���ԃJ�E���^
	EnemyChaseMark* markRenderer;						// �ǐՃ}�[�N�`��

	// �`��I�u�W�F�N�g
	StaticMeshRenderer* wolfRenderer;					// �`��I�u�W�F�N�g
	StaticMeshRenderer* tigerRenderer;					// �`��I�u�W�F�N�g
	StaticMeshRenderer* bearRenderer;					// �`��I�u�W�F�N�g
	StaticMeshRenderer* tigerBodyRenderer;				// �`��I�u�W�F�N�g
	StaticMeshRenderer* tigerBulletRenderer;


public:
	void initialize(std::string _sceneName, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix, GameMaster* _gameMaster, Player* _player);
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
	enemyNS::EnemyData* findEnemyData(int _enemyID);
	Enemy* findEnemy(int _enemyID);
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
