//====================================
//�^�C�}�[����[Timer.h]
//����@�W��
//====================================
#pragma once

//====================================
//�C���N���[�h
//====================================
#include "Sprite.h"
#include "TextureLoader.h"

//====================================
//���O���
//====================================
namespace timerNS
{
	//�^�C�}�[
	const int WIDTH_TIMER = (100);		//�e�����̉���
	const int HEIGHT_TIMER = (100);		//�e�����̏c��
	const int DIGIT_TIMER = (4);		//�^�C�}�[�̌���
	const D3DXVECTOR3 POSITION_TIMER = D3DXVECTOR3(1000, 100, 0);	//�^�C�}�[�̕\���ʒu(��)
	const D3DXVECTOR3 ROTATION_TIMER = D3DXVECTOR3(0, 0, 0);		//��]
	const D3DCOLOR COLOR_TIMER = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F

}
//====================================
//�N���X��`
//====================================
class Timer
{
private://�����o�[�ϐ�
	Sprite *timer[timerNS::DIGIT_TIMER];
	D3DXVECTOR3 pos[timerNS::DIGIT_TIMER];	//�\���ʒu�Ǘ��p
	float		 keepTime;						//���ԊǗ��p
	//uv���W�p�̕ϐ�
	D3DXVECTOR2 uvCoord01;
	D3DXVECTOR2 uvCoord02;
	D3DXVECTOR2 uvCoord03;
	D3DXVECTOR2 uvCoord04;
	

public://�����o�[�֐�
	Timer();				//�R���X�g���N�^
	~Timer();				//�f�X�g���N�^
	void initialize();		//������
	void render();			//�`�揈��
	void update();			//�X�V����
	void uninitialize();	//�I������
	void setUV(int time);	//uv���W�̐ݒ�
	void assingUV();		//uv���W�̑��

};
