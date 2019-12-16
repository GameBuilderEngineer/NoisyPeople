//===================================================================================================================================
//�yCreate.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/18
//===================================================================================================================================
#pragma once

#ifdef _DEBUG
//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include <vector>
#include "AbstractScene.h"
#include "TmpObject.h"
#include "StaticMeshRenderer.h"
#include "Stone.h"
#include "DeadTree.h"
#include "TreeTypeA.h"
#include "TreeTypeB.h"
#include "TestEffect.h"
#include "EnemyManager.h"
#include "AIDirector.h"
#include "NavigationMesh.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Sound.h"

#include "EnemyTools.h"
#include "ItemTools.h"
#include "TreeTools.h"
#include "MapObjectTool.h"

//===================================================================================================================================
//�y�񋓌^�萔�z
//===================================================================================================================================
enum TOOLS_TYPE			//�c�[���̎��
{
	TOOLS_NONE,
	TOOLS_ENEMY,		//�G�l�~�[
	TOOLS_ITEM,			//�A�C�e��
	TOOLS_TREE,			//�c���[
	TOOLS_MAP_OBJECT,	//�}�b�v�I�u�W�F�N�g
	TOOLS_MAX
};

//===================================================================================================================================
//�y�Q�[���V�[���N���X�z
//===================================================================================================================================
class Create : public AbstractScene
{
private:

	//hukankamera
	Camera* topView;
	bool onTopView;
	//���I�u�W�F�N�g
	TmpObject *tmpObject;
	StaticMeshRenderer* tmpObjRenderer;
	//�t�B�[���h
	Object* testField;
	StaticMeshRenderer* testFieldRenderer;

	//�C���X�^���V���O�r���{�[�h�e�X�g
	TestEffect* testEffect;

	//�G�l�~�[�c�[��
	ENEMY_TOOLS *enemyTools;
	//�A�C�e���c�[��
	ITEM_TOOLS *itemTools;
	//�c���[�c�[��
	TREE_TOOLS *treeTools;
	//�}�b�v�I�u�W�F�N�g�c�[��
	MPOJ_TOOLS *mapObjTools;

	//�͖�
	DeadTree* deadTree;
	//�؂`
	TreeTypeA* treeA;
	//��B
	TreeTypeB* treeB;
	//��
	Stone* stone;

public:
	Create();
	~Create();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	void render3D(Camera currentCamera);
	void renderUI();

	int ToolsListboxType;
	int meshId;

	virtual void createGUI() override;
	void toolsGUI();
	void collideGUI();
};
#endif