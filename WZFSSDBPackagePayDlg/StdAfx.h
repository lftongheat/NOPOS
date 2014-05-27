#if !defined(AFX_STDAFX_H__3AD9E916_5D80_4FEC_B94E_CD2B53118241__INCLUDED_)
#define AFX_STDAFX_H__3AD9E916_5D80_4FEC_B94E_CD2B53118241__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//#include <openssl/rsa.h>
#include <time.h>
#include <math.h>
#include "Des.h"
#include "Convert.h"

#include <afxsock.h>
#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Comon Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#include "ICC.h"
#include "Level2.h"
#include "WRITELog.h"
#include "ParseTLV.h"
#endif // _AFX_NO_AFXCMN_SUPPORT
#define FilexorStr ")un&*lNHBUGVde5hd]cgcfgsr^&RFYw3"
#define xorStr     "40&&*lNHBUGVde5hd]cgcfgsr^&RFYw3$SDXTFdr5EH$ndctjn"


extern	int ReadKeyData(HANDLE hCommDev,char *Data,long TimeOut);
extern	int DesInput(HANDLE hCommDev,int Msg,char *Data,long TimeOut);
extern	int CryptDataKey(HANDLE hCommDev,char M,char N,char Len,char *SrcData,char *Data,long TimeOut);
extern	int ModifyUserKeyM(HANDLE hCommDev,int M,int N,char *NewKey,long TimeOut);
extern	BOOL CloseComm();
extern	BOOL OpenComm(int pPort, int nBaudRate);
extern	int GetPinPadType();
extern	int InitKeyPad(int iComm,int iBuad);
extern	int GetComport();
extern	int GetCommData(char *Data,long TimeOut);
extern	int UnDesData(char *Src, char *Dest, char *KEY);
extern	int DesData(char *Src, int Len, char *Dest, char *KEY);
extern	int Ascii2ExtAscii(unsigned char *Src, int Len, unsigned char *Dest);
extern	int ExtAscii2Ascii(unsigned char *Src,unsigned char *Dest);
extern	void des(unsigned char *source, unsigned char *dest, unsigned char *inkey, int flg);
extern  long SendToSDB(char *sendString,char *recvString,int iTimeOut,char *SendIP,char *SendPort,unsigned int sendBufferLen);
extern  long AnalyzeP(char * IPacket,char * OPacket,char * IpayName,int IPassLen ,int Irandnum);
extern  long AnalyzeCZP(char * IPacket,char * OPacket,char * IpayName);
extern  long AnalyzeQP(char * IPacket,char * OPacket,char * IpayName);
extern  long AnalyzeCZR(char * IPacket,char * OPacket,char * IpayName);
extern  long AnalyzeVP(char * IPacket,char * OPacket,char * IpayName);
extern  long AnalyzeVou(char * IPacket,char * OPacket,char * IpayName);
extern  long AnalyzeRPP(char * IPacket,char * OPacket,char * IpayName);
extern  long AnalyzeinVaildVou(char * IPacket,char * OPacket,char * IpayName);
extern  long AnalyzeQPayed(char * IPacket,char * OPacket,char * IpayName);
extern  char StartVou[500];
extern  void EnumerateSerialPorts(CUIntArray& ports);
extern  int	 genUsrPin(char * cardNo,char * orgPass,char *encPass);
extern  void WriteDiary(CString strInfo,CString filename);
extern  void XOR(WCHAR *szSouse,   WCHAR *szDest);
extern  void FullPacketLen(char *inStrPacket);
extern  CString CPayAmount,CLateAmt,m_inNodeNO,m_inNoCount,cm_intzdType,cm_inBatP,cm_OutBatP,cm_OutFSCode;
extern  CString cm_inPayID,cm_inPayName,cm_inFeeItemNo1,cm_inFeeLevel1,cm_inFeeItemNo2,cm_inFeeLevel2,cm_inFeeItemNo3,cm_inFeeLevel3;
extern  CString cm_inFeeItemNo4,cm_inFeeLevel4,cm_inFeeItemNo5,cm_inFeeLevel5,cm_inFeeItemNo6,cm_inFeeLevel6;
extern  CString cm_inFeeItemName1,cm_inFeeItemName2,cm_inFeeItemName3,cm_inFeeItemName4,cm_inFeeItemName5,cm_inFeeItemName6;
extern  CString sendICDomain;

extern  CString cm_AdminNO,cm_UnitNO,cm_VouKind,cm_VouDate,cm_VouNO,cm_PayerName,cm_VouFee,cm_MakeUserGuId,cm_TermNO,cm_SubTermNO;
extern  CString cm_PayKind,cm_BankNO,cm_BankUnit,cm_BankName,cm_BankOper,cm_BankInDate,cm_CheckDate;
extern  CString TCPSLLMethod(int ntype,CString strMess,int nLen);

//add by tong
extern  long ToPrintDate(char * InStr,char * OutStr);
extern  long ToPrintTime(char * InStr,char * OutStr);
extern  long ToPrintMoney(char * InStr,char * OutStr);

extern  CString p_CardNO;	//卡号
extern  CString p_TransDate;//交易日期
extern  CString p_TransTime;//交易时间
extern  CString p_TraceNO;	//流水号
extern  CString p_BatchNO;	//批次号
extern  CString p_AuthNO;	//授权号
extern  CString p_RefNO;	//参考号
extern  CString p_ValidDate;//卡片有效期
extern  CString p_TransMoney;//交易金额

/*
extern  bool  OpenKeyComm(const char * CommPort, int iBaud = 9600);
extern  bool  CloseKeyComm();
extern  int SDBReadCustomerPin(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * accNo, const char * modex, const int timeout,char * idnum, char * pinLength, char * pinCrypt, char * TPKCheck);
extern  DWORD SDBOutput(void* pBuf,DWORD dwCount);
extern  int  SDBCOMSendCmd(int Length, BYTE Cmd[], BYTE Response[], int TimeOut);
extern  void SDBClearReadBuffer();
extern  void SDBClearWriteBuffer();
extern  DWORD SDBGetInBufferCount();
extern  DWORD SDBGetInput(void* pBuf,DWORD dwCount,DWORD dwMilliseconds=1000);
extern  void  SDBProcessMessageLoop();
extern  long  SDBFormatAnsiX98Account(const char * AccNo, char * AnsiX98AccNo);
extern  int   SDBReadDeviceInfo(char *commPort, char *strInfo);
extern  int   LoadWorkKey(char *Commport, char *keyValue,short nMNo ,short nWNo);
extern  short LoadWorkKye(char *Commport,char *keyValue,char *keyCheck);
extern  int   vOne2Two(unsigned char *DataIn,unsigned char *DataOut,int iLen);
extern  int   vTwo2One( unsigned char *psFrom, unsigned char *psTo, int  uiFromLen );
extern  int   SDBInputKEY(const char * CommPort, const char *ExtPort,const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue);
*/
extern  int   SDBReadDeviceInfo(char *commPort, char *strInfo);
extern  bool  OpenKeyComm(const char * CommPort, int iBaud = 9600);
extern  bool  CloseKeyComm();
extern  DWORD SDBOutput(void* pBuf,DWORD dwCount);
extern  int  SDBCOMSendCmd(int Length, BYTE Cmd[], BYTE Response[], int TimeOut);
extern  void SDBClearReadBuffer();
extern  void SDBClearWriteBuffer();
extern  DWORD SDBGetInBufferCount();
extern  DWORD SDBGetInput(void* pBuf,DWORD dwCount,DWORD dwMilliseconds=1000);
extern  void  SDBProcessMessageLoop();
extern  short LoadWorkKye(char *Commport,char *keyValue,char *keyCheck);
extern  int SDBInputKEY(const char * CommPort, const char *ExtPort,const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue);
extern  int SDBReadCustomerPin(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * accNo, const char * modex, const int timeout,char * idnum, char * pinLength, char * pinCrypt, char * TPKCheck);

extern int WriteData( unsigned char *buffer, int size );
extern int ReadData( void *buffer, int limit );
extern  void  ClearBuffer();
extern int GetReturn(unsigned char *buffer, int Least, int OutTime, char* Endflag= NULL, int nflagL=0);

extern  int   hasReadPasswd,hasReadCard,CancelFlag,RefundFlag;
extern	int   m_Comport,m_TimeOutV;
extern 	      HANDLE hCommDev;// 串口设备句柄
extern	int   BpKeyType;
extern	char  Track2Data[256];
extern	char  Track3Data[256];
extern  char  m_inPayNO[500];
extern  char  m_OutPacket[6000];
extern  char  cardno[30];
extern  long  FReturnVal;
extern  char  m_IP1[50],m_Port1[10],m_IP2[50],m_Port2[10];
extern  char  prtWaterNo[30];
extern  char  pReturnV[50];
extern  long  prtFlag;
extern  char  m_MerchantNO[16];

extern   OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
extern   BOOL m_bOpened;  //串口打开标志

extern	char  m_FeeItemNo[10][20+1];
extern	char  m_FeeLevel[10][16+1];
extern	char  m_FeeItemKind[10][20+1];
extern	char  m_FeeItemCount[10][16+1];
extern	char  m_FeeItemAmt[10][16+1];
// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>			// MFC database classes
#include <afxdao.h>			// MFC DAO database classes

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3AD9E916_5D80_4FEC_B94E_CD2B53118241__INCLUDED_)
