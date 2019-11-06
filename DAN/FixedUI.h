//=================================
// �Œ肳��Ă���UI[FixedUI]
// ����W��
//=================================

//=================================
// �C���N���[�h
//=================================
#include "Sprite.h"
#include "TextureLoader.h"

//=================================
// ���O���
//=================================
namespace fixedUiNS
{
	const int WIDTH_TIMERFLAME = (1920);	//�^�C�}�[�t���[���̉���
	const int HEIGHT_TIMERFLAME = (169);	//�^�C�}�[�t���[���̏c��
	const D3DXVECTOR3 POSITION_TIMERFLAME = D3DXVECTOR3(960, 84, 0);	//�^�C�}�[�t���[���̕\���ʒu(��)
	const D3DXVECTOR3 ROTATION_TIMERFLAME = D3DXVECTOR3(0, 0, 0);		//��]
	const D3DCOLOR COLOR_TIMERFLAME = D3DCOLOR_RGBA(255, 255, 255, 255);//�F

	const int WIDTH_BUTTON = (1920);	//�^�C�}�[�t���[���̉���
}

//=================================
//�N���X��`
//=================================
class FixedUI
{
public://�����o�[�ϐ�
private:
	Sprite * fixedUI;

public://�����o�[�֐�
	FixedUI();
	~FixedUI();
	void initialize();
	void render();
	void update();
	void uninitialize();
private:
};