//===================================================================================================================================
//�yDigitalShiftEffect.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/25
// [�X�V��]2019/11/25
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"
#include "EffekseerManager.h"
#include "InstancingBillboard.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace DigitalShiftEffectNS
{
	//�G�t�F�N�g�Đ����X�g
	const int	START_SHIFT			= 0x00000001;
	const int	RUNNING_SHIFT		= 0x00000002;
	const int	END_SHIFT			= 0x00000004;


	//�G�t�F�N�g����
	const float	START_TIME			= 1.0f;
	const float	RUNNING_TIME		= 1.0f;
	const float	END_TIME			= 1.0f;

}

//===================================================================================================================================
//�y�f�W�^���V�t�g�G�t�F�N�g�N���X�z
//===================================================================================================================================
class DigitalShiftEffect :public Base
{
private:
	LinkedList<Object*>*	objectList;		//�I�u�W�F�N�g���X�g
	StaticMeshRenderer*		renderer;
	
	InstancingBillboard* billboard;



	//�����ʒu���X�g
	std::vector<D3DXVECTOR3*> syncPositionList;

public:
	//�R���X�g���N�^/�f�X�g���N�^
	DigitalShiftEffect();
	~DigitalShiftEffect();
	//��{
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//�Đ�/�ꎞ��~
	void play(int effectType,D3DXVECTOR3 position);
	//void syncPlay(int effectType,D3DXVECTOR3* syncPosition);
	void pause();
	
	//�Đ����X�g
	void playStartShift(D3DXVECTOR3 position);
	void playRunningShift(D3DXVECTOR3 position);
	void playEndShift(D3DXVECTOR3 position);
	
	//�I�u�W�F�N�g����
	void createDigitalSphere(D3DXVECTOR3 position, float runTime);

	//�폜
	void autoDestroy();


};

