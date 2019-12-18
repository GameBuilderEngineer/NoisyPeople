#include "SerialCommunication.h"

//RS232C �V���A���ʐM
//�S�̓I�ȗ���
//�@CreateFile()�֐��Ńt�@�C�����I�[�v������B(COM�|�[�g�̃n���h�����擾����)
//�ASetupComm()PurgeComm()�֐��ő���M�o�b�t�@�[�̐ݒ�y�я��������s��

SerialCommunication::SerialCommunication()
{
	//1.�|�[�g���I�[�v��
	//COM�|�[�g�n���h���̎擾
	//�|�[�g���t�@�C���Ƃ݂Ȃ���CreateFile()�֐��ŃI�[�v�����ăn���h�����擾����
	arduino = CreateFile(
		"\\\\.\\COM3",						//�|�[�g�̖��O�F�ǂ̃|�[�g���J���̂�
		GENERIC_WRITE, 						//�A�N�Z�X���[�h�F�ʏ푗��M�Ƃ�����̂œǂݏ����������w��
		0,									//���L���[�h�F�ʏ�0�ɐݒ�@�ăI�[�v���֎~
		NULL, 								//�Z�L�����e�B�A�g���r���[�g�F�ʏ�ł�NULL�ɐݒ�
		OPEN_EXISTING, 						//�N���G�C�g�f�B�X�g���r���[�V�����F�ʏ�COM�|�[�g�͊��ɑ��݂��Ă���̂�OPEN_EXISTING�Ƃ���B
		FILE_ATTRIBUTE_NORMAL, 				//�����F�ʐM�̏ꍇ�����͂Ȃ��̂�FILE_ATTRIBUTE_NORMAL(�����Ȃ�)���w��
		NULL);								//�e���v���[�g�̃n���h���F�ʐM�̏ꍇ�֌W�Ȃ��@�ʏ�NULL���w��

	//�G���[����
	if (arduino == INVALID_HANDLE_VALUE)	//�n���h���擾�Ɏ��s�����ꍇ
	{
		MSG("PORT COULD NOT OPEN");
		return;
	}

	//2.����M�o�b�t�@������
	//�ESetupComm()�֐��ő���M�o�b�t�@�[�̗e�ʂ�ݒ肷��
	//�EPurgeComm()�֐��ő���M�o�b�t�@�[�Ɏc���Ă���f�[�^��j������
	//(��)����M�o�b�t�@�[�e�ʂ̓h���C�o�[�AOS����݂���̂�UART(�W�ω�H)���̂��̂̃n�[�h�E�F�A�I�T�C�Y�Ƃ͈قȂ�܂��B//UART�iUniversal Asynchronous Receiver Transmitter�j�V���A���ʐM�̐M���ƃp�������ʐM�p�̐M���̕ϊ����s�����i�i�W�ω�H�j
	
	returnValue = SetupComm(arduino, 1024, 1024);
	//�G���[����
	if (!returnValue)
	{
		MSG("SET UP FAILED");
		CloseHandle(arduino);
		return;
	}

	returnValue = PurgeComm(arduino, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	//�G���[����
	if (!returnValue)
	{
		MSG("CLEAR FAILED");
		CloseHandle(arduino);
		return;
	}

	//3.��{�ʐM�����̐ݒ�
	DCB dcb;
	GetCommState(arduino, &dcb);
	dcb.DCBlength	= sizeof(DCB);
	dcb.BaudRate	= 9600;
	dcb.fBinary		= TRUE;
	dcb.ByteSize	= 8;
	dcb.fParity		= NOPARITY;
	dcb.StopBits	= ONESTOPBIT;

	returnValue = SetCommState(arduino, &dcb);
	if (!returnValue)
	{
		MSG("SetCommState FAILED");
		CloseHandle(arduino);
		return;
	}


}


SerialCommunication::~SerialCommunication()
{
	CloseHandle(arduino);
}

//���M
void SerialCommunication::send(BYTE	data)
{
	DWORD	sendSize;
	//DWORD	errorMask;

	returnValue = WriteFile(arduino, &data, sizeof(data), &sendSize, NULL);
	if (!returnValue)
	{
		MSG("SEND FAILED");
		CloseHandle(arduino);
		return;
	}

}
