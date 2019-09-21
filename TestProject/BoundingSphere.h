//===================================================================================================================================
//�yBoundingSphere.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
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
	LPD3DXMESH		mesh;		//���b�V��
public:
	//Method
	BoundingSphere();
	~BoundingSphere();
	void initialize(LPDIRECT3DDEVICE9 device,D3DXVECTOR3* position, LPD3DXMESH ownerMesh);
	void initialize(LPDIRECT3DDEVICE9 device, float _raidus);
	void render(LPDIRECT3DDEVICE9 device,D3DXMATRIX owner);
	bool collide(D3DXVECTOR3 targetCenter,float targetRadius, D3DXMATRIX ownerMatrix, D3DXMATRIX targetMatrix);

	//setter

	//getter
	float getRadius();
	D3DXVECTOR3 getCenter();
	LPD3DXMESH	getMesh();
};