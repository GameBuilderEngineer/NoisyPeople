//-----------------------------------------------------------------------------
// �A�i���O�c���[�N���X [Tree.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
#pragma once
#include"Tree.h"

//=============================================================================
//�N���X��`
//=============================================================================
class AnalogTree: public Tree
{
private:

public:
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// Getter

	// Setter
};
