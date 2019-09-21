//===================================================================================================================================
//�ySplash.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
//#include "Splash2D.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace splashNS
{
	static const float EFFECT_MAX = 1500;	// �G�t�F�N�g���

	enum PLAYER_TYPE
	{
		PLAYER_1,
		PLAYER_2,
		PLAYER_TYPE_MAX
	};
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION[splashNS::PLAYER_TYPE::PLAYER_TYPE_MAX] =
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
//�y�X�v���b�V���V�[���N���X�z
//===================================================================================================================================
class Splash :public AbstractScene
{
private:
	// �X�v���b�V��2D
	//Splash2D splash2D;

public:
	Splash();
	~Splash();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;
	//void render3D(Camera _currentCamera);
	void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};

