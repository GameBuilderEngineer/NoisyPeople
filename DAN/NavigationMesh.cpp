//-----------------------------------------------------------------------------
// �i�r�Q�[�V�������b�V�������N���X [NavigationMesh.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/5
//-----------------------------------------------------------------------------
#include "NavigationMesh.h"
#include "ImguiManager.h"
#include "Direct3D9.h"


//=============================================================================
// �R���X�g���N�^
//=============================================================================
NavigationMesh::NavigationMesh(StaticMesh* _staticMesh)
{
	staticMesh = _staticMesh;
}


//=============================================================================
// ����������
//=============================================================================
void NavigationMesh::initialize()
{
	meshData.initialize(staticMesh);
	vtxAccessor.initialize(staticMesh);
	aStar.initialize(&meshData, &vtxAccessor);
}


//=============================================================================
// �I������
//=============================================================================
void NavigationMesh::uninitialize()
{

}


//=============================================================================
// �X�V����
//=============================================================================
void NavigationMesh::update()
{

}


//=============================================================================
// �o�H�T��
// [�@�\] 2�n�_���Ȃ��o�H�����߂�
// [����] from�F���ݒn���W�@dest�F�ړI�n���W�@flag�F���ݒn/�ړI�n�̌�����@
// STANDARD �������W�ɍł��߂��|���S��
// GRAVITY  �������W����d�͕����Ƀ��C�L���X�g���ďՓ˂����|���S��
//=============================================================================
void NavigationMesh::pathSearch(D3DXVECTOR3 from, D3DXVECTOR3 dest, SEARCH_FLAG flag)
{
	// ���[���h�}�g���N�X�̋t�}�g���N�X�Ŏw�����W�����[�J�����W�ɕϊ�����

	// �d�͕���
	//D3DXIntersect(targetMesh, &start, &direction, &hit, &index, NULL, NULL, &distance, NULL, NULL);
	// �d�͕����͌��X�����Ă���
	
	DWORD startFaceIndex, destFaceIndex;
	
}


#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�`��
//=============================================================================
void NavigationMesh::debugRender(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{

	//renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);

	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = getDevice();

	// ���[���h�}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_WORLD, &matrixWorld);

	// �����_�[�X�e�[�g�̐ݒ�
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_FILLMODE, objectNS::SOLID);

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
}


//=============================================================================
// ImGUI�ւ̏o��
//=============================================================================
void NavigationMesh::outputGUI()
{
	//ImGui::Text(sceneName.c_str());

	if (ImGui::CollapsingHeader("NaviAIInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;

		ImGui::Text("speedVectorLength %f", D3DXVec3Length(&speed));


		ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
		ImGui::Checkbox("onActive", &onActive);											//�A�N�e�B�u���t���O
	}
}
#endif // _DEBUG


// Astar�ł̓G�b�W�̏����������āA���̃G�b�W�̂ǂ���ړI�n�Ƃ��邩�A�͕ʏ����iAI�����ɏ����Ă���j