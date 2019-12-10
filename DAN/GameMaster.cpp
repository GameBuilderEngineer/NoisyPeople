//===================================================================================================================================
//�yGameMaster.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/25
//===================================================================================================================================
#include "GameMaster.h"
#include "ImguiManager.h"
#include "Input.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace gameMasterNS
{
	Input* input = NULL;
}

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace gameMasterNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
GameMaster::GameMaster()
{
	gameTimer			= 0.0f;								//�Q�[������
	countDownTimer		= 0.0f;								//�J�E���g�_�E������
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		ZeroMemory(&playerInformation[i], sizeof(PlayerTable));
	}
	treeNum				= 0;
	//conversionOrder		= NULL;
	input				= getInput();
	gameTimerStop		= false;
#ifdef _DEBUG
	showGUI = true;
#endif // _DEBUG
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
GameMaster::~GameMaster()
{
	//SAFE_DELETE_ARRAY(conversionOrder);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void GameMaster::initialize()
{

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void GameMaster::update(float frameTime)
{
	//�Q�[�����Ȃ�΁A�|�[�Y�؂�ւ���t
	if (gameTimer > 0)
	{
		//�X�^�[�g�{�^��/�o�L�[����͂��ꂽ�ꍇ�|�[�Y�ɂ���B
		if (input->wasKeyPressed('P') || 
			input->getController()[0]->wasButton(virtualControllerNS::SPECIAL_MAIN)||
			input->getController()[1]->wasButton(virtualControllerNS::SPECIAL_MAIN))
		{
			pause = !pause;
		}
#ifdef _DEBUG
		//�^�b�`�p�b�h(SUB)�{�^��/Q�L�[����͂��ꂽ�ꍇ�Q�[���^�C�}�[���~�B
		if (input->wasKeyPressed('Q') ||
			input->getController()[0]->wasButton(virtualControllerNS::SPECIAL_SUB) ||
			input->getController()[1]->wasButton(virtualControllerNS::SPECIAL_SUB))
		{
			gameTimerStop = !gameTimerStop;
		}

#endif // _DEBUG


	}
}

//===================================================================================================================================
//�y�|�[�Y�����z
//===================================================================================================================================
bool GameMaster::paused()
{
	return pause;
}

//===================================================================================================================================
//�y�Q�[���J�n�������z
//===================================================================================================================================
void GameMaster::startGame()
{
	gameTimer		= GAME_TIME;
	gameTimerStop	= false;
	pause = false;
	progress = 0x00000000;
	discardConversionOrder();
}

//===================================================================================================================================
//�y�Q�[�����Ԃ̍X�V�z
//===================================================================================================================================
void GameMaster::updateGameTime(float frameTime)
{
	if (gameTimerStop)return;

	gameTimer -= frameTime;
}

//===================================================================================================================================
//�y�c�莞��1���̍X�V�z
//===================================================================================================================================
bool GameMaster::playActionRamaining1Min()
{
	if (gameTimer > 60)return false;
	if (whetherAchieved(PASSING_REMAINING_ONE_MINUTE))return false;

	setProgress(PASSING_REMAINING_ONE_MINUTE);

	return true;
}


#pragma region conversionOrderFunction
//===================================================================================================================================
//�y�Ή������؂̖{�����L�^�z
//===================================================================================================================================
void GameMaster::recordTreeTable(TreeTable treeTable)
{
	TreeTable data = treeTable;
	data.eventTime = GAME_TIME - gameTimer;
	treeTableList.insertFront(data);
	treeTableList.listUpdate();
}

//===================================================================================================================================
//�y�ϊ����ԕϐ�����������z
//===================================================================================================================================
void GameMaster::setTreeNum(int num) 
{
	treeNum = num;
}

//===================================================================================================================================
//�y�ϊ����ԕϐ���j������z
//===================================================================================================================================
void GameMaster::discardConversionOrder() {
	this->treeNum = 0;
	treeTableList.allClear();
}
#pragma endregion

//===================================================================================================================================
//�ysetter�z
//===================================================================================================================================
void GameMaster::setConversionOrder(int* newValue) {} // conversionOrder = newValue; };
void GameMaster::setProgress(int achievement) { progress |= achievement; }

//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
PlayerTable* GameMaster::getPlayerInfomation(){	return playerInformation;}				//�v���C���[���̎擾
float GameMaster::getGameTime() {return gameTimer;}										//�Q�[���������Ԃ̎擾
int GameMaster::getProgress() { return progress; }
bool GameMaster::whetherAchieved(int ahievement) { return progress & ahievement; }
//�Ή����̎擾
int	 GameMaster::getGreeningRate() {
	int numGreening = 0;
	for (int i = 0; i < treeTableList.nodeNum; i++)
	{
		TreeTable data = *treeTableList.getValue(i);
		switch (data.eventType)
		{
		case TO_DEAD: numGreening--;				break;
		case TO_GREEN_WITH_ANALOG:numGreening++;	break;
		case TO_GREEN_WITH_DIGITAL:numGreening++;	break;
		}
	}
	float rate = numGreening / treeNum;
	return (int)(rate*100.0f);
}													
//�Ή��{��
int  GameMaster::getGreeningTreeNum(int playerNo)
{
	int numGreening = 0;
	for (int i = 0; i < treeTableList.nodeNum; i++)
	{
		TreeTable data = *treeTableList.getValue(i);
		if (data.player != playerNo)continue;
		switch (data.eventType)
		{
		case TO_GREEN_WITH_ANALOG:numGreening++;	break;
		case TO_GREEN_WITH_DIGITAL:numGreening++;	break;
		}
	}
	return numGreening;
}
//���ރG�l�~�[��
int  GameMaster::getKillEnemyNum(int playerNo)
{
	return killEnemyNum[playerNo];
}
//�C�x���g���X�g�̎擾
int GameMaster::getEventList(TreeTable* out,float time)
{
	int eventNum = 0;
	
	//�擾�������Ԃ���C�x���g�̐����擾����B
	for (int i = 0;i< treeTableList.nodeNum;i++)
	{
		TreeTable table = *treeTableList.getValue(i);	//�C�x���g���擾
		if (table.playBacked)continue;					//���ɍĐ��ς݂Ȃ̂ŁA�X���[
		if (time > table.eventTime)	eventNum++;			//���Đ����ׂ��C�x���g�Ȃ̂ŁA�C�x���g�������Z
	}

	//���X�g�̍쐬
	//�E�C�x���g���X�g�̎擾�v���L(out�Ƀ|�C���^�L)
	//�E�C�x���g����0�łȂ�
	if (out && eventNum)
	{
		//�A�E�g�v�b�g����C�x���g���X�g���쐬����B
		TreeTable* list = NULL;
		list = new TreeTable[eventNum];
		int current = 0;
		for (int i = 0; i < treeTableList.nodeNum; i++)
		{
			//���ݒl���J�E���g�����C�x���g���𒴂�����X���[[�������K�[�h]
			if (current >= eventNum)continue;
			
			//�C�x���g���擾
			TreeTable table = *treeTableList.getValue(i);

			//���ɍĐ��ς݂Ȃ�X���[
			if (table.playBacked)continue;

			//���Đ����ׂ��C�x���g�Ȃ̂ŁA���X�g�֓o�^����
			if (time >= table.eventTime)		
			{
				//���X�g�֓o�^
				list[current] = table;
				current++;
			}
		}

		out = list;
	}

	return eventNum;
}

//===================================================================================================================================
//�yGUI�z
//===================================================================================================================================
#ifdef _DEBUG
void GameMaster::createGUI()
{
	ImGui::Text("gameTime = %f",		gameTimer);
	ImGui::Text("countDownTimer = %f",	countDownTimer);
	ImGui::Text("TreeNum = %d",	treeNum);
	if (ImGui::CollapsingHeader("TreeTableList"))
	{
		ImGui::Text("listNum: %d ", treeTableList.nodeNum);
		
		for (int i = 0; i < treeTableList.nodeNum; i++)
		{
			if (ImGui::CollapsingHeader("TreeTable"))
			{
				TreeTable table = *treeTableList.getValue(i);
				ImGui::Text("No:[%d]",i);				
				ImGui::Text("TreeId = %d",table.id);
				ImGui::Text("PlayerNo:%d",table.player+1);
				ImGui::Text("EventTime = %.02f",table.eventTime);
				ImGui::Text("model = %d",table.modelType);
				ImGui::Text("position(%.02f,%.02f,%.02f)",table.position.x,table.position.y,table.position.z);
				ImGui::Text("rotation(%.02f,%.02f,%.02f,%.02f)",table.rotation.x,table.rotation.y,table.rotation.z,table.rotation.w);
				ImGui::Text("scale(%.02f,%.02f,%.02f)",table.scale.x,table.scale.y,table.scale.z);
				
				if (table.playBacked)	ImGui::Text("playBacked:true");
				else					ImGui::Text("playBacked:false");

				switch (table.eventType)
				{
				case EVENT_TYPE::TO_DEAD:				ImGui::Text("EVENT:TO_DEAD");					break;
				case EVENT_TYPE::TO_GREEN_WITH_ANALOG:	ImGui::Text("EVENT:TO_GREEN_WITH_ANALOG");		break;
				case EVENT_TYPE::TO_GREEN_WITH_DIGITAL:	ImGui::Text("EVENT:TO_GREEN_WITH_DIGITAL");		break;
				}
			}
		}
	}
}
#endif