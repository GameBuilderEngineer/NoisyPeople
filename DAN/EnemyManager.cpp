//-----------------------------------------------------------------------------
// �G�l�~�[�Ǘ��N���X [Enemy.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include <cassert>
#include "EnemyManager.h"
#include "ImguiManager.h"
#include "EnemyTools.h"
using namespace enemyNS;


//=============================================================================
// ������
//=============================================================================
void EnemyManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;								// ���񔭍sID��0�ɏ�����
	Enemy::resetNumOfEnemy();				// �G�l�~�[�I�u�W�F�N�g�̐���������
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()�œ��I�Ȋm�ۂ������ςނ悤�������𑝂₵�Ă���

	// �ڒn�t�B�[���h�����Z�b�g
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// �`��I�u�W�F�N�g�̍쐬
	wolfRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::ENEMY_01));
	tigerRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON));
	bearRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON_X10));

#if 0	// �G�l�~�[�c�[���̃f�[�^��ǂݍ���
	ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
	playParameters = new PLAY_PARAMETERS[enemyTools->GetEnemyMax()*gameMasterNS::PLAYER_NUM];
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		for (int j = 0; j < gameMasterNS::PLAYER_NUM; j++)
		{
			//3D�T�E���h
			switch (enemyTools->GetEnemySet(i).type)
			{
			case ENEMY_TYPE::WOLF:
				playParameters[(i*gameMasterNS::PLAYER_NUM) + j] = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, GAME_S3D_LIST::GAME_S3D_FOOTSTEP_01, true ,NULL,true,j };
				break;
			case ENEMY_TYPE::TIGER:
				playParameters[(i*gameMasterNS::PLAYER_NUM) + j] = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, GAME_S3D_LIST::GAME_S3D_FOOTSTEP_02, true ,NULL,true,j };
				break;
			case ENEMY_TYPE::BEAR:
				playParameters[(i*gameMasterNS::PLAYER_NUM) + j] = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, GAME_S3D_LIST::GAME_S3D_FOOTSTEP_03, true ,NULL,true,j };
				break;
			default:
				playParameters[(i*gameMasterNS::PLAYER_NUM) + j] = { ENDPOINT_VOICE_LIST::ENDPOINT_S3D, GAME_S3D_LIST::GAME_S3D_FOOTSTEP_03, true ,NULL,true,j };
				break;
			}
			SoundInterface::S3D->playSound(&playParameters[(i*gameMasterNS::PLAYER_NUM) + j]);
		}

		createEnemyData(enemyTools->GetEnemySet(i));
	}
	SAFE_DELETE(enemyTools);
#endif

#if 0	// �G�l�~�[�I�u�W�F�N�g���c�[���f�[�^�����ɍ쐬����
	for (size_t i = 0; i < enemyDataList.nodeNum; i++)
	{
		if (1/* �{���̓v���C���[�̏����ʒu�Ƌ߂���΁`�ȂǏ������t�� */)
		{
			createEnemy(enemyDataList.getValue(i));
		}
	}
#endif
}


//=============================================================================
// �I������
//=============================================================================
void EnemyManager::uninitialize()
{
	// �S�ẴG�l�~�[�I�u�W�F�N�g��j��
	destroyAllEnemy();

	// �S�ẴG�l�~�[�f�[�^��j��
	destroyAllEnemyData();

	// �x�N�^�[�̊m�ۃ��������������i�������A���P�[�^�����ɖ߂��j
	std::vector<Enemy*> temp;
	enemyList.swap(temp);

	// �`��I�u�W�F�N�g��j��
	SAFE_DELETE(wolfRenderer);
	SAFE_DELETE(tigerRenderer);
	SAFE_DELETE(bearRenderer);
}

//=============================================================================
// �X�V����
//=============================================================================
void EnemyManager::update(float frameTime)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->update(frameTime);

		// ���ނ����G�l�~�[�I�u�W�F�N�g��j������
		if (enemyList[i]->getEnemyData()->isAlive == false)
		{
			destroyEnemy(enemyList[i]->getEnemyData()->enemyID);
		}
	}
	wolfRenderer->update();
	tigerRenderer->update();
	bearRenderer->update();

}


//=============================================================================
// �`�揈��
//=============================================================================
void EnemyManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	wolfRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	tigerRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	bearRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// �G�l�~�[�f�[�^�̍쐬
//=============================================================================
enemyNS::EnemyData* EnemyManager::createEnemyData(enemyNS::ENEMYSET enemySetting)
{
	// ENEMYSET�\���̂���EnemyData�����
	EnemyData enemyData;
	enemyData.enemyID = enemySetting.enemyID;
	enemyData.type = enemySetting.type;
	enemyData.defaultState = enemySetting.defaultState;
	enemyData.defaultPosition = enemySetting.defaultPosition;
	enemyData.defaultDirection = enemySetting.defaultDirection;
	enemyData.setUp();
	// ���X�g�ɒǉ�
	enemyDataList.insertFront(enemyData);
	enemyDataList.listUpdate();
	// �G�l�~�[�f�[�^ID�̍ő�l���X�V�i�Ō�ɒǉ������f�[�^ + 1�����񔭍sID�ƂȂ�j
	nextID = enemyData.enemyID + 1;
	// ���X�g�ɒǉ������f�[�^�̃|�C���^��Ԃ�
	return enemyDataList.getValue(0);
}


//=============================================================================
// �G�l�~�[�I�u�W�F�N�g���쐬
//=============================================================================
void EnemyManager::createEnemy(EnemyData* enemyData)
{
	Enemy* enemy = NULL;

	switch (enemyData->type)
	{
	case WOLF:
		enemy = new Wolf(staticMeshNS::reference(staticMeshNS::ENEMY_01), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		wolfRenderer->registerObject(enemy);
		break;

	case TIGER:
		enemy = new Tiger(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		tigerRenderer->registerObject(enemy);
		break;

	case BEAR:
		enemy = new Bear(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON_X10), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		bearRenderer->registerObject(enemy);
		break;
	}
}


//=============================================================================
// �G�l�~�[�f�[�^�̔j��
//=============================================================================
void EnemyManager::destroyEnemyData(int _enemyID)
{
	for (int i = 0; i < enemyDataList.nodeNum; i++)
	{
		if (enemyDataList.getValue(i)->enemyID != _enemyID) { continue; }
		// �G�l�~�[�f�[�^�\���̂̓m�[�h���Ƀ�����������Ă��邽��
		// �m�[�h�̔j���ƍ��킹�ăG�l�~�[�f�[�^���j������Ă���
		enemyDataList.remove(enemyDataList.getNode(i));
		enemyDataList.listUpdate();
		break;
	}

#ifdef _DEBUG
	assertDestructionOrder();
#endif//_DEBUG
}


//=============================================================================
// �S�ẴG�l�~�[�f�[�^��j��
//=============================================================================
void EnemyManager::destroyAllEnemyData()
{
	// �G�l�~�[�f�[�^�\���̂̓m�[�h���Ƀ�����������Ă��邽��
	// �m�[�h�̔j���ƍ��킹�ăG�l�~�[�f�[�^���j������Ă���
	enemyDataList.allClear();
	enemyDataList.listUpdate();

#ifdef _DEBUG
	assertDestructionOrder();
#endif//_DEBUG
}


//=============================================================================
// �G�l�~�[�I�u�W�F�N�g��j��
//=============================================================================
void EnemyManager::destroyEnemy(int _enemyID)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->enemyID == _enemyID)
		{
			// �`��̉���
			switch (enemyList[i]->getEnemyData()->type)
			{
			case WOLF:
				wolfRenderer->unRegisterObjectByID(enemyList[i]->id);
				break;

			case TIGER:
				tigerRenderer->unRegisterObjectByID(enemyList[i]->id);
				break;

			case BEAR:
				bearRenderer->unRegisterObjectByID(enemyList[i]->id);
				break;
			}
			SAFE_DELETE(enemyList[i]);				// �C���X�^���X�j��
			enemyList.erase(enemyList.begin() + i);	// �x�N�^�[�v�f������
			break;
		}
	}
}


//=============================================================================
// �S�ẴG�l�~�[�I�u�W�F�N�g��j��
//=============================================================================
void EnemyManager::destroyAllEnemy()
{
	// �`��S����
	wolfRenderer->allUnRegister();
	tigerRenderer->allUnRegister();
	bearRenderer->allUnRegister();

	for (size_t i = 0; i < enemyList.size(); i++)
	{
		SAFE_DELETE(enemyList[i]);
	}
	enemyList.clear();
}


//=============================================================================
// �j�������ᔽ
//=============================================================================
void EnemyManager::assertDestructionOrder()
{
	// �G�l�~�[�̎Q�Ɛ�̃G�l�~�[�f�[�^���G�l�~�[�f�[�^���X�g�ɂȂ��ꍇ�̃A�T�[�V����
	// ��Ƀf�[�^��j�����Č�ɃG�l�~�[��j�����悤�Ƃ���ꍇ�ɊY������
	int cnt = 0;
	for (int i = 0; i < enemyDataList.nodeNum; i++)
	{
		for (int k = 0; k < enemyList.size(); k++)
		{
			if (enemyDataList.getValue(i)->enemyID == enemyList[k]->getEnemyData()->enemyID)
			{
				cnt++;
			}
		}
	}
	if (cnt != enemyList.size())
	{
		int unko = 1;
	}
	assert(cnt == enemyList.size());
	assert(enemyDataList.nodeNum >= enemyList.size());
}


//=============================================================================
// �G�l�~�[ID�𔭍s����
//=============================================================================
int EnemyManager::issueNewEnemyID()
{
	int ans = nextID;
	nextID++;
	return ans;
}


//=============================================================================
// ImGui�ɏo��
//=============================================================================
void EnemyManager::outputGUI()
{
#ifdef _DEBUG

	// �t���O
	bool createFlag = false;
	bool createDebugEnemy = false;
	bool returnPlayer = false;
	bool destroyAllFlag = false;

	if (ImGui::CollapsingHeader("EnemyInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("enemyDataList.nodeNum:%d\n", enemyDataList.nodeNum);
		ImGui::Text("enemyList.size()     :%d\n", enemyList.size());
		ImGui::Text("numOfEnemy           :%d\n", Enemy::getNumOfEnemy());
		ImGui::Text("nextID	:%d\n", nextID);

		// �{�^��
		ImGui::Checkbox("Create Enemy", &createFlag);
		ImGui::Checkbox("Create Debug Enemy", &createDebugEnemy);
		ImGui::Checkbox("Return Player", &returnPlayer);
		ImGui::Checkbox("Delete All Enemy", &destroyAllFlag);
	}

	if (createFlag)
	{
		enemyNS::ENEMYSET tmp =
		{
			issueNewEnemyID(),
			enemyNS::WOLF,
			enemyNS::CHASE,
			*player->getPosition(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		};
		EnemyData* p = createEnemyData(tmp);
		createEnemy(p);
	}

	if (createDebugEnemy)
	{
		Enemy* debugEnemy = getEnemyList().back();
		debugEnemy->debugEnemyID = debugEnemy->getEnemyID();
		camera->setTarget(debugEnemy->getPosition());
		camera->setTargetX(&debugEnemy->getAxisX()->direction);
		camera->setTargetY(&debugEnemy->getAxisY()->direction);
		camera->setTargetZ(&debugEnemy->getAxisZ()->direction);
		debugEnemy->setDebugEnvironment();
		debugEnemy->setCamera(&camera[0]);
		debugEnemy->setDebugEnvironment();
	}

	if (returnPlayer)
	{
		camera->setTarget(player->getPosition());
		camera->setTargetX(&player->getAxisX()->direction);
		camera->setTargetY(&player->getAxisY()->direction);
		camera->setTargetZ(&player->getAxisZ()->direction);
	}

	if (destroyAllFlag)
	{
		destroyAllEnemy();
		destroyAllEnemyData();
	}
#endif
}


//=============================================================================
// �t�@�C���ɏ������ăG�l�~�[�I�u�W�F�N�g���Ĕz�u����
//=============================================================================
void EnemyManager::relocateEnemyAccordingToFile()
{
	destroyAllEnemy();						// �S�ẴG�l�~�[�I�u�W�F�N�g��j��
	destroyAllEnemyData();					// �S�ẴG�l�~�[�f�[�^��j��
	nextID = 0;								// ���񔭍sID��0�ɏ�����
	Enemy::resetNumOfEnemy();				// �G�l�~�[�I�u�W�F�N�g�̐���������
	enemyList.reserve(ENEMY_OBJECT_MAX);		// update()�œ��I�Ȋm�ۂ������ςނ悤�������𑝂₵�Ă���

	// �G�l�~�[�c�[���̃f�[�^��ǂݍ���
	ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		createEnemyData(enemyTools->GetEnemySet(i));
	}
	SAFE_DELETE(enemyTools);

	// �G�l�~�[�I�u�W�F�N�g���c�[���f�[�^�����ɍ쐬����
	for (size_t i = 0; i < enemyDataList.nodeNum; i++)
	{
		createEnemy(enemyDataList.getValue(i));
	}
}

//=============================================================================
// �I������(�T�E���h)
//=============================================================================
void EnemyManager::uninitializeSound()
{
	SoundInterface::S3D->uninitSoundStop();

	SAFE_DELETE_ARRAY(playParameters);
}

//=============================================================================
// �����̏���
//=============================================================================
void EnemyManager::footsteps(D3DXVECTOR3 playerPos, int playerID)
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		float distance = D3DXVec3Length(&(enemyDataList.getValue(i)->position - playerPos));
		float volume = 0.0f;
		if (distance < DISTANCE_MAX)
		{
			volume = (DISTANCE_MAX - distance) / DISTANCE_MAX;
		}

		SoundInterface::S3D->SetVolume(playParameters[(i*gameMasterNS::PLAYER_NUM) + playerID], volume);
	}
}

//=============================================================================
// Getter
//=============================================================================
LinkedList<enemyNS::EnemyData>* EnemyManager::getEnemyDataList() { return &enemyDataList; }
std::vector<Enemy*>& EnemyManager::getEnemyList() { return enemyList; }
int EnemyManager::getNextID() { return nextID; }

