//===================================================================================================================================
//�ynetwork.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/12/05
// [�X�V��]2019/12/05
//===================================================================================================================================
#include "networkClient.h"
#include "ImguiManager.h"

TreeTable NETWORK_CLIENT::treeTable[20] = { 0 };
int NETWORK_CLIENT::treeNum = 0;
bool NETWORK_CLIENT::requestConnection = false;
bool NETWORK_CLIENT::initialConnection = true;
int NETWORK_CLIENT::connectionTarget = -1;
//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace ServerListNS
{

	enum PC_LIST
	{
		KANNO,
		NAKAGOMI,
		DATE,
		SAI,
		SOMEYA,
		SHINZATO,
		PC_NUM
	};

	const char* SERVER_NAME_LIST[PC_NUM] = {
		{"ths80619"},			//KANNO,
		{"DESKTOP-2C7Q2ME"},	//NAKAGOMI
		{"ths80214"},			//DATE,
		{"INDES"},				//SAI,
		{"MSI"},				//SOMEYA,
		{"LAPTOP-74FA5EHG"},	//SHINZATO
	};
}

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace ServerListNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
NETWORK_CLIENT::NETWORK_CLIENT()
{
	success = false;
	onResetDisplay = false;
	//�ڑ��v���ɉ�����
	if (!requestConnection)return;//�ڑ��v�����Ȃ�
	//---------
	//���ʏ���
	//---------
	// WSA
	nRtn = WSAStartup(MAKEWORD(1, 1), &wsaData);

	// Socket
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		WSACleanup();
	}

	switch (initialConnection)
	{

	//---------
	//�����ڑ�
	//---------
	case true:
		//�ڑ��e�X�g
		// IPHost
		for (int i = 0; i < PC_NUM; i++)
		{
			//�ڑ��悪�I������Ă���ꍇ�̓e�X�g���Ȃ�
			if (connectionTarget != -1)continue;
			//SERVER_NAME���z�X�g���Ƃ��Đڑ������݂�
			lpHostEnt = (HOSTENT *)gethostbyname(SERVER_NAME_LIST[i]);
			if (lpHostEnt != NULL)
			{
				//�ڑ�����
				TCHAR confirm[MAX_PATH + 1];
				sprintf(confirm, "���̃z�X�g�Ɛڑ����s���܂����H\n%s",SERVER_NAME_LIST[i]);
				if (MessageBox(0, confirm, "�m�F", MB_YESNO| MB_TOPMOST) == IDYES)
				{
					connectionTarget = i;
				}
			}
			else {
				//�z�X�g���ɂ��ڑ��Ɏ��s�����̂�
				//SERVER_NAME��IP�A�h���X�Ƃ��Đڑ������݂�
				addr = inet_addr(SERVER_NAME_LIST[i]);
				if (addr != INADDR_NONE)
				{
					lpHostEnt = (HOSTENT *)gethostbyaddr((char *)&addr, 4, AF_INET);
				}
				if (lpHostEnt != NULL)
				{
					//�ڑ�����
					TCHAR confirm[MAX_PATH + 1];
					sprintf(confirm, "����IP�A�h���X�Ɛڑ����s���܂����H\n%s", SERVER_NAME_LIST[i]);
					if (MessageBox(0, confirm, "�m�F", MB_YESNO | MB_TOPMOST) == IDYES)
					{
						connectionTarget = i;
					}
				}
			}
		}

		//�ڑ��悪�I�����ꂽ���ǂ���
		if (connectionTarget == -1)
		{
			MessageBox(0,"�C���^�[�l�b�g�ڑ��Ɏ��s���܂����B","�m�F",MB_OK|MB_TOPMOST);
		}
		else {
			TCHAR confirm[MAX_PATH + 1];
			sprintf(confirm, "�C���^�[�l�b�g�ڑ��ɐ������܂����B\n�ڑ���F%s", SERVER_NAME_LIST[connectionTarget]);
			MessageBox(0, confirm, "�m�F", MB_OK | MB_TOPMOST);
		}

		//�����ڑ��I��
		initialConnection = false;

		break;

	//---------
	//���ۂ̐ڑ�
	//---------
	case false:
		//�ڑ��悪�I������Ă��Ȃ��ꍇ�͐ڑ����Ȃ�
		if (connectionTarget == -1)return;

		// IPHost
		//SERVER_NAME���z�X�g���Ƃ��Đڑ������݂�
		lpHostEnt = (HOSTENT *)gethostbyname(SERVER_NAME_LIST[connectionTarget]);
		if (lpHostEnt != NULL)
		{
			//�ڑ�����
			success = true;
		}
		else {
			//�z�X�g���ɂ��ڑ��Ɏ��s�����̂�
			//SERVER_NAME��IP�A�h���X�Ƃ��Đڑ������݂�
			addr = inet_addr(SERVER_NAME_LIST[connectionTarget]);
			if (addr != INADDR_NONE)
			{
				lpHostEnt = (HOSTENT *)gethostbyaddr((char *)&addr, 4, AF_INET);
			}
			if (lpHostEnt != NULL)
			{
				//�ڑ�����
				success = true;
			}
		}

		if (!success)
		{
			//���s�����̂ŏI��
			return;
		}

		//
		memset(&addrin, 0, sizeof(addrin));
		memcpy(&(addrin.sin_addr),
			lpHostEnt->h_addr_list[0],
			lpHostEnt->h_length);
		addrin.sin_port = htons(port);
		addrin.sin_family = AF_INET;
		addrin.sin_addr.s_addr = *((unsigned long *)lpHostEnt->h_addr);
	
		packageID = 0;

		break;
	}
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
NETWORK_CLIENT::~NETWORK_CLIENT()
{
	closesocket(s);
	WSACleanup();
	//if (treeNum != 0)
	//{
	//	//SAFE_DELETE_ARRAY(treeTable);
	//}
}

////===================================================================================================================================
////�y���M�z
////===================================================================================================================================
//void NETWORK_CLIENT::send(float time)
//{
//	// buf
//	packageID++;
//	PACKAGE tmpPackage;
//	memset(&tmpPackage, 0, sizeof(tmpPackage));
//	tmpPackage.num = 10;
//	tmpPackage.tmpPos = D3DXVECTOR3(100, 200, 300);
//	tmpPackage.networkTester = true;
//	tmpPackage.treeMax = treeNum;
//	if (tmpPackage.treeMax > 0)
//	{
//		memcpy(tmpPackage.treeTable, treeTable, sizeof(TreeTable)*tmpPackage.treeMax);
//	}
//	tmpPackage.timer = time;
//	
//	//PACKAGE�̏��̂�
//	char *buf1;	//�p�b�P�[�W�p(PACKAGE�̏��̂�)
//	int sizeOfPackage = sizeof(PACKAGE) - (sizeof(TreeTable*));
//	buf1 = (char *)malloc(sizeOfPackage);
//	memcpy(buf1, &tmpPackage, sizeOfPackage);
//	nRtn = sendto(s, buf1, sizeOfPackage, 0,
//		(LPSOCKADDR)&addrin, sizeof(addrin));
//	free(buf1);
//
//	//TreeTable�̎���
//	if (tmpPackage.treeMax > 0)
//	{
//		//TreeTable�̎���
//		char *buf2; //�p�b�P�[�W�p(TreeTable�̎���)
//		int sizeOfTreeTable = sizeof(TreeTable)*tmpPackage.treeMax;
//		buf2 = (char *)malloc(sizeOfTreeTable);
//		memcpy(buf2, tmpPackage.treeTable, sizeOfTreeTable);
//		nRtn = sendto(s, buf2, sizeOfTreeTable, 0,
//			(LPSOCKADDR)&addrin, sizeof(addrin));
//		free(buf2);
//
//		memset(treeTable, 0, sizeof(treeTable));
//		treeNum = 0;	//0�N���A
//	}
//}

//===================================================================================================================================
//�y���M�z
//===================================================================================================================================
void NETWORK_CLIENT::send(float time)
{
	//�ڑ��Ɏ��s���Ă���̂ŁA���M���s��Ȃ�
	if (!success)return;
	
	// buf
	packageID++;
	PACKAGE tmpPackage;
	memset(&tmpPackage, 0, sizeof(tmpPackage));
	tmpPackage.num = 10;
	tmpPackage.tmpPos = D3DXVECTOR3(100, 200, 300);
	tmpPackage.networkTester = true;
	tmpPackage.treeMax = treeNum;
	tmpPackage.sceneReset = onResetDisplay;
	onResetDisplay = false;
	if (tmpPackage.treeMax > 0)
	{
		memcpy(tmpPackage.treeTable, treeTable, sizeof(treeTable));
	}
	tmpPackage.timer = time;

	//PACKAGE�̏��̂�
	char *buf;	//�p�b�P�[�W�p(PACKAGE�̏��̂�)
	buf = (char *)malloc(sizeof(tmpPackage));
	memcpy(buf, &tmpPackage, sizeof(tmpPackage));
	nRtn = sendto(s, buf, sizeof(tmpPackage), 0,
		(LPSOCKADDR)&addrin, sizeof(addrin));
	free(buf);

	//TreeTable�̎���
	memset(treeTable, 0, sizeof(treeTable));
	treeNum = 0;	//0�N���A
}


//===================================================================================================================================
//�y�f�B�X�v���C�V�[���̃��Z�b�g�M���̃Z�b�g�z
//===================================================================================================================================
void NETWORK_CLIENT::resetDisplay()
{
	onResetDisplay = true;
}

//===================================================================================================================================
//�yGUI�z
//===================================================================================================================================
void NETWORK_CLIENT::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("NetworkInformation"))
	{
		ImGui::Checkbox("initilaConnection", &initialConnection);
		ImGui::Checkbox("requestConnection", &requestConnection);
		ImGui::InputInt("connectionTarget", &connectionTarget);
	}
#endif
}

//===================================================================================================================================
//�y���M�z
//===================================================================================================================================
void NETWORK_CLIENT::setSendTreeTable(const TreeTable inTreeTable)
{
	//�ڑ��Ɏ��s���Ă���̂ŁA���M���s��Ȃ�
	if (!success)return;

	//if (treeNum == NULL)
	//{
	//	treeTable = new TreeTable;
	//	memcpy(treeTable, &inTreeTable, sizeof(TreeTable));
	//}
	//else
	//{
	//	TreeTable *tmpTreeTable;
	//	tmpTreeTable = new TreeTable[treeNum];
	//	memcpy(tmpTreeTable, treeTable, sizeof(TreeTable)*treeNum);
	//	SAFE_DELETE_ARRAY(treeTable);
	//	treeTable = new TreeTable[treeNum + 1];
	//	memcpy(treeTable, tmpTreeTable, sizeof(TreeTable)*treeNum);
	//	SAFE_DELETE_ARRAY(tmpTreeTable);
	//	memcpy(&treeTable[treeNum], &inTreeTable, sizeof(TreeTable));
	//}
	treeTable[treeNum] = inTreeTable;

	treeNum++;
}