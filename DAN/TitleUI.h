//============================
// [TitleUI.h]
// ����@�W��
//============================

//============================
//�C���N���[�h
//============================
#include "Sprite.h"
#include "TextureLoader.h"
#include "Input.h"
#include "VirtualController.h"

//============================
//���O��� �萔
//============================
namespace titleUiNS
{
	
	//���S
	const int WIDTH_LOGO = (908 * WINDOW_WIDTH / 1920);						//��
	const int HEIGHT_LOGO = (376 * WINDOW_HEIGHT / 1080);						//����
	const D3DXVECTOR3 POSITION_LOGO = D3DXVECTOR3(960 * WINDOW_WIDTH / 1920, 255 * WINDOW_HEIGHT / 1080, 0);//�ʒu
	
	//�I����
	const int WIDTH_MENU = (536 * WINDOW_WIDTH / 1920);						//��
	const int HEIGHT_MENU = (218 * WINDOW_HEIGHT / 1080);						//����
	const D3DXVECTOR3 POSITION_MENU = D3DXVECTOR3(1600 * WINDOW_WIDTH / 1920, 895 * WINDOW_HEIGHT / 1080, 0);//�ʒu

	//�I�����̐���
	const int WIDTH_MENU_INFO = (587 * WINDOW_WIDTH / 1920);					//��
	const int HEIGHT_MENU_INFO = (37 * WINDOW_HEIGHT / 1080);					//����
	const D3DXVECTOR3 POSITION_MENU_INFO = D3DXVECTOR3(340 * WINDOW_WIDTH / 1920, 960 * WINDOW_HEIGHT / 1080, 0);//�ʒu

	//����ҏ��
	const int WIDTH_COPYRIGHT = (587 * WINDOW_WIDTH / 1920);					//��
	const int HEIGHT_COPYRIGHT = (34 * WINDOW_HEIGHT / 1080);					//����
	const D3DXVECTOR3 POSITION_COPYRIGHT = D3DXVECTOR3(340 * WINDOW_WIDTH / 1920, 1005 * WINDOW_HEIGHT / 1080, 0);//�ʒu

	//�I���o�[
	const int WIDTH_BAR = (737 * WINDOW_WIDTH / 1920);						//��
	const int HEIGHT_BAR = (43 * WINDOW_HEIGHT / 1080);						//����
	const D3DXVECTOR3 POSITION_BAR_TUTO = D3DXVECTOR3(1600 * WINDOW_WIDTH / 1920, 810 * WINDOW_HEIGHT / 1080, 0);//�ʒu
	const D3DXVECTOR3 POSITION_BAR_GAME = D3DXVECTOR3(1600 * WINDOW_WIDTH / 1920, 865 * WINDOW_HEIGHT / 1080, 0);//�ʒu
	const D3DXVECTOR3 POSITION_BAR_CREDIT = D3DXVECTOR3(1600 * WINDOW_WIDTH / 1920, 925 * WINDOW_HEIGHT / 1080, 0);//�ʒu
	const D3DXVECTOR3 POSITION_BAR_EXIT = D3DXVECTOR3(1600 * WINDOW_WIDTH / 1920, 990 * WINDOW_HEIGHT / 1080, 0);//�ʒu
	
	//PressAnyButton
	const int WIDTH_PRESS = (1024 * WINDOW_WIDTH / 1920);					//��
	const int HEIGHT_PRESS = (64 * WINDOW_HEIGHT / 1080);					//����
	const D3DXVECTOR3 POSITION_PRESS = D3DXVECTOR3(925 * WINDOW_WIDTH / 1920, 800 * WINDOW_HEIGHT / 1080, 0);//�ʒu

	//����
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//��]
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F

	//���S�̕����オ���Ă��鑬�x
	const float ALPHA_SPEED = 1.2f;

	//���S�������オ���Ă��鎞��
	const float FLOAT_LOGO_TIME = 4.0f;

	enum UI_TYPE
	{
		UI_LOGO,			//���S
		UI_MENU,			//�I����
		UI_INFO_TUTO,		//�`���[�g���A���I�����̐���
		UI_INFO_GAME,		//�Q�[���I�����̐���
		UI_INFO_CREDIT,		//�N���W�b�g�I�����̐���
		UI_INFO_EXIT,		//�I���I�����̐���
		UI_COPYRIGHT,		//����ҏ��
		UI_BAR,				//�I�����̃o�[
		UI_PRESS,			//PressAnyButton
		UI_MAX				//UI�̑���
	};

	//���j���[�I����
	enum MENU_TYPE
	{
		TUTORIAL,
		GAME,
		CREDIT,
		EXIT,
		MENU_MAX,
		CREATE,
		PHOTOGRAPH
	};
}
//============================
//�N���X��`
//============================
class TitleUI
{
public: //�����o�[�ϐ�
	Sprite * UI[titleUiNS::UI_MAX];
	int selectState;
	float alpha;//���l
	int titleState;//�^�C�g���̏��
	float titleTime;//�^�C�g������
public: //�����o�[�֐�
	void initialize();		//����
	void uninitialize();	//�I��
	void render();			//�`��
	void update(Input *input, float flametime);//�X�V
	int getSelectState();	//�I������Ԃ̎擾
	void moveBar();			//�o�[�̈ړ�
	void ringSE(Input *input);//SE��炷����
	void flash(float flashtime);
private:
};
