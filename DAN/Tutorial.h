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
#include "tutorialPlane.h"
#include "Sky.h"
#include "Timer.h"
#include "movep.h"
#include "TutorialUI.h"

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
	const D3DXVECTOR3 PLAYER_P1_POSITION = D3DXVECTOR3(-20, 100, -45);
	const D3DXVECTOR3 PLAYER_P2_POSITION = D3DXVECTOR3(20, 100, -45);

	//�G�l�~�[�����ʒu
	const D3DXVECTOR3 ENEMY_POSTITION = D3DXVECTOR3(0, 20, 0);

	//�J�������Έʒu
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f, 5.0f, 5.0f, 0.0f);

	//�J�������Β����ʒu
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0, 2.0f, 0);
}

//===================================================================================================================================
//�y�`���[�g���A���V�[���N���X�z
//===================================================================================================================================
class Tutorial : public AbstractScene
{
private:
	//�`���[�g���A��2D
	TutorialTex tutorialTex;

	Linear8TreeManager<Object>*		linear8TreeManager;	//���`�W���؊Ǘ��N���X
	ObjectTree<Object>*				objectTreeArray;		//�I�u�W�F�N�g�c���[
	DWORD							collisionNum;		//�Փ˔����
	CollisionList<Object>*			collisionList;		//�Փ˔��胊�X�g

	Player*							player;				//�v���C���[
	StaticMeshRenderer*				maleRenderer;		//�j�v���C���[�����_���[
	StaticMeshRenderer*				femaleRenderer;		//���v���C���[�����_���[
	Object*							testField;			//�t�B�[���h
	StaticMeshRenderer*				testFieldRenderer;	//�t�B�[���h�����_���[

	Sky*								sky;					//�X�J�C�h�[��

	StaticMeshRenderer*				MoveP;
	MOVEP*							MoveP1;

	EnemyManager*					enemyManager;		// �G�l�~�[�}�l�[�W���[

	//UI
	TutorialUI *tutorialUI;

	//�f�B�X�v���C�v���[��
	TutorialPlane **plane;

	//�^�C�}�[
	Timer *timer;

	//test(sai[del])
	enemyNS::EnemyData* enemyData;

	//�i��
	int planeStep[gameMasterNS::PLAYER_NUM];
	int step[gameMasterNS::PLAYER_NUM];

public:
	Tutorial();
	~Tutorial();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;
	void render3D(Camera currentCamera, int playerID);
	void renderUI();

	void tree8Reregister(Object* tmp);

#ifdef _DEBUG
	int playerSelect;
	virtual void createGUI() override;
#endif
};
