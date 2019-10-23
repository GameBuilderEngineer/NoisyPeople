//-----------------------------------------------------------------------------
// �G�l�~�[�Ǘ��N���X [Enemy.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "EnemyManager.h"
#include "ImguiManager.h"
#include "EnemyTools.h"
using namespace enemyNS;


//=============================================================================
// ������
//=============================================================================
void EnemyManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;

	// �ڒn�t�B�[���h�����Z�b�g
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// �`��I�u�W�F�N�g�̍쐬
	wolfRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	tigerRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	bearRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));

#if 0	// �G�l�~�[�c�[���̃f�[�^��ǂݍ���
	ENEMY_TOOLS* enemyTools = new ENEMY_TOOLS;
	enemyDataList.reserve(enemyTools->GetEnemyMax());
	EnemyData enemyData;
	for (int i = 0; i < enemyTools->GetEnemyMax(); i++)
	{
		enemyData.id = enemyTools->GetEnemySet(i).id;
		enemyData.type = enemyTools->GetEnemySet(i).type;
		enemyData.defaultState = enemyTools->GetEnemySet(i).defaultState;
		enemyData.defaultPosition = enemyTools->GetEnemySet(i).defaultPosition;
		enemyData.defaultDirection = enemyTools->GetEnemySet(i).defaultDirection;
		enemyData.setUp();
		enemyDataList.push_back(enemyData);
	}
	SAFE_DELETE(enemyTools);
	nextID = enemyData.id + 1;	// �G�l�~�[�f�[�^ID�̍ő�l���X�V
#endif

#if 0	// �G�l�~�[�I�u�W�F�N�g���f�[�^�����ɍ쐬����
	enemyList.reserve(ENEMY_OBJECT_MAX);	// update()�œ��I�Ȋm�ۂ������ςނ悤�������𑝂₵�Ă���
	for (size_t i = 0; i < enemyDataList.size(); i++)
	{
		if (1/* �{���̓v���C���[�̏����ʒu�Ƌ߂���΁`�ȂǏ������t�� */)
		{
			createEnemy(&enemyDataList[i]);
		}
	}
#endif
}


//=============================================================================
// �I������
//=============================================================================
void EnemyManager::uninitialize()
{
	// �S�G�l�~�[�I�u�W�F�N�g��j��
	destroyAllEnemy();

	// �x�N�^�[�̊m�ۃ��������������i�������A���P�[�^�����ɖ߂��j
	std::vector<Enemy*> temp;
	enemyList.swap(temp);


	// �`��I�u�W�F�N�g�̔j��
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
// �G�l�~�[�I�u�W�F�N�g�̍쐬
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
		wolfRenderer->registerObject(enemy);
		break;

	case TIGER:
		break;

	case BEAR:
		break;

	default:
		break;
	}
}


//=============================================================================
// �G�l�~�[�I�u�W�F�N�g�̔j��
// ���g�p�s��
//=============================================================================
void EnemyManager::destroyEnemy(int _id)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->id == _id)
		{
			switch (enemyList[i]->getEnemyData()->type)
			{
			case WOLF:
				wolfRenderer->unRegisterObjectByID(enemyList[i]->id);
				break;

			case TIGER:
				//tigerRenderer->
				break;

			case BEAR:
				break;

			default:
				break;

			}
			SAFE_DELETE(enemyList[i]);

			enemyList.erase(enemyList.begin() + i);
			break;
		}
	}
	// ����StaticMeshRenderer�ɓo�^���ꂽ�I�u�W�F�N�g���ʂɔj���ł��Ȃ�
	
}


//=============================================================================
// �S�G�l�~�[�I�u�W�F�N�g�̔j��
// ��������
//=============================================================================
void EnemyManager::destroyAllEnemy()
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		SAFE_DELETE(enemyList[i]);
	}

	wolfRenderer->allUnRegister();
	tigerRenderer->allUnRegister();
	bearRenderer->allUnRegister();

	enemyList.clear();
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
		//float limitTop = 1000;
		//float limitBottom = -1000;

		ImGui::Text("numOfEnemy:%d", Enemy::getNumOfEnemy());

		//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//�ʒu
		//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
		//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//�X�P�[��
		//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//���a
		//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//���ߒl
		//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//���x
		//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//�����x
		//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//�d��

		//ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
		//ImGui::Checkbox("onActive", &onActive);										//�A�N�e�B�u���t���O
		//ImGui::Checkbox("onRender", &onRender);										//�`��L�����t���O
		//ImGui::Checkbox("onLighting", &onLighting);									//���������t���O
		//ImGui::Checkbox("onTransparent", &onTransparent);								//���߃t���O
		//ImGui::Checkbox("operationAlpha", &operationAlpha);							//���ߒl�̑���L���t���O

		//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//���ߒl�̑���L���t���O
	}
#endif
}