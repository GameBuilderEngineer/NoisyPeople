//===================================================================================================================================
//�yDirect3D9.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/17
// [�X�V��]2019/11/09
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Direct3D9.h"

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
static LPDIRECT3DDEVICE9* pointerDevice = NULL;
static Direct3D9* pointerDirect3D9 = NULL;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Direct3D9::Direct3D9()
{
	ZeroMemory(this, sizeof(Direct3D9));
	pointerDevice = &this->device;
	pointerDirect3D9 = this;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Direct3D9::~Direct3D9()
{
	pointerDevice = NULL;
	pointerDirect3D9 = NULL;
	SAFE_RELEASE(device);
	SAFE_RELEASE(d3d);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
HRESULT Direct3D9::initialize(HWND targetWnd)
{
	//Direct3D�I�u�W�F�N�g�̍쐬
	if (NULL == (d3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MSG("Direct3D�̍쐬�Ɏ��s���܂���");
		return E_FAIL;
	}
	//�uDIRECT3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	//d3dpp.BackBufferFormat			= D3DFMT_UNKNOWN;
	D3DDISPLAYMODE dMode;
	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dMode);
	d3dpp.BackBufferFormat			= dMode.Format;
	d3dpp.BackBufferCount			= 1;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed					= false;
	d3dpp.Windowed					= true;
	d3dpp.BackBufferWidth			= WINDOW_WIDTH;
	d3dpp.BackBufferHeight			= WINDOW_HEIGHT;
	d3dpp.EnableAutoDepthStencil	= true;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;

	if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, targetWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &device)))
	{
		MessageBox(0, "HARDWARE���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nSOFTWARE���[�h�ōĎ��s���܂�", NULL, MB_OK);
		if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, targetWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &device)))
		{
			MessageBox(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
			if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, targetWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &device)))
			{
				if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, targetWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &device)))
				{
					MessageBox(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	device->SetRenderState(D3DRS_ZENABLE, true);					//Z�o�b�t�@�[������L���ɂ���
	device->SetRenderState(D3DRS_LIGHTING, true);					//���C�g��L���ɂ���
	device->SetRenderState(D3DRS_AMBIENT, 0x22111111);				//�A���r�G���g���C�g�i�����j��ݒ肷��
	device->SetRenderState(D3DRS_SPECULARENABLE, true);				//�X�y�L�����i���򔽎ˁj��L���ɂ���
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//�J�����O�̖�����
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			//�����v���J�����O�L����
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);			//���v���J�����O�L����
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);		//�A���`�G�C���A�V���O��������
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			//���u�����h���s��
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//���\�[�X�J���[�̎w��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//���f�X�e�B�l�[�V�����J���[�̎w��
	device->GetViewport(&viewPort);									//�r���[�|�[�g���擾
	device->GetRenderTarget(0,&backBuffer);							//�o�b�N�o�b�t�@���擾
	device->GetDepthStencilSurface(&zBuffer);						//Z�o�b�t�@���擾
	return S_OK;
}

//===================================================================================================================================
//�yFPS�\�������z
//===================================================================================================================================
void Direct3D9::showFPS()
{
	static INT frames = 0, FPS = 0;
	static LARGE_INTEGER frq = { 0 }, previous = { 0 }, current = { 0 };
	DOUBLE time = 0;
	//char sz[11] = { 0 };

	QueryPerformanceFrequency(&frq);

	QueryPerformanceCounter(&current);
	time = (DOUBLE)(current.QuadPart - previous.QuadPart);
	time *= (DOUBLE)1100.0 / (DOUBLE)frq.QuadPart;
	if (time >= 1100.000)
	{
		previous = current;
		FPS = frames;
		frames = 0;
	}

	frames++;
}

//===================================================================================================================================
//�y��ʃN���A�����z
//===================================================================================================================================
void Direct3D9::clear()
{
	//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}
void Direct3D9::clear(D3DCOLOR color)
{
	//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

//===================================================================================================================================
//�y��ʍX�V�����z
//===================================================================================================================================
HRESULT Direct3D9::present()
{
	//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�ɂ���j
	device->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

//===================================================================================================================================
//�y�`��J�n�����z
//===================================================================================================================================
HRESULT Direct3D9::beginScene()
{
	if (device == NULL)return E_FAIL;
	return device->BeginScene();//�`��̂��߂̃V�[�����J�n����
}

//===================================================================================================================================
//�y�`��I�������z
//===================================================================================================================================
HRESULT Direct3D9::endScene()
{
	if (device == NULL)return E_FAIL;
	return device->EndScene();
}

//===================================================================================================================================
//�y�`���ؑ֏����z
//[�ؑ֐�]�E�B���h�E�S��
//===================================================================================================================================
HRESULT Direct3D9::changeViewportFullWindow()
{
	changeViewport(0, 0, viewPort.Width, viewPort.Height);
	return S_OK;
}

//===================================================================================================================================
//�y�`���ؑ֏����z
//[�ؑ֐�]�E�B���h�E����
//===================================================================================================================================
HRESULT Direct3D9::changeViewport1PWindow()
{
	changeViewport(0, 0, viewPort.Width / 2, viewPort.Height);
	return S_OK;
}

//===================================================================================================================================
//�y�`���ؑ֏����z
//[�ؑ֐�]�E�B���h�E�E��
//===================================================================================================================================
HRESULT Direct3D9::changeViewport2PWindow()
{
	changeViewport(viewPort.Width / 2, 0, viewPort.Width / 2, viewPort.Height);
	return S_OK;
}

//===================================================================================================================================
//�y�r���[�|�[�g�ؑ֏����z
//[�p�r]�r���[�|�[�g��؂�ւ��邱�Ƃŕ`����؂�ւ���
//===================================================================================================================================
HRESULT Direct3D9::changeViewport(DWORD x, DWORD y, DWORD width, DWORD height)
{
	D3DVIEWPORT9 vp;

	vp.X = x;
	vp.Y = y;
	vp.Width = width;
	vp.Height = height;
	vp.MinZ = 0;
	vp.MaxZ = 1;

	MFAIL(device->SetViewport(&vp), "�r���[�|�[�g�؂�ւ����s");

	return S_OK;
}

//===================================================================================================================================
//�y�o�b�N�o�b�t�@�փ����_�[�^�[�Q�b�g����z
//===================================================================================================================================
void Direct3D9::setRenderBackBuffer(DWORD index)
{
	device->SetRenderTarget(index, backBuffer);
	device->SetDepthStencilSurface(zBuffer);
}

//===================================================================================================================================
//�yDirect3D�f�o�C�X�擾�֐��z
//===================================================================================================================================
LPDIRECT3DDEVICE9 getDevice() { return *pointerDevice; }

//===================================================================================================================================
//�yDirect3D9�N���X�擾�֐��z
//===================================================================================================================================
Direct3D9* getDirect3D9() { return pointerDirect3D9; }
