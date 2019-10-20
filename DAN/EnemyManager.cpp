//-----------------------------------------------------------------------------
// �G�l�~�[�Ǘ��N���X [Enemy.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "EnemyManager.h"
#include "ImguiManager.h"
using namespace enemyNS;


//=============================================================================
// ������
//=============================================================================
void EnemyManager::initialize()
{
#if 0
	// �G�l�~�[�t�@�C������G�l�~�[�����X�e�[�^�X��ǂݍ���
	FILE	*fp = NULL;
	fopen("enemy.enemy", "rb");
	ENEMY_FILE enemyFile;
	memset(&enemyFile, 0, sizeof(ENEMY_FILE));
	fread(&enemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);
	enemyFile.efmt = new ENEMY_EFMT[enemyFile.enmy.enemyMax];
	fread(&enemyFile.efmt, sizeof(ENEMY_EFMT), enemyFile.enmy.enemyMax, fp);
	fclose(fp);

	// �G�l�~�[�f�[�^���X�g�̒ǉ�
	EnemyData enemyData;
	enemyDataList.reserve(enemyFile.enmy.enemyMax + DYNAMIC_SPAWN_MAX);
	for (int i = 0; i < enemyDataList.size(); i++)
	{
		enemyData.zeroClear();
		if (i >= enemyFile.enmy.enemyMax) { continue; }

		//enemyFile.efmt[i].chunkId;
		//enemyFile.efmt[i].posX;

		enemyDataList.push_back(enemyData);
	}
	SAFE_DELETE_ARRAY(enemyFile.efmt);
	nextID = enemyDataList.back().id + 1;	// �G�l�~�[�f�[�^ID�̍ő�l���X�V
#endif

	// update()�œ��I�Ȋm�ۂ������ςނ悤�G�l�~�[���X�g�̃L���p�V�e�B�i�������j�𑝂₵�Ă���
	enemyList.reserve(ENEMY_OBJECT_MAX);

	// �G�l�~�[�I�u�W�F�N�g�̍쐬
	for (size_t i = 0; enemyDataList.size(); i++)
	{
		if (/* �v���C���[�̏����ʒu�Ƌ߂���� */0)
		{
			createEnemy(&enemyDataList[i]);
		}
	}

	// �`��I�u�W�F�N�g�̍쐬
	wolfRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	tigerRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	bearRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
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
			SAFE_DELETE(enemyList[i]);
			enemyList.erase(enemyList.begin() + i);
			break;
		}
	}
	// ����StaticMeshObject�ɓo�^���ꂽ�I�u�W�F�N�g���ʂɔj���ł��Ȃ�
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

	enemyList.clear();
	SAFE_DELETE(wolfRenderer);
	SAFE_DELETE(tigerRenderer);
	SAFE_DELETE(bearRenderer);
	wolfRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	tigerRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
	bearRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_REDBULL));
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