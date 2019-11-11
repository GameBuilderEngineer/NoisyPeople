//===========================================
//�X�^�b�c�t���[���N���X[StatusFrame.h]
//����W��
//===========================================
#pragma once

//===========================================
//�C���N���[�h
//===========================================
#include "BasicUI.h"

//===========================================
//���O���
//===========================================
namespace statusFrameNS
{
	const int WIDTH_STATUSFLAME = (672);	//�X�^�b�c�t���[���̉���
	const int HEIGHT_STATUSFLAME = (143);	//�X�b�^�c�t���[���̏c��
	const D3DXVECTOR3 POSITION_STATUSFLAME01 = D3DXVECTOR3(350, 100, 0);	//�X�b�^�c�t���[���̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_STATUSFLAME02 = D3DXVECTOR3(1440, 30, 0);//�X�b�^�c�t���[���̕\���ʒu(��)
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
	void initialize();
	void render();
	void update();
};