//===================================================================================================================================
//�yOcean.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/07
// [�X�V��]2020/01/15
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Ocean.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Ocean::Ocean() {
	bumpTexture = NULL;
	waveMove	= D3DXVECTOR2(0,0);
	height		= 1.0f;
	deltaHeight = 0.01f;
	initialize();
};

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Ocean::~Ocean() {
	SAFE_DELETE(object);
	SAFE_RELEASE(bumpTexture);
};

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Ocean::initialize() {
	object = new Object();
	object->initialize(&D3DXVECTOR3(0, 0, 0));
	object->scale = D3DXVECTOR3(1, 1, 1);

	device = getDevice();
	effect = *shaderNS::reference(shaderNS::OCEAN);
	staticMesh = staticMeshNS::reference(staticMeshNS::OCEAN);
	
	//���_�錾
	D3DVERTEXELEMENT9 vertexElement[65];
	int i = 0;
	while (staticMesh->vertexElement[i].Type != D3DDECLTYPE_UNUSED)
	{
		vertexElement[i] = staticMesh->vertexElement[i];
		i++;
	}
	vertexElement[i] = { 1, 0,					D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 };
	i++;
	vertexElement[i] = { 1, sizeof(float) * 4,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 2 };
	i++;
	vertexElement[i] = { 1, sizeof(float) * 4 * 2,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 3 };
	i++;
	vertexElement[i] = { 1, sizeof(float) * 4 * 3,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 4 };
	i++;
	vertexElement[i] = D3DDECL_END();
	i++;
	device->CreateVertexDeclaration(vertexElement, &declaration);
	needUpdate = true;

	//���O�X�V
	object->update();

	//���[���h�}�g���b�N�X�o�b�t�@�p�z��̍č쐬
	SAFE_DELETE_ARRAY(worldMatrix);
	worldMatrix = new D3DXMATRIX[1];

	//���[���h�}�g���b�N�X���o�b�t�@�̍č쐬
	SAFE_RELEASE(matrixBuffer);
	device->CreateVertexBuffer(sizeof(D3DXMATRIX)*1, 0, 0, D3DPOOL_MANAGED, &matrixBuffer, 0);

	worldMatrix[0] = object->matrixWorld;
	copyVertexBuffer(sizeof(D3DXMATRIX)*1, worldMatrix, matrixBuffer);

	//�o���v�}�b�s���O�p�̃e�N�X�`���C���[�W�̍쐬
	LPDIRECT3DTEXTURE9 heightTexture = *textureNS::reference(textureNS::OCEAN_BUMP);//�n�C�g�}�b�v�e�N�X�`��
	D3DSURFACE_DESC desc;

	heightTexture->GetLevelDesc(0, &desc);
	if (FAILED(
		D3DXCreateTexture(
			device, 
			desc.Width, 
			desc.Height, 
			0,
			0, 
			D3DFMT_X8R8G8B8,
			D3DPOOL_MANAGED, 
			&bumpTexture)))
	{
		MSG("�o���v�}�b�s���O�p�̃e�N�X�`���C���[�W�̍쐬�Ɏ��s���܂����B");
	}
	if (FAILED(
		D3DXComputeNormalMap(
			bumpTexture, 
			heightTexture, 
			NULL, 
			0,
			D3DX_CHANNEL_LUMINANCE,
			//D3DX_CHANNEL_RED,
			20.0f)))
			//1.0f)))
	{
		MSG("�o���v�}�b�s���O�p�e�N�X�`���̖@���̎Z�o�Ɏ��s���܂����B");
	}

};

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Ocean::update()
{

	object->update();

	//�g�̈ړ�
	waveMove.x += 0.0001f;

	//�g�̍�������
	height += deltaHeight;
	
	//�g�̍��������̐ؑ�
	if (height <= 0.8)
	{
		deltaHeight = 0.002;
	}
	else if (height > 1.2)
	{
		deltaHeight = -0.002;
	}

	//���[���h�s��̋t�]�u�s��
	//D3DXMatrixInverse(&worldInverseTranspose, NULL, &object->matrixWorld);	//�t�s��
	//D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);	//�]�u�s��

}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Ocean::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	//�����_�[�X�e�[�g�̐ݒ�
	device->SetRenderState(D3DRS_FILLMODE, staticMeshRendererNS::SOLID);

	//���_�錾��ǉ�
	device->SetVertexDeclaration(declaration);

	//�X�g���[���Ƀ��b�V���̒��_�o�b�t�@���o�C���h
	device->SetStreamSource(0, staticMesh->vertexBuffer,	0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, matrixBuffer,				0, sizeof(D3DXMATRIX));

	//�C���f�b�N�X�o�b�t�@���Z�b�g
	device->SetIndices(staticMesh->indexBuffer);

	//�V�F�[�_�֒l���Z�b�g
	effect->SetTechnique("tecOcean");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);
	//effect->SetMatrixTranspose("WIT", &worldInverseTranspose);	//���[���h�t�]�u�s��

	//���C�g�̕����x�N�g���̃Z�b�g
	D3DXMATRIX m;
	D3DXVECTOR4 v;
	D3DXVECTOR4 lightPosition = D3DXVECTOR4(-10, 100.1, -0.01, 0);
	D3DXMatrixInverse(&m, NULL, &object->matrixWorld);
	D3DXVec4Transform(&v, &lightPosition, &m);
	D3DXVec3Normalize((D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v);
	effect->SetVector("lightDirection", &v);

	//�r���[�̋t�s��
	//D3DXMATRIX inverseView;
	//D3DXMatrixInverse(&inverseView, NULL, &view);
	//effect->SetMatrix("inverseView", &inverseView);

	//�J�����̈ʒu
	//�l�@�F����cameraPosition�ł��������@���̓r���[�s��̋t�s��ŋ��߂Ă���E�I�u�W�F�N�g�̃��[���h��Ԃ��g�p���Ă���̂Ń_������
	D3DXMATRIX worldView = object->matrixWorld * view;
	D3DXMatrixInverse(&worldView, NULL, &worldView);//�t�s��
	D3DXVECTOR4 eyePosition = D3DXVECTOR4(0, 0, 0, 1);
	D3DXVec4Transform(&eyePosition, &eyePosition, &worldView);
	effect->SetVector("eyePosition", &eyePosition);

	//�g�̒l
	effect->SetValue("waveMove", &waveMove, sizeof(waveMove));	//�ړ��l
	effect->SetValue("height", &height, sizeof(FLOAT));	//����
	
	// �����_�����O
	effect->Begin(0, 0);
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		//effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		effect->SetTexture("textureDecal", *textureNS::reference(textureNS::OCEAN));
		effect->SetTexture("normalMap", bumpTexture);

		//�V�F�[�_�[�X�V
		effect->CommitChanges();

		effect->BeginPass(0);
		//Draw�R�[��
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,									//D3DPRIMITIVETYPE Type				:�`��^�C�v
			0,													//INT BaseVertexIndex				:�I�t�Z�b�g
			staticMesh->attributeTable[i].VertexStart,			//UINT MinIndex						:���̑����O���[�v�̒��_�ōŏ��̃C���f�b�N�X
			staticMesh->attributeTable[i].VertexCount,			//UINT NumVertices					:���_���i���̑����O���[�v�̒��_���j
			staticMesh->attributeTable[i].FaceStart * 3,		//UINT StartIndex					:�C���f�b�N�X�̃C���f�b�N�X�i�C���f�b�N�X�o�b�t�@���̃C���f�b�N�X�j
			staticMesh->attributeTable[i].FaceCount);			//UINT PrimitiveCount				:�|���S����

		effect->EndPass();
	}
	effect->End();

	device->SetStreamSource(0, NULL, 0, NULL);
	device->SetStreamSource(1, NULL, 0, NULL);

	//��n��
	device->SetStreamSourceFreq(0, 0);
	device->SetStreamSourceFreq(1, 0);
}
