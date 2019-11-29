//===================================================================================================================================
//�yDebugBox.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/24
//===================================================================================================================================
#include "DebugBox.h"
#include "Direct3D9.h"


//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
DebugBox::DebugBox(D3DXVECTOR3 size)
{
	D3DXCreateBox(getDevice(), size.x, size.y, size.z, &mesh, NULL);
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
DebugBox::~DebugBox()
{
	SAFE_RELEASE(mesh);
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void DebugBox::render(D3DXMATRIX worldMatrix)
{
	LPDIRECT3DDEVICE9 device = getDevice();

	//���C�e�B���O���[�h��ݒ�
	device->SetRenderState(D3DRS_LIGHTING, true);
	// �����_�����O���[�h�̐ݒ�
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);//���C���[�t���[���\��

	//���[���h�}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_WORLD, &worldMatrix);

	// �e�N�X�`���̐ݒ��NULL�ɂ���
	device->SetTexture(0, NULL);

	// �}�e���A�����ɑ΂���|�C���^���擾
	D3DMATERIAL9 material;

	// �}�e���A������ԐF
	material.Diffuse = D3DCOLORVALUE(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	material.Ambient = D3DCOLORVALUE(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	material.Specular = D3DCOLORVALUE(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	material.Emissive = D3DCOLORVALUE(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	material.Power = 120.0f;

	// �}�e���A���̐ݒ�
	device->SetMaterial(&material);

	//�`��
	mesh->DrawSubset(0);

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//�\���b�h�\��

}
