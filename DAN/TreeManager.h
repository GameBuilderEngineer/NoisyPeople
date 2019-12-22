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
#include "TreeEffect.h"
#include "GameMaster.h"
#include "networkClient.h"

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
	//�����_���[�F�A�i���O�c���[
	StaticMeshRenderer* aTrunkRenderer;			// A���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* aLeafRenderer;			// A���f���t�`��I�u�W�F�N�g
	StaticMeshRenderer* bTrunkRenderer;			// B���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* bLeafRenderer;			// B���f���t�`��I�u�W�F�N�g
	StaticMeshRenderer* cTrunkRenderer;			// C���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* cLeafRenderer;			// C���f���t�`��I�u�W�F�N�g

	//�G�t�F�N�g
	TreeEffect*	treeEffect;		//�f�W�^���c���[�G�t�F�N�g

	int nextID;									// ����c���[���sID
	LPD3DXMESH	attractorMesh;					// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;				// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X

	int treeNum;
	int greeningTreeNum;						//�Ή��{��
	float greeningRate;							//�Ή���
	bool playedDigitalTreeEffect[gameMasterNS::PLAYER_NUM];

	GameMaster*			gameMaster;				//�Q�[���}�X�^�[�ւ̃|�C���^

public:
	~TreeManager();
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(Camera* camera);

	//�c���[�̐���
	void createTree(treeNS::TreeData treeData);
	void createUsingTool();
	//�S�Ẵc���[�̍폜
	void destroyAllTree();

	//�S�Ă̕`��o�^/����
	void allRegister();
	void allUnRegister();
	//���̕`��o�^/����
	void registerTrunk(Tree* tree);
	void unRegisterTrunk(Tree* tree);
	//�t�̕`��o�^/����
	void registerLeafRendering(Object* leaf, int _model);
	void unRegisterLeafRendering(Object* leaf, int _model);
	//�����ڂ̐ؑ�
	void switchingVisionView(int playerNo);
	void switchingNormalView(int playerNo);

	//�f�W�^���c���[�G�t�F�N�g
	void playDigitalTreeEffect(int playerNo);
	void stopDigitalTreeEffect(int playerNo);


	//�c���[ID�𔭍s����
	int	issueNewTreeID();
	void outputGUI();

	//Setter
	void setGameMaster(GameMaster* gameMaster);

	// Getter
	std::vector<Tree*>& getTreeList();
	float getGreeningRate();
	int getTreeNum();
};
