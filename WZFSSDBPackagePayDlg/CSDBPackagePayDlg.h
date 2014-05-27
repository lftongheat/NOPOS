#if !defined(AFX_CSDBPACKAGEPAYDLG_H__1FC9BF6F_0AA7_46F3_9260_DE16B4256A68__INCLUDED_)
#define AFX_CSDBPACKAGEPAYDLG_H__1FC9BF6F_0AA7_46F3_9260_DE16B4256A68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CSDBPackagePayDlg.h : main header file for CSDBPACKAGEPAYDLG.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgApp : See CSDBPackagePayDlg.cpp for implementation.

class CCSDBPackagePayDlgApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSDBPACKAGEPAYDLG_H__1FC9BF6F_0AA7_46F3_9260_DE16B4256A68__INCLUDED)
