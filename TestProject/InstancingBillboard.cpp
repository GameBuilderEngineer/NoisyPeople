//===================================================================================================================================
//�yInstancingBillboard.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/27
// [�X�V��]2019/09/28
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
	onRender = false;
	vertexBuffer = NULL;
	indexBuffer = NULL;
	positionBuffer = NULL;
	colorBuffer = NULL;
	declation = NULL;
	effect = NULL;
	position = NULL;
	color = NULL;
	texture = NULL;
	device = getDevice();
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
InstancingBillboard::~InstancingBillboard()
{
	//SAFE_RELEASE
	if (vertexBuffer != NULL)
		SAFE_RELEASE(vertexBuffer);
	if (indexBuffer != NULL)
		SAFE_RELEASE(indexBuffer);
	if (positionBuffer != NULL)
		SAFE_RELEASE(positionBuffer);
	if (colorBuffer != NULL)
		SAFE_RELEASE(colorBuffer);
	if (declation != NULL)
		SAFE_RELEASE(declation);
	//SAFE_DELETE
	if (position != NULL)			
		SAFE_DELETE_ARRAY(position);
	if (color != NULL)
		SAFE_DELETE_ARRAY(color);
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
		{D3DXVECTOR2( -1.0f,  1.0f),D3DXVECTOR2(0.0f,0.0f)},
		{D3DXVECTOR2(  1.0f,  1.0f),D3DXVECTOR2(1.0f,0.0f)},
		{D3DXVECTOR2( -1.0f, -1.0f),D3DXVECTOR2(0.0f,1.0f)},
		{D3DXVECTOR2(  1.0f, -1.0f),D3DXVECTOR2(1.0f,1.0f)},
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
		{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	//���_���W
		{ 0, sizeof(D3DXVECTOR2), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },//UV
		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 },	//�ʒu
		{ 2, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR, 0 },	//�J���[
		D3DDECL_END()
	};
	device->CreateVertexDeclaration(vertexElement, &declation);

	//�V�F�[�_�[��ݒ�
	effect = _effect;
	
	//�e�N�X�`����ݒ�
	texture = _texture;

	//�C���X�^���X���X�g�̏�����
	instanceList = new InstancingBillboardNS::InstanceList();

	return S_OK;
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void InstancingBillboard::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	int instanceNum = getInstanceNum();
	if (!onRender)return;
	if (instanceNum <= 0)return;
	//��]��ł������B
	//D3DXMATRIX cancelRotation = view;
	//cancelRotation._41 = cancelRotation._42 = cancelRotation._43 = 0;
	//D3DXMatrixInverse(&cancelRotation, NULL,&cancelRotation);

	//���e�X�g
	//device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ���u�����h���s��
	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// ���\�[�X�J���[�̎w��
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// ���f�X�e�B�l�[�V�����J���[�̎w��
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//���Z�������s��
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

	// ���u�����h���s��
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// ���f�X�e�B�l�[�V�����J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�C���X�^���X�錾
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | instanceNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// ���_�錾��ʒm
	device->SetVertexDeclaration(declation);
	
	//�f�o�C�X�f�[�^�X�g���[���Ƀ��b�V���̊e�o�b�t�@���o�C���h
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(InstancingBillboardNS::Vertex));		//���_�o�b�t�@
	device->SetStreamSource(1, positionBuffer, 0, sizeof(D3DXVECTOR3));						//�ʒu�o�b�t�@
	device->SetStreamSource(2, colorBuffer, 0, sizeof(D3DXCOLOR));							//�J���[�o�b�t�@

	//�C���f�b�N�X�o�b�t�@���Z�b�g
	device->SetIndices(indexBuffer);

	effect->SetTechnique("mainTechnique");
	//effect->SetMatrix("cancelRotation", &cancelRotation);
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);
	effect->SetTexture("planeTexture", texture);
	effect->Begin(0, 0);
	effect->BeginPass(0);
		
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	effect->EndPass();
	effect->End();

	//��n��
	device->SetStreamSourceFreq(0, 1);
	device->SetStreamSourceFreq(1, 1);
	device->SetStreamSourceFreq(2, 1);
	// ���u�����h��؂�
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//===================================================================================================================================
//�y�J���[�o�b�t�@���Z�b�g����z
//===================================================================================================================================
void InstancingBillboard::updateColor()
{
	int instanceNum = getInstanceNum();
	if (instanceNum <= 0)			return;
	if (device == NULL)				return;

	//�J���[�z��̍X�V
	for (int i = 0; i < instanceNum; i++)
	{
		color[i] = instanceList->getValue(i)->color;
	}

	//�J���[�o�b�t�@�̉��
	if (this->colorBuffer != NULL)	SAFE_RELEASE(this->colorBuffer);

	//�J���[�o�b�t�@�̍쐬
	device->CreateVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, 0, 0, D3DPOOL_MANAGED, &colorBuffer, 0);
	copyVertexBuffer(sizeof(D3DXCOLOR)*instanceNum, color, colorBuffer);

}

//===================================================================================================================================
//�y�`�搔�̈ʒu���o�b�t�@���Z�b�g����z
//===================================================================================================================================
void InstancingBillboard::updatePosition()
{
	int instanceNum = getInstanceNum();
	if (device == NULL)	return;
	if (getInstanceNum() <= 0) return;
	
	for (int i = 0; i < instanceNum; i++)
	{
		position[i] = instanceList->getValue(i)->position;
	}

	//�ʒu�o�b�t�@�̉��
	if (positionBuffer != NULL)SAFE_RELEASE(positionBuffer);

	//�ʒu���o�b�t�@�̍쐬
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	copyVertexBuffer(sizeof(D3DXVECTOR3)*instanceNum, position, positionBuffer);

}

//===================================================================================================================================
//�y�C���X�^���X�̐����z
//===================================================================================================================================
void InstancingBillboard::generateInstance(InstancingBillboardNS::Instance instance)
{
	int instanceNum = getInstanceNum();

	instanceList->insertFront();
	instanceList->listUpdate();

	//�ʒu�o�b�t�@
	if(this->position != NULL)SAFE_DELETE_ARRAY(this->position);
	position = new D3DXVECTOR3[instanceNum];

	//�J���[�o�b�t�@�̍쐬
	if (this->color != NULL)	SAFE_DELETE_ARRAY(this->color);
	color = new D3DXCOLOR[instanceNum];

	*instanceList->getValue(0) = instance;

}

//===================================================================================================================================
//�y�C���X�^���X�̍폜�z
//===================================================================================================================================
void InstancingBillboard::deleteInstance(int i)
{
	if (instanceList->getValue(i)->existenceTimer >= 0)return;
	instanceList->remove(instanceList->getNode(i));
	instanceList->listUpdate();
}

//===================================================================================================================================
//�y�`������Ȃ��z
//===================================================================================================================================
void InstancingBillboard::offRender()
{
	onRender = false;
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void InstancingBillboard::update(float frameTime)
{
	if (getInput()->isKeyDown('G'))
	{
		srand((unsigned int)time(NULL));
		InstancingBillboardNS::Instance instance;
		instance.position = D3DXVECTOR3((float)(rand()%100-50),(float)(rand()%100-50),(float)(rand()%100-50));
		instance.color = D3DCOLOR_RGBA(rand()%255,rand()%255,rand()%255,255);
		//instance.texture = *textureNS::reference(textureNS::LIGHT_001);
		instance.existenceTimer = (float)(rand()%120);
		generateInstance(instance);
	}

	for (int i = 0; i < instanceList->nodeNum; i++)
	{
		instanceList->getValue(i)->existenceTimer -= frameTime;
		deleteInstance(i);
	}

}

//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
InstancingBillboardNS::InstanceList InstancingBillboard::getList() { return *instanceList; }
int InstancingBillboard::getInstanceNum() { return instanceList->nodeNum; }
