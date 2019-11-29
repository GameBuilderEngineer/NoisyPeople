//-----------------------------------------------------------------------------
// �t�@�W�[���_ [Fuzzy.h]
// Author�FGP12A332 32 �����a�P
// �쐬���F2019/6/23
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
struct FuzzyDistance
{
	float	nearSet;
	float	normalSet;
	float	farSet;
};

class Fuzzy
{
public:
	// �����o�[�V�b�v�֐��u�㏸�ΕӁv�^
	float grade(float value, float x0, float x1);

	// �����o�[�V�b�v�֐��u���~�ΕӁv�_
	float reverseGrade(float value, float x0, float x1);

	// �����o�[�V�b�v�֐��u�O�p�`�v�Q�^�_�Q
	float triangle(float value, float x0, float x1, float x2);

	// �����o�[�V�b�v�֐��u��`�v�Q�^�P�_�Q
	float trapenoid(float value, float x0, float x1, float x2, float x3);

	// �w�b�W�֐��u�ƂĂ������v
	float hedgeVery(float a);

	// �w�b�W�֐��u�ƂĂ��キ�v
	float hedgeNotVery(float a);

	// �t�@�W�[AND���Z
	float AND(float a, float b);

	// �t�@�W�[OR���Z
	float OR(float a, float b);

	// �t�@�W�[NOT���Z
	float NOT(float a);
};
