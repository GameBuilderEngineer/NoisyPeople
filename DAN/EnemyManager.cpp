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
void EnemyManager::initialize(std::string _sceneName, LPD3DXMESH _attractorMesh,
	D3DXMATRIX* _attractorMatrix, GameMaster* _gameMaster, Player* _player)
{
	nextID = 0;								// ���񔭍sID��0�ɏ�����
	Enemy::resetNumOfEnemy();				// �G�l�~�[�I�u�W�F�N�g�̐���������
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()�œ��I�Ȋm�ۂ������ςނ悤�������𑝂₵�Ă���
	cntTimeDataList = 0.0f;

	// �ڒn�t�B�[���h�ƃv���C���[���Z�b�g
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
	gameMaster = _gameMaster;
	player = _player;

	// �`��I�u�W�F�N�g�̍쐬
	wolfRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::WOLF));
	tigerRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::TIGER));
	bearRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::BEAR));

// �`���[�g���A���V�[���ł̏���������
if (_sceneName == "Scene -Tutorial-")
{

	return;
}

#if 1	// �G�l�~�[�c�[���̃f�[�^��ǂݍ���
ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
{
	createEnemyData(enemyTools->GetEnemySet(i));
}
SAFE_DELETE(enemyTools);
#endif

#if 1	// �G�l�~�[�I�u�W�F�N�g���c�[���f�[�^�����ɍ쐬����
for (int i = 0; i < enemyDataList.nodeNum; i++)
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
	//----------------------
	// �G�l�~�[�f�[�^���X�g
	//----------------------
	cntTimeDataList += frameTime;
	if (cntTimeDataList > DATA_LIST_CHECK_INTERVAL)
	{
		cntTimeDataList = 0.0f;

		for (int i = 0; i < enemyDataList.nodeNum; i++)
		{
			// ���݂��Ă��� or ���S�ς݂̃f�[�^���p�X����
			if (enemyDataList.getValue(i)->isObjectExists ||
				enemyDataList.getValue(i)->isAlive == false)
			{
				continue;
			}

			// �ߋ����G�l�~�[�̎����쐬
			float dist1 = D3DXVec3LengthSq(&(enemyDataList.getValue(i)->position - player[gameMasterNS::PLAYER_1P].position));
			float dist2 = D3DXVec3LengthSq(&(enemyDataList.getValue(i)->position - player[gameMasterNS::PLAYER_2P].position));
			if (dist1 < NEAR_DISTANCE2 || dist2 < NEAR_DISTANCE2)
			{
				enemyDataList.getValue(i)->setUp();		// �����z�u�G�l�~�[�̃f�[�^������������
				createEnemy(enemyDataList.getValue(i));
			}
		}
	}

	//----------------
	// �G�l�~�[���X�g
	//----------------
	vector<Enemy*>::iterator itr = enemyList.begin();
	while(itr != enemyList.end())
	{
		// �G�l�~�[�̍X�V
		(*itr)->update(frameTime);

		// �j���t���O
		bool isDestroyTarget = false;

		// �������G�l�~�[��j������
		float dist1 = D3DXVec3LengthSq(&((*itr)->position - player[gameMasterNS::PLAYER_1P].position));
		float dist2 = D3DXVec3LengthSq(&((*itr)->position - player[gameMasterNS::PLAYER_2P].position));
		if ((dist1 > FAR_DISTANCE2 && dist2 > FAR_DISTANCE2) && (*itr)->getEnemyData()->type != enemyNS::BEAR)
		{
			isDestroyTarget = true;
		}

		// ���S�ς݃G�l�~�[��j������
		if ((*itr)->getEnemyData()->isAlive == false)
		{
			isDestroyTarget = true;
		}

		// �j������
		if (isDestroyTarget)
		{
			// ���I�쐬�C�x���g�ō�����G�l�~�[��ID���L�^����
			int destroyTargetEnemyData = -1;
			if ((*itr)->getEnemyData()->isGeneratedBySpawnEvent)
			{
				destroyTargetEnemyData = (*itr)->getEnemyID();
			}

			// �G�l�~�[�I�u�W�F�N�g�̔j��
			(*itr)->getEnemyData()->isObjectExists = false;
			destroyEnemy(*itr);
			itr = enemyList.erase(itr);

			// ���I�쐬�C�x���g�ō�����G�l�~�[�̂݃G�l�~�[�f�[�^���j������i�G�l�~�[�̔j����j
			if (destroyTargetEnemyData != -1)
			{
				destroyEnemyData(destroyTargetEnemyData);
			}
		}
		else
		{
			itr++;
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

#ifdef _DEBUG

	for (size_t i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->debugRender();

		// �Z���T�[��`��
#ifdef RENDER_SENSOR
		for (int k = 0; k < 4; k++)
		{
			enemyList[i]->eyeAngleRay[k].render(VISIBLE_DISTANCE[enemyList[i]->getEnemyData()->type]);
		}
		float len = D3DXVec3Length(&(player[0].center - enemyList[i]->center));
		enemyList[i]->hearingSphere[0].render(enemyList[i]->matrixCenter);
		enemyList[i]->hearingSphere[1].render(enemyList[i]->matrixCenter);
#endif

	}

#endif// _DEBUG
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
	enemyData.numRoute = enemySetting.numRoute;
	//enemyData.patrolRoute = new D3DXVECTOR3[enemyData.numRoute];
	//memcpy(enemyData.patrolRoute, (const void*)enemySetting.numRoute, sizeof(D3DXVECTOR3) * enemyData.numRoute);
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

	enemyNS::ConstructionPackage constructionPackage;
	constructionPackage.enemyData = enemyData;
	constructionPackage.gameMaster = gameMaster;
	constructionPackage.player = player;
	constructionPackage.attractorMesh = attractorMesh;
	constructionPackage.attractorMatrix = attractorMatrix;
	constructionPackage.enemyData->isObjectExists = true;

	switch (enemyData->type)
	{
	case WOLF:
		constructionPackage.staticMesh = staticMeshNS::reference(staticMeshNS::WOLF);
		enemy = new Wolf(constructionPackage);
		wolfRenderer->registerObject(enemy);
		wolfRenderer->updateAccessList();
		break;

	case TIGER:
		constructionPackage.staticMesh = staticMeshNS::reference(staticMeshNS::TIGER);
		enemy = new Tiger(constructionPackage);
		tigerRenderer->registerObject(enemy);
		tigerRenderer->updateAccessList();
		break;

	case BEAR:
		constructionPackage.staticMesh = staticMeshNS::reference(staticMeshNS::BEAR);
		enemy = new Bear(constructionPackage);
		bearRenderer->registerObject(enemy);
		tigerRenderer->updateAccessList();
		break;
	}

	enemyList.emplace_back(enemy);
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
// �G�l�~�[�I�u�W�F�N�g��j�����̂P
//=============================================================================
void EnemyManager::destroyEnemy(int _enemyID)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->enemyID == _enemyID)
		{
			destroyEnemy(enemyList[i]);	
			enemyList.erase(enemyList.begin() + i);	// �x�N�^�[�v�f������
			break;
		}
	}
}


//=============================================================================
// �G�l�~�[�I�u�W�F�N�g��j�����̂Q�i���ۂ̔j���͂�����j
//=============================================================================
void EnemyManager::destroyEnemy(Enemy* enemy)
{
	// �`��̉���
	switch (enemy->getEnemyData()->type)
	{
	case WOLF:
		//wolfRenderer->updateAccessList();
		wolfRenderer->unRegisterObjectByID(enemy->id);
		break;

	case TIGER:
		//tigerRenderer->updateAccessList();
		tigerRenderer->unRegisterObjectByID(enemy->id);
		break;

	case BEAR:
		//bearRenderer->updateAccessList();
		bearRenderer->unRegisterObjectByID(enemy->id);
		break;
	}
	enemy->treeCell.remove();					// �Փ˔��胊�X�g����폜
	SAFE_DELETE(enemy);							// �C���X�^���X�j��
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
		enemyList[i]->treeCell.remove();		// �Փ˔��胊�X�g����폜
		SAFE_DELETE(enemyList[i]);				// �C���X�^���X�j��
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
// �G�l�~�[�f�[�^��T��
//=============================================================================
enemyNS::EnemyData* EnemyManager::findEnemyData(int _enemyID)
{
	for (int i = 0; i < enemyDataList.nodeNum; i++)
	{
		if (enemyDataList.getValue(i)->enemyID == _enemyID)
		{
			return enemyDataList.getValue(i);
		}
	}

	return NULL;
}


//=============================================================================
// �G�l�~�[��T��
//=============================================================================
Enemy* EnemyManager::findEnemy(int _enemyID)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyID() == _enemyID)
		{
			return enemyList[i];
		}
	}
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
		//ImGui::Text("wolfRenderer.objectNum:%d\n", wolfRenderer->objectNum);
		//ImGui::Text("tigerRenderer.objectNum:%d\n", tigerRenderer->objectNum);
		//ImGui::Text("bearRenderer.objectNum:%d\n", bearRenderer->objectNum);

		// �{�^��
		ImGui::Checkbox("Create Enemy", &createFlag);
		ImGui::Checkbox("Create Debug Enemy", &createDebugEnemy);
		ImGui::Checkbox("Return Player", &returnPlayer);
		ImGui::Checkbox("Delete All Enemy", &destroyAllFlag);

		// �f�o�b�O�G�l�~�[
		for (size_t i = 0; i < enemyList.size(); i++)
		{
			if (enemyList[i]->getEnemyID() == Enemy::debugEnemyID)
			{
				//ImGui::Text("horizontalAngle:%f\n", enemyList[i]->horizontalAngle);
			}
		}
	}

	if (createFlag)
	{
		enemyNS::ENEMYSET tmp =
		{
			issueNewEnemyID(),
			enemyNS::TIGER,
			stateMachineNS::PATROL,
			*player->getPosition(),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			0
		};
		EnemyData* p = createEnemyData(tmp);
		createEnemy(p);
	}

	if (createDebugEnemy)
	{
		if (getEnemyList().empty() == false)
		{
			Enemy* debugEnemy = getEnemyList().back();
			debugEnemy->debugEnemyID = debugEnemy->getEnemyID();
			camera->setTarget(debugEnemy->getPosition());
			camera->setTargetX(&debugEnemy->getAxisX()->direction);
			camera->setTargetY(&debugEnemy->getAxisY()->direction);
			camera->setTargetZ(&debugEnemy->getAxisZ()->direction);
			camera->setRelative(D3DXQUATERNION(0.0f, 6.0f, -4.5f, 0.0f));
			debugEnemy->setDebugEnvironment();
			debugEnemy->setCamera(&camera[0]);
			debugEnemy->setDebugEnvironment();
		}

		
	}

	if (returnPlayer)
	{
		camera->setTarget(player->getPosition());
		camera->setTargetX(&player->getAxisX()->direction);
		camera->setTargetY(&player->getAxisY()->direction);
		camera->setTargetZ(&player->getAxisZ()->direction);
		//camera->setRelative(D3DXQUATERNION(0.0f, 0.0f, -1.5f, 0.0f));
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
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()�œ��I�Ȋm�ۂ������ςނ悤�������𑝂₵�Ă���

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
// Getter
//=============================================================================
LinkedList<enemyNS::EnemyData>* EnemyManager::getEnemyDataList() { return &enemyDataList; }
std::vector<Enemy*>& EnemyManager::getEnemyList() { return enemyList; }
int EnemyManager::getNextID() { return nextID; }

