//============================
//[UIrank.h]
// ����@�W��
//============================

//============================
//�C���N���[�h
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "VirtualController.h"
#include "BasicUI.h"

//============================
//���O��� �萔
//============================
namespace uiRankNS
{
	//�����N�̎��
	enum UIRANK_TYPE
	{
		EXCELLENT,
		GREAT,
		CLEARE,
		FAILED,
		RANK_MAX,
	};

	//�����N�̕\���ʒu
	const int WIDTH_RANK = (2960 * WINDOW_WIDTH / 1920);	//�����̕�
	const int HEIGHT_RANK = (2551 * WINDOW_HEIGHT / 1080);	//�����̍���
	const int END_WIDTH_RANK = (740 * WINDOW_WIDTH / 1920);		//�ŏI�̕�
	const int END_HEIGHT_RANK = (331 * WINDOW_HEIGHT / 1080);		//�ŏI�̍���
	const D3DXVECTOR3 POSITION_RANK_P1 = D3DXVECTOR3(480 * WINDOW_WIDTH / 1920, 825 * WINDOW_HEIGHT / 1080, 0);//�v���C���[�P�p�̕\���ʒu
	const D3DXVECTOR3 POSITION_RANK_P2 = D3DXVECTOR3(1440 * WINDOW_WIDTH / 1920, 825 * WINDOW_HEIGHT / 1080, 0);//�v���C���[2�p�̕\���ʒu
	
	//����
	const D3DXVECTOR3 ROTATION_TEX = D3DXVECTOR3(0, 0, 0);			//��]
	const D3DCOLOR COLOR_TEX = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F
	const int SHRINK_SPEED = 20;									//�k���X�s�[�h
}
//============================
//�N���X��`
//============================
class UIrank:public BasicUI
{
public: //�����o�[�ϐ�
	Sprite * rank[basicUiNS::PLAYER_TYPE_MAX][uiRankNS::RANK_MAX];//�X�v���C�g�̍쐬
	int widthSize;								//���̃T�C�Y�Ǘ��p
	int heightSize;								//�����T�C�Y�Ǘ��p
	bool playedSE;								//SE�Đ��t���O
public: //�����o�[�֐�
	UIrank();//�R���X�g���N�^
	~UIrank();//�f�X�g���N�^
	void initialize();//������
	void render(int rankType,int rankType2);//�`��
	void update(int rankType1, int rankType2);//�X�V
};
