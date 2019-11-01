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
#include "TestPlane.h"
#include "EnemyManager.h"
#include "AIDirector.h"
#include "NavigationMesh.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Sky.h"
#include "SpriteGauge.h"
#include "Battery.h"
#include "telop.h"
#include "Timer.h"

#include "Sound.h"
#include "SoundBase.h"
#include "LinearTreeCell.h"
#include "movep.h"
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
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0,2.0f,0);
}

//===================================================================================================================================
//�y�Q�[���V�[���N���X�z
//===================================================================================================================================
class Game : public AbstractScene
{
private:

	//Linear4TreeManager<Object>*	linear4TreeManager;	//���`�S���؊Ǘ��N���X
	Linear8TreeManager<Object>*		linear8TreeManager;	//���`�W���؊Ǘ��N���X
	ObjectTree<Object>*				objectTreeArray;	//�I�u�W�F�N�g�c���[
	DWORD							collisionNum;		//�Փ˔����
	CollisionList<Object>*			collisionList;		//�Փ˔��胊�X�g

	Player*							player;				//�v���C���[
	StaticMeshRenderer*				maleRenderer;		//�j�v���C���[�����_���[
	StaticMeshRenderer*				femaleRenderer;		//���v���C���[�����_���[
	Object*							testField;			//�t�B�[���h
	StaticMeshRenderer*				testFieldRenderer;	//�t�B�[���h�����_���[

	DeadTree*						deadTree;			//�͖�
	TreeTypeA*						treeA;				//�؂`
	TreeTypeB*						treeB;				//��B
	Stone*							stone;				//��
	Sky*							sky;				//�X�J�C�h�[��

	StaticMeshRenderer*		MoveP;
	MOVEP*					MoveP1;

	TestEffect*						testEffect;			//�C���X�^���V���O�r���{�[�h�e�X�g
	TestPlane*						samplePlane;		//�f�B�X�v���C�p�v���[���T���v��

	EnemyManager*					enemyManager;		// �G�l�~�[�}�l�[�W���[
	TreeManager*					treeManager;		// �c���[�}�l�[�W���[
	ItemManager*					itemManager;		// �A�C�e���}�l�[�W���[

	AIDirector*						aiDirector;			// AI
	NavigationMesh*					naviMesh;			// �i�r�Q�[�V�������b�V��
	Telop*							telop;				// �e���b�v

	//�Đ��p�����[�^
	PLAY_PARAMETERS playParameters[4];

	//Sprite����
	SpriteGauge*					spriteGauge;
	//�^�C�}�[
	Timer *timer;
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
