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

}

//===================================================================================================================================
//�y�Q�[���V�[���N���X�z
//===================================================================================================================================
class Game : public AbstractScene
{
private:

	Player*					player;				//�v���C���[
	StaticMeshRenderer*		maleRenderer;		//�j�v���C���[�����_���[
	StaticMeshRenderer*		femaleRenderer;		//���v���C���[�����_���[
	Object*					testField;			//�t�B�[���h
	StaticMeshRenderer*		testFieldRenderer;	//�t�B�[���h�����_���[

	DeadTree*				deadTree;			//�͖�
	TreeTypeA*				treeA;				//�؂`
	TreeTypeB*				treeB;				//��B
	Stone*					stone;				//��
	Sky*					sky;				//�X�J�C�h�[��

	TestEffect*				testEffect;			//�C���X�^���V���O�r���{�[�h�e�X�g

	EnemyManager*			enemyManager;		// �G�l�~�[�}�l�[�W���[
	Enemy*					enemy;				// �G�l�~�[
	TreeManager*			treeManager;		// �c���[�}�l�[�W���[
	ItemManager*			itemManager;		// �A�C�e���}�l�[�W���[

	Telop*					telop;				// �e���b�v

	AIDirector*				aiDirector;			// AI
	NavigationMesh*			naviAI;				// naviAI

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
