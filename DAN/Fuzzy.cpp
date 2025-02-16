//-----------------------------------------------------------------------------
// ファジー理論 [Fuzzy.cpp]
// Author：GP12A332 32 中込和輝
// 作成日：2019/6/23
//-----------------------------------------------------------------------------
#include "Fuzzy.h"

//=============================================================================
// メンバーシップ関数「上昇斜辺」／
//=============================================================================
float Fuzzy::grade(float value, float x0, float x1)
{
	if (value < x0)	 { return 0.0f; }
	if (value >= x1) { return 1.0f; }

	if (x0 <= value && value < x1)
	{
		return (value - x0) / (x1 - x0);
	}

	return -1.0f;
}


//=============================================================================
// メンバーシップ関数「下降斜辺」＼
//=============================================================================
float Fuzzy::reverseGrade(float value, float x0, float x1)
{
	float result = NOT(grade(value, x0, x1));

	if (result != 2.0f/* 1.0f - (-1.0f) */)
	{
		return result;
	}

	return -1.0f;
}


//=============================================================================
// メンバーシップ関数「三角形」＿／＼＿
//=============================================================================
float Fuzzy::triangle(float value, float x0, float x1, float x2)
{
	if (value < x0)		{ return 0.0f; }
	if (value >= x2)	{ return 0.0f; }

	if (x0 <= value && value < x1)
	{
		return grade(value, x0, x1);
	}

	if (x1 <= value && value < x2)
	{
		return NOT(grade(value, x1, x2));
	}

	return -1.0f;
}


//=============================================================================
// メンバーシップ関数「台形」＿／￣＼＿
//=============================================================================
float Fuzzy::trapenoid(float value, float x0, float x1, float x2, float x3)
{
	if (value < x0)	 { return 0.0f; }

	if (x0 <= value && value < x1)
	{
		return grade(value, x0, x1);
	}

	if (x1 <= value && value < x2) { return 1.0f; }

	if (x2 <= value && value < x3)
	{
		return NOT(grade(value, x2, x3));
	}

	if (value >= x3) { return 0.0f; }

	return -1.0f;
}


//=============================================================================
// ヘッジ関数「とても強く」
//=============================================================================
float Fuzzy::hedgeVery(float a)
{
	return powf(a, 2.0f);
}


//=============================================================================
// ヘッジ関数「とても弱く」
//=============================================================================
float Fuzzy::hedgeNotVery(float a)
{ 
	return powf(a, 0.5f);
}


//=============================================================================
// ファジーAND演算
//=============================================================================
float Fuzzy::AND(float a, float b) 
{
	return fminf(a, b);
}


//=============================================================================
// ファジーOR演算
//=============================================================================
float Fuzzy::OR(float a, float b)
{
	return fmaxf(a, b);
}


//=============================================================================
// ファジーNOT演算
//=============================================================================

float Fuzzy::NOT(float a)
{
	return 1.0f - a;
}
