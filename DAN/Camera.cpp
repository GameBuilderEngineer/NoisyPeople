//===================================================================================================================================
//�yCamera.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/12/11
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Camera.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace cameraNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Camera::Camera()
{
	D3DXQuaternionIdentity(&relativeQuaternion);
	posture = D3DXQUATERNION(0, 0, 0, 1);
	D3DXMatrixRotationQuaternion(&world, &posture);
	nearZ = INIT_NEAR_Z;
	farZ = INIT_FAR_Z;
	betweenGaze = relativeDistance = 1.0f;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Camera::~Camera()
{
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
HRESULT Camera::initialize(DWORD _windowWidth, DWORD _windowHeight)
{
	setAspect(_windowWidth,_windowHeight);
	updateOrtho();
	return S_OK;
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Camera::update()
{
	if (target != NULL) {
		gazePosition = *target;
		position = *target;
	}
	else {
		position = (D3DXVECTOR3)relativeQuaternion;
	}
	
	D3DXVECTOR3 axisY(world._21, world._22, world._23);
	if(targetY != NULL)Base::postureControl(&posture,axisY,*targetY ,0.1f);

	D3DXVECTOR3 relativePosition = (D3DXVECTOR3)relativeQuaternion;
	D3DXVec3Normalize(&relativePosition,&relativePosition);
	relativePosition *= relativeDistance;

	//�p���N�H�[�^�j�I������p���s����쐬����
	D3DXMatrixRotationQuaternion(&world, &posture);
	position +=
		relativePosition.x*D3DXVECTOR3(world._11,world._12,world._13)+
		relativePosition.y*D3DXVECTOR3(world._21,world._22,world._23)+
		relativePosition.z*D3DXVECTOR3(world._31,world._32,world._33);
	gazePosition += 
		relativeGaze.x*D3DXVECTOR3(world._11,world._12,world._13)+
		relativeGaze.y*D3DXVECTOR3(world._21,world._22,world._23)+
		relativeGaze.z*D3DXVECTOR3(world._31,world._32,world._33);
	setViewProjection();
}

//===================================================================================================================================
//�y�J�����̕`�揀���z
//===================================================================================================================================
void Camera::renderReady()
{
	LPDIRECT3DDEVICE9 device = getDevice();
	device->SetTransform(D3DTS_VIEW, &view);
	device->SetTransform(D3DTS_PROJECTION, &projection);
}

//===================================================================================================================================
//�y�J�����̔C�ӎ���]�����z
//===================================================================================================================================
void Camera::rotation(D3DXVECTOR3 axis,float degree)
{
	D3DXQUATERNION conjugateQ;
	D3DXQUATERNION rotationQ(0,0,0,1);
	float radian = D3DXToRadian(degree);
	D3DXQuaternionRotationAxis(&rotationQ, &axis, radian);
	D3DXQuaternionConjugate(&conjugateQ, &rotationQ);

	D3DXQUATERNION temporaryQ;
	//����*��]�Ώ�*��]�N�H�[�^�j�I��
	temporaryQ = conjugateQ * relativeQuaternion * rotationQ;

	D3DXVECTOR3 relativePosition = (D3DXVECTOR3)temporaryQ;
	float relativeLength = D3DXVec3Length(&relativePosition);
	if (limit & ROTATION_TOP_LIMIT && 
		relativePosition.y > relativeLength-limitValueRotaionTop)return;
	if (limit & ROTATION_BOTTOM_LIMIT && 
		relativePosition.y < -relativeLength+limitValueRotaionBottom)return;

	//����*��]�Ώ�*��]�N�H�[�^�j�I��
	relativeQuaternion = temporaryQ;
}

//===================================================================================================================================
//�y��]���A���Έʒu�x�N�g���ɍ����i������j�������Z�b�g����z
//===================================================================================================================================
void Camera::setLimitRotationTop(float value)
{
	enableLimit(ROTATION_TOP_LIMIT);
	limitValueRotaionTop = value;
}

//===================================================================================================================================
//�y��]���A���Έʒu�x�N�g���ɍ����i�������j�������Z�b�g����z
//===================================================================================================================================
void Camera::setLimitRotationBottom(float value)
{
	enableLimit(ROTATION_BOTTOM_LIMIT);
	limitValueRotaionBottom = value;
}

//===================================================================================================================================
//�y�����p�����[�^��L���ɂ���z
//===================================================================================================================================
void Camera::enableLimit(int limitParameter)
{
	limit |= limitParameter;
}

//===================================================================================================================================
//�y�����p�����[�^�𖳌��ɂ���z
//===================================================================================================================================
void Camera::disableLimit(int limitParameter)
{
	limit &= ~limitParameter;
}

//===================================================================================================================================
//�y�^�[�Q�b�g�ʒu�����b�N�I������z
//===================================================================================================================================
void Camera::lockOn(D3DXVECTOR3 lockOnTarget,float frameTime)
{
	D3DXVECTOR3 axis(0, 1, 0);
	float radian;

	//�v���C���[�ƃ��b�N�I���Ώۂ̕����x�N�g��
	D3DXVECTOR3 lockOnTargetDirection;
	between2VectorDirection(&lockOnTargetDirection, *target, lockOnTarget);
	lockOnTargetDirection = slip(lockOnTargetDirection, getDirectionY());
	D3DXVec3Normalize(&lockOnTargetDirection, &lockOnTargetDirection);
	D3DXVECTOR3 front = slip(getDirectionZ(), getDirectionY());
	D3DXVec3Normalize(&front, &front);
	if (formedRadianAngle(&radian, front, lockOnTargetDirection))
	{
		rotation(axis, D3DXToDegree(radian));
	}

}

//===================================================================================================================================
//�y�߂荞�ݕ␳�z
//===================================================================================================================================
bool Camera::insetCorrection(LPD3DXMESH mesh, D3DXMATRIX matrix) 
{
	Ray ray;
	if (target)		ray.initialize(*target, -getDirectionZ());
	else			ray.initialize(position, -getDirectionZ());

	if (ray.rayIntersect(mesh, matrix) && ray.distance < betweenGaze)
	{
		relativeDistance = max(0.01f,ray.distance);
	}
	else {
		relativeDistance = betweenGaze;//�Œ�l�̑��
	}

	return false;
}

//===================================================================================================================================
//�y���ɂ��Փ˔���z
//===================================================================================================================================
bool Camera::sphereCollide(D3DXVECTOR3 position, float radius)
{
	return false;
}

//===================================================================================================================================
//�y���C�ɂ��Փ˔���z
//===================================================================================================================================
bool Camera::rayCollide(LPD3DXMESH mesh, D3DXMATRIX matrix)
{
	return false;
}

//===================================================================================================================================
//�y�����_�ԋ����̐ݒ�z
//===================================================================================================================================
void Camera::setGazeDistance(float newValue)
{
	betweenGaze = relativeDistance = newValue;
}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void Camera::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("Camera Property"))
	{
		ImGuiIO& io = ImGui::GetIO();
		float limitTop = 1000;
		float limitBottom = -1000;

		float minFOV = (D3DX_PI / 180) * 1;
		float maxFOV = (D3DX_PI / 180) * 180;

		float minRelative = 0;
		float maxRelative = 5;

		ImGui::SliderFloat("fieldOfView", &fieldOfView, minFOV, maxFOV);						//����p
		ImGui::SliderFloat("nearZ", &nearZ, INIT_NEAR_Z, INIT_FAR_Z);							//���F�ߋ���
		ImGui::SliderFloat("farZ", &farZ, INIT_FAR_Z, INIT_FAR_Z*10);							//���F������

		ImGui::SliderFloat3("position", position, limitBottom, limitTop);						//�ʒu
		ImGui::SliderFloat3("gazePosition", gazePosition, limitBottom, limitTop);				//�����ʒu
		ImGui::SliderFloat4("relativeQuaternion", relativeQuaternion, minRelative, maxRelative);//���Έʒu
		ImGui::SliderFloat("relativeDistance", &relativeDistance, 0.01f, 10.0f);				//�����_�ԋ���
		ImGui::SliderFloat3("upVector", upVector, -1, 1);										//����x�N�g��
		ImGui::SliderFloat4("posture", posture, limitBottom, limitTop);							//�p���N�H�[�^�j�I��

		ImGui::Checkbox("onGaze", &onGaze);														//�����t���O

	}
#endif // _DEBUG
}

//===================================================================================================================================
//�y���ˉe�s����X�V����z
//===================================================================================================================================
void Camera::updateOrtho()
{
	//���ˉe�s������߂�
	float nearH = nearZ * tanf(fieldOfView*0.5f)*2.0f;
	float nearW = nearH * (float)windowWidth / (float)windowHeight;
	D3DXMatrixOrthoLH(&ortho, nearW, nearH, nearZ, farZ);
}

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
//�E�B���h�E�T�C�Y�ɂ��A�X�y�N�g��̐ݒ�
void Camera::setAspect(DWORD _windowWidth, DWORD _windowHeight)
{
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;
	aspect = (FLOAT)windowWidth / (FLOAT)windowHeight;
}
//����p�̐ݒ�
void Camera::setFieldOfView(float value)
{
	fieldOfView = value;
}
//�v���W�F�N�V�����s��ƃr���[�s��̐ݒ�
HRESULT Camera::setViewProjection()
{
	D3DXMatrixLookAtLH(&view, &position, &gazePosition, &upVector);
	// �v���W�F�N�V����
	D3DXMatrixPerspectiveFovLH(&projection, fieldOfView, aspect, nearZ, farZ);
	return S_OK;
}
void Camera::setNearZ(float value) { nearZ = value; }
void Camera::setFarZ(float value) { farZ = value; }
void Camera::GetViewMatrix(D3DXMATRIX* viewOut, D3DXMATRIX* world)
{
	D3DXMATRIX CameraW;
	//�J�����̎p�������[���h�ϊ�
	D3DXMatrixMultiply(&CameraW, &view, world);
	//�r���[�ϊ��s��쐬
	D3DXMatrixLookAtLH(
		viewOut,
		&D3DXVECTOR3(CameraW._11, CameraW._13, CameraW._13),
		&D3DXVECTOR3(CameraW._31, CameraW._32, CameraW._33),
		&D3DXVECTOR3(CameraW._21, CameraW._23, CameraW._23)
	);
}

//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
D3DXVECTOR3 Camera::getAxisZ()	{
	D3DXVECTOR3 direction;
	Base::between2VectorDirection(&direction, position, gazePosition);
	return direction;}
D3DXVECTOR3 Camera::getAxisY()	{return D3DXVECTOR3(world._21, world._22, world._23);}
D3DXVECTOR3 Camera::getHorizontalAxis()
{
	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis,&D3DXVECTOR3(0,1,0),&D3DXVECTOR3(relativeQuaternion.x, relativeQuaternion.y, relativeQuaternion.z));
	if (isnan(axis.x)||isnan(axis.y)||isnan(axis.z))return D3DXVECTOR3(1,0,0);	
	return axis;
}
