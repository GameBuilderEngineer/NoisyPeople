//========================================
//�p�[�Z���g�N���X[ResultBG.h]
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
namespace persentNS
{
	const int WIDTH_PERSENT = (115*WINDOW_WIDTH/1920);//����
	const int HEIGHT_PERSENT = (124*WINDOW_HEIGHT/1080);//�c��
	const D3DXVECTOR3 PERSNT_POSITION = D3DXVECTOR3(1250 * WINDOW_WIDTH / 1920, 220 *WINDOW_HEIGHT / 1080, 0);
}

//========================================
//�N���X��`
//========================================
class Persent :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * persent;		//�X�v���C�g
private:
public://�����o�[�֐�
	Persent();				//�R���X�g���N�^
	~Persent();				//�f�X�g���N�^
	void initialize();		//������
	void render();			//�`��
	void update();			//�X�V
private:
};
