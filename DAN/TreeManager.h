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

	//�����_���[�F�f�W�^���c���[
	StaticMeshRenderer* aDTrunkRenderer;		// A���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* aDLeafRenderer;			// A���f���t�`��I�u�W�F�N�g
	StaticMeshRenderer* bDTrunkRenderer;		// B���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* bDLeafRenderer;			// B���f���t�`��I�u�W�F�N�g
	StaticMeshRenderer* cDTrunkRenderer;		// C���f�����`��I�u�W�F�N�g
	StaticMeshRenderer* cDLeafRenderer;			// C���f���t�`��I�u�W�F�N�g


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
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//�c���[�̐���
	void createTree(treeNS::TreeData treeData);
	void createUsingTool();

	//�A�i���O�ƃf�W�^���̕ϊ�
	void swapDA(Tree* tree,int beforeType);

	//�t(�A�i���O)�̕`��o�^/����
	void registerLeafRendering(Object* leaf, int _model);
	void unRegisterLeafRendering(Object* leaf, int _model);

	//�f�W�^���c���[�`��o�^/����
	void registerDigital(Tree* tree);
	void unRegisterDigital(Tree* tree);

	//�A�i���O�c���[�`��o�^/����
	void registerAnalog(Tree* tree);
	void unRegisterAnalog(Tree* tree);


	void destroyAllTree();
	int	issueNewTreeID();
	void outputGUI();

	void switchingVisionView(int playerNo);;
	void switchingNormalView(int playerNo);;

	void playDigitalTreeEffect(int playerNo);
	void stopDigitalTreeEffect(int playerNo);

	void changeWireFrame();
	void changeSolid();

	//Setter
	void setGameMaster(GameMaster* gameMaster);

	// Getter
	std::vector<Tree*>& getTreeList();
	float getGreeningRate();
	int getTreeNum();
};
