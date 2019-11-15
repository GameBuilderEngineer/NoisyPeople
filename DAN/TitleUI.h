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
	const int WIDTH_LOGO = (908);						//��
	const int HEIGHT_LOGO = (376);						//����
	const D3DXVECTOR3 POSITION_LOGO = D3DXVECTOR3(960, 255, 0);//�ʒu
	
	//�I����
	const int WIDTH_MENU = (536);						//��
	const int HEIGHT_MENU = (218);						//����
	const D3DXVECTOR3 POSITION_MENU = D3DXVECTOR3(1600, 895, 0);//�ʒu

	//�I�����̐���
	const int WIDTH_MENU_INFO = (692);					//��
	const int HEIGHT_MENU_INFO = (37);					//����
	const D3DXVECTOR3 POSITION_MENU_INFO = D3DXVECTOR3(340, 960, 0);//�ʒu

	//����ҏ��
	const int WIDTH_COPYRIGHT = (587);					//��
	const int HEIGHT_COPYRIGHT = (34);					//����
	const D3DXVECTOR3 POSITION_COPYRIGHT = D3DXVECTOR3(340, 1005, 0);//�ʒu

	//�I���o�[
	const int WIDTH_BAR = (737);						//��
	const int HEIGHT_BAR = (43);						//����
	const D3DXVECTOR3 POSITION_BAR_TUTO = D3DXVECTOR3(1600, 810, 0);//�ʒu
	const D3DXVECTOR3 POSITION_BAR_GAME = D3DXVECTOR3(1600, 865, 0);//�ʒu
	const D3DXVECTOR3 POSITION_BAR_CREDIT = D3DXVECTOR3(1600, 925, 0);//�ʒu
	const D3DXVECTOR3 POSITION_BAR_EXIT = D3DXVECTOR3(1600, 990, 0);//�ʒu
	
	//����
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//��]
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F
	
	//UI�̎��
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
		UI_MAX,				//UI�̑���
	};

	//���j���[�I����
	enum MENU_TYPE
	{
		TUTORIAL,
		GAME,
		CREDIT,
		EXIT,
		MENU_MAX,
		CREATE
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

public: //�����o�[�֐�
	void initialize();		//����
	void uninitialize();	//�I��
	void render();			//�`��
	void update(Input *input);			//�X�V
	int getSelectState();	//�I������Ԃ̎擾
	void moveBar();			//�o�[�̈ړ�

};
