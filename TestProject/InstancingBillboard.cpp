//===================================================================================================================================
//�yInstancingBillboard.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/08/07
// [�X�V��]2019/09/12
//===================================================================================================================================
#include "InstancingBillboard.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
InstancingBillboard::InstancingBillboard()
{
	renderNum = 0;
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
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
HRESULT InstancingBillboard::initialize(LPDIRECT3DDEVICE9 device, LPD3DXEFFECT _effect, LPDIRECT3DTEXTURE9 _texture)
{
	InstancingBillboardVertex vertex[4] = {
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

	//�ʒu���o�b�t�@�̍쐬
	//device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	//copyVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, position, positionBuffer);

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

	//setShaderDirectory();
	//HRESULT hr;
	//LPD3DXBUFFER err = NULL;
	//if (FAILED(hr = D3DXCreateEffectFromFile(device, "InstancingBillboard.fx", NULL, NULL, 0, NULL, &effect, &err)))
	//{
	//	MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "ERROR", MB_OK);
	//}
	
	//�e�N�X�`����ݒ�
	texture = _texture;
	//setVisualDirectory();
	//D3DXCreateTextureFromFileA(device, "ring.png", &texture);

	return S_OK;
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void InstancingBillboard::render(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	if (!onRender)return;
	if (renderNum <= 0)return;
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
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | renderNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// ���_�錾��ʒm
	device->SetVertexDeclaration(declation);
	
	//�f�o�C�X�f�[�^�X�g���[���Ƀ��b�V���̊e�o�b�t�@���o�C���h
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(InstancingBillboardVertex));	//���_�o�b�t�@
	device->SetStreamSource(1, positionBuffer, 0, sizeof(D3DXVECTOR3));				//�ʒu�o�b�t�@
	device->SetStreamSource(2, colorBuffer, 0, sizeof(D3DXCOLOR));					//�J���[�o�b�t�@

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
//�y����Ɉʒu���������_���ɍ쐬����z
//===================================================================================================================================
void InstancingBillboard::createPositionSpherical(LPDIRECT3DDEVICE9 device, int num ,float radius)
{
	renderNum = num;
	//�ʒu���z��̍쐬
	position = new D3DXVECTOR3[num];
	for (int i = 0; i < num; i++)
	{
		D3DXVECTOR3 pos((float)(rand() % 100 - 50),(float)(rand() % 100 - 50),(float)(rand() % 100 - 50));
		D3DXVec3Normalize(&pos, &pos);
		pos *= radius;
		position[i] = pos;
	}
	//�ʒu���o�b�t�@�̍쐬
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	copyVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, position, positionBuffer);

	//�J���[���z��̍쐬
	D3DXCOLOR* col = new D3DXCOLOR[num];
	for (int i = 0; i < num; i++)
	{
		col[i] = D3DXCOLOR( 
			((float)(rand() % 100)) / 100, 
			((float)(rand() % 100)) / 100, 
			((float)(rand() % 100)) / 100, 
			1.0f);
	}
	//�J���[
	setColorBuffer(device, renderNum, col);

	SAFE_DELETE_ARRAY(col);

	onRender = true;
}

//===================================================================================================================================
//�y�J���[�o�b�t�@���Z�b�g����z
//�E�ăZ�b�g���\
//===================================================================================================================================
void InstancingBillboard::setColorBuffer(LPDIRECT3DDEVICE9 device, int num, D3DXCOLOR* colorList)
{
	if (num <= 0) {
		return;
	}
	if(colorBuffer!=NULL)
		SAFE_RELEASE(colorBuffer);
	if(color!=NULL)
		SAFE_DELETE_ARRAY(color);
	
	renderNum = num;

	//�J���[�o�b�t�@�̍쐬
	color = new D3DXCOLOR[num];
	for (int i = 0; i < num; i++)
	{
		color[i] = colorList[i];
	}

	//�J���[�o�b�t�@�̍쐬
	device->CreateVertexBuffer(sizeof(D3DXCOLOR)*renderNum, 0, 0, D3DPOOL_MANAGED, &colorBuffer, 0);
	copyVertexBuffer(sizeof(D3DXCOLOR)*renderNum, color, colorBuffer);
}

//===================================================================================================================================
//�y�`�搔�̈ʒu���o�b�t�@���Z�b�g����z
//===================================================================================================================================
void InstancingBillboard::setNumOfRender(LPDIRECT3DDEVICE9 device, int num, D3DXVECTOR3* positionList)
{
	if (num <= 0) {
		if(positionBuffer != NULL)SAFE_RELEASE(positionBuffer);
		onRender = false;
		return;
	}
	renderNum = num;

	position = new D3DXVECTOR3[num];
	for (int i = 0; i < num; i++)
	{
		position[i] = positionList[i];
	}

	if (positionBuffer != NULL)SAFE_RELEASE(positionBuffer);
	//�ʒu���o�b�t�@�̍쐬
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	copyVertexBuffer(sizeof(D3DXVECTOR3)*renderNum, position, positionBuffer);
	SAFE_DELETE_ARRAY(position);

	if (onRender == false)
	{
		//�J���[���z��̍쐬
		D3DXCOLOR* col = new D3DXCOLOR[num];
		for (int i = 0; i < num; i++)
		{
			col[i] = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		}
		//�J���[
		setColorBuffer(device, renderNum, col);
		SAFE_DELETE_ARRAY(col);
	}

	onRender = true;
}

void InstancingBillboard::offRender()
{
	onRender = false;
}