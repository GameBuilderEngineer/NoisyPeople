//=========================================================
//�t�B�i�[���e�N�X�`���N���X[Finale.h]
//����W��
//=========================================================

//=========================================================
//�C���N���[�h
//=========================================================
#include "BasicUI.h"

//=========================================================
//���O���
//=========================================================
namespace finaleTexNS
{
	const int WIDTH_FIN = (512 * WINDOW_WIDTH / 1920);//����
	const int HEIGHT_FIN = (256 * WINDOW_HEIGHT / 1080);//����
	const D3DXVECTOR3 POSITION_FIN = D3DXVECTOR3(1700 * WINDOW_WIDTH / 1920, 900 * WINDOW_HEIGHT / 1080, 0);//�\���ʒu
	const D3DCOLOR COLOR_FIN = D3DCOLOR_RGBA(255, 255, 255, 0);	//�F
	const float ALPHA_SPEED = 3.0f;
}

//=========================================================
//�N���X��`
//=========================================================
class FinaleTex :public BasicUI
{
private://�����o�[�ϐ�
	Sprite * fin;
	int alpha;
public://�����o�[�֐�
	FinaleTex();
	~FinaleTex();
	void initialize();
	void render();
	void update();
};