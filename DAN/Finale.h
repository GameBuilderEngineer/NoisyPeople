//===================================================================================================================================
//�yFinale.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/12/12
// [�X�V��]2019/12/12
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
#include "UtilityFunction.h"
#include "Object.h"
#include "StaticMeshRenderer.h"
#include "Sky.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace FinaleNS
{
	//�v���C���[�����ʒu
	const D3DXVECTOR3 PLAYER_POSITION = D3DXVECTOR3(0, 100, 0);

	//�J�������Έʒu
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION = D3DXQUATERNION(0.0f, 0.0f, -1.5f, 0.0f);

	//�J�������Β����ʒu
	const D3DXVECTOR3 CAMERA_RELATIVE_GAZE = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===================================================================================================================================
//�y�t�B�i�[���V�[���N���X�z
//===================================================================================================================================
class Finale : public AbstractScene
{
private:

	//�t�B�[���h
	Object*					finaleField;
	StaticMeshRenderer*		finaleFieldRenderer;
	//�X�J�C�h�[��
	Sky* sky;
	//�^�[�Q�b�g�I�u�W�F�N�g
	Object* target;

	int stateCamera;						//�J�����X�e�[�^�X
	float frameDegree = 1.0f / 10.0f;		//�������x
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

	D3DXQUATERNION cameraRelativeQuaternion;

public:
	Finale();
	~Finale();
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	virtual void render() override;
	void render3D(Camera _currentCamera);
	void render2D();
	virtual void collisions(void) override;
	virtual void AI(void) override;
#ifdef _DEBUG
	virtual void createGUI() override;
#endif

};

