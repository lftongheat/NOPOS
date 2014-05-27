// CSDBPackagePayDlgCtl.cpp : Implementation of the CCSDBPackagePayDlgCtrl ActiveX Control class.

#include "stdafx.h"
#include "CSDBPackagePayDlg.h"
#include "CSDBPackagePayDlgCtl.h"
#include "CSDBPackagePayDlgPpg.h"
#include "ReadCardDlg.h"
#include "enumser.h"
#include <winsock.h>
#include "afxtempl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCSDBPackagePayDlgCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCSDBPackagePayDlgCtrl, COleControl)
	//{{AFX_MSG_MAP(CCSDBPackagePayDlgCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCSDBPackagePayDlgCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCSDBPackagePayDlgCtrl)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inPnCount", m_inPnCount, OnInPnCountChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inReceivableAmt", m_inReceivableAmt, OnInReceivableAmtChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inUnitNO", m_inUnitNO, OnInUnitNOChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "outDataPackage", m_outDataPackage, OnOutDataPackageChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inTimeOut", m_TimeOut, OnInTimeOutChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inPnNO", m_inPnNO, OnInPnNOChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "OutStatus", m_Status, OnOutStatusChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "OutAccount", m_outAccount, OnOutAccountChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "InPayID", m_inPayID, OnInPayIDChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "InPayName", m_inPayName, OnInPayNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemNo1", m_inFeeItemNo1, OnInFeeItemNo1Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemFee1", m_inFeeItemFee1, OnInFeeItemFee1Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemNo2", m_inFeeItemNo2, OnInFeeItemNo2Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemFee2", m_inFeeItemFee2, OnInFeeItemFee2Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemFee3", m_inFeeItemFee3, OnInFeeItemFee3Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemNo3", m_inFeeItemNo3, OnInFeeItemNo3Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemName1", m_inFeeItemName1, OnInFeeItemName1Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemName2", m_inFeeItemName2, OnInFeeItemName2Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemName3", m_inFeeItemName3, OnInFeeItemName3Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inLateFee", m_inLateFee, OnInLateFeeChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inBigFee", m_inBigFee, OnInBigFeeChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "InTranDate", m_inTranDate, OnInTranDateChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inCardNO", m_inCardNO, OnInCardNOChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inWaterNo", m_inWaterNo, OnInWaterNoChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inUnitName", m_inUnitName, OnInUnitNameChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inPrtVou", m_inPrtVou, OnInPrtVouChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inPrtMode", m_inPrtMode, OnInPrtModeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "SFile", m_SFile, OnSFileChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "DFile", m_DFile, OnDFileChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "czReason", m_czReason, OnCzReasonChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inDataPackage", m_inDataPackage, OnInDataPackageChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inVouNO", m_inVouNO, OnInVouNOChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inVouType", m_inVouType, OnInVouTypeChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inInvaildNoReason", m_inInvaildNoReason, OnInInvaildNoReasonChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemFee4", m_inFeeItemFee4, OnInFeeItemFee4Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemFee5", m_inFeeItemFee5, OnInFeeItemFee5Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemFee6", m_inFeeItemFee6, OnInFeeItemFee6Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemName4", m_inFeeItemName4, OnInFeeItemName4Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemName5", m_inFeeItemName5, OnInFeeItemName5Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemName6", m_inFeeItemName6, OnInFeeItemName6Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemNo4", m_inFeeItemNo4, OnInFeeItemNo4Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemNo5", m_inFeeItemNo5, OnInFeeItemNo5Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inFeeItemNo6", m_inFeeItemNo6, OnInFeeItemNo6Changed, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "intzdType", m_intzdType, OnIntzdTypeChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inBatP", m_inBatP, OnInBatPChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "outBatP", m_outBatP, OnOutBatPChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "outFSCode", m_outFSCode, OnOutFSCodeChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inMerchantNO", m_inMerchantNO, OnInMerchantNOChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inProfitNO", m_inProfitNO, OnInProfitNOChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CCSDBPackagePayDlgCtrl, "inAddForpeiFee", m_inAddForpeiFee, OnInAddForpeiFeeChanged, VT_BSTR)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "PrtVou", PrtVou, VT_I4, VTS_NONE)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "PPrtVou", PPrtVou, VT_I4, VTS_NONE)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "PRTNORMAL", PrtNormal, VT_I4, VTS_NONE)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "PrtForfeit", PrtForfeit, VT_I4, VTS_NONE)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "DoLogon", DoLogon, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "DoSale", DoSale, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "DoRefund", DoRefund, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "DoReprint", DoReprint, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CCSDBPackagePayDlgCtrl, "GetIniParam", GetIniParam, VT_BSTR, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CCSDBPackagePayDlgCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCSDBPackagePayDlgCtrl, COleControl)
	//{{AFX_EVENT_MAP(CCSDBPackagePayDlgCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCSDBPackagePayDlgCtrl, 1)
	PROPPAGEID(CCSDBPackagePayDlgPropPage::guid)
END_PROPPAGEIDS(CCSDBPackagePayDlgCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCSDBPackagePayDlgCtrl, "CSDBPACKAGEPAYDLG.CSDBPackagePayDlgCtrl.1",
	0xcaeb78ef, 0xad75, 0x4a32, 0xb7, 0xfb, 0xe1, 0xc1, 0x13, 0x63, 0x5b, 0xcd)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCSDBPackagePayDlgCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCSDBPackagePayDlg =
		{ 0x5ec478d8, 0x4511, 0x4e16, { 0x8c, 0x80, 0xaa, 0x85, 0x1, 0x93, 0x29, 0x21 } };
const IID BASED_CODE IID_DCSDBPackagePayDlgEvents =
		{ 0xef135380, 0x7d5f, 0x4788, { 0xb1, 0xaa, 0xd2, 0x98, 0xbd, 0xb5, 0x1e, 0x5a } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCSDBPackagePayDlgOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCSDBPackagePayDlgCtrl, IDS_CSDBPACKAGEPAYDLG, _dwCSDBPackagePayDlgOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl::CCSDBPackagePayDlgCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCSDBPackagePayDlgCtrl

BOOL CCSDBPackagePayDlgCtrl::CCSDBPackagePayDlgCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CSDBPACKAGEPAYDLG,
			IDB_CSDBPACKAGEPAYDLG,
			afxRegApartmentThreading,
			_dwCSDBPackagePayDlgOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl::CCSDBPackagePayDlgCtrl - Constructor

CCSDBPackagePayDlgCtrl::CCSDBPackagePayDlgCtrl()
{
	InitializeIIDs(&IID_DCSDBPackagePayDlg, &IID_DCSDBPackagePayDlgEvents); 

	    memset(m_IP1, 0x00,sizeof(m_IP1));       //��ѯ�������������˿�
		memset(m_Port1,0x00,sizeof(m_Port1));

		memset(m_IP2, 0x00,sizeof(m_IP2));       //ˢ���������������˿�
		memset(m_Port2,0x00,sizeof(m_Port2));

	    GetPrivateProfileString("SOCKET","QueryIP","1",
							m_IP1,30,"C:\\SDBMISPOS\\DLL\\SDBServer.ini");

	    GetPrivateProfileString("SOCKET","CardIP","1",
							m_IP2,30,"C:\\SDBMISPOS\\DLL\\SDBServer.ini");


        GetPrivateProfileString("SOCKET","QueryPORT","1",
								m_Port1,8,"C:\\SDBMISPOS\\DLL\\SDBServer.ini");

		GetPrivateProfileString("SOCKET","CardPORT","1",
							    m_Port2,8,"C:\\SDBMISPOS\\DLL\\SDBServer.ini");
  
// 		char ip[20] = "10.88.19.69";
// 		memcpy(m_IP2, ip, sizeof(ip));
// 		memcpy(m_Port2, "16001", 5);		

		memset(prtWaterNo,0x00,sizeof(prtWaterNo));
		memset(pReturnV,0x00,sizeof(pReturnV));

		prtFlag =0;  //Ĭ�����ã�ͨ�����뿨�š���ˮ��ӡƱ�ݣ�5��������ÿ��š���ˮ��
		memset(m_MerchantNO, 0x00,sizeof(m_MerchantNO)); 
		memcpy(m_MerchantNO, "307333082110001", 15);
	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl::~CCSDBPackagePayDlgCtrl - Destructor

CCSDBPackagePayDlgCtrl::~CCSDBPackagePayDlgCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl::OnDraw - Drawing function

void CCSDBPackagePayDlgCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl::DoPropExchange - Persistence support

void CCSDBPackagePayDlgCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl::OnResetState - Reset control to default state

void CCSDBPackagePayDlgCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl::AboutBox - Display an "About" box to the user

void CCSDBPackagePayDlgCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_CSDBPACKAGEPAYDLG);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl message handlers
void CCSDBPackagePayDlgCtrl::OnInPnCountChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}


void CCSDBPackagePayDlgCtrl::OnInReceivableAmtChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInUnitNOChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnOutDataPackageChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}


void CCSDBPackagePayDlgCtrl::OnInTimeOutChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInPnNOChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnOutStatusChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnOutAccountChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInPayIDChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInPayNameChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}



void CCSDBPackagePayDlgCtrl::OnInFeeItemNo1Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemFee1Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemNo2Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemFee2Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemFee3Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemNo3Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

long CCSDBPackagePayDlgCtrl::PrtVou() 
{
	// TODO: Add your dispatch handler code here
	/////
	DWORD dwBytesWritten=0;
	int  ret,i;
	int  itemNum,superLongitemNum;
	char prtCardNo[21];
	//CString  FormatItemName1,FormatItemName2,FormatItemName3;
	CString  FormatItemFee1 ,FormatItemFee2 ,FormatItemFee3;
	memset(prtCardNo,0x00,sizeof(prtCardNo));
	if ( prtFlag == 5)
	   memcpy(prtCardNo,cardno,strlen(cardno));
	else
	   strcpy(prtCardNo,m_inCardNO);
    for(i=6;i<12;i++)
		prtCardNo[i] ='*';
	itemNum = 0;
	superLongitemNum =0;
    ret =0;
	
	if(!m_Comport)
	{
		ret=GetComport();
        hasReadCard = ret;
		if(ret<0)
		{ 
			FReturnVal = -83;
			m_Status = FReturnVal;
			return(FReturnVal);
		}
	}
	if(OpenComm(m_Comport,9600))
	{
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1b\x55\x00",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1b\x40",2,&dwBytesWritten, NULL); 
	    WriteFile(hCommDev,"\x1d\x28\x46\x04\x00\x02\x00\x63\x00",9,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1b\x64\x04",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"          ",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inUnitNO,strlen(m_inUnitNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev," ",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"Ʊ�ݺ�:",7,&dwBytesWritten, NULL);
        WriteFile(hCommDev,m_inPrtVou,strlen(m_inPrtVou),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		
        WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"          ",10,&dwBytesWritten, NULL);
		if( prtFlag == 5)
		    WriteFile(hCommDev,prtWaterNo,strlen(prtWaterNo),&dwBytesWritten, NULL);
		else
			WriteFile(hCommDev,m_inWaterNo,strlen(m_inWaterNo),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
   
        WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"          ",10,&dwBytesWritten, NULL);
		//if( prtFlag == 5)
		WriteFile(hCommDev,prtCardNo,strlen(prtCardNo),&dwBytesWritten, NULL);
		//else
        //  WriteFile(hCommDev,m_inCardNO,strlen(m_inCardNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

        WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"          ",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inTranDate,strlen(m_inTranDate),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
   
		WriteFile(hCommDev,"\x1B\x4A\x05",3,&dwBytesWritten, NULL);
        //WriteFile(hCommDev,"\x1C\x21\x10",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"               ",15,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inUnitName,strlen(m_inUnitName),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"               ",15,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inPnNO,strlen(m_inPnNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x09",1,&dwBytesWritten, NULL);

		
		
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x09",1,&dwBytesWritten, NULL);

        //WriteFile(hCommDev,"    ",4,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1B\x4A\x05",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inPayName,strlen(m_inPayName),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x09",1,&dwBytesWritten, NULL);

	    if (atof(m_inFeeItemFee1) >0) itemNum ++;
		if (atof(m_inFeeItemFee2) >0) itemNum ++;
		if (atof(m_inFeeItemFee3) >0) itemNum ++;

	//	if (itemNum==1)
	//	   WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);


		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);		
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x1B\x4A\x0A",3,&dwBytesWritten, NULL);
		m_inFeeItemFee1.Replace(" ","");
		if (atof(m_inFeeItemFee1) >0) 
		{
		    WriteFile(hCommDev,"\x1B\x44\x14\x38\x00",5,&dwBytesWritten, NULL);
			if ( strlen(m_inFeeItemName1)>38 && strlen(m_inFeeItemName1)<= 50 ) 
                m_inFeeItemName1.Format("%-55s",m_inFeeItemName1);
			if ( strlen(m_inFeeItemName1)<39)
                m_inFeeItemName1.Format("%-38s",m_inFeeItemName1);
		    WriteFile(hCommDev,m_inFeeItemName1,strlen(m_inFeeItemName1),&dwBytesWritten, NULL);
		    WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
			FormatItemFee1.Format("%8s",m_inFeeItemFee1);
		    WriteFile(hCommDev,FormatItemFee1,strlen(FormatItemFee1),&dwBytesWritten, NULL);
			if ( strlen(m_inFeeItemName1)>=50 ) 
				superLongitemNum ++;
		}
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		m_inFeeItemFee2.Replace(" ","");
        if (atof(m_inFeeItemFee2) >0)
		{
		   WriteFile(hCommDev,"\x1B\x44\x14\x38\x00",5,&dwBytesWritten, NULL);
		   if ( strlen(m_inFeeItemName2)>38 && strlen(m_inFeeItemName2)<= 50 ) 
               m_inFeeItemName2.Format("%-55s",m_inFeeItemName2);
		   if ( strlen(m_inFeeItemName2)<39)
               m_inFeeItemName2.Format("%-38s",m_inFeeItemName2);
		   WriteFile(hCommDev,m_inFeeItemName2,strlen(m_inFeeItemName2),&dwBytesWritten, NULL);
		   WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
		   FormatItemFee2.Format("%8s",m_inFeeItemFee2);
		   WriteFile(hCommDev,FormatItemFee2,strlen(FormatItemFee2),&dwBytesWritten, NULL);
		   if (strlen(m_inFeeItemName2)>=50) 
				superLongitemNum ++;
		}
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		m_inFeeItemFee3.Replace(" ","");
        if (atof(m_inFeeItemFee3) >0)
		{
		   	   if ( strlen(m_inFeeItemName3)>38 && strlen(m_inFeeItemName3)<= 50 ) 
               m_inFeeItemName3.Format("%-55s",m_inFeeItemName3);
		   if ( strlen(m_inFeeItemName3)<39)
               m_inFeeItemName3.Format("%-38s",m_inFeeItemName3);
           WriteFile(hCommDev,"\x1B\x44\x14\x38\x00",5,&dwBytesWritten, NULL);
		   WriteFile(hCommDev,m_inFeeItemName3,strlen(m_inFeeItemName3),&dwBytesWritten, NULL);
		   WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
		   FormatItemFee3.Format("%8s",m_inFeeItemFee3);
		   WriteFile(hCommDev,FormatItemFee3,strlen(FormatItemFee3),&dwBytesWritten, NULL);
		   if (strlen(m_inFeeItemName3)>=50) 
				superLongitemNum ++;
		}
        if (superLongitemNum ==0)
		{
			WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
			WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
			WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        }
		if (superLongitemNum ==1)
		{
			WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
			WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        }
		if (superLongitemNum ==2)
		{
			WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        }
        if (strlen(m_inPayName) < 50)
	       WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);	
	
		if (itemNum == 1)
           WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
        if (itemNum == 2)
           WriteFile(hCommDev,"\x1B\x4A\x0A",3,&dwBytesWritten, NULL);
		if (itemNum == 3)
           WriteFile(hCommDev,"\x1B\x4A\x08",3,&dwBytesWritten, NULL);
		m_inReceivableAmt.Replace(" ","");
		WriteFile(hCommDev,"              ",14,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"��",2,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inReceivableAmt,strlen(m_inReceivableAmt),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"\x1C\x21\x00",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"              ",14,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inBigFee,strlen(m_inBigFee),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"   ",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1d\x56\x00",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);
		CloseComm();
	}
	/////
	return 0;
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemName1Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemName2Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemName3Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInLateFeeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInBigFeeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInTranDateChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInCardNOChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInWaterNoChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInUnitNameChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

long CCSDBPackagePayDlgCtrl::PPrtVou() 
{
	// TODO: Add your dispatch handler code here
	
	/////
	DWORD dwBytesWritten=0;

	int  ret,i;
	int  itemNum,superLongitemNum;
	char prtCardNo[21];

	CString  FormatItemFee1 ,FormatItemFee2 ,FormatItemFee3,FormatLateFee;
    WriteDiary(" ��ʼ��ӡƱ��..." ,"Diary.log");
	WriteDiary(" Ʊ�����ͣ�" + m_inVouType ,"Diary.log");
	WriteDiary(" Ʊ�ݺ��룺" + m_inPrtVou ,"Diary.log");
	memset(prtCardNo,0x00,sizeof(prtCardNo));
	if ( prtFlag == 5)
	   memcpy(prtCardNo,cardno,strlen(cardno));
	else
	   strcpy(prtCardNo,m_inCardNO);
    for(i=6;i<12;i++)
		prtCardNo[i] ='*';
	itemNum = 0;
	superLongitemNum =0;
    ret =0;
	
	if(!m_Comport)
	{
		ret=GetComport();
        hasReadCard = ret;
		if(ret<0)
		{ 
			FReturnVal = -83;
			m_Status = FReturnVal;
			return(FReturnVal);
		}
	}
	if(OpenComm(m_Comport,9600))
	{
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);

		//WriteFile(hCommDev,"\x1b\x7E\x26\x03",4,&dwBytesWritten, NULL); //����ֽ��
		WriteFile(hCommDev,"\x1b\x7E\x07\x03",4,&dwBytesWritten, NULL); //����ֽ��
	    WriteFile(hCommDev,"\x1b\x64\x03",3,&dwBytesWritten, NULL);     //��ֽ����
		WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"�������㣺",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inUnitNO,strlen(m_inUnitNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		
        WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"������ˮ��",10,&dwBytesWritten, NULL);
		if( prtFlag == 5)
		    WriteFile(hCommDev,prtWaterNo,strlen(prtWaterNo),&dwBytesWritten, NULL);
		else
			WriteFile(hCommDev,m_inWaterNo,strlen(m_inWaterNo),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
   
        WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"���׿��ţ�",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,prtCardNo,strlen(prtCardNo),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

        WriteFile(hCommDev,"\x1B\x4A\x06",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"�������ڣ�",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inTranDate,strlen(m_inTranDate),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
   
		WriteFile(hCommDev,"\x1B\x4A\x05",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"ִ�յ�λ��",12,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inUnitName,strlen(m_inUnitName),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"�ɿ�֪ͨ����",12,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inPnNO,strlen(m_inPnNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�ɿ������ƣ�",12,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"    ",4,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inPayName,strlen(m_inPayName),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�շ���Ŀ                �շѽ��",32,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"---------------------------------",33,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		m_inFeeItemFee1.Replace(" ","");
		if (atof(m_inFeeItemFee1) >0) 
		{
		    WriteFile(hCommDev,"\x1B\x44\x19\x38\x00",5,&dwBytesWritten, NULL);
			if ( strlen(m_inFeeItemName1)>27 && strlen(m_inFeeItemName1)<= 50 ) 
                m_inFeeItemName1.Format("%-55s",m_inFeeItemName1);
			if ( strlen(m_inFeeItemName1)<27)
                m_inFeeItemName1.Format("%-20s",m_inFeeItemName1);

		    WriteFile(hCommDev,m_inFeeItemName1,strlen(m_inFeeItemName1),&dwBytesWritten, NULL);
		    WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
			FormatItemFee1.Format("%8s",m_inFeeItemFee1);
		    WriteFile(hCommDev,FormatItemFee1,strlen(FormatItemFee1),&dwBytesWritten, NULL);
		}
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		m_inFeeItemFee2.Replace(" ","");
        if (atof(m_inFeeItemFee2) >0)
		{
		   WriteFile(hCommDev,"\x1B\x44\x19\x38\x00",5,&dwBytesWritten, NULL);
		   if ( strlen(m_inFeeItemName2)>28 && strlen(m_inFeeItemName2)<= 50 ) 
               m_inFeeItemName2.Format("%-55s",m_inFeeItemName2);
		   if ( strlen(m_inFeeItemName2)<27)
               m_inFeeItemName2.Format("%-20s",m_inFeeItemName2);

		   WriteFile(hCommDev,m_inFeeItemName2,strlen(m_inFeeItemName2),&dwBytesWritten, NULL);
		   WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
		   FormatItemFee2.Format("%8s",m_inFeeItemFee2);
		   WriteFile(hCommDev,FormatItemFee2,strlen(FormatItemFee2),&dwBytesWritten, NULL);
		}
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		m_inFeeItemFee3.Replace(" ","");
        if (atof(m_inFeeItemFee3) >0)
		{
		   if ( strlen(m_inFeeItemName3)>28 && strlen(m_inFeeItemName3)<= 50 ) 
               m_inFeeItemName3.Format("%-55s",m_inFeeItemName3);
		   if ( strlen(m_inFeeItemName3)<27)
               m_inFeeItemName3.Format("%-20s",m_inFeeItemName3);

           WriteFile(hCommDev,"\x1B\x44\x19\x38\x00",5,&dwBytesWritten, NULL);
		   WriteFile(hCommDev,m_inFeeItemName3,strlen(m_inFeeItemName3),&dwBytesWritten, NULL);
		   WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
		   FormatItemFee3.Format("%8s",m_inFeeItemFee3);
		   WriteFile(hCommDev,FormatItemFee3,strlen(FormatItemFee3),&dwBytesWritten, NULL);
		}
        if (atof(m_inLateFee) >0)
		{
		   CString  m_LateFeeName;
		   m_LateFeeName ="���ɽ�";
		   if ( strlen(m_LateFeeName)>28 && strlen(m_LateFeeName)<= 50 ) 
              m_LateFeeName.Format("%-55s",m_LateFeeName);
		   if ( strlen(m_LateFeeName)<27)
               m_LateFeeName.Format("%-20s",m_LateFeeName);

           WriteFile(hCommDev,"\x1B\x44\x19\x38\x00",5,&dwBytesWritten, NULL);
		   WriteFile(hCommDev,m_LateFeeName,strlen(m_LateFeeName),&dwBytesWritten, NULL);
		   WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
		   FormatLateFee.Format("%8s",m_inLateFee);
		   WriteFile(hCommDev,FormatLateFee,strlen(FormatLateFee),&dwBytesWritten, NULL);
		}
	    WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		m_inReceivableAmt.Replace(" ","");
		WriteFile(hCommDev,"�ϼƽ�",10,&dwBytesWritten, NULL);
		//WriteFile(hCommDev,"��",2,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inReceivableAmt,strlen(m_inReceivableAmt),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"�ϼƴ�д��",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inBigFee,strlen(m_inBigFee),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"---------------------------------",33,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        WriteFile(hCommDev,"�ֿ���ǩ����",12,&dwBytesWritten, NULL);
	//  WriteFile(hCommDev,"\x1d\x56\x00",3,&dwBytesWritten, NULL);  //�ڱ���ֽ
		WriteFile(hCommDev,"\x1b\x7F",2,&dwBytesWritten, NULL);      //������ֽ
	    WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);  //��ʼ��
		CloseComm();
	}
	/////
	return 0;
}

void CCSDBPackagePayDlgCtrl::OnInPrtVouChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInPrtModeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnSFileChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnDFileChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnCzReasonChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInDataPackageChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInVouNOChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInVouTypeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}



void CCSDBPackagePayDlgCtrl::OnInInvaildNoReasonChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemFee4Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemFee5Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemFee6Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemName4Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemName5Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemName6Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemNo4Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemNo5Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInFeeItemNo6Changed() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnIntzdTypeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInBatPChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnOutBatPChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnOutFSCodeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInMerchantNOChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

long CCSDBPackagePayDlgCtrl::PrtNormal() 
{
	DWORD dwBytesWritten=0;

	int  ret;
	int  itemNum,superLongitemNum;
// 	char prtCardNo[21];

    WriteDiary(" ��ʼ��ӡ����СƱ..." ,"Diary.log");
// 	WriteDiary("         ���ţ�" + p_CardNO ,"Diary.log");
// 	WriteDiary("     �������ڣ�" + p_TransDate ,"Diary.log");
// 	WriteDiary("     ����ʱ�䣺" + p_TransTime,"Diary.log");	
// 	WriteDiary("       ��ˮ�ţ�" + p_TraceNO,"Diary.log");	
// 	WriteDiary("       ���κţ�" + p_BatchNO,"Diary.log");	
// 	WriteDiary("       ��Ȩ�ţ�" + p_AuthNO ,"Diary.log");
// 	WriteDiary("       �ο��ţ�" + p_RefNO ,"Diary.log");
// 	WriteDiary("   ��Ƭ��Ч�ڣ�" + p_ValidDate ,"Diary.log");
//     WriteDiary("     ���׽�" + p_TransMoney,"Diary.log");

	
// 	memset(prtCardNo,0x00,sizeof(prtCardNo));
// 	if ( prtFlag == 5)
// 	   memcpy(prtCardNo,cardno,strlen(cardno));
// 	else
// 	   strcpy(prtCardNo,m_inCardNO);
//     for(i=6;i<12;i++)
// 		prtCardNo[i] ='*';

	itemNum = 0;
	superLongitemNum =0;
    ret =0;
	
	CTime time;
	time=CTime::GetCurrentTime();
	CString localTime=time.Format("%Y-%m-%d %H:%M:%S");

	if(!m_Comport)
	{
		ret=GetComport();
        hasReadCard = ret;
		if(ret<0)
		{ 
			FReturnVal = -83;
			m_Status = FReturnVal;
			return(FReturnVal);
		}
	}
	if(OpenComm(m_Comport,9600))
	{
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"\x1b\x7E\x07\x03",4,&dwBytesWritten, NULL); //����ֽ��
	    WriteFile(hCommDev,"\x1b\x64\x01",3,&dwBytesWritten, NULL);     //��ֽ1��
		WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"ƽ������",8,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);//����
	    WriteFile(hCommDev,"----------------------------------",34,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�̻����룺",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);//����
		WriteFile(hCommDev,"         ",9,&dwBytesWritten, NULL);//�ո�
		WriteFile(hCommDev,"307333082110001",15,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);//����

		WriteFile(hCommDev,"���׿��ţ�",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);//����
		WriteFile(hCommDev,"         ",9,&dwBytesWritten, NULL);//�ո�
		WriteFile(hCommDev,p_CardNO,strlen(p_CardNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�������ڣ�",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,p_TransDate,strlen(p_TransDate),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"����ʱ�䣺",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,p_TransTime,strlen(p_TransTime),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"��ˮ�ţ�  ",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,p_TraceNO,strlen(p_TraceNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"���κţ�  ",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,p_BatchNO,strlen(p_BatchNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"��Ȩ�ţ�  ",10,&dwBytesWritten, NULL);
// 		WriteFile(hCommDev,p_AuthNO,strlen(p_AuthNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�ο��ţ�  ",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,p_RefNO,strlen(p_RefNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"��Ƭ��Ч�ڣ�",12,&dwBytesWritten, NULL);
// 		WriteFile(hCommDev,p_ValidDate,strlen(p_ValidDate),&dwBytesWritten, NULL);
//		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"----------------------------------",34,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"һ��ɿ�����룺 ",17,&dwBytesWritten, NULL);
		WriteFile(hCommDev,cm_VouNO,strlen(cm_VouNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);//����

		WriteFile(hCommDev,"���׽�RMB����",20,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);//����
		WriteFile(hCommDev,"                ",16,&dwBytesWritten, NULL);//�ո�
		WriteFile(hCommDev,p_TransMoney,strlen(p_TransMoney),&dwBytesWritten, NULL);

		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        WriteFile(hCommDev,"----------------------------------",34,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		/*
		m_inFeeItemFee1.Replace(" ","");
		if (atof(m_inFeeItemFee1) >0) 
		{
		    WriteFile(hCommDev,"\x1B\x44\x19\x38\x00",5,&dwBytesWritten, NULL);
			if ( strlen(m_inFeeItemName1)>27 && strlen(m_inFeeItemName1)<= 50 ) 
                m_inFeeItemName1.Format("%-55s",m_inFeeItemName1);
			if ( strlen(m_inFeeItemName1)<27)
                m_inFeeItemName1.Format("%-20s",m_inFeeItemName1);

		    WriteFile(hCommDev,m_inFeeItemName1,strlen(m_inFeeItemName1),&dwBytesWritten, NULL);
		    WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
			FormatItemFee1.Format("%9s",m_inFeeItemFee1);
		    WriteFile(hCommDev,FormatItemFee1,strlen(FormatItemFee1),&dwBytesWritten, NULL);
		}
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		m_inFeeItemFee2.Replace(" ","");
        if (atof(m_inFeeItemFee2) >0)
		{
		   WriteFile(hCommDev,"\x1B\x44\x19\x38\x00",5,&dwBytesWritten, NULL);
		   if ( strlen(m_inFeeItemName2)>28 && strlen(m_inFeeItemName2)<= 50 ) 
               m_inFeeItemName2.Format("%-55s",m_inFeeItemName2);
		   if ( strlen(m_inFeeItemName2)<27)
               m_inFeeItemName2.Format("%-20s",m_inFeeItemName2);

		   WriteFile(hCommDev,m_inFeeItemName2,strlen(m_inFeeItemName2),&dwBytesWritten, NULL);
		   WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
		   FormatItemFee2.Format("%9s",m_inFeeItemFee2);
		   WriteFile(hCommDev,FormatItemFee2,strlen(FormatItemFee2),&dwBytesWritten, NULL);
		}
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		m_inFeeItemFee3.Replace(" ","");
        if (atof(m_inFeeItemFee3) >0)
		{
		   if ( strlen(m_inFeeItemName3)>28 && strlen(m_inFeeItemName3)<= 50 ) 
               m_inFeeItemName3.Format("%-55s",m_inFeeItemName3);
		   if ( strlen(m_inFeeItemName3)<27)
               m_inFeeItemName3.Format("%-20s",m_inFeeItemName3);

           WriteFile(hCommDev,"\x1B\x44\x19\x38\x00",5,&dwBytesWritten, NULL);
		   WriteFile(hCommDev,m_inFeeItemName3,strlen(m_inFeeItemName3),&dwBytesWritten, NULL);
		   WriteFile(hCommDev,"\x09",2,&dwBytesWritten, NULL);
		   FormatItemFee3.Format("%9s",m_inFeeItemFee3);
		   WriteFile(hCommDev,FormatItemFee3,strlen(FormatItemFee3),&dwBytesWritten, NULL);
		}
		*/


		/*
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        WriteFile(hCommDev,"----------------------------------",34,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		CString fmtZNJFee,fmtTotalFee;
		WriteFile(hCommDev,"���ɽ�ϼƣ�",12,&dwBytesWritten, NULL);
		fmtZNJFee.Format("%12s",m_inLateFee);
        WriteFile(hCommDev,fmtZNJFee,strlen(fmtZNJFee),&dwBytesWritten, NULL);
        WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		m_inReceivableAmt.Replace(" ","");
		WriteFile(hCommDev,"���ϼƣ�  ",12,&dwBytesWritten, NULL);
        m_inReceivableAmt = "��"+m_inReceivableAmt;
		
		fmtTotalFee.Format("%12s",m_inReceivableAmt);
		WriteDiary("���ϼƣ�" + fmtTotalFee ,"Diary.log");
		WriteFile(hCommDev,fmtTotalFee,strlen(fmtTotalFee),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"(��д�������",13,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inBigFee,strlen(m_inBigFee),&dwBytesWritten, NULL);
		WriteFile(hCommDev,")",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"----------------------------------",34,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		*/

        WriteFile(hCommDev,"�ֿ���ǩ����",12,&dwBytesWritten, NULL);
	//  WriteFile(hCommDev,"\x1d\x56\x00",3,&dwBytesWritten, NULL);  //�ڱ���ֽ
		WriteFile(hCommDev,"\x1b\x7F",2,&dwBytesWritten, NULL);      //������ֽ

	    WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);  //��ʼ��
		CloseComm();
	}


	return 0;
}

long CCSDBPackagePayDlgCtrl::PrtForfeit() 
{
	// TODO: Add your dispatch handler code here
	DWORD dwBytesWritten=0;

	int  ret,i;
	int  itemNum,superLongitemNum;
	char prtCardNo[21];

	CString  FormatItemFee1 ,FormatItemFee2 ,FormatItemFee3,FormatLateFee;
    WriteDiary(" ��ʼ��ӡ��û��˰Ʊ��..." ,"Diary.log");
	WriteDiary("     Ʊ�����ͣ�" + m_inVouType ,"Diary.log");
	WriteDiary("     Ʊ�ݺ��룺" + m_inPrtVou ,"Diary.log");
	WriteDiary("     ������룺" + m_inUnitNO,"Diary.log");	
	WriteDiary("   �ɿ��˱�ʶ��" + m_inPayID,"Diary.log");	
	WriteDiary("   �ɿ������ƣ�" + m_inPayName,"Diary.log");	
	WriteDiary("   �ɿ�֪ͨ����" + m_inPnNO ,"Diary.log");
	WriteDiary("   ���������飺" + m_inProfitNO ,"Diary.log");
	WriteDiary("   ֪ͨ�����ͣ�" + m_intzdType ,"Diary.log");
    WriteDiary("     �ӷ���" + m_inAddForpeiFee,"Diary.log");
    WriteDiary("     ���ɽ�" + m_inLateFee,"Diary.log");
	WriteDiary("Ӧ��/�����" + m_inReceivableAmt,"Diary.log");
	WriteDiary(" �ۿ����д��" + m_inBigFee,"Diary.log");
	WriteDiary("     �������ڣ�" + m_inTranDate,"Diary.log");
	WriteDiary("�շ���Ŀ����1��" + m_inFeeItemNo1,"Diary.log");
	WriteDiary("�շ���Ŀ����1��" + m_inFeeItemName1,"Diary.log");
	WriteDiary("�շ���Ŀ���1��" + m_inFeeItemFee1 ,"Diary.log");
	WriteDiary("�շ���Ŀ����2��" + m_inFeeItemNo2,"Diary.log");
	WriteDiary("�շ���Ŀ����2��" + m_inFeeItemName2,"Diary.log");
	WriteDiary("�շ���Ŀ���2��" + m_inFeeItemFee2 ,"Diary.log");
	WriteDiary("�շ���Ŀ����3��" + m_inFeeItemNo3,"Diary.log");
	WriteDiary("�շ���Ŀ����3��" + m_inFeeItemName3,"Diary.log");
	WriteDiary("�շ���Ŀ���3��" + m_inFeeItemFee3 ,"Diary.log");
	WriteDiary("�շ���Ŀ����4��" + m_inFeeItemNo4,"Diary.log");
	WriteDiary("�շ���Ŀ����4��" + m_inFeeItemName4,"Diary.log");
	WriteDiary("�շ���Ŀ���4��" + m_inFeeItemFee4 ,"Diary.log");
	WriteDiary("�շ���Ŀ����5��" + m_inFeeItemNo5,"Diary.log");
	WriteDiary("�շ���Ŀ����5��" + m_inFeeItemName5,"Diary.log");
	WriteDiary("�շ���Ŀ���5��" + m_inFeeItemFee5 ,"Diary.log");
	WriteDiary("�շ���Ŀ����6��" + m_inFeeItemNo6,"Diary.log");
	WriteDiary("�շ���Ŀ����6��" + m_inFeeItemName6,"Diary.log");
	WriteDiary("�շ���Ŀ���6��" + m_inFeeItemFee6,"Diary.log");
	WriteDiary(" ִ�յ�λ���ƣ�" + m_inUnitName ,"Diary.log");

	memset(prtCardNo,0x00,sizeof(prtCardNo));
	if ( prtFlag == 5)
	   memcpy(prtCardNo,cardno,strlen(cardno));
	else
	   strcpy(prtCardNo,m_inCardNO);
    for(i=6;i<12;i++)
		prtCardNo[i] ='*';
	itemNum = 0;
	superLongitemNum =0;
    ret =0;
	
	if(!m_Comport)
	{
		ret=GetComport();
        hasReadCard = ret;
		if(ret<0)
		{ 
			FReturnVal = -83;
			m_Status = FReturnVal;
			return(FReturnVal);
		}
	}
	if(OpenComm(m_Comport,9600))
	{
		dwBytesWritten=0;
		WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"\x1b\x7E\x07\x03",4,&dwBytesWritten, NULL); //����ֽ��
	    WriteFile(hCommDev,"\x1b\x64\x02",3,&dwBytesWritten, NULL);     //��ֽ2��
		WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�̻����ƣ�(ƽ��)",16,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inMerchantNO,strlen(m_inMerchantNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�����նˣ�",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inUnitNO,strlen(m_inUnitNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);
	
		WriteFile(hCommDev,"������ˮ��",10,&dwBytesWritten, NULL);
		if( prtFlag == 5)
		    WriteFile(hCommDev,prtWaterNo,strlen(prtWaterNo),&dwBytesWritten, NULL);
		else
			WriteFile(hCommDev,m_inWaterNo,strlen(m_inWaterNo),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�տ����ڣ�",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inTranDate,strlen(m_inTranDate),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"���׿��ţ�",10,&dwBytesWritten, NULL);
		WriteFile(hCommDev,prtCardNo,strlen(prtCardNo),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"ִ�յ�λ��",12,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inUnitName,strlen(m_inUnitName),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x1B\x4A\x02",3,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�ɿ�֪ͨ����",12,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inPnNO,strlen(m_inPnNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"�ɿλ(��)���ƣ�",18,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"    ",4,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inPayName,strlen(m_inPayName),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"����������ţ�",14,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inProfitNO,strlen(m_inProfitNO),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        WriteFile(hCommDev,"---------------------------------",33,&dwBytesWritten, NULL);
        WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);

		WriteFile(hCommDev,"��Ŀ���ƣ�������Ŀ",18,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		CString fmtFKFee,fmtJFFee,fmtZNJFee,fmtTotalFee;
		WriteFile(hCommDev,"�����    ",14,&dwBytesWritten, NULL);
		fmtFKFee.Format("%12s",m_inFeeItemFee1);
		WriteFile(hCommDev,fmtFKFee,strlen(fmtFKFee),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"���շ����",14,&dwBytesWritten, NULL);
		fmtJFFee.Format("%12s",m_inAddForpeiFee);
		WriteDiary("���շ����" + fmtJFFee ,"Diary.log");
		WriteFile(hCommDev,fmtJFFee,strlen(fmtJFFee),&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"���ɽ�ϼƣ�  ",14,&dwBytesWritten, NULL);
		fmtZNJFee.Format("%12s",m_inLateFee);
        WriteFile(hCommDev,fmtZNJFee,strlen(fmtZNJFee),&dwBytesWritten, NULL);
        WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		m_inReceivableAmt.Replace(" ","");
		WriteFile(hCommDev,"���ϼƣ�    ",14,&dwBytesWritten, NULL);
		m_inReceivableAmt = "��"+ m_inReceivableAmt;
		WriteDiary("���ϼƣ�" + m_inReceivableAmt ,"Diary.log");
		fmtTotalFee.Format("%12s",m_inReceivableAmt);
		WriteFile(hCommDev,fmtTotalFee,strlen(fmtTotalFee),&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"(��д�������",13,&dwBytesWritten, NULL);
		WriteFile(hCommDev,m_inBigFee,strlen(m_inBigFee),&dwBytesWritten, NULL);
		WriteFile(hCommDev,")",1,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
	    WriteFile(hCommDev,"---------------------------------",33,&dwBytesWritten, NULL);
		WriteFile(hCommDev,"\x0A",1,&dwBytesWritten, NULL);
        WriteFile(hCommDev,"�ֿ���ǩ����",12,&dwBytesWritten, NULL);
	//  WriteFile(hCommDev,"\x1d\x56\x00",3,&dwBytesWritten, NULL);  //�ڱ���ֽ
		WriteFile(hCommDev,"\x1b\x7F",2,&dwBytesWritten, NULL);      //������ֽ
	    WriteFile(hCommDev,"\x1b\x25\x41",3,&dwBytesWritten, NULL);  //��ʼ��
		CloseComm();
	}

	return 0;
}

void CCSDBPackagePayDlgCtrl::OnInProfitNOChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CCSDBPackagePayDlgCtrl::OnInAddForpeiFeeChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

//�ն�ǩ������
BSTR CCSDBPackagePayDlgCtrl::DoLogon(LPCTSTR makeUserGuId, LPCTSTR confirmStr1, LPCTSTR confirmStr2) 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	
	memset(StartVou,  0x00,sizeof(StartVou));

    WriteDiary("------------------------------------------------------","Diary.log");
    WriteDiary("�ն�ǩ����ʼ...","Diary.log");

    CString cm_SendPacket;
	
	char CommPort       [8 +1];
	char port           [2 +1];
	char termNO         [20 +1];

	char workKey        [32 +1];
	char returnVal      [2 +1];
	int i=0;

	memset(workKey,0x00,sizeof(workKey));
	memset(returnVal,0x00,sizeof(returnVal));
	int ret = 0;
	if(!m_Comport)
	{
		ret=GetComport();
		if(ret<0){
			WriteDiary("��ȡ�豸���ò���ʧ�ܣ�" ,"Diary.log");
			return 0;
		}
	}

	memset(CommPort,0x00,sizeof(CommPort));
	memset(port,0x00,sizeof(port));
	memset(termNO,0x00,sizeof(termNO));

	strcpy(CommPort,"COM");
	sprintf(port,"%d",m_Comport);
	strcat(CommPort,port);
	
	//��ȡ�ն˱��
	ret = SDBReadDeviceInfo(CommPort,termNO);
	memset(&termNO[strlen(termNO)],0x20,20-strlen(termNO));//

	cm_SendPacket.Format("0002%-8s%-20s",makeUserGuId,termNO);

	char SendDesPacket[2500];
	memset(SendDesPacket,0x00,sizeof(SendDesPacket));

	strcpy(SendDesPacket,cm_SendPacket);
	WriteDiary("���׹�����Կ���ݰ���" + CString(cm_SendPacket),"Diary.log");
    unsigned int inSendLen = strlen(SendDesPacket);


	char   m_RecvPacket[6000];
	memset(m_RecvPacket,0x00,sizeof(m_RecvPacket));

	FReturnVal = SendToSDB(SendDesPacket,m_RecvPacket,60,m_IP2,m_Port2,inSendLen);
	
	WriteDiary("���ذ���" + CString(m_RecvPacket),"Diary.log");
	//���ع̶�ֵ
	//FReturnVal = 0;
	//strcpy(m_RecvPacket,"00001234567890123456789012345678901234567890");

	WriteDiary("------------------------------------------------------","Diary.log");

	char KeyValue[9] = "\0";
	if (FReturnVal == 0)
	{   
		strncpy(returnVal,m_RecvPacket,2);
		if(!strcmp(returnVal,"00"))
		{
			WriteDiary("��Կ����ɹ���","Diary.log");		
			strncpy(workKey,&m_RecvPacket[2],32);
			WriteDiary("CommPort:" + CString(CommPort),"Diary.log");
			// WriteDiary("��Կ:" + CString(workKey),"Diary.log");
			
			char ExtPort[8] = "\0";
			char TMKIndex[] = "001";
			char KEYIndex[] = "01";
			char KeyLen[] = "2";
 			char KeyByTMK[33] = "\0";
			char KeyValue[9] = "\0";

 			strncpy(KeyByTMK,workKey, sizeof(workKey));
// 			WriteDiary("KeyByTMK:" + CString(KeyByTMK),"Diary.log");
			//������Կ
			int nret = SDBInputKEY(CommPort,ExtPort,TMKIndex,KEYIndex,KeyLen,KeyByTMK,KeyValue);
			// WriteDiary("����KeyValue:" + CString(KeyValue),"Diary.log");
			if(nret!=0)
			{
				WriteDiary("��Կ����ʧ�ܣ�","Diary.log");
				strResult.Format("03��Կ����ʧ�ܣ�");
			}
			else
				strResult.Format("00%s",makeUserGuId);
		}
		else
		{
			WriteDiary("��Կ����ʧ�ܣ�","Diary.log");
			strResult.Format("02��Կ����ʧ�ܣ�");
		}
	}
	else
	{
		WriteDiary("��Կ����ʧ�ܣ�","Diary.log");
		strResult.Format("01��Կ����ʧ�ܣ�");
	}
	WriteDiary("------------------------------------------------------","Diary.log");
	WriteDiary("���ظ����������ݣ�" + strResult,"Diary.log");
	return strResult.AllocSysString();
}

BSTR CCSDBPackagePayDlgCtrl::DoSale(LPCTSTR track1Data, LPCTSTR track2Data, LPCTSTR totalMoney, LPCTSTR makeUserGuId, LPCTSTR billInfo, LPCTSTR billInfoDetail) 
{
	CString strResult;
		////////////////////////////////////////////////
	memset(Track2Data,0x00,sizeof(Track2Data));
	memset(Track3Data,0x00,sizeof(Track3Data));
    memset(cardno,    0x00,sizeof(cardno));
	char AdminNO[6+1];//��������
	char UnitNO[20+1];//��λ����
	char VouKind[3+1];//Ʊ������
	char VouDate[10+1];//��Ʊ����
	char VouNO[10+1];//Ʊ�ݺ���
	char PayerName[50+1];//����������
	char VouFee[20+1];//Ʊ�ݺϼƽ��
	char MakeUserGuId[8+1];//����Ա��

	char FeeItemNo[10][20+1];	
	char FeeItemKind[10][20+1];//������λ
	char FeeLevel[10][16+1];//�շѱ�׼
	char FeeItemCount[10][16+1];
	char FeeItemAmt[10][16+1];

	CString cm_billInfo;
	CString cm_billInfoDetail;
	cm_billInfo=billInfo;
	cm_billInfoDetail=billInfoDetail;

	//��ʼ��
	memset(AdminNO,0x00,sizeof(AdminNO));
	memset(UnitNO,0x00,sizeof(UnitNO));
	memset(VouDate,0x00,sizeof(VouDate));
	memset(VouKind,0x00,sizeof(VouKind));
	memset(VouNO,0x00,sizeof(VouNO));
	memset(PayerName,0x00,sizeof(PayerName));
	memset(VouFee,0x00,sizeof(VouFee));
	memset(MakeUserGuId,0x00,sizeof(MakeUserGuId));

	memset(FeeItemNo,0x00,sizeof(FeeItemNo));
	memset(FeeLevel,0x00,sizeof(FeeLevel));
	memset(FeeItemKind,0x00,sizeof(FeeItemKind));
	memset(FeeItemCount,0x00,sizeof(FeeItemCount));
	memset(FeeItemAmt,0x00,sizeof(FeeItemAmt));


	char CommPort       [8 +1];
	char port           [2 +1];
	char termNO         [20 +1];
	char subTermNO		[8+1];

	int ret = 0;
	if(!m_Comport)
	{
		ret=GetComport();
		if(ret<0){
			WriteDiary("��ȡ�豸���ò���ʧ�ܣ�" ,"Diary.log");
			strResult.Format("01��ȡPOS����ʧ�ܣ�");
			return strResult.AllocSysString();
		}
	}
	
	memset(CommPort,0x00,sizeof(CommPort));
	memset(port,0x00,sizeof(port));
	memset(termNO,0x00,sizeof(termNO));
	memset(subTermNO,0x00,sizeof(subTermNO));

	strcpy(CommPort,"COM");
	sprintf(port,"%d",m_Comport);
	strcat(CommPort,port);

	//��ȡ�ն˱��
 	ret = SDBReadDeviceInfo(CommPort,termNO);
	WriteDiary("��ȡ�豸�ն˺ţ�" + CString(termNO),"Diary.log");
	memset(&termNO[strlen(termNO)],0x20,20-strlen(termNO));//
	cm_TermNO.Format("%s",termNO);

	//��ȡ20λ�ն˱�ź�8λ
	memcpy(subTermNO,termNO+strlen(termNO)-sizeof(subTermNO)+1, 9);
	WriteDiary("��ȡ�豸�ն˺ţ�" + CString(subTermNO),"Diary.log");
	cm_SubTermNO.Format("%s",subTermNO);
	
	char c_billInfo[150];
	char c_billInfoDetial[1000];
	memset(c_billInfo,0x00,sizeof(c_billInfo));
	memset(c_billInfoDetial,0x00,sizeof(c_billInfoDetial));
	strncpy(c_billInfo,(LPCTSTR)cm_billInfo,sizeof(c_billInfo));
	strncpy(c_billInfoDetial,(LPCTSTR)cm_billInfoDetail,sizeof(c_billInfoDetial));

	char SendValue[1000];
	int  j=0,k=0,l=0,flag =0,m=0,nLenR=0,i=0;
	memset(SendValue,0x00,sizeof(SendValue));
	nLenR = strlen(c_billInfo);

    for(i=0;i<=nLenR;i++)
	{
	   if(c_billInfo[i] != ';' && flag ==0 )
	   {
		   AdminNO[k] = c_billInfo[i];
		   l++;
	   }
	   else
	   {
		   AdminNO[k] =0x00;
		   flag =1;
	   }
	   j++;
       k++;	
	}
   l++;
   j=0;
  
   m=l;
   for(i=l;i<=nLenR;i++)
	{
	   if(c_billInfo[i] != ';')
	   {
		   UnitNO[j] = c_billInfo[i];
		   m++;
	   }
	   else
	   {
		   UnitNO[j] =0x00;
		   break;
	   }
	   j++;
	}
   m++;
   j=0;
   for(i=m;i<=nLenR;i++)
	{
	   if(c_billInfo[i] != ';')
	   {
		   if(c_billInfo[i] != '-')
		   {
				VouDate[j] = c_billInfo[i];
				j++;
		   }
		   m++;
	   }
	   else
	   {
		   VouDate[j] =0x00;
		   break;
	   }
//	   j++;
	}
   m++;
   j=0;
   for(i=m;i<=nLenR;i++)
	{
	   if(c_billInfo[i] != ';')
	   {
		   VouKind[j] = c_billInfo[i];
		   m++;
	   }
	   else
	   {
		   VouKind[j] =0x00;
		   break;
	   }
	   j++;
	}
   m++;
   j=0;
   for(i=m;i<=nLenR;i++)
	{
	   if(c_billInfo[i] != ';')
	   {
		   VouNO[j] = c_billInfo[i];
		   m++;
	   }
	   else
	   {
		   VouNO[j] =0x00;
		   break;
	   }
	   j++;
	}
   m++;
   j=0;
    for(i=m;i<=nLenR;i++)
	{
	   if(c_billInfo[i] != ';')
	   {
		   PayerName[j] = c_billInfo[i];
		   m++;
	   }
	   else
	   {
		   PayerName[j] =0x00;
		   break;
	   }
	   j++;
	}



	char MXSendValue[1000];
	int nMXLenR=0,nMXCount=0;
	j=0,k=0,l=0,flag =0,m=0,i=0;
	memset(MXSendValue,0x00,sizeof(MXSendValue));
	nMXLenR = strlen(c_billInfoDetial);

	for(nMXCount=0;nMXCount<10;nMXCount++)
	{	
		if (c_billInfoDetial[m] == '~')
		{
			m++;
		}

		k=0;l=m;
		for(i=m;i<=nMXLenR;i++)
		{
		   if(c_billInfoDetial[i] != ';' && c_billInfoDetial[i] != '~')
		   {
			   FeeItemNo[nMXCount][k] = c_billInfoDetial[i];
			   l++;
		   }
		   else
		   {
			   FeeItemNo[nMXCount][k] =0x00;
			   break;
		   }
		   j++;
		   k++;	
		}
	   l++;
	   j=0;
  
	   m=l;
	   for(i=l;i<=nMXLenR;i++)
		{
		   if(c_billInfoDetial[i] != ';')
		   {
			   FeeItemKind[nMXCount][j] = c_billInfoDetial[i];
			   m++;
		   }
		   else
		   {
			   FeeItemKind[nMXCount][j] =0x00;
			   break;
		   }
		   j++;
		}
	   m++;
	   j=0;
	   for(i=m;i<=nMXLenR;i++)
		{
		   if(c_billInfoDetial[i] != ';')
		   {
			   FeeLevel[nMXCount][j] = c_billInfoDetial[i];
			   m++;
		   }
		   else
		   {
			   FeeLevel[nMXCount][j] =0x00;
			   break;
		   }
		   j++;
		}
	   m++;
	   j=0;
	   for(i=m;i<=nMXLenR;i++)
		{
		   if(c_billInfoDetial[i] != ';')
		   {
			   FeeItemCount[nMXCount][j] = c_billInfoDetial[i];
			   m++;
		   }
		   else
		   {
			   FeeItemCount[nMXCount][j] =0x00;
			   break;
		   }
		   j++;
		}
	   m++;
	   j=0;
//	   ::AfxMessageBox(FeeItemCount[nMXCount],NULL,MB_OK);
// 	   WriteDiary("         ������" +CString(FeeItemCount[nMXCount]),"Diary.log");
		for(i=m;i<=nMXLenR;i++)
		{
		   if(c_billInfoDetial[i] != '~')
		   {
			   FeeItemAmt[nMXCount][j] = c_billInfoDetial[i];
			   m++;
		   }
		   else
		   {
			   FeeItemAmt[nMXCount][j] =0x00;
			   break;
		   }
		   j++;
		}
		m++;
//	   ::AfxMessageBox(FeeItemAmt[nMXCount],NULL,MB_OK);
		if(m >= nMXLenR) break;
	}

	//����ת��ΪԪ
	double vouFee=0;
	vouFee = strtod(totalMoney,NULL)/100;
	sprintf(VouFee,"%.2f",vouFee);
	cm_VouFee.Format("%s",VouFee);
	WriteDiary(cm_VouFee,"Diary,log");
	CPayAmount = cm_VouFee;

    memset(StartVou,  0x00,sizeof(StartVou));
    memcpy(StartVou,VouNO,strlen(VouNO));
	m_TimeOut=60;
	WriteDiary("------------------------------------------------------","Diary.log");
	WriteDiary("����ˢ���ۿ��ˢ����ʼ......","Diary.log");
	WriteDiary("     �������룺" + CString(AdminNO),"Diary.log");	
	WriteDiary("     ��λ���룺" + CString(UnitNO),"Diary.log");
	WriteDiary("     Ʊ�����ࣺ" + CString(VouKind) ,"Diary.log");
	WriteDiary("     ��Ʊ���ڣ�" + CString(VouDate),"Diary.log");
	WriteDiary("     Ʊ�ݺ��룺" + CString(VouNO) ,"Diary.log");	
	WriteDiary("   ������������" + CString(PayerName),"Diary.log");
	WriteDiary(" Ʊ�ݺϼƽ�" + CString(VouFee) ,"Diary.log");

	/*
	for(i=0;i <= nMXCount;i++)
	{
			WriteDiary("	   ִ���룺" + CString(FeeItemNo[i]),"Diary.log");	
			WriteDiary("     ������λ��" + CString(FeeItemKind[i]),"Diary.log");
			WriteDiary("     �շѱ�׼��" + CString(FeeLevel[i]) ,"Diary.log");
			WriteDiary("	     ������" + CString(FeeItemCount[i]),"Diary.log");
			WriteDiary("	     ��" + CString(FeeItemAmt[i]) ,"Diary.log");
	}
	*/
    if (atof(VouFee)	<0)
	{
		WriteDiary("�ۿ���Ƿ�������Ϊ��:" + CString(VouFee),"Diary.log");
		WriteDiary("------------------------------------------------------","Diary.log");
		return 0;
    }


////////////////////////////////////////////////
	ret=0;
	prtFlag    = 5;
	CLateAmt   = m_inLateFee;
	m_TimeOutV = m_TimeOut;

	cm_AdminNO.Format("%s",AdminNO);
	cm_UnitNO.Format("%s",UnitNO);
	cm_VouKind.Format("%s",VouKind);
	cm_VouDate.Format("%s",VouDate);
	cm_VouNO.Format("%s",VouNO);
	cm_PayerName.Format("%s",PayerName);

	memcpy(MakeUserGuId,makeUserGuId,strlen(makeUserGuId));
	cm_MakeUserGuId.Format("%s",MakeUserGuId);
	WriteDiary("   ����Ա�ţ�" + cm_MakeUserGuId,"Diary.log");


	for(i=0;i<10;i++)
	{
		strcpy(m_FeeItemNo[i],FeeItemNo[i]);
		strcpy(m_FeeLevel[i],FeeLevel[i]);
		strcpy(m_FeeItemKind[i],FeeItemKind[i]);
		strcpy(m_FeeItemCount[i],FeeItemCount[i]);
		strcpy(m_FeeItemAmt[i],FeeItemAmt[i]);
	}


	CancelFlag = 0;
	hasReadCard= 0;
	memset(m_OutPacket,0x00,sizeof(m_OutPacket));
	memset(m_inPayNO,0x00,sizeof(m_inPayNO));
	memset(cardno,0x00,sizeof(cardno));
    
	prtFlag = 1;

	ReadCardDlg  Rcd;
	ret = Rcd.DoModal();

//	m_Status = FReturnVal;
//	m_outDataPackage.Format("%s",m_OutPacket);
//	m_outAccount.Format("%d",ret);
//	WriteDiary("   ret��" + m_outAccount,"Diary.log");


	//���ع̶�ֵ ������
//	strcpy(m_OutPacket,"6225123412341234123201402211213140123456                    0714000000000100123456         ");

	//�������ذ� 
	//���°��ղ���Ҫ����� 00yyyymmddhhmiss000000------******%%%%%%%%$$$$totalMoneytermNo
	//���˵����������2 ����8 ʱ��6 �ն���ˮ��6 ���κ�6 ��Ȩ��6 ������8 ��Ƭ��Ч��4 ���׽��12��0 �ն˱��15 
	
	
	if (FReturnVal == 0 && prtFlag == 0)
	{
		WriteDiary("��ȡ�豸�ն˺ţ�" + CString(subTermNO),"Diary.log");
		strResult.Format("%s%s%s",m_OutPacket,m_MerchantNO,cm_SubTermNO);
		//��ӡƾ��
		PrtNormal();	
	} 
	else
	{
		strResult.Format("%s",m_OutPacket);
	}
	
	
	WriteDiary("���ظ����������ݣ�" + strResult,"Diary.log");
	return strResult.AllocSysString();
}

BSTR CCSDBPackagePayDlgCtrl::DoRefund(LPCTSTR track1Data, LPCTSTR track2Data, LPCTSTR totalMoney, LPCTSTR bookNo, LPCTSTR makeUserGuId, LPCTSTR confirmStr1, LPCTSTR confirmStr2) 
{
	CString strResult;
		////////////////////////////////////////////////
	memset(Track2Data,0x00,sizeof(Track2Data));
	memset(Track3Data,0x00,sizeof(Track3Data));
	char VouFee[20+1];//Ʊ�ݺϼƽ��
	long     ret =0;
	char     m_SendPacket[2500],m_RecvPacket[512];
	CString  CZSendPacket;
	memset(m_OutPacket,0x00,sizeof(m_OutPacket));
	memset(VouFee,0x00,sizeof(VouFee));

	//�ն˱��
	char CommPort       [8 +1];
	char port           [2 +1];
	char termNO         [20 +1];
	char subTermNO      [8 +1];

	if(!m_Comport)
	{
		ret=GetComport();
		if(ret<0){
			WriteDiary("��ȡ�豸���ò���ʧ�ܣ�" ,"Diary.log");
			return 0;
		}
	}

	memset(CommPort,0x00,sizeof(CommPort));
	memset(port,0x00,sizeof(port));
	memset(termNO,0x00,sizeof(termNO));
	memset(subTermNO,0x00,sizeof(subTermNO));

	strcpy(CommPort,"COM");
	sprintf(port,"%d",m_Comport);
	strcat(CommPort,port);
	
	//��ȡ�ն˱��
	ret = SDBReadDeviceInfo(CommPort,termNO);
	memset(&termNO[strlen(termNO)],0x20,20-strlen(termNO));

	//��ȡ20λ�ն˱�ź�8λ
	memcpy(subTermNO,termNO+strlen(termNO)-sizeof(subTermNO)+1, 9);
	WriteDiary("��ȡ�豸�ն˺ţ�" + CString(subTermNO),"Diary.log");
	cm_SubTermNO.Format("%s",subTermNO);

	FReturnVal =0;
	WriteDiary("-----------------------------------------------------","Diary.log");
	WriteDiary("���ʳ������׷��Ϳ�ʼ......","Diary.log");
	WriteDiary("       ����Ա��" + CString(makeUserGuId),"Diary.log");
	WriteDiary("   ������ˮ�ţ�" + CString(bookNo),"Diary.log");
	WriteDiary("     ������" + CString(totalMoney) ,"Diary.log");

// 	RefundFlag = 1;
// 	CancelFlag = 0;
// 	hasReadCard= 0;
// 	m_TimeOutV = 60;
// 	ReadCardDlg  Rcd;
// 	ret = Rcd.DoModal();//ֻ�Ƕ�ȡ����

	//����ת��ΪԪ
	double vouFee=0;
	vouFee = strtod(totalMoney,NULL)/100;
	sprintf(VouFee,"%.2f",vouFee);
	cm_VouFee.Format("%s",VouFee);

	memset(m_SendPacket,0x00,sizeof(m_SendPacket));
	memset(m_RecvPacket,0x00,sizeof(m_RecvPacket));
	char payMode[5];
	memset(payMode,0x00,sizeof(payMode));
	memcpy(payMode,"0001",4);
    CZSendPacket.Format("%-4s%-20s%-8s%-20s%-6s",
			payMode,  termNO,   makeUserGuId,  	cm_VouFee,   bookNo
			);
	WriteDiary("�������ݰ���" + CZSendPacket ,"Diary.log");	
    memcpy(m_SendPacket,CZSendPacket,strlen(CZSendPacket));
	unsigned int inSendLen = strlen(m_SendPacket);
	FReturnVal = SendToSDB(m_SendPacket,m_RecvPacket,60,m_IP2,m_Port2,inSendLen);
// 	if (FReturnVal == 0)
// 	{
// 	FReturnVal = SendToSDB(m_SendPacket,m_RecvPacket,60,m_IP2,m_Port2,inSendLen);
// 	}
 	
	//���͹̶�ֵ
// 	FReturnVal = 0;
// 	strcpy(m_RecvPacket,"001234567891234567899911111111222222333333444444555555");
	WriteDiary("�������ݰ���" + CString(m_RecvPacket) ,"Diary.log");
	if (FReturnVal == 0)
	{   
		FReturnVal = AnalyzeCZP(m_RecvPacket,m_OutPacket,payMode);
	}

	if (FReturnVal == 0)
	{
		strResult.Format("%s%s%s",m_OutPacket,m_MerchantNO,cm_SubTermNO);
	} 
	else
	{
		strResult.Format("%s",m_OutPacket);
	}

	WriteDiary("���ظ����������ݣ�" + strResult,"Diary.log");

	return strResult.AllocSysString();
}

BSTR CCSDBPackagePayDlgCtrl::DoReprint(LPCTSTR bookNo, LPCTSTR makeUserGuId, LPCTSTR confirmStr1, LPCTSTR confirmStr2) 
{
	CString strResult;
		////////////////////////////////////////////////
	memset(Track2Data,0x00,sizeof(Track2Data));
	memset(Track3Data,0x00,sizeof(Track3Data));

	long     ret =0;
	char     m_SendPacket[2500],m_RecvPacket[512];
	CString  RPSendPacket;
	memset(m_OutPacket,0x00,sizeof(m_OutPacket));
	char BookNO[7];
	memset(BookNO,0x00,sizeof(BookNO));
	memcpy(BookNO,bookNo,strlen(bookNo));

	FReturnVal =0;
	WriteDiary("-----------------------------------------------------","Diary.log");
	WriteDiary("ƾ���ش��׿�ʼ......","Diary.log");
	WriteDiary("   ������ˮ�ţ�" + CString(BookNO),"Diary.log");	
	WriteDiary("   ����Ա��ţ�" + CString(makeUserGuId),"Diary.log");		
	if (strlen(BookNO) <0)
	{
		WriteDiary("��Ч��ˮ��:" + CString(BookNO),"Diary.log");
		WriteDiary("------------------------------------------------------","Diary.log");
	}

	memset(m_SendPacket,0x00,sizeof(m_SendPacket));
	memset(m_RecvPacket,0x00,sizeof(m_RecvPacket));
	char payMode[5];
	memset(payMode,0x00,sizeof(payMode));
	memcpy(payMode,"0003",4);
    RPSendPacket.Format("%-4s%-6s%-8s",
			payMode  ,    BookNO,   makeUserGuId
			);	
	WriteDiary("�����" + RPSendPacket ,"Diary.log");	
	memcpy(m_SendPacket,RPSendPacket,strlen(RPSendPacket));
//	strcpy(m_SendPacket,"000373525512221811");//����
	WriteDiary("�������ݰ���" + CString(m_SendPacket) ,"Diary.log");	
	unsigned int inSendLen = strlen(m_SendPacket);
 	FReturnVal = SendToSDB(m_SendPacket,m_RecvPacket,60,m_IP2,m_Port2,inSendLen);
	WriteDiary("�������ݰ���" + CString(m_RecvPacket) ,"Diary.log");	

//  	FReturnVal = 0;
//  	strcpy(m_RecvPacket,"00123456789123456789911111111222222333333444444555555666666662099000000100000");
	
	if (FReturnVal == 0)
	{   
		FReturnVal = AnalyzeRPP(m_RecvPacket,m_OutPacket,BookNO);
	}
	
	
	if (FReturnVal ==0)
	{
		PrtNormal();
		strResult.Format("00");	
	}
	else
	{
		strResult.Format("01ƾ���ش�ʧ�ܣ�");
	}

	WriteDiary("���ظ����������ݣ�" + strResult,"Diary.log");
	return strResult.AllocSysString();
}

BSTR CCSDBPackagePayDlgCtrl::GetIniParam(LPCTSTR confirmStr1, LPCTSTR confirmStr2) 
{
	CString strResult;
	
	//���̨ͨ�Ų�����ע��
	/*
    WriteDiary("------------------------------------------------------","Diary.log");
    WriteDiary("��ȡ�̻��ſ�ʼ...","Diary.log");
    CString cm_SendPacket;
	

	char CommPort       [8 +1];
	char port           [2 +1];
	char termNO         [20 +1];

	int ret = 0;
	if(!m_Comport)
	{
		ret=GetComport();
		if(ret<0){
			WriteDiary("��ȡ�豸���ò���ʧ�ܣ�" ,"Diary.log");
			return 0;
		}
	}

	memset(CommPort,0x00,sizeof(CommPort));
	memset(port,0x00,sizeof(port));
	memset(termNO,0x00,sizeof(termNO));

	strcpy(CommPort,"COM");
	sprintf(port,"%d",m_Comport);
	strcat(CommPort,port);

	memset(termNO,0x00,sizeof(termNO));
	//��ȡ�ն˱��
	ret = SDBReadDeviceInfo(CommPort,termNO);
	memset(&termNO[strlen(termNO)],0x20,20-strlen(termNO));

	cm_SendPacket.Format("0004%s",termNO);

	char m_SendPacket[2500];
	memset(m_SendPacket,0x00,sizeof(m_SendPacket));

	strcpy(m_SendPacket,cm_SendPacket);
    unsigned int inSendLen = strlen(m_SendPacket);

	char   m_RecvPacket[6000];
	memset(m_RecvPacket,0x00,sizeof(m_RecvPacket));

	FReturnVal = SendToSDB(m_SendPacket,m_RecvPacket,60,m_IP2,m_Port2,inSendLen);
	//FReturnVal = 0;
	//strcpy(m_RecvPacket,"0000123456776543210");
	WriteDiary("���ذ���" + CString(m_RecvPacket),"Diary.log");
	WriteDiary("------------------------------------------------------","Diary.log");

	if (FReturnVal == 0)
	{   
		char strCheck[3],merchantNO[15];
		memset(strCheck,0x00,sizeof(strCheck));
		memset(merchantNO,0x00,sizeof(merchantNO));
		memcpy(strCheck,m_RecvPacket,2);
		if(!strcmp(strCheck,"00"))
		{
			WriteDiary("��ȡ�̻��ųɹ���","Diary.log");
// 			memcpy(merchantNO,&m_RecvPacket[2],15);
			strResult.Format("%s",m_RecvPacket);
		}
		else
		{
			strResult.Format("02");
		}
	}
	else
	{
		WriteDiary("��ȡ�̻���ʧ�ܣ�","Diary.log");
		strResult.Format("01��ȡ�̻���ʧ�ܣ�");
	}
	WriteDiary("------------------------------------------------------","Diary.log");
	*/

// 	strResult.Format("00%s",m_MerchantNO);
	WriteDiary("���ظ����������ݣ�" + strResult,"Diary.log");
	return strResult.AllocSysString();
}
