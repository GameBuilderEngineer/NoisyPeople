//===================================================================================================================================
//�yBoundingSphere.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/24
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "BoundingSphere.h"
#include "Direct3D9.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
BoundingSphere::BoundingSphere()
{
	ZeroMemory(this, sizeof(BoundingSphere));
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
BoundingSphere::~BoundingSphere()
{
	SAFE_RELEASE(mesh);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
//���b�V����񂩂珉����
void BoundingSphere::initialize(D3DXVECTOR3* _position, LPD3DXMESH ownerMesh)
{
	LPDIRECT3DDEVICE9 device = getDevice();
	position = _position;

	// ���b�V���̒��_�o�b�t�@�����b�N����
	LPVOID buffer;
	ownerMesh->LockVertexBuffer(D3DLOCK_READONLY, &buffer);

	// ���b�V���̋��E�������߂�
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3 *)buffer,
		ownerMesh->GetNumVertices(),
		ownerMesh->GetNumBytesPerVertex(),
		&center, &radius);

	// ���b�N�������_�o�b�t�@���A�����b�N
	ownerMesh->UnlockVertexBuffer();

	//���b�V�����쐬
	D3DXCreateSphere(device, radius, 8, 8, &mesh, NULL);

}
//���a��񂩂珉����
void BoundingSphere::initialize(D3DXVECTOR3* _position,float _raidus)
{
	LPDIRECT3DDEVICE9 device = getDevice();
	position = _position;
	radius = _raidus;
	//���b�V�����쐬
	D3DXCreateSphere(device, radius, 8, 8, &mesh, NULL);
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void BoundingSphere::render(D3DXMATRIX owner)
{
	LPDIRECT3DDEVICE9 device = getDevice();

	// ���C�e�B���O���[�h��ݒ�
	device->SetRenderState(D3DRS_LIGHTING, false);
	// �����_�����O���[�h�̐ݒ�
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);//���C���[�t���[���\��
	// �A���t�@�E�u�����f�B���O���s��
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// ���ߏ������s��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// �������������s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	D3DMATERIAL9 matDef;
	// ���݂̃}�e���A�����擾
	device->GetMaterial(&matDef);

	//���[���h�}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_WORLD, &owner);

	// �e�N�X�`���̐ݒ��NULL�ɂ���
	device->SetTexture(0, NULL);

	// �}�e���A�����ɑ΂���|�C���^���擾
	D3DMATERIAL9 material;

	// �}�e���A������ΐF�ɂ��A��������
	material.Diffuse = D3DCOLORVALUE(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f));
	material.Ambient = D3DCOLORVALUE(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	material.Specular = D3DCOLORVALUE(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	material.Emissive = D3DCOLORVALUE(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	material.Power = 120.0f;

	// �}�e���A���̐ݒ�
	device->SetMaterial(&material);

	//�`��
	mesh->DrawSubset(0);

	//�}�e���A�����f�t�H���g�ɖ߂�
	device->SetMaterial(&matDef);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//���C���[�t���[���\��
}

//===================================================================================================================================
//�y�Փ˔���z
//===================================================================================================================================
bool BoundingSphere::collide(D3DXVECTOR3 targetCenter, float targetRadius, D3DXMATRIX ownerMatrix, D3DXMATRIX targetMatrix)
{
	D3DXVECTOR3 ownerCenter;
	D3DXVECTOR3 _targetCenter;
	D3DXVec3TransformCoord(&ownerCenter, &center, &ownerMatrix);
	D3DXVec3TransformCoord(&_targetCenter, &targetCenter, &targetMatrix);

	//�Q�̕��̂̒��S�Ԃ̋��������߂�
	float length = D3DXVec3Length(&(_targetCenter - ownerCenter));
	// ���̋������A�Q�̕��̂̔��a�𑫂������̂�菬�����ꍇ�A���E�����m���d�Ȃ��Ă���i�Փ˂��Ă���j�Ƃ�������
	if (length <= radius + targetRadius)
	{
		return true;
	}
	return false;
}

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================


//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
float BoundingSphere::getRadius() { return radius; }
D3DXVECTOR3 BoundingSphere::getCenter() { return center; }
LPD3DXMESH BoundingSphere::getMesh() { return mesh; }
