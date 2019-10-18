//===================================================================================================================================
//�yGame.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/05
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include <vector>
#include "AbstractScene.h"
#include "Player.h"
#include "StaticMeshObject.h"
#include "InstancingBillboard.h"
#include "Sound.h"
#include "SoundBase.h"
#include "EnemyTools.h"

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
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f);

	//�J�������Β����ʒu
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0,0,0);

	//const int NUM_SAMPLE = 1000;

}

//===================================================================================================================================
//�y�Q�[���V�[���N���X�z
//===================================================================================================================================
class Game : public AbstractScene
{
private:

	//Text text;					//Sample
	//Text text2;					//Sample
	//PointSprite pointSprite;		//Sample
	//InstancingBillboard plane;	//Sample
	//Object testObject;			//Sample
	//Object testCube;				//Sample

	//�v���C���[
	Player *player;

	//�t�B�[���h
	StaticMeshObject* testField;	

	//�C���X�^���V���O�r���{�[�h�e�X�g
	InstancingBillboard instancingBillboardTest;

	//�Đ��p�����[�^
	PLAY_PARAMETERS playParameters[3];


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

#ifdef _DEBUG
	//�G�l�~�[�c�[��
	ENEMY_TOOLS *enemyTools;

	virtual void createGUI() override;
#endif
};