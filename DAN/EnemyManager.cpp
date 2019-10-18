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
	// �t�@�C������G�l�~�[�����X�e�[�^�X��ǂݍ���
	FILE	*fp = NULL;
	fopen("enemy.enemy", "rb");

	ENEMY_FILE tmpEnemyFile;
	memset(&tmpEnemyFile, 0, sizeof(ENEMY_FILE));

	//ENMY�̓ǂݍ���
	fread(&tmpEnemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);

	//�������m��
	tmpEnemyFile.efmt = new ENEMY_EFMT[tmpEnemyFile.enmy.enemyMax];

	//EFMT�̓ǂݍ���
	fread(&tmpEnemyFile.efmt, sizeof(ENEMY_EFMT), tmpEnemyFile.enmy.enemyMax, fp);

	fclose(fp);

	// �G�l�~�[�f�[�^���X�g�̒ǉ�
	EnemyData enemyData;
	enemyDataList.reserve(tmpEnemyFile.enmy.enemyMax);
	for (int i = 0; i < tmpEnemyFile.enmy.enemyMax; i++)
	{
		enemyData.clear();

		//enemyData.id = tmpEnemyFile.efmt->chunkId;

		enemyDataList.push_back(enemyData);
	}
	
	// �G�l�~�[�I�u�W�F�N�g�̍쐬
	enemyList.reserve(INITIAL_RESERVE);
	for (size_t i = 0; enemyDataList.size(); i++)
	{
		if (/* �v���C���[�̏����ʒu�Ƌ߂���� */1)
		{
			createEnemy(&enemyDataList[i]);
		}
	}

	SAFE_DELETE_ARRAY(tmpEnemyFile.efmt);
}


//=============================================================================
// �I������
//=============================================================================
void EnemyManager::uninitialize()
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		SAFE_DELETE(enemyList[i]);
	}

	std::vector<Enemy*> temp;
	enemyList.swap(temp);
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
	//for (size_t i = 0; i < enemyList.size(); i++)
	//{
	//	enemyList[i]->render(view, projection, cameraPosition);
	//}
}


//=============================================================================
// �G�l�~�[�I�u�W�F�N�g�̍쐬
//=============================================================================
void EnemyManager::createEnemy(EnemyData* enemyData)
{
	switch (enemyData->type)
	{
	case WOLF:
		break;
	case TIGER:
		break;
	case BEAR:
		break;

	default:
		//enemyList.emplace_back(new Enemy);
		break;
	}

	enemyList.back()->setDataToEnemy(enemyData);
}


//=============================================================================
// �G�l�~�[�I�u�W�F�N�g�̔j��
//=============================================================================
void EnemyManager::destroyEnemy(int _id)
{
	for (size_t i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->getEnemyData()->id == _id)
		{
			delete enemyList[i];
			enemyList.erase(enemyList.begin() + i);
		}
	}
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