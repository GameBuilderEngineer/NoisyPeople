//===================================================================================================================================
//�yOperation.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
//#include "KeyOpe.h"
//#include "PadOpe.h"
//#include "SceneEffect.h"

//===================================================================================================================================
//�y�}�N����`�z
//===================================================================================================================================
#define OPE_MAX (1)		// �I�y���[�V�����ړ����E

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace operationNS
{
	static const float EFFECT_MAX = 1500;	// �G�t�F�N�g���

	enum PLAYER_TYPE
	{
		PLAYER_1,
		PLAYER_2,
		PLAYER_TYPE_MAX
	};
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION[operationNS::PLAYER_TYPE::PLAYER_TYPE_MAX] =
	{
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f),
#ifdef _DEBUG
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f)
#else
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f)
#endif // _DEBUG
	};
}

//===================================================================================================================================
//�y������@�����V�[���N���X�z
//===================================================================================================================================
class Operation : public AbstractScene
{
private:
	// �L�[�{�[�h����
	//KeyOpe keyOpe;
	// �p�b�h����
	//PadOpe padOpe;
	//SceneEffect sceneEffect;//	�V�[���G�t�F�N�g

public:
	Operation();
	~Operation();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	//void render3D(Camera _currentCamera);
	void renderUI(LPDIRECT3DDEVICE9 device);
	int opeTransition;	// �I�y���[�V�����摜����ւ�

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};