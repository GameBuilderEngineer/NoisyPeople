//===================================================================================================================================
//�yEnemyTools.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#include "EnemyTools.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
ENEMY_TOOLS::ENEMY_TOOLS()
{
#ifdef _DEBUG
	//�G�l�~�[�t�@�C���\���̂̏�����
	enemyFile = { 0 };
	enemyFile.efmt = nullptr;

	//�p�X
	setToolsDirectory();

	//�t�@�C��
	FILE	*fp = NULL;
	fp = fopen(ENEMY_FILE_PATH, "rb");
	if (fp == NULL)	//�t�@�C����������܂���
	{
		//�G�l�~�[�t�@�C���̐V�K�쐬
		CreatNewEnemyFile();
	}
	else
	{
		//ENMY�̓ǂݍ���
		fread(&enemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);

		//�������m�ۂ�EFMT�̓ǂݍ���
		if (enemyFile.enmy.enemyMax != NULL)
		{
			//�������m��
			enemyFile.efmt = new ENEMY_EFMT[enemyFile.enmy.enemyMax];

			for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
			{
				//EFMT�̓ǂݍ���
				fread(&enemyFile.efmt[i], sizeof(ENEMY_EFMT), 1, fp);
			}
		}


		fclose(fp);

		//�G�l�~�[���
		EnemyListboxCurrent = 0;
		EnemyListboxType = enemyNS::ENEMY_TYPE::WOLF;
		EnemyListboxState = enemyNS::ENEMY_STATE::CHASE;

		//�`��p
		renderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
		initialize();
		for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		{
			generate(D3DXVECTOR3(enemyFile.efmt[i].posX, enemyFile.efmt[i].posY, enemyFile.efmt[i].posZ));
			renderer->updateAccessList();
		}
	}
#endif
}

//===================================================================================================================================
//�y�G�l�~�[�t�@�C���̐V�K�쐬�z
//===================================================================================================================================
void ENEMY_TOOLS::CreatNewEnemyFile(void)
{
	//�t�@�C��
	FILE	 *fp = NULL;
	fp = fopen(ENEMY_FILE_PATH, "wb");

	//�G�l�~�[�t�@�C���\����
	ENEMY_FILE tmpEnemyFile;
	
	//�`�����N
	memcpy(tmpEnemyFile.enmy.chunkId, ENMY_CHUNK, sizeof(tmpEnemyFile.enmy.chunkId));

	//�G�l�~�[�̏�����
	tmpEnemyFile.enmy.enemyMax = 0;

	//ENMY�̃T�C�Y
	tmpEnemyFile.enmy.size = sizeof(ENEMY_ENMY);

	//�����o��
	fwrite(&tmpEnemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);

	fclose(fp);
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
ENEMY_TOOLS::~ENEMY_TOOLS()
{
#ifdef _DEBUG
	//�t�@�C���̏����o��
	OutputEnemyFile();

	if (enemyFile.enmy.enemyMax != NULL)
	{
		SAFE_DELETE_ARRAY(enemyFile.efmt);
	}

	//�`��p
	SAFE_DELETE(renderer);

#endif
}

//===================================================================================================================================
//�y�������z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::initialize()
{
	needUpdate = true;
};

//===================================================================================================================================
//�y�X�V�z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::update()
{
	if (!needUpdate)return;
	renderer->updateBuffer();
	renderer->updateArray();
	renderer->update();
	needUpdate = false;
}

//===================================================================================================================================
//�y�`��z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
}

//===================================================================================================================================
//�y�����z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::generate(D3DXVECTOR3 position)
{
	Object* object = new Object();
	renderer->registerObject(object);
	object->existenceTimer = -1;		// < 0 �Ȃ������
	object->initialize(&position);
}

//===================================================================================================================================
//�y�G�l�~�[�̎�ނ�ݒ�z
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemyType(short enemyId, short enemyType)
{
	enemyFile.efmt[enemyId].enemyType = enemyType;
}

//===================================================================================================================================
//�y�G�l�~�[�̏�Ԃ�ݒ�z
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemyState(short enemyId, short enemyState)
{
	enemyFile.efmt[enemyId].enemyState = enemyState;
}

//===================================================================================================================================
//�y�G�l�~�[�̈ʒu��ݒ�z
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemyPos(short enemyId, const D3DXVECTOR3 pos)
{
	enemyFile.efmt[enemyId].posX = pos.x;
	enemyFile.efmt[enemyId].posY = pos.y;
	enemyFile.efmt[enemyId].posZ = pos.z;
}

//===================================================================================================================================
//�y�G�l�~�[�̉�]Y����ݒ�z
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemyDir(short enemyId, const D3DXVECTOR3 dir)
{
	enemyFile.efmt[enemyId].dirX = dir.x;
	enemyFile.efmt[enemyId].dirY = dir.y;
	enemyFile.efmt[enemyId].dirZ = dir.z;
}

//===================================================================================================================================
//�y�G�l�~�[�̐ݒu�z
//===================================================================================================================================
void ENEMY_TOOLS::SetEnemy(short enemyId, short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�`�����N
	memcpy(enemyFile.efmt[enemyId].chunkId, EFMT_CHUNK, sizeof(enemyFile.efmt[enemyId].chunkId));

	//�G�l�~�[���
	SetEnemyType(enemyId, enemyType);
	SetEnemyState(enemyId, enemyState);
	SetEnemyPos(enemyId, pos);
	SetEnemyDir(enemyId, dir);

	//�T�C�Y
	enemyFile.efmt[enemyId].size = (short)sizeof(ENEMY_EFMT);
}

//===================================================================================================================================
//�y�G�l�~�[�t�@�C���̏����o�������z
//===================================================================================================================================
void ENEMY_TOOLS::OutputEnemyFile(void)
{
	//�p�X
	setToolsDirectory();

	FILE	*fp = NULL;					//�t�@�C��
	fp = fopen(ENEMY_FILE_PATH, "wb");

	//ENMY�̏����o��
	fwrite(&enemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);

	//EFMT�̏����o��
	for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
	{
		//EFMT�̏����o��
		fwrite(&enemyFile.efmt[i], sizeof(ENEMY_EFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//�y�G�l�~�[�̃t�H�[�}�b�g�\���̂�ǉ��z
//===================================================================================================================================
void ENEMY_TOOLS::AddEnemyFormat(short enemyType, short enemyState, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�G�l�~�[�̐�+1
	enemyFile.enmy.enemyMax++;

	//�G�l�~�[�̃t�H�[�}�b�g�\���̂𐮗�
	UpdateEfmt(enemyFile.enmy.enemyMax - 1);

	//�G�l�~�[�̃t�H�[�}�b�g�\���̂̍Ō�ɒǉ�
	SetEnemy(enemyFile.enmy.enemyMax - 1, enemyType, enemyState, pos, dir);

	//�`��p
	generate(pos);
	renderer->updateAccessList();
	needUpdate = true;
}

//===================================================================================================================================
//�y�G�l�~�[�̍ő吔���擾�z
//===================================================================================================================================
int ENEMY_TOOLS::GetEnemyMax(void)
{
	return enemyFile.enmy.enemyMax;
}

//===================================================================================================================================
//�y�G�l�~�[�̃t�H�[�}�b�g�\���̂𐮗��z
//===================================================================================================================================
void ENEMY_TOOLS::UpdateEfmt(int oldEnemyMax)
{
	//�o�b�N�A�b�v�p
	ENEMY_EFMT *backUpEmft = new ENEMY_EFMT[enemyFile.enmy.enemyMax];

	//����(enemyType��ENEMY_TYPE::TYPE_MAX�Ȃ���)
	int newEnemyMax = 0;	//���g������efmt
	for (int i = 0; i < oldEnemyMax; i++)
	{
		if (enemyFile.efmt[i].enemyType == enemyNS::ENEMY_TYPE::TYPE_MAX)
		{
			//�X�L�b�v
			continue;
		}
		else
		{
			//�o�b�N�A�b�v�����
			backUpEmft[newEnemyMax] = enemyFile.efmt[i];
			newEnemyMax++;
		}
	}

	//������蒼���K�v�������
	if (oldEnemyMax > 0)
	{
		//���������
		SAFE_DELETE_ARRAY(enemyFile.efmt);
	}

	//�������m�ۂ�����
	if (enemyFile.enmy.enemyMax > 0)
	{
		enemyFile.efmt = new ENEMY_EFMT[enemyFile.enmy.enemyMax];

		//���[�h�o�b�N�A�b�v�f�[�^
		for (int i = 0; i < newEnemyMax; i++)
		{
			enemyFile.efmt[i] = backUpEmft[i];
			enemyFile.efmt[i].enemyId = i;	//Id�̍X�V
		}

		//ENMY�̏�������
		enemyFile.enmy.size = (short)(sizeof(ENEMY_ENMY) + (sizeof(ENEMY_EFMT) * enemyFile.enmy.enemyMax));
	}
	//���������
	SAFE_DELETE_ARRAY(backUpEmft);
}

//===================================================================================================================================
//�y�G�l�~�[�Z�b�g�\���̂��擾�z
// �g����:
// ENEMYSET *enemySet = new (ENEMYSET[GetEnemyMax()]);
// for(int i=0;i<GetEnemyMax();i++)
// {  
//   enemySet[i]=GetEnemySet(i);
// }
// ...
// SAFE_DELETE_ARRAY(enemySet);
//===================================================================================================================================
enemyNS::ENEMYSET ENEMY_TOOLS::GetEnemySet(short enemyId)
{
	enemyNS::ENEMYSET tmpEnemySet	= { 0 };
	tmpEnemySet.id					= enemyFile.efmt[enemyId].enemyId;
	tmpEnemySet.type				= enemyFile.efmt[enemyId].enemyType;
	tmpEnemySet.defaultState		= enemyFile.efmt[enemyId].enemyState;
	tmpEnemySet.defaultPosition		= D3DXVECTOR3(enemyFile.efmt[enemyId].posX, enemyFile.efmt[enemyId].posY, enemyFile.efmt[enemyId].posZ);
	tmpEnemySet.defaultDirection	= D3DXVECTOR3(enemyFile.efmt[enemyId].dirX, enemyFile.efmt[enemyId].dirY, enemyFile.efmt[enemyId].dirZ);
	
	return tmpEnemySet;
}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void ENEMY_TOOLS::outputEnemyToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	//�t���O
	bool creatFlag = false;
	bool deleteFlag = false;
	
	ImGui::Begin("Enemy Tools");

	//�@�\
	ImGui::Checkbox("New Enemy", &creatFlag);
	ImGui::Checkbox("Delete", &deleteFlag);

	//�G�l�~�[�̎��
	const char* listboxEnemyType[] = { "WOLF", "TIGER", "BEAR" };
	ImGui::ListBox("Enemy Type", &EnemyListboxType, listboxEnemyType, enemyNS::ENEMY_TYPE::TYPE_MAX);

	//�G�l�~�[�̏��
	const char* listboxEnemyState[] = { "CHASE", "PATROL", "REST","DIE","DEAD" };
	ImGui::ListBox("Enemy State", &EnemyListboxState, listboxEnemyState, enemyNS::ENEMY_STATE::STATE_MAX);

	//�G�l�~�[�̏��
	if (enemyFile.enmy.enemyMax != NULL)
	{
		//�������m��
		char **ListboxIndexName = new char*[enemyFile.enmy.enemyMax];
		for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		{
			ListboxIndexName[i] = new char[CHAR_MAX];
			sprintf(ListboxIndexName[i], "%d", i);
		}

		ImGui::ListBox("Enemy ", &EnemyListboxCurrent, ListboxIndexName, enemyFile.enmy.enemyMax);
		ImGui::Text("Enemy pos:%f %f %f", enemyFile.efmt[EnemyListboxCurrent].posX,
			enemyFile.efmt[EnemyListboxCurrent].posY,
			enemyFile.efmt[EnemyListboxCurrent].posZ);
		ImGui::Text("Enemy dir:%f %f %f", enemyFile.efmt[EnemyListboxCurrent].dirX,
			enemyFile.efmt[EnemyListboxCurrent].dirY,
			enemyFile.efmt[EnemyListboxCurrent].dirZ);
		ImGui::Text("Enemy Type:%s  Enemy State:%s",
			listboxEnemyType[enemyFile.efmt[EnemyListboxCurrent].enemyType],
			listboxEnemyState[enemyFile.efmt[EnemyListboxCurrent].enemyState]);

		//���������
		for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		{
			delete[] ListboxIndexName[i];
		}
		delete[] ListboxIndexName;
	}

	ImGui::End();

	//�I��


	//�폜
	if (deleteFlag)
	{
		//�폜
		DeleteEnemyFormat(EnemyListboxCurrent);
	
		//�߂�
		EnemyListboxCurrent--;
		if (EnemyListboxCurrent < 0)
		{
			EnemyListboxCurrent = 0;
		}
	}

	//�V�K�쐬
	if (creatFlag)
	{
		//�쐬
		AddEnemyFormat(EnemyListboxType, EnemyListboxState, pos, dir);

		//�i��
		EnemyListboxCurrent = enemyFile.enmy.enemyMax - 1;
	}

#endif
}
//
////===================================================================================================================================
////�y�G�l�~�[�c�[���̑I�𒆂�\�������z
////===================================================================================================================================
//void ENEMY_TOOLS::SpinEnemy(short enemyId)
//{
//	//�����p(MAX�Ȃ����)
//	for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
//	{
//		renderer->
//	}
//}

//===================================================================================================================================
//�y�G�l�~�[�̃t�H�[�}�b�g�\���̂������z
//===================================================================================================================================
void ENEMY_TOOLS::DeleteEnemyFormat(short enemyId)
{
	//�����p(MAX�Ȃ����)
	if (enemyFile.enmy.enemyMax != NULL)
	{
		//�������
		enemyFile.enmy.enemyMax--;
		if (enemyFile.enmy.enemyMax < 0)
		{
			enemyFile.enmy.enemyMax = 0;
		}

		//TYPE_MAX�Ȃ����
		enemyFile.efmt[enemyId].enemyType = enemyNS::ENEMY_TYPE::TYPE_MAX;

		//�G�l�~�[�̃t�H�[�}�b�g�\���̂𐮗�
		UpdateEfmt(enemyFile.enmy.enemyMax + 1);

		//�`��p
		renderer->unRegisterObject(enemyFile.enmy.enemyMax - enemyId);
		renderer->updateAccessList();
		needUpdate = true;
	}
}
