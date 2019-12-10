//===================================================================================================================================
//�yTreeTools.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#include "TreeTools.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TREE_TOOLS::TREE_TOOLS()
{
	//�c���[�t�@�C���\���̂̏�����
	treeFile = { 0 };
	treeFile.tfmt = nullptr;

	//�p�X
	setToolsDirectory();

#ifdef _DEBUG
	//�����_���[�̏�����
	initRender();
#endif

	//�t�@�C��
	FILE	*fp = NULL;
	fp = fopen(TREE_FILE_PATH, "rb");
	if (fp == NULL)	//�t�@�C����������܂���
	{
		//�c���[�t�@�C���̐V�K�쐬
		CreatNewTreeFile();
	}
	else
	{
		//TREE�̓ǂݍ���
		fread(&treeFile.tree, sizeof(TREE_TREE), 1, fp);

		//�������m�ۂ�TFMT�̓ǂݍ���
		if (treeFile.tree.treeMax != NULL)
		{
			//�������m��
			treeFile.tfmt = new TREE_TFMT[treeFile.tree.treeMax];

			for (int i = 0; i < treeFile.tree.treeMax; i++)
			{
				//TFMT�̓ǂݍ���
				fread(&treeFile.tfmt[i], sizeof(TREE_TFMT), 1, fp);
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
	//�c���[���
	TreeListboxCurrent = 0;
	TreeListboxType = treeNS::TREE_TYPE::ANALOG_TREE;
	TreeListboxState = treeNS::GREEN_STATE::GREEN;
	TreeListboxSize = treeNS::TREE_SIZE::STANDARD;
	TreeListboxModel = treeNS::TREE_MODEL::A_MODEL;

	Model[treeNS::TREE_TYPE::ANALOG_TREE] = staticMeshNS::A_TRUNK;
	Model[treeNS::TREE_TYPE::DIGITAL_TREE] = staticMeshNS::B_TRUNK;

	//�����_���[�̃��Z�b�g����
	ResetRenderer();

	input = getInput();
#endif
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TREE_TOOLS::~TREE_TOOLS()
{
	//�t�@�C���̏����o��
	OutputTreeFile();

	if (treeFile.tree.treeMax != NULL)
	{
		SAFE_DELETE_ARRAY(treeFile.tfmt);
#ifdef _DEBUG
		for (int i = 0; i < treeFile.tree.treeMax; i++)
		{
			SAFE_DELETE(object[i]);
		}
		SAFE_DELETE_ARRAY(object);
		SAFE_DELETE_ARRAY(bodyCollide);
#endif
	}

#ifdef _DEBUG
	//�����_���[
	for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
	{
		SAFE_DELETE(trunkRenderer[i]);
	}
	SAFE_DELETE_ARRAY(trunkRenderer);
	for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
	{
		SAFE_DELETE(leafRenderer[i]);
	}
	SAFE_DELETE_ARRAY(leafRenderer);
#endif
}

//===================================================================================================================================
//�y�c���[�̍ő吔���擾�z
//===================================================================================================================================
int TREE_TOOLS::GetTreeMax(void)
{
	return treeFile.tree.treeMax;
}

//===================================================================================================================================
//�y�c���[�\���̂��擾�z
//===================================================================================================================================
treeNS::TREESET TREE_TOOLS::GetTreeSet(short treeId)
{
	treeNS::TREESET tmpTreeSet = { 0 };
	tmpTreeSet.treeID = treeFile.tfmt[treeId].treeId;
	tmpTreeSet.type = (treeNS::TREE_TYPE)treeFile.tfmt[treeId].treeType;
	tmpTreeSet.greenState = (treeNS::GREEN_STATE)treeFile.tfmt[treeId].treeState;
	tmpTreeSet.size = (treeNS::TREE_SIZE)treeFile.tfmt[treeId].treeSize;
	tmpTreeSet.model = (treeNS::TREE_MODEL)treeFile.tfmt[treeId].treeModel;
	tmpTreeSet.initialPosition = D3DXVECTOR3(treeFile.tfmt[treeId].posX, treeFile.tfmt[treeId].posY, treeFile.tfmt[treeId].posZ);
	tmpTreeSet.initialDirection = D3DXVECTOR3(treeFile.tfmt[treeId].dirX, treeFile.tfmt[treeId].dirY, treeFile.tfmt[treeId].dirZ);

	return tmpTreeSet;
}

#ifdef _DEBUG
//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void TREE_TOOLS::outputTreeToolsGUI(int GUIid, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	if (GUIid == TREE_GUI_ID)
	{
		//�t���O
		bool creatFlag = false;
		bool deleteFlag = false;
		if (input->wasKeyPressed('J'))
		{
			creatFlag = true;
		}
		else if (input->wasKeyPressed('K'))
		{
			deleteFlag = true;
		}

		//�@�\
		ImGui::Checkbox("New Tree", &creatFlag);
		ImGui::Checkbox("Delete", &deleteFlag);

		//�c���[�̎��
		const char* listboxTreeType[] = { "ANALOG_TREE" ,"DIGITAL_TREE" };
		ImGui::ListBox("Tree Type", &TreeListboxType, listboxTreeType, treeNS::TREE_TYPE::TREE_TYPE_MAX);

		//�c���[�̏��
		const char* listboxTreeState[] = { "GREEN" ,"DEAD" };
		ImGui::ListBox("Tree State", &TreeListboxState, listboxTreeState, treeNS::GREEN_STATE::GREEN_STATE_MAX);

		//�c���[�̃T�C�Y
		const char* listboxTreeSize[] = { "STANDARD" ,"LARGE","VERY_LARGE" };
		ImGui::ListBox("Tree Size", &TreeListboxSize, listboxTreeSize, treeNS::TREE_SIZE::TREE_SIZE_MAX);

		//�c���[�̃��f��
		const char* listboxTreeModel[] = { "A_MODEL" ,"B_MODEL" ,"C_MODEL" };
		ImGui::ListBox("Tree Model", &TreeListboxModel, listboxTreeModel, treeNS::TREE_MODEL::TREE_MAX);

		//�c���[�̏��
		if (treeFile.tree.treeMax != NULL)
		{
			//�������m��
			char **ListboxIndexName = new char*[treeFile.tree.treeMax];
			for (int i = 0; i < treeFile.tree.treeMax; i++)
			{
				ListboxIndexName[i] = new char[CHAR_MAX];
				sprintf(ListboxIndexName[i], "%d", i);
			}

			ImGui::ListBox("Tree ", &TreeListboxCurrent, ListboxIndexName, treeFile.tree.treeMax);
			ImGui::Text("ITem ID:%d", treeFile.tfmt[TreeListboxCurrent].treeId);
			ImGui::Text("Tree pos:%f %f %f", treeFile.tfmt[TreeListboxCurrent].posX,
				treeFile.tfmt[TreeListboxCurrent].posY,
				treeFile.tfmt[TreeListboxCurrent].posZ);
			ImGui::Text("Tree dir:%f %f %f", treeFile.tfmt[TreeListboxCurrent].dirX,
				treeFile.tfmt[TreeListboxCurrent].dirY,
				treeFile.tfmt[TreeListboxCurrent].dirZ);
			ImGui::Text("Tree Type:%s",
				listboxTreeType[treeFile.tfmt[TreeListboxCurrent].treeType]);
			ImGui::Text("Tree State:%s",
				listboxTreeState[treeFile.tfmt[TreeListboxCurrent].treeState]);
			ImGui::Text("Tree Size:%s",
				listboxTreeSize[treeFile.tfmt[TreeListboxCurrent].treeSize]);
			ImGui::Text("Tree Model:%s",
				listboxTreeModel[treeFile.tfmt[TreeListboxCurrent].treeModel]);

			//���������
			for (int i = 0; i < treeFile.tree.treeMax; i++)
			{
				delete[] ListboxIndexName[i];
			}
			delete[] ListboxIndexName;
		}

		//�폜
		if (deleteFlag)
		{
			//�폜
			DeleteTreeFormat(TreeListboxCurrent);
		}

		//�V�K�쐬
		if (creatFlag)
		{
			//�쐬
			AddTreeFormat(TreeListboxType, TreeListboxState, TreeListboxSize, TreeListboxModel, pos, dir);
		}
	}
#endif
}

//===================================================================================================================================
//�y�����_���[�̏������z�`��p
//===================================================================================================================================
void TREE_TOOLS::initRender()
{
	//�����_�[�̏�����
	trunkRenderer = new StaticMeshRenderer*[treeNS::TREE_MODEL::TREE_MAX];
	for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
	{
		trunkRenderer[i] = new StaticMeshRenderer(staticMeshNS::reference(GetStaticTrunkMeshID(i)));
	}
	leafRenderer = new StaticMeshRenderer*[treeNS::GREEN_STATE::GREEN_STATE_MAX];
	for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
	{
		leafRenderer[i] = new StaticMeshRenderer(staticMeshNS::reference(GetStaticLeafMeshID(i)));
	}

	//�X�V�t���O
	needUpdate = true;
};

//===================================================================================================================================
//�y�I�u�W�F�N�g�̏������z�`��p
//===================================================================================================================================
void TREE_TOOLS::initObject()
{
	//�I�u�W�F�N�g
	object = new Object*[treeFile.tree.treeMax];
	bodyCollide = new BoundingSphere[treeFile.tree.treeMax];
	for (int i = 0; i < treeFile.tree.treeMax; i++)
	{
		//�I�u�W�F�N�g�̐���
		object[i] = new Object();
		generateTrunk(object[i], treeFile.tfmt[i].treeType,
			D3DXVECTOR3(treeFile.tfmt[i].posX, treeFile.tfmt[i].posY, treeFile.tfmt[i].posZ), 
			D3DXVECTOR3(treeFile.tfmt[i].dirX, treeFile.tfmt[i].dirY, treeFile.tfmt[i].dirZ));
		generateLeaf(object[i], treeFile.tfmt[i].treeState, 
			D3DXVECTOR3(treeFile.tfmt[i].posX, treeFile.tfmt[i].posY, treeFile.tfmt[i].posZ),
			D3DXVECTOR3(treeFile.tfmt[i].dirX, treeFile.tfmt[i].dirY, treeFile.tfmt[i].dirZ));
		trunkRenderer[treeFile.tfmt[i].treeType]->updateAccessList();
		leafRenderer[treeFile.tfmt[i].treeState]->updateAccessList();

		// �R���C�_�̏�����
		bodyCollide->initialize(&D3DXVECTOR3(treeFile.tfmt[i].posX, treeFile.tfmt[i].posY, treeFile.tfmt[i].posZ),
			staticMeshNS::reference(GetStaticLeafMeshID(treeFile.tfmt[i].treeState))->mesh);
	}
}

//===================================================================================================================================
//�y�X�V�z�`��p
//===================================================================================================================================
void TREE_TOOLS::update()
{
	if (!needUpdate)return;
	for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
	{
		trunkRenderer[i]->updateBuffer();
		trunkRenderer[i]->updateArray();
		trunkRenderer[i]->update();
	}
	for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
	{
		leafRenderer[i]->updateBuffer();
		leafRenderer[i]->updateArray();
		leafRenderer[i]->update();
	}

	needUpdate = false;
}

//===================================================================================================================================
//�y�`��z�`��p
//===================================================================================================================================
void TREE_TOOLS::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (treeFile.tree.treeMax != NULL)
	{
		for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
		{
			trunkRenderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
		for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
		{
			leafRenderer[i]->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
		}
	}
}

//===================================================================================================================================
//�y�����z�`��p
//===================================================================================================================================
void TREE_TOOLS::generateTrunk(Object *object, short treeType, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 �Ȃ������
	trunkRenderer[treeType]->registerObject(object);
}

//===================================================================================================================================
//�y�����z�`��p
//===================================================================================================================================
void TREE_TOOLS::generateLeaf(Object *object, short treeState, D3DXVECTOR3 position, D3DXVECTOR3 dir)
{
	object->initialize(&position);
	object->postureControl(object->axisZ.direction, dir, 1.0f);
	object->existenceTimer = 1.0f;		// < 0 �Ȃ������
	leafRenderer[treeState]->registerObject(object);
}
#endif

//===================================================================================================================================
//�y�c���[�t�@�C���̏����o�������z
//===================================================================================================================================
void TREE_TOOLS::OutputTreeFile(void)
{
	//�p�X
	setToolsDirectory();

	FILE	*fp = NULL;					//�t�@�C��
	fp = fopen(TREE_FILE_PATH, "wb");

	//TREE�̏����o��
	fwrite(&treeFile.tree, sizeof(TREE_TREE), 1, fp);

	//TFMT�̏����o��
	for (int i = 0; i < treeFile.tree.treeMax; i++)
	{
		//TFMT�̏����o��
		fwrite(&treeFile.tfmt[i], sizeof(TREE_TFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//�y�c���[�t�@�C���̐V�K�쐬�z
//===================================================================================================================================
void TREE_TOOLS::CreatNewTreeFile(void)
{
	//�t�@�C��
	FILE	 *fp = NULL;
	fp = fopen(TREE_FILE_PATH, "wb");

	//�c���[�t�@�C���\����
	TREE_FILE tmpEnemyFile;

	//�`�����N
	memcpy(tmpEnemyFile.tree.chunkId, TREE_CHUNK, sizeof(tmpEnemyFile.tree.chunkId));

	//�c���[�̏�����
	tmpEnemyFile.tree.treeMax = 0;

	//ENMY�̃T�C�Y
	tmpEnemyFile.tree.size = sizeof(TREE_TREE);

	//�����o��
	fwrite(&tmpEnemyFile.tree, sizeof(TREE_TREE), 1, fp);

	//�t�@�C��
	fclose(fp);
}

#ifdef _DEBUG
//===================================================================================================================================
//�y�����_���[�̃��Z�b�g�����z�`��p
//===================================================================================================================================
void TREE_TOOLS::ResetRenderer(void)
{
	//�I������
	for (int i = 0; i < treeFile.tree.treeMax - 1; i++)
	{
		SAFE_DELETE(object[i]);
	}
	SAFE_DELETE_ARRAY(object);

	if (treeFile.tree.treeMax != NULL)
	{
		for (int i = 0; i < treeNS::TREE_MODEL::TREE_MAX; i++)
		{
			SAFE_DELETE(trunkRenderer[i]);
		}
		SAFE_DELETE_ARRAY(trunkRenderer);

		for (int i = 0; i < treeNS::GREEN_STATE::GREEN_STATE_MAX; i++)
		{
			SAFE_DELETE(leafRenderer[i]);
		}
		SAFE_DELETE_ARRAY(leafRenderer);

		//�����_���[�̏�����
		initRender();

		//�I�u�W�F�N�g�̏�����
		initObject();
	}
}

//===================================================================================================================================
//�y�X�^�e�B�b�N���b�V����ID���擾�z�`��p
//===================================================================================================================================
int TREE_TOOLS::GetStaticTrunkMeshID(short treeType)
{
	int staticMeshNo = 0;
	switch (treeType)
	{
	case treeNS::TREE_MODEL::A_MODEL:
		staticMeshNo = staticMeshNS::A_TRUNK;
		break;
	case treeNS::TREE_MODEL::B_MODEL:
		staticMeshNo = staticMeshNS::B_TRUNK;
		break;
	case treeNS::TREE_MODEL::C_MODEL:
		staticMeshNo = staticMeshNS::GREEN_TREE_002;
		break;
	default:
		break;
	}
	return staticMeshNo;
}

//===================================================================================================================================
//�y�X�^�e�B�b�N���b�V����ID���擾�z�`��p
//===================================================================================================================================
int TREE_TOOLS::GetStaticLeafMeshID(short treeState)
{
	int staticMeshNo = 0;
	switch (treeState)
	{
	case treeNS::GREEN_STATE::GREEN:
		staticMeshNo = staticMeshNS::A_LEAF;
		break;
	case treeNS::GREEN_STATE::DEAD:
		staticMeshNo = staticMeshNS::GRASS;
		break;
	default:
		break;
	}
	return staticMeshNo;
}

//===================================================================================================================================
//�y�c���[�̎�ނ�ݒ�z
//===================================================================================================================================
void TREE_TOOLS::SetTreeType(short treeId, short treeType)
{
	treeFile.tfmt[treeId].treeType = treeType;
}

//===================================================================================================================================
//�y�c���[�̏�Ԃ�ݒ�z
//===================================================================================================================================
void TREE_TOOLS::SetTreeState(short treeId, short treeState)
{
	treeFile.tfmt[treeId].treeState = treeState;
}

//===================================================================================================================================
//�y�c���[�̃T�C�Y��ݒ�z
//===================================================================================================================================
void TREE_TOOLS::SetTreeSize(short treeId, short treeSize)
{
	treeFile.tfmt[treeId].treeSize = treeSize;
}

//===================================================================================================================================
//�y�c���[�̃��f����ݒ�z
//===================================================================================================================================
void TREE_TOOLS::SetTreeModel(short treeId, short treeModel)
{
	treeFile.tfmt[treeId].treeModel = treeModel;
}

//===================================================================================================================================
//�y�c���[�̈ʒu��ݒ�z
//===================================================================================================================================
void TREE_TOOLS::SetTreePos(short treeId, const D3DXVECTOR3 pos)
{
	treeFile.tfmt[treeId].posX = pos.x;
	treeFile.tfmt[treeId].posY = pos.y;
	treeFile.tfmt[treeId].posZ = pos.z;
}

//===================================================================================================================================
//�y�c���[�̉�]����ݒ�z
//===================================================================================================================================
void TREE_TOOLS::SetTreeDir(short treeId, const D3DXVECTOR3 dir)
{
	treeFile.tfmt[treeId].dirX = dir.x;
	treeFile.tfmt[treeId].dirY = dir.y;
	treeFile.tfmt[treeId].dirZ = dir.z;
}

//===================================================================================================================================
//�y�c���[�̐ݒu�z
//===================================================================================================================================
void TREE_TOOLS::SetTree(short treeId, short treeType, short treeState,
	short treeSize, short treeModel, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�`�����N
	memcpy(treeFile.tfmt[treeId].chunkId, TREE_CHUNK, sizeof(treeFile.tfmt[treeId].chunkId));

	//�c���[���
	SetTreeType(treeId, treeType);
	SetTreeState(treeId, treeState);
	SetTreeSize(treeId, treeSize);
	SetTreeModel(treeId, treeModel);
	SetTreePos(treeId, pos);
	SetTreeDir(treeId, dir);

	//�T�C�Y
	treeFile.tfmt[treeId].size = (short)sizeof(TREE_TFMT);
	//ID
	treeFile.tfmt[treeId].treeId = treeId;
}

//===================================================================================================================================
//�y�c���[�̃t�H�[�}�b�g�\���̂𐮗��z
//===================================================================================================================================
void TREE_TOOLS::UpdateTfmt(int oldTreeMax)
{
	//�o�b�N�A�b�v�p
	TREE_TFMT *backUpTfmt = new TREE_TFMT[treeFile.tree.treeMax];

	//����(treeType��TREE_TYPE::TREE_TYPE_MAX�Ȃ���)
	int newTreeMax = 0;	//���g������tfmt
	for (int i = 0; i < oldTreeMax; i++)
	{
		if (treeFile.tfmt[i].treeType == treeNS::TREE_TYPE::TREE_TYPE_MAX)
		{
			//�X�L�b�v
			continue;
		}
		else
		{
			//�o�b�N�A�b�v�����
			backUpTfmt[newTreeMax] = treeFile.tfmt[i];
			newTreeMax++;
		}
	}

	//������蒼���K�v�������
	if (oldTreeMax > 0)
	{
		//���������
		SAFE_DELETE_ARRAY(treeFile.tfmt);
	}

	//�������m�ۂ�����
	if (treeFile.tree.treeMax > 0)
	{
		treeFile.tfmt = new TREE_TFMT[treeFile.tree.treeMax];

		//���[�h�o�b�N�A�b�v�f�[�^
		for (int i = 0; i < newTreeMax; i++)
		{
			treeFile.tfmt[i] = backUpTfmt[i];
			treeFile.tfmt[i].treeId = i;	//Id�̍X�V
		}

		//TREE�̏�������
		treeFile.tree.size = (short)(sizeof(TREE_TREE) + (sizeof(TREE_TFMT) * treeFile.tree.treeMax));
	}
	//���������
	SAFE_DELETE_ARRAY(backUpTfmt);
}

//===================================================================================================================================
//�y�c���[�̃t�H�[�}�b�g�\���̂������z
//===================================================================================================================================
void TREE_TOOLS::DeleteTreeFormat(short treeId)
{
	//�����p(MAX�Ȃ����)
	if (treeFile.tree.treeMax != NULL)
	{
		//�c���[�̐�-1
		treeFile.tree.treeMax--;
		if (treeFile.tree.treeMax < 0)
		{
			treeFile.tree.treeMax = 0;
		}

		//TYPE_MAX�Ȃ����
		treeFile.tfmt[treeId].treeType = treeNS::TREE_TYPE::TREE_TYPE_MAX;

		//�c���[�̃t�H�[�}�b�g�\���̂𐮗�
		UpdateTfmt(treeFile.tree.treeMax + 1);
	}

	//�߂�
	TreeListboxCurrent--;
	if (TreeListboxCurrent < 0)
	{
		TreeListboxCurrent = 0;
	}

	//�t�@�C���̃A�b�v�f�[�g
	OutputTreeFile();

	//�����_���[�̃��Z�b�g����
	ResetRenderer();
}

//===================================================================================================================================
//�y�c���[�̃t�H�[�}�b�g�\���̂�ǉ��z
//===================================================================================================================================
void TREE_TOOLS::AddTreeFormat(short treeType, short treeState,
	short treeSize, short treeModel, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�c���[�̐�+1
	treeFile.tree.treeMax++;

	//�c���[�̃t�H�[�}�b�g�\���̂𐮗�
	UpdateTfmt(treeFile.tree.treeMax - 1);

	//�c���[�̃t�H�[�}�b�g�\���̂̍Ō�ɒǉ�
	SetTree(treeFile.tree.treeMax - 1, treeType, treeState,
		treeSize, treeModel, pos, dir);

	//�i��
	TreeListboxCurrent = treeFile.tree.treeMax - 1;

	//�t�@�C���̃A�b�v�f�[�g
	OutputTreeFile();

	//�����_���[�̃��Z�b�g����
	ResetRenderer();
}
#endif