//-----------------------------------------------------------------------------
// �p���[�A�b�v�N���X [Powerup.h]
// Author : �V���@���m
// �쐬�J�n�� : 2020/ 01/14  
//-----------------------------------------------------------------------------
#include "Powerup.h"
#include "ImguiManager.h"
using namespace itemNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Powerup::Powerup(StaticMesh* _staticMesh, ItemData _itemData): Item(_staticMesh, _itemData)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Powerup::~Powerup()
{
}

//������
void Powerup::initialize()
{
	flag = true;
	ray.initialize(position, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{
		fieldFacePosition = position + ray.direction * ray.distance;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Powerup::update(float frameTime)
{
	// 
	Item::update(frameTime);
	if (flag == false) { return; }

	if (ray.rayIntersect(attractorMesh, *attractorMatrix))
	{

		if (position.y > fieldFacePosition.y)
		{
			position += D3DXVECTOR3(0.0f, -1.0f, 0.0f) * 30.0f * frameTime;
		}
		else
		{
			position = fieldFacePosition;
			flag = false;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Powerup::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{

}