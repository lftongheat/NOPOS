#if !defined(AFX_READCARDDLG_H__1F39BB70_5840_4527_801A_FFEA6387C701__INCLUDED_)
#define AFX_READCARDDLG_H__1F39BB70_5840_4527_801A_FFEA6387C701__INCLUDED_
#include "ColorStaticST.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReadCardDlg.h : header file
//
#define WM_READCARDFINISHED WM_USER+2
UINT CThreadReadCardProc(LPVOID pParam);
/////////////////////////////////////////////////////////////////////////////
// ReadCardDlg dialog
extern  CString cm_TermNO;

class ReadCardDlg : public CDialog
{
// Construction
public:
	int ICConnectCard();
	int ReadPassWord();
	int ReadCardNo();
	int ThreadReadCard();
	ReadCardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ReadCardDlg)
	enum { IDD = IDD_DIALOGReadCard };
	CEdit	m_payamount;
	CButton	m_cancelCard;
	CEdit	m_comcardnoctl;
	CEdit	m_compasswordctl;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReadCardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    
	// Generated message map functions
	//{{AFX_MSG(ReadCardDlg)
	  virtual BOOL OnInitDialog();
	  afx_msg LRESULT OnReadCardFinished(WPARAM wParam,LPARAM lParam);
	  afx_msg int OnBcancelRead();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	   CColorStaticST m_comsprompt;
	   CFont m_Font;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READCARDDLG_H__1F39BB70_5840_4527_801A_FFEA6387C701__INCLUDED_)
