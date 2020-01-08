//===================================================================================================================================
//�yTutorial.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
#include "Player.h"
#include "TutorialTex.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "tutorialPlane.h"
#include "Sky.h"
#include "Timer.h"
#include "movep.h"
#include "movep1.h"
#include "TutorialUI.h"
#include "UtilityFunction.h"
#include "DamageUI.h"
#include "Player1UI.h"
#include "Player2UI.h"
#include "Reticle.h"
#include "FixedUI.h"



//===================================================================================================================================
//�y�}�N����`�z
//===================================================================================================================================
#define NEXT_MAX	(4)	// �`���[�g���A���}�b�N�X�l

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace tutorialNS
{
	// �v���C���[�l��
	enum {
		PLAYER1,
		PLAYER2,
		NUM_PLAYER,
	};

	//�v���C���[�����ʒu
	const D3DXVECTOR3 PLAYER_P1_POSITION = D3DXVECTOR3(0, 20, -45);
	const D3DXVECTOR3 PLAYER_P2_POSITION = D3DXVECTOR3(-1000, 20, -45);

	//�G�l�~�[�����ʒu
	const D3DXVECTOR3 ENEMY_POSTITION = D3DXVECTOR3(0, 0, 0);

	//�c���[�����ʒu
	const D3DXVECTOR3 FIRST_TREE_POSTITION = D3DXVECTOR3(-20, 0, -50);

	//�J�������Έʒu
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f, 0.0f, -1.5f, 0.0f);

	//�J�������Β����ʒu
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===================================================================================================================================
//�y�`���[�g���A���V�[���N���X�z
//===================================================================================================================================
class Tutorial : public AbstractScene
{
private:

	//�`���[�g���A��2D
	TutorialTex tutorialTex;

	//�f�B�X�v���C�v���[��
	TutorialPlane **plane;

	//�f�[�^
	enemyNS::EnemyData* enemyData;

	//�i��
	float tutorialTimer;								// �`���[�g���A������
	int planeStep[gameMasterNS::PLAYER_NUM];
	int step[gameMasterNS::PLAYER_NUM];


	//System
	int								nowRenderingWindow;	//���݂̕`��E�B���h�E���ʎq
	Linear8TreeManager<Object>*		linear8TreeManager;	//���`�W���؊Ǘ��N���X
	ObjectTree<Object>*				objectTreeArray;	//�I�u�W�F�N�g�c���[
	DWORD							collisionNum;		//�Փ˔����
	CollisionList<Object>*			collisionList;		//�Փ˔��胊�X�g

	//3DObject
	Player*							player;				//�v���C���[
	Object**						testField;			//�t�B�[���h
	StaticMeshRenderer*				testFieldRenderer;	//�t�B�[���h�����_���[
	Sky*							sky;				//�X�J�C�h�[��
	StaticMeshRenderer*				MoveP;
	MOVEP*							MoveP1;
	EnemyManager*					enemyManager;		// �G�l�~�[�}�l�[�W���[
	TreeManager*					treeManager;		// �c���[�}�l�[�W���[
	NavigationMesh*					naviMesh;			// �i�r�Q�[�V�������b�V��

	//UI
	TutorialUI *tutorialUI;
	Reticle*						reticle;			//���e�B�N��
	FixedUI*						fixedUI;			//�Œ肳�ꂽUI
	Player1UI*						player1UI;			//�v���C���[�����UI
	Player2UI*						player2UI;			//�v���C���[�Q�����UI
	MarkerRenderer*					markerRenderer;		//�}�[�J�[
	DamageUI*						damageUI;			//�_���[�WUI

public:
	Tutorial();
	~Tutorial();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;
	void render3D(Camera* currentCamera);
	void renderUI();

	void tree8Reregister(Object* tmp);

#ifdef _DEBUG
	int playerSelect;
	virtual void createGUI() override;
#endif
};
