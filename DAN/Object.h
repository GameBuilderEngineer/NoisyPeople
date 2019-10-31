//===================================================================================================================================
//�yObject.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/23
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
#include "Animation.h"
#include "LinearTreeCell.h"

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
	D3DXQUATERNION		quaternion;				//��]
	D3DXVECTOR3			scale;					//�X�P�[��
	float				radius;					//�Փ˔��a
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

	//�^�C�}�[
	float				existenceTimer;			//���ݎ���

	//�I�u�W�F�N�g�c���[�֏������邽�߂̃C���^�[�t�F�[�X�N���X
	ObjectTree<Object>	treeCell;		//�؋��

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
	D3DXMATRIX*				getMatrixWorld()								{ return &matrixWorld; }	
	D3DXVECTOR3*			getPosition()									{ return &position; };
	D3DXQUATERNION			getQuaternion()									{ return quaternion; };
	float					getRadius()										{ return radius; }
	D3DXVECTOR3				getSpeed()										{ return speed; }
	D3DXVECTOR3				getAcceleration()								{ return acceleration; }
	D3DXVECTOR3				getGravity()									{ return gravity; };
	Ray*					getAxisX()										{ return &axisX; };
	Ray*					getAxisY()										{ return &axisY; };
	Ray*					getAxisZ()										{ return &axisZ; };
	Ray*					getReverseAxisX()								{ return &reverseAxisX; };
	Ray*					getReverseAxisY()								{ return &reverseAxisY; };
	Ray*					getReverseAxisZ()								{ return &reverseAxisZ; };
	Ray*					getGravityRay()									{ return &gravityRay; };
	bool					getActive()										{ return onActive; }
	float					getRight()										{ return position.x + radius;}
	float					getLeft()										{ return position.x - radius;}
	float					getTop()										{ return position.z + radius;}
	float					getBottom()										{ return position.z - radius;}
	float					getFront()										{ return position.z + radius;}
	float					getBack()										{ return position.z - radius;}
	D3DXVECTOR3				getMin()										{ return position - D3DXVECTOR3(radius,radius,radius);}
	D3DXVECTOR3				getMax()										{ return position + D3DXVECTOR3(radius,radius,radius);}

	//setter
	void					setSpeed(D3DXVECTOR3 _speed)					{ speed = _speed; }
	void					addSpeed(D3DXVECTOR3 add)						{ speed += add; }
	void					setPosition(D3DXVECTOR3 _position)				{ position = _position; }
	void					setQuaternion(D3DXQUATERNION _quaternion)		{ quaternion = _quaternion; }
	void					setAlpha(float value);							//���l�̐ݒ�
	void					setGravity(D3DXVECTOR3 source, float power);	//�d�͂̐ݒ�
	void					activation();									//�A�N�e�B�u��
	void					inActivation();									//��A�N�e�B�u��

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

