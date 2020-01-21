//===================================================================================================================================
//�yGame.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/11/05
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
#include "movep1.h"
#include "Equipment.h"
#include "TelopManager.h"
#include "Player1UI.h"
#include "Player2UI.h"
//#include "Text.h"
#include "networkClient.h"
#include "CountUI.h"
#include "Announcement.h"
#include "Marker.h"
#include "DamageUI.h"
#include "Title.h"
#include "tmpOcean.h"
#include "PowerUp.h"
#if 1
#define CHEAT_PREZEN
#endif

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

	enum OPENING_CAMERA_LIST
	{
		CAMERA0,
		CAMERA1,
		CAMERA2,
		CAMERA3,
		CAMERA4,
		CAMERA5,
		CAMERA6,
		CAMERA7,
		CAMERA8,
		CAMERA9,
		MAX_CAMERA
	};
	
	enum ITEM_TYPE
	{
		BATTERY,
		EXAMPLE,
		POWER_UP,
		ITEM_TYPE_MAX
	};

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

	Object*							testField;			//�t�B�[���h
	StaticMeshRenderer*				testFieldRenderer;	//�t�B�[���h�����_���[
	Object*							faceField;			//�����t�B�[���h 
	StaticMeshRenderer*				faceFieldRenderer;	//�����t�B�[���h�����_���[

	Sky*							sky;				//�X�J�C�h�[��
	Ocean*							ocean;				//�C��
	//TmpOcean*						tmpOcean;			//���C��

	StaticMeshRenderer*				MoveP;
	MOVEP*							MoveP1;

	//�r���{�[�h
	//TestEffect*						testEffect;		//�C���X�^���V���O�r���{�[�h�e�X�g
	TestPlane*						samplePlane;		//�f�B�X�v���C�p�v���[���T���v��

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
	CountUI*						countUI;			//�J�E���gUI
	Announcement*					announcement;		//�A�i�E���X
	MarkerRenderer*					markerRenderer;		//�}�[�J�[
	DamageUI*						damageUI;			//�_���[�WUI
	//�Đ��p�����[�^
	PLAY_PARAMETERS					playParameters[6];


	//Network
	NETWORK_CLIENT*					networkClient;
	
	
	//�I�[�v�j���O�J����
	Camera* cameraOP;
	//GameOP�J�����^�[�Q�b�g�I�u�W�F�N�g
	Object* target;

	//BOSS�J����
	Camera* cameraBoss;

	int count;

	float moveTime;				//�I�_�܂ł̎���
	float moveTimer;			//�ړ��^�C�}�[
	float displayTime;
	float displayTimer;
	D3DXVECTOR3 startPos;		//���[�v�n�_

	//GameOP�J��������ϐ�
	float rate;
	float rateY;
	float rateX;
	float degreeY;
	float degreeX;
	float degreeTimer;
	float degreeTime;
	float distance;
	float moveDistance;
	
	float degreeTimerX;
	float degreeTimeX;

	int stateCamera;
	float frameDegree = 1.0f / 5.0f;		//�������x
	float inputDegree = 2.0f;				//���͑��x

	//�J�����̎��擾�ϐ�
	D3DXVECTOR3 cameraAxisX;			//�J������]��X
	D3DXVECTOR3 cameraAxisY;			//�J������]��Y
	D3DXVECTOR3 cameraAxisZ;			//�J������]��Z
	D3DXVECTOR3 fixedAxisX;				//�Œ�X��
	D3DXVECTOR3 fixedAxisZ;				//�Œ�Z��
	D3DXQUATERNION tmpCameraQ;			//�J�����̑��Έʒu�x�N�g���̈ꎞ�ۑ�
	D3DXVECTOR3 BezierCurveS1;			//S���x�W�F�Ȑ��_�P
	D3DXVECTOR3 BezierCurveS2;			//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 targetDistance;
	D3DXVECTOR3 newRelative;

	D3DXVECTOR3 BezierPoint1;			//S���x�W�F�Ȑ��_�P
	D3DXVECTOR3 BezierPoint2;			//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 BezierPoint3;			//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 BezierPoint4;			//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 BezierPoint5;			//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 BezierPoint6;			//S���x�W�F�Ȑ��_�Q

	D3DXVECTOR3 Curve1;					//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 Curve2;					//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 Curve3;					//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 Curve4;					//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 Curve5;					//S���x�W�F�Ȑ��_�Q
	bool measurement = true;

public:
	Game();
	~Game();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	void render3D(Camera* currentCamera);
	void renderUI();
	void test();
	//void tree4Reregister(Object* tmp);//�I�u�W�F�N�g�̕��؋�Ԃւ̍ēo�^����
	void tree8Reregister(Object* tmp);//�I�u�W�F�N�g�̕��؋�Ԃւ̍ēo�^����

	//�X�V����
	void updateOP(); //OP�̍X�V

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};

