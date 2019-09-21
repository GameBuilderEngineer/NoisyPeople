//===================================================================================================================================
//�yUtiltyFunction.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/08/07
// [�X�V��]2019/08/07
//===================================================================================================================================
#pragma once
#include <stdio.h>
#include <crtdbg.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>
#include <cmath>
#include <iostream>
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
//�x����\��
#pragma warning(disable:4305)
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#pragma warning(disable:4111)

namespace utility
{
	struct Polar3D
	{
		float radius;	//���a			[��`��] 0<= raduys <= ��
		float theta;	//�Ɋp(�V���p)  [��`��] 0<= theta  <= ��
		float phi;		//���ʊp(�Ίp)  [��`��] 0<= phi    <  2��
	};
}

class UtilityFunction
{
public:
	UtilityFunction();
	~UtilityFunction();

	static int wrap(int x, int low, int high);
	static float wrap(float x, float low, float high);
	template<typename T>
	static T clamp(T x, T low, T high);

	//�y�ϊ��z�ɍ��W->�������W
	//Polar Coordinates Transformation
	static D3DXVECTOR3 fromTransformationPolar3D(float radius, float theta, float phi);
	//�y�ϊ��z�������W->�ɍ��W
	static utility::Polar3D fromRectangular3D(D3DXVECTOR3 coordinate);
	//1����Ԋ֐�
	static float lerp(float start, float end, float time);
};