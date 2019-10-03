//===================================================================================================================================
//�yInstancingBillboard.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/27
// [�X�V��]2019/09/28
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Object.h"
#include "LinkedList.h"

//===================================================================================================================================
//�y�C���X�^���V���O�r���{�[�h���_�\���́z
//===================================================================================================================================
namespace InstancingBillboardNS {
	//===================================================================================================================================
	//�y�C���X�^���V���O�r���{�[�h���_�\���́z
	//===================================================================================================================================
	struct Vertex
	{
		D3DXVECTOR2 coord;
		D3DXVECTOR2 uv;
	};

	//===================================================================================================================================
	//�y�C���X�^���X�N���X�z
	//===================================================================================================================================
	class Instance
	{
	public:
		D3DXVECTOR3 position;
		D3DXVECTOR3 rotation;
		D3DXVECTOR3 scale;
		D3DXCOLOR color;
		D3DXVECTOR2 uv;

		float existenceTimer;
		Instance() {
			ZeroMemory(this, sizeof(Instance));
			existenceTimer = 1.0f;
		}
		void update(float frameTime)
		{

		}

	};
	//===================================================================================================================================
	//�y�C���X�^���X���X�g�N���X�z
	//===================================================================================================================================
	class InstanceList:public LinkedList<InstancingBillboardNS::Instance>
	{
	};
}


//===================================================================================================================================
//�y�C���X�^���V���O�r���{�[�h�N���X�z
//===================================================================================================================================
class InstancingBillboard : public Base
{
protected:
	LPDIRECT3DDEVICE9 device;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;
	LPDIRECT3DVERTEXBUFFER9 positionBuffer;
	LPDIRECT3DVERTEXBUFFER9 colorBuffer;
	IDirect3DVertexDeclaration9* declation;
	LPD3DXEFFECT effect;

	D3DXVECTOR3* position;
	D3DXCOLOR* color;
	LPDIRECT3DTEXTURE9 texture;

	InstancingBillboardNS::InstanceList* instanceList;

	bool onRender;

public:
	InstancingBillboard();
	~InstancingBillboard();

	virtual HRESULT initialize(LPD3DXEFFECT effect,LPDIRECT3DTEXTURE9 texture);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void offRender();

	void update(float frameTime);

	//�`�悷��C���X�^���X�𐶐�
	void generateInstance(InstancingBillboardNS::Instance newInstance);
	//�`�悷��C���X�^���X���폜
	void deleteInstance(int instanceNo);

	//�ʒu�����X�V����
	void updatePosition();

	//�`�搔�̃J���[����ݒ肷��
	void updateColor();

	//getter
	int getInstanceNum();									//�C���X�^���X�̐����擾����
	InstancingBillboardNS::InstanceList getList();	//���X�g���擾����
};
