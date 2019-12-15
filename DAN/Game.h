//===================================================================================================================================
//�yGame.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/11/05
//===================================================================================================================================
#pragma once

// �`�[�g�v���[���L���̏ꍇ�̓R�����g�A�E�g���͂���
//#define CHEAT_PRESENTATION

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
#include "TestPlane.h"
#include "EnemyManager.h"
#include "AIDirector.h"
#include "NavigationMesh.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Wind.h"
#include "MapObjectManager.h"
#include "Telop.h"
#include "Sky.h"
#include "SpriteGauge.h"
#include "Battery.h"
#include "Timer.h"
#include "Reticle.h"
#include "Ocean.h"
#include "FixedUI.h"
#include "Advertisement.h"

#include "Sound.h"
#include "SoundBase.h"
#include "LinearTreeCell.h"
#include "movep.h"

#include "TelopManager.h"
#include "Player1UI.h"
#include "Player2UI.h"
//#include "Text.h"


//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace gameNS
{
	//�v���C���[�����ʒu
	const D3DXVECTOR3 PLAYER_POSITION =	D3DXVECTOR3(0,100,0);

	//�J�������Έʒu
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f,0.0f,-1.5f,0.0f);

	//�J�������Β����ʒu
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//===================================================================================================================================
//�y�Q�[���V�[���N���X�z
//===================================================================================================================================
class Game : public AbstractScene
{
private:

	int								nowRenderingWindow;	//���݂̕`��E�B���h�E���ʎq

	//�Փ˔���
	//Linear4TreeManager<Object>*	linear4TreeManager;	//���`�S���؊Ǘ��N���X
	Linear8TreeManager<Object>*		linear8TreeManager;	//���`�W���؊Ǘ��N���X
	ObjectTree<Object>*				objectTreeArray;	//�I�u�W�F�N�g�c���[
	DWORD							collisionNum;		//�Փ˔����
	CollisionList<Object>*			collisionList;		//�Փ˔��胊�X�g

	//3D�I�u�W�F�N�g
	Player*							player;				//�v���C���[
	StaticMeshRenderer*				maleRenderer;		//�j�v���C���[�����_���[
	StaticMeshRenderer*				femaleRenderer;		//���v���C���[�����_���[

	Object*							testField;			//�t�B�[���h
	StaticMeshRenderer*				testFieldRenderer;	//�t�B�[���h�����_���[

	Sky*							sky;				//�X�J�C�h�[��
	Ocean*							ocean;				//�C��

	StaticMeshRenderer*		MoveP;
	MOVEP*					MoveP1;

	//�r���{�[�h
	//TestEffect*						testEffect;			//�C���X�^���V���O�r���{�[�h�e�X�g
	TestPlane*						samplePlane;		//�f�B�X�v���C�p�v���[���T���v��
	Advertisement*					ad;					//�J�����L��

	EnemyManager*					enemyManager;		// �G�l�~�[�}�l�[�W���[
	TreeManager*					treeManager;		// �c���[�}�l�[�W���[
	ItemManager*					itemManager;		// �A�C�e���}�l�[�W���[
	WindManager*					windManager;		// ���}�l�[�W���[
	MapObjectManager*				mapObjectManager;	// �}�b�v�I�u�W�F�N�g�}�l�[�W���[

	AIDirector*						aiDirector;			// AI
	NavigationMesh*					naviMesh;			// �i�r�Q�[�V�������b�V��

	//UI
	SpriteGauge*					spriteGauge;		//Sprite����
	Reticle*						reticle;			//���e�B�N��
	TelopManager*					telopManager;		//�e���b�v�}�l�W���[
	FixedUI*						fixedUI;			//�Œ肳�ꂽUI
	Player1UI*						player1UI;			//�v���C���[�����UI
	Player2UI*						player2UI;			//�v���C���[�Q�����UI

	//�Đ��p�����[�^
	PLAY_PARAMETERS playParameters[4];
	
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
	//void tree4Reregister(Object* tmp);//�I�u�W�F�N�g�̕��؋�Ԃւ̍ēo�^����
	void tree8Reregister(Object* tmp);//�I�u�W�F�N�g�̕��؋�Ԃւ̍ēo�^����

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};


//===================================================================================================================================
//�y�f�o�b�O�z
//===================================================================================================================================
#ifdef _DEBUG



#endif// _DEBUG
