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

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace reticleNS
{
	//���e�B�N�����̒�`
	const int	NUM_U			= 5;
	const int	NUM_V			= 3;
	const float UNIT_U			= 1.0f/NUM_U;//�P��U
	const float UNIT_V			= 1.0f/NUM_V;//�P��V
	const int WIDTH				= 1250;//�e�N�X�`���S�̂̕�
	const int HEIGHT			= 750;//�e�N�X�`���S�̂̍���	
	const int UNIT_WIDTH		= (int)(WIDTH/NUM_U);//�P�ʕ�
	const int UNIT_HEIGHT		= (int)(HEIGHT/NUM_V);//�P�ʍ���

	const D3DXVECTOR3 POSITION1 = D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 POSITION2 = D3DXVECTOR3((float)WINDOW_WIDTH / 4 *3, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);

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
	D3DXVECTOR2				uv[reticleNS::NUM_U][reticleNS::NUM_V];
	Sprite*					reticle2D;
	InstancingBillboard*	billboard;

	//���e�B�N�����C
	D3DXVECTOR3*			cameraPosition;
	D3DXVECTOR3*			aimingPosition1;
	D3DXVECTOR3*			aimingPosition2;
	float					collideDistance;

	int currentU;
	int currentV;
	int type;

public:
	Reticle();
	~Reticle();
	
	void setCameraPosition(D3DXVECTOR3* position);
	void setAimingPosition1(D3DXVECTOR3* position);
	void setAimingPosition2(D3DXVECTOR3* position);
	void setAnime();

	void update(float frameTime);
	void render3D(int playerType,D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void render2D();
};

