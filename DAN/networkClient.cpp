//===================================================================================================================================
//�ynetwork.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/12/05
// [�X�V��]2019/12/05
//===================================================================================================================================
#include "networkClient.h"
#include "ImguiManager.h"

TreeTable *NETWORK_CLIENT::treeTable = nullptr;
int NETWORK_CLIENT::treeNum = 0;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
NETWORK_CLIENT::NETWORK_CLIENT()
{
	// WSA
	nRtn = WSAStartup(MAKEWORD(1, 1), &wsaData);

	// Socket
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		WSACleanup();
	}

	// IPHost
	lpHostEnt = (HOSTENT *)gethostbyname(szServer);
	if (lpHostEnt == NULL) {
		addr = inet_addr(szServer);
		lpHostEnt = (HOSTENT *)gethostbyaddr((char *)&addr, 4, AF_INET);
	}

	memset(&addrin, 0, sizeof(addrin));
	memcpy(&(addrin.sin_addr),
		lpHostEnt->h_addr_list[0],
		lpHostEnt->h_length);
	addrin.sin_port = htons(port);
	addrin.sin_family = AF_INET;
	addrin.sin_addr.s_addr = *((unsigned long *)lpHostEnt->h_addr);

	packageID = 0;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
NETWORK_CLIENT::~NETWORK_CLIENT()
{
	closesocket(s);
	WSACleanup();
	if (treeNum != 0)
	{
		SAFE_DELETE_ARRAY(treeTable);
	}
}

//===================================================================================================================================
//�y���M�z
//===================================================================================================================================
void NETWORK_CLIENT::send(float time)
{
	// buf
	packageID++;
	PACKAGE tmpPackage;
	memset(&tmpPackage, 0, sizeof(tmpPackage));
	tmpPackage.num = 10;
	tmpPackage.tmpPos = D3DXVECTOR3(100, 200, 300);
	tmpPackage.networkTester = true;
	tmpPackage.treeMax = treeNum;
	tmpPackage.treeTable = treeTable;
	tmpPackage.timer = time;
	
	//PACKAGE�̏��̂�
	char *buf1;	//�p�b�P�[�W�p(PACKAGE�̏��̂�)
	int sizeOfPackage = sizeof(PACKAGE) - (sizeof(TreeTable*));
	buf1 = (char *)malloc(sizeOfPackage);
	memcpy(buf1, &tmpPackage, sizeOfPackage);
	nRtn = sendto(s, buf1, sizeOfPackage, 0,
		(LPSOCKADDR)&addrin, sizeof(addrin));
	free(buf1);

	//TreeTable�̎���
	if (tmpPackage.treeMax > 0)
	{
		//TreeTable�̎���
		char *buf2; //�p�b�P�[�W�p(TreeTable�̎���)
		int sizeOfTreeTable = sizeof(TreeTable)*tmpPackage.treeMax;
		buf2 = (char *)malloc(sizeOfTreeTable);
		memcpy(buf2, tmpPackage.treeTable, sizeOfTreeTable);
		nRtn = sendto(s, buf2, sizeOfTreeTable, 0,
			(LPSOCKADDR)&addrin, sizeof(addrin));
		free(buf2);

		SAFE_DELETE_ARRAY(treeTable);
		treeNum = 0;	//0�N���A
	}
}

//===================================================================================================================================
//�yGUI�z
//===================================================================================================================================
void NETWORK_CLIENT::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("NetworkInformation"))
	{
		ImGui::Text("Server Name = %s", szServer);
	}
#endif
}

//===================================================================================================================================
//�y���M�z
//===================================================================================================================================
void NETWORK_CLIENT::setSendTreeTable(const TreeTable inTreeTable)
{
	if (treeNum == NULL)
	{
		treeTable = new TreeTable;
		memcpy(treeTable, &inTreeTable, sizeof(TreeTable));
	}
	else
	{
		TreeTable *tmpTreeTable;
		tmpTreeTable = new TreeTable[treeNum];
		memcpy(tmpTreeTable, treeTable, sizeof(TreeTable)*treeNum);
		SAFE_DELETE_ARRAY(treeTable);
		treeTable = new TreeTable[treeNum + 1];
		memcpy(treeTable, tmpTreeTable, sizeof(TreeTable)*treeNum);
		SAFE_DELETE_ARRAY(tmpTreeTable);
		memcpy(&treeTable[treeNum], &inTreeTable, sizeof(TreeTable));
	}

	treeNum++;
}