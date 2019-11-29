//===================================================================================================================================
//�yBoundingSphere.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/24
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//�y�o�E���f�B���O�X�t�B�A�N���X�F�R���C�_�[�z
//===================================================================================================================================
class BoundingSphere : public Base
{
private:
	//Data
	D3DXVECTOR3*	position;	//�ʒu
	float			radius;		//���a
	D3DXVECTOR3		center;		//���S
	float			scale;		//�X�P�[��
	LPD3DXMESH		mesh;		//���b�V��

public:
	//Method
	BoundingSphere();
	BoundingSphere(D3DXVECTOR3*	position,float radius);
	~BoundingSphere();
	void initialize(D3DXVECTOR3* position, LPD3DXMESH ownerMesh);
	void initialize(D3DXVECTOR3* position, float _raidus);
	void render(D3DXMATRIX owner);
	void render();
	bool collide(D3DXVECTOR3 targetCenter,float targetRadius, D3DXMATRIX ownerMatrix, D3DXMATRIX targetMatrix);

	//setter
	void setScale(float value);

	//getter
	float getRadius();
	D3DXVECTOR3 getCenter();
	LPD3DXMESH	getMesh();
};