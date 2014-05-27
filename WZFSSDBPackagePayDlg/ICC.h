#if !defined(_ICC_H_)
#define _ICC_H_

#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 

extern EXIT;
extern FILE *fp;

typedef struct
{
   unsigned char    Command[4];
   unsigned char    Lc;
   unsigned char    DataIn[256];
   unsigned char    Le;
} G_APDU_COMM;

typedef struct
{
   unsigned int	    LengthOut;
   unsigned char	DataOut[512];
   unsigned char 	SW1;
   unsigned char 	SW2;

} G_APDU_RESP;



#define SUCC             0      /*操作成功*/
#define FAIL             -1     /*操作失败*/

#define NoOpenCom        -100   /*串口没打开*/
#define TimeOutErr       -101   /*超时*/
#define SendDataErr      -102   /*发送数据失败*/
#define RevDataErr       -103   /*接收数据错误*/
#define NoCard           -104	/*没有插卡*/
#define CardNoPower      -105	/*卡未上电*/
#define CardNoResp       -106	/*卡没有响应*/
#define CommandErr       -107	/*命令错误*/
#define UnknownErr       -108	/*其他错误*/ 
#define CardNumErr       -109	/*卡端口号错误*/
#define CardTypeErr      -110	/*卡类型错误*/
#define NoData           -111	/*卡无返回数据*/

extern int ICC_PowerUp(int *ATRLen, BYTE *ATRData);    
extern int ICC_PowerDown();
extern int ICC_ReadVersion(char *cVer);
extern int ICC_SetEMVMode();
extern int ICC_CardPresent(BYTE *Sta);
extern int ICC_CardPresentEx();
extern int ICC_CardPresentEx2(BYTE *Sta);
extern int ICC_SelectCard(int iCardNo, int iCardType);
extern int ICC_Exchange_APDU(G_APDU_COMM *ApduComm, G_APDU_RESP *ApduResp);
extern int ICC_Exchange_APDUEx(int ApduCommLen, unsigned char *ApduComm,G_APDU_RESP *ApduResp);
extern int ICC_Exchange_APDUEx2(int ApduCommLen, BYTE *ApduComm,int *ApduRespLen,BYTE *ApduResp);


#endif