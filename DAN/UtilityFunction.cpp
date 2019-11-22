//===================================================================================================================================
//�yUtiltyFunction.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/22
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "UtilityFunction.h"

//===================================================================================================================================
//using�錾
//===================================================================================================================================
using namespace utility;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
UtilityFunction::UtilityFunction()
{

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
UtilityFunction::~UtilityFunction()
{

}

//===================================================================================================================================
//�y�ɍ��W���璼�����W�̂R�������W�����߂�z
//[����]
//float radius	�F���a			[��`��] 0<= radius <= ��
//float theta	�F�Ɋp(�V���p)  [��`��] 0<= theta  <= ��
//float phi		�F���ʊp(�Ίp)  [��`��] 0<= phi    <  2��
//[�ߒl]�������W��3�������W
//===================================================================================================================================
D3DXVECTOR3 UtilityFunction::fromTransformationPolar3D(float _radius,float _theta,float _phi)
{
	float radius = max(0, _radius);
	float theta = _theta;//wrap(_theta,0.0f,(float)(2.0f*D3DX_PI));
	float phi = (theta>D3DX_PI? -1 : 1) *  wrap(_phi,0.0f,(float)(2.0f*D3DX_PI));
	D3DXVECTOR3 result;
	result.x = 
		radius * sinf(theta) * cosf(phi);
	result.y = 
		radius * cosf(theta);
	result.z = 
		radius * sinf(theta) * sinf(phi);
	return result;
}

//===================================================================================================================================
//�y�������W����ɍ��W�̂R�l�����߂�z
//[����]
//float radius	�F���a			[��`��] 0<= radius <= ��
//float theta	�F�Ɋp(�V���p)  [��`��] 0<= theta  <= ��
//float phi		�F���ʊp(�Ίp)  [��`��] 0<= phi    <  2��
//[�ߒl]�������W��3�������W
//===================================================================================================================================
Polar3D UtilityFunction::fromRectangular3D(D3DXVECTOR3 coordinate)
{
	float x = coordinate.x;
	float y = coordinate.y;
	float z = coordinate.z;
	Polar3D result;
	result.radius = sqrtf(x*x+y*y+z*z);
	result.theta = (result.radius != 0) ? acosf(y/result.radius) : 0;
	result.phi = (x != 0) ? atanf(z/x) : 0;



	if (x > 0 && z > 0) {}						//++
	else if(x<0&&z>0)result.phi += D3DX_PI;		//-+
	else if(x<0&&z<=0)result.phi += D3DX_PI;	//--
	else if(x>0&&z<=0)result.phi += 2*D3DX_PI;	//+-

	return result;
}

//===================================================================================================================================
//�y1����Ԋ֐��z
//===================================================================================================================================
float UtilityFunction::lerp(float start,float end, float rate)
{
	float result;
	clamp(rate, 0.0f, 1.0f);
	result = start * (1 - rate) + end * rate;
	return result;
}

//===================================================================================================================================
//�y(int�^)���b�v�֐��z
//===================================================================================================================================
int UtilityFunction::wrap(int x, int low, int high)
{
	assert(low < high);
	const int n = (x - low) % (high - low);
	return (n >= 0) ? (n + low) : (n + high);
}

//===================================================================================================================================
//�y(float�^)���b�v�֐��z
//===================================================================================================================================
float UtilityFunction::wrap(float x, float low, float high)
{
	assert(low < high);
	const float n = std::fmod(x - low,high - low);
	return (n >= 0) ? (n + low) : (n + high);
}

//===================================================================================================================================
//�y������J�����O�z
//===================================================================================================================================
bool UtilityFunction::culling(D3DXVECTOR3 center, float radius, D3DXMATRIX view, float angle, float  nearClip, float farClip, float aspect)
{
	D3DXPLANE VFLeftPlane, VFRightPlane, VFTopPlane, VFBottomPlane;
	//�܂��A�W�I���g���̈ʒu�x�N�g�������[���h����r���[��Ԃɕϊ�
	D3DXVECTOR3 position;
	D3DXVec3TransformCoord(&position, &center, &view);

	//���E�A�㉺�̕��ʂ��v�Z
	{
		D3DXVECTOR3 p1, p2, p3;
		//����
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -farClip * ((FLOAT)tan(angle / 2)*aspect);
		p2.y = -farClip * (FLOAT)tan(angle / 2);
		p2.z = farClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFLeftPlane, &p1, &p2, &p3);
		//�E��
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = farClip * ((FLOAT)tan(angle / 2)*aspect);
		p2.y = farClip * (FLOAT)tan(angle / 2);
		p2.z = farClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFRightPlane, &p1, &p2, &p3);
		//���
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -farClip * ((FLOAT)tan(angle / 2)*aspect);
		p2.y = farClip * (FLOAT)tan(angle / 2);
		p2.z = farClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFTopPlane, &p1, &p2, &p3);
		//����
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = farClip * ((FLOAT)tan(angle / 2)*aspect);
		p2.y = -farClip * (FLOAT)tan(angle / 2);
		p2.z = farClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFBottomPlane, &p1, &p2, &p3);
	}

	//6�̕��ʂƃW�I���g�����E�����`�F�b�N
	{
		//�j�A�N���b�v�ʂɂ���
		if ((position.z + radius) < nearClip)
		{
			return false;
		}
		//�t�@�[�N���b�v�ʂɂ���
		if ((position.z - radius) > farClip)
		{
			return false;
		}
		//���N���b�v�ʂɂ���
		FLOAT Distance = (position.x * VFLeftPlane.a) + (position.z * VFLeftPlane.c);
		if (Distance > radius)
		{
			return false;
		}
		//�E�N���b�v�ʂɂ���
		Distance = (position.x * VFRightPlane.a) + (position.z * VFRightPlane.c);
		if (Distance > radius)
		{
			return false;
		}
		//��N���b�v�ʂɂ���
		Distance = (position.y * VFTopPlane.b) + (position.z * VFTopPlane.c);
		if (Distance > radius)
		{
			return false;
		}
		//���N���b�v�ʂɂ���
		Distance = (position.y * VFBottomPlane.b) + (position.z * VFBottomPlane.c);
		if (Distance > radius)
		{
			return false;
		}
	}

	return true;

}
