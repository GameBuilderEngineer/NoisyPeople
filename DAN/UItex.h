//============================
//[UItex.h]
// ����@�W��
//============================
#pragma once
//============================
//�C���N���[�h
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "VirtualController.h"
//#include "UIcharacter.h"

//============================
//���O��� �萔
//============================
namespace uiTexNS
{
	//UI�̎��
	enum UITEX_TYPE
	{
		LINE,
		FLAME01,
		FLAME02,
		CHARA_A,
		CHARA_B,
		NEXT,
		UITEX_MAX,
	};

	//���U���g�̃t�F�C�Y
	enum RESULT_PHASE
	{
		PHASE_01,		//���U���g���S�̃X���C�h�C��
		PHASE_02,		//���\��
		PHASE_03,		//���ʕ\��(�����Ȃ�)
		PHASE_04,		//�^�C�g���ɖ߂�t�F�C�Y
		PHASE_05,
		PHASE_MAX,		//�t�F�C�Y�̑���
	};

	//���C��
	const int WIDTH_LINE = (1920 * WINDOW_WIDTH / 1920);			//��
	const int HEIGHT_LINE = (15 * WINDOW_HEIGHT / 1080);			//����
	const D3DXVECTOR3 POSITION_LINE = D3DXVECTOR3(-1000, 150 * WINDOW_HEIGHT / 1080, 0);//�����ʒu
	const float END_POS_LINE = (960 * WINDOW_WIDTH / 1920);		//�ŏI�ʒu��x���W

	//�t���[��
	const int WIDTH_FLAME = (1024 * WINDOW_WIDTH / 1920);			//��
	const int HEIGHT_FLAME = (1024 * WINDOW_HEIGHT / 1080);			//����
	const D3DXVECTOR3 POSITION_FLAME_01 = D3DXVECTOR3(480 * WINDOW_WIDTH / 1920, 2000, 0);//�ʒu
	const D3DXVECTOR3 POSITION_FLAME_02 = D3DXVECTOR3(1440 * WINDOW_WIDTH / 1920, 2000, 0);//�ʒu
	const float END_POS_FLAME = (578 * WINDOW_HEIGHT / 1080);		//�ŏI�ʒu��y���W

	//�L����A
	const int WIDTH_CHARA_A = (512 * WINDOW_WIDTH / 1920);		//��
	const int HEIGHT_CHARA_A = (1024 * WINDOW_HEIGHT / 1080);		//����
	const D3DXVECTOR3 POSITION_CHARA_A = D3DXVECTOR3(480 * WINDOW_WIDTH / 1920, 2500, 0);//�ʒu
	const float END_POS_CHARA_A = (618 * WINDOW_HEIGHT / 1080);	//�ŏI���W��y���W

	//�L����B
	const int WIDTH_CHARA_B = (512 * WINDOW_WIDTH / 1920);		//��
	const int HEIGHT_CHARA_B = (1024 * WINDOW_HEIGHT / 1080);		//����
	const D3DXVECTOR3 POSITION_CHARA_B = D3DXVECTOR3(1440 * WINDOW_WIDTH / 1920,2500, 0);//�ʒu
	const float END_POS_CHARA_B = (628 * WINDOW_HEIGHT / 1080);	//�ŏI���W��y���W

	//NEXT�e�N�X�`��
	const int WIDTH_NEXT = (512 * WINDOW_WIDTH / 1920);		//��
	const int HEIGHT_NEXT = (64 * WINDOW_HEIGHT / 1080);		//����
	const D3DXVECTOR3 POSITION_NEXT = D3DXVECTOR3(4500, 1017, 0);//�ʒu
	const float END_POS_NEXT = (1730 * WINDOW_WIDTH / 1920);	//�ŏI���W��x���W

	//�^�C�g���֖߂�{�^��
	const int WIDTH_TO_TITLE = (512 * WINDOW_WIDTH / 1920);		//��
	const int HEIGHT_TO_TITLET = (64 * WINDOW_HEIGHT / 1080);		//����

	//����
	const D3DXVECTOR3 ROTATION_TEX = D3DXVECTOR3(0, 0, 0);			//��]
	const D3DCOLOR COLOR_TEX = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F
	const float SLIDE_SPEED_TEX = (20);

}
//============================
//�N���X��`
//============================
class UItexture 
{
public: //�����o�[�ϐ�
	Sprite * uiTexture[uiTexNS::UITEX_MAX];	//�X�v���C�g
	D3DXVECTOR3 pos[uiTexNS::UITEX_MAX];	//�\���ʒu�Ǘ��p

public: //�����o�[�֐�
	void initialize();						//������
	void render(int resultPhase);			//�`��
	void update(int resultPhase);			//�X�V
	void uninitialize();					//�I��
	void slideRight(int uiType);			//�E�փX���C�h�C���֐�
	void slideLeft(int uiType);				//���փX���C�h�C���֐�
	void slideUp(int uiType);				//��փX���C�h�C���֐�
	void assignPos(int uiType);				//�\���ʒu�̑���֐�
};



