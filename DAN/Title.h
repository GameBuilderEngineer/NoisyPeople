//===================================================================================================================================
//�yTitle.h�z
// [�쐬��]HAL����GP12A332 25 ���� �W��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/17
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
#include "TestEffect.h"
#include "Sound.h"
#include "SoundBase.h"
#include "UtilityFunction.h"
//#include "SceneEffect.h"
#include "TitleUI.h"

#include "Object.h"
#include "StaticMeshRenderer.h"

#include "WaveBall.h"
#include "Sky.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace titleNS
{
	enum TRANSITION_SCENE_LIST
	{
		GAME,
		CREDIT,
		OPERATION,
		TUTORIAL,
		NUM_TRANSITION_SCENE
	};
	
	enum PHOTOGRAPH_LIST
	{
		CAMERA0,
		CAMERA1,
		CAMERA2,
		CAMERA3,
		CAMERA4,
		CAMERA5,
		MAX_CAMERA
	};

	const float TITLE_CAMERA_SPEED = 1.0f;
	//const D3DXVECTOR3 firstEndPoint()

}

//===================================================================================================================================
//�y�^�C�g���V�[���N���X�z
//===================================================================================================================================
class Title : public AbstractScene
{
private:


	TestEffect* testEffect;
	WaveBall *waveBall;
	
	//�t�B�[���h
	Object*					titleField;
	StaticMeshRenderer*		titleFieldRenderer;	
	//�X�J�C�h�[��
	Sky* sky;
	//�^�[�Q�b�g�I�u�W�F�N�g
	Object* target;

	TitleUI titleUI;										//�^�C�g��UI
	int selectStateMemory;


	int stateCamera;						//�J�����X�e�[�^�X
	float frameDegree = 1.0f / 10.0f;		//�������x
	float inputDegree = 2.0f;				//���͑��x
	
	float moveTime;				//�I�_�܂ł̎���
	float moveTimer;			//�ړ��^�C�}�[
	float moveTime2;				//�I�_�܂ł̎���
	float moveTimer2;			//�ړ��^�C�}�[
	float moveTime3;				//�I�_�܂ł̎���
	float moveTimer3;			//�ړ��^�C�}�[
	float moveTime4;				//�I�_�܂ł̎���
	float moveTimer4;			//�ړ��^�C�}�[
	D3DXVECTOR3 startPos;		//���[�v�n�_
	
	float rate;
	float rateY;
	float rateX;
	float degreeY;
	float degreeX;
	float degreeTimer = 0.0f;
	float degreeTime;

	//D3DXVECTOR3 P0_1;
	//D3DXVECTOR3 P1_2;



	//�J�����̎��擾�ϐ�
	D3DXVECTOR3 cameraAxisZ;
	D3DXVECTOR3 fixedAxisZ;
	D3DXVECTOR3 cameraAxisX;
	D3DXVECTOR3 cameraAxisY;
	D3DXVECTOR3 fixedAxisX;
	

public:

	Title(void);
	~Title(void);
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	void updateInput(void);
	virtual void render() override;
	void render3D(Camera* _currentCamera);
	void render2D();
	virtual void collisions(void) override;
	virtual void AI(void) override;

	D3DXVECTOR3 BezierCurve(D3DXVECTOR3 startpoint, D3DXVECTOR3 curvepoint, D3DXVECTOR3 endpoint, float rate);

	float tmpVolume = 0;
#ifdef _DEBUG
	virtual void createGUI() override;

#endif
};