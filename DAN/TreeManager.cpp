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
	treeList.reserve(INITIAL_RESERVE);
}


//=============================================================================
// �I������
//=============================================================================
void TreeManager::uninitialize()
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		SAFE_DELETE(treeList[i]);
	}

	std::vector<Tree*> temp;
	treeList.swap(temp);
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
//=============================================================================
void TreeManager::destroyTree(int _id)
{
	for (size_t i = 0; i < treeList.size(); i++)
	{
		if (treeList[i]->getTreeData()->id == _id)
		{
			delete treeList[i];
			treeList.erase(treeList.begin() + i);
		}
	}
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