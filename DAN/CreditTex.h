//============================
// [CreditTex.h]
// ����@�W��
//============================

//============================
//�C���N���[�h
//============================
#include "Sprite.h"
#include "TextureLoader.h"

//============================
//���O��� �萔
//============================
namespace creditTex
{
	const int WIDTH = WINDOW_WIDTH;								//����
	const int HEIGHT = WINDOW_HEIGHT*2;							//�c��
	const D3DXVECTOR3 POSITION = D3DXVECTOR3(WINDOW_WIDTH/2, WINDOW_HEIGHT/2,0);//�ʒu
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0,0,0);			//��]
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F
	const float MOVE_MAX = WINDOW_HEIGHT/15;					//������ւ̈ړ��̌��E�l
	const float MOVE_SPEED = 1.0f;								//��ɓ����X�s�[�h
}
//============================
//�N���X��`
//============================
class CreditTex
{
public: //�����o�[�ϐ�
	Sprite *credit;
	D3DXVECTOR3 pos;
	SpriteNS::Vertex2D	*vertexWk[SpriteNS::NUM_VERTEX];// ���_���i�[���[�N

public: //�����o�[�֐�
	void initialize();		//����
	void uninitialize();	//�I��
	void render();			//�`��
	void update();			//�X�V
	void moveCredit();		//��ɓ���������


};