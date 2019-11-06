//-----------------------------------------------------------------------------
// ���^AI�N���X [AIDirector.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EnemyManager.h"
#include "ImguiManager.h"
using namespace aiNS;


//=============================================================================
// ������
//=============================================================================
void AIDirector::initialize(GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
	TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager)
{
	gameMaster = _gameMaster;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;

	sensor.initialize(&playerAD, &enemyAD, &treeAD, &itemAD, gameMaster, player, enemyManager, treeManager, itemManager);
	eventMaker.initialize(&playerAD, &enemyAD, &treeAD, &itemAD);
	opeGenerator.initialize(&playerAD, &enemyAD, &treeAD, &itemAD, gameMaster, player, enemyManager, treeManager, itemManager, _telopManager);
	frameCnt = 0;
}


//=============================================================================
// ���s
//=============================================================================
void AIDirector::run()
{
	switch (frameCnt %= 2)
	{
	case 0:	sensor.update();		break;
	case 1:	eventMaker.update();	break;
	}
	frameCnt++;
}


//=============================================================================
// ImGui�ɏo��
//=============================================================================
void AIDirector::outputGUI()
{
#ifdef _DEBUG

	//if (ImGui::CollapsingHeader("EnemyInformation"))
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	//float limitTop = 1000;
	//	//float limitBottom = -1000;

	//	ImGui::Text("numOfEnemy:%d", Enemy::getNumOfEnemy());

	//	//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//�ʒu
	//	//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
	//	//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//�X�P�[��
	//	//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//���a
	//	//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//���ߒl
	//	//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//���x
	//	//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//�����x
	//	//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//�d��

	//	//ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
	//	//ImGui::Checkbox("onActive", &onActive);										//�A�N�e�B�u���t���O
	//	//ImGui::Checkbox("onRender", &onRender);										//�`��L�����t���O
	//	//ImGui::Checkbox("onLighting", &onLighting);									//���������t���O
	//	//ImGui::Checkbox("onTransparent", &onTransparent);								//���߃t���O
	//	//ImGui::Checkbox("operationAlpha", &operationAlpha);							//���ߒl�̑���L���t���O

	//	//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//���ߒl�̑���L���t���O
	//}

#endif
}