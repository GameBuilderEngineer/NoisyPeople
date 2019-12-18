//===================================================================================================================================
//�yCountUI.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/12/12
// [�X�V��]2019/12/12
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace CountUINS
{
	//�\������
	const float COUNT_DISPLAY_TIME	= 1.0f;
	const float START_DISPLAY_TIME	= 3.0f;
	const float FINISH_DISPLAY_TIME	= 5.0f;

	//�X�v���b�V���X�v���C�g���̒�`
	//�J�n��
	namespace StartNS {
		const int WIDTH					= 512;
		const int DIVIDE_WIDTH			= 4;//���̕�����
		const int HEIGHT				= 128;
	}

	//�I����
	namespace FinishNS {
		const int WIDTH					= 1024;
		const int DIVIDE_WIDTH			= 11;//���̕�����
		const int HEIGHT				= 128;
	}
	const D3DXVECTOR3 POSITION		= D3DXVECTOR3((float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR			= D3DCOLOR_RGBA(255, 255, 255, 0);


	enum {
		START,
		FINISH,
	};
}

//===================================================================================================================================
//�y�J�E���g�t�h�N���X�z
//===================================================================================================================================
class CountUI :public Sprite
{
private:
	float	displayTimer;
	float	displayTime;
	int		count;
	bool	finished;
public:
	CountUI();
	~CountUI();
	void update(float frameTime);
	void startCount(int count);
	void finishCount(int count);

};

