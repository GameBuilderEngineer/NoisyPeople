//===================================================================================================================================
//�yDisplay .h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/12/10
// [�X�V��]2019/12/10
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
#include "StaticMeshRenderer.h"
#include "TreeManager.h"
#include "networkServer.h"
#include "Sky.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace DisplayNS
{
	const D3DXQUATERNION	CAMERA_RELATIVE_QUATERNION	= D3DXQUATERNION(0.0f,300.0f, 50.0f, 0.0f);
	const float				CAMERA_SPEED				= 8.0f;


	const int				SERVER_MODE					= 0x00000001;
	const int				CLIENT_MODE					= 0x00000002;
}

//===================================================================================================================================
//�y�f�B�X�v���C�V�[���N���X�z
//===================================================================================================================================
class Display : public AbstractScene
{
private:
	//�t�B�[���h
	Object*							testField;			//�t�B�[���h
	StaticMeshRenderer*				testFieldRenderer;	//�t�B�[���h�����_���[
	NETWORK_INTERFACE*				networkServer;
	TreeManager*					treeManager;		//�c���[
	Sky*							sky;				//�X�J�C�h�[��


	float							syncTimer;			//�����^�C�}�[

	int								testMode;			//�e�X�g�p���[�h�ؑ�

public:
	Display();
	~Display();

	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	virtual void render() override;
	virtual void collisions(void) override;
	virtual void AI(void) override;

	void render3D(Camera* currentCamera);
	//void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};

