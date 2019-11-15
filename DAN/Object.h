//===================================================================================================================================
//�yObject.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#include "Ray.h"
#include "StaticMeshLoader.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "LinearTreeCell.h"
#include "ObjectTypeList.h"
#include "DebugBox.h"
#include "BoundingSphere.h"


//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace objectNS {
	enum FILLMODE
	{
		SOLID		= (int)D3DFILL_SOLID,
		WIREFRAME	= (int)D3DFILL_WIREFRAME,
		POINT		= (int)D3DFILL_POINT,
	};

	void resetCounter();
}

//===================================================================================================================================
//�y�I�u�W�F�N�g�N���X�z
//===================================================================================================================================
class Object:public Base
{
public:
	//Data
	//�X�e�[�^�X�ϐ�
	static int			objectCounter;			//�I�u�W�F�N�g�J�E���^�[�FID�̊����Ɏg�p
	int					id;						//ID
	D3DXVECTOR3			position;				//�ʒu
	D3DXVECTOR3			center;					//���S�ʒu
	D3DXQUATERNION		quaternion;				//��]
	D3DXVECTOR3			scale;					//�X�P�[��
	float				radius;					//�Փ˔��a
	D3DXVECTOR3			size;					//�Փ˃T�C�Y(��/����/���s��)
	float				alpha;					//���ߒl

	//�ړ��n�ϐ�
	D3DXVECTOR3			speed;					//���x
	D3DXVECTOR3			acceleration;			//�����x
	D3DXVECTOR3			gravity;				//�d��


	//�e��t���O
	bool				onGravity;				//�d�͗L�����t���O
	bool				onActive;				//�A�N�e�B�u���t���O

	//����6��
	Ray					axisX;					//x��
	Ray					axisY;					//y��
	Ray					axisZ;					//z��
	Ray					reverseAxisX;			//-x��
	Ray					reverseAxisY;			//-y��
	Ray					reverseAxisZ;			//-z��

	//�d��Ray
	Ray					gravityRay;				//�d��Ray

	//�s��i�ʒu�E��]�E�X�P�[���E���[���h�j
	D3DXMATRIX			matrixPosition;			//�ʒu�s��
	D3DXMATRIX			matrixRotation;			//��]�s��
	D3DXMATRIX			matrixScale;			//�X�P�[���s��
	D3DXMATRIX			matrixWorld;			//���[���h�s��
	D3DXMATRIX			matrixCenter;			//���S���W�s��

	//�^�C�}�[
	float				existenceTimer;			//���ݎ���

	//�I�u�W�F�N�g�c���[�֏������邽�߂̃C���^�[�t�F�[�X�N���X
	ObjectTree<Object>	treeCell;				//�؋��

	//�Փ˔���C���X�^���X
	BoundingSphere* sphere;
	DebugBox* box;

public:
	//Method
	Object();
	virtual ~Object();
	
	//��{�֐�
	HRESULT initialize(D3DXVECTOR3* _position);
	void update();

	//�f�o�b�O�֌W�֐�
	void debugRender();				//�f�o�b�O�p���[�e�B���e�B�����_�[
	virtual void outputGUI();			//ImGUI��Object�̏����o��

	//getter
	D3DXMATRIX*				getMatrixWorld();
	D3DXVECTOR3*			getPosition();
	D3DXQUATERNION			getQuaternion();
	float					getRadius();
	D3DXVECTOR3				getSpeed();
	D3DXVECTOR3				getAcceleration();
	D3DXVECTOR3				getGravity();
	Ray*					getAxisX();
	Ray*					getAxisY();
	Ray*					getAxisZ();
	Ray*					getReverseAxisX();
	Ray*					getReverseAxisY();
	Ray*					getReverseAxisZ();
	Ray*					getGravityRay();
	bool					getActive();
	float					getRight();
	float					getLeft();
	float					getTop();
	float					getBottom();
	float					getFront();
	float					getBack();
	D3DXVECTOR3				getMin();
	D3DXVECTOR3				getMax();

	//setter
	void					setSpeed(D3DXVECTOR3 _speed);
	void					addSpeed(D3DXVECTOR3 add);
	void					setPosition(D3DXVECTOR3 _position);
	void					setQuaternion(D3DXQUATERNION _quaternion);
	void					setAlpha(float value);							//���l�̐ݒ�
	void					setGravity(D3DXVECTOR3 source, float power);	//�d�͂̐ݒ�
	void					activation();									//�A�N�e�B�u��
	void					inActivation();									//��A�N�e�B�u��
	void					setRadius(float value);
	void					setSize(D3DXVECTOR3 value);

	//�p������
	void postureControl(D3DXVECTOR3 currentDirection, D3DXVECTOR3 nextDirection,float t)
	{
		Base::postureControl(&quaternion,currentDirection, nextDirection, t);
	}

	//�C�ӎ����^��ԉ�]
	void anyAxisRotationSlerp(D3DXVECTOR3 axis, float radian,float t)
	{
		Base::anyAxisRotationSlerp(&quaternion, axis, radian, t);
	}
	
	//�C�ӎ���]
	void anyAxisRotation(D3DXVECTOR3 axis, float degree)
	{
		Base::anyAxisRotation(&quaternion, axis, degree);
	}
};

