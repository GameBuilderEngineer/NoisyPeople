//-----------------------------------------------------------------------------
// �i�r�Q�[�V�������b�V�������N���X [NavigationMesh.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/5
//-----------------------------------------------------------------------------
#include "NavigationMesh.h"
#include "ImguiManager.h"
#include "Direct3D9.h"
using namespace meshDataNS;
using namespace navigationMeshNS;

NavigationMesh* NavigationMesh::pointer = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
NavigationMesh::NavigationMesh(StaticMesh* _staticMesh)
{
	pointer = this;
	staticMesh = _staticMesh;
}

//=============================================================================
// ����������
//=============================================================================

void NavigationMesh::initialize()
{
	vtxAccessor.initialize(staticMesh);
	meshData.initialize(staticMesh, &vtxAccessor);
	aStar.initialize(&meshData, &vtxAccessor);
	pathFollowing.initialize(&meshData, &vtxAccessor);	
	gravityDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	// �d�͕�����ݒ�

#ifdef _DEBUG
	position = D3DXVECTOR3(0.0f, FLOATING_HEIGHT, 0.0f);// ������������
	renderer = new StaticMeshRenderer(staticMesh);		// �`��I�u�W�F�N�g���쐬
	renderer->registerObject(this);						// �`��I�u�W�F�N�g�ɓo�^����
	changeColor();										// ���b�V���̐F��ς���
#endif //_DEBUG
}


//=============================================================================
// �I������
//=============================================================================
void NavigationMesh::uninitialize()
{
	// edgeList��NavigationMesh�N���X�ł͂Ȃ�
	// pathSearch()���Ă񂾃N���X���j�����s��

#ifdef _DEBUG
	renderer->allUnRegister();
	SAFE_DELETE(renderer);
#endif //_DEBUG
}


//=============================================================================
// �X�V����
//=============================================================================
void NavigationMesh::update()
{

}


//=============================================================================
// �o�H�T��
// [����1] D3DXVECTOR3 from�@���ݒn���W
// [����2] D3DXVECTOR3 dest�@�ړI�n���W
//=============================================================================
NAVIRESULT NavigationMesh::pathSearch(LinkedList<meshDataNS::Index2>** pOut,
	DWORD* faceIndex, D3DXVECTOR3 _from, D3DXVECTOR3 _dest)
{
	// �o�H�����X�V
	from = _from;
	dest = _dest;
	startIndex = -1;
	destIndex = -1;
	edgeList = NULL;

	// ���C�̏����i�t�}�g���N�X�Ń��b�V���̈ʒu�E��]�����C�ɓK�p�j
	D3DXMATRIX Inv;
	D3DXVECTOR3 rayPos;
	BOOL hit;
	D3DXMatrixInverse(&Inv, NULL, &matrixWorld);

	// ���ݒn���i�r���b�V���ɐڒn���邩�`�F�b�N
	rayPos = _from;
	D3DXVec3TransformCoord(&rayPos, &rayPos, &Inv);
	D3DXIntersect(staticMesh->mesh, &rayPos, &gravityDirection,
		&hit, &startIndex, NULL, NULL, NULL, NULL, NULL);
	*faceIndex = startIndex;								// �Ăяo�����̃C���f�b�N�X���X�V
	if (hit == false) { return CURRENT_NOT_ON_MESH; }		// ���ݒn���i�r���b�V����ł͂Ȃ�

	// �ړI�n���i�r���b�V���ɐڒn���邩�`�F�b�N
	rayPos = _dest;
	D3DXVec3TransformCoord(&rayPos, &rayPos, &Inv);
	D3DXIntersect(staticMesh->mesh, &rayPos, &gravityDirection,
		&hit, &destIndex, NULL, NULL, NULL, NULL, NULL);
	if (hit == false) { return DESTINATION_NOT_ON_MESH; }	// �ړI�n���i�r���b�V����ł͂Ȃ�

	// A*�A���S���Y���ɂ��o�H�T���ŃG�b�W���X�g���擾����
	edgeList = aStar.pathSearch(startIndex, destIndex, from, dest);
	if (edgeList == NULL) { return IMPASSE; }				// �܏��H�̂��߃p�X�������s

	// �G�b�W���X�g���X�V���ԋp����
	edgeList->listUpdate();
	*pOut = edgeList;

	return NAVI_OK;
}


//=============================================================================
// �X�e�A�����O�i�ړ��x�N�g���̍쐬�j
//=============================================================================
NAVIRESULT NavigationMesh::steering(D3DXVECTOR3* out, DWORD* faceIndex, D3DXVECTOR3 _position,
	LinkedList<meshDataNS::Index2>* _edgeList)
{
	// ���C�̏����i�t�}�g���N�X�Ń��b�V���̈ʒu�E��]�����C�ɓK�p�j
	D3DXMATRIX Inv;
	D3DXVECTOR3 rayPos;
	BOOL hit;
	DWORD currentIndex;
	float distance;
	D3DXMatrixInverse(&Inv, NULL, &matrixWorld);

	// ���C�Ńi�r���b�V���ɐڒn���邩�`�F�b�N
	rayPos = _position;
	D3DXVec3TransformCoord(&rayPos, &rayPos, &Inv);
	D3DXIntersect(staticMesh->mesh, &rayPos, &gravityDirection,
		&hit, &currentIndex, NULL, NULL, &distance, NULL, NULL);
	if (hit == false) { return CURRENT_NOT_ON_MESH; }	// ���ݒn���i�r���b�V����ł͂Ȃ�

	// �|���S���ʃC���f�b�N�X���ς��΃G�b�W�����X�g����m�[�h���폜��ɍX�V
	if (currentIndex != *faceIndex)
	{
		*faceIndex = currentIndex;
		_edgeList->removeFront();
		_edgeList->listUpdate();
	}

	// �|���S���ʃC���f�b�N�X�����������Ȃ�����ēx�o�H�T�����čĎ��s
	// �����̉����߂�
	// A*�̌o�H�m�F
	  
	// �ڒn���W�����ƂɃp�X�t�H���[�C���O�����s
	D3DXVECTOR3 surfaceIntersection = _position + gravityDirection * distance;
	pathFollowing.createVector(out, surfaceIntersection, faceIndex, edgeList);
	return NAVI_OK;
}


#ifdef _DEBUG
//=============================================================================
// �G�b�W���X�g���t�@�C���Ƀ_���v
//=============================================================================
void NavigationMesh::dumpEdgeList()
{
	FILE* fp;
	setDataDirectory();

	fp = fopen("EdgeList.txt", "w");
	fprintf(fp, "�G�b�W���X�g�̒��g\n�o�͌��FNavigationMesh.cpp\n");

	edgeList->listUpdate();
	for (int i = 0; i < edgeList->nodeNum; i++)
	{
		Index2 vtx = *edgeList->getValue(i);
		fprintf(fp, "%d, %d\n", vtx.index[0], vtx.index[1]);
	}
	fclose(fp);
}


//=============================================================================
// �f�o�b�O�`��
//=============================================================================
void NavigationMesh::debugRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#if 1	// ���b�V����`�悷��
	debugRenderMesh(view, projection, cameraPosition);
#endif
}


//=============================================================================
// ���b�V����`��
//=============================================================================
void NavigationMesh::debugRenderMesh(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#if 0	// StaticMeshRenderer�ɂ��`��
	renderer->update();
	renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
#endif

#if 1	// DrowSubset�ɂ��`��
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = getDevice();

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMatrixTranslation(&matrixWorld, position.x, position.y, position.z);
	device->SetTransform(D3DTS_WORLD, &matrixWorld);

	// �����_�[�X�e�[�g�̐ݒ�
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_FILLMODE, objectNS::SOLID);
	//device->SetRenderState(D3DRS_FILLMODE, objectNS::WIREFRAME);
	//device->SetRenderState(D3DRS_FILLMODE, objectNS::POINT);

	// ���݂̃}�e���A�����擾
	device->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*) staticMesh->bufferMaterial->GetBufferPointer();

	// ���u�����h��L����
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`���̃����Ⴂ�ꍇ�f�t�H���g�̃u�����h���ƃ|���S���������邽�߃|���S���̃��������̗p����
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// �|���S���ƃe�N�X�`���̐F��������
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	for (int nCntMat = 0; nCntMat < (int)staticMesh->numMaterial; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		device->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		device->SetTexture(0, *staticMesh->textures);

		// �`��
		staticMesh->mesh->DrawSubset(nCntMat);
	}

	// ���u�����h�𖳌���
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// �}�e���A�����f�t�H���g�ɖ߂�
	device->SetMaterial(&matDef);
#endif
}


//=============================================================================
// �G�b�W��`��
//=============================================================================
void NavigationMesh::debugRenderEdge(LinkedList<meshDataNS::Index2>* _edgeList)
{
	if (_edgeList == NULL) return;
	_edgeList->listUpdate();

	// ���C�̔z����쐬
	Ray* line = new Ray[_edgeList->nodeNum];

	for (int i = 0; i < _edgeList->nodeNum; i++)
	{
		// �C���f�b�N�X�̒��_���W���擾
		BYTE* p1 = meshData.getVertexPointerFromVertexIndex(edgeList->getValue(i)->index[0]);
		BYTE* p2 = meshData.getVertexPointerFromVertexIndex(edgeList->getValue(i)->index[1]);
		D3DXVECTOR3 tempPos1 = *(D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p1);
		D3DXVECTOR3 tempPos2 = *(D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p2);
		tempPos1 += D3DXVECTOR3(0.0f, FLOATING_HEIGHT + 0.1f, 0.0f);	
		tempPos2 += D3DXVECTOR3(0.0f, FLOATING_HEIGHT + 0.1f, 0.0f);
		// ���C�Ƀp�����[�^���Z�b�g
		line[i].start = tempPos1;
		line[i].direction = tempPos2 - tempPos1;
		D3DXVec3Normalize(&line[i].direction, &line[i].direction);
		float len = D3DXVec3Length(&(tempPos2 - tempPos1));
		line[i].color = D3DXCOLOR(0, 0, 255, 255);
		// �`��
		line[i].render(len);
	}

	// ���C�̔z���j��
	SAFE_DELETE_ARRAY(line);
}


//=============================================================================
// ���b�V���̐F��ς���
//=============================================================================
void NavigationMesh::changeColor()
{
	D3DCOLOR setting = D3DCOLOR_RGBA(255, 255, 255, 155);

	// ���_�o�b�t�@�̏���
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	BYTE* pVtx;

	// ���_�J���[������������
	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		for (int i = 0; i < meshData.getNumVertices(); i++)
		{
			D3DCOLOR* pColor = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, &pVtx[i * meshData.getStride()]);
			*pColor = setting;
		}
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", NULL, MB_OK);
		return;
	}
}


//=============================================================================
// �G�b�W�̒��_�ɉe����^����
//=============================================================================
void NavigationMesh::affectToEdgeVertex()
{
	if (edgeList == NULL) return;
	edgeList->listUpdate();
	D3DCOLOR red = D3DCOLOR_RGBA(255, 0, 0, 255);

	// ���_�o�b�t�@�̏���
	LPDIRECT3DVERTEXBUFFER9	vertexBuffer;
	staticMesh->mesh->GetVertexBuffer(&vertexBuffer);
	BYTE* pVtx;

	if (SUCCEEDED(vertexBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		for (int i = 0; i < edgeList->nodeNum; i++)
		{
			// �C���f�b�N�X���璸�_���擾
			WORD index1 = edgeList->getValue(i)->index[0];
			WORD index2 = edgeList->getValue(i)->index[1];
			BYTE* p1 = meshData.getVertexPointerFromVertexIndex(index1, pVtx);
			BYTE* p2 = meshData.getVertexPointerFromVertexIndex(index2, pVtx);

			// ���_�J���[������������
			D3DCOLOR* pColor1 = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, p1);
			D3DCOLOR* pColor2 = (D3DCOLOR*)vtxAccessor.getPointer(vtxAccess::COLOR, p2);
			*pColor1 = *pColor2 = red;

			//// ���_���{�R�{�R���N
			//D3DXVECTOR3* pPos1= (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p1);
			//D3DXVECTOR3* pPos2 = (D3DXVECTOR3*)vtxAccessor.getPointer(vtxAccess::POSITION, p2);
			//*pPos1 += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//*pPos2 += D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//// UV���W������
			//D3DXVECTOR2* pUv1 = (D3DXVECTOR2*)vtxAccessor.getPointer(vtxAccess::UV, p1);
			//D3DXVECTOR2* pUv2 = (D3DXVECTOR2*)vtxAccessor.getPointer(vtxAccess::UV, p2);
			//*pUv1 *= 0;
			//*pUv2 *= 0;

		}
		vertexBuffer->Unlock();
	}
	else
	{
		MessageBox(0, "���_�o�b�t�@�̃��b�N�Ɏ��s���܂���", NULL, MB_OK);
		return;
	}
}


//=============================================================================
// ImGUI�ւ̏o��
//=============================================================================
void NavigationMesh::outputGUI()
{
	if (ImGui::CollapsingHeader("NaviAIInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("from(%.3f, %.3f, %.3f)\n", from.x, from.y, from.z);
		ImGui::Text("from-->faceIndex:%d\n", startIndex);
		ImGui::Text("dest(%.3f, %.3f, %.3f)\n", dest.x, dest.y, dest.z);
		ImGui::Text("dest-->faceIndex:%d\n", destIndex);
		if(edgeList) ImGui::Text("edgeList->nodeNum:%d\n", edgeList->nodeNum);
	}
}
#endif // _DEBUG
