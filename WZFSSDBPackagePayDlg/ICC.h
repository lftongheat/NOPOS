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



#define SUCC             0      /*�����ɹ�*/
#define FAIL             -1     /*����ʧ��*/

#define NoOpenCom        -100   /*����û��*/
#define TimeOutErr       -101   /*��ʱ*/
#define SendDataErr      -102   /*��������ʧ��*/
#define RevDataErr       -103   /*�������ݴ���*/
#define NoCard           -104	/*û�в忨*/
#define CardNoPower      -105	/*��δ�ϵ�*/
#define CardNoResp       -106	/*��û����Ӧ*/
#define CommandErr       -107	/*�������*/
#define UnknownErr       -108	/*��������*/ 
#define CardNumErr       -109	/*���˿ںŴ���*/
#define CardTypeErr      -110	/*�����ʹ���*/
#define NoData           -111	/*���޷�������*/

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