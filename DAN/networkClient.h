//===================================================================================================================================
//【networkClient.h】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/12/05
// [更新日]2019/12/05
//===================================================================================================================================
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//===================================================================================================================================
//【ライブラリのロード】
//===================================================================================================================================
#include "Base.h"
#include "networkServer.h"

#include <WinSock.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

//===================================================================================================================================
//【クライアント】
//===================================================================================================================================
class NETWORK_CLIENT
{
//Method
public:
	NETWORK_CLIENT();
	~NETWORK_CLIENT();

	void send(float time);
	void outputGUI();
	static void recordTreeTable(const TreeTable inTreeTable, int tableNo);
	static void setSendTreeTable(const TreeTable inTreeTable);
	static void recordPlayerPosition(const D3DXVECTOR3 position, int playerNo);
	void resetDisplay();

//Data
public:
	static bool requestConnection;//接続要求
	static bool initialConnection;//初期接続
	static int connectionTarget;//接続先
	static bool success;
	bool onResetDisplay;
private:
	int packageID;
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN addrin;
	HOSTENT *lpHostEnt;
	unsigned int addr;
	int nRtn, nNo = 5;

	//ツリー情報
	static TreeTable treeTable[TREE_TABLE_SIZE];
	static int treeNum;

	//プレイヤー位置情報
	static D3DXVECTOR3 pos1P;
	static bool record1P;
	static D3DXVECTOR3 pos2P;
	static bool record2P;

};
