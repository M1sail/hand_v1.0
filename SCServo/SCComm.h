/*
 * SCComm.h
 * 基于MFC的COM口程序接口
 * 日期: 2016.8.26
 * 作者: 
 */

#ifndef _SCCOMM_H
#define _SCCOMM_H

class CSCComm
{
public:
	CSCComm(void);
	~CSCComm(void);
	virtual int Write(UCHAR *nDat, UINT nLen);
	virtual int Read(UCHAR *nDat, UINT nLen);
	int SetupTimeOut(UINT TimeOut);
	BOOL OpenDev(CString ComStrName);
	BOOL CloseDev(void);
	UINT SetupDev(UINT BaudRate, int Parity);
	void FlushRXComm();

	CString ComSerialList[256];
	int	GetComList(void);
public:
	int ComListNum;
	int	IOTimeOut;
	UINT BaudRate;
	UINT OldParity;
protected:
	HANDLE hCom;
	CString Com2DevCom(CString ComStrName);
	CString Com2DevCom(int ComNum);
};

#endif