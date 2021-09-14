/*
 * SCSerial.h
 * 飞特串行舵机硬件接口层程序
 * 日期: 2020.7.24
 * 作者: 
 */

#ifndef _SCSERIAL_H
#define _SCSERIAL_H

#include "SCS.h"
#include "SCComm.h"

class SCSerial : public SCS
{
public:
	SCSerial();
	SCSerial(u8 End);
	SCSerial(u8 End, u8 Level);

protected:
	virtual int writeSCS(unsigned char *nDat, int nLen);//输出nLen字节
	virtual int readSCS(unsigned char *nDat, int nLen);//输入nLen字节
	virtual int writeSCS(unsigned char bDat);//输出1字节
	virtual void rFlushSCS();//
	virtual void wFlushSCS();//
public:
	unsigned long int IOTimeOut;//输入输出超时
	int Err;
public:
	virtual int getErr(){  return Err;  }
	virtual int setBaudRate(int baudRate);
	virtual bool begin(int baudRate, const char* serialPort);
	virtual void end();
protected:
	CSCComm *pSerial;//串口指针
};

#endif