//============================
// [ResultUI.h]
// ����@�W��
//============================

//============================
//�C���N���[�h
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"


//============================
//���O��� �萔
//============================
namespace resultUiNS
{
	//UI�̎��
	enum RESULTUI_TYPE
	{
		RESULT,
		LINE,
		FLAME,
		CHARA_A,
		CHARA_B,
		PLAYER,
		GREENIG,
		RYOKUKA,
		PERSENT,
		NUMBER,
		DEFEAT,
		GEKITAI,
		EXCELLENT,
		GREAT,
		CLEAR,
		FAIL,
		RESULTUI_MAX,	//UI�̑���
	};

	//���U���g�̃t�F�C�Y
	enum RESULT_PHASE
	{
		PHASE_01,		//���U���g���S�̃X���C�h�C��
		PHASE_02,		//���\��
		PHASE_03,		//���ʕ\��(�����Ȃ�)
		PHASE_04,		//�^�C�g���ɖ߂�t�F�C�Y
		PHASE_MAX,		//�t�F�C�Y�̑���
	};

	//���U���g���S
	const int WIDTH_RESULT = (330);			//��
	const int HEIGHT_RESULT = (170);		//����
	const D3DXVECTOR3 POSITION_RESULT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�t���[��
	const int WIDTH_FLAME = (860);			//��
	const int HEIGHT_FLAME = (755);			//����
	const D3DXVECTOR3 POSITION_FLAME = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//���C��
	const int WIDTH_LINE = (1920);			//��
	const int HEIGHT_LINE = (15);			//����
	const D3DXVECTOR3 POSITION_LINE = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�L����A
	const int WIDTH_CHARA_A = (478);		//��
	const int HEIGHT_CHARA_A = (624);		//����
	const D3DXVECTOR3 POSITION_CHARA_A = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�L����B
	const int WIDTH_CHARA_B = (478);		//��
	const int HEIGHT_CHARA_B = (624);		//����
	const D3DXVECTOR3 POSITION_CHARA_B = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�v���C���[
	const int WIDTH_PLAYER = (345);			//��
	const int HEIGHT_PLAYER = (180);		//����
	const D3DXVECTOR3 POSITION_PLAYER = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�Ή�(�p��)
	const int WIDTH_GREENIG = (334);		//��
	const int HEIGHT_GREENIG = (53);		//����
	const D3DXVECTOR3 POSITION_GREENIG = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�Ή�
	const int WIDTH_RYOKUKA = (256);		//��
	const int HEIGHT_RYOKUKA = (86);		//����
	const D3DXVECTOR3 POSITION_RYOKUKA = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�p�[�Z���g
	const int WIDTH_PERSENT = (115);		//��
	const int HEIGHT_PERSENT = (124);		//����
	const D3DXVECTOR3 POSITION_PERSENT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//����
	const int WIDTH_NUMBER = (115);			//��
	const int HEIGHT_NUMBER = (124);		//����
	const D3DXVECTOR3 POSITION_NUMBER = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//����(�p��)
	const int WIDTH_DEFEAT = (390);			//��
	const int HEIGHT_DEFEAT = (53);			//����
	const D3DXVECTOR3 POSITION_DEFEAT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//����
	const int WIDTH_GEKITAI = (255);			//��
	const int HEIGHT_GEKITAI = (86);			//����
	const D3DXVECTOR3 POSITION_GEKITAI = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�G�N�Z�����g
	const int WIDTH_EXCELLENT = (740);		//��
	const int HEIGHT_EXCELLENT = (331);		//����
	const D3DXVECTOR3 POSITION_EXCELLENT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�O���C�g
	const int WIDTH_GREAT = (740);			//��
	const int HEIGHT_GREAT = (331);			//����
	const D3DXVECTOR3 POSITION_GREAT = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//�N���A
	const int WIDTH_CLEAR = (740);			//��
	const int HEIGHT_CLEAR = (331);			//����
	const D3DXVECTOR3 POSITION_CLEAR = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//���s
	const int WIDTH_FAIL = (740);			//��
	const int HEIGHT_FAIL = (331);			//����
	const D3DXVECTOR3 POSITION_FAIL = D3DXVECTOR3(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 5, 0);//�ʒu

	//����
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//��]
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F

}
//============================
//�N���X��`
//============================
class ResultUI
{
public: //�����o�[�ϐ�
	Sprite * resultUI[resultUiNS::RESULTUI_MAX];	//�X�v���C�g
	int resultPhase;								//�t�F�C�Y�Ǘ��p

public: //�����o�[�֐�
	void initialize();			//������
	void render();				//�`��
	void update(Input *input);	//�X�V
	void uninitialize();		//�I��
	void renderPhase();

};

