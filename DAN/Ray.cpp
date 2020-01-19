//===================================================================================================================================
//�yRay.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/29
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Ray.h"
#ifdef _DEBUG
#include "Direct3D9.h"
#endif // _DEBUG

//===================================================================================================================================
//�y�v���g�^�C�v�錾�z
//===================================================================================================================================
HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices);

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Ray::Ray()
{
	ZeroMemory(this, sizeof(Ray));
	direction = D3DXVECTOR3(0, 0, 1);//Z����
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Ray::~Ray()
{

}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void Ray::initialize(D3DXVECTOR3 _start, D3DXVECTOR3 _direction)
{
	start = _start;
	direction = _direction;
}

//===================================================================================================================================
//�y���C�ɂ��Փ˔���z
// ���C�����胁�b�V���ƌ�������ꍇ�́ApfDistance�ɋ���������true��Ԃ�
// �������Ă���|���S�����Ԃ�
//===================================================================================================================================
bool Ray::rayIntersect(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix)
{
	BOOL hit = false;
	D3DXMATRIX Inv;
	//���C���o�����b�V���̈ʒu�E��]�����C�ɓK�p
	//���C�𓖂Ă郁�b�V���������Ă������]���Ă���ꍇ�ł��Ώۂ̃��[���h�s��̋t�s���p����ΐ��������C��������
	D3DXMatrixInverse(&Inv, NULL, &targetMatrix);
	D3DXVec3TransformCoord(&start, &start, &Inv);
	D3DXVec3Normalize(&direction, &direction);
	Inv._41 =
	Inv._42 =
	Inv._43 = 0.0f;
	D3DXVec3TransformCoord(&direction, &direction, &Inv);

	DWORD index = 0;//�|���S���̃C���f�b�N�X�ۑ��ϐ�
	//----------------------------------------------------------------------------------------------------
	//D3DXIntersect�̌���
	//index:���C�ƏՓ˂��Ă���|���S���̃C���f�b�N�X�ԍ����擾����
	//hit:���C�Ƃ̏Փ˂̌��m
	//distance:���C�ƏՓ˂����|���S���Ƃ̋���
	D3DXIntersect(targetMesh, &start, &direction, &hit, &index, NULL, NULL, &distance, NULL, NULL);
	//----------------------------------------------------------------------------------------------------
	if (hit)
	{
		//�������Ă���|���S���̒��_��������
		D3DXVECTOR3 vertex[3];
		FindVerticesOnPoly(targetMesh, index, vertex);
		D3DXPLANE plane;
		//���̒��_���畽�ʕ������𓾂�
		D3DXPlaneFromPoints(&plane, &vertex[0], &vertex[1], &vertex[2]);
		//���ʕ������̌W�����@���̐���
		normal.x = plane.a;
		normal.y = plane.b;
		normal.z = plane.c;

		return true;
	}
	return false;
}

//===================================================================================================================================
//�y�X���b�v�֐��z
// L:���˃x�N�g���i���C�j N:�|���S���̖@��
//�@���ƕ��s�ȓ��˃x�N�g��������ł�����
//===================================================================================================================================
D3DXVECTOR3 Ray::slip(D3DXVECTOR3 L, D3DXVECTOR3 N)
{
	D3DXVECTOR3 S; //����x�N�g���i��������j

	//����x�N�g�� S=L-(N * L)/(|N|^2)*N
	S = L - ((D3DXVec3Dot(&N, &L)) / (pow(D3DXVec3Length(&N), 2)))*N;

	return S;
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Ray::update(D3DXVECTOR3 _start, D3DXVECTOR3 _direction)
{
	start = _start;
	direction = _direction;
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Ray::render(float length)
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 device = getDevice();
	device->SetFVF(D3DFVF_XYZ);
	D3DXVECTOR3 vPnt[2];
	vPnt[0] = start;
	vPnt[1] = start + (direction * length);

	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	device->SetTransform(D3DTS_WORLD, &mWorld);

	// �A���t�@�E�u�����f�B���O���s��
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//���C�̃}�e���A���ݒ�
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient =
	mtrl.Diffuse = color;
	device->SetMaterial(&mtrl);
	device->SetTexture(0, NULL);
	device->SetRenderState(D3DRS_LIGHTING, true);

	//���C�̃����_�����O
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vPnt, sizeof(D3DXVECTOR3));


#endif // _DEBUG
}

//===================================================================================================================================
//�y���b�V����̃|���S���̒��_�T���z
//HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh,DWORD dwPolyIndex,D3DXVECTOR3* pvVertices )
//���̃|���S���̒��_��������
//===================================================================================================================================
HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices)
{
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVertexNum = pMesh->GetNumVertices();
	DWORD dwPolyNum = pMesh->GetNumFaces();
	WORD* pwPoly = NULL;
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pwPoly);

	BYTE *pbVertices = NULL;
	FLOAT* pfVetices = NULL;
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;
	pMesh->GetVertexBuffer(&VB);
	if (SUCCEEDED(VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pvVertices[0].x = pfVetices[0];
		pvVertices[0].y = pfVetices[1];
		pvVertices[0].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pvVertices[1].x = pfVetices[0];
		pvVertices[1].y = pfVetices[1];
		pvVertices[1].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pvVertices[2].x = pfVetices[0];
		pvVertices[2].y = pfVetices[1];
		pvVertices[2].z = pfVetices[2];

		pMesh->UnlockIndexBuffer();
		VB->Unlock();
		VB->Release();
	}
	return S_OK;
}