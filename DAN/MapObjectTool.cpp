//===================================================================================================================================
//�yMpojTools.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#include "MapObjectTool.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
MPOJ_TOOLS::MPOJ_TOOLS()
{
	//�}�b�v�I�u�W�F�N�g�t�@�C���\���̂̏�����
	mpojFile = { 0 };
	mpojFile.mfmt = nullptr;

	//�p�X
	setToolsDirectory();

#ifdef _DEBUG
	//�����_���[�̏�����
	initRender();
#endif

	//�t�@�C��
	FILE	*fp = NULL;
	fp = fopen(MPOJ_FILE_PATH, "rb");
	if (fp == NULL)	//�t�@�C����������܂���
	{
		//�}�b�v�I�u�W�F�N�g�t�@�C���̐V�K�쐬
		CreatNewMpojFile();
	}
	else
	{
		//MPOJ�̓ǂݍ���
		fread(&mpojFile.mpoj, sizeof(MPOJ_MPOJ), 1, fp);

		//�������m�ۂ�MFMT�̓ǂݍ���
		if (mpojFile.mpoj.mpojMax != NULL)
		{
			//�������m��
			mpojFile.mfmt = new MPOJ_MFMT[mpojFile.mpoj.mpojMax];

			for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
			{
				//MFMT�̓ǂݍ���
				fread(&mpojFile.mfmt[i], sizeof(MPOJ_MFMT), 1, fp);
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
	//�}�b�v�I�u�W�F�N�g���
	MpojListboxCurrent = 0;
	MpojListboxType = mapObjectNS::MAPOBJECT_TYPE::STONE_01;
	Model[mapObjectNS::MAPOBJECT_TYPE::STONE_01] = staticMeshNS::STONE_003;
	Model[mapObjectNS::MAPOBJECT_TYPE::STONE_02] = staticMeshNS::STONE_003;
	Model[mapObjectNS::MAPOBJECT_TYPE::STONE_03] = staticMeshNS::STONE_003;

	//�����_���[�̃��Z�b�g����
	ResetRenderer();

#endif
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
MPOJ_TOOLS::~MPOJ_TOOLS()
{
	//�t�@�C���̏����o��
	OutputMpojFile();

	if (mpojFile.mpoj.mpojMax != NULL)
	{
		SAFE_DELETE_ARRAY(mpojFile.mfmt);
#ifdef _DEBUG
		for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
		{
			SAFE_DELETE(object[i]);
		}
		SAFE_DELETE_ARRAY(object);
		SAFE_DELETE_ARRAY(bodyCollide);
#endif
	}

#ifdef _DEBUG
	//�����_���[
	for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
	{
		SAFE_DELETE(mapobjRenderer[i]);
	}
	SAFE_DELETE_ARRAY(mapobjRenderer);
#endif
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�̍ő吔���擾�z
//===================================================================================================================================
int MPOJ_TOOLS::GetMpojMax(void)
{
	return mpojFile.mpoj.mpojMax;
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�\���̂��擾�z
//===================================================================================================================================
mapObjectNS::MAPOBJECTSET MPOJ_TOOLS::GetMpojSet(short mpojId)
{
	mapObjectNS::MAPOBJECTSET tmpMpojSet = { 0 };
	tmpMpojSet.mapObjectID = mpojFile.mfmt[mpojId].mpojId;
	tmpMpojSet.type = (mapObjectNS::MAPOBJECT_TYPE)mpojFile.mfmt[mpojId].mpojType;
	tmpMpojSet.defaultPosition = D3DXVECTOR3(mpojFile.mfmt[mpojId].posX, mpojFile.mfmt[mpojId].posY, mpojFile.mfmt[mpojId].posZ);
	tmpMpojSet.defaultDirection = D3DXVECTOR3(mpojFile.mfmt[mpojId].dirX, mpojFile.mfmt[mpojId].dirY, mpojFile.mfmt[mpojId].dirZ);

	return tmpMpojSet;
}

#ifdef _DEBUG
//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void MPOJ_TOOLS::outputMpojToolsGUI(int GUIid, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	if (GUIid == MPOJ_GUI_ID)
	{
		//�t���O
		bool creatFlag = false;
		bool deleteFlag = false;

		//�@�\
		ImGui::Checkbox("New Mpoj", &creatFlag);
		ImGui::Checkbox("Delete", &deleteFlag);

		//�}�b�v�I�u�W�F�N�g�̎��
		const char* listboxMpojType[] = { "STONE_003" ,"STONE_003", "STONE_003"};
		ImGui::ListBox("Mpoj Type", &MpojListboxType, listboxMpojType, mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX);

		//�}�b�v�I�u�W�F�N�g�̏��
		if (mpojFile.mpoj.mpojMax != NULL)
		{
			//�������m��
			char **ListboxIndexName = new char*[mpojFile.mpoj.mpojMax];
			for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
			{
				ListboxIndexName[i] = new char[CHAR_MAX];
				sprintf(ListboxIndexName[i], "%d", i);
			}

			ImGui::ListBox("Mpoj ", &MpojListboxCurrent, ListboxIndexName, mpojFile.mpoj.mpojMax);
			ImGui::Text("ITem ID:%d", mpojFile.mfmt[MpojListboxCurrent].mpojId);
			ImGui::Text("Mpoj pos:%f %f %f", mpojFile.mfmt[MpojListboxCurrent].posX,
				mpojFile.mfmt[MpojListboxCurrent].posY,
				mpojFile.mfmt[MpojListboxCurrent].posZ);
			ImGui::Text("Mpoj dir:%f %f %f", mpojFile.mfmt[MpojListboxCurrent].dirX,
				mpojFile.mfmt[MpojListboxCurrent].dirY,
				mpojFile.mfmt[MpojListboxCurrent].dirZ);
			ImGui::Text("Mpoj Type:%s",
				listboxMpojType[mpojFile.mfmt[MpojListboxCurrent].mpojType]);
			//���������
			for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
			{
				delete[] ListboxIndexName[i];
			}
			delete[] ListboxIndexName;
		}

		//�폜
		if (deleteFlag)
		{
			//�폜
			DeleteMpojFormat(MpojListboxCurrent);
		}

		//�V�K�쐬
		if (creatFlag)
		{
			//�쐬
			AddMpojFormat(MpojListboxType, pos, dir);
		}
	}
#endif
}

//===================================================================================================================================
//�y�����_���[�̏������z�`��p
//===================================================================================================================================
void MPOJ_TOOLS::initRender()
{
	//�����_�[�̏�����
	mapobjRenderer = new StaticMeshRenderer*[mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX];
	for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
	{
		mapobjRenderer[i] = new StaticMeshRenderer(staticMeshNS::reference(Model[i]));
	}

	//�X�V�t���O
	needUpdate = true;
};

//===================================================================================================================================
//�y�I�u�W�F�N�g�̏������z�`��p
//===================================================================================================================================
void MPOJ_TOOLS::initObject()
{
	//�I�u�W�F�N�g
	object = new Object*[mpojFile.mpoj.mpojMax];
	bodyCollide = new BoundingSphere[mpojFile.mpoj.mpojMax];
	for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
	{
		//�I�u�W�F�N�g�̐���
		object[i] = new Object();
		generateMapobj(object[i], mpojFile.mfmt[i].mpojType,
			D3DXVECTOR3(mpojFile.mfmt[i].posX, mpojFile.mfmt[i].posY, mpojFile.mfmt[i].posZ),
			D3DXVECTOR3(mpojFile.mfmt[i].dirX, mpojFile.mfmt[i].dirY, mpojFile.mfmt[i].dirZ));
		mapobjRenderer[mpojFile.mfmt[i].mpojType]->updateAccessList();

		// �R���C�_�̏�����
		bodyCollide->initialize(&D3DXVECTOR3(mpojFile.mfmt[i].posX, mpojFile.mfmt[i].posY, mpojFile.mfmt[i].posZ),
			staticMeshNS::reference(Model[mpojFile.mfmt[i].mpojType])->mesh);
	}
}

//===================================================================================================================================
//�y�X�V�z�`��p
//===================================================================================================================================
void MPOJ_TOOLS::update()
{
	if (!needUpdate)return;
	for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
	{
		mapobjRenderer[i]->updateBuffer();
		mapobjRenderer[i]->updateArray();
		mapobjRenderer[i]->update();
	}

	needUpdate = false;
}

//===================================================================================================================================
//�y�`��z�`��p
//===================================================================================================================================
void MPOJ_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (mpojFile.mpoj.mpojMax != NULL)
	{
		for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
		{
			mapobjRenderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
	}
}

//===================================================================================================================================
//�y�����z�`��p
//===================================================================================================================================
void MPOJ_TOOLS::generateMapobj(Object *object, short mpojType, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 �Ȃ������
	mapobjRenderer[mpojType]->registerObject(object);
}
#endif

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�t�@�C���̏����o�������z
//===================================================================================================================================
void MPOJ_TOOLS::OutputMpojFile(void)
{
	//�p�X
	setToolsDirectory();

	FILE	*fp = NULL;					//�t�@�C��
	fp = fopen(MPOJ_FILE_PATH, "wb");

	//MPOJ�̏����o��
	fwrite(&mpojFile.mpoj, sizeof(MPOJ_MPOJ), 1, fp);

	//MFMT�̏����o��
	for (int i = 0; i < mpojFile.mpoj.mpojMax; i++)
	{
		//MFMT�̏����o��
		fwrite(&mpojFile.mfmt[i], sizeof(MPOJ_MFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�t�@�C���̐V�K�쐬�z
//===================================================================================================================================
void MPOJ_TOOLS::CreatNewMpojFile(void)
{
	//�t�@�C��
	FILE	 *fp = NULL;
	fp = fopen(MPOJ_FILE_PATH, "wb");

	//�}�b�v�I�u�W�F�N�g�t�@�C���\����
	MPOJ_FILE tmpEnemyFile;

	//�`�����N
	memcpy(tmpEnemyFile.mpoj.chunkId, MPOJ_CHUNK, sizeof(tmpEnemyFile.mpoj.chunkId));

	//�}�b�v�I�u�W�F�N�g�̏�����
	tmpEnemyFile.mpoj.mpojMax = 0;

	//ENMY�̃T�C�Y
	tmpEnemyFile.mpoj.size = sizeof(MPOJ_MPOJ);

	//�����o��
	fwrite(&tmpEnemyFile.mpoj, sizeof(MPOJ_MPOJ), 1, fp);

	//�t�@�C��
	fclose(fp);
}

#ifdef _DEBUG
//===================================================================================================================================
//�y�����_���[�̃��Z�b�g�����z�`��p
//===================================================================================================================================
void MPOJ_TOOLS::ResetRenderer(void)
{
	//�I������
	for (int i = 0; i < mpojFile.mpoj.mpojMax - 1; i++)
	{
		SAFE_DELETE(object[i]);
	}
	SAFE_DELETE_ARRAY(object);

	if (mpojFile.mpoj.mpojMax != NULL)
	{
		for (int i = 0; i < mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX; i++)
		{
			SAFE_DELETE(mapobjRenderer[i]);
		}
		SAFE_DELETE_ARRAY(mapobjRenderer);

		//�����_���[�̏�����
		initRender();

		//�I�u�W�F�N�g�̏�����
		initObject();
	}
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�̎�ނ�ݒ�z
//===================================================================================================================================
void MPOJ_TOOLS::SetMpojType(short mpojId, short mpojType)
{
	mpojFile.mfmt[mpojId].mpojType = mpojType;
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�̈ʒu��ݒ�z
//===================================================================================================================================
void MPOJ_TOOLS::SetMpojPos(short mpojId, const D3DXVECTOR3 pos)
{
	mpojFile.mfmt[mpojId].posX = pos.x;
	mpojFile.mfmt[mpojId].posY = pos.y;
	mpojFile.mfmt[mpojId].posZ = pos.z;
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�̉�]����ݒ�z
//===================================================================================================================================
void MPOJ_TOOLS::SetMpojDir(short mpojId, const D3DXVECTOR3 dir)
{
	mpojFile.mfmt[mpojId].dirX = dir.x;
	mpojFile.mfmt[mpojId].dirY = dir.y;
	mpojFile.mfmt[mpojId].dirZ = dir.z;
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�̐ݒu�z
//===================================================================================================================================
void MPOJ_TOOLS::SetMpoj(short mpojId, short mpojType,
	 const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�`�����N
	memcpy(mpojFile.mfmt[mpojId].chunkId, MFMT_CHUNK, sizeof(mpojFile.mfmt[mpojId].chunkId));

	//�}�b�v�I�u�W�F�N�g���
	SetMpojType(mpojId, mpojType);
	SetMpojPos(mpojId, pos);
	SetMpojDir(mpojId, dir);

	//�T�C�Y
	mpojFile.mfmt[mpojId].size = (short)sizeof(MPOJ_MFMT);
	//ID
	mpojFile.mfmt[mpojId].mpojId = mpojId;
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�̃t�H�[�}�b�g�\���̂𐮗��z
//===================================================================================================================================
void MPOJ_TOOLS::UpdateMfmt(int oldMpojMax)
{
	//�o�b�N�A�b�v�p
	MPOJ_MFMT *backUpMfmt = new MPOJ_MFMT[mpojFile.mpoj.mpojMax];

	//����(mpojType��MPOJ_TYPE::MPOJ_TYPE_MAX�Ȃ���)
	int newMpojMax = 0;	//���g������mfmt
	for (int i = 0; i < oldMpojMax; i++)
	{
		if (mpojFile.mfmt[i].mpojType == mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX)
		{
			//�X�L�b�v
			continue;
		}
		else
		{
			//�o�b�N�A�b�v�����
			backUpMfmt[newMpojMax] = mpojFile.mfmt[i];
			newMpojMax++;
		}
	}

	//������蒼���K�v�������
	if (oldMpojMax > 0)
	{
		//���������
		SAFE_DELETE_ARRAY(mpojFile.mfmt);
	}

	//�������m�ۂ�����
	if (mpojFile.mpoj.mpojMax > 0)
	{
		mpojFile.mfmt = new MPOJ_MFMT[mpojFile.mpoj.mpojMax];

		//���[�h�o�b�N�A�b�v�f�[�^
		for (int i = 0; i < newMpojMax; i++)
		{
			mpojFile.mfmt[i] = backUpMfmt[i];
			mpojFile.mfmt[i].mpojId = i;	//Id�̍X�V
		}

		//MPOJ�̏�������
		mpojFile.mpoj.size = (short)(sizeof(MPOJ_MPOJ) + (sizeof(MPOJ_MFMT) * mpojFile.mpoj.mpojMax));
	}
	//���������
	SAFE_DELETE_ARRAY(backUpMfmt);
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�̃t�H�[�}�b�g�\���̂������z
//===================================================================================================================================
void MPOJ_TOOLS::DeleteMpojFormat(short mpojId)
{
	//�����p(MAX�Ȃ����)
	if (mpojFile.mpoj.mpojMax != NULL)
	{
		//�}�b�v�I�u�W�F�N�g�̐�-1
		mpojFile.mpoj.mpojMax--;
		if (mpojFile.mpoj.mpojMax < 0)
		{
			mpojFile.mpoj.mpojMax = 0;
		}

		//TYPE_MAX�Ȃ����
		mpojFile.mfmt[mpojId].mpojType = mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX;

		//�}�b�v�I�u�W�F�N�g�̃t�H�[�}�b�g�\���̂𐮗�
		UpdateMfmt(mpojFile.mpoj.mpojMax + 1);
	}

	//�߂�
	MpojListboxCurrent--;
	if (MpojListboxCurrent < 0)
	{
		MpojListboxCurrent = 0;
	}

	//�t�@�C���̃A�b�v�f�[�g
	OutputMpojFile();

	//�����_���[�̃��Z�b�g����
	ResetRenderer();
}

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�̃t�H�[�}�b�g�\���̂�ǉ��z
//===================================================================================================================================
void MPOJ_TOOLS::AddMpojFormat(short mpojType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�}�b�v�I�u�W�F�N�g�̐�+1
	mpojFile.mpoj.mpojMax++;

	//�}�b�v�I�u�W�F�N�g�̃t�H�[�}�b�g�\���̂𐮗�
	UpdateMfmt(mpojFile.mpoj.mpojMax - 1);

	//�}�b�v�I�u�W�F�N�g�̃t�H�[�}�b�g�\���̂̍Ō�ɒǉ�
	SetMpoj(mpojFile.mpoj.mpojMax - 1, mpojType,
	 pos, dir);

	//�i��
	MpojListboxCurrent = mpojFile.mpoj.mpojMax - 1;

	//�t�@�C���̃A�b�v�f�[�g
	OutputMpojFile();

	//�����_���[�̃��Z�b�g����
	ResetRenderer();
}
#endif