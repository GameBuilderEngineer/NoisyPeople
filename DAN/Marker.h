//===================================================================================================================================
//�yMarker.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/12/17
// [�X�V��]2019/12/17
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"
#include "InstancingBillboard.h"
#include "Camera.h"
#include "GameMaster.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace MarkerNS
{
	enum RenderMode
	{
		MODE_2D,
		MODE_3D,
	};

	enum MarkerType
	{
		PLAYER1,
		PLAYER2,
		BOSS_ENEMY,
		ATTACKED_TREE,
		MARKER_NUM,
	};

	namespace PlayerMarkerNS{
		const int WIDTH			= 128;
		const int HEIGHT		= 128;
		const int DIVIDE_WIDTH	= 1;//���̕�����
	}
	namespace BossEnemyMarkerNS{
		const int WIDTH			= 128;
		const int HEIGHT		= 128;
		const int DIVIDE_WIDTH	= 1;//���̕�����
	}
	namespace AttackedTreeNS{
		const int WIDTH			= 128;
		const int HEIGHT		= 128;
		const int DIVIDE_WIDTH	= 1;//���̕�����
	}
	const D3DXVECTOR3 POSITION	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 CENTER1P	= D3DXVECTOR3((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT / 2, 0.0f);;
	const D3DXVECTOR3 CENTER2P	= D3DXVECTOR3((float)WINDOW_WIDTH / 4 * 3, (float)WINDOW_HEIGHT / 2, 0.0f);
	const D3DXVECTOR3 ROTATION	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DCOLOR COLOR		= D3DCOLOR_RGBA(255, 255, 255, 255);

}

//===================================================================================================================================
//�y�}�[�J�[�N���X�z
//===================================================================================================================================
class Marker :	public InstancingBillboardNS::Instance
{
private:
	MarkerNS::RenderMode				mode;
	Sprite*								sprite;
	
public:
	Marker(int type);
	~Marker();
	virtual void update(float frameTime);
	void render(D3DXVECTOR3 position);
};

//===================================================================================================================================
//�y�}�[�J�[�����_���[�N���X�z
//===================================================================================================================================
class MarkerRenderer :	public Base
{
//Data
private:
	Marker* marker[MarkerNS::MARKER_NUM];
	InstancingBillboard* billboard;
public:
	//�e�ʒu�|�C���^
	D3DXVECTOR3* playerPosition[gameMasterNS::PLAYER_NUM];
	D3DXVECTOR3* bossEnemyPosition;
	D3DXVECTOR3* attackedTree;

//Method
public:
	MarkerRenderer();
	~MarkerRenderer();
	void update(float frameTime);
	void render(int playerNo,Camera* camera);
	D3DXVECTOR3 conversion2D(Camera* camera, D3DXVECTOR3 target);//3D���W������2D�ϊ�

};

