#include "ICC.h"
#include "SerialPort.h"
#include "WRITELog.h"
#include "stdafx.h"

//拆字
int Dpsplit(int ArrayLen, BYTE *InArray, BYTE *OutBuffer)
{
	int i,j;
	char ch1,ch2;
	j=0;
	for(i=0;i<ArrayLen;i++)
	{
		ch1=(InArray[i]&0xf0)>>4;
		ch2=(InArray[i]&0x0f);
		if(ch1<10)
		{
			OutBuffer[j++]=ch1+0x30;
		}
		else
		{
			OutBuffer[j++]=ch1+0x37;
		}
	
		if(ch2<10)
		{
			OutBuffer[j++]=ch2+0x30;
		}
		else
		{
			OutBuffer[j++]=ch2+0x37;
		}
	}
	OutBuffer[j]=0;
	return j;
}
//合字
int UnDpsplit(int InLen, BYTE *InBuffer, BYTE *OutArray)
{
	int  i = 0,j = 0;
	char tmp;
	for( i=0,j=0;i < InLen;)
	{
	    if( InBuffer[i] >= 0x30 && InBuffer[i] <= 0x39 )
	      {
	       	tmp = (InBuffer[i] - 0x30)<<4;
	       }
	    else if( InBuffer[i] >= 0x41 && InBuffer[i] <= 0x46 )
	      {
	       	tmp=(InBuffer[i] - 0x37)<<4;
	       }
	    else if( InBuffer[i] >= 0x61 && InBuffer[i] <= 0x66 )
	      {
	       	tmp=(InBuffer[i] - 0x57)<<4;
	       			
	      }
	    i++;
	      		
	    if( InBuffer[i] >= 0x30 && InBuffer[i] <= 0x39 )
	      {
	       	tmp = tmp | (InBuffer[i] - 0x30);
	       }
	    else if( InBuffer[i] >= 0x41 && InBuffer[i] <= 0x46 )
	      {
	       	tmp= tmp | (InBuffer[i] - 0x37);
	
	      }
	    else if( InBuffer[i] >= 0x61 && InBuffer[i] <= 0x66 )
	      {
	       	tmp =tmp | (InBuffer[i] - 0x57);
	       }
	      		
		OutArray[j++] = tmp;
		i++;
	}
   	return(j);
}
//错误代码
int ICC_Errcode(BYTE Code)
{
	int ret;
	switch(Code)
	{
		case 0xFB:
			ret= NoCard;
			break;
		case 0xA2:
			ret= CardNoResp;
			break;
		case 0x15:
			ret= CardNoPower;
			break;
		case 0x1B:
			ret=CommandErr;
			break;
		default:
			ret=UnknownErr;
			break;
	}
	return  ret;
}
//将长度为SendLen的SendBuff打包为TLP224格式然后发送并接收应答数据
int ICC_TLP(int SendLen, BYTE *SendBuff, int *ReadLen, BYTE *ReadBuff, int TimeOut)
{
	BYTE Data[300],Tmp[512],ReBuf[1024],SendData[1024];
	BYTE LRC;
	int i,Slen,Rlen;
	if(TimeOut < 1 || TimeOut > 300) TimeOut = 15;

	memset(Data,0,sizeof(Data));
	memset(Tmp,0,sizeof(Tmp));
	memset(ReBuf,0,sizeof(ReBuf));
	memset(SendData,0,sizeof(SendData));

	*ReadLen=0;

	Data[0]=0x60;
	Data[1]=SendLen;
	LRC =0x00;
	for(i=0;i<SendLen;i++)
	{
		Data[2+i]=SendBuff[i];
	}

	Slen=SendLen+2;

	for(i=0;i<Slen;i++)
	{
		LRC=LRC^Data[i];
	}
	
	Data[Slen++]=LRC;
    
#ifdef DebugLog
    WriteLog("Send:");
    WriteLog_APDU(Data,Slen);
#endif    
	Slen=Dpsplit((SendLen+3),Data,SendData);
    
	SendData[Slen++]=0x03;
	SendData[Slen]=0;
    	
    ClearBuffer();
	int nRet=WriteData(SendData, Slen);
	if(nRet != Slen) return SendDataErr;
    
	Slen = GetReturn(ReBuf,1,TimeOut,"\x03",1);	

	if(Slen<1)  return TimeOutErr;  //超时
	if(ReBuf[Slen-1]!=0x03) return RevDataErr; //返回格式错		

	Rlen=UnDpsplit(Slen-1,ReBuf,Tmp);
    
#ifdef DebugLog
    WriteLog("Resp:");
    WriteLog_APDU(Tmp, Rlen);
    WriteLog("\n");
#endif    

	LRC=0;
	for(i=0;i<Rlen;i++)
	{
	    LRC^=Tmp[i];
	}
	
	if(Tmp[0] != 0x60|| LRC!=0x00 )   return RevDataErr;//返回格式错
	if(Tmp[2] != 0x00 && (Tmp[2]&0xF0) != 0xE0)   return (ICC_Errcode(Tmp[2]));//返回出错代码   

    *ReadLen=Rlen-4;
	for(i=0;i<*ReadLen;i++)
	{
	   ReadBuff[i] = Tmp[i+3]; 
	}

	return SUCC;
}

//上电
int ICC_PowerUp(int *ATRLen, BYTE *ATRData)
{
	BYTE CmdData[32], ReBuf[300];
	int	Ret;
/*
    ClosePort();
    Ret=OpenPort(1,9600,8,1,0);
    if(Ret!=SUCC) return NoOpenCom;
    ClearBuffer();
    SelectExtPort('C');

    Ret=ICC_SelectCard(0,0);
    if(Ret!=SUCC) 
    { 
        ClosePort();
        return Ret;
    }
*/
	memset(CmdData,0,sizeof(CmdData));
	memset(ReBuf,0,sizeof(ReBuf));
	CmdData[0]=0x12;
	Ret=ICC_TLP(1,CmdData,ATRLen,ATRData,10);
  //  ClosePort();
	return Ret; 
}
//IC卡下电
int ICC_PowerDown()
{
	BYTE CmdData[32], ReBuf[300];
	int	Ret;
	int RLen;
 /*   
    Ret=OpenPort(1,9600,8,1,0);
    if(Ret!=SUCC) return NoOpenCom;
    ClearBuffer();
    SelectExtPort('C');
*/
	memset(CmdData,0,sizeof(CmdData));
	memset(ReBuf,0,sizeof(ReBuf));
	CmdData[0]=0x11;

	Ret=ICC_TLP(1,CmdData,&RLen,ReBuf,10);
   // ClosePort();
	return Ret;   
}
//读取IC卡读卡器版本号
int ICC_ReadVersion(char *cVer)
{
	BYTE CmdData[32], ReBuf[300];
	int	Ret;
	int RLen,i;

	memset(CmdData,0,sizeof(CmdData));
	memset(ReBuf,0,sizeof(ReBuf));
	CmdData[0]=0x22;
	CmdData[1]=0x05;
	CmdData[2]=0x3F;
	CmdData[3]=0xE0;
    CmdData[4]=0x10;

	Ret=ICC_TLP(5,CmdData,&RLen,ReBuf,10);
	if(Ret==SUCC)
	{
		for(i=0;i<RLen;i++) cVer[i]=ReBuf[i];
		cVer[i]=0;
	}
    return Ret;  
}
//设置IC卡设置为EMV模式
int ICC_SetEMVMode()
{
	BYTE CmdData[32], ReBuf[300];
	int Len;
	int	Ret;

	memset(CmdData,0,sizeof(CmdData));
	memset(ReBuf,0,sizeof(ReBuf));
    CmdData[0]=0x17;
    CmdData[1]=0x00;
	CmdData[2]=0x45;	
	Ret=ICC_TLP(3,CmdData,&Len,ReBuf,10);
	return Ret;  
}
//获取卡片状态1
int ICC_CardPresent(BYTE *Sta)
{
	BYTE CmdData[32], ReBuf[300];
	int Len;
	int	Ret;

	memset(CmdData,0,sizeof(CmdData));
	memset(ReBuf,0,sizeof(ReBuf));
   	
	CmdData[0]=0x17;
	Ret=ICC_TLP(1,CmdData,&Len,ReBuf,10);
	if(Ret==SUCC)
	{ 
	  memcpy(Sta,ReBuf,Len);
	}
	return Ret;  

}
//获取卡片状态2
int ICC_CardPresentEx()
{
	BYTE CmdData[32], ReBuf[300];
	int Len;
	int	Ret;
	memset(CmdData,0,sizeof(CmdData));
	memset(ReBuf,0,sizeof(ReBuf));
   	
	CmdData[0]=0x17;
	Ret=ICC_TLP(1,CmdData,&Len,ReBuf,1);
	if(Ret==0)
	{   
		if((ReBuf[0]&0x04) ==0)      return 2;/*没插卡*/
	    else if((ReBuf[0]&0x02)==0)  return 0;/*已插卡，未上电*/
	    else return 1;	/*已插卡，已上电*/
    }
    else return Ret; /*其实错误信息*/
}
//获取卡片状态3
int ICC_CardPresentEx2(BYTE *Sta)
{
	BYTE CmdData[32], ReBuf[300];
	int Len;
	int	Ret;

	memset(CmdData,0,sizeof(CmdData));
	memset(ReBuf,0,sizeof(ReBuf));
   	
	CmdData[0]=0x17;
	Ret=ICC_TLP(1,CmdData,&Len,ReBuf,10);
	if(Ret==0)
	{
	    memcpy(Sta,ReBuf,Len);
		if((ReBuf[0]&0x04) ==0)      return 2;/*没插卡*/
	    else if((ReBuf[0]&0x02)==0)  return 0;/*已插卡，未上电*/
	    else return 1;	/*已插卡，已上电*/
    }
    else return Ret; /*其实错误信息*/
}

//设置卡型 iCardNo:主卡、辅卡... iCardType:接触与非接触
int ICC_SelectCard(int iCardNo, int iCardType)
{	
	int len;
	int Ret;
	BYTE CardNo,CardType;
	BYTE SBuff[16];
	BYTE RBuff[128];
    memset(SBuff,0,sizeof(SBuff));
    memset(RBuff,0,sizeof(RBuff));
	if (iCardNo<0 || iCardNo>7)     return CardNumErr; 
	if (iCardType<0 || iCardType>4) return CardTypeErr; 
	
	if(iCardNo == 0) CardNo = 0x00;//主卡
	if(iCardNo == 1) CardNo = 0x01;//辅卡
	if(iCardNo == 2) CardNo = 0x07;//非接触卡
	if(iCardNo == 3) CardNo = 0x02;//SAM1卡
	if(iCardNo == 4) CardNo = 0x03;//SAM2卡
	if(iCardNo == 5) CardNo = 0x04;//SAM3卡
	if(iCardNo == 6) CardNo = 0x05;
	if(iCardNo == 7) CardNo = 0x06;
		
	if(iCardType == 0) CardType = 0x02;//接触
	if(iCardType == 1) CardType = 0x22;//非接触
	if(iCardType == 2) CardType = 0x42;
	if(iCardType == 3) CardType = 0x02;
		
	SBuff[0]=0x17;
	SBuff[1]=CardType;
	SBuff[2]=CardNo;
	Ret=ICC_TLP(3,SBuff,&len,RBuff,10);
	return Ret;    
}
//APDU数据交换
int ICC_Exchange_APDU(G_APDU_COMM *ApduComm, G_APDU_RESP *ApduResp)
{
	BYTE SBuff[500], RBuff[1000],Stmp[500];
	int len;
	BYTE Dsw[2]={0x00};
	int	SendLen,i,Ret,TmpLen;
/*
    Ret=OpenPort(1,9600,8,1,0);
    if(Ret!=SUCC) return NoOpenCom;
    ClearBuffer();
    SelectExtPort('C');
*/

	memset(Stmp,0,sizeof(Stmp));
    memset(SBuff,0,sizeof(SBuff));
	memset(RBuff,0,sizeof(RBuff));
 
	TmpLen=0;
    Stmp[TmpLen++]=0x15; //主卡
	Stmp[TmpLen++]=ApduComm->Command[0];
	Stmp[TmpLen++]=ApduComm->Command[1];
    Stmp[TmpLen++]=ApduComm->Command[2];
	Stmp[TmpLen++]=ApduComm->Command[3];
	
	if(ApduComm->Lc !=0)
    {
      Stmp[TmpLen++]=ApduComm->Lc;
	  for(i=0;i<ApduComm->Lc;i++) Stmp[TmpLen++]=ApduComm->DataIn[i];
	}
	
	else
    {
      Stmp[TmpLen++]=ApduComm->Le;
	}
	
    if(TmpLen<=255)
	{  
	  Ret=ICC_TLP(TmpLen,Stmp,&len,RBuff,10);

      if(Ret!=SUCC) return Ret;
	  if(len<2) return  RevDataErr;/*no get data*/

	  ApduResp->LengthOut = len-2;
      for(i=0;i<len-2;i++) ApduResp->DataOut[i]=RBuff[i];

      ApduResp->SW1 =RBuff[len-2];
      ApduResp->SW2 =RBuff[len-1];
    }
	else//需要发送的数据总长度len>255，分成两次交换，No1: 15/1D FF  FF  FF FF Data(lc-254) Le
    {   
	  SendLen=0;
	  SBuff[SendLen++]=0x15; 
      SBuff[SendLen++]=0xFF;
      SBuff[SendLen++]=0xFF;
      SBuff[SendLen++]=0xFF;
      SBuff[SendLen++]=0xFF;
	  
	  SBuff[SendLen++]=TmpLen-255; //Lc-255
      
	  for(i=0;i<TmpLen-255;i++)
	  {
	   SBuff[SendLen++]=Stmp[255+i];
	  }
      
 	  Ret=ICC_TLP(SendLen,SBuff,&len,RBuff,10);//第一个数据包交换 
	  if(Ret!=SUCC) return Ret;
	  Ret=ICC_TLP(255,Stmp,&len,RBuff,10);//第二个数据包交换
	  if(Ret!=SUCC) return Ret;
	  if(len<2) return RevDataErr;/*no get data*/
 
	  ApduResp->LengthOut = len-2;
      for(i=0;i<len-2;i++) ApduResp->DataOut[i]=RBuff[i];

      ApduResp->SW1 =RBuff[len-2];
      ApduResp->SW2 =RBuff[len-1];
	}

    //ClosePort();
	return SUCC; 
}


int ICC_Exchange_APDUEx(int ApduCommLen, unsigned char *ApduComm,G_APDU_RESP *ApduResp)
{

	BYTE SBuff[500], RBuff[1000],Stmp[500];
	int len;
	BYTE Dsw[2]={0x00};
	int	SendLen,i,Ret,TmpLen;

	memset(Stmp,0,sizeof(Stmp));
    memset(SBuff,0,sizeof(SBuff));
	memset(RBuff,0,sizeof(RBuff));

	TmpLen=0;
    Stmp[TmpLen++]=0x15; 
    for(i=0;i<ApduCommLen;i++)
	{
	   Stmp[TmpLen++]=ApduComm[i];
	}

   if(TmpLen<=255)
	{  
	  Ret=ICC_TLP(TmpLen,Stmp,&len,RBuff,10);

      if(Ret!=SUCC) return Ret;
	  if(len<2) return  RevDataErr;/*no get data*/
	  
	  ApduResp->LengthOut = len-2;
      for(i=0;i<len-2;i++) ApduResp->DataOut[i]=RBuff[i];

      ApduResp->SW1 =RBuff[len-2];
      ApduResp->SW2 =RBuff[len-1];
    }
	else//需要发送的数据总长度len>255，分成两次交换，No1: 15/1D FF  FF  FF FF Data(lc-254) Le
    {   
	  SendLen=0;
	  SBuff[SendLen++]=0x15; 
      SBuff[SendLen++]=0xFF;
      SBuff[SendLen++]=0xFF;
      SBuff[SendLen++]=0xFF;
      SBuff[SendLen++]=0xFF;
	  
	  SBuff[SendLen++]=TmpLen-255; //Lc-255
      
	  for(i=0;i<TmpLen-255;i++)
	  {
	    SBuff[SendLen++]=Stmp[255+i];
	  }
      
 	  Ret=ICC_TLP(SendLen,SBuff,&len,RBuff,10);//第一个数据包交换 
	  if(Ret!=SUCC) return Ret;
	  Ret=ICC_TLP(255,Stmp,&len,RBuff,10);//第二个数据包交换
	  if(Ret!=SUCC) return Ret;
	  if(len<2) return RevDataErr;/*no get data*/
	  
	  ApduResp->LengthOut = len-2;
      for(i=0;i<len-2;i++) ApduResp->DataOut[i]=RBuff[i];

      ApduResp->SW1 =RBuff[len-2];
      ApduResp->SW2 =RBuff[len-1];
	}
	return SUCC; 
}
int ICC_Exchange_APDUEx2(int ApduCommLen, BYTE *ApduComm,int *ApduRespLen,BYTE *ApduResp)
{

    BYTE SBuff[500], RBuff[1000],Stmp[500];
    BYTE Dsw[2]={0x00};
    int SendLen,i,Ret,TmpLen;

    memset(Stmp,0,sizeof(Stmp));
    memset(SBuff,0,sizeof(SBuff));
    memset(RBuff,0,sizeof(RBuff));

    TmpLen=0;
    Stmp[TmpLen++]=0x15; 
    for(i=0;i<ApduCommLen;i++)
    {
        Stmp[TmpLen++]=ApduComm[i];
    }

    if(TmpLen<=255)
    {  
        Ret=ICC_TLP(TmpLen,Stmp,ApduRespLen,ApduResp,10);

        if(Ret!=SUCC) return Ret;
        if(*ApduRespLen<2) return  RevDataErr;/*no get data*/

    }
    else//需要发送的数据总长度len>255，分成两次交换，No1: 15/1D FF  FF  FF FF Data(lc-254) Le
    {   
        SendLen=0;
        SBuff[SendLen++]=0x15; 
        SBuff[SendLen++]=0xFF;
        SBuff[SendLen++]=0xFF;
        SBuff[SendLen++]=0xFF;
        SBuff[SendLen++]=0xFF;

        SBuff[SendLen++]=TmpLen-255; //Lc-255

        for(i=0;i<TmpLen-255;i++)
        {
            SBuff[SendLen++]=Stmp[255+i];
        }

        Ret=ICC_TLP(SendLen,SBuff,ApduRespLen,ApduResp,10);//第一个数据包交换 
        if(Ret!=SUCC) return Ret;
        Ret=ICC_TLP(255,Stmp,ApduRespLen,ApduResp,10);//第二个数据包交换
        if(Ret!=SUCC) return Ret;
        if(*ApduRespLen<2) return RevDataErr;/*no get data*/

    }
    return SUCC; 
}

