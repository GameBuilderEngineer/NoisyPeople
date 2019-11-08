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
	//�G�l�~�[�t�@�C���\���̂̏�����
	enemyFile = { 0 };
	enemyFile.efmt = nullptr;

	//�p�X
	setToolsDirectory();

#ifdef _DEBUG
	//�����_���[�̏�����
	initRender();
#endif

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

		//�t�@�C��
		fclose(fp);
#ifdef _DEBUG
		//�I�u�W�F�N�g�̏�����
		initObject();
#endif
	}

#ifdef _DEBUG
	//�G�l�~�[���
	EnemyListboxCurrent = 0;
	EnemyListboxType = enemyNS::ENEMY_TYPE::WOLF;
	EnemyListboxState = enemyNS::ENEMY_STATE::CHASE;
#endif
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
ENEMY_TOOLS::~ENEMY_TOOLS()
{
	//�t�@�C���̏����o��
	OutputEnemyFile();

	//�t�H�[�}�b�g���Ȃ���
	if (enemyFile.enmy.enemyMax != NULL)
	{
		SAFE_DELETE_ARRAY(enemyFile.efmt);
#ifdef _DEBUG
		for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		{
			SAFE_DELETE(object[i]);
		}
		SAFE_DELETE_ARRAY(object);
		SAFE_DELETE_ARRAY(bodyCollide);
#endif
	}

#ifdef _DEBUG
	//�����_���[
	for (int i = 0; i < enemyNS::ENEMY_TYPE::TYPE_MAX; i++)
	{
		SAFE_DELETE(renderer[i]);
	}
	SAFE_DELETE_ARRAY(renderer);
#endif
}

//===================================================================================================================================
//�y�G�l�~�[�̐����擾�z
//===================================================================================================================================
int ENEMY_TOOLS::GetEnemyMax(void)
{
	return enemyFile.enmy.enemyMax;
}

//===================================================================================================================================
//�y�G�l�~�[�Z�b�g�\���̂��擾�z
//===================================================================================================================================
enemyNS::ENEMYSET ENEMY_TOOLS::GetEnemySet(short enemyId)
{
	enemyNS::ENEMYSET tmpEnemySet = { 0 };
	tmpEnemySet.enemyID = enemyFile.efmt[enemyId].enemyId;
	tmpEnemySet.type = enemyFile.efmt[enemyId].enemyType;
	tmpEnemySet.defaultState = enemyFile.efmt[enemyId].enemyState;
	tmpEnemySet.defaultPosition = D3DXVECTOR3(enemyFile.efmt[enemyId].posX, enemyFile.efmt[enemyId].posY, enemyFile.efmt[enemyId].posZ);
	tmpEnemySet.defaultDirection = D3DXVECTOR3(enemyFile.efmt[enemyId].dirX, enemyFile.efmt[enemyId].dirY, enemyFile.efmt[enemyId].dirZ);

	return tmpEnemySet;
}

#ifdef _DEBUG
//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void ENEMY_TOOLS::outputEnemyToolsGUI(int GUIid, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	if (GUIid == ENEMY_GUI_ID)
	{
		//�t���O
		bool creatFlag = false;
		bool deleteFlag = false;

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
			ImGui::Text("Enemy ID:%d", enemyFile.efmt[EnemyListboxCurrent].enemyId);
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

		//�폜
		if (deleteFlag)
		{
			//�폜
			DeleteEnemyFormat(EnemyListboxCurrent);
		}

		//�V�K�쐬
		if (creatFlag)
		{
			//�쐬
			AddEnemyFormat(EnemyListboxType, EnemyListboxState, pos, dir);
		}
	}
}

//===================================================================================================================================
//�y�����_���[�̏������z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::initRender()
{
	//�����_�[�̏�����
	renderer = new StaticMeshRenderer*[enemyNS::ENEMY_TYPE::TYPE_MAX];
	for (int i = 0; i < enemyNS::ENEMY_TYPE::TYPE_MAX; i++)
	{
		renderer[i] = new StaticMeshRenderer(staticMeshNS::reference(GetStaticMeshID(i)));
	}

	//�X�V�t���O
	needUpdate = true;
};

//===================================================================================================================================
//�y�I�u�W�F�N�g�̏������z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::initObject()
{
	//�I�u�W�F�N�g
	object = new Object*[enemyFile.enmy.enemyMax];
	bodyCollide = new BoundingSphere[enemyFile.enmy.enemyMax];
	for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
	{
		//�I�u�W�F�N�g�̐���
		object[i] = new Object();
		generate(object[i], enemyFile.efmt[i].enemyType, D3DXVECTOR3(enemyFile.efmt[i].posX, enemyFile.efmt[i].posY, enemyFile.efmt[i].posZ),
			D3DXVECTOR3(enemyFile.efmt[i].dirX, enemyFile.efmt[i].dirY, enemyFile.efmt[i].dirZ));
		renderer[enemyFile.efmt[i].enemyType]->updateAccessList();

		// �R���C�_�̏�����
		bodyCollide->initialize(&D3DXVECTOR3(enemyFile.efmt[i].posX, enemyFile.efmt[i].posY, enemyFile.efmt[i].posZ),
			staticMeshNS::reference(GetStaticMeshID(enemyFile.efmt[i].enemyType))->mesh);	
	}
}

//===================================================================================================================================
//�y�X�V�z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::update()
{
	if (!needUpdate)return;
	for (int i = 0; i < enemyNS::ENEMY_TYPE::TYPE_MAX; i++)
	{
		renderer[i]->updateBuffer();
		renderer[i]->updateArray();
		renderer[i]->update();
	}
	needUpdate = false;
}

//===================================================================================================================================
//�y�`��z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (enemyFile.enmy.enemyMax != NULL)
	{
		for (int i = 0; i < enemyNS::ENEMY_TYPE::TYPE_MAX; i++)
		{
			renderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
	}
}

//===================================================================================================================================
//�y�����z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::generate(Object *object, short enemyType, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 �Ȃ������
	renderer[enemyType]->registerObject(object);
}
#endif

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

	//�t�@�C��
	fclose(fp);
}

#ifdef _DEBUG
//===================================================================================================================================
//�y�����_���[�̃��Z�b�g�����z�`��p
//===================================================================================================================================
void ENEMY_TOOLS::ResetRenderer(void)
{
	//�I������
	for (int i = 0; i < enemyFile.enmy.enemyMax - 1; i++)
	{
		SAFE_DELETE(object[i]);
	}
	SAFE_DELETE_ARRAY(object);

	if (enemyFile.enmy.enemyMax != NULL)
	{
		for (int i = 0; i < enemyNS::ENEMY_TYPE::TYPE_MAX; i++)
		{
			SAFE_DELETE(renderer[i]);
		}
		SAFE_DELETE_ARRAY(renderer);

		//�����_���[�̏�����
		initRender();

		//�I�u�W�F�N�g�̏�����
		initObject();
	}
}

//===================================================================================================================================
//�y�X�^�e�B�b�N���b�V����ID���擾�z�`��p
//===================================================================================================================================
int ENEMY_TOOLS::GetStaticMeshID(short enemyType)
{
	int staticMeshNo = 0;
	switch (enemyType)
	{
	case enemyNS::ENEMY_TYPE::WOLF:
		staticMeshNo = staticMeshNS::ENEMY_01;
		break;
	case enemyNS::ENEMY_TYPE::TIGER:
		staticMeshNo = staticMeshNS::STAR_REGULAR_POLYHEDRON;
		break;
	case enemyNS::ENEMY_TYPE::BEAR:
		staticMeshNo = staticMeshNS::STAR_REGULAR_POLYHEDRON_X10;
		break;
	default:
		break;
	}
	return staticMeshNo;
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
//�y�G�l�~�[�̉�]����ݒ�z
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
	//ID
	enemyFile.efmt[enemyId].enemyId = enemyId;
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
//�y�G�l�~�[�̃t�H�[�}�b�g�\���̂������z
//===================================================================================================================================
void ENEMY_TOOLS::DeleteEnemyFormat(short enemyId)
{
	//�����p(MAX�Ȃ����)
	if (enemyFile.enmy.enemyMax != NULL)
	{
		//�G�l�~�[�̐�-1
		enemyFile.enmy.enemyMax--;
		if (enemyFile.enmy.enemyMax < 0)
		{
			enemyFile.enmy.enemyMax = 0;
		}

		//TYPE_MAX�Ȃ����
		enemyFile.efmt[enemyId].enemyType = enemyNS::ENEMY_TYPE::TYPE_MAX;

		//�G�l�~�[�̃t�H�[�}�b�g�\���̂𐮗�
		UpdateEfmt(enemyFile.enmy.enemyMax + 1);
	}

	//�߂�
	EnemyListboxCurrent--;
	if (EnemyListboxCurrent < 0)
	{
		EnemyListboxCurrent = 0;
	}

	//�t�@�C���̏����o��
	OutputEnemyFile();

	//�����_���[�̃��Z�b�g����
	ResetRenderer();
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

	//�i��
	EnemyListboxCurrent = enemyFile.enmy.enemyMax - 1;

	//�t�@�C���̏����o��
	OutputEnemyFile();

	//�����_���[�̃��Z�b�g����
	ResetRenderer();
}
#endif		