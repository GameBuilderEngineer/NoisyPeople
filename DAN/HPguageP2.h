//======================================
//P2HP�Q�[�W�N���X[HPguageP2.h]
//����W��
//======================================
#pragma once

//======================================
//�C���N���[�h
//======================================
#include "BasicUI.h"
#include "VirtualController.h"

//======================================
//���O���
//======================================
namespace hpGuageP2NS
{
	const int WIDTH_HP_GUAGE = (500 * WINDOW_WIDTH / 1920);	//HP�Q�[�W�̉���
	const int HEIGHT_HP_GUAGE = (30 * WINDOW_HEIGHT / 1080);//HP�Q�[�W�̏c��
	const D3DXVECTOR3 POSITION_HP_GUAGE = D3DXVECTOR3(1825 * WINDOW_WIDTH / 1920, 35 * WINDOW_HEIGHT / 1080, 0);//HP�Q�[�W�̕\���ʒu(��)

}

//======================================
//�N���X��`
//======================================
class HPguageP2 :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * hpGuage;
	int      hp;//HP�ۑ��p
	int		 width;
	//UV���W�Ǘ��p
	D3DXVECTOR2 uvCoord1;
	D3DXVECTOR2 uvCoord2;
	D3DXVECTOR2 uvCoord3;
	D3DXVECTOR2 uvCoord4;

private:
public://�����o�[�֐�
	HPguageP2();
	~HPguageP2();
	void initialize();
	void render();
	void update(int hp);
	void changeGage(int hp);
};
