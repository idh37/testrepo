#pragma once

#include "UPGDlgBase.h"

//[행운아바타] 2006.07.24	
class UPG_COMMON_EXPORT CBuyTimeLimitAvatarDlg : public CUPGDlgBase
{
public:
	CBuyTimeLimitAvatarDlg(CWnd* pParent = NULL);   // standard constructor	

	NMBASE::SKINGDI::CMyBitmap Back;		
	
	CString m_ChanceNavigateURL;
	CString				m_CookieData;	
	SYSTEMTIME			m_RecvTime;
	int					m_nItemCode;
	NMBASE::SKINGDI::CHTransRegion m_Rgn;
	NMBASE::UTIL::CNMSSOWebBrowser m_pWebGameItem;
	int					m_nBuySpecialItemCnt;
		

	int					get_BuyItem_GameCode( int itemcode );	

	BOOL SafeClose();
	virtual void OnCancel();

	virtual int DoModal();	

	enum { IDD = IDD_DIALOG_BUY_TIMELIMIT };
	NMBASE::SKINGDI::CMyButton	m_CloseBtn;

public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	static void WebClose();

protected:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
protected:
	
	DWORD m_dwStartTime;
	void ClosePopup( short i );
	void UpdateMoney( BOOL bClosePopup );
	DECLARE_DISPATCH_MAP()
		
};