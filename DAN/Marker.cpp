//===================================================================================================================================
//�yMarker.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/12/17
// [�X�V��]2019/12/17
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Marker.h"
#include "TextureLoader.h"
#include "UtilityFunction.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace MarkerNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Marker::Marker(int type)
{
	sprite = new Sprite;
		
	switch (type)
	{
	case PLAYER1:
		{
			using namespace PlayerMarkerNS;
			sprite->initialize(
				*textureNS::reference(textureNS::MARK_2D_MALE),		//�e�N�X�`���|�C���^
				SpriteNS::CENTER,									//���_
				WIDTH,												//��
				HEIGHT,												//����
				POSITION,											//�ʒu
				ROTATION,											//��]
				COLOR												//�F
			);
			break;
		}

	case PLAYER2:
		{
			using namespace PlayerMarkerNS;
			sprite->initialize(
				*textureNS::reference(textureNS::MARK_2D_FEMALE),	//�e�N�X�`���|�C���^
				SpriteNS::CENTER,									//���_
				WIDTH,												//��
				HEIGHT,												//����
				POSITION,											//�ʒu
				ROTATION,											//��]
				COLOR												//�F
			);
			break;
		}

	case BOSS_ENEMY:
		{
			using namespace BossEnemyMarkerNS;
			sprite->initialize(
				*textureNS::reference(textureNS::MARK_BOSS),		//�e�N�X�`���|�C���^
				SpriteNS::CENTER,									//���_
				WIDTH,												//��
				HEIGHT,												//����
				POSITION,											//�ʒu
				ROTATION,											//��]
				COLOR												//�F
			);
			break;
		}

	case ATTACKED_TREE:	
		{
			using namespace AttackedTreeNS;
			sprite->initialize(
				*textureNS::reference(textureNS::LIGHT_001),		//�e�N�X�`���|�C���^
				SpriteNS::CENTER,									//���_
				WIDTH,												//��
				HEIGHT,												//����
				POSITION,											//�ʒu
				ROTATION,											//��]
				COLOR												//�F
			);
			break;
		}
	}

	scale = D3DXVECTOR2(0.05f, 0.05f);


}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Marker::~Marker()
{
	SAFE_DELETE(sprite);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Marker::update(float frameTime)
{

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Marker::render(D3DXVECTOR3 position)
{
	sprite->setPosition(position);
	sprite->setVertex();
	sprite->render();
}



//�����_���[
//===================================================================================================================================
//�y�}�[�J�[�����_���[�F�R���X�g���N�^�z
//===================================================================================================================================
MarkerRenderer::MarkerRenderer()
{
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		playerPosition[i] = NULL;
	}
	bossEnemyPosition		= NULL;
	attackedTree			= NULL;


	//�C���X�^���V���O�r���{�[�h�̏�����
	billboard = new InstancingBillboard();
	billboard->initialize(*textureNS::reference(textureNS::COLOR_SCALE),7,7);
	billboard->setRenderType(InstancingBillboardNS::FOREGROUND_PASS| InstancingBillboardNS::FIXED_SIZE_PASS);

	//�X�v���C�g�̏�����
	for (int i = 0; i < MARKER_NUM; i++)
	{
		marker[i] = new Marker(i);
		billboard->generateInstance(marker[i]);//�C���X�^���g�Ƃ��ēo�^
	}
}

//===================================================================================================================================
//�y�}�[�J�[�����_���[�F�f�X�g���N�^�z
//===================================================================================================================================
MarkerRenderer::~MarkerRenderer()
{
	for (int i = 0; i < MARKER_NUM; i++)
	{
		SAFE_DELETE(marker[i]);
	}
	SAFE_DELETE(billboard);
}

//===================================================================================================================================
//�y�}�[�J�[�����_���[�F�X�V�z
//===================================================================================================================================
void MarkerRenderer::update(float frameTime)
{
	//�}�[�J�[�X�v���C�g�̍X�V
	for (int i = 0; i < MARKER_NUM; i++)
	{
		marker[i]->update(frameTime);
	}

	//�C���X�^���V���O�r���{�[�h�̍X�V
	billboard->update(frameTime);
}

//===================================================================================================================================
//�y�}�[�J�[�����_���[�F�`��z
//===================================================================================================================================
void MarkerRenderer::render(int playerNo, Camera* camera)
{

	D3DXVECTOR3 center;
	float		radius;
	float length = sqrtf((float)(WINDOW_WIDTH*WINDOW_WIDTH + WINDOW_HEIGHT * WINDOW_HEIGHT));
	float xLimit = (float)(WINDOW_WIDTH / 4 - PlayerMarkerNS::WIDTH / 2);
	float yLimit = (float)(WINDOW_HEIGHT / 2 - PlayerMarkerNS::HEIGHT / 2);

	//�v���C���[�}�[�J�[�i�ǂ��炩����݂̂�`��j
	switch(playerNo)
	{
	case gameMasterNS::PLAYER_1P:
		//���g�̃r���{�[�h�}�[�J�[�͕`�悵�Ȃ�
		marker[PLAYER1]->color = D3DCOLOR_RGBA(255, 255, 255, 0);
		//�^�[�Q�b�g�����݂��邩�ǂ���
		if (playerPosition[gameMasterNS::PLAYER_2P]) {
			center = *playerPosition[gameMasterNS::PLAYER_2P];
			radius = 1.0f;
		}
		else
		{
			marker[PLAYER2]->color = D3DCOLOR_RGBA(255, 255, 255, 0);	//�r���{�[�h�`�悵�Ȃ�
			break;
		}

		//�J�����O����
		if (UtilityFunction::culling(
			center, radius, camera->view, camera->fieldOfView,
			camera->nearZ, camera->farZ, camera->aspect))
		{
			//������F�r���{�[�h�ɂ��`��
			marker[PLAYER2]->position = center+D3DXVECTOR3(0.0f,1.0f,0.0f);
			marker[PLAYER2]->color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else {
			//����O�F�X�v���C�g�ɂ��`��

			//�r���{�[�h�Ƃ��Ă͕`�悵�Ȃ�
			marker[PLAYER2]->color = D3DCOLOR_RGBA(255, 255, 255, 0);

			//�`��ʒu�����߂�
			D3DXVECTOR3 renderPosition;
			renderPosition = CENTER1P;

			//3D��ԏ�̕������A2D���ʍ��W�֕ϊ�����
			D3DXVECTOR3 addVector = conversion2D(camera, center);
			addVector *= length;
			addVector.x = UtilityFunction::clamp(addVector.x, -xLimit, xLimit);
			addVector.y = UtilityFunction::clamp(addVector.y, -yLimit, yLimit);

			//�`��ʒu�։��Z
			renderPosition += addVector;

			//�`��
			marker[PLAYER2]->render(renderPosition);
		}
		break;
	case gameMasterNS::PLAYER_2P:
		//���g�̃r���{�[�h�}�[�J�[�͕`�悵�Ȃ�
		marker[PLAYER2]->color = D3DCOLOR_RGBA(255, 255, 255, 0);
		//�^�[�Q�b�g�����݂��邩�ǂ���
		if (playerPosition[gameMasterNS::PLAYER_1P]) {
			center = *playerPosition[gameMasterNS::PLAYER_1P];
		}
		else
		{
			marker[PLAYER1]->color = D3DCOLOR_RGBA(255, 255, 255, 0);	//�r���{�[�h�`�悵�Ȃ�
			break;
		}

		radius = 1.0f;
		//�J�����O����
		if (UtilityFunction::culling(
			center, radius, camera->view, camera->fieldOfView,
			camera->nearZ, camera->farZ, camera->aspect))
		{
			//������F�r���{�[�h�ɂ��`��
			marker[PLAYER1]->position = center;
			marker[PLAYER1]->color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else {
			//����O�F�X�v���C�g�ɂ��`��

			//�r���{�[�h�Ƃ��Ă͕`�悵�Ȃ�
			marker[PLAYER1]->color = D3DCOLOR_RGBA(255, 255, 255, 0);

			//�J�����ɑ΂���Ώۂ̕��������߂�
			D3DXVECTOR3 direction;
			Base::between2VectorDirection(&direction, camera->position, center);
			direction.y = 0;//Y�����폜
			D3DXVec3Normalize(&direction, &direction);//���K��

			//�`��ʒu�����߂�
			D3DXVECTOR3 renderPosition;
			renderPosition = CENTER2P;

			//3D��ԏ�̕������A2D���ʍ��W�֕ϊ�����
			D3DXVECTOR3 addVector = conversion2D(camera, center);
			addVector *= length;
			addVector.x = UtilityFunction::clamp(addVector.x, -xLimit,xLimit);
			addVector.y = UtilityFunction::clamp(addVector.y, -yLimit,yLimit);

			//�`��ʒu�։��Z
			renderPosition += addVector;
			
			//�`��
			marker[PLAYER1]->render(renderPosition);
		}
		break;
	}

	//���̃}�[�J�[�i�ǂ���̉�ʂɂ��`��j
	for (int i = BOSS_ENEMY; i < MARKER_NUM; i++)
	{
		//�Ώۂ����݂��邩�ǂ�������
		bool skip = false;
		switch (i)
		{
		case BOSS_ENEMY:
			if(bossEnemyPosition)	center = *bossEnemyPosition;	//�ʒu���Z�b�g
			else skip = true;
			radius = 1.0f;
			break;
		case ATTACKED_TREE:
			if (attackedTree)		center = *attackedTree;			//�ʒu���Z�b�g
			else skip = true;
			radius = 1.0f;
			break;
		}

		//�}�[�N�Ώۂ����݂��Ȃ��̂ŃX�L�b�v
		if (skip)
		{
			marker[i]->color = D3DCOLOR_RGBA(255, 255, 255, 0);
			continue;
		}

		//�J�����O����
		if (UtilityFunction::culling(
			center, radius, camera->view, camera->fieldOfView,
			camera->nearZ, camera->farZ, camera->aspect))
		{
			//������F�r���{�[�h�ɂ��`��
			marker[i]->position = center;
			marker[i]->color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else {
			//����O�F�X�v���C�g�ɂ��`��

			marker[i]->color = D3DCOLOR_RGBA(255, 255, 255, 0);
			//�J�����ɑ΂���Ώۂ̕��������߂�
			D3DXVECTOR3 direction;
			Base::between2VectorDirection(&direction, camera->position, center);
			direction.y = 0;//Y�����폜
			D3DXVec3Normalize(&direction, &direction);//���K��

			//�`��ʒu�����߂�
			D3DXVECTOR3 renderPosition = D3DXVECTOR3(0,0,0);
			//
			switch (playerNo){
			case gameMasterNS::PLAYER_1P:	renderPosition = CENTER1P;	break;
			case gameMasterNS::PLAYER_2P:	renderPosition = CENTER2P;	break;
			}

			//3D��ԏ�̕������A2D���ʍ��W�֕ϊ�����
			D3DXVECTOR3 addVector = conversion2D(camera, center);
			addVector *= length;
			addVector.x = UtilityFunction::clamp(addVector.x, -xLimit, xLimit);
			addVector.y = UtilityFunction::clamp(addVector.y, -yLimit, yLimit);
			renderPosition.x += direction.x * (WINDOW_WIDTH / 4 - PlayerMarkerNS::WIDTH / 2);
			renderPosition.y += -direction.z * (WINDOW_HEIGHT / 2 - PlayerMarkerNS::HEIGHT / 2);

			marker[i]->render(renderPosition);
		}
	}

	//�C���X�^���V���O�r���{�[�h�̍X�V
	billboard->update(0.0f);
	//�C���X�^���V���O�r���{�[�h
	billboard->render(camera->view, camera->projection, camera->position);
}

D3DXVECTOR3  MarkerRenderer::conversion2D(Camera* camera,D3DXVECTOR3 target)
{

	//�J�����ɑ΂���Ώۂ̕��������߂�
	D3DXVECTOR3 direction;
	Base::between2VectorDirection(&direction, camera->position, target);
	direction.y = 0;//Y�����폜
	D3DXVec3Normalize(&direction, &direction);//���K��

	//�J�����O����
	D3DXVECTOR3 front = camera->getDirectionZ();
	front.y = 0.0f;
	D3DXVec3Normalize(&front, &front);
	//�E����
	D3DXVECTOR3 right = front;
	Base::anyAxisRotation(&right, D3DXVECTOR3(0, 1, 0), 90);
	//������
	D3DXVECTOR3 left = front;
	Base::anyAxisRotation(&left, D3DXVECTOR3(0, 1, 0), -90);

	//�����E��
	float rightDot = D3DXVec3Dot(&right, &direction);
	bool isRight = (rightDot > 0);
	float leftDot = D3DXVec3Dot(&left, &direction);
	bool isLeft = (leftDot > 0);

	//��]�p�����߂�
	float angle;
	if (Base::formedRadianAngle(&angle, front, direction))
	{
		angle = D3DXToDegree(angle);
		if (isRight)	angle *= 1.0f;
		else if (isLeft)angle *= -1.0f;
	}
	else {
		angle = 180.0f;
	}

	//�`��ʒu�����߂�
	D3DXVECTOR3 renderPosition;
	renderPosition = CENTER1P;

	D3DXVECTOR3 addVector = D3DXVECTOR3(0, 0, 1);
	Base::anyAxisRotation(&addVector, D3DXVECTOR3(0, 1, 0), angle);
	addVector.y = -addVector.z;
	addVector.z = 0.0f;

	return addVector;
}