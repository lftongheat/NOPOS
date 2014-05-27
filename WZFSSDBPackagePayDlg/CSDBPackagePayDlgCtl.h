#if !defined(AFX_CSDBPACKAGEPAYDLGCTL_H__B37D7911_81C1_47FB_AA93_EB762876B216__INCLUDED_)
#define AFX_CSDBPACKAGEPAYDLGCTL_H__B37D7911_81C1_47FB_AA93_EB762876B216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CSDBPackagePayDlgCtl.h : Declaration of the CCSDBPackagePayDlgCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CCSDBPackagePayDlgCtrl : See CSDBPackagePayDlgCtl.cpp for implementation.

class CCSDBPackagePayDlgCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCSDBPackagePayDlgCtrl)

// Constructor
public:
	CCSDBPackagePayDlgCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSDBPackagePayDlgCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CCSDBPackagePayDlgCtrl();

	DECLARE_OLECREATE_EX(CCSDBPackagePayDlgCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCSDBPackagePayDlgCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCSDBPackagePayDlgCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCSDBPackagePayDlgCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCSDBPackagePayDlgCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCSDBPackagePayDlgCtrl)
	CString m_inPnCount;
	afx_msg void OnInPnCountChanged();
	CString m_inReceivableAmt;
	afx_msg void OnInReceivableAmtChanged();
	CString m_inUnitNO;
	afx_msg void OnInUnitNOChanged();
	CString m_outDataPackage;
	afx_msg void OnOutDataPackageChanged();
	short m_TimeOut;
	afx_msg void OnInTimeOutChanged();
	CString m_inPnNO;
	afx_msg void OnInPnNOChanged();
	long m_Status;
	afx_msg void OnOutStatusChanged();
	CString m_outAccount;
	afx_msg void OnOutAccountChanged();
	CString m_inPayID;
	afx_msg void OnInPayIDChanged();
	CString m_inPayName;
	afx_msg void OnInPayNameChanged();
	CString m_inFeeItemNo1;
	afx_msg void OnInFeeItemNo1Changed();
	CString m_inFeeItemFee1;
	afx_msg void OnInFeeItemFee1Changed();
	CString m_inFeeItemNo2;
	afx_msg void OnInFeeItemNo2Changed();
	CString m_inFeeItemFee2;
	afx_msg void OnInFeeItemFee2Changed();
	CString m_inFeeItemFee3;
	afx_msg void OnInFeeItemFee3Changed();
	CString m_inFeeItemNo3;
	afx_msg void OnInFeeItemNo3Changed();
	CString m_inFeeItemName1;
	afx_msg void OnInFeeItemName1Changed();
	CString m_inFeeItemName2;
	afx_msg void OnInFeeItemName2Changed();
	CString m_inFeeItemName3;
	afx_msg void OnInFeeItemName3Changed();
	CString m_inLateFee;
	afx_msg void OnInLateFeeChanged();
	CString m_inBigFee;
	afx_msg void OnInBigFeeChanged();
	CString m_inTranDate;
	afx_msg void OnInTranDateChanged();
	CString m_inCardNO;
	afx_msg void OnInCardNOChanged();
	CString m_inWaterNo;
	afx_msg void OnInWaterNoChanged();
	CString m_inUnitName;
	afx_msg void OnInUnitNameChanged();
	CString m_inPrtVou;
	afx_msg void OnInPrtVouChanged();
	short m_inPrtMode;
	afx_msg void OnInPrtModeChanged();
	CString m_SFile;
	afx_msg void OnSFileChanged();
	CString m_DFile;
	afx_msg void OnDFileChanged();
	CString m_czReason;
	afx_msg void OnCzReasonChanged();
	CString m_inDataPackage;
	afx_msg void OnInDataPackageChanged();
	CString m_inVouNO;
	afx_msg void OnInVouNOChanged();
	CString m_inVouType;
	afx_msg void OnInVouTypeChanged();
	CString m_inInvaildNoReason;
	afx_msg void OnInInvaildNoReasonChanged();
	CString m_inFeeItemFee4;
	afx_msg void OnInFeeItemFee4Changed();
	CString m_inFeeItemFee5;
	afx_msg void OnInFeeItemFee5Changed();
	CString m_inFeeItemFee6;
	afx_msg void OnInFeeItemFee6Changed();
	CString m_inFeeItemName4;
	afx_msg void OnInFeeItemName4Changed();
	CString m_inFeeItemName5;
	afx_msg void OnInFeeItemName5Changed();
	CString m_inFeeItemName6;
	afx_msg void OnInFeeItemName6Changed();
	CString m_inFeeItemNo4;
	afx_msg void OnInFeeItemNo4Changed();
	CString m_inFeeItemNo5;
	afx_msg void OnInFeeItemNo5Changed();
	CString m_inFeeItemNo6;
	afx_msg void OnInFeeItemNo6Changed();
	CString m_intzdType;
	afx_msg void OnIntzdTypeChanged();
	CString m_inBatP;
	afx_msg void OnInBatPChanged();
	CString m_outBatP;
	afx_msg void OnOutBatPChanged();
	CString m_outFSCode;
	afx_msg void OnOutFSCodeChanged();
	CString m_inMerchantNO;
	afx_msg void OnInMerchantNOChanged();
	CString m_inProfitNO;
	afx_msg void OnInProfitNOChanged();
	CString m_inAddForpeiFee;
	afx_msg void OnInAddForpeiFeeChanged();
	afx_msg long PrtVou();
	afx_msg long PPrtVou();
	afx_msg long PrtNormal();
	afx_msg long PrtForfeit();
	afx_msg BSTR DoLogon(LPCTSTR makeUserGuId, LPCTSTR confirmStr1, LPCTSTR confirmStr2);
	afx_msg BSTR DoSale(LPCTSTR track1Data, LPCTSTR track2Data, LPCTSTR totalMoney, LPCTSTR makeUserGuId, LPCTSTR billInfo, LPCTSTR billInfoDetail);
	afx_msg BSTR DoRefund(LPCTSTR track1Data, LPCTSTR track2Data, LPCTSTR totalMoney, LPCTSTR bookNo, LPCTSTR makeUserGuId, LPCTSTR confirmStr1, LPCTSTR confirmStr2);
	afx_msg BSTR DoReprint(LPCTSTR bookNo, LPCTSTR makeUserGuId, LPCTSTR confirmStr1, LPCTSTR confirmStr2);
	afx_msg BSTR GetIniParam(LPCTSTR confirmStr1, LPCTSTR confirmStr2);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCSDBPackagePayDlgCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CCSDBPackagePayDlgCtrl)
	dispidInPnCount = 1L,
	dispidInReceivableAmt = 2L,
	dispidInUnitNO = 3L,
	dispidOutDataPackage = 4L,
	dispidInTimeOut = 5L,
	dispidInPnNO = 6L,
	dispidOutStatus = 7L,
	dispidOutAccount = 8L,
	dispidInPayID = 9L,
	dispidInPayName = 10L,
	dispidInFeeItemNo1 = 11L,
	dispidInFeeItemFee1 = 12L,
	dispidInFeeItemNo2 = 13L,
	dispidInFeeItemFee2 = 14L,
	dispidInFeeItemFee3 = 15L,
	dispidInFeeItemNo3 = 16L,
	dispidInFeeItemName1 = 17L,
	dispidInFeeItemName2 = 18L,
	dispidInFeeItemName3 = 19L,
	dispidInLateFee = 20L,
	dispidInBigFee = 21L,
	dispidInTranDate = 22L,
	dispidInCardNO = 23L,
	dispidInWaterNo = 24L,
	dispidInUnitName = 25L,
	dispidInPrtVou = 26L,
	dispidInPrtMode = 27L,
	dispidSFile = 28L,
	dispidDFile = 29L,
	dispidCzReason = 30L,
	dispidInDataPackage = 31L,
	dispidInVouNO = 32L,
	dispidInVouType = 33L,
	dispidInInvaildNoReason = 34L,
	dispidInFeeItemFee4 = 35L,
	dispidInFeeItemFee5 = 36L,
	dispidInFeeItemFee6 = 37L,
	dispidInFeeItemName4 = 38L,
	dispidInFeeItemName5 = 39L,
	dispidInFeeItemName6 = 40L,
	dispidInFeeItemNo4 = 41L,
	dispidInFeeItemNo5 = 42L,
	dispidInFeeItemNo6 = 43L,
	dispidIntzdType = 44L,
	dispidInBatP = 45L,
	dispidOutBatP = 46L,
	dispidOutFSCode = 47L,
	dispidInMerchantNO = 48L,
	dispidInProfitNO = 49L,
	dispidInAddForpeiFee = 50L,
	dispidPrtVou = 51L,
	dispidPPrtVou = 52L,
	dispidPRTNORMAL = 53L,
	dispidPrtForfeit = 54L,
	dispidDoLogon = 55L,
	dispidDoSale = 56L,
	dispidDoRefund = 57L,
	dispidDoReprint = 58L,
	dispidGetIniParam = 59L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSDBPACKAGEPAYDLGCTL_H__B37D7911_81C1_47FB_AA93_EB762876B216__INCLUDED)
