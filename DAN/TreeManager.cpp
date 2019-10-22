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
	nextID = 0;								// ���񔭍sID��0�ɏ�����

	// �`��I�u�W�F�N�g�̍쐬
	tree1Renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	leaf1Renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	tree2Renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	leaf2Renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	tree3Renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	leaf3Renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
#if 0
#endif
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
	SAFE_DELETE(tree1Renderer);
	SAFE_DELETE(leaf1Renderer);
	SAFE_DELETE(tree2Renderer);
	SAFE_DELETE(leaf2Renderer);
	SAFE_DELETE(tree3Renderer);
	SAFE_DELETE(leaf3Renderer);
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

	tree1Renderer->update();
	leaf1Renderer->update();
	tree2Renderer->update();
	leaf2Renderer->update();
	tree3Renderer->update();
	leaf3Renderer->update();
}


//=============================================================================
// �`�揈��
//=============================================================================
void TreeManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	tree1Renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	leaf1Renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	tree2Renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	leaf2Renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	tree3Renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	leaf3Renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// �c���[�I�u�W�F�N�g�̍쐬
//=============================================================================
void TreeManager::createTree(TreeData* treeData)
{
	switch (treeData->modelId)
	{
	case TREE_01:
		//if()
		//if(treeData->geenState == GREEN){leaf1Renderer->generateObject()}
		break;

	case TREE_02:
		break;

	case TREE_03:
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
	//SAFE_DELETE(greenA);
	//SAFE_DELETE(deadA);
	//SAFE_DELETE(greenB);
	//SAFE_DELETE(deadB);
	//greenA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_001));
	//deadA = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
	//greenB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	//deadB = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::DEAD_TREE));
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