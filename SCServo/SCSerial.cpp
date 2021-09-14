/*
 * SCServo.cpp
 * 飞特串行舵机硬件接口层程序
 * 日期: 2020.7.24
 * 作者: 
 */

#include "stdafx.h"
#include "SCSerial.h"

SCSerial::SCSerial()
{
	IOTimeOut = 100;
	pSerial = new CSCComm;
}

SCSerial::SCSerial(u8 End):SCS(End)
{
	IOTimeOut = 100;
	pSerial = new CSCComm;
}

SCSerial::SCSerial(u8 End, u8 Level):SCS(End, Level)
{
	IOTimeOut = 100;
	pSerial = new CSCComm;
}

bool SCSerial::begin(int baudRate, const char* serialPort)
{
	//printf("servo port:%s\n", serialPort);
    if(serialPort == NULL)
		return false;
	
	if(!pSerial->OpenDev(serialPort)){
        return false;
	}
	pSerial->SetupDev(baudRate, 0);
	return true;
}

int SCSerial::setBaudRate(int baudRate)
{ 
	return pSerial->SetupDev(baudRate, 0);
}

int SCSerial::readSCS(unsigned char *nDat, int nLen)
{	
	if(nDat==NULL){
		return 0;
	}
	return pSerial->Read(nDat, nLen);
}

int SCSerial::writeSCS(unsigned char *nDat, int nLen)
{
	if(nDat==NULL){
		return 0;
	}
	return pSerial->Write(nDat, nLen);
}

int SCSerial::writeSCS(unsigned char bDat)
{
	return pSerial->Write(&bDat, 1);
}

void SCSerial::rFlushSCS()
{
	pSerial->FlushRXComm();
}

void SCSerial::wFlushSCS()
{
}

void SCSerial::end()
{
	pSerial->CloseDev();
}