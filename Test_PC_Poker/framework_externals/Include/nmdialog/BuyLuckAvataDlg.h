#if !defined(AFX_BUYLUCKAVATADLG_H__58D3B261_B65C_4DA6_91D8_9BB8D075B365__INCLUDED_)
#define AFX_BUYLUCKAVATADLG_H__58D3B261_B65C_4DA6_91D8_9BB8D075B365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BuyLuckAvataDlg.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "MYButton.h"
#include "hTransRegion.h"
/////////////////////////////////////////////////////////////////////////////
// CBuyLuckAvataDlg dialog

//[행운아바타] 2006.07.24	
class NMDIALOG_CLASS CBuyLuckAvataDlg : public CDialog
{
// Construction
public:
	CBuyLuckAvataDlg(CWnd* pParent = NULL);   // standard constructor	

	CMyBitmap Back;		
	
	CString m_ChanceNavigateURL;
	CString				m_CookieData;	
	SYSTEMTIME			m_RecvTime;
	int					m_nItemCode;
	CHTransRegion m_Rgn;
	CUPGSSOWebBrowser m_pWebGameItem;
	int					m_nBuySpecialItemCnt;
		

	int					get_BuyItem_GameCode( int itemcode );	

	BOOL SafeClose();
	virtual void OnCancel();

	virtual int DoModal(char* cookiedata,  SYSTEMTIME ItemRecvTime ,int itemcode, int nBuySpecialItemCnt = 0);	
// Dialog Data
	//{{AFX_DATA(CBuyLuckAvataDlg)
	enum { IDD = IDD_DIALOG_BUY_LUCKAVATA };
	CMyButton	m_CloseBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuyLuckAvataDlg)
	public:
	
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBuyLuckAvataDlg)
	virtual BOOL OnInitDialog();
	
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


		
protected:
	
	DWORD m_dwStartTime;
	void ClosePopup( short i );
	void UpdateMoney( BOOL bClosePopup );
	DECLARE_DISPATCH_MAP()
		
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUYLUCKAVATADLG_H__58D3B261_B65C_4DA6_91D8_9BB8D075B365__INCLUDED_)
