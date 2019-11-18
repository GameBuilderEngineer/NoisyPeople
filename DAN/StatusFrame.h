//===========================================
//�X�^�b�c�t���[���N���X[StatusFrame.h]
//����W��
//===========================================
#pragma once

//===========================================
//�C���N���[�h
//===========================================
#include "BasicUI.h"
#include "VirtualController.h"


//===========================================
//���O���
//===========================================
namespace statusFrameNS
{
	const int WIDTH_STATUSFLAME = (672 * WINDOW_WIDTH / 1920);	//�X�^�b�c�t���[���̉���
	const int HEIGHT_STATUSFLAME = (143 * WINDOW_HEIGHT / 1080);	//�X�b�^�c�t���[���̏c��
	const D3DXVECTOR3 POSITION_STATUSFLAME01 = D3DXVECTOR3(350 * WINDOW_WIDTH / 1920, 100 * WINDOW_HEIGHT / 1080, 0);	//�X�b�^�c�t���[���̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_STATUSFLAME02 = D3DXVECTOR3(1570 * WINDOW_WIDTH / 1920, 100 * WINDOW_HEIGHT / 1080, 0);//�X�b�^�c�t���[���̕\���ʒu(��)
}

//===========================================
//�N���X��`
//===========================================
class StatusFrame:public BasicUI
{
public://�����o�[�ϐ�
	Sprite * statusFrame;

private:
public://�����o�[�֐�
	StatusFrame();
	~StatusFrame();
	void initialize(int playerType);
	void render();
	void update();
};