//===================================================================================================================================
//�yOcean.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/07
// [�X�V��]2019/11/07
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Ocean.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Ocean::Ocean() {
	initialize();
};

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Ocean::~Ocean() {
	SAFE_DELETE(object);
};

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Ocean::initialize() {
	object = new Object();
	object->initialize(&D3DXVECTOR3(0, 0, 0));
	object->scale = D3DXVECTOR3(20000, 1, 20000);

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
};

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Ocean::update()
{
	object->update();
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
	device->SetStreamSource(0, staticMesh->vertexBuffer, 0, staticMesh->numBytesPerVertex);

	//�C���f�b�N�X�o�b�t�@���Z�b�g
	device->SetIndices(staticMesh->indexBuffer);


	//�V�F�[�_�֒l���Z�b�g
	effect->SetTechnique("mainTechnique");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);

	// �����_�����O
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		effect->SetTexture("textureDecal", staticMesh->textures[i]);

		//�V�F�[�_�[�X�V
		effect->CommitChanges();
		effect->Begin(0, 0);

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
		effect->End();
	}

	device->SetStreamSource(0, NULL, 0, NULL);
	device->SetStreamSource(1, NULL, 0, NULL);

	//��n��
	device->SetStreamSourceFreq(0, 0);
	device->SetStreamSourceFreq(1, 0);
}
