//============================
// [CreditTex.h]
// ����@�W��
//============================

//============================
//�C���N���[�h
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "VirtualController.h"

//============================
//���O��� �萔
//============================
namespace creditTex
{
	const int WIDTH = 1920;								//����
	const int HEIGHT = 3240;							//�c��
	const D3DXVECTOR3 POSITION = D3DXVECTOR3(WINDOW_WIDTH/2, WINDOW_HEIGHT+540,0);//�ʒu
	const  D3DXVECTOR3 POSITION_2 = D3DXVECTOR3( WINDOW_WIDTH / 2, WINDOW_HEIGHT+3780, 0);//�ʒu
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0,0,0);			//��]
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F
	const float MOVE_MAX = -540;			//������ւ̈ړ��̌��E�l
	const float MOVE_SPEED = 2.75f;								//��ɓ����X�s�[�h
	const int CREDIT_MAX = 2;
}
//============================
//�N���X��`
//============================
class CreditTex
{
public: //�����o�[�ϐ�
	Sprite *credit[creditTex::CREDIT_MAX];
	D3DXVECTOR3 pos[creditTex::CREDIT_MAX];
	float       alpha;

public: //�����o�[�֐�
	void initialize();		//����
	void uninitialize();	//�I��
	void render();			//�`��
	void update();			//�X�V
	void moveCredit();		//��ɓ���������
	void fadeOut();			//�t�F�[�h�A�E�g


};