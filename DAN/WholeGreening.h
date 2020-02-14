//=========================================
// �S�̗Ή������N���X
//����W��
//=========================================
#pragma once

//=========================================
//�C���N���[�h
//=========================================
#include "BasicUI.h"

//=========================================
//���O���
//=========================================
namespace wholeGreenig_CharaNS
{
	//�Ή��{��(�p��)
	const int WIDTH_GREENIG = (256 * WINDOW_WIDTH / 1920);		//��
	const int HEIGHT_GREENIG = (128 * WINDOW_HEIGHT / 1080);		//����
	const D3DXVECTOR3 POSITION_GREENIG = D3DXVECTOR3(750 * WINDOW_WIDTH / 1920, 220 * WINDOW_HEIGHT / 1080, 0);//�v���C���[�P�p�����ʒu

}

//=========================================
//�N���X��`
//=========================================
class WholeGreening : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * greening;
private:
public:
	WholeGreening();		//�R���X�g���N�^
	~WholeGreening();	//�f�X�g���N�^
	void initialize();
	void render();
	void update();
private:
};

