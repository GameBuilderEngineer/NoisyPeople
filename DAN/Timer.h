//====================================
//�^�C�}�[����[Timer.h]
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
namespace timerNS
{
	//�^�C�}�[
	const int WIDTH_TIMER = (60 * WINDOW_WIDTH / 1920);		//�e�����̉���
	const int HEIGHT_TIMER = (60 * WINDOW_HEIGHT / 1080);		//�e�����̏c��
	const int DIGIT_TIMER = (3);		//�^�C�}�[�̌���
	const D3DXVECTOR3 POSITION_TIMER = D3DXVECTOR3(1015 * WINDOW_WIDTH / 1920, 100 * WINDOW_HEIGHT / 1080, 0);	//���^�C�}�[�̕\���ʒu(��)
	const D3DXVECTOR3 ROTATION_TIMER = D3DXVECTOR3(0, 0, 0);		//��]
	const D3DCOLOR COLOR_TIMER = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F
	const int      GAME_TIME = 600;									//�Q�[������
}

//====================================
//�N���X��`
//====================================
class Timer:public BasicUI,public GameMaster
{
private://�����o�[�ϐ�
	Sprite *timer[timerNS::DIGIT_TIMER];
	float		 keepTime;							//���ԊǗ��p

public://�����o�[�֐�
	Timer();				//�R���X�g���N�^
	~Timer();				//�f�X�g���N�^
	void initialize();		//������
	void render();			//�`�揈��
	void update();			//�X�V����
	void uninitialize();	//�I������
	void setUV(int time);	//uv���W�̐ݒ�
};
