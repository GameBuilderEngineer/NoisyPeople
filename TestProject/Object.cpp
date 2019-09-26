//===================================================================================================================================
//�yObject.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/09/23
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Object.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace objectNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Object::Object()
{
	ZeroMemory(this, sizeof(Object));			//�����o�ϐ���0�ŏ�����
	alpha = 1.0f;								//���l�̐ݒ�
	fillMode = objectNS::FILLMODE::SOLID;		//�`����@
	onLighting = true;							//���C�e�B���O�t���O
	onTransparent = false;						//���߃t���O
	operationAlpha = false;						//���l����t���O
	D3DXMatrixIdentity(&matrixRotation);
	quaternion = D3DXQUATERNION(0, 0, 0, 1);
	axisX.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(1, 0, 0));
	axisY.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(0, 1, 0));
	axisZ.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(0, 0, 1));
	reverseAxisX.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(-1, 0, 0));
	reverseAxisY.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(0, -1, 0));
	reverseAxisZ.initialize(D3DXVECTOR3(0, 0, 0),D3DXVECTOR3(0, 0, -1));
	renderNum = 1;
#ifdef _DEBUG
	axisX.color			= D3DXCOLOR(255, 0, 0, 255);
	axisY.color			= D3DXCOLOR(0, 255, 0, 255);
	axisZ.color			= D3DXCOLOR(0, 0, 255, 255);
	reverseAxisX.color	= D3DXCOLOR(255, 0, 0, 255);
	reverseAxisY.color	= D3DXCOLOR(0, 255, 0, 255);
	reverseAxisZ.color	= D3DXCOLOR(0, 0, 255, 255);
#endif // _DEBUG
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Object::~Object()
{
	
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
HRESULT Object::initialize(D3DXVECTOR3* _position)
{
	//�����ʒu
	memcpy(position, _position, sizeof(D3DXVECTOR3));
	//���O�X�V
	update();
	return S_OK;
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Object::update()
{
	if (onActive == false)return;
	//���[���h�g�����X�t�H�[���i���[�J�����W�����[���h���W�ւ̕ϊ��j	
	D3DXMatrixTranslation(&matrixPosition, position.x, position.y, position.z);

	//�N�H�[�^�j�I������]�ʃp�����[�^�[�Ɏg�p����
	D3DXMatrixRotationQuaternion(&matrixRotation, &quaternion);
	D3DXMatrixMultiply(&matrixWorld, &matrixRotation, &matrixPosition);

	//���[���h���W���玩�g�̎����C���X�V����
	axisX.update(position, D3DXVECTOR3(matrixWorld._11,matrixWorld._12,matrixWorld._13));
	axisY.update(position, D3DXVECTOR3(matrixWorld._21,matrixWorld._22,matrixWorld._23));
	axisZ.update(position, D3DXVECTOR3(matrixWorld._31,matrixWorld._32,matrixWorld._33));
	reverseAxisX.update(position, -D3DXVECTOR3(matrixWorld._11, matrixWorld._12, matrixWorld._13));
	reverseAxisY.update(position, -D3DXVECTOR3(matrixWorld._21, matrixWorld._22, matrixWorld._23));
	reverseAxisZ.update(position, -D3DXVECTOR3(matrixWorld._31, matrixWorld._32, matrixWorld._33));
}

//===================================================================================================================================
//�y�C���X�^���V���O�`��ݒ�z
//===================================================================================================================================
//void Object::setInstancing(LPDIRECT3DDEVICE9 device,int num, D3DXVECTOR3* _positionList)
//{
//	renderNum = num;
//	//�ʒu���o�b�t�@�̍쐬
//	device->CreateVertexBuffer(sizeof(D3DXVECTOR3)*num, 0, 0, D3DPOOL_MANAGED, &positionBuffer, 0);
//	copyVertexBuffer(sizeof(D3DXVECTOR3)*num, _positionList, positionBuffer);
//
//	//���_�錾
//	D3DVERTEXELEMENT9 vertexElement[65];
//	int i = 0;
//	while(staticMesh->vertexElement[i].Type != D3DDECLTYPE_UNUSED)
//	{
//		vertexElement[i] = staticMesh->vertexElement[i];
//		i++;
//	}
//	vertexElement[i] = { 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 1 };
//	i++;
//	vertexElement[i] = D3DDECL_END();
//	i++;
//	device->CreateVertexDeclaration(vertexElement, &declaration);
//}

//===================================================================================================================================
//�y�C���X�^���V���O�`��z
//===================================================================================================================================
//void Object::instancingRender(LPDIRECT3DDEVICE9 device, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon,LPD3DXEFFECT effect)
//{
//	if (onRender == false)return;
//	//device->SetRenderState(D3DRS_LIGHTING, true);
//
//	D3DMATERIAL9 matDef;
//	device->GetMaterial(&matDef);
//
//	//�C���X�^���X�錾
//	device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | renderNum);
//	device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
//
//	//���_�錾��ǉ�
//	device->SetVertexDeclaration(declaration);
//
//	//�X�g���[���Ƀ��b�V���̒��_�o�b�t�@���o�C���h
//	device->SetStreamSource(0, staticMesh->vertexBuffer, 0, staticMesh->numBytesPerVertex);
//	device->SetStreamSource(1, positionBuffer, 0, sizeof(D3DXVECTOR3));
//
//	//�C���f�b�N�X�o�b�t�@���Z�b�g
//	device->SetIndices(staticMesh->indexBuffer);
//
//	effect->SetTechnique("mainTechnique");
//	effect->SetMatrix("matrixProjection", &projection);
//	effect->SetMatrix("matrixView", &view);
//
//	effect->Begin(0, 0);
//	effect->BeginPass(0);
//
//	// �����_�����O
//	for (DWORD i = 0; i < staticMesh->attributeTableSize; i++)
//	{
//		//device->SetMaterial(&staticMesh->materials[i]);
//		effect->SetValue("diffuse", &staticMesh->materials[i].Diffuse,16);
//		effect->SetTexture("textureDecal", staticMesh->textures[i]);
//
//		device->DrawIndexedPrimitive(
//			D3DPT_TRIANGLELIST,									//D3DPRIMITIVETYPE Type
//			0,													//INT BaseVertexIndex
//			staticMesh->attributeTable[i].VertexStart,			//UINT MinIndex
//			staticMesh->attributeTable[i].VertexCount,			//UINT NumVertices
//			staticMesh->attributeTable[i].FaceStart * 3,		//UINT StartIndex
//			staticMesh->attributeTable[i].FaceCount);			//UINT PrimitiveCount
//	}
//
//	effect->EndPass();
//	effect->End();
//
//	// �}�e���A�����f�t�H���g�ɖ߂�
//	device->SetMaterial(&matDef);
//	// �e�N�X�`���̐ݒ��NULL�ɂ���
//	device->SetTexture(0, NULL);
//
//	//��n��
//	device->SetStreamSourceFreq(0, 1);
//	device->SetStreamSourceFreq(1, 1);
//
//}

//===================================================================================================================================
//�y�f�o�b�O�p�`��z
//===================================================================================================================================
void Object::debugRender()
{
#ifdef _DEBUG
	axisX.render(10.0f);
	axisY.render(10.0f);
	axisZ.render(10.0f);
	reverseAxisX.render(10.0f);
	reverseAxisY.render(10.0f);
	reverseAxisZ.render(10.0f);
#endif // _DEBUG
}

//===================================================================================================================================
//�y�g�D�[�������_�[�`��z
//===================================================================================================================================
//void Object::toonRender(LPDIRECT3DDEVICE9 device,D3DXMATRIX view,D3DXMATRIX projection,D3DXVECTOR3 cameraPositon,
//	LPD3DXEFFECT effect, LPDIRECT3DTEXTURE9 textureShade, LPDIRECT3DTEXTURE9 textureLine)
//{
//	device->SetTransform(D3DTS_WORLD, &matrixWorld);
//
//	effect->SetTechnique("ToonShading");
//	effect->SetMatrix("mProj", &projection);
//	effect->SetMatrix("mView", &view);
//	effect->SetMatrix("mWorld", &matrixWorld);
//	effect->SetTexture("ShadeTexture", textureShade);
//	effect->SetTexture("LineTexture", textureLine);
//
//	effect->SetVector("LightPos", &D3DXVECTOR4(0, 0, 5, 1));
//	effect->SetVector("EyePos", (D3DXVECTOR4*)&cameraPositon);
//	effect->Begin(NULL, 0);
//
//	// �����_�����O			
//	for (DWORD i = 0; i <staticMesh->numMaterial; i++)
//	{
//		effect->BeginPass(0);
//		effect->SetFloatArray("Diffuse", (FLOAT*)&staticMesh->materials[i].Diffuse, 4);
//		//effect->SetTexture("DecalTexture", meshTextures[i]);
//		//device->SetMaterial(&meshMaterials[i]);
//		//device->SetTexture(0, meshTextures[i]);
//		staticMesh->mesh->DrawSubset(i);
//		effect->EndPass();
//	}
//	effect->End();
//
//#ifdef _DEBUG
//	axisX.render(10.0f);
//	axisY.render(10.0f);
//	axisZ.render(10.0f);
//	reverseAxisX.render(10.0f);
//	reverseAxisY.render(10.0f);
//	reverseAxisZ.render(10.0f);
//#endif // _DEBUG
//
//}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void Object::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("ObjectInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop		= 1000;
		float limitBottom	= -1000;

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//�ʒu
		ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
		ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);						//�X�P�[��
		ImGui::SliderFloat("radius", &radius, 0, limitTop);								//���a
		ImGui::SliderFloat("alpha", &alpha, 0, 255);									//���ߒl
		ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//���x
		ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//�����x
		ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);					//�d��

		ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
		ImGui::Checkbox("onActive", &onActive);											//�A�N�e�B�u���t���O
		ImGui::Checkbox("onRender", &onRender);											//�`��L�����t���O
		ImGui::Checkbox("onLighting", &onLighting);										//���������t���O
		ImGui::Checkbox("onTransparent", &onTransparent);								//���߃t���O
		ImGui::Checkbox("operationAlpha", &operationAlpha);								//���ߒl�̑���L���t���O
		
		ImGui::SliderInt("renderNum", &renderNum,1,(int)limitTop);						//���ߒl�̑���L���t���O
	}
#endif // _DEBUG
}

void Object::setGravity(D3DXVECTOR3 source, float power)
{
	D3DXVec3Normalize(&gravity, &source);
	//gravity *= min(power, reverseAxisY.distance);
	gravity *= power;
	
	if (onGravity)acceleration += gravity;
}

void Object::activation()
{
	onRender = true;
	onActive = true;
}

void Object::inActivation()
{
	onRender = false;
	onActive = false;
}

void Object::setAlpha(float value)
{
	alpha = value;
}

void Object::switchTransparent(bool flag)
{
	onTransparent = flag;
}

void Object::switchOperationAlpha(bool flag)
{
	operationAlpha = flag;
}

void Object::setRenderFlag(bool flag)
{
	onRender = flag;
}