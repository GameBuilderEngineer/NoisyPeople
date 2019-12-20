//===================================================================================================================================
//�yReticle.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/07
// [�X�V��]2019/11/07
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Sprite.h"
#include "InstancingBillboard.h"
#include "Ray.h"
#include "Player.h"
#include "Camera.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace reticleNS
{
	//2D���e�B�N�����̒�`
	namespace Reticle2DNS{
		const int	NUM_U			= 5;
		const int	NUM_V			= 2;
		const float UNIT_U			= 1.0f/NUM_U;//�P��U
		const float UNIT_V			= 1.0f/NUM_V;//�P��V
		const int WIDTH				= 640;//�e�N�X�`���S�̂̕�
		const int HEIGHT			= 256;//�e�N�X�`���S�̂̍���	
		const int UNIT_WIDTH		= (int)(WIDTH/NUM_U);//�P�ʕ�
		const int UNIT_HEIGHT		= (int)(HEIGHT/NUM_V);//�P�ʍ���
		const D3DXVECTOR3 POSITION1 = D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 POSITION2 = D3DXVECTOR3((float)WINDOW_WIDTH / 4 *3, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 196);
	}

	//2D�����[�h�\�����̒�`
	namespace Reload2DNS{
		const int	NUM_U			= 10;
		const int	NUM_V			= 5;
		const float UNIT_U			= 1.0f/NUM_U;//�P��U
		const float UNIT_V			= 1.0f/NUM_V;//�P��V
		const int WIDTH				= 1280;//�e�N�X�`���S�̂̕�
		const int HEIGHT			= 640;//�e�N�X�`���S�̂̍���	
		const int UNIT_WIDTH		= (int)(WIDTH/NUM_U);//�P�ʕ�
		const int UNIT_HEIGHT		= (int)(HEIGHT/NUM_V);//�P�ʍ���
		const D3DXVECTOR3 POSITION1 = D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 POSITION2 = D3DXVECTOR3((float)WINDOW_WIDTH / 4 *3, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 ROTATION	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		const D3DCOLOR COLOR		= D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//2D�d�͕s���\�����̒�`
	namespace EnergyEmpty2DNS{
		const int	NUM_U			= 1;
		const int	NUM_V			= 1;
		const float UNIT_U			= 1.0f/NUM_U;//�P��U
		const float UNIT_V			= 1.0f/NUM_V;//�P��V
		const int WIDTH				= 256;//�e�N�X�`���S�̂̕�
		const int HEIGHT			= 128;//�e�N�X�`���S�̂̍���	
		const int UNIT_WIDTH		= (int)(WIDTH/NUM_U);//�P�ʕ�
		const int UNIT_HEIGHT		= (int)(HEIGHT/NUM_V);//�P�ʍ���
		const D3DXVECTOR3 POSITION1 = D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 POSITION2 = D3DXVECTOR3((float)WINDOW_WIDTH / 4 *3, (float)WINDOW_HEIGHT / 2, 0.0f);
		const D3DXVECTOR3 ROTATION	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		const D3DCOLOR COLOR		= D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//3D���e�B�N�����
	namespace Reticle3DNS {
		const int	NUM_U			= 1;
		const int	NUM_V			= 1;
		const float UNIT_U			= 1.0f / NUM_U;//�P��U
		const float UNIT_V			= 1.0f / NUM_V;//�P��V
		const int WIDTH				= 128;//�e�N�X�`���S�̂̕�
		const int HEIGHT			= 128;//�e�N�X�`���S�̂̍���	
		const int UNIT_WIDTH		= (int)(WIDTH / NUM_U);//�P�ʕ�
		const int UNIT_HEIGHT		= (int)(HEIGHT / NUM_V);//�P�ʍ���
	}



	class Reticle3D :public InstancingBillboardNS::Instance
	{
	public:
		//�R���X�g���N�^
		Reticle3D(D3DXVECTOR3 position,D3DXVECTOR2 uv);
		//�X�V
		virtual void update(float frameTime)override;
	};

}

//===================================================================================================================================
//�y���e�B�N���N���X�z
//===================================================================================================================================
class Reticle :	public Base
{
private:
	//2D
	//���e�B�N��
	D3DXVECTOR2				uv[reticleNS::Reticle2DNS::NUM_U][reticleNS::Reticle2DNS::NUM_V];
	Sprite*					reticle2D;
	int currentU;
	int currentV;
	int type;
	int remainingBullet[gameMasterNS::PLAYER_NUM];//�c�e��

	//�����[�h
	D3DXVECTOR2				uvReload[reticleNS::Reload2DNS::NUM_U][reticleNS::Reload2DNS::NUM_V];
	Sprite*					reload2D;
	int						reloadU;
	int						reloadV;


	//�d�͕s���\��
	Sprite*					energyEmpty2D;



	//3D
	InstancingBillboard*	billboard;
	//���e�B�N�����C
	D3DXVECTOR3*			cameraPosition;
	D3DXVECTOR3*			aimingPosition1;
	D3DXVECTOR3*			aimingPosition2;
	float					collideDistance;


public:
	Reticle();
	~Reticle();
	
	void setCameraPosition(D3DXVECTOR3* position);
	void setAimingPosition1(D3DXVECTOR3* position);
	void setAimingPosition2(D3DXVECTOR3* position);
	void setAnime(int playerState);
	void setRemainingBullet(int num, int playerNo);//�c�e��

	void update(float frameTime);
	void render3D(int playerType,Camera* camera);
	void render2D(Player* player);
};
