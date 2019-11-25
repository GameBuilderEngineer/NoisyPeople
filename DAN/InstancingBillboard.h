//===================================================================================================================================
//�yInstancingBillboard.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/27
// [�X�V��]2019/10/28
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
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
		D3DXVECTOR2		scale;		//�T�C�Y
		D3DXVECTOR3		speed;		//�X�s�[�h
		D3DXVECTOR2		uv;			//UV
		D3DXCOLOR		color;		//�J���[
		float			lifeTimer;	//��������
		float			limitTime;	//�������E����

		Instance() {
			position				= D3DXVECTOR3(0.0f,0.0f,0.0f);
			rotation				= D3DXVECTOR3(0.0f,0.0f,0.0f);
			scale					= D3DXVECTOR2(1.0f,1.0f);
			speed					= D3DXVECTOR3(0.0f,0.0f,0.0f);
			uv						= D3DXVECTOR2(0.0f,0.0f);
			color					= D3DCOLOR_RGBA(255,255,255,255);
			lifeTimer				= 0.0f;
			limitTime				= 1.0f;
		}

		virtual void update(float frameTime)
		{
			lifeTimer += frameTime;
			if (lifeTimer >= limitTime)return;
		}

	};
	//===================================================================================================================================
	//�y�C���X�^���X���X�g�N���X�z
	//===================================================================================================================================
	class InstanceList:public LinkedList<InstancingBillboardNS::Instance*>
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
	LPDIRECT3DDEVICE9						device;					//DirectX9�f�o�C�X
	IDirect3DVertexDeclaration9*			declation;				//���_�錾
	LPD3DXEFFECT							effect;					//�V�F�[�_�[�G�t�F�N�g
	bool									onRender;				//�`��t���O
	bool									didGenerate;			//�����t���O
	bool									didDelete;				//�폜�t���O

	//�Z�b�g�X�g���[���p�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9					vertexBuffer;			//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9					indexBuffer;			//�C���f�b�N�X�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9					positionBuffer;			//�ʒu�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9					rotationBuffer;			//��]�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9					scaleBuffer;			//�X�P�[���o�b�t�@
	LPDIRECT3DVERTEXBUFFER9					colorBuffer;			//�J���[�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9					uvBuffer;				//UV�o�b�t�@

	//copy�p�z��
	D3DXVECTOR3*							position;				//�ʒu�z��
	D3DXVECTOR3*							rotation;				//��]�z��
	D3DXVECTOR2*							scale;					//�X�P�[���z��
	D3DXCOLOR*								color;					//�J���[�z��
	D3DXVECTOR2*							uv;						//UV�z��

	//���ʃe�N�X�`�����
	LPDIRECT3DTEXTURE9						texture;				//�e�N�X�`���|�C���^		�E�\�\���t
	float									unitU;					//�P��U�l�i��/�������j		�b
	float									unitV;					//�P��V�l�i����/�c�����j	�b
	int										divideU;				//U�l������					��
	int										divideV;				//V�l������					�u

	//�C���X�^���X���X�g
	InstancingBillboardNS::InstanceList*	instanceList;			//�C���X�^���X���X�g
	int										instanceNum=0;			//�C���X�^���X�̐�

public:
	//Method
	InstancingBillboard();
	virtual ~InstancingBillboard();

	//��{�֐�
	virtual HRESULT initialize(LPDIRECT3DTEXTURE9 texture,int divideU = 0,int divideV = 0);	//������
	virtual void update(float frameTime);													//�X�V
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);	//�`��

	//�`����I�t�ɂ���
	void offRender();

	//���X�g����
	void generateInstance(InstancingBillboardNS::Instance* newInstance);					//�`�悷��C���X�^���X�𐶐�
	void deleteInstance(int instanceNo);													//�`�悷��C���X�^���X���폜
	void updateAccessList();																//�����_���A�N�Z�X�p�z��̍X�V�F�C���X�^���X�̐����܂��͍폜���s�������ɍX�V���s���B

	//�e�o�b�t�@�l�X�V
	void updatePosition();																	//�ʒu�o�b�t�@���X�V����
	void updateRotation();																	//��]�o�b�t�@���X�V����
	void updateScale();																		//�X�P�[���o�b�t�@���X�V����
	void updateUV();																		//UV�o�b�t�@���X�V����
	void updateColor();																		//�J���[�o�b�t�@���X�V����

	//�o�b�t�@�̃��\�[�X�X�V
	void updateBuffer();
	//copy�p�z��̃������X�V
	void updateArray();

	//getter
	int getInstanceNum();																	//�C���X�^���X�̐����擾����
	InstancingBillboardNS::InstanceList getList();											//���X�g���擾����

};
