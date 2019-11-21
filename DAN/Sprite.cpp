//=============================================================================
// �X�v���C�g���� [Sprite.cpp]
//
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�� : 2019/9/19
//=============================================================================
#include "Sprite.h"
#include "Direct3D9.h"
using namespace SpriteNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Sprite::Sprite()
{
	onReadFile = false;
	texture = NULL;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Sprite::~Sprite()
{
	if (onReadFile)	SAFE_RELEASE(texture);
}


//=============================================================================
// �����������i�e�N�X�`�����t�@�C������ǂݍ��ށj
//=============================================================================
void Sprite::initialize(const char* fileName, int _pivot,
	int _width, int _height, D3DXVECTOR3 _position, D3DXVECTOR3 _rotation, D3DCOLOR color)
{
	if (fileName)
	{
		setTextureFromFile(fileName);
	}

	setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
	setPivot(_pivot);
	setSize(_width, _height);
	initialRadius = radius;
	position = _position;
	rotation = _rotation;
	setVertex();
	setColorAllVertex(color);
	vertexWk[0].rhw = vertexWk[1].rhw = vertexWk[2].rhw = vertexWk[3].rhw = 1.0f;
}


//=============================================================================
// �����������i�e�N�X�`�����|�C���^�œn���j
//=============================================================================
void Sprite::initialize(LPDIRECT3DTEXTURE9 _texture, int _pivot,
	int _width, int _height, D3DXVECTOR3 _position, D3DXVECTOR3 _rotation, D3DCOLOR color)
{
	if (_texture)
	{
		setTexturePointer(_texture);
	}

	setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
	setPivot(_pivot);
	setSize(_width, _height);
	initialRadius = radius;
	position = _position;
	rotation = _rotation;
	setVertex();
	setColorAllVertex(color);
	vertexWk[0].rhw = vertexWk[1].rhw = vertexWk[2].rhw = vertexWk[3].rhw = 1.0f;
}


//=============================================================================
// �`�揈��
//=============================================================================
void Sprite::render()
{
	LPDIRECT3DDEVICE9 device = getDevice();								//�f�o�C�X�̎擾

	// ���e�X�g��L����
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// ���ߏ����̐ݒ�
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// ���ߏ����̐ݒ�
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ���ߏ����̐ݒ�

	device->SetFVF(FVF_VERTEX_2D);
	device->SetTexture(0, texture);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(SpriteNS::Vertex2D));

	// ���e�X�g�𖳌���
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//=============================================================================
// �e�N�X�`�����t�@�C������ǂݍ���
//=============================================================================
HRESULT Sprite::setTextureFromFile(const char* fileName)
{
	LPDIRECT3DDEVICE9 device = getDevice();	//�f�o�C�X�̎擾

	//setVisualDirectory();	// �J�����g�f�B���N�g���̕ύX

	if (FAILED(D3DXCreateTextureFromFile(device, fileName, &texture)))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", fileName, MB_OK);
		return E_FAIL;
	}
	onReadFile = true;
	return S_OK;
}


//=============================================================================
// �e�N�X�`�����|�C���^�ŃZ�b�g����
//=============================================================================
void Sprite::setTexturePointer(LPDIRECT3DTEXTURE9 _texture)
{
	texture = _texture;
}


//=============================================================================
// UV���W��ݒ�
//=============================================================================
void Sprite::setUVCoord(D3DXVECTOR2 v0, D3DXVECTOR2 v1, D3DXVECTOR2 v2, D3DXVECTOR2 v3)
{
	vertexWk[0].uv = v0;
	vertexWk[1].uv = v1;
	vertexWk[2].uv = v2;
	vertexWk[3].uv = v3;
}


//=============================================================================
// �T�C�Y��ݒ�
//=============================================================================
void Sprite::setSize(int _width, int _height)
{
	width = _width;
	height = _height;

	if (pivot == CENTER)
	{
		// ���S���W���璸�_�ւ̊p�x��������
		centralAngle = atan2f((float)height / 2, (float)width / 2);	
		// ���S���W���璸�_�ւ̃x�N�g�������߂�
		D3DXVECTOR2 vec = D3DXVECTOR2((float) width / 2, (float)height / 2);
		// ���a�̏�����
		radius = D3DXVec2Length(&vec);
	}
	else
	{
		// �O�����̒藝
		radius = sqrtf((float)(width * width + height * height));
	}
}


//=============================================================================
// ���_���W��ݒ�
//=============================================================================
void Sprite::setVertex()
{
	switch (pivot)
	{
	case CENTER:
		vertexWk[0].vtx.x = position.x - cosf(centralAngle + rotation.z) * radius;
		vertexWk[0].vtx.y = position.y - sinf(centralAngle + rotation.z) * radius;
		vertexWk[1].vtx.x = position.x + cosf(centralAngle - rotation.z) * radius;
		vertexWk[1].vtx.y = position.y - sinf(centralAngle - rotation.z) * radius;
		vertexWk[2].vtx.x = position.x - cosf(centralAngle - rotation.z) * radius;
		vertexWk[2].vtx.y = position.y + sinf(centralAngle - rotation.z) * radius;
		vertexWk[3].vtx.x = position.x + cosf(centralAngle + rotation.z) * radius;
		vertexWk[3].vtx.y = position.y + sinf(centralAngle + rotation.z) * radius;
		break;

	case TOP_LEFT:
		vertexWk[0].vtx.x = position.x;
		vertexWk[0].vtx.y = position.y;
		vertexWk[1].vtx.x = position.x + cosf(rotation.z) * width;
		vertexWk[1].vtx.y = position.y + sinf(rotation.z) * width;
		vertexWk[2].vtx.x = position.x - sin(rotation.z) * height;
		vertexWk[2].vtx.y = position.y + cos(rotation.z) * height;
		vertexWk[3].vtx.x = position.x + cosf(rotation.z) * width - sin(rotation.z) * height;
		vertexWk[3].vtx.y = position.y + sinf(rotation.z) * width + cos(rotation.z) * height;
		break;
	case TOP_CENTER:
		break;
	case TOP_RIGHT://�����A��]���邱�Ƃ͍l�����ĂȂ��ł�
		vertexWk[0].vtx.x = position.x - width;
		vertexWk[0].vtx.y = position.y ;
		vertexWk[1].vtx.x = position.x;
		vertexWk[1].vtx.y = position.y;
		vertexWk[2].vtx.x = position.x - width;
		vertexWk[2].vtx.y = position.y + height;
		vertexWk[3].vtx.x = position.x ;
		vertexWk[3].vtx.y = position.y + height;
		break;

	}
	vertexWk[0].vtx.z = 0.0f;
	vertexWk[1].vtx.z = 0.0f;
	vertexWk[2].vtx.z = 0.0f;
	vertexWk[3].vtx.z = 0.0f;
}


//=============================================================================
// ���_�J���[�𒸓_�ʂɐݒ�
//=============================================================================
void Sprite::setColor(D3DCOLOR v0, D3DCOLOR v1, D3DCOLOR v2, D3DCOLOR v3)
{
	vertexWk[0].diffuse = v0;
	vertexWk[1].diffuse = v1;
	vertexWk[2].diffuse = v2;
	vertexWk[3].diffuse = v3;
}


//=============================================================================
// ���_�J���[���ꊇ�ݒ�
//=============================================================================
void Sprite::setColorAllVertex(D3DCOLOR color)
{
	vertexWk[0].diffuse =
	vertexWk[1].diffuse =
	vertexWk[2].diffuse =
	vertexWk[3].diffuse = color;
}


//=============================================================================
// ���l�𒸓_�ʂɐݒ�
//=============================================================================
void Sprite::setAlpha(int v0, int v1, int v2, int v3)
{
	vertexWk[0].diffuse &= 0x00ffffff;
	vertexWk[1].diffuse &= 0x00ffffff;
	vertexWk[2].diffuse &= 0x00ffffff;
	vertexWk[3].diffuse &= 0x00ffffff;

	vertexWk[0].diffuse |= ((v0) & 0xff) << 24;
	vertexWk[1].diffuse |= ((v1) & 0xff) << 24;
	vertexWk[2].diffuse |= ((v2) & 0xff) << 24;
	vertexWk[3].diffuse |= ((v3) & 0xff) << 24;
}


//=============================================================================
// ���l���ꊇ�ݒ�
//=============================================================================
void Sprite::setAlphaAllVertex(int _alpha)
{
	vertexWk[0].diffuse &= 0x00ffffff;
	vertexWk[1].diffuse &= 0x00ffffff;
	vertexWk[2].diffuse &= 0x00ffffff;
	vertexWk[3].diffuse &= 0x00ffffff;

	vertexWk[0].diffuse |= ((_alpha) & 0xff) << 24;
	vertexWk[1].diffuse |= ((_alpha) & 0xff) << 24;
	vertexWk[2].diffuse |= ((_alpha) & 0xff) << 24;
	vertexWk[3].diffuse |= ((_alpha) & 0xff) << 24;
}


//=============================================================================
// ���W��ݒ�
//=============================================================================
void Sprite::setPosition(D3DXVECTOR3 _position)
{
	position = _position;

}


//=============================================================================
// ��]��ݒ�
//=============================================================================
void Sprite::setRotation(D3DXVECTOR3 _rotation)
{ 
	rotation = _rotation;
}


//=============================================================================
// ��]����ݒ�
//=============================================================================
void Sprite::setPivot(int _pivot)
{
	pivot = _pivot;
}


//=============================================================================
// ���a��ݒ�i��]�������S�̂Ƃ��̂ݗL���j
//=============================================================================
void Sprite::setRadius(float _radius)
{
	radius = _radius;
}


//=============================================================================
// Getter
//=============================================================================
SpriteNS::Vertex2D* Sprite::getVertexPointer() { return vertexWk; }
D3DXVECTOR3	 Sprite::getPosition() { return position; }
D3DXVECTOR3* Sprite::getPositionPointer() { return &position; }
D3DXVECTOR3	Sprite::getRotation() { return rotation; }
D3DXVECTOR3* Sprite::getRotationPointer() { return &rotation; }
float Sprite::getInitialRadius() { return initialRadius; }
LPDIRECT3DTEXTURE9 Sprite::getTexture() { return texture; }

