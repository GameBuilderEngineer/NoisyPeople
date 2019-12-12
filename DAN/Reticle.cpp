//===================================================================================================================================
//�yReticle.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/07
// [�X�V��]2019/11/07
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Reticle.h"
#include "TextureLoader.h"
#include "GameMaster.h"
#include "Player.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace reticleNS;

//===================================================================================================================================
//�y�C���X�^���X�N���X�F�R���X�g���N�^�z
//===================================================================================================================================
Reticle3D::Reticle3D(D3DXVECTOR3 position,D3DXVECTOR2 uv):InstancingBillboardNS::Instance()
{
	//�����l�̐ݒ�
	this->position	= position;
	this->rotation	= D3DXVECTOR3(0, 0, 0);
	this->scale		= D3DXVECTOR2(1.0f,1.0f);
	this->speed		= D3DXVECTOR3(0, 0, 0);
	this->lifeTimer	= 0.0f;
	this->limitTime = 1.0f;
	this->uv		= uv;
}

//===================================================================================================================================
//�y�C���X�^���X�N���X�F�X�V�z
//===================================================================================================================================
void Reticle3D::update(float frameTime)
{

}

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Reticle::Reticle()
{
	//�e�N�X�`���ɑ΂���UV�l�̐ݒ�
	//UV�l�̐ݒ�
	for (int u = 0; u < NUM_U; u++)
	{
		for (int v = 0; v < NUM_V; v++)
		{
			uv[u][v] = D3DXVECTOR2(u*UNIT_U, v*UNIT_V);
		}
	}

	//���e�B�N��2D
	reticle2D = new Sprite();
	//UI���̐ݒ�
	reticle2D->initialize(
		*textureNS::reference(textureNS::UI_SUB_RETICLE),	//�e�N�X�`���|�C���^
		SpriteNS::CENTER,								//���_
		(int)((float)UNIT_WIDTH),						//��
		(int)((float)UNIT_HEIGHT),					//����
		POSITION1,										//�ʒu
		ROTATION,										//��]
		COLOR											//�F
	);
	//�`��UV�l��ݒ�
	reticle2D->setUVCoord(
		uv[0][0] + D3DXVECTOR2(0.0, 0.0),
		uv[0][0] + D3DXVECTOR2(UNIT_U, 0.0),
		uv[0][0] + D3DXVECTOR2(0.0, UNIT_V),
		uv[0][0] + D3DXVECTOR2(UNIT_U, UNIT_V));

	//���e�B�N��3D
	currentU = 0;
	currentV = 0;
	billboard = new InstancingBillboard();
	billboard->initialize(*textureNS::reference(textureNS::UI_MAIN_RETICLE),NUM_U-1,NUM_V-1);
	billboard->generateInstance(new Reticle3D(D3DXVECTOR3(0,0,0),D3DXVECTOR2(UNIT_U*0,UNIT_V*0)) );
	billboard->setRenderType(InstancingBillboardNS::FOREGROUND_PASS);

	//�c�e��
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		remainingBullet[i] = 0;
	}
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Reticle::~Reticle()
{
	SAFE_DELETE(reticle2D);
	SAFE_DELETE(billboard);
}

//===================================================================================================================================
//�y�e�N�X�`���ʒu�̃Z�b�g�z
//===================================================================================================================================
void Reticle::setAnime()
{
	reticle2D->setUVCoord(
		uv[currentU][currentV] + D3DXVECTOR2(0.0, 0.0),
		uv[currentU][currentV] + D3DXVECTOR2(UNIT_U, 0.0),
		uv[currentU][currentV] + D3DXVECTOR2(0.0, UNIT_V),
		uv[currentU][currentV] + D3DXVECTOR2(UNIT_U, UNIT_V));
}

//===================================================================================================================================
//�y�c�e�����Z�b�g�z
//===================================================================================================================================
void Reticle::setRemainingBullet(int num,int playerNo)
{
	remainingBullet[playerNo] = num;
}

//===================================================================================================================================
//�y�J�����|�W�V�����̃|�C���^���Z�b�g�z
//===================================================================================================================================
void Reticle::setCameraPosition(D3DXVECTOR3* position)
{
	cameraPosition = position;
}

//===================================================================================================================================
//�y1P�Ə��ʒu�̃|�C���^���Z�b�g�z
//===================================================================================================================================
void Reticle::setAimingPosition1(D3DXVECTOR3* position)
{
	aimingPosition1 = position;
}

//===================================================================================================================================
//�y2P�Ə��ʒu�̃|�C���^���Z�b�g�z
//===================================================================================================================================
void Reticle::setAimingPosition2(D3DXVECTOR3* position)
{
	aimingPosition2 = position;
}

//===================================================================================================================================
//�y�X�V����z
//===================================================================================================================================
void Reticle::update(float frameTime)
{
	billboard->update(frameTime);
}

//===================================================================================================================================
//�y�R�c���̕`��z
//===================================================================================================================================
void Reticle::render3D(int playerType,D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	
	//�`��ʒu�̐ݒ�
	switch (playerType)
	{
	case gameMasterNS::PLAYER_1P:(*billboard->getList().getFrontValue())->position = *aimingPosition1; break;
	case gameMasterNS::PLAYER_2P:(*billboard->getList().getFrontValue())->position = *aimingPosition2; break;
	}
	billboard->update(0);
	billboard->render(view, projection, cameraPositon);				//�`��


}

//===================================================================================================================================
//�y�Q�c���̕`��z
//===================================================================================================================================
void Reticle::render2D(Player* player)
{
	//�v���C���[�ԍ�
	int playerNo = player->getInfomation()->playerType;

	//�`�悷��e�N�X�`���̐؂�ւ�
	switch (player->getState())
	{
	case playerNS::STATE::VISION: 
	case playerNS::STATE::SKY_VISION:
	case playerNS::STATE::DIGITAL_SHIFT:
		//�V�t�g�ł��邩�ǂ����̃��e�B�N���ւ̔��f
		if (player->whetherValidOperation(playerNS::ENABLE_SHIFT))
		{
			reticle2D->setTexturePointer(*textureNS::reference(textureNS::UI_MAIN_DIGITAL_RETICLE));
		}
		else {
			reticle2D->setTexturePointer(*textureNS::reference(textureNS::UI_SUB_DIGITAL_RETICLE));
		}
		break;

	case playerNS::STATE::NORMAL:
	case playerNS::STATE::DEATH:
		int bulletNum = player->getBulletManager()->getRemaining();
		setRemainingBullet(bulletNum, playerNo);
		currentU = remainingBullet[playerNo] % NUM_U;
		currentV = remainingBullet[playerNo] / NUM_U;
		setAnime();
		reticle2D->setTexturePointer(*textureNS::reference(textureNS::UI_SUB_RETICLE));
		break;
	}

	//�`��ʒu
	switch (player->getInfomation()->playerType)
	{
	//1P
	case gameMasterNS::PLAYER_1P:
		reticle2D->setPosition(POSITION1);
		break;
	//2P
	case gameMasterNS::PLAYER_2P:
		reticle2D->setPosition(POSITION2);
		break;
	} 
	reticle2D->setVertex();

	//�`��
	reticle2D->render();
}

