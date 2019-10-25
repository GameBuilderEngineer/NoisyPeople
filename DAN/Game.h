//===================================================================================================================================
//�yGame.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/18
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include <vector>
#include "AbstractScene.h"
#include "Player.h"
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
#include "Sky.h"

#include "SpriteGauge.h"
#include "Battery.h"

#include "Sound.h"
#include "SoundBase.h"
//#include "GameMaster.h"
//#include "PointSprite.h"
//#include "Object.h"
//#include "Text.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace gameNS
{
	//�v���C���[�����ʒu
	const D3DXVECTOR3 PLAYER_POSITION =	D3DXVECTOR3(0,100,0);

	//�J�������Έʒu
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f,5.0f,5.0f,0.0f);

	//�J�������Β����ʒu
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0,5.0f,0);

	//const int NUM_SAMPLE = 1000;

}

//===================================================================================================================================
//�y�Q�[���V�[���N���X�z
//===================================================================================================================================
class Game : public AbstractScene
{
private:

	//Text text;							//Sample
	//Text text2;							//Sample
	//PointSprite pointSprite;		//Sample
	//InstancingBillboard plane;	//Sample
	//Object testObject;				//Sample
	//Object testCube;					//Sample

	//�v���C���[
	Player *player;
	StaticMeshRenderer* playerRenderer;

	//�t�B�[���h
	Object* testField;
	StaticMeshRenderer* testFieldRenderer;
	//�C���X�^���V���O�r���{�[�h�e�X�g
	//InstancingBillboard instancingBillboardTest;
	TestEffect* testEffect;

	//�͖�
	DeadTree* deadTree;
	//�؂`
	TreeTypeA* treeA;
	//��B
	TreeTypeB* treeB;
	//��
	Stone* stone;
	//�X�J�C�h�[��
	Sky* sky;

	//�F�X�ȃI�u�W�F�N�g�̕`��T���v���e�X�g

	//�X�^�e�B�b�N���b�V���ŁA�P�̃I�u�W�F�N�g�݂̂�`�悷��Ƃ��y�ÓI�z
	//StaticSingleStaticMeshRendererTest ssSMO

	//�X�^�e�B�b�N���b�V���ŁA�����̃I�u�W�F�N�g���C���X�^���V���O�`�悷��Ƃ��y�ÓI�z
	//StaticMultiStaticMeshRendererTest smSMO

	//�X�^�e�B�b�N���b�V���ŁA�����̃I�u�W�F�N�g���C���X�^���V���O�`�悷��Ƃ��y���I�z
	//DynamicMultiStaticMeshRendererTest dmSMO

	//�r���{�[�h�ŁA1�̃I�u�W�F�N�g��`�悷��Ƃ��y�ÓI�z
	//StaticSingleInstancingBillboardTest ssIB

	//�r���{�[�h�ŁA�����̃I�u�W�F�N�g���C���X�^���V���O�`�悷��Ƃ��y�ÓI�z
	//StaticMultiInstancingBillboardTest smIB

	//�r���{�[�h�ŁA�����̃I�u�W�F�N�g���C���X�^���V���O�`�悷��Ƃ��y���I�z
	//DynamicMultiInstancingBillboardTest dmIB

	//�ŁA�P�̃I�u�W�F�N�g���C���X�^���V���O�`�悷��Ƃ��y�ÓI�z
	//StaticSingleInstancingPlaneTest ssIP

	//�ŁA�����̃I�u�W�F�N�g���C���X�^���V���O�`�悷��Ƃ��y�ÓI�z
	//StaticMultiInstancingPlaneTest smIP

	//�ŁA�����̃I�u�W�F�N�g���C���X�^���V���O�`�悷��Ƃ��y���I�z
	//DynamicMultiInstancingPlaneTest dmIP

	// �G�l�~�[
	EnemyManager* enemyManager;
	Enemy* enemy;

	// �c���[
	TreeManager* treeManager;

	// �A�C�e��
	ItemManager* itemManager;

	// �e���b�v
	Telop* telop;

	// AI
	AIDirector* aiDirector;
	NavigationMesh* naviAI;

	//�Đ��p�����[�^
	PLAY_PARAMETERS playParameters[4];

	//Sprite����
	SpriteGauge* spriteGauge;

public:
	Game();
	~Game();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	void render3D(Camera currentCamera);
	void renderUI();
	void test();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};


//===================================================================================================================================
//�y�f�o�b�O�z
//===================================================================================================================================
#ifdef _DEBUG

#if 1	// ������1�Ńi�r���b�V���f�o�b�O���[�h
#define DEBUG_NAVIMESH
#endif


#endif// _DEBUG
