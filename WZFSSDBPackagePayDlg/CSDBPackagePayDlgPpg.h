#if !defined(AFX_CSDBPACKAGEPAYDLGPPG_H__0AB16002_8975_46CC_AC36_01A65B158CEE__INCLUDED_)
#define AFX_CSDBPACKAGEPAYDLGPPG_H__0AB16002_8975_46CC_AC36_01A65B158CEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CSDBPackagePayDlgPpg.h : Declaration of the CCSDBPackagePayDlgPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgPropPage : See CSDBPackagePayDlgPpg.cpp.cpp for implementation.

class CCSDBPackagePayDlgPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCSDBPackagePayDlgPropPage)
	DECLARE_OLECREATE_EX(CCSDBPackagePayDlgPropPage)

// Constructor
public:
	CCSDBPackagePayDlgPropPage();

// Dialog Data
	//{{AFX_DATA(CCSDBPackagePayDlgPropPage)
	enum { IDD = IDD_PROPPAGE_CSDBPACKAGEPAYDLG };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCSDBPackagePayDlgPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSDBPACKAGEPAYDLGPPG_H__0AB16002_8975_46CC_AC36_01A65B158CEE__INCLUDED)
