/*
 * SCComm.cpp
 * 基于MFC的COM口程序接口
 * 日期: 2016.8.26
 * 作者: 谭雄乐
 */
#include "stdafx.h"
#include "SCComm.h"


CSCComm::CSCComm(void)
{
	hCom = (HANDLE)-1;
	IOTimeOut = 100;
	ComListNum = -1;
	BaudRate = 1000000;
	OldParity = 0;
}

CSCComm::~CSCComm(void)
{
}

int CSCComm::Read(UCHAR *nDat, UINT nLen)
{
	if(hCom==(HANDLE)-1)
		return 0;
	int rlen = 0;
	ReadFile(hCom, nDat, nLen, (LPDWORD)&rlen, NULL);
	return rlen;
}

int CSCComm::Write(UCHAR *nDat, UINT nLen)
{
	if(hCom==(HANDLE)-1)
		return 0;
	int wlen = 0;
	WriteFile(hCom, nDat, nLen, (LPDWORD)&wlen, NULL);
	return wlen;
}

void CSCComm::FlushRXComm()
{
	PurgeComm(hCom, PURGE_RXCLEAR);//清空缓冲区
}

int CSCComm::GetComList(void)
{
	CHAR Name[25]; 
	UCHAR szPortName[25];
	LONG Status; 
	DWORD dwIndex = 0; 
	DWORD dwName; 
	DWORD dwSizeofPortName; 
	DWORD Type;
	HKEY hKey; 
	LPCTSTR data_Set="HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
	dwName = sizeof(Name); 
	dwSizeofPortName = sizeof(szPortName);

	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey);//打开一个制定的注册表键,成功返回ERROR_SUCCESS即“0”值
	if(ret0 == ERROR_SUCCESS) 
	{
		ComListNum = 0;
		do 
		{
			Status = RegEnumValue(hKey, dwIndex++, Name, &dwName, NULL, &Type, szPortName, &dwSizeofPortName);//读取键值 
			if((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA)) 
			{ 
				ComSerialList[ComListNum] = CString(szPortName);// 串口字符串保存 
				TRACE("serial:%s\n",ComSerialList[ComListNum]);
				ComListNum++;// 串口计数 
			} 
			//每读取一次dwName和dwSizeofPortName都会被修改
			dwName = sizeof(Name); 
			dwSizeofPortName = sizeof(szPortName); 
		} while((Status == ERROR_SUCCESS)||(Status == ERROR_MORE_DATA)); 

		RegCloseKey(hKey); 
	}
	return ComListNum;
}

CString CSCComm::Com2DevCom(CString ComStrName)
{
	int ComNum = (USHORT)_tcstoul(ComStrName.GetBuffer() + 3, NULL, 10);
	if(ComNum>9){
		ComStrName.Format("\\\\.\\COM%d", ComNum);
	}
	return ComStrName;
}

CString CSCComm::Com2DevCom(int ComNum)
{
	CString ComStrName;
	if(ComNum>9){
		ComStrName.Format("\\\\.\\COM%d", ComNum);
	}else{
		ComStrName.Format("COM%d", ComNum);
	}
	return ComStrName;
}

BOOL CSCComm::OpenDev(CString ComStrName)
{
	ComStrName = Com2DevCom(ComStrName);
	hCom = CreateFile(ComStrName, GENERIC_READ|GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if(hCom==(HANDLE)-1)
		return FALSE;
	return TRUE;
}

BOOL CSCComm::CloseDev(void)
{
	BOOL res = CloseHandle(hCom);
	hCom = (HANDLE)-1;
	return res;
}

int CSCComm::SetupTimeOut(UINT TimeOut)
{
	int oldTimeOut = IOTimeOut;
	IOTimeOut = TimeOut;
	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout = 0;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = TimeOut;
	TimeOuts.WriteTotalTimeoutMultiplier = 0;
	TimeOuts.WriteTotalTimeoutConstant = TimeOut;
	SetCommTimeouts(hCom, &TimeOuts);//设置超时
	return oldTimeOut;
}

UINT CSCComm::SetupDev(UINT BaudRate, int Parity)
{
	int OldBaudRate;
	if(hCom==(HANDLE)-1)
		return BaudRate;
	SetupComm(hCom,1024,1024);//设置读写缓冲区

	SetupTimeOut(IOTimeOut);
	
	DCB dcb;
	GetCommState(hCom, &dcb);

	OldBaudRate = dcb.BaudRate;
	dcb.BaudRate = BaudRate;
	dcb.ByteSize = 8;
	OldParity = dcb.Parity;
	switch(Parity){
		case 0:
			dcb.Parity = NOPARITY;
			break;
		case 1:
			dcb.Parity = ODDPARITY;
			break;
		case 2:
			dcb.Parity = EVENPARITY;
			break;
		default:
			dcb.Parity = NOPARITY;
			break;
	}
	dcb.StopBits = ONESTOPBIT;
	dcb.fRtsControl = dcb.fDtrControl = 0;
	SetCommState(hCom, &dcb);//设置串口

	PurgeComm(hCom, PURGE_TXCLEAR|PURGE_RXCLEAR);//清空缓冲区
	this->BaudRate = BaudRate;
	return OldBaudRate;
}