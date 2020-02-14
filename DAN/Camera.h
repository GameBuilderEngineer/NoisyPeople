//===================================================================================================================================
//�yCamera.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/12/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Window.h"
#include "Direct3D9.h"
#include "Ray.h"
#include "Object.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace cameraNS {
	const float LIMIT_BOTTOM_Y = -3.0f;
	const float LIMIT_TOP_Y = 7.0f;
	const float INIT_NEAR_Z = 0.1f;
	const float INIT_FAR_Z = 30000.0f;

	enum {
		ROTATION_TOP_LIMIT		= 0x00000001,
		ROTATION_BOTTOM_LIMIT	= 0x00000002,
	};

	const int ALL_LIMIT = ROTATION_TOP_LIMIT | ROTATION_BOTTOM_LIMIT;
}

//===================================================================================================================================
//�y�J�����N���X�z
//===================================================================================================================================
class Camera : public Base
{
private:
public:
	//Data
	Direct3D9* d3d;

	float				fieldOfView;				//����p
	float				aspect;						//��ʏc����
	float				nearZ;						//���F�ߋ���
	float				farZ;						//���F������
	DWORD				windowWidth;
	DWORD				windowHeight;
	bool				onGaze;						//=true �������胂�[�h
	int					limit;						//�����t���O
	float				limitValueRotaionTop;		//��������
	float				limitValueRotaionBottom;	//��������
	D3DXVECTOR3			position;					//�J�����ʒu
	D3DXVECTOR3			gazePosition;				//����
	D3DXVECTOR3			relativeGaze;				//�������Βl
	D3DXVECTOR3			upVector;					//����x�N�g��
	D3DXQUATERNION		posture;					//�p���N�H�[�^�j�I��
	D3DXMATRIX			world;						//�p������p�s��
	D3DXQUATERNION		relativeQuaternion;			//�^�[�Q�b�g�ɑ΂��鑊�Έʒu�x�N�g��
	float				relativeDistance;			//���Έʒu�x�N�g������
	float				betweenGaze;				//���Έʒu�x�N�g���Œ苗��
	D3DXVECTOR3*		target;						//�����^�[�Q�b�g�ʒu�|�C���^
	D3DXVECTOR3*		targetX;					//�����^�[�Q�b�gX�����|�C���^
	D3DXVECTOR3*		targetY;					//�����^�[�Q�b�gY�����|�C���^
	D3DXVECTOR3*		targetZ;					//�����^�[�Q�b�gZ�����|�C���^
	D3DXMATRIX			view;						//�r���[�}�g���b�N�X
	D3DXMATRIX			projection;					//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			ortho;						//���ˉe�s��

	//Method
	Camera();
	~Camera();

	//��{�֐�
	HRESULT initialize(DWORD _windowWidth, DWORD _windowHeight);
	void update();
	void renderReady();
	virtual void outputGUI();

	//�Փ˕␳
	bool insetCorrection(LPD3DXMESH mesh, D3DXMATRIX matrix);	//�߂荞�ݕ␳
	bool sphereCollide(D3DXVECTOR3 position,float radius);		//������
	bool rayCollide(LPD3DXMESH mesh, D3DXMATRIX matrix);		//���C����

	//����[limit]
	void enableLimit(int limitParameter);				//�����̗L����
	void disableLimit(int limitParameter);				//�����̖�����
	void setLimitRotationTop(float value);				//��]���������
	void setLimitRotationBottom(float value);			//��]����������

	//����[operation]
	void rotation(D3DXVECTOR3 axis, float angle);
	void lockOn(D3DXVECTOR3 lockOnTarget,float frameTime);

	//�^�C�g������ �^�[�Q�b�g�ɒǏ]
	void GetViewMatrix(D3DXMATRIX* viewOut, D3DXMATRIX* world);

	//�����_�ԋ����̕␳
	void setGazeDistance(float newValue);
	//���ˉe�s����X�V����
	void updateOrtho();

	//getter
	D3DXVECTOR3 getDirectionX() { 
		D3DXVECTOR3 result;
		D3DXVec3Normalize(&result, &(gazePosition- position));
		D3DXVec3Cross(&result, &upVector,&result);
		D3DXVec3Normalize(&result, &result);
		return result;}
	D3DXVECTOR3 getDirectionY() {
		return upVector;}
	D3DXVECTOR3 getDirectionZ() {
		D3DXVECTOR3 result;
		D3DXVec3Normalize(&result, &(gazePosition- position));
		return result;}
	D3DXVECTOR3 getHorizontalAxis();
	D3DXVECTOR3 getAxisZ();
	D3DXVECTOR3 getAxisY();

	//setter
	void setAspect(DWORD _windowWidth, DWORD _windowHeight);
	void setFieldOfView(float value);
	void setPosition(D3DXVECTOR3 _position) { position = _position; };
	void setGaze(D3DXVECTOR3 _gazePosition) { gazePosition = _gazePosition; };
	void setUpVector(D3DXVECTOR3 _upVector) { upVector = _upVector; };
	void setRelative(D3DXQUATERNION _relativeQuaternion) {relativeQuaternion = _relativeQuaternion; };
	void setRelative(D3DXVECTOR3 relativeVector) {
		relativeQuaternion.x = relativeVector.x;
		relativeQuaternion.y = relativeVector.y;
		relativeQuaternion.z = relativeVector.z;
		relativeQuaternion.w = 1.0f;};
	void setRelativeLength(float length) { relativeDistance = length; }
	void setTarget(D3DXVECTOR3* _target) { target = _target; }
	void setTargetX(D3DXVECTOR3* targetAxisX) { targetX = targetAxisX; }
	void setTargetY(D3DXVECTOR3* targetAxisY) { targetY = targetAxisY; }
	void setTargetZ(D3DXVECTOR3* targetAxisZ) { targetZ = targetAxisZ; }
	void setRelativeGaze(D3DXVECTOR3 _relativeGaze) { relativeGaze = _relativeGaze;}
	HRESULT setViewProjection();
	void setNearZ(float value);
	void setFarZ(float value);

};