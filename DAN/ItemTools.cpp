//===================================================================================================================================
//�yItemTools.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#include "ItemTools.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
ITEM_TOOLS::ITEM_TOOLS()
{
	//�A�C�e���t�@�C���\���̂̏�����
	itemFile = { 0 };
	itemFile.ifmt = nullptr;

	//�p�X
	setToolsDirectory();

	//�����_���[�̏�����
	initRender();

	//�t�@�C��
	FILE	*fp = NULL;
	fp = fopen(ITEM_FILE_PATH, "rb");
	if (fp == NULL)	//�t�@�C����������܂���
	{
		//�A�C�e���t�@�C���̐V�K�쐬
		CreatNewItemFile();
	}
	else
	{
		//ITEM�̓ǂݍ���
		fread(&itemFile.item, sizeof(ITEM_ITEM), 1, fp);

		//�������m�ۂ�IFMT�̓ǂݍ���
		if (itemFile.item.itemMax != NULL)
		{
			//�������m��
			itemFile.ifmt = new ITEM_IFMT[itemFile.item.itemMax];

			for (int i = 0; i < itemFile.item.itemMax; i++)
			{
				//IFMT�̓ǂݍ���
				fread(&itemFile.ifmt[i], sizeof(ITEM_IFMT), 1, fp);
			}
		}

		//�t�@�C��
		fclose(fp);

		//�I�u�W�F�N�g�̏�����
		initObject();
	}

	//�A�C�e�����
	ItemListboxCurrent = 0;
	ItemListboxType = itemNS::ITEM_TYPE::BATTERY;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
ITEM_TOOLS::~ITEM_TOOLS()
{
	//�t�@�C���̏����o��
	OutputItemFile();

	if (itemFile.item.itemMax != NULL)
	{
		SAFE_DELETE_ARRAY(itemFile.ifmt);
		for (int i = 0; i < itemFile.item.itemMax; i++)
		{
			SAFE_DELETE(object[i]);
		}
		SAFE_DELETE_ARRAY(object);
		SAFE_DELETE_ARRAY(bodyCollide);
	}

	//�����_���[
	for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
	{
		SAFE_DELETE(renderer[i]);
	}
	SAFE_DELETE_ARRAY(renderer);
}

//===================================================================================================================================
//�y�A�C�e���̍ő吔���擾�z
//===================================================================================================================================
int ITEM_TOOLS::GetItemMax(void)
{
	return itemFile.item.itemMax;
}

//===================================================================================================================================
//�y�A�C�e���\���̂��擾�z
//===================================================================================================================================
itemNS::ITEMSET ITEM_TOOLS::GetItemSet(short itemId)
{
	itemNS::ITEMSET tmpItemSet = { 0 };
	tmpItemSet.itemID = itemFile.ifmt[itemId].itemId;
	tmpItemSet.type = itemFile.ifmt[itemId].itemType;
	tmpItemSet.defaultPosition = D3DXVECTOR3(itemFile.ifmt[itemId].posX, itemFile.ifmt[itemId].posY, itemFile.ifmt[itemId].posZ);
	tmpItemSet.defaultDirection = D3DXVECTOR3(itemFile.ifmt[itemId].dirX, itemFile.ifmt[itemId].dirY, itemFile.ifmt[itemId].dirZ);

	return tmpItemSet;
}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void ITEM_TOOLS::outputItemToolsGUI(int GUIid, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	if (GUIid == ITEM_GUI_ID)
	{
		//�t���O
		bool creatFlag = false;
		bool deleteFlag = false;

		//�@�\
		ImGui::Checkbox("New Item", &creatFlag);
		ImGui::Checkbox("Delete", &deleteFlag);

		//�A�C�e���̎��
		const char* listboxItemType[] = { "BATTERY" ,"EXAMPLE" };
		ImGui::ListBox("Item Type", &ItemListboxType, listboxItemType, itemNS::ITEM_TYPE::ITEM_TYPE_MAX);

		//�A�C�e���̏��
		if (itemFile.item.itemMax != NULL)
		{
			//�������m��
			char **ListboxIndexName = new char*[itemFile.item.itemMax];
			for (int i = 0; i < itemFile.item.itemMax; i++)
			{
				ListboxIndexName[i] = new char[CHAR_MAX];
				sprintf(ListboxIndexName[i], "%d", i);
			}

			ImGui::ListBox("Item ", &ItemListboxCurrent, ListboxIndexName, itemFile.item.itemMax);
			ImGui::Text("ITem ID:%d", itemFile.ifmt[ItemListboxCurrent].itemId);
			ImGui::Text("Item pos:%f %f %f", itemFile.ifmt[ItemListboxCurrent].posX,
				itemFile.ifmt[ItemListboxCurrent].posY,
				itemFile.ifmt[ItemListboxCurrent].posZ);
			ImGui::Text("Item dir:%f %f %f", itemFile.ifmt[ItemListboxCurrent].dirX,
				itemFile.ifmt[ItemListboxCurrent].dirY,
				itemFile.ifmt[ItemListboxCurrent].dirZ);
			ImGui::Text("Item Type:%s",
				listboxItemType[itemFile.ifmt[ItemListboxCurrent].itemType]);

			//���������
			for (int i = 0; i < itemFile.item.itemMax; i++)
			{
				delete[] ListboxIndexName[i];
			}
			delete[] ListboxIndexName;
		}

		//�폜
		if (deleteFlag)
		{
			//�폜
			DeleteItemFormat(ItemListboxCurrent);
		}

		//�V�K�쐬
		if (creatFlag)
		{
			//�쐬
			AddItemFormat(ItemListboxType, pos, dir);
		}
	}
#endif
}

//===================================================================================================================================
//�y�����_���[�̏������z�`��p
//===================================================================================================================================
void ITEM_TOOLS::initRender()
{
	//�����_�[�̏�����
	renderer = new StaticMeshRenderer*[itemNS::ITEM_TYPE::ITEM_TYPE_MAX];
	for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
	{
		renderer[i] = new StaticMeshRenderer(staticMeshNS::reference(GetStaticMeshID(i)));
	}

	//�X�V�t���O
	needUpdate = true;
};

//===================================================================================================================================
//�y�I�u�W�F�N�g�̏������z�`��p
//===================================================================================================================================
void ITEM_TOOLS::initObject()
{
	//�I�u�W�F�N�g
	object = new Object*[itemFile.item.itemMax];
	bodyCollide = new BoundingSphere[itemFile.item.itemMax];
	for (int i = 0; i < itemFile.item.itemMax; i++)
	{
		//�I�u�W�F�N�g�̐���
		object[i] = new Object();
		generate(object[i], itemFile.ifmt[i].itemType, D3DXVECTOR3(itemFile.ifmt[i].posX, itemFile.ifmt[i].posY, itemFile.ifmt[i].posZ),
			D3DXVECTOR3(itemFile.ifmt[i].dirX, itemFile.ifmt[i].dirY, itemFile.ifmt[i].dirZ));
		renderer[itemFile.ifmt[i].itemType]->updateAccessList();

		// �R���C�_�̏�����
		bodyCollide->initialize(&D3DXVECTOR3(itemFile.ifmt[i].posX, itemFile.ifmt[i].posY, itemFile.ifmt[i].posZ),
			staticMeshNS::reference(GetStaticMeshID(itemFile.ifmt[i].itemType))->mesh);
	}
}

//===================================================================================================================================
//�y�X�V�z�`��p
//===================================================================================================================================
void ITEM_TOOLS::update()
{
	if (!needUpdate)return;
	for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
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
void ITEM_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (itemFile.item.itemMax != NULL)
	{
		for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
		{
			renderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
	}
}

//===================================================================================================================================
//�y�����z�`��p
//===================================================================================================================================
void ITEM_TOOLS::generate(Object *object, short enemyType, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 �Ȃ������
	renderer[enemyType]->registerObject(object);
}

//===================================================================================================================================
//�y�A�C�e���t�@�C���̏����o�������z
//===================================================================================================================================
void ITEM_TOOLS::OutputItemFile(void)
{
	//�p�X
	setToolsDirectory();

	FILE	*fp = NULL;					//�t�@�C��
	fp = fopen(ITEM_FILE_PATH, "wb");

	//ITEM�̏����o��
	fwrite(&itemFile.item, sizeof(ITEM_ITEM), 1, fp);

	//IFMT�̏����o��
	for (int i = 0; i < itemFile.item.itemMax; i++)
	{
		//IFMT�̏����o��
		fwrite(&itemFile.ifmt[i], sizeof(ITEM_IFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//�y�A�C�e���t�@�C���̐V�K�쐬�z
//===================================================================================================================================
void ITEM_TOOLS::CreatNewItemFile(void)
{
	//�t�@�C��
	FILE	 *fp = NULL;
	fp = fopen(ITEM_FILE_PATH, "wb");

	//�A�C�e���t�@�C���\����
	ITEM_FILE tmpEnemyFile;

	//�`�����N
	memcpy(tmpEnemyFile.item.chunkId, ITEM_CHUNK, sizeof(tmpEnemyFile.item.chunkId));

	//�A�C�e���̏�����
	tmpEnemyFile.item.itemMax = 0;

	//ENMY�̃T�C�Y
	tmpEnemyFile.item.size = sizeof(ITEM_ITEM);

	//�����o��
	fwrite(&tmpEnemyFile.item, sizeof(ITEM_ITEM), 1, fp);

	//�t�@�C��
	fclose(fp);
}

//===================================================================================================================================
//�y�����_���[�̃��Z�b�g�����z�`��p
//===================================================================================================================================
void ITEM_TOOLS::ResetRenderer(void)
{
	//�I������
	for (int i = 0; i < itemFile.item.itemMax - 1; i++)
	{
		SAFE_DELETE(object[i]);
	}
	SAFE_DELETE_ARRAY(object);

	if (itemFile.item.itemMax != NULL)
	{
		for (int i = 0; i < itemNS::ITEM_TYPE::ITEM_TYPE_MAX; i++)
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
int ITEM_TOOLS::GetStaticMeshID(short enemyType)
{
	int staticMeshNo = 0;
	switch (enemyType)
	{
	case itemNS::ITEM_TYPE::BATTERY:
		staticMeshNo = staticMeshNS::SAMPLE_SCISSORS;
		break;
	case itemNS::ITEM_TYPE::EXAMPLE:
		staticMeshNo = staticMeshNS::YAMADA_ROBOT2;
	default:
		break;
	}
	return staticMeshNo;
}

//===================================================================================================================================
//�y�A�C�e���̎�ނ�ݒ�z
//===================================================================================================================================
void ITEM_TOOLS::SetItemType(short itemId, short itemType)
{
	itemFile.ifmt[itemId].itemType = itemType;
}

//===================================================================================================================================
//�y�A�C�e���̈ʒu��ݒ�z
//===================================================================================================================================
void ITEM_TOOLS::SetItemPos(short itemId, const D3DXVECTOR3 pos)
{
	itemFile.ifmt[itemId].posX = pos.x;
	itemFile.ifmt[itemId].posY = pos.y;
	itemFile.ifmt[itemId].posZ = pos.z;
}

//===================================================================================================================================
//�y�A�C�e���̉�]����ݒ�z
//===================================================================================================================================
void ITEM_TOOLS::SetItemDir(short itemId, const D3DXVECTOR3 dir)
{
	itemFile.ifmt[itemId].dirX = dir.x;
	itemFile.ifmt[itemId].dirY = dir.y;
	itemFile.ifmt[itemId].dirZ = dir.z;
}

//===================================================================================================================================
//�y�A�C�e���̐ݒu�z
//===================================================================================================================================
void ITEM_TOOLS::SetItem(short itemId, short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�`�����N
	memcpy(itemFile.ifmt[itemId].chunkId, ITEM_CHUNK, sizeof(itemFile.ifmt[itemId].chunkId));

	//�A�C�e�����
	SetItemType(itemId, itemType);
	SetItemPos(itemId, pos);
	SetItemDir(itemId, dir);

	//�T�C�Y
	itemFile.ifmt[itemId].size = (short)sizeof(ITEM_IFMT);
	//ID
	itemFile.ifmt[itemId].itemId = itemId;
}

//===================================================================================================================================
//�y�A�C�e���̃t�H�[�}�b�g�\���̂𐮗��z
//===================================================================================================================================
void ITEM_TOOLS::UpdateIfmt(int oldItemMax)
{
	//�o�b�N�A�b�v�p
	ITEM_IFMT *backUpIfmt = new ITEM_IFMT[itemFile.item.itemMax];

	//����(itemType��ITEM_TYPE::ITEM_TYPE_MAX�Ȃ���)
	int newItemMax = 0;	//���g������ifmt
	for (int i = 0; i < oldItemMax; i++)
	{
		if (itemFile.ifmt[i].itemType == itemNS::ITEM_TYPE::ITEM_TYPE_MAX)
		{
			//�X�L�b�v
			continue;
		}
		else
		{
			//�o�b�N�A�b�v�����
			backUpIfmt[newItemMax] = itemFile.ifmt[i];
			newItemMax++;
		}
	}

	//������蒼���K�v�������
	if (oldItemMax > 0)
	{
		//���������
		SAFE_DELETE_ARRAY(itemFile.ifmt);
	}

	//�������m�ۂ�����
	if (itemFile.item.itemMax > 0)
	{
		itemFile.ifmt = new ITEM_IFMT[itemFile.item.itemMax];

		//���[�h�o�b�N�A�b�v�f�[�^
		for (int i = 0; i < newItemMax; i++)
		{
			itemFile.ifmt[i] = backUpIfmt[i];
			itemFile.ifmt[i].itemId = i;	//Id�̍X�V
		}

		//ITEM�̏�������
		itemFile.item.size = (short)(sizeof(ITEM_ITEM) + (sizeof(ITEM_IFMT) * itemFile.item.itemMax));
	}
	//���������
	SAFE_DELETE_ARRAY(backUpIfmt);
}

//===================================================================================================================================
//�y�A�C�e���̃t�H�[�}�b�g�\���̂������z
//===================================================================================================================================
void ITEM_TOOLS::DeleteItemFormat(short itemId)
{
	//�����p(MAX�Ȃ����)
	if (itemFile.item.itemMax != NULL)
	{
		//�A�C�e���̐�-1
		itemFile.item.itemMax--;
		if (itemFile.item.itemMax < 0)
		{
			itemFile.item.itemMax = 0;
		}

		//TYPE_MAX�Ȃ����
		itemFile.ifmt[itemId].itemType = itemNS::ITEM_TYPE::ITEM_TYPE_MAX;

		//�A�C�e���̃t�H�[�}�b�g�\���̂𐮗�
		UpdateIfmt(itemFile.item.itemMax + 1);
	}

	//�߂�
	ItemListboxCurrent--;
	if (ItemListboxCurrent < 0)
	{
		ItemListboxCurrent = 0;
	}

	//�t�@�C���̃A�b�v�f�[�g
	OutputItemFile();

	//�����_���[�̃��Z�b�g����
	ResetRenderer();
}

//===================================================================================================================================
//�y�A�C�e���̃t�H�[�}�b�g�\���̂�ǉ��z
//===================================================================================================================================
void ITEM_TOOLS::AddItemFormat(short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�A�C�e���̐�+1
	itemFile.item.itemMax++;

	//�A�C�e���̃t�H�[�}�b�g�\���̂𐮗�
	UpdateIfmt(itemFile.item.itemMax - 1);

	//�A�C�e���̃t�H�[�}�b�g�\���̂̍Ō�ɒǉ�
	SetItem(itemFile.item.itemMax - 1, itemType, pos, dir);

	//�i��
	ItemListboxCurrent = itemFile.item.itemMax - 1;

	//�t�@�C���̃A�b�v�f�[�g
	OutputItemFile();

	//�����_���[�̃��Z�b�g����
	ResetRenderer();
}