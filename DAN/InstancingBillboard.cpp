//===================================================================================================================================
//�yInstancingBillboard.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/27
// [�X�V��]2019/10/23
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "InstancingBillboard.h"
#include "Direct3D9.h"
#include "Input.h"
#include <time.h>

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
InstancingBillboard::InstancingBillboard()
{
	onRender			= false;
	didGenerate			= false;
	didDelete			= false;
	vertexBuffer		= NULL;
	indexBuffer			= NULL;
	positionBuffer		= NULL;
	colorBuffer			= NULL;
	uvBuffer			= NULL;
	declation			= NULL;
	effect				= NULL;
	position			= NULL;
	color				= NULL;
	uv					= NULL;
	texture				= NULL;
	device				= getDevice();
	srand((unsigned int)time(NULL));
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
InstancingBillboard::~InstancingBillboard()
{
	//SAFE_RELEASE
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(positionBuffer);
	SAFE_RELEASE(rotationBuffer);
	SAFE_RELEASE(scaleBuffer);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(uvBuffer);
	SAFE_RELEASE(declation);

	//SAFE_DELETE
	SAFE_DELETE_ARRAY(position);
	SAFE_DELETE_ARRAY(rotation);
	SAFE_DELETE_ARRAY(scale);
	SAFE_DELETE_ARRAY(color);
	SAFE_DELETE_ARRAY(uv);

	for (int i = 0; i < instanceNum; i++)
	{
		SAFE_DELETE(*instanceList->getValue(i));			//���X�g���̎��̂��폜	
	}
	instanceList->terminate();
	SAFE_DELETE(instanceList);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
HRESULT InstancingBillboard::initialize(LPD3DXEFFECT _effect, LPDIRECT3DTEXTURE9 _texture)
{
	//�C���X�^���V���O�r���{�[�h�̒��_��`
	InstancingBillboardNS::Vertex vertex[4] = {
		{D3DXVECTOR2(-1.0f,  1.0f),	D3DXVECTOR2(0.0f,0.0f)},
		{D3DXVECTOR2(1.0f,  1.0f),	D3DXVECTOR2(1.0f,0.0f)},
		{D3DXVECTOR2(-1.0f, -1.0f),	D3DXVECTOR2(0.0f,1.0f)},
		{D3DXVECTOR2(1.0f, -1.0f),	D3DXVECTOR2(1.0f,1.0f)},
	};

	//���_�o�b�t�@�̍쐬
	device->CreateVertexBuffer(sizeof(vertex), 0, 0, D3DPOOL_MANAGED, &vertexBuffer, 0);//�T�C�Y�m��
	copyVertexBuffer(sizeof(vertex), &vertex, vertexBuffer);

	WORD index[6] = { 
		0,1,2,
		2,1,3 
	};

	//�C���f�b�N�X�o�b�t�@�̍쐬
	device->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuffer, 0);
	void *p = 0;
	indexBuffer->Lock(0, 0, &p, 0);
	memcpy(p, index, sizeof(index));
	indexBuffer->Unlock();

	//���_�錾
	D3DVERTEXELEMENT9 vertexElement[] = {
		{ 0, 0,									D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },	//���_���W
		{ 0, sizeof(D3DXVECTOR2),				D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },	//UV
		{ 1, 0,									D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	1 },	//�ʒu
		{ 2, 0,									D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,			0 },	//�J���[
		{ 3, 0,									D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2 },	//����UV
		{ 4, 0,									D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3 },	//��]
		{ 5, 0,									D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4 },	//�X�P�[��
		D3DDECL_END()
	};
	device->CreateVertexDeclaration(vertexElement, &declation);

	//�V�F�[�_�[��ݒ�
	effect = _effect;
	
	//�e�N�X�`����ݒ�
	texture = _texture;

	//�C���X�^���X���X�g�̏�����
	instanceList = new InstancingBillboardNS::InstanceList();

	//�`����I���ɂ���
	onRender = true;

	return S_OK;
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void InstancingBillboard::update(float frameTime)
{

	for (int i = 0; i < instanceNum; i++)
	{
		(*instanceList->getValue(i))->update(frameTime);		//�X�V����
		deleteInstance(i);										//�폜����
	}

	//�폜�������͍쐬���s�����ꍇ�̃���������
	if (didDelete || didGenerate)
	{
		updateAccessList();		//���X�g�̍X�V
		updateBuffer();			//�o�b�t�@�̍X�V
		updateArray();			//�R�s�[�z��̍X�V
		didGenerate = false;
		didDelete = false;
	}
	
	//�l�̍X�V
	updatePosition();
	updateRotation();
	updateScale();
	updateColor();
	updateUV();

}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void InstancingBillboard::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (!onRender)return;
	int instanceNum = getInstanceNum();
	if (instanceNum <= 0)return;

	//Z�[�x�o�b�t�@
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);

	//���e�X�g
	//device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//device->SetRenderState(D3DRS_ALPHAREF, 0x00);

	// ���u�����h���s��
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//���Z�������s��
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	// ���f�X�e�B�l�[�V�����J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�C���X�^���X�錾
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA		| instanceNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(4, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(5, D3DSTREAMSOURCE_INSTANCEDATA		| 1);

	// ���_�錾��ʒm
	device->SetVertexDeclaration(declation);
	
	//�f�o�C�X�f�[�^�X�g���[���Ƀ��b�V���̊e�o�b�t�@���o�C���h
	device->SetStreamSource(0, vertexBuffer,	0, sizeof(InstancingBillboardNS::Vertex));		//���_�o�b�t�@
	device->SetStreamSource(1, positionBuffer,	0, sizeof(D3DXVECTOR3));								//�ʒu�o�b�t�@
	device->SetStreamSource(2, colorBuffer,		0, sizeof(D3DXCOLOR));									//�J���[�o�b�t�@
	device->SetStreamSource(3, uvBuffer,		0, sizeof(D3DXVECTOR2));								//UV�o�b�t�@
	device->SetStreamSource(4, rotationBuffer,	0, sizeof(D3DXVECTOR3));								//��]�o�b�t�@
	device->SetStreamSource(5, scaleBuffer,		0, sizeof(D3DXVECTOR2));								//�X�P�[���o�b�t�@

	//�C���f�b�N�X�o�b�t�@���Z�b�g
	device->SetIndices(indexBuffer);

	effect->SetTechnique("mainTechnique");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);
	effect->SetTexture("planeTexture", texture);

	effect->Begin(0, 0);
	effect->BeginPass(0);
		
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	effect->EndPass();
	effect->End();

	device->SetStreamSource(0, NULL, 0, NULL);
	device->SetStreamSource(1, NULL, 0, NULL);
	device->SetStreamSource(2, NULL, 0, NULL);
	device->SetStreamSource(3, NULL, 0, NULL);
	device->SetStreamSource(4, NULL, 0, NULL);
	device->SetStreamSource(5, NULL, 0, NULL);

	//��n��
	device->SetStreamSourceFreq(0, 0);
	device->SetStreamSourceFreq(1, 0);
	device->SetStreamSourceFreq(2, 0);
	device->SetStreamSourceFreq(3, 0);
	device->SetStreamSourceFreq(4, 0);
	device->SetStreamSourceFreq(5, 0);
	// ���u�����h��؂�
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//===================================================================================================================================
//�y�ʒu�o�b�t�@���X�V����z
//===================================================================================================================================
void InstancingBillboard::updatePosition()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		position[i] = (*instanceList->getValue(i))->position;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, position, positionBuffer);
}

//===================================================================================================================================
//�y��]�o�b�t�@���X�V����z
//===================================================================================================================================
void InstancingBillboard::updateRotation()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		rotation[i] = (*instanceList->getValue(i))->rotation;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, rotation, rotationBuffer);
}

//===================================================================================================================================
//�y�X�P�[���o�b�t�@���X�V����z
//===================================================================================================================================
void InstancingBillboard::updateScale()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		scale[i] = (*instanceList->getValue(i))->scale;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, scale, scaleBuffer);
}

//===================================================================================================================================
//�yUV�o�b�t�@���X�V����z
//===================================================================================================================================
void InstancingBillboard::updateUV()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;	

	for (int i = 0; i < instanceNum; i++)
	{
		uv[i] = (*instanceList->getValue(i))->uv;
	}
	copyVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, uv, uvBuffer);
}

//===================================================================================================================================
//�y�J���[�o�b�t�@���X�V����z
//===================================================================================================================================
void InstancingBillboard::updateColor()
{
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;

	for (int i = 0; i < instanceNum; i++)
	{
		color[i] = (*instanceList->getValue(i))->color;
	}
	copyVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, color, colorBuffer);
}

//===================================================================================================================================
//�y�C���X�^���X�̐����z
//===================================================================================================================================
void InstancingBillboard::generateInstance(InstancingBillboardNS::Instance* instance)
{
	instanceList->insertFront(instance);			//�C���X�^���X���쐬
	didGenerate = true;
}

//===================================================================================================================================
//�y�C���X�^���X�̍폜�z
//===================================================================================================================================
void InstancingBillboard::deleteInstance(int i)
{
	if ((*instanceList->getValue(i))->lifeTimer <= (*instanceList->getValue(i))->limitTime)return;
	SAFE_DELETE(*instanceList->getValue(i));			//���X�g���̎��̂��폜
	instanceList->remove(instanceList->getNode(i));		//���X�g���̃C���X�^���X���폜
	didDelete = true;
}

//===================================================================================================================================
//�y�����_���A�N�Z�X�p�z��̍X�V�z
//===================================================================================================================================
void InstancingBillboard::updateAccessList()
{
	instanceList->listUpdate();
	instanceNum = instanceList->nodeNum;
}

//===================================================================================================================================
//�y�`������Ȃ��z
//===================================================================================================================================
void InstancingBillboard::offRender()
{
	onRender = false;
}

//===================================================================================================================================
//�y�o�b�t�@�̃��\�[�X�X�V�z
//===================================================================================================================================
void InstancingBillboard::updateBuffer() 
{
	//�C���X�^���X���̎擾
	instanceNum = getInstanceNum();
	if (instanceNum <= 0)
	{
		SAFE_RELEASE(positionBuffer);		//�ʒu�o�b�t�@�̉��
		SAFE_RELEASE(rotationBuffer);		//��]�o�b�t�@�̉��
		SAFE_RELEASE(scaleBuffer);			//�X�P�[���o�b�t�@�̉��
		SAFE_RELEASE(uvBuffer);				//UV�o�b�t�@�̉��
		SAFE_RELEASE(colorBuffer);			//�J���[�o�b�t�@�̉��
		return;
	}

	//�ʒu���o�b�t�@�̍č쐬
	SAFE_RELEASE(positionBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);

	//��]���o�b�t�@�̍č쐬
	SAFE_RELEASE(rotationBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, 0, 0, D3DPOOL_MANAGED, &rotationBuffer, 0);

	//�X�P�[�����o�b�t�@�̍č쐬
	SAFE_RELEASE(scaleBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, 0, 0, D3DPOOL_MANAGED, &scaleBuffer, 0);

	//UV���o�b�t�@�̍č쐬
	SAFE_RELEASE(uvBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR2)*instanceNum, 0, 0, D3DPOOL_MANAGED, &uvBuffer, 0);

	//�J���[���o�b�t�@�̍č쐬
	SAFE_RELEASE(colorBuffer);
	device->CreateVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, 0, 0, D3DPOOL_MANAGED, &colorBuffer, 0);

}

//===================================================================================================================================
//�ycopy�p�z��̃������X�V�z
//===================================================================================================================================
void InstancingBillboard::updateArray() 
{
	//�C���X�^���X���̎擾
	instanceNum = getInstanceNum();

	//�ʒu�o�b�t�@�p�z��̍č쐬
	SAFE_DELETE_ARRAY(position);
	position = new D3DXVECTOR3[instanceNum];

	//��]�o�b�t�@�p�z��̍č쐬
	SAFE_DELETE_ARRAY(rotation);
	rotation = new D3DXVECTOR3[instanceNum];

	//�X�P�[���o�b�t�@�p�z��̍č쐬
	SAFE_DELETE_ARRAY(scale);
	scale = new D3DXVECTOR2[instanceNum];

	//UV�o�b�t�@�p�z��̍č쐬
	SAFE_DELETE_ARRAY(uv);
	uv = new D3DXVECTOR2[instanceNum];

	//�J���[�o�b�t�@�p�z��̍č쐬
	SAFE_DELETE_ARRAY(color);
	color = new D3DXCOLOR[instanceNum];

}

//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
InstancingBillboardNS::InstanceList InstancingBillboard::getList() { return *instanceList; }
int InstancingBillboard::getInstanceNum() { return instanceList->nodeNum; }

