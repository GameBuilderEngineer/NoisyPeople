//===================================================================================================================================
//�yPhotograph.h�z
// [�쐬��]HAL����GP12A332 21 �V�� ���m
// [�쐬��]2019/12/26
// [�X�V��]2019/12/26
//===================================================================================================================================
#pragma once

#ifdef _DEBUG
//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include <vector>
#include "AbstractScene.h"
#include "TmpObject.h"
#include "StaticMeshRenderer.h"
#include "Stone.h"
#include "DeadTree.h"
#include "TreeTypeA.h"
#include "TreeTypeB.h"
#include "TestEffect.h"
#include "EnemyManager.h"
#include "AIDirector.h"
#include "NavigationMesh.h"
#include "TreeManager.h"
#include "ItemManager.h"
#include "Telop.h"
#include "Sound.h"

#include "EnemyTools.h"
#include "ItemTools.h"
#include "TreeTools.h"
#include "MapObjectTool.h"

#include "EffekseerManager.h"

//===================================================================================================================================
//�y�}�N���z
//===================================================================================================================================
#define START_GUI_ID	(1)
#define END_GUI_ID		(2)
#define CURVE_GUI_ID	(3)
#define MAX_CURVE_POINT (1)

//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct
{
	short pointMax;
}CURVE_POINT;


//===================================================================================================================================
//�y�񋓌^�萔�z
//===================================================================================================================================
enum POINT_TYPE
{
	POINT_NONE,
	POINT_START,
	POINT_END,
	POINT_CURVE,
	POINT_MAX
};

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace photographNS
{
	//=============================================================================
	// �X�^�[�g�|�C���g�G�t�F�N�g
	//=============================================================================
	class StartEffect :public effekseerNS::Instance
	{
	public:
		const D3DXVECTOR3* syncPosition;
		StartEffect(int no, const D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::DROP_ITEM;
			position = *syncPosition;
		}
		virtual void update()
		{
			Instance::update();
		};
	};

	//=============================================================================
	// �G���h�|�C���g�G�t�F�N�g
	//=============================================================================
	class EndEffect :public effekseerNS::Instance
	{
	public:
		const D3DXVECTOR3* syncPosition;
		EndEffect(int no, const D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::DROP_ITEM;
			position = *syncPosition;
		}
		virtual void update()
		{
			Instance::update();
		}
	};

	//=============================================================================
	// �J�[�u�|�C���g�G�t�F�N�g
	//=============================================================================
	class CurveEffect :public effekseerNS::Instance
	{
	public:
		const D3DXVECTOR3* syncPosition;
		CurveEffect(int no, const D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::WIND;
			position = *syncPosition;
		}
		virtual void update()
		{
			Instance::update();
		}

	};

}


//===================================================================================================================================
//�y�N���G�C�g���[�r�[�N���X�z
//===================================================================================================================================
class Photograph : public AbstractScene
{
private:

	photographNS::StartEffect* startEffect;
	photographNS::EndEffect* endEffect;
	photographNS::CurveEffect* curveEffect;


	//���ՃJ����
	Camera* topView;
	bool onTopView;			//�J�����X�C�b�`
	//���I�u�W�F�N�g
	TmpObject* tmpObject;
	StaticMeshRenderer* tmpObjRenderer;
	//�m�F�p�I�u�W�F�N�g
	Object* cameraObject;
	StaticMeshRenderer* cameraObjectRenderer;
	//�t�B�[���h
	Object* testField;
	StaticMeshRenderer* testFieldRenderer;
	//�^�[�Q�b�g
	Object* target;
	//�B�e�J�����m�F�p
	//Object* cameraObject;
	//StaticMeshRenderer* cameraObjectRenderer;

	//�G�l�~�[�c�[��
	ENEMY_TOOLS *enemyTools;




	int stateCamera;						//�J�����X�e�[�^�X
	float frameDegree = 1.0f / 10.0f;		//�������x
	float inputDegree = 2.0f;				//���͑��x

	float moveTime;				//�I�_�܂ł̎���
	float moveTimer;			//�ړ��^�C�}�[
	D3DXVECTOR3 startPos;		//���[�v�n�_


	float rate;
	float rateY;
	float rateX;
	float degreeY;
	float degreeX;
	float degreeTimer;
	float degreeTime;
	float distance;
	float moveDistance;

	//�J�����̎��擾�ϐ�
	D3DXVECTOR3 cameraAxisX;			//�J������]��X
	D3DXVECTOR3 cameraAxisY;			//�J������]��Y
	D3DXVECTOR3 cameraAxisZ;			//�J������]��Z
	D3DXVECTOR3 fixedAxisX;				//�Œ�X��
	D3DXVECTOR3 fixedAxisZ;				//�Œ�Z��
	D3DXQUATERNION tmpCameraQ;			//�J�����̑��Έʒu�x�N�g���̈ꎞ�ۑ�
	D3DXVECTOR3 BezierCurveS1;			//S���x�W�F�Ȑ��_�P
	D3DXVECTOR3 BezierCurveS2;			//S���x�W�F�Ȑ��_�Q
	D3DXVECTOR3 targetDistance;
	D3DXVECTOR3 newRelative;
	bool measurement = true;

	D3DXVECTOR3 startPoint;
	D3DXVECTOR3 endPoint;
	D3DXVECTOR3 curvePoint;

	float photoTime = 5.0f;
	float photoTimer = 5.0f;
	//float rate;
	bool bezier = false;

public:
	Photograph();
	~Photograph();
	virtual void initialize() override;
	virtual void update(float frameTime) override;
	virtual void render() override;
	virtual void collisions() override;
	virtual void AI() override;
	virtual void uninitialize() override;

	void render3D(Camera currentCamera);
	void renderUI();

	int PointToolsListboxType;
	int meshId;

	virtual void createGUI() override;
	void toolsGUI();
	
	
	void outputStartPointGUI(int GUIid,  D3DXVECTOR3* pos);
	void outputEndPointGUI(int GUIid, D3DXVECTOR3* pos);
	void outputCurvePointGUI(int GUIid, D3DXVECTOR3* pos);
	
	void addCurvePoint(D3DXVECTOR3* pos);

	static D3DXVECTOR3 setBezier(D3DXVECTOR3 startpoint, D3DXVECTOR3 curvepoint, D3DXVECTOR3 endpoint, float rate);

};

#endif
