//-----------------------------------------------------------------------------
// �p���[�A�b�v�N���X [Powerup.h]
// Author : �V���@���m
// �쐬�J�n�� : 2020/  01/14  
//-----------------------------------------------------------------------------
#pragma once
#include "Item.h"
class Powerup:public Item
{
	bool flag;
	Ray ray;
	D3DXVECTOR3 fieldFacePosition;
public:
	Powerup(StaticMesh* _staticMesh, itemNS::ItemData _itemData);
	~Powerup();

	static void initialize(StaticMesh* staticMesh);
	void initialize();
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
};

