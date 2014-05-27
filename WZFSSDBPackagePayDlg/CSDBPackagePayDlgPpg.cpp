// CSDBPackagePayDlgPpg.cpp : Implementation of the CCSDBPackagePayDlgPropPage property page class.

#include "stdafx.h"
#include "CSDBPackagePayDlg.h"
#include "CSDBPackagePayDlgPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCSDBPackagePayDlgPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCSDBPackagePayDlgPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCSDBPackagePayDlgPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid
//4C16571B-BA83-41BC-9CF5-42A8FBA39664
IMPLEMENT_OLECREATE_EX(CCSDBPackagePayDlgPropPage, "CSDBPACKAGEPAYDLG.CSDBPackagePayDlgPropPage.1",
	0x4c16571b, 0xba83, 0x41bc, 0x9c, 0xf5, 0x42, 0xa8, 0xfb, 0xa3, 0x96, 0x64)


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgPropPage::CCSDBPackagePayDlgPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCSDBPackagePayDlgPropPage

BOOL CCSDBPackagePayDlgPropPage::CCSDBPackagePayDlgPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CSDBPACKAGEPAYDLG_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgPropPage::CCSDBPackagePayDlgPropPage - Constructor

CCSDBPackagePayDlgPropPage::CCSDBPackagePayDlgPropPage() :
	COlePropertyPage(IDD, IDS_CSDBPACKAGEPAYDLG_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCSDBPackagePayDlgPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgPropPage::DoDataExchange - Moves data between page and properties

void CCSDBPackagePayDlgPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCSDBPackagePayDlgPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgPropPage message handlers
