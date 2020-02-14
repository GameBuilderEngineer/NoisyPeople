//===================================================================================================================================
//�ynetworkClient.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/12/05
// [�X�V��]2019/12/05
//===================================================================================================================================
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//===================================================================================================================================
//�y���C�u�����̃��[�h�z
//===================================================================================================================================
#include "Base.h"
#include "networkServer.h"

#include <WinSock.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

//===================================================================================================================================
//�y�N���C�A���g�z
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
	static bool requestConnection;//�ڑ��v��
	static bool initialConnection;//�����ڑ�
	static int connectionTarget;//�ڑ���
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

	//�c���[���
	static TreeTable treeTable[TREE_TABLE_SIZE];
	static int treeNum;

	//�v���C���[�ʒu���
	static D3DXVECTOR3 pos1P;
	static bool record1P;
	static D3DXVECTOR3 pos2P;
	static bool record2P;

};
