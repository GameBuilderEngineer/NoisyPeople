//====================================
//�Ή�������[GreenRate.h]
//����@�W��
//====================================
#pragma once
//====================================
//�C���N���[�h
//====================================
#include "BasicUI.h"
#include "GameMaster.h"
#include "VirtualController.h"

//====================================
//���O���
//====================================
namespace greenRateNS
{
	//�^�C�}�[
	const int WIDTH_GREEN_RATE = (30 * WINDOW_WIDTH / 1920);		//�e�����̉���
	const int HEIGHT_GREEN_RATE = (30 * WINDOW_HEIGHT / 1080);		//�e�����̏c��
	const int DIGIT_GREEN_RATE = (3);		//�^�C�}�[�̌���
	const D3DXVECTOR3 POSITION_GREEN_RATE = D3DXVECTOR3(340 * WINDOW_WIDTH / 1920, 200 * WINDOW_HEIGHT / 1080, 0);	//���^�C�}�[�̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_GREEN_RATE_P2 = D3DXVECTOR3(1820 * WINDOW_WIDTH / 1920, 200 * WINDOW_HEIGHT / 1080, 0);	//���^�C�}�[�̕\���ʒu(��)
	const D3DXVECTOR3 ROTATION_GREEN_RATE = D3DXVECTOR3(0, 0, 0);		//��]
	const D3DCOLOR COLOR_GREEN_RATE = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F
}

//====================================
//�N���X��`
//====================================
class GreenRate :public BasicUI, public GameMaster
{
private://�����o�[�ϐ�
	Sprite * timer[greenRateNS::DIGIT_GREEN_RATE];
	float		 keepRate;//�Ǘ��p

public://�����o�[�֐�
	GreenRate();//�R���X�g���N�^
	~GreenRate();//�f�X�g���N�^
	void initialize(int playerType);//������
	void render(int playerType);//�`�揈��
	void update(float gametime);//�X�V����
	void uninitialize();//�I������
	void setUV(int time);//uv���W�̐ݒ�
};

