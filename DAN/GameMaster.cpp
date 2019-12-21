//===================================================================================================================================
//�yGameMaster.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/12/21
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
	openingTimer		= 0.0f;								//�I�[�v�j���O����
	endingTimer			= 0.0f;								//�G���f�B���O����
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
	openingTimer	= OPENING_TIME;
	endingTimer		= ENDING_TIME;
	countDownTimer	= COUNT_DOWN_TIME;

#ifdef _DEBUG
#if 1
	gameTimer		= DEBUG_GAME_TIME;
#else
	gameTimer		= GAME_TIME;
#endif
#else
	//Release
	gameTimer		= GAME_TIME;
#endif
	gameTimerStop	= false;
	pause = false;
	progress = 0x00000000;
	discardConversionOrder();
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		killEnemyNum[i] = 0;
	}

	ZeroMemory(wasStartVoicePlayed, 2 * sizeof(bool));
	ZeroMemory(wasFinishVoicePlayed, 2 * sizeof(bool));
}

//===================================================================================================================================
//�y�Q�[���I�[�v�j���O���Ԃ̍X�V�z
//===================================================================================================================================
void GameMaster::updateOpeningTime(float frameTime)
{
	if (gameTimerStop)return;

	openingTimer -= frameTime;
	if (openingTimer <= 0.0f)
	{
		setProgress(PASSING_GAME_OPENING);
	}
}

//===================================================================================================================================
//�y�Q�[���J�n�J�E���g�_�E���̍X�V�z
//===================================================================================================================================
void GameMaster::updateStartCountDown(float frameTime)
{
	if (gameTimerStop)return;

	//�Q�[�����J�n���ꂽ��X�V���Ȃ�
	if (whetherAchieved(PASSING_GAME_START))return;
	
	//�Q�[���I�[�v�j���O���I�����Ă����ꍇ
	if(whetherAchieved(PASSING_GAME_OPENING))
		countDownTimer -= frameTime;
}

//===================================================================================================================================
//�y�Q�[���I���J�E���g�_�E���̍X�V�z
//===================================================================================================================================
void GameMaster::updateFinishCountDown(float frameTime)
{
	if (gameTimerStop)return;

	//�c��10�b�̏ꍇ
	if(gameTimer <= 11)
		countDownTimer -= frameTime;
}

//===================================================================================================================================
//�y�Q�[�����Ԃ̍X�V�z
//===================================================================================================================================
void GameMaster::updateGameTime(float frameTime)
{
	if (gameTimerStop)return;

	//�Q�[�����I�����Ă���ꍇ
	if (whetherAchieved(PASSING_GAME_FINISH))
	{
		gameTimer = 0.0f;
		return;
	}

	//�Q�[�����J�n����Ă���ꍇ
	if (whetherAchieved(PASSING_GAME_START))
	{
		gameTimer -= frameTime;
	}
}

//===================================================================================================================================
//�y�Q�[���G���f�B���O���Ԃ̍X�V�z
//===================================================================================================================================
void GameMaster::updateEndingTime(float frameTime)
{
	if (gameTimerStop)return;

	//�Q�[�����I�����Ă���ꍇ
	if(whetherAchieved(PASSING_GAME_FINISH))
		endingTimer -= frameTime;
	
	if (endingTimer <= 0.0f)
	{
		setProgress(PASSING_GAME_ENDING);
	}
}

//===================================================================================================================================
//�y�J�n�J�E���g�_�E�����̃A�N�V�����z
//===================================================================================================================================
bool GameMaster::playActionStartCount(int countNum)
{
	//�Q�[���I�[�v�j���O���I�������ꍇ�̂�
	if (!whetherAchieved(PASSING_GAME_OPENING))return false;

	if (countDownTimer > (float)countNum)return false;
	switch (countNum)
	{
	case 3:
		if (whetherAchieved(PASSING_COUNT_DOWN_THREE))return false;
		setProgress(PASSING_COUNT_DOWN_THREE);
		return true;break;
	case 2:
		if (whetherAchieved(PASSING_COUNT_DOWN_TWO))return false;
		setProgress(PASSING_COUNT_DOWN_TWO);
		return true;break;
	case 1:
		if (whetherAchieved(PASSING_COUNT_DOWN_ONE))return false;
		setProgress(PASSING_COUNT_DOWN_ONE);
		return true;break;
	case 0:
		if (whetherAchieved(PASSING_GAME_START))return false;
		setProgress(PASSING_GAME_START);
		countDownTimer = 10.0f;
		return true; break;
	}
	return false;
}

//===================================================================================================================================
//�y�c�莞��1���̃A�N�V�����z
//===================================================================================================================================
bool GameMaster::playActionRamaining1Min()
{
	if (gameTimer > 60)return false;
	if (whetherAchieved(PASSING_REMAINING_ONE_MINUTE))return false;

	setProgress(PASSING_REMAINING_ONE_MINUTE);

	return true;
}

//===================================================================================================================================
//�y�I���J�E���g�_�E�����̃A�N�V�����z
//===================================================================================================================================
bool GameMaster::playActionFinishCount(int countNum)
{
	//�Q�[���^�C�}�[��10�b�ȏ゠��
	if (gameTimer > 10)return false;

	if (countDownTimer > (float)countNum)return false;
	switch (countNum)
	{
	case 10:
		if (whetherAchieved(PASSING_REMAINING_10))return false;
		setProgress(PASSING_REMAINING_10);
		return true; break;
	case 9:
		if (whetherAchieved(PASSING_REMAINING_9))return false;
		setProgress(PASSING_REMAINING_9);
		return true; break;
	case 8:
		if (whetherAchieved(PASSING_REMAINING_8))return false;
		setProgress(PASSING_REMAINING_8);
		return true; break;
	case 7:
		if (whetherAchieved(PASSING_REMAINING_7))return false;
		setProgress(PASSING_REMAINING_7);
		return true; break;
	case 6:
		if (whetherAchieved(PASSING_REMAINING_6))return false;
		setProgress(PASSING_REMAINING_6);
		return true; break;
	case 5:
		if (whetherAchieved(PASSING_REMAINING_5))return false;
		setProgress(PASSING_REMAINING_5);
		return true; break;
	case 4:
		if (whetherAchieved(PASSING_REMAINING_4))return false;
		setProgress(PASSING_REMAINING_4);
		return true; break;
	case 3:
		if (whetherAchieved(PASSING_REMAINING_3))return false;
		setProgress(PASSING_REMAINING_3);
		return true; break;
	case 2:
		if (whetherAchieved(PASSING_REMAINING_2))return false;
		setProgress(PASSING_REMAINING_2);
		return true; break;
	case 1:
		if (whetherAchieved(PASSING_REMAINING_1))return false;
		setProgress(PASSING_REMAINING_1);
		return true; break;
	case 0:
		if (whetherAchieved(PASSING_GAME_FINISH))return false;
		setProgress(PASSING_GAME_FINISH);
		return true; break;
	}
	return false;
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
void GameMaster::addKillEnemyNum(int playerNo) { killEnemyNum[playerNo]++; }
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
	float rate = 1.0f;
	if(treeNum)
		rate = (float)numGreening / (float)treeNum;
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
int GameMaster::getEventList(TreeTable** out,float time)
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
	if (out != NULL && eventNum)
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
			TreeTable* table = treeTableList.getValue(i);

			//���ɍĐ��ς݂Ȃ�X���[
			if (table->playBacked)continue;

			//���Đ����ׂ��C�x���g�Ȃ̂ŁA���X�g�֓o�^����
			if (time >= table->eventTime)		
			{
				//���X�g�֓o�^
				list[current] = *table;
				//���X�g�ԍ���i�߂�
				current++;
				//�C�x���g�e�[�u�����Đ�������Ԃɂ���
				table->playBacked = true;
			}
		}

		*out = list;
	}

	return eventNum;
}

//===================================================================================================================================
//�yGUI�z
//===================================================================================================================================
#ifdef _DEBUG
void GameMaster::createGUI()
{
	ImGui::Text("openingTimer = %f",	openingTimer);
	ImGui::Text("countDownTimer = %f",	countDownTimer);
	ImGui::Text("gameTime = %f",		gameTimer);
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