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
	const int	SELECT_LIGHT		= 0x00000008;


	//�G�t�F�N�g����
	const float	START_TIME			= 1.0f;
	const float	RUNNING_TIME		= 1.0f;
	const float	END_TIME			= 1.0f;

	//�f�W�^���c���[�I���G�t�F�N�g
	//�I��p���C�g�N���X
	class SelectLight:public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		SelectLight(int no,D3DXVECTOR3* sync) {
			syncPosition = sync;
			scale = D3DXVECTOR3(1.0f, 30.0f, 1.0f);
			managerNo = no;
			effectNo = effekseerNS::DIGIT_TREE_SELECT;
		}
		virtual void update() {
			position = *syncPosition;
			Instance::update();
		};
	};

}

//===================================================================================================================================
//�y�f�W�^���V�t�g�G�t�F�N�g�N���X�z
//===================================================================================================================================
class DigitalShiftEffect :public Base
{
private:
	//�f�W�^���X�t�B�A
	LinkedList<Object*>*	sphereList;		
	StaticMeshRenderer*		sphereRenderer;

	
	InstancingBillboard*	billboard;


	//�I��\�����C�g���X�g
	bool onPlayedSelectLight;
	DigitalShiftEffectNS::SelectLight* selectLight;

	//�����ʒu���X�g
	std::vector<D3DXVECTOR3*> syncPositionList;

public:
	//�R���X�g���N�^/�f�X�g���N�^
	DigitalShiftEffect();
	~DigitalShiftEffect();
	//��{
	void update(float frameTime);
	void updateSelectLight(D3DXVECTOR3 position);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//�Đ�/�ꎞ��~
	void play(int effectType,D3DXVECTOR3 position);
	//void syncPlay(int effectType,D3DXVECTOR3* syncPosition);
	void pause();
	
	//�Đ����X�g
	void playStartShift(D3DXVECTOR3 position);
	void playRunningShift(D3DXVECTOR3 position);
	void playEndShift(D3DXVECTOR3 position);
	void playSelectLight(int no,D3DXVECTOR3* position);
	void stopSelectLight(int no);
	void shownSelectLigth(bool shown);

	//�I�u�W�F�N�g����
	void createDigitalSphere(D3DXVECTOR3 position, float runTime);

	//�G�t�F�N�g�C���X�^���X����/�폜
	void createSelectLight(D3DXVECTOR3* position);
	void deleteSelectLight();


	//�폜
	void autoDestroy();



};

