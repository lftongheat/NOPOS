#if !defined(_LEVEL2_H_)
#define _LEVEL2_H_
#include "ICC.h"
#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 

extern FILE *fp;


#define ERROR_Script	   -401   /*执行脚本错误*/
#define No9000	           -402   /*卡返回的SW1和SW2不等于9000H*/

extern  int LKE_ICExecute(BYTE *sCript, int nCriptLen);
extern	int LKE_ICGetAAc(BYTE *sCdol2Data, int nCodl2Len);
extern	int LKE_ICGetTc(BYTE *sCdol2Data, int nCodl2Len);
extern	int LKE_ICExAuth(BYTE *sArpc, int nArpcLen);
extern	int LKE_ICGetArqc(BYTE *sCdol1, int nCdol1Len);
extern	int LKE_ICGenCdol(BYTE *sCdol, int iCdollen, BYTE *sCdolData);
extern	int LKE_ICGetData(char *sTag);
extern	int LKE_ICGenGpo(BYTE *sPdolData, int DataLen);
extern	int LKE_ICReadRecord(BYTE sFi, int rid);
extern	int LKE_ICSendAPDU(G_APDU_COMM *ptg_apdu_comm, G_APDU_RESP *ptg_apdu_resp);
extern	int LKE_ICSelect(BYTE *sAid, int nAidLen);
extern  int LKE_ICGenerateAC(BYTE *sPdolData, int DataLen);
extern	void Parse_tlv_Trdata(char *src);

extern int ICRT_GetICCInfo(int USBFlag,char *TagList, char *AIDList, char *UserInfo,char *ICInfo,char *InputInfo);
extern int ICRT_GetTrDetail(BOOL USBFlag,int NOLog,char *AIDList, char *TrDtail);
extern int ICRT_GetARQC(BOOL USBFlag,char *TxData, char *AIDList, char *ARQC, BYTE *TemData,int *TemLen);
extern int ICRT_ARPCExeScript(BOOL USBFlag,char *Txdata, char *ARPC,int sCDol2_Len,BYTE *sCDol2, char *ScriptResult, char *TC);
extern int ICRT_Qpboc(BOOL USBFlag,char *Txdata, char *AIDList);
extern int ICRT_EEC(BOOL USBFlag,char *Txdata, char *AIDList);
#endif