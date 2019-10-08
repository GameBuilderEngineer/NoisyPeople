//===================================================================================================================================
//�yInstancingBillboard.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/27
// [�X�V��]2019/10/08
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
		D3DXVECTOR3		position;	//�|�W�V����
		D3DXVECTOR3		rotation;	//��]
		D3DXVECTOR3		scale;		//�T�C�Y
		D3DXVECTOR3		speed;		//�T�C�Y
		D3DXVECTOR2		uv;			//UV
		D3DXCOLOR			color;		//�J���[
		float existenceTimer;				//��������

		Instance() {
			ZeroMemory(this, sizeof(Instance));
			existenceTimer = 1.0f;
		}
		void update(float frameTime)
		{
			existenceTimer -= frameTime;
			if (existenceTimer <= 0)return;
			position += speed*frameTime;
			if (position.x > 500)position.x = -500;
			else if(position.x < -500)position.x = 500;
			if (position.y > 500)position.y = -500;
			else if(position.y < -500)position.y = 500;
			if (position.z > 500)position.z = -500;
			else if(position.z < -500)position.z = 500;
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
	//Data
	LPDIRECT3DDEVICE9							device;					//DirectX9�f�o�C�X
	IDirect3DVertexDeclaration9*				declation;				//���_�錾
	LPD3DXEFFECT									effect;					//�V�F�[�_�[�G�t�F�N�g
	bool													onRender;				//�`��t���O
	bool													didGenerate;			//�����t���O
	bool													didDelete;				//�폜�t���O

	//�Z�b�g�X�g���[���p�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9				vertexBuffer;			//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9				indexBuffer;			//�C���f�b�N�X�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9				positionBuffer;		//�ʒu�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9				colorBuffer;			//�J���[�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9				uvBuffer;				//UV�o�b�t�@

	//copy�p�z��
	D3DXVECTOR3*									position;				//�ʒu�z��
	D3DXCOLOR*									color;					//�J���[�z��
	D3DXVECTOR2*									uv;						//UV�z��

	//���ʃe�N�X�`��
	LPDIRECT3DTEXTURE9						texture;					//�e�N�X�`���|�C���^
	float													textureHight;			//�e�N�X�`���̍���
	float													textureWidth;			//�e�N�X�`���̕�

	//�C���X�^���X���X�g
	InstancingBillboardNS::InstanceList*	instanceList;			//�C���X�^���X���X�g

public:
	//Method
	InstancingBillboard();
	~InstancingBillboard();

	//��{�֐�
	virtual HRESULT initialize(LPD3DXEFFECT effect,LPDIRECT3DTEXTURE9 texture);							//������
	void update(float frameTime);																										//�X�V
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);	//�`��

	//�`����I�t�ɂ���
	void offRender();

	//���X�g����
	void generateInstance(InstancingBillboardNS::Instance newInstance);		//�`�悷��C���X�^���X�𐶐�
	void deleteInstance(int instanceNo);														//�`�悷��C���X�^���X���폜
	void updateAccessList();																			//�����_���A�N�Z�X�p�z��̍X�V�F�C���X�^���X�̐����܂��͍폜���s�������ɍX�V���s���B

	//�e�o�b�t�@�l�X�V
	void updatePosition();																				//�ʒu�o�b�t�@���X�V����
	void updateUV();																					//UV�o�b�t�@���X�V����
	void updateColor();																				//�J���[�o�b�t�@���X�V����

	//�o�b�t�@�̃��\�[�X�X�V
	void updateBuffer();
	//copy�p�z��̃������X�V
	void updateArray();

	//getter
	int getInstanceNum();																			//�C���X�^���X�̐����擾����
	InstancingBillboardNS::InstanceList getList();											//���X�g���擾����

};
