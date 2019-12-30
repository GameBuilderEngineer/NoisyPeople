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
#include "Marker.h"


//=============================================================================
// ���O���
//=============================================================================
namespace enemyNS
{
	// �����ɑ��݂��Ă悢�G�l�~�[�̍ő吔
	const int ENEMY_OBJECT_MAX = 30;

	// �I�u�W�F�N�g�쐬��̋ߋ���(2��)
	const float NEAR_DISTANCE2 = 22500.0f;	// 150 * 150

	// �I�u�W�F�N�g�j����̉�����(2��)
	const float FAR_DISTANCE2 = 28900.0f;	// 170 * 170

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
	bool canUpdate;										// �A�b�v�f�[�g���s�t���O

public:
	// �`��I�u�W�F�N�g
	static StaticMeshRenderer* wolfBodyRenderer;		// WOLF�̓���
	static StaticMeshRenderer* wolfArmRenderer;			// WOLF�̘r
	static StaticMeshRenderer* tigerBodyRenderer;		// TIGER�̓���
	static StaticMeshRenderer* tigerGunRenderer;		// TIGER�̏e��
	static StaticMeshRenderer* tigerLegLRenderer;		// TIGER�̍���
	static StaticMeshRenderer* tigerLegRRenderer;		// TIGER�̉E��
	static StaticMeshRenderer* bearBodyRenderer;		// BEAR�̓���
	static StaticMeshRenderer* bearArmLRenderer;		// BEAR�̍��r
	static StaticMeshRenderer* bearArmRRenderer;		// BEAR�̉E�r
	static StaticMeshRenderer* bearWaistRenderer;		// BEAR�̍�
	static StaticMeshRenderer* bearLegLRenderer;		// BEAR�̍���
	static StaticMeshRenderer* bearLegRRenderer;		// BEAR�̉E��
	static EnemyChaseMark* markRenderer;				// �ǐՃ}�[�N�`��
	MarkerRenderer*	markerRenderer;						// �}�[�J�[�iBEAR�ʒu�\���p�j

	void initialize(std::string _sceneName, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix, GameMaster* _gameMaster, Player* _player, MarkerRenderer*	_markerRenderer);
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
	void updatePartsRenderer();
	int issueNewEnemyID();
	void outputGUI();
	void relocateEnemyAccordingToFile();

	// Getter
	LinkedList<enemyNS::EnemyData>* getEnemyDataList();
	std::vector<Enemy*>& getEnemyList();
	int getNextID();

	// Setter
	void setUpdate(bool setting);

#ifdef _DEBUG
	Camera* camera;
	void setDebugEnvironment(Camera* _camera, Player* _player)
	{
		camera = _camera;
		player = _player;
	}
#endif
};
