//===================================================================================================================================
//�yStaticMeshObject.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/08
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "StaticMeshObject.h"
#include "Direct3D9.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
StaticMeshObject::StaticMeshObject(StaticMesh* _staticMesh)
{
	this->staticMesh = _staticMesh;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = getDevice();

	//���_�錾
	D3DVERTEXELEMENT9 vertexElement[65];
	int i = 0;
	while(staticMesh->vertexElement[i].Type != D3DDECLTYPE_UNUSED)
	{
		vertexElement[i] = staticMesh->vertexElement[i];
		i++;
	}
	vertexElement[i] = { 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 };
	i++;
	vertexElement[i] = D3DDECL_END();
	i++;
	device->CreateVertexDeclaration(vertexElement, &declaration);

	//�ʒu�o�b�t�@�̍쐬
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3) * 1/*instanceNum*/, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
	copyVertexBuffer(sizeof(D3DXVECTOR3)*1/*instanceNum*/, position, positionBuffer);

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
StaticMeshObject::~StaticMeshObject()
{

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void StaticMeshObject::update()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = getDevice();

	Object::update();

	updatePosition();


}

//===================================================================================================================================
//�y�o�b�t�@�̃��\�[�X�X�V�z
//===================================================================================================================================
void StaticMeshObject::updateBuffer()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = getDevice();

	//�ʒu���o�b�t�@�̍č쐬
	SAFE_RELEASE(positionBuffer);
	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*1/*instanceNum*/, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
}

//===================================================================================================================================
//�y�ʒu�o�b�t�@���X�V����z
//===================================================================================================================================
void StaticMeshObject::updatePosition()
{
	copyVertexBuffer(sizeof(D3DXVECTOR3)*1, position, positionBuffer);
}


//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void StaticMeshObject::render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	//�`�悪�L���łȂ��ꍇ�I��
	if (onRender == false)return;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = getDevice();

	//���[���h�}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_WORLD, &matrixWorld);
	
	//�����_�[�X�e�[�g�̐ݒ�
	device->SetRenderState(D3DRS_LIGHTING, onLighting);
	device->SetRenderState(D3DRS_FILLMODE, fillMode);

	//���ߏ���
	if (onTransparent)
	{//�L��
		//���u�����f�B���O��ݒ肷��
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);	// �|���S���̃���
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);	// �e�N�X�`���̃���
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// ������
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);				// ���Z�������s��
	}
	else
	{//����
		//���u�����f�B���O��ݒ肵�Ȃ�
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	//�C���X�^���X�錾
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | renderNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//���_�錾��ǉ�
	device->SetVertexDeclaration(declaration);

	//�X�g���[���Ƀ��b�V���̒��_�o�b�t�@���o�C���h
	device->SetStreamSource(0, staticMesh->vertexBuffer, 0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, positionBuffer, 0, sizeof(D3DXVECTOR3));

	//�C���f�b�N�X�o�b�t�@���Z�b�g
	device->SetIndices(staticMesh->indexBuffer);

	effect->SetTechnique("mainTechnique");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);

	// �����_�����O			
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		if (onTransparent) {
			D3DMATERIAL9 material = staticMesh->materials[i];
			material.Diffuse.a = alpha;
			effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		}
		else {
			effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		}

		effect->SetTexture("textureDecal", staticMesh->textures[i]);

		effect->Begin(0, 0);
		effect->BeginPass(0);

		//Draw�R�[��
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,									//D3DPRIMITIVETYPE Type
			0,													//INT BaseVertexIndex
			staticMesh->attributeTable[i].VertexStart,			//UINT MinIndex
			staticMesh->attributeTable[i].VertexCount,			//UINT NumVertices
			staticMesh->attributeTable[i].FaceStart * 3,		//UINT StartIndex
			staticMesh->attributeTable[i].FaceCount);			//UINT PrimitiveCount

		effect->EndPass();
		effect->End();
	}



	// �e�N�X�`���̐ݒ��NULL�ɂ���
	//device->SetTexture(0, NULL);

	//��n��
	device->SetStreamSourceFreq(0, 1);
	device->SetStreamSourceFreq(1, 1);



	//�f�o�b�O�p���I�u�W�F�N�g�̕`��
	debugRender();
}

//===================================================================================================================================
//�y�X�^�e�B�b�N���b�V���̎擾�z
//===================================================================================================================================
StaticMesh* StaticMeshObject::getStaticMesh()
{

	return staticMesh;
}