//-----------------------------------------------------------------------------
// �c���[�Ǘ��N���X [Tree.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
// �X�V�� : 2019/11/12 �y���� ���z
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshRenderer.h"
#include "Tree.h"
#include "AnalogTree.h"
#include "DigitalTree.h"


//=============================================================================
// ���O���
//=============================================================================
namespace treeManagerNS
{	
}


//=============================================================================
//�N���X��`
//=============================================================================
class TreeManager
{
private:
	std::vector<Tree*> treeList;				// �c���[�|�C���^���X�g
	StaticMeshRenderer* aTrunkRenderer;			// A���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* aLeafRenderer;			// A���f���t�`��I�u�W�F�N�g
	StaticMeshRenderer* bTrunkRenderer;			// B���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* bLeafRenderer;			// B���f���t�`��I�u�W�F�N�g
	StaticMeshRenderer* cTrunkRenderer;			// C���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* cLeafRenderer;			// C���f���t�`��I�u�W�F�N�g
	int nextID;									// ����c���[���sID
	LPD3DXMESH	attractorMesh;					// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;				// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X

	int greeningTreeNum;						//�Ή��{��
	float greeningRate;							//�Ή���

public:
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createTree(treeNS::TreeData treeData);
	void registerLeafRendering(Object* leaf, int _model);
	void unRegisterLeafRendering(Object* leaf, int _model);
	void destroyAllTree();
	int issueNewTreeID();
	void outputGUI();

	// Getter
	std::vector<Tree*>& getTreeList();
};
