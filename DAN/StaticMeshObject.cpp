//===================================================================================================================================
//�yStaticMeshObject.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/23
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "StaticMeshObject.h"
#include "Direct3D9.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
StaticMeshObject::StaticMeshObject(StaticMesh* _staticMesh)
{
	device					= getDevice();		//�f�o�C�X�̎擾
	this->staticMesh		= _staticMesh;
	onRender				= true;
	onTransparent			= false;
	onLight					= true;
	didDelete				= false;
	didGenerate				= false;
	objectNum				= 0;
	fillMode				= staticMeshObjectNS::FILLMODE::SOLID;
	matrixBuffer			= NULL;
	worldMatrix				= NULL;
	declaration				= NULL;

	//���_�錾
	D3DVERTEXELEMENT9 vertexElement[65];
	int i = 0;
	while(staticMesh->vertexElement[i].Type != D3DDECLTYPE_UNUSED)
	{
		vertexElement[i] = staticMesh->vertexElement[i];
		i++;
	}
	vertexElement[i] = { 1, 0,					D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 };
	i++;
	vertexElement[i] = { 1, sizeof(float)*4,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 2 };
	i++;
	vertexElement[i] = { 1, sizeof(float)*4*2,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 3 };
	i++;
	vertexElement[i] = { 1, sizeof(float)*4*3,	D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 4 };
	i++;
	vertexElement[i] = D3DDECL_END();
	i++;
	device->CreateVertexDeclaration(vertexElement, &declaration);

	objectList = new staticMeshObjectNS::ObjectList();

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
StaticMeshObject::~StaticMeshObject()
{
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(uvBuffer);

	SAFE_DELETE_ARRAY(worldMatrix);
	SAFE_DELETE_ARRAY(color);

	for (int i = 0; i < objectNum; i++)
	{
		SAFE_DELETE(*objectList->getValue(i));
	}
	objectList->terminate();
	SAFE_DELETE(objectList);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void StaticMeshObject::update()
{
	for (int i = 0; i < objectNum; i++)
	{
		(*objectList->getValue(i))->update();			//�X�V����
		deleteObject(i);								//�폜����
	}

	if (didDelete || didGenerate)
	{
		objectList->listUpdate();
		updateBuffer();
		updateArray();
		didGenerate = false;
		didDelete = false;
	}

	//�l�̍X�V
	updateWorldMatrix();
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void StaticMeshObject::render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	//�`�悪�L���łȂ��ꍇ�I��
	if (onRender == false)return;
	if (objectNum <= 0)return;
	
	//�����_�[�X�e�[�g�̐ݒ�
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
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA		| objectNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA		| 1);

	//���_�錾��ǉ�
	device->SetVertexDeclaration(declaration);

	//�X�g���[���Ƀ��b�V���̒��_�o�b�t�@���o�C���h
	device->SetStreamSource(0, staticMesh->vertexBuffer,	0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, matrixBuffer,						0, sizeof(D3DXMATRIX));

	//�C���f�b�N�X�o�b�t�@���Z�b�g
	device->SetIndices(staticMesh->indexBuffer);

	effect->SetTechnique("mainTechnique");

	//�V�F�[�_�֒l���Z�b�g
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);	
	//effect->SetValue("positionList", position,objectNum*sizeof(D3DXVECTOR3));

	// �����_�����O
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		effect->SetTexture("textureDecal", staticMesh->textures[i]);
		
		//�V�F�[�_�[�X�V
		//effect->CommitChanges();
		effect->Begin(0, 0);
		if(onLight)		effect->BeginPass(0);
		else			effect->BeginPass(1);

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

//===================================================================================================================================
//�y���[���h�}�g���b�N�X�o�b�t�@���X�V����z
//===================================================================================================================================
void StaticMeshObject::updateWorldMatrix()
{
	objectNum = objectList->nodeNum;
	if (objectNum <= 0)return;
	Object* obj;

	for (int i = 0; i < objectNum; i++)
	{
		obj = *objectList->getValue(i);
		worldMatrix[i] = obj->matrixWorld;
	}

	copyVertexBuffer(sizeof(D3DXMATRIX)*objectNum, worldMatrix, matrixBuffer);

}

//===================================================================================================================================
//�y�J���[�o�b�t�@���X�V����z
//===================================================================================================================================
void StaticMeshObject::updateColor()
{
	objectNum = objectList->nodeNum;
	if (objectNum <= 0)return;

	for (int i = 0; i < objectNum; i++)
	{
		//color[i] = (*objectList->getValue(i))->color;
	}
	copyVertexBuffer(sizeof(D3DXCOLOR)*objectNum, color, colorBuffer);
}

//===================================================================================================================================
//�y�I�u�W�F�N�g�̐����z
//===================================================================================================================================
void StaticMeshObject::generateObject(Object* object)
{
	objectList->insertFront(object);			//�I�u�W�F�N�g���쐬
	didGenerate = true;
}

//===================================================================================================================================
//�y�I�u�W�F�N�g�̑S�폜�z
//===================================================================================================================================
void StaticMeshObject::allDelete()
{
	for (int i = 0; i < objectNum; i++)
	{
		SAFE_DELETE(*objectList->getValue(i));
	}
	objectList->allClear();
	objectNum = objectList->nodeNum;
}

//===================================================================================================================================
//�yID���g�p���ăI�u�W�F�N�g�̍폜�z
//===================================================================================================================================
void StaticMeshObject::deleteObjectByID(int id)
{
	for (int i = 0; i < objectNum; i++)
	{	
		if ((*objectList->getValue(i))->id == id)
		{
			(*objectList->getValue(i))->existenceTimer = 0.0f;//���ݎ��Ԃ�0�ɂ��č폜�\��Ԃɂ���
			return;		//�����I��
		}
	}
}

//===================================================================================================================================
//�y�I�u�W�F�N�g�̍폜�z
//===================================================================================================================================
void StaticMeshObject::deleteObject(int i)
{
	if ((*objectList->getValue(i))->existenceTimer > 0)return;
	SAFE_DELETE(*objectList->getValue(i));
	objectList->remove(objectList->getNode(i));		//���X�g���̃I�u�W�F�N�g���폜
	didDelete = true;
}

//===================================================================================================================================
//�y�����_���A�N�Z�X�p�z��̍X�V�z
//===================================================================================================================================
void StaticMeshObject::updateAccessList()
{
	objectList->listUpdate();
}

//===================================================================================================================================
//�y�o�b�t�@�̃��\�[�X�X�V�z
//===================================================================================================================================
void StaticMeshObject::updateBuffer()
{
	objectNum = objectList->nodeNum;				//�I�u�W�F�N�g�̐��̎擾

	//���[���h�}�g���b�N�X���o�b�t�@�̍č쐬
	SAFE_RELEASE(matrixBuffer);
	device->CreateVertexBuffer(sizeof(D3DXMATRIX)*objectNum, 0, 0, D3DPOOL_MANAGED, &matrixBuffer, 0);
}

//===================================================================================================================================
//�ycopy�p�z��̃������X�V�z
//===================================================================================================================================
void StaticMeshObject::updateArray()
{
	//�C���X�^���X���̎擾
	objectNum = objectList->nodeNum;

	//���[���h�}�g���b�N�X�o�b�t�@�p�z��̍č쐬
	SAFE_DELETE_ARRAY(worldMatrix);
	worldMatrix = new D3DXMATRIX[objectNum];

}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
#ifdef _DEBUG
void StaticMeshObject::outputGUI()
{
	Object* obj;
	for (int i = 0; i < objectNum; i++)
	{
		obj = *objectList->getValue(i);
		obj->outputGUI();
	}
}
#endif // _DEBUG

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void StaticMeshObject::enableLight()	{ onLight = true; }
void StaticMeshObject::disableLight()	{ onLight = false; }

//===================================================================================================================================
//�y�X�^�e�B�b�N���b�V���̎擾�z
//===================================================================================================================================
StaticMesh* StaticMeshObject::getStaticMesh()
{

	return staticMesh;
}
