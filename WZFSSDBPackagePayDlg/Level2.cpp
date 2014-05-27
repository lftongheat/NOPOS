#include "ParseTLV.h"
#include "Level2.h"
#include "WRITELog.h"
#include "stdafx.h"

BOOL CCIDFlag;


G_APDU_COMM tg_apdu_comm;
G_APDU_RESP tg_apdu_resp;

/*����Ϊ���55����������*/
BYTE TeTxnQu[4]={0x40,0x00,0x00,0x00}; /* �ն˽������� */
BYTE Amt[6]={0x00,0x00,0x00,0x00,0x00,0x00};      /* ��Ȩ��� */
BYTE OtherAmt[6]={0x00,0x00,0x00,0x00,0x00,0x00};   /*�������*/
BYTE TerCountryCd[2]={0x01,0x56}; /* �ն˹��Ҵ��� */
BYTE CurCd[2]={0x01,0x56};    /* ���׻��Ҵ��� */
BYTE TVR[5]={0x00,0x00,0x00,0x08,0x00};/*�ն���֤���TVR,��JR/T 0025.6����A.7����λ�����ض�����,ע���4�ֽڵ�4λΪ1ʱΪ�̻�Ҫ����������(��0000000800)������Ϊ0���ѻ�����Ч�����õ�*/
BYTE TxnType[1] ={0x00};  /* �������� JR/T 0025.2 ��AP44,ֵ01��EDȦ��;02��EPȦ��;03��Ȧ��;04��EDȡ��;05��ED����;06��EP����;07��ED�޸�͸֧�޶�;08���������ѣ�����ED:���Ӵ��ۣ�EP:����Ǯ��*/
BYTE Merchant[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*�̻�����*/
BYTE Time[3]={0x00,0x00,0x00};/*����ʱ��*/
BYTE Date[3]={0x00,0x00,0x00};      /* �������� */
BYTE Random[8]={0x00,0x00,0x00,0x00};   /* ����Ԥ֪�� */
BYTE ICAIP[3]; /*Ӧ�ý������������JR/T 0025.5��2010��P104����ֵ��GPO��Ӧ������:80+����+AIP+AFL*/
BYTE Terpo[3]={0x00,0x00,0x00}; /*�ն�����*/

#ifdef DebugLog	
	char LogDate[10]="\0";
	char LogTime[20]="\0";
#endif
//��IC������APDU����
int LKE_ICSendAPDU(G_APDU_COMM *ptg_apdu_comm, G_APDU_RESP *ptg_apdu_resp)
{ 
	int iRet = 0;
    iRet =ICC_Exchange_APDU(ptg_apdu_comm, ptg_apdu_resp);
    if(iRet==SUCC)
    {   
       if(ptg_apdu_resp->SW1==0x61)
        {
         memset(ptg_apdu_comm,0,sizeof(ptg_apdu_comm));
         memset(ptg_apdu_resp,0,sizeof(ptg_apdu_resp));
         ptg_apdu_comm->Command[0]=0x00;
         ptg_apdu_comm->Command[1]=0xC0;
         ptg_apdu_comm->Command[2]=0x00;
         ptg_apdu_comm->Command[3]=0x00;
	     ptg_apdu_comm->Lc=0x00;  	 
   	     ptg_apdu_comm->Le=ptg_apdu_resp->SW2;

		iRet =ICC_Exchange_APDU(ptg_apdu_comm, ptg_apdu_resp);
	
        }	
        else if(ptg_apdu_resp->SW1==0x6c)
        {
          memset(ptg_apdu_resp,0,sizeof(ptg_apdu_resp));
          ptg_apdu_comm->Le=ptg_apdu_resp->SW2;
           iRet =ICC_Exchange_APDU(ptg_apdu_comm, ptg_apdu_resp);
        }
     } 
   
   if( iRet != SUCC) return iRet;
   return SUCC;
}

//ѡ����
int LKE_ICSelect(BYTE *sAid, int nAidLen)
{
	int iRet = 0;
    memset(&tg_apdu_comm, 0, sizeof(tg_apdu_comm));
    memset(&tg_apdu_resp, 0, sizeof(tg_apdu_resp));
    tg_apdu_comm.Command[0] = '\x00';            /* CLA = 0x00 */
    tg_apdu_comm.Command[1] = '\xA4';            /* INS = 0xA4 */
    tg_apdu_comm.Command[2] = '\x04';            /* P1  = 0x04 */
    tg_apdu_comm.Command[3] = '\x00';            /* P2  = 0x02 */
    memcpy(tg_apdu_comm.DataIn, sAid, nAidLen);
    tg_apdu_comm.Lc = nAidLen;        
    tg_apdu_comm.Le = '\x00';                    /* Le  = 0x00 */
    iRet = LKE_ICSendAPDU(&tg_apdu_comm, &tg_apdu_resp);
    if (iRet != SUCC) return iRet;
	return SUCC;
}
//����¼����
int LKE_ICReadRecord(BYTE sFi, int rid)
{
	int iRet = 0;
    memset(&tg_apdu_comm, 0, sizeof(tg_apdu_comm));
    memset(&tg_apdu_resp, 0, sizeof(tg_apdu_resp));
    tg_apdu_comm.Command[0] = '\x00';                               /* CLA = 0x00 */
    tg_apdu_comm.Command[1] = '\xB2';                               /* INS = 0xB2 */
    tg_apdu_comm.Command[2] = rid;                     /* P1  = ��¼��*/
    tg_apdu_comm.Command[3] = ((sFi << 3) | '\x04');   /* P2  = 0x02 */
    tg_apdu_comm.Le = '\x00';                                       /* Le  = 0x00 */
	iRet = LKE_ICSendAPDU(&tg_apdu_comm, &tg_apdu_resp);
    if (iRet != SUCC) return iRet;
	return SUCC;
}
//��ȡIC�����ݺ���
int LKE_ICGetData(char *sTag)
{
	int iRet = 0;
    memset(&tg_apdu_comm, 0, sizeof(tg_apdu_comm));
    memset(&tg_apdu_resp, 0, sizeof(tg_apdu_resp));
    tg_apdu_comm.Command[0] = '\x80';                  /* CLA = 0x80 */
    tg_apdu_comm.Command[1] = '\xCA';                  /* INS = 0xCA */
    tg_apdu_comm.Command[2] = sTag[0];                 /* P1  =      */
    tg_apdu_comm.Command[3] = sTag[1];                 /* P2  =      */
    tg_apdu_comm.Le = '\x00';                          /* Le  = 0x00 */
	iRet = LKE_ICSendAPDU(&tg_apdu_comm, &tg_apdu_resp);
    if (iRet != SUCC) return iRet;
	return SUCC;
}


//GPO
int LKE_ICGenGpo(BYTE *sPdolData, int DataLen)
{
	int iRet = 0;
    memset(&tg_apdu_comm, 0, sizeof(tg_apdu_comm));
    memset(&tg_apdu_resp, 0, sizeof(tg_apdu_resp));
    tg_apdu_comm.Command[0] = '\x80';            /* CLA = 0x80 */
    tg_apdu_comm.Command[1] = '\xA8';            /* INS = 0xA8 */
    tg_apdu_comm.Command[2] = '\x00';            /* P1  = 0x00 */
    tg_apdu_comm.Command[3] = '\x00';            /* P2  = 0x00 */
    tg_apdu_comm.Le = '\x00';                    /* Le  = 0x00 */
    memcpy(tg_apdu_comm.DataIn, sPdolData, DataLen);
    tg_apdu_comm.Lc = DataLen;
	iRet = LKE_ICSendAPDU(&tg_apdu_comm, &tg_apdu_resp);
    if (iRet != SUCC) return iRet;
	return SUCC;
}
//��PDOL�������PDOL���ݰ�,�Ӵ���ǽӵ�Pdol���ݲ�һ���������Ҳ��һ��
int LKE_ICGenPdol(BYTE *sPdol, int iPdollen, BYTE *sPdolData)
{
	int  iCount = 0x00;
    int iCount2 = 0;
    int iRet = 0;
    BYTE sTag[3];
    BYTE sPdolData2[256];
	memset(sTag,0,sizeof(sTag));
	memset(sPdolData2,0,sizeof(sPdolData2));
/*	while(iPdollen > 0) 
	{
		memset(sTag, 0 , sizeof(sTag));  
		iRet = Find_dol_tag(sTag, sPdol+iCount2, &iPdollen);
		if (iRet < SUCC) return iRet;

		/* �ն˽������ԣ���һ�ֽڵ�6λΪ1��ʾ֧��qPBOC,��7λΪ1��ʾ֧�ַǽ�PBOC����8λΪ1��ʾ֧��MSD */
/*		if (!memcmp(sTag, "\x9F\x66", 2))
		{
			memcpy(sPdolData2 + iCount, TeTxnQu, 4);//�ѻ�ʱΪA0
			iCount += 4;
			iCount2 += 3;
		}

		/*��Ȩ���*/
/*		if (!memcmp(sTag, "\x9F\x02", 2))
		{
			memcpy(sPdolData2+iCount, Amt, 6);
			iCount += 6;
			iCount2 += 3;
		}

		/*�������*/
/*		if (!memcmp(sTag, "\x9F\x03", 2))
		{
			memcpy(sPdolData2+iCount, OtherAmt, 6);
			iCount += 6;
			iCount2 += 3;
		}

		/*�ն˹��Ҵ���*/
/*		if (!memcmp(sTag, "\x9F\x1A", 2))
		{
			memcpy(sPdolData2+iCount, TerCountryCd, 2);
			iCount += 2;
			iCount2 += 3;
		}
		
		/*�ն���֤���TVR*/
/*		if (!memcmp(sTag, "\x95", 1))
		{
			memcpy(sPdolData2+iCount, TVR, 5);
			iCount += 5;
			iCount2 += 2;
		}
		
		/*���׻��Ҵ���*/
/*		if (!memcmp(sTag, "\x5F\x2A", 2))
		{
			memcpy(sPdolData2+iCount, CurCd, 2);
			iCount += 2;
			iCount2 += 3;
		}
		/*��������*/
/*		if (!memcmp(sTag, "\x9A", 1))
		{
			memcpy(sPdolData2+iCount,Date, 3);
			iCount += 3;
			iCount2 += 2;
		}

		/*��������TTI(��Transaction Type Identifier)*/
/*		if (!memcmp(sTag, "\x9C", 1))
		{
			memcpy(sPdolData2+iCount, TxnType, 1);
			iCount += 1;
			iCount2 += 2;
		}
		
		/*����Ԥ֪��*/
/*		if (!memcmp(sTag, "\x9F\x37", 2))
		{
			memcpy(sPdolData2+iCount, Random, 4);
			iCount += 4;
			iCount2 += 3;
		}

		/*�����ֽ��ն�֧��ָʾ����EC Terminal Support Indicator��*/
/*		if (!memcmp(sTag, "\x9F\x7A", 2))
		{
			memcpy(sPdolData2+iCount, "\x01", 1);//Ϊ1ʱ��ʾ֧�ֵ����ֽ���
			iCount += 1;
			iCount2 += 3;
		}

	}
*/	
	memcpy(sPdolData2,sPdol,iPdollen);
	iCount += iPdollen;
	iRet = Pack_tlv_Data(sPdolData,"\x83", sPdolData2, iCount);

	if (iRet < 0) 	return iRet;
	return iRet;
}



/*��Src���ҵ�tag��ǩ�����ݷ���TagMBuf�У�����TagMember��ͷ*/
int FindTagMember(char TagMember,char *TagMBuf,char *tag,BYTE *Src,int SrcLen)
{
	int iRet;
	BYTE TmpTag[512];
	memset(TmpTag,0,sizeof(TmpTag));
	TagMBuf[0]=TagMember;
	iRet=Parse_tlv_Data(TmpTag,tag,Src,SrcLen);
	if(iRet<0)
	{	 
		sprintf(TagMBuf+1,"%03d",0);
		return iRet;
	}
    if((TagMember=='B') ||(TagMember=='D'))
    {
        sprintf(TagMBuf+1,"%03d",iRet);
        memcpy(&TagMBuf[4],TmpTag,iRet);
        return (iRet);

    }
	sprintf(TagMBuf+1,"%03d",iRet*2);
	Hex2Str(iRet,TmpTag,&TagMBuf[4]);
	return (iRet*2);
}

int LKE_ICGenerateAC(BYTE *sPdolData, int DataLen)
{
	int iRet = 0;
    memset(&tg_apdu_comm, 0, sizeof(tg_apdu_comm));
    memset(&tg_apdu_resp, 0, sizeof(tg_apdu_resp));
    tg_apdu_comm.Command[0] = '\x80';                  /* CLA = 0x80 */
    tg_apdu_comm.Command[1] = '\xAE';                  /* INS = 0xAE */
    tg_apdu_comm.Command[2] = '\x40';                  /* P1  =      */
    tg_apdu_comm.Command[3] = '\x00';                  /* P2  =      */
    tg_apdu_comm.Le = '\x00';                          /* Le  = 0x00 */
	memcpy(tg_apdu_comm.DataIn, sPdolData, DataLen);
    tg_apdu_comm.Lc = DataLen;
	iRet = LKE_ICSendAPDU(&tg_apdu_comm, &tg_apdu_resp);
    if (iRet != SUCC) return iRet;
	return SUCC;
}

int ICRT_GetICCInfo(int USBFlag,char *TagList, char *AIDList, char *UserInfo, char *ICInfo,char *InputInfo)
{
        char str[200]="\0"; 
		int i,j=0,m,n;
		int iRet;
		int AfSFIcount=0;
		int FirstRecord;
		int LastRecord;
        int TagListCount=0;
		int TagListCount1=0;
	    int TagListCount2=0;
        
		BYTE AfSFIdata[50];
		BYTE sFi;
		BYTE cATR[256];
		BYTE tmpAID[20];
		BYTE PSE[14]={0x31,0x50,0x41,0x59,0x2E,0x53,0x59,0x53,0x2E,0x44,0x44,0x46,0x30,0x31};
		BYTE ICData[1024];
		int ICData_Len=0;
		int iATR_Len=0;
		int iPDOLLen = 0;		 /* PDOL���� */
		BYTE sPDOL[256];	/* PDOL */
		BYTE sPDOLData[256]={0};
		BYTE TmpTag[512];
		int iTaglen=0;
		BYTE tmpInputInfo[512];


		char AData[512]="\0"; //�˺�
		char BData[512]="\0"; //����
		char CData[512]="\0"; //֤������
		char DData[512]="\0"; //֤������
		
		char EData[512]="\0"; //���
		char FData[512]="\0"; //�������
		char GData[512]="\0"; //���ʽ����޶�
		char HData[512]="\0"; //Ӧ�û�������
		
		char IData[512]="\0"; //ʧЧ����
        char JData[512]="\0"; //IC�����к�
		char KData[512]="\0"; //���ŵ�����
		char LData[512]="\0"; //һ�ŵ�����

		char Atemp[512]="\0"; //��Ȩ���
		char Btemp[512]="\0"; //�������
		char Ctemp[512]="\0"; //�ն˹��Ҵ���
		char Dtemp[512]="\0"; //�ն���֤���
		char Etemp[512]="\0"; //���׻��Ҵ���
		char Ftemp[512]="\0"; //��������
		char Gtemp[512]="\0"; //��������
		char Htemp[512]="\0"; //����Ԥ֪��
		char Itemp[512]="\0"; //����ʱ��
		char Jtemp[512]="\0"; //�̻�����

		memcpy(Atemp,InputInfo,12);
		Atemp[12]='\0';
		memcpy(Btemp,InputInfo+12,12);
		Btemp[12]='\0';
		memcpy(Ctemp,InputInfo+24,4);
		Ctemp[4]='\0';
		memcpy(Dtemp,InputInfo+28,10);
		Dtemp[10]='\0';
		memcpy(Etemp,InputInfo+38,4);
		Etemp[4]='\0';
		memcpy(Ftemp,InputInfo+42,6);
		Ftemp[6]='\0';
		memcpy(Gtemp,InputInfo+48,2);
		Gtemp[2]='\0';
		memcpy(Htemp,InputInfo+50,8);
		Htemp[8]='\0';
		memcpy(Itemp,InputInfo+58,6);
		Itemp[6]='\0';
		memcpy(Jtemp,InputInfo+64,40);
		Jtemp[40]='\0';

		char Ktemp[4]  ="\0"; //�����к�
		char Ltemp[17] ="\0"; //Ӧ������
		char Mtemp[3]  ="\0"; //������Ϣ����
		char Ntemp[65] ="\0"; //������Ӧ������
		char Otemp[5]  ="\0"; //Ӧ�ý��׼�����
		char Ptemp[5]  ="\0"; //Ӧ�ý�������
		BYTE PtempHex[3] =  {0};
        char TemData[512]="\0";


		int AData_Len=-1;
		int BData_Len=-1;
		int CData_Len=-1;
		int DData_Len=-1;
		int HData_Len=-1;
		int IData_Len=-1;
		int JData_Len=-1;
		int KData_Len=-1;
		int LData_Len=-1;

		char TagList1[5]="\0";//���EFGHZ
		char TagList2[20]="\0";
	
		memset(cATR,0,sizeof(cATR));
		memset(tmpAID,0,sizeof(tmpAID));
		memset(sPDOL,0,sizeof(sPDOL));
		memset(sPDOLData,0,sizeof(sPDOLData));
		memset(AfSFIdata,0,sizeof(AfSFIdata));
		memset(ICData,0,sizeof(ICData));
		memset(TmpTag,0,sizeof(TmpTag));
		memset(UserInfo,0,sizeof(UserInfo));
		memset(ICInfo,0,sizeof(ICInfo));

        CCIDFlag=USBFlag;
#ifdef DebugLog	
	GetTime(LogDate,LogTime);
	if(fp!= NULL)
	{
    	fseek(fp, 0, SEEK_END);
        fprintf(fp, "-----------------------------------------------------------------------\n");
    	fprintf(fp, "%s  %s  %s\n\n","Get IC Card Infomation Start at",LogDate,LogTime);
	}
#endif
        
	     
    		iRet=ICC_PowerUp(&iATR_Len,cATR);
    		if(iRet!=SUCC) goto EXIT;


		if(AIDList!= NULL)
		{
			iRet=Str2Hex(strlen(AIDList),AIDList,tmpAID);
			iRet=LKE_ICSelect(tmpAID,iRet);
			if((iRet!=SUCC)||((tg_apdu_resp.SW1==0x6A)&&(tg_apdu_resp.SW2==0x82)))
			{
				goto NOAID;
			}
		}
		else 
		{
	NOAID:	iRet=LKE_ICSelect(PSE,14);
			if(iRet!=SUCC) goto EXIT;
		
			iRet=Parse_tlv_data_Ttag(&sFi, "\x88","\x6F\xA5" ,tg_apdu_resp.DataOut, tg_apdu_resp.LengthOut);
			if(iRet<0) goto EXIT;

			iRet=LKE_ICReadRecord(sFi,1);
			if(iRet<0) goto EXIT;
		
			iRet=Parse_tlv_data_Ttag(tmpAID,"\x4F","\x70\x61",tg_apdu_resp.DataOut,tg_apdu_resp.LengthOut);
			if(iRet<0) goto EXIT;

			iRet=LKE_ICSelect(tmpAID,iRet);
			if(iRet!=SUCC) goto EXIT;
		}
	
		iPDOLLen=Parse_tlv_data_Ttag(sPDOL, "\x9F\x38", "\x6F\xA5",tg_apdu_resp.DataOut, tg_apdu_resp.LengthOut);
		if(iPDOLLen<0)
		{

			//goto EXIT; //����POL���ݣ���'83'����ĳ�����Ϊ'00'(emv4.3 book3 6.5.8.3)
		}


		for(i=0;i<strlen(TagList);i++)
		{
		
			if((TagList[i]!='E')&&(TagList[i]!='F')&&(TagList[i]!='G')&&(TagList[i]!='H'))
			{
				TagList2[TagListCount2++]=TagList[i];
			}
			else
			{
                TagList1[TagListCount1++]=TagList[i];
			}

		}

        if(TagListCount1>0)
        {
            for(i=0;i<TagListCount1;i++)
    		{
    			if(TagList1[i]=='E')
                {     
    				iRet=LKE_ICGetData("\x9F\x79");
    				if(iRet!=SUCC) goto EXIT;
    				FindTagMember('E',EData,"\x9F\x79",tg_apdu_resp.DataOut,tg_apdu_resp.LengthOut);
                              
    			}		
    			if(TagList1[i]=='F')
    			{
    				iRet=LKE_ICGetData("\x9F\x77");
    				if(iRet!=SUCC) goto EXIT;
			        FindTagMember('F',FData,"\x9F\x77",tg_apdu_resp.DataOut,tg_apdu_resp.LengthOut);
     
                }
    			
    			if(TagList1[i]=='G')
    			{
    				iRet=LKE_ICGetData("\x9F\x78");
    				if(iRet!=SUCC) goto EXIT;
    				FindTagMember('G',GData,"\x9F\x78",tg_apdu_resp.DataOut,tg_apdu_resp.LengthOut);                               	  
                }
                
                if(TagList1[i]=='H')
    			{
    				iRet=LKE_ICGetData("\x9F\x51");
    				if(iRet!=SUCC) goto EXIT;
    				FindTagMember('H',HData,"\x9F\x51",tg_apdu_resp.DataOut,tg_apdu_resp.LengthOut);                               	  
                }
    		}

        }

        if(TagListCount2==0)  goto EXIT;
        
        memcpy(TeTxnQu,"\x40\x00\x00\x00",4);/* �ն˽�������,���� */
        memcpy(TVR,"\x00\x00\x80\x00\x00",5);/* �ն���֤���,���� */

		iRet=LKE_ICGenPdol(sPDOL,iPDOLLen,sPDOLData);	
		if(iRet<0) goto EXIT;
	
		iRet=LKE_ICGenGpo(sPDOLData, iRet);
		if(iRet!=SUCC) goto EXIT;
	
		AfSFIcount=(tg_apdu_resp.DataOut[1]-2)/4;//Ӧ���ļ�����
		memcpy(AfSFIdata,tg_apdu_resp.DataOut+4,(tg_apdu_resp.DataOut[1]-2));
		memcpy(PtempHex,tg_apdu_resp.DataOut+2,2);
		Hex2Str(2,PtempHex,Ptemp);

		for(i=0;i<AfSFIcount;i++)
		{
			sFi=*(AfSFIdata+i*4);
			sFi=sFi>>3;
	
			FirstRecord=*(AfSFIdata+1+i*4);
			LastRecord=*(AfSFIdata+2+i*4);
	
			for(j=FirstRecord;j<=LastRecord;j++)
			{
				iRet=LKE_ICReadRecord(sFi, j);
				if(iRet!=SUCC) 
				{	
					goto EXIT;
				}
         
				if(tg_apdu_resp.DataOut[1]<0x80)
				{   
				
				    for(m=0;m<TagListCount2;m++)
					{
						if(TagList2[m]=='A')
						{
							if(AData_Len<0) 
							{
    			                 AData_Len=FindTagMember('A',TemData,"\x5A",&tg_apdu_resp.DataOut[2],tg_apdu_resp.LengthOut-2);
                                 if(AData_Len>=0)  
                                 {
                                    for(n=AData_Len+3;n>0;n--)
                                    {
                                        if(TemData[n]!='F') break;
                                    }
                                    AData[0]='A';
                                    sprintf(AData+1,"%03d",(n-3));
                                    memcpy(AData+4,&TemData[4],(n-3));
                                    TagListCount++;  
                                 }
							}
						}

						if(TagList2[m]=='B')
						{
						   if(BData_Len<0) 
							{
							   	BData_Len=FindTagMember('B',BData,"\x5F\x20",&tg_apdu_resp.DataOut[2],tg_apdu_resp.LengthOut-2);									
								if(BData_Len>=0) TagListCount++;	
							}
						}
						if(TagList2[m]=='C')
						{
							if(CData_Len<0) 
							{
							   	CData_Len=FindTagMember('C',CData,"\x9F\x62",&tg_apdu_resp.DataOut[2],tg_apdu_resp.LengthOut-2);
                                if(CData_Len>=0) TagListCount++;	
							}
							
						}
						if(TagList2[m]=='D')
						{
							if(DData_Len<0) 
							{
							   	DData_Len=FindTagMember('D',DData,"\x9F\x61",&tg_apdu_resp.DataOut[2],tg_apdu_resp.LengthOut-2);							
								if(DData_Len>=0) TagListCount++;	
							}
								
						}

						if(TagList2[m]=='I')
						{
							if(IData_Len<0) 
							{
							   	IData_Len=FindTagMember('I',IData,"\x5F\x24",&tg_apdu_resp.DataOut[2],tg_apdu_resp.LengthOut-2);						
								if(IData_Len>=0) TagListCount++;	
							}
								
						}

                        if(TagList2[m]=='J')
						{
							if(JData_Len<0) 
							{
							   	JData_Len=FindTagMember('J',JData,"\x5F\x34",&tg_apdu_resp.DataOut[2],tg_apdu_resp.LengthOut-2);	
								if(JData_Len>=0) TagListCount++;	
							}
								
						}

						if(TagList2[m]=='K')
						{
							if(KData_Len<0) 
							{
								KData_Len=FindTagMember('K',KData,"\x57",&tg_apdu_resp.DataOut[2],tg_apdu_resp.LengthOut-2);//�ұ�֪��ʲô��ǩ
								if(KData_Len>=0) TagListCount++;
							}
						}
						if(TagList2[m]=='L')
						{
							if(LData_Len<0) 
							{
							   	LData_Len=FindTagMember('L',LData,"\x9F\x1F",tg_apdu_resp.DataOut,tg_apdu_resp.LengthOut);
								if(LData_Len>=0) TagListCount++;
							}
						}
					}

				if(TagListCount==TagListCount2)  goto EXIT;
				}
			}
		}
	EXIT:
		for(i=0;i<strlen(TagList);i++)
		{
			if(TagList[i]=='A')
			{
				strcat(UserInfo,AData);
				continue;
			}
			if(TagList[i]=='B')
			{
				strcat(UserInfo,BData);
				continue;
			}
			if(TagList[i]=='C')
			{
				strcat(UserInfo,CData);
				continue;
			}
			if(TagList[i]=='D')
			{
				strcat(UserInfo,DData);
				continue;
			}
			if(TagList[i]=='E')
			{
				strcat(UserInfo,EData);
				continue;
			}
			if(TagList[i]=='F')
			{
				strcat(UserInfo,FData);
				continue;
			}
			if(TagList[i]=='G')
			{
				strcat(UserInfo,GData);
				continue;
			}
			if(TagList[i]=='H')
			{
				strcat(UserInfo,HData);
				continue;
			}
			if(TagList[i]=='I')
			{
				strcat(UserInfo,IData);
				continue;
			}
			if(TagList[i]=='J')
			{
				strcat(UserInfo,JData);
				continue;
			}
			if(TagList[i]=='K')
			{
				strcat(UserInfo,KData);
				continue;
			}
			if(TagList[i]=='L')
			{
				strcat(UserInfo,LData);
				continue;
			}
		}
	iRet=Str2Hex(strlen(InputInfo),InputInfo,tmpInputInfo);
	iRet=LKE_ICGenerateAC(tmpInputInfo, iRet);
	int lenth=tg_apdu_resp.LengthOut;
	BYTE data[512]={0};
	char retGenAC[512]="\0";
	char temp[512]="\0";
	memcpy(data,tg_apdu_resp.DataOut,lenth);
	Hex2Str(lenth,data,retGenAC);
	memcpy(temp,JData+4,2);
	temp[2]='\0';
	sprintf(Ktemp,"0%2s",temp);	
	memcpy(Ltemp,retGenAC+10,16);
	Ltemp[16]='\0';
	memcpy(Mtemp,retGenAC+4,2);
	Mtemp[2]='\0';
	memcpy(Ntemp,retGenAC+26,16);
	Ntemp[16]='\0';
	memcpy(Otemp,retGenAC+6,4);
	Otemp[4]='\0';

	memset(ICInfo,0x00,sizeof(ICInfo));

	sprintf(ICInfo,"%-3s%-16s%-2s%-64s%-8s%-4s%-10s%-6s%-2s%-12s%-4s", &
		Ktemp,Ltemp,Mtemp,Ntemp,Htemp,Otemp,Dtemp,Ftemp,Gtemp,Atemp,Ptemp);
//	WriteDiary(UserInfo,"Diary.log");
	ICC_PowerDown();

#ifdef DebugLog	
	GetTime(LogDate,LogTime);
	if(fp != NULL)
	{
    	fseek(fp, 0, SEEK_END);
        fprintf(fp, "UserInfo:%s\n\n%s - %s  %s\n",UserInfo,"Get IC Card Infomation End at",LogDate,LogTime);
    }
#endif
		return iRet;

}
