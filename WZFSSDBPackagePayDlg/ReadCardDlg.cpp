// ReadCardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSDBPackagePayDlg.h"
#include "ReadCardDlg.h"
#include "afxmt.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEvent m_Tend;
CEvent m_Tbegin;


/////////////////////////////////////////////////////////////////////////////
// ReadCardDlg dialog


ReadCardDlg::ReadCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ReadCardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ReadCardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ReadCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ReadCardDlg)
	DDX_Control(pDX, IDC_GetAmount, m_payamount);
	DDX_Control(pDX, IDC_BCANCEL, m_cancelCard);
	DDX_Control(pDX, IDC_GetCardNo, m_comcardnoctl);
	DDX_Control(pDX, IDC_GetPass, m_compasswordctl);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ReadCardDlg, CDialog)
	//{{AFX_MSG_MAP(ReadCardDlg)
		// NOTE: the ClassWizard will add message map macros here
	ON_MESSAGE(WM_READCARDFINISHED,OnReadCardFinished)
	ON_BN_CLICKED(IDC_BCANCEL, OnBcancelRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReadCardDlg message handlers
LRESULT ReadCardDlg::OnReadCardFinished(WPARAM wParam,LPARAM lParam)
{
		WriteDiary("获得刷卡卡号：" + CString(cardno),"Diary.log");
	    m_comsprompt.StartBkBlink(FALSE, CColorStaticST::ST_FLS_FAST);
		m_comsprompt.StartTextBlink(FALSE, CColorStaticST::ST_FLS_FAST);
		m_comsprompt.SetTextColor(RGB(255,255,255));
	    m_comsprompt.SetBkColor(250);
		m_cancelCard.EnableWindow(FALSE);
	    int hasReadPasswd =-1;
 		if(hasReadCard == 0 || hasReadCard == 1)//0:磁条卡 1:IC卡
 		{
			if (strlen(cardno) < 10) goto EXIT0;
 			m_comcardnoctl.SetWindowText(CString(cardno));
 			m_comcardnoctl.RedrawWindow();
			m_comsprompt.SetWindowText("  刷卡成功!  等待客户输入密码......");
			unsigned int i;
			char wCardno[20];
			memset(wCardno,0x00,sizeof(wCardno));
			strcpy(wCardno,cardno);
			for(i=6;i<12;i++)
			{
			   wCardno[i] ='*';
			}

			WriteDiary("获得刷卡卡号：" + CString(wCardno),"Diary.log");
 		}
 		else
        {
EXIT0:	    DWORD dwBytesWritten;
			dwBytesWritten=0;
			WriteFile(hCommDev,"\033N",2,&dwBytesWritten,NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\033B4",3,&dwBytesWritten,NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1B\x30\x1B%A",5,&dwBytesWritten,NULL);
			CloseComm();
			m_Tend.SetEvent();
			WriteDiary("刷卡失败！ 退出交易！","Diary.log");
		    if (strlen(cardno)< 10)	
			{
				FReturnVal = -81;
				memcpy(pReturnV,"-81",3);
				WriteDiary("无效卡号！","Diary.log");
 			    EndDialog(-81);
			}
			else
			{
				FReturnVal = -81;
				memcpy(pReturnV,"-81",3);
				WriteDiary("刷卡有误！","Diary.log");
                EndDialog(hasReadCard);
			}
 			return -100; 
 		}
		if ( CancelFlag == 0 )
		{
			WriteDiary("刷卡成功!  等待客户输入密码......","Diary.log");
 			hasReadPasswd = ReadPassWord();
			EndDialog(hasReadPasswd);
		}
		else
		{
			FReturnVal = -80;
			memcpy(pReturnV,"-80",3);
			EndDialog(-80);
		}
		ReadCardDlg::OnOK();
		return 0;
}

BOOL ReadCardDlg::OnInitDialog() 
{
// 	WriteDiary("-----------oninitdialog-----------","Diary.log");
	CDialog::OnInitDialog();
	m_payamount.SetWindowText(CPayAmount);
	// TODO: Add extra initialization here
    // Create a font
	m_Font.CreateFont(14, 0, 0, 0, FALSE, FALSE, FALSE,0,0,0,0,0,0, "Verdana");
	// Create the colored 
	m_comsprompt.SubclassDlgItem(IDC_Prompt, this);
	// Set background blinking colors
	// Medium and light red
	m_comsprompt.SetBlinkBkColors(RGB(128, 0, 0), RGB(255,0,0));
	// Start background blinking
	m_comsprompt.StartBkBlink(TRUE, CColorStaticST::ST_FLS_FAST);
	// Set text blinking colors
	// Medium red and white
	m_comsprompt.SetBlinkTextColors(RGB(128,0,0), RGB(255,255,255));
	// Start text blinking
	m_comsprompt.StartTextBlink(TRUE, CColorStaticST::ST_FLS_FAST);
	// Set a special font
	m_comsprompt.SetFont(&m_Font);
	// We want be notified when the control blinks
	m_comsprompt.EnableNotify(this, WM_USER + 10);
    AfxBeginThread(CThreadReadCardProc,GetSafeHwnd());  
    m_Tbegin.SetEvent();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
UINT CThreadReadCardProc(LPVOID pParam)
{
	//调用等待启动事件,如果没有启动事件信号将一直等待下去
	::WaitForSingleObject(m_Tbegin,INFINITE);
 	//调用等待停止事件
 	ReadCardDlg   PReadComCardDlg;
	PReadComCardDlg.ThreadReadCard();
	///////////////////////////////////////
	    if(::WaitForSingleObject(m_Tend,0)==WAIT_OBJECT_0)
		{
			//告诉主窗口线程已经结束
		   ::PostMessage((HWND)pParam,WM_READCARDFINISHED,0,0);
		}
	return 0;
}

int ReadCardDlg::ThreadReadCard()
{
	WriteDiary("开始刷卡....","Diary.log");
	hasReadCard = ReadCardNo();
    m_Tend.SetEvent();
	return 0;
}

int ReadCardDlg::OnBcancelRead() 
{
	// TODO: Add your control notification handler code here
	WriteDiary("刷卡已撤销！","Diary.log");
	CancelFlag = 1;
	DWORD dwBytesWritten;
	dwBytesWritten=0;
	WriteFile(hCommDev,"\033N",2,&dwBytesWritten,NULL);
	dwBytesWritten=0;
	WriteFile(hCommDev,"\033B4",3,&dwBytesWritten,NULL);
	dwBytesWritten=0;
	WriteFile(hCommDev,"\x1B\x30\x1B%A",5,&dwBytesWritten,NULL);
	CloseComm();
	m_Tend.SetEvent();
	FReturnVal =-80;
	EndDialog(-80);
	ReadCardDlg::OnOK();
	return 0;	
}

int ReadCardDlg::ReadCardNo()
{
 	WriteDiary("-----------onReadCardNo-----------","Diary.log");
	int ret,I_Ri;
	char cpayamount[64],cbuf[300];
	int  iloop,ircvlen,ilen;
    DWORD dwBytesWritten=0;
	float cfamount;
	char val[2];
	memset(val,0x00,2);
    sendICDomain = "";

    ret =0;
	if(!m_Comport)
	{
		ret=GetComport();
        hasReadCard = ret;
		if(ret<0)
		{ 
			FReturnVal = -83;
			return(-83);
		}
	}
	ret =0;
	hasReadCard =-1;
	if(OpenComm(m_Comport,9600))
	{
		//显示扣款金额
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1b\x25K",3,&dwBytesWritten, NULL);
		memset(cbuf,0x00,sizeof(cbuf));
		switch(BpKeyType) {
		case 1: //使用LG(DES)密码键盘
			strcpy(cbuf,"\033V`98`85`7;`7<`9:`9;: ");
			memset(cpayamount,0x00,sizeof(cpayamount));
			for(ilen=0;ilen<CPayAmount.GetLength();++ilen)
				cpayamount[ilen]=CPayAmount.GetAt(ilen);
			cpayamount[ilen]=0x00;
			cfamount = (float) atof(cpayamount);
			sprintf(cpayamount,"%7.2f",cfamount);
			memcpy(cbuf + strlen(cbuf),cpayamount,strlen(cpayamount));
			memcpy(cbuf + strlen(cbuf),"`7?\015\012",5);
			dwBytesWritten=0;
			WriteFile(hCommDev,cbuf,strlen(cbuf),&dwBytesWritten,NULL);
			Sleep(500);
			break;
		case 2: 
		case 3://新密码键盘
			strcpy(cbuf,"\033V");
			memset(cpayamount,0x00,sizeof(cpayamount));
			for(ilen=0;ilen<CPayAmount.GetLength();++ilen)
				cpayamount[ilen]=CPayAmount.GetAt(ilen);
			cpayamount[ilen]=0x00;
			cfamount = (float) atof(cpayamount);
			sprintf(cpayamount,"%7.2f",cfamount);
			memcpy(cbuf + strlen(cbuf),cpayamount,strlen(cpayamount));
			memcpy(cbuf + strlen(cbuf),"\015\012",5);
			dwBytesWritten=0;
			WriteFile(hCommDev,cbuf,strlen(cbuf),&dwBytesWritten,NULL);
//			MessageBox(cbuf,NULL,MB_OK);
//			Sleep(3000);
			break;
		}
		//读卡操作
		if(3 == BpKeyType) {
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1b\x25\x42",3,&dwBytesWritten, NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1b\x79\x0D\x0A",4,&dwBytesWritten, NULL);
			memset(cbuf,0x00,sizeof(cbuf));
			ircvlen=GetCommData(cbuf,m_TimeOutV);
			for(iloop=0;iloop<ircvlen;iloop++){
				if(cbuf[iloop]=='I'&&cbuf[iloop+1]=='C') 
				{
					hasReadCard=ICConnectCard();
					break;
				}
			}
			if(hasReadCard > 0) return hasReadCard;
		}
		else{
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1b\x25\x42",3,&dwBytesWritten, NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1bN",2,&dwBytesWritten, NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1b\x42\x34",3,&dwBytesWritten, NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1b\x42]",3,&dwBytesWritten, NULL);
			memset(cbuf,0x00,sizeof(cbuf));
			ircvlen=GetCommData(cbuf,m_TimeOutV);
		}		
		if(ircvlen>0) {
			for(iloop=0;(iloop<ircvlen)&&(cbuf[iloop]!='s');iloop++);
			iloop++;
			ilen=0;
			for(;(iloop<ircvlen)&&(cbuf[iloop]!='A');iloop++) {
					Track2Data[ilen]=cbuf[iloop];
					ilen++; }
			Track2Data[ilen]='\0';
			iloop++;
			ilen=0;
			for(;(iloop<ircvlen)&&(cbuf[iloop]!='?');iloop++) {
					Track3Data[ilen]=cbuf[iloop];
					ilen++; }
			Track3Data[ilen]='\0';
			if(Track2Data[0]==0x7f&&Track3Data[0]==0x7f) { ret=-81;hasReadCard= ret; goto Exit;}
		}
		else {
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1B\x30",2,&dwBytesWritten,NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1B%K",3,&dwBytesWritten,NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1B\x52",2,&dwBytesWritten,NULL);
			ret=-80;
			FReturnVal = -80;
			hasReadCard= ret;
			goto Exit;
		}
Exit:
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);
		hasReadCard= ret;
	}
	else ret=-83;
	FReturnVal = ret;
	hasReadCard= ret;
	if (ret < 0 ) CloseComm();
	///获得卡号
	memset(cardno,0x00,sizeof(cardno));
	for (I_Ri=0; I_Ri<20; I_Ri++)
	{
		if(Track2Data[I_Ri] == 0x27 || Track2Data[I_Ri] == 0x3D)
		break;
		cardno[I_Ri] = Track2Data[I_Ri];
	}
	cardno[I_Ri+1] = '\0';
    ///////////
    return(ret);
}


int ReadCardDlg::ReadPassWord()
{
	long ret;
	char cbuf[300],cmpstr[128],temp[128],keydata[128];
	unsigned int i,ilen;
	DWORD dwBytesWritten=0;
	char MKey,NowKey[64],CryptKey[64],CryptedKey[64];
//	unsigned int i;
	char encPass[17];
	char m_SendPacket[2048],m_RecvPacket[512];
    char CPackRand[7];
	CString cm_SendPacket;

	memset(CPackRand,0x00,sizeof(CPackRand));
	memset(m_SendPacket,0x00,sizeof(m_SendPacket));
	memset(m_RecvPacket,0x00,sizeof(m_RecvPacket));
	memset(encPass,0x00,sizeof(encPass));

    ret =0;

	if(1 == BpKeyType){
		WriteDiary("使用旧键盘中......","Diary.log");
		//定义32组管理密钥
			unsigned char MasterKey[32][8]={	0x04,0x34,0xC4,0x04,0xC4,0x3C,0x20,0x10, 
						0x0F,0xE8,0x08,0x08,0x28,0x18,0x00,0x00,
						0x10,0x08,0x06,0x01,0x82,0x8C,0x40,0x30, 
						0x0C,0x03,0x0C,0x10,0x60,0xC0,0x40,0x00, 
		//	Code= 1h,
						0x40,0x42,0x44,0xC8,0x00,0xFC,0x04,0x02, 
						0x82,0xFC,0x04,0x04,0x04,0xFE,0x04,0x00, 
						0x00,0x40,0x20,0x1F,0x20,0x47,0x42,0x41, 
						0x40,0x7F,0x40,0x42,0x44,0x63,0x20,0x00, 
		//	Code= 2h,
						0x40,0x40,0x42,0x44,0x58,0xC0,0x40,0x7F, 
						0x40,0xC0,0x50,0x48,0x46,0x64,0x40,0x00, 
						0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x00, 
						0x00,0x3F,0x40,0x40,0x40,0x40,0x70,0x00, 
		//      Code= 3h,
						0x00,0xFC,0x00,0xFF,0x40,0x20,0x10,0x0C, 
						0x2B,0x48,0xC8,0x08,0x08,0x8C,0x08,0x00, 
						0x00,0x1F,0x00,0xFF,0x00,0xFF,0x41,0x41, 
						0x41,0x7F,0x41,0x41,0x41,0xFF,0x01,0x00, 
		//      Code= 4h,
						0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0xFF, 
						0x08,0x08,0x08,0x08,0x08,0xFC,0x08,0x00, 
						0x00,0x03,0x01,0x01,0x01,0x01,0x01,0xFF, 
						0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00, 
		//      Code= 5h,
						0x00,0xFE,0x02,0x0A,0x8A,0x8A,0x8A,0xFA, 
						0x8A,0xCA,0x8E,0x0A,0x02,0xFF,0x02,0x00, 
						0x00,0xFF,0x40,0x50,0x50,0x50,0x50,0x5F, 
						0x50,0x52,0x54,0x50,0x40,0xFF,0x00,0x00, 
		//      Code= 6h,
						0x80,0x40,0x78,0xCF,0x48,0x48,0x00,0xFE, 
						0x92,0x92,0x92,0x92,0xFF,0x02,0x00,0x00, 
						0x02,0x02,0x02,0x7F,0x22,0x12,0x08,0xFF, 
						0x42,0x24,0x08,0x14,0x22,0x63,0x20,0x00, 
		//      Code= 7h,
						0x00,0x10,0x88,0xC4,0x23,0x40,0x42,0x42, 
						0x42,0x42,0x42,0xC2,0x43,0x62,0x40,0x00, 
						0x02,0x01,0x00,0xFF,0x00,0x00,0x00,0x00, 
						0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00, 
		//	Code= 8h,

			};

	//密码键盘输入操作
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1b\x25K",3,&dwBytesWritten, NULL);
			MKey=(char) (rand()%32);   //随机从32组管理密钥中选取一组作为当前管理密钥
			for(i=0;i<8;++i)
				NowKey[i]=MasterKey[MKey][i]; 
			NowKey[i]=0x00;
			for(i=0;i<8;++i)			//随机取8字节的工作密钥
				CryptKey[i]=rand()%256;
			CryptKey[i]=0x00;
			DesData(CryptKey,8,CryptedKey,NowKey); //使用当前管理密钥加密工作密钥得到加密的工作密钥
			ilen=ModifyUserKeyM(hCommDev,MKey,0x01,CryptedKey,10); //加密下载工作密钥
			if(ilen==1) {
				strcpy(cbuf,"12345678");	
				ilen=CryptDataKey(hCommDev,MKey,0x01,0x08,cbuf,temp,10); //校验下载的工作密钥是否正确
				if(ilen>=0) {
					DesData(cbuf,8,cmpstr,CryptKey);
					if(strcmp(cmpstr,temp)) {
						ret=-82;		//校验工作密码下载失败返回-82
						FReturnVal=-82;
						goto Exit; }
					ilen=DesInput(hCommDev,0,temp,m_TimeOutV); //校验工作密钥成功后驱动密码键盘加密输入密码
					if(ilen>=0) {
						UnDesData(temp,keydata,CryptKey); //输入密码成功后与原系统兼容解密等到客户密码的明文
						char EncCardNo[21];
						memset(EncCardNo,0x00,sizeof(EncCardNo));
						memcpy(EncCardNo,"00000000000000000000",20);
						genUsrPin( EncCardNo,keydata,encPass);
						//MessageBox(encPass,NULL,MB_OK);					
						ret=0; 
					}
					else {
						strcpy(keydata,"");
						ret=-82;
						FReturnVal =-82;
					}
				}
				else ret=-82;
			}
			else ret=-82;
	}
	else if(BpKeyType > 1){
// 		WriteDiary("使用新键盘中......","Diary.log");
/*		char strTmp[20]={0};
		char sCardNo[30],sId[30],sPinLen[10],sCheck[100];
		char sCardNo1[30];
		CString strcardNOTemp="00000000000000000000";

//		strcardNOTemp = cardno;
//		strcardNOTemp.Replace(" ","");


		memset(sCardNo,0x00,sizeof(sCardNo));
		memset(sCardNo1,0x00,sizeof(sCardNo1));

		memset(sId,0x00,sizeof(sId));

		memset(sPinLen,0x00,sizeof(sPinLen));

		memset(sCheck,0x00,sizeof(sCheck));
		
		memcpy(sCardNo,strcardNOTemp,strcardNOTemp.GetLength());

		memcpy(sCardNo1,&sCardNo[strlen(sCardNo)-13],12);//倒数第二位开始，往前12位，卡号最后一位为校验位
		
	//	::AfxMessageBox(sCardNo1,0x00,sizeof(sCardNo1));
		char CommPort[8] = "COM";
		char port[3];
		char cKeyPadID[30];
		memset(cKeyPadID,0x00,sizeof(cKeyPadID));
		memset(port,0x00,sizeof(port));

		if(!m_Comport)
		{
			ret=GetComport();
			hasReadCard = ret;
			if(ret<0)
			{ 
				FReturnVal = -83;
				return(-83);
			}
		}
		sprintf(port,"%d",m_Comport);
		strcat(CommPort,port);
		
		short nret=SDBReadCustomerPin(CommPort,"","001",sCardNo1,"0",60,cKeyPadID,sPinLen,encPass,sCheck);

		WriteDiary(cKeyPadID,"Diary.log");
	//	WriteDiary(encPass,"Diary.log");
*/
		char CommPort[8] = "\0";
		char TMKIndex[4] = "\0";
		char ExtPort[8] = "\0";
		char accNo[17] = "\0";
		char modex[2] = "\0";

		char idnum[32] = "\0";
		char pinLength[8] = "\0";
		char TPKCheck[32] = "\0";
		char port[2] = "\0";
		int timeout = 30;

		strcpy(CommPort,"COM");
		sprintf(port,"%d",m_Comport);
		strcat(CommPort,port);
		strcpy(ExtPort,"");
		strcpy(TMKIndex,"001");
//		LKFormatAnsiX98Account(cardno,accNo);
//		strcpy(accNo,"400002051279");
		strcpy(accNo,"000000000000");
		strcpy(modex,"0");


		OpenKeyComm(port,9600);
		//////////////////

		///////////////////
		ret = SDBReadCustomerPin(CommPort,ExtPort,TMKIndex,accNo,modex,timeout,idnum,pinLength,encPass,TPKCheck);
		CloseKeyComm();
//		WriteDiary(idnum,"Diary.log");
	}
		//数据打包
		if (ret == 0 )
		{
			
			m_compasswordctl.SetWindowText("******");
			m_compasswordctl.RedrawWindow();
			m_comsprompt.SetWindowText("  读密码成功!   交易发送中......");
			
			WriteDiary("用户输入密码完毕！","Diary.log");
			char payMode[5];
			memset(payMode,0x00,sizeof(payMode));
			memcpy(payMode,"0000",4);

			char MXData[1024]="\0";
			char MXTemp[100]="\0";
			for(i = 0;i < 10; i++)
			{
				memset(MXTemp,0x00,sizeof(MXTemp));
				sprintf(MXTemp,"%-20s%-20s%-16s%-16s%-16s%",m_FeeItemNo[i],m_FeeItemKind[i],m_FeeLevel[i],m_FeeItemCount[i],m_FeeItemAmt[i]);
				strcat(MXData,MXTemp);
				//WriteDiary("执收码：" + CString(MXTemp),"Diary.log");
			}
			
			//test 磁道3信息读不到
			//strcpy(Track3Data, "996210985840012721715=1561560000000000000003000000114141400001=000000000000=000000000000=000000054106000");
			//WriteDiary("全部执收码：" + CString(MXData),"Diary.log");
			//交易方式 卡号 密码 2磁道信息 3磁道信息 操作员号 行政区划码 单位编码 开票日期 票据种类编码 票据号码 付款人姓名 票据合计金额
			//执收码1 计量单位1 收费标准1 数量1 单条金额1 ... 执收码10 计量单位10 收费标准10 数量10 单条金额10
			cm_SendPacket.Format("%-4s%-20s%-20s%-40s%-110s%-20s%-8s%-6s%-20s%-10s%-3s%-10s%-50s%-20s%-880s",
				payMode  ,    cardno,       encPass,	Track2Data,   Track3Data,  cm_TermNO, cm_MakeUserGuId,
				cm_AdminNO,		cm_UnitNO,	cm_VouDate,    cm_VouKind,   cm_VouNO,	cm_PayerName,	cm_VouFee,	MXData
				);
			/////////////////////////////////////////
			//WriteDiary("发送数据包：" + CString(cm_SendPacket),"Diary.log");
			}			
Exit:
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);
		EndWaitCursor();
		CloseComm();
		/////
		if(ret==0)
		{
/*			if (strlen(keydata)!=4 && strlen(keydata) != 6)
			{
				memcpy(m_OutPacket,"287|ErrPassLen|",15);
				FReturnVal =0;
				memcpy(pReturnV,"287",3);
				WriteDiary("密码长度不正确！退出交易！","Diary.log");
				WriteDiary("返回数据包：" + CString(m_OutPacket),"Diary.log");

				return (0);
			}
*/
			memcpy(m_SendPacket,cm_SendPacket,strlen(cm_SendPacket));
            WriteDiary("发送数据包：" + CString(m_SendPacket),"Diary.log");

			unsigned int inSendLen;
			inSendLen = strlen(m_SendPacket);
		    ret = SendToSDB(m_SendPacket,m_RecvPacket,30,m_IP2,m_Port2,inSendLen);
			WriteDiary("返回数据包：" + CString(m_RecvPacket) ,"Diary.log");
			//ret = 0;
			//strcpy(m_RecvPacket,"0000123456789      140221000123452014022112324462251234123412341239876543210011016              ");
			if (ret == 0)
			{   
				int iPassLen,iRandNum;
				iPassLen = 6;//strlen(keydata);
				iRandNum = atoi(CPackRand);
				char iPayName[500];
				memset(iPayName,0x00,sizeof(iPayName));
				memcpy(iPayName,m_inPayNO,strlen(m_inPayNO));
//				MessageBox(m_RecvPacket,NULL,MB_OK);
				ret = AnalyzeP(m_RecvPacket,m_OutPacket,iPayName,iPassLen,iRandNum);
			}
		}
		else
		{
			BOOL flag;
			char CommPort[8] = "\0";
			char port[2] = "\0";

			strcpy(CommPort,"COM");
			sprintf(port,"%d",m_Comport);
			strcat(CommPort,port);
			flag = OpenKeyComm(CommPort,9600);
			if( 0 == flag )
				WriteDiary("打开串口成功，取消输入密码！","Diary.log");
			else
				WriteDiary("打开串口不成功","Diary.log");
			DWORD dwBytesWritten=0;
			WriteFile(hCommDev,"\x1B\x25\x4B",3,&dwBytesWritten, NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1B\x30\x0D\x0A",4,&dwBytesWritten, NULL);
			dwBytesWritten=0;
			WriteFile(hCommDev,"\x1B\x25\x41",3,&dwBytesWritten, NULL);
			CloseKeyComm();
 		   m_comsprompt.SetWindowText("读密码键盘出错!");
		   WriteDiary("读密码键盘出错!","Diary.log");
		}
		/////
//	m_Status=ret;
    FReturnVal = ret;
	return(ret);		
}

int bOpen_Flag=0;
int bCCICOpen_Flag=0;
int bHIDOpen_Flag=0;
int ReadCardDlg::ICConnectCard()
{
	int ret =0;
	int lreturn;
	WriteDiary("获取接触式IC卡数据!","Diary.log");
	int i;
	CString Info="";
	CString tepInfo="";
	char CardData[20];
	char CardLen[4];
	char TrackData[128];
	char TrackLen[4];
	int  InfoLen=0;
	char Money[20]="\0";

	int USBFlag=0;
	char TagList[12]={'A','B','C','D','E','F','G','H','I','J','K','L'};
	char AIDList[128]="A0000003330101";
    char UserInfo[512]="\0";
	char ICInfo[512]="\0";

	char card_seq_id[4]="\0";//卡序列号
	char app_crypto[17]="\0";//应用密文
	char crypto_info_data[3]="\0";//密文信息数据
	char issr_app_data[65]="\0";//发卡行应用数据
	char unpredic_num[9]="1EAA228E";//不可预知数
	char app_trans_count[5]="\0";//应用交易计数器
	char termnl_veri_resl[11]="0000800000";//终端验证结果
	char trans_date[7]="\0";//交易日期
	char trans_type[3]="\0";//交易类型
	char trans_amt[13]="\0";//授权金额
	char app_interch_profl[5]="\0";//应用交互特征
	char InputInfo[512]="\0";
	char FeeAmt[13],SendFeeAmt[13];
	unsigned int iAmtLen;
		
	iAmtLen = 12;
	memset(FeeAmt,0x00,sizeof(FeeAmt));
	memset(SendFeeAmt,0x00,sizeof(SendFeeAmt));
    memcpy(FeeAmt,CPayAmount,strlen(CPayAmount));
	int j =0;
	for(i = 0;i< strlen(FeeAmt);i++)
	{
        if(FeeAmt[i] != '.' && FeeAmt[i] != ' ')
		{
			SendFeeAmt[j] = FeeAmt[i];
			j++;
			}
		}
		memset(FeeAmt,0x00,sizeof(FeeAmt));
		if (strlen(SendFeeAmt) < iAmtLen)
		{
			for(i=0;i< iAmtLen - strlen(SendFeeAmt);i++)
			FeeAmt[i] ='0';
		}
    memcpy(FeeAmt + iAmtLen - strlen(SendFeeAmt),SendFeeAmt,strlen(SendFeeAmt));
	char OtherAmt[13]="000000000000";
	char TermCountry[5]="0156";
	char BillCountry[5]="0156";
	
	char TranDate[7]="\0";
	char TranTime[7]="\0";
	CTime time;
	time=time.GetCurrentTime();
	sprintf(TranDate,"%s",time.Format("%y%m%d"));
	sprintf(TranTime,"%s",time.Format("%H%M%S"));

	strcpy(trans_type,"00");
	char Merchant[41]="0000000000000000000000000000000000000000";  
	sprintf(InputInfo,"%12s%12s%4s%10s%4s%6s%2s%8s%6s%40s", &
		FeeAmt,      OtherAmt,     TermCountry,  termnl_veri_resl, &
		BillCountry, TranDate,     trans_type,   unpredic_num, &
		TranTime,    Merchant);
	lreturn = ICRT_GetICCInfo(USBFlag,TagList,AIDList, UserInfo, ICInfo,InputInfo);
	if(lreturn==0) 
	{  	
		for(i=0;i<strlen(UserInfo);i++)
		{
			if(UserInfo[i]=='A')
			{  
				memset(CardData,0,sizeof(CardData));
				memcpy(CardLen,&UserInfo[i+1],3);
				InfoLen=atoi(CardLen);
				memcpy(CardData,&UserInfo[i+4],InfoLen);
				CardData[InfoLen]='\0';
			}
			if(UserInfo[i]=='K')
			{
				memset(TrackData,0,sizeof(TrackData));
				memcpy(TrackLen,&UserInfo[i+1],3);
				InfoLen=atoi(TrackLen);
				if(InfoLen!=0) memcpy(TrackData,&UserInfo[i+4],InfoLen);
				TrackData[InfoLen]='\0';
			}
		}
		strcpy(cardno,CardData);
		if (strlen(TrackData) > 37)//如果2磁道信息长度大于37，去掉后面的'F'
		{
			for (i=0; i<37; i++)
			{
				Track2Data[i] = TrackData[i];
			}
			Track2Data[i] = '\0';
		}
		else
		{
			strcpy(Track2Data,TrackData);
		}

		hasReadCard = 1;
	}
	else
	{
		int iRet;
		hasReadCard = -1;
		iRet=CloseComm();
		if(iRet==SUCC) bOpen_Flag=0;
	}
	return hasReadCard;
}
