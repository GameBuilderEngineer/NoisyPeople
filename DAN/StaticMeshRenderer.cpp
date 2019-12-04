//===================================================================================================================================
//�yStaticMeshRenderer.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/11/11
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "StaticMeshRenderer.h"
#include "Direct3D9.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace staticMeshRendererNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
StaticMeshRenderer::StaticMeshRenderer(StaticMesh* _staticMesh)
{
	device					= getDevice();		//�f�o�C�X�̎擾
	this->staticMesh		= _staticMesh;
	onRender				= true;
	onTransparent			= false;
	onForeground			= false;
	onLight					= true;
	didRegister				= false;
	didUnRegister			= false;
	objectNum				= 0;
	fillMode				= staticMeshRendererNS::FILLMODE::SOLID;
	matrixBuffer			= NULL;
	worldMatrix				= NULL;
	declaration				= NULL;
	renderPass				= LAMBERT_PASS;
	alpha					= 1.0f;

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

	objectList = new staticMeshRendererNS::ObjectList();

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
StaticMeshRenderer::~StaticMeshRenderer()
{
	SAFE_RELEASE(matrixBuffer);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(uvBuffer);

	SAFE_DELETE_ARRAY(worldMatrix);
	SAFE_DELETE_ARRAY(color);

	objectList->terminate();
	SAFE_DELETE(objectList);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void StaticMeshRenderer::update()
{
	if (didRegister || didUnRegister)
	{
		objectList->listUpdate();
		objectNum = objectList->nodeNum;
	}

	for (int i = 0; i < objectNum; i++)
	{
		unRegisterObject(i);							//��������
	}

	if (didRegister|| didUnRegister)
	{
		objectList->listUpdate();
		updateBuffer();
		updateArray();
		didRegister = false;
		didUnRegister = false;
	}

	//�l�̍X�V
	updateWorldMatrix();
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void StaticMeshRenderer::render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//�`�悪�L���łȂ��ꍇ�I��
	if (onRender == false)return;
	if (objectNum <= 0)return;
	
	//�����_�[�X�e�[�g�̐ݒ�
	device->SetRenderState(D3DRS_FILLMODE, fillMode);

	//�C���X�^���X�錾
	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA		| objectNum);
	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA		| 1);
	device->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA		| 1);

	//���_�錾��ǉ�
	device->SetVertexDeclaration(declaration);

	//�X�g���[���Ƀ��b�V���̒��_�o�b�t�@���o�C���h
	device->SetStreamSource(0, staticMesh->vertexBuffer,	0, staticMesh->numBytesPerVertex);
	device->SetStreamSource(1, matrixBuffer,				0, sizeof(D3DXMATRIX));

	//�C���f�b�N�X�o�b�t�@���Z�b�g
	device->SetIndices(staticMesh->indexBuffer);


	//�V�F�[�_�֒l���Z�b�g
	effect->SetTechnique("mainTechnique");
	effect->SetMatrix("matrixProjection", &projection);
	effect->SetMatrix("matrixView", &view);	
	effect->SetFloat("alphaValue", alpha);

	// �����_�����O
	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
	{
		effect->SetFloatArray("diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
		effect->SetTexture("textureDecal", staticMesh->textures[i]);

		//�V�F�[�_�[�X�V
		effect->CommitChanges();
		effect->Begin(0, 0);

		//�`��p�X�̐ؑ�
		switch (renderPass)
		{
		case LAMBERT_PASS:		effect->BeginPass(0);	break;
		case TEXEL_PASS:		effect->BeginPass(1);	break;
		case TRANSPARENT_PASS:	effect->BeginPass(2);	break;
		case FOREGROUND_PASS:	effect->BeginPass(3);	break;
		}

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
void StaticMeshRenderer::updateWorldMatrix()
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
void StaticMeshRenderer::updateColor()
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
//�y�`��I�u�W�F�N�g�̓o�^�z
//===================================================================================================================================
void StaticMeshRenderer::registerObject(Object* object)
{
	objectList->insertFront(object);			//�I�u�W�F�N�g�|�C���^���쐬
	didRegister = true;
}

//===================================================================================================================================
//�y�`��I�u�W�F�N�g�̑S�����z
//===================================================================================================================================
void StaticMeshRenderer::allUnRegister()
{
	objectList->allClear();
	objectNum = objectList->nodeNum;
}

//===================================================================================================================================
//�yID���g�p���ĕ`��I�u�W�F�N�g�̉����z
//===================================================================================================================================
void StaticMeshRenderer::unRegisterObjectByID(int id)
{
	//�������茟��
	for (int i = 0; i < objectNum; i++)
	{	
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//�����Ŏ~�܂�o�O����(2019/11/24)
		//�L�F����
		//�����炭�o���b�g�̃T�C�N���ɖ�肠��(��11/25�����������̂Ŗ��Ȃ������H)
		//���̃N���X�Ȃǂł����Œ�~����o�O���������ꍇ�͐���֕񍐂��Ă��������B
		//�����������s���Ă���̂ŁA�e�����o�Ă��鋰�ꂪ����܂��B
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if ((*objectList->getValue(i))->id == id)
		{
			//unRegisterObject(i);
			//���ݎ��Ԃ�0�ɂ��č폜�\��Ԃɂ���
			//(*objectList->getValue(i))->existenceTimer = 0.0f;
			objectList->remove(objectList->getNode(i));
			didUnRegister = true;
			//�����I��
			return;	
		}
	}
}

//===================================================================================================================================
//�y�`��I�u�W�F�N�g�̉����z
//===================================================================================================================================
void StaticMeshRenderer::unRegisterObject(int i)
{
	//if (*objectList->getValue(i)) 
	//{
	//�L���l�Ȃ�^�C�}�[�`�F�b�N���������
	if ((*objectList->getValue(i))->existenceTimer > 0)return;	//�^�C�}�[�`�F�b�N
	objectList->remove(objectList->getNode(i));					//���X�g���̃I�u�W�F�N�g�|�C���^���폜
	didUnRegister = true;
	//}
	//else
	//{
	//	//�L���l�łȂ���Ύ����I�ɉ���
	//	objectList->remove(objectList->getNode(i));					//���X�g���̃I�u�W�F�N�g�|�C���^���폜
	//	didUnRegister = true;
	//}
}

//===================================================================================================================================
//�y�����_���A�N�Z�X�p�z��̍X�V�z
//===================================================================================================================================
void StaticMeshRenderer::updateAccessList()
{
	objectList->listUpdate();
	objectNum = objectList->nodeNum;
}

//===================================================================================================================================
//�y�o�b�t�@�̃��\�[�X�X�V�z
//===================================================================================================================================
void StaticMeshRenderer::updateBuffer()
{
	objectNum = objectList->nodeNum;				//�I�u�W�F�N�g�̐��̎擾

	//���[���h�}�g���b�N�X���o�b�t�@�̍č쐬
	SAFE_RELEASE(matrixBuffer);
	device->CreateVertexBuffer(sizeof(D3DXMATRIX)*objectNum, 0, 0, D3DPOOL_MANAGED, &matrixBuffer, 0);
}

//===================================================================================================================================
//�ycopy�p�z��̃������X�V�z
//===================================================================================================================================
void StaticMeshRenderer::updateArray()
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
void StaticMeshRenderer::outputGUI()
{

}
#endif // _DEBUG

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void StaticMeshRenderer::enableLight()			{ onLight = true; }
void StaticMeshRenderer::disableLight()			{ onLight = false; }
void StaticMeshRenderer::enableTransparent()	{ onTransparent = true; }
void StaticMeshRenderer::disableTransparent()	{ onTransparent = false; }
void StaticMeshRenderer::enableForeground()		{ onForeground = true; }
void StaticMeshRenderer::disableForeground()	{ onForeground = false; }
void StaticMeshRenderer::setFillMode(int mode)	{ fillMode = mode; }
void StaticMeshRenderer::setRenderPass(int pass){ renderPass = pass; }
void StaticMeshRenderer::setAlpha(float value)	{ alpha = value; }

//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
StaticMesh* StaticMeshRenderer::getStaticMesh(){return staticMesh;}
int			StaticMeshRenderer::getObjectNum() { return objectNum; }
float*		StaticMeshRenderer::getAlpha() { return &alpha; }
