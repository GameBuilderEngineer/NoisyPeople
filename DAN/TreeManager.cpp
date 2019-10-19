//-----------------------------------------------------------------------------
// �c���[�Ǘ��N���X [Tree.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "TreeManager.h"
#include "ImguiManager.h"
using namespace treeNS;


//=============================================================================
// ������
//=============================================================================
void TreeManager::initialize()
{
#if 0
	// �c���[�t�@�C����ǂݍ���
	FILE	*fp = NULL;
	fopen("enemy.enemy", "rb");
	ENEMY_FILE enemyFile;
	memset(&enemyFile, 0, sizeof(ENEMY_FILE));
	fread(&enemyFile.enmy, sizeof(ENEMY_ENMY), 1, fp);
	enemyFile.efmt = new ENEMY_EFMT[enemyFile.enmy.enemyMax];
	fread(&enemyFile.efmt, sizeof(ENEMY_EFMT), enemyFile.enmy.enemyMax, fp);
	fclose(fp);

	for (size_t i = 0; i < enemyFile.enmy.enemyMax; i++)
	{
		tree.XX = //enemyFile.efmt[i].chunkId;
	}
	SAFE_DELETE_ARRAY(enemyFile.efmt);
#endif

	// �`��I�u�W�F�N�g�̍쐬
	greenA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	deadA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	greenB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	deadB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
}


//=============================================================================
// �I������
//=============================================================================
void TreeManager::uninitialize()
{
	// �S�c���[�I�u�W�F�N�g��j��
	destroyAllTree();

	// �x�N�^�[�̊m�ۃ��������������i�������A���P�[�^�����ɖ߂��j
	std::vector<Tree*> temp;
	treeList.swap(temp);

	// �`��I�u�W�F�N�g�̔j��
	SAFE_DELETE(greenA);
	SAFE_DELETE(deadA);
	SAFE_DELETE(greenB);
	SAFE_DELETE(deadB);
}


//=============================================================================
// �X�V����
//=============================================================================
void TreeManager::update(float frameTime)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->update(frameTime);
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void TreeManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		treeList[i]->render(view, projection, cameraPosition);
	}
}


//=============================================================================
// �c���[�I�u�W�F�N�g�̍쐬
//=============================================================================
void TreeManager::createTree(TreeData* treeData)
{
	switch (treeData->treeType)
	{
	case ANALOG_TREE:
		break;

	case DIGITAL_TREE:
		break;

	default:
		treeList.emplace_back(new Tree);
		break;
	}

	treeList.back()->setDataToTree(treeData);
}


//=============================================================================
// �c���[�I�u�W�F�N�g�̔j��
// ���g�p�s��
//=============================================================================
void TreeManager::destroyTree(int _id)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		if (treeList[i]->getTreeData()->id == _id)
		{
			SAFE_DELETE(treeList[i]);
			treeList.erase(treeList.begin() + i);
			break;
		}
	}
	// ����StaticMeshObject�ɓo�^���ꂽ�I�u�W�F�N�g���ʂɔj���ł��Ȃ�
}


//=============================================================================
// �S�c���[�I�u�W�F�N�g�̔j��
// ��������
//=============================================================================
void TreeManager::destroyAllTree()
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}

	treeList.clear();
	SAFE_DELETE(greenA);
	SAFE_DELETE(deadA);
	SAFE_DELETE(greenB);
	SAFE_DELETE(deadB);
	greenA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	deadA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	greenB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	deadB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
}



//=============================================================================
// ImGui�ɏo��
//=============================================================================
void TreeManager::outputGUI()
{
#ifdef _DEBUG

	if (ImGui::CollapsingHeader("TreeInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		//float limitTop = 1000;
		//float limitBottom = -1000;

		ImGui::Text("numOfTree:%d", Tree::getNumOfTree());

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