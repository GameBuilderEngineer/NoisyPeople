//========================================
//���U���gBG�N���X[ResultBG.h]
//����W��
//========================================
#pragma once

//========================================
//�C���N���[�h
//========================================
#include "BasicUI.h"

//========================================
//���O���
//========================================
namespace resultBGNS
{
	const int WIDTH_BG = WINDOW_WIDTH;//����
	const int HEIGHT_BG = WINDOW_HEIGHT;//�c��
	const D3DXVECTOR3 BG_POSITION = D3DXVECTOR3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);

}

//========================================
//�N���X��`
//========================================
class ResultBG :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * resultBG;		//�X�v���C�g
private:
public://�����o�[�֐�
	ResultBG();				//�R���X�g���N�^
	~ResultBG();			//�f�X�g���N�^
	void initialize();		//������
	void render();			//�`��
	void update();			//�X�V
private:
};