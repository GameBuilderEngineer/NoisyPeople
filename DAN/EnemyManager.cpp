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
	wolfRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	tigerRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_BUNNY));
	bearRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_HAT));

#if 1	// �G�l�~�[�c�[���̃f�[�^��ǂݍ���
	ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		createEnemyData(enemyTools->GetEnemySet(i));
	}
	SAFE_DELETE(enemyTools);
#endif

#if 1	// �G�l�~�[�I�u�W�F�N�g���c�[���f�[�^�����ɍ쐬����
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
			destroyEnemy(enemyList[i]->getEnemyData()->id);
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
	enemyData.id = enemySetting.id;
	enemyData.type = enemySetting.type;
	enemyData.defaultState = enemySetting.defaultState;
	enemyData.defaultPosition = enemySetting.defaultPosition;
	enemyData.defaultDirection = enemySetting.defaultDirection;
	enemyData.setUp();
	// ���X�g�ɒǉ�
	enemyDataList.insertFront(enemyData);
	enemyDataList.listUpdate();
	// �G�l�~�[�f�[�^ID�̍ő�l���X�V�i�Ō�ɒǉ������f�[�^ + 1�����񔭍sID�ƂȂ�j
	nextID = enemyData.id + 1;
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
		enemy = new Wolf(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		wolfRenderer->generateObject(enemy);
		break;

	case TIGER:
		enemy = new Tiger(staticMeshNS::reference(staticMeshNS::SAMPLE_BUNNY), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		tigerRenderer->generateObject(enemy);
		break;

	case BEAR:
		enemy = new Bear(staticMeshNS::reference(staticMeshNS::SAMPLE_HAT), enemyData);
		enemy->setAttractor(attractorMesh, attractorMatrix);
		enemyList.emplace_back(enemy);
		bearRenderer->generateObject(enemy);
		break;
	}
}


//=============================================================================
// �G�l�~�[�f�[�^�̔j��
//=============================================================================
void EnemyManager::destroyEnemyData(int _id)
{
	for (int i = 0; i < enemyDataList.nodeNum; i++)
	{
		if (enemyDataList.getValue(i)->id != _id) { continue; }

		// �j������G�l�~�[�f�[�^��ID�ɖ����l����������ł����i��assertDestructionOrder()�j
		enemyDataList.getValue(i)->id = -1;

		// �G�l�~�[�f�[�^�\���̂̓m�[�h���Ƀ�����������Ă��邽��
		// �m�[�h�̔j���ƍ��킹�ăG�l�~�[�f�[�^���j������Ă���
		enemyDataList.remove(enemyDataList.getNode(i));
		break;
	}

}


//=============================================================================
// �S�ẴG�l�~�[�f�[�^��j��
//=============================================================================
void EnemyManager::destroyAllEnemyData()
{
	// �G�l�~�[�f�[�^�\���̂̓m�[�h���Ƀ�����������Ă��邽��
	// �m�[�h�̔j���ƍ��킹�ăG�l�~�[�f�[�^���j������Ă���
	enemyDataList.allClear();
}


//=============================================================================
// �G�l�~�[�I�u�W�F�N�g��j��
//=============================================================================
void EnemyManager::destroyEnemy(int _id)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->id != _id) { continue; }

		switch (enemyList[i]->getEnemyData()->type)
		{
		case WOLF:
			wolfRenderer->deleteObjectByID(enemyList[i]->id);
			break;

		case TIGER:
			tigerRenderer->deleteObjectByID(enemyList[i]->id);
			break;

		case BEAR:
			bearRenderer->deleteObjectByID(enemyList[i]->id);
			break;
		}
		enemyList.erase(enemyList.begin() + i);	// �x�N�^�[�v�f������
		break;
	}

#ifdef _DEBUG
	assertDestructionOrder();
#endif//_DEBUG
}


//=============================================================================
// �S�ẴG�l�~�[�I�u�W�F�N�g��j��
//=============================================================================
void EnemyManager::destroyAllEnemy()
{
	wolfRenderer->allDelete();
	tigerRenderer->allDelete();
	bearRenderer->allDelete();
	enemyList.clear();	// �x�N�^�[�v�f����0�ɂ���

#ifdef _DEBUG
	assertDestructionOrder();
#endif//_DEBUG
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
			// �G�l�~�[�f�[�^�P�̂�j������ۂ�ID�ɖ����l���������܂�Ă��邽��
			// ����ς݂̃������Ƀf�[�^���c���Ă����Ƃ��Ă����Lif�������͋U�ƂȂ�
			if (enemyDataList.getValue(i)->id == enemyList[k]->getEnemyData()->id)
			{
				cnt++;
			}
		}
	}
	assert(cnt == enemyList.size());
	assert(enemyDataList.nodeNum >= enemyList.size());
}


//=============================================================================
// �G�l�~�[ID�𔭍s����
//=============================================================================
int EnemyManager::issueNewID()
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
	if (ImGui::CollapsingHeader("EnemyInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("enemyDataList.nodeNum:%d\n", enemyDataList.nodeNum);
		ImGui::Text("enemyList.size()     :%d\n", enemyList.size());
		ImGui::Text("numOfEnemy           :%d\n", Enemy::getNumOfEnemy());
		ImGui::Text("nextID				�@:%d\n", nextID);
		//for (int i = 0; i < enemyList.size(); i++)
		//{
		//	ImGui::Text("%d", enemyList[i]->getEnemyData()->id);
		//}
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
