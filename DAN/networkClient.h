//===================================================================================================================================
//�ynetwork.h�z
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
//�y�萔��`�z
//===================================================================================================================================
//const char szServer[64] = "ths80619";				//����PC
//const char szServer[64] = "DESKTOP-2C7Q2ME";		//����PC
//const char szServer[64] = "ths80214";				//�ɒBPC
//const char szServer[64] = "INDES";					//�T�C

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
	static void setSendTreeTable(const TreeTable inTreeTable);
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

	static TreeTable treeTable[20];
	static int treeNum;
};
