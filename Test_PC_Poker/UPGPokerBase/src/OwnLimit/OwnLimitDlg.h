#if !defined(AFX_OWNLIMITDLG_H__4BC0E703_5E41_46FC_BAA6_1403A1A34A93__INCLUDED_)
#define AFX_OWNLIMITDLG_H__4BC0E703_5E41_46FC_BAA6_1403A1A34A93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OwnLimitDlg.h : header file
//

/*#include "BmpToRegion.h"*/

// 1. 정액제상품추천 URL
// http://game1.netmarble.net/family/LimitMoney/popRecommendGoods.asp?game=영문게임코드
// 2. 초과머니 회수금 안내 URL
// http://game1.netmarble.net/family/LimitMoney/popCollectOverMoney.asp?game=영문게임코드
// 3. 보정머니/자동결제실패 회수금 안내 URL
// http://game1.netmarble.net/family/LimitMoney/popCollectRevisionMoney.asp?game=영문게임코드
// 4. 초과머니/보정머니 중복존재 안내 URL
// http://game1.netmarble.net/family/LimitMoney/popCollectBothMoney.asp?game=영문게임코드
// 5. 보유한도달성 안내 URL
// http://game1.netmarble.net/family/LimitMoney/popAchieveLimitMoney.asp?game=영문게임코드


enum HANDOPOPUP
{
	HANDOPOPUP_NULL,
	HANDOPOPUP_COLLECT_OVER_MONEY,		// 정액제상품추천 URL
	HANDOPOPUP_ACHIEVE_LIMIT_MONEY,		// 보유한도달성 안내 URL
	HANDOPOPUP_COLLECT_REVISION_MONEY,// 보정머니/자동결제실패 회수금 안내 URL
	HANDOPOPUP_COLLECT_BOTH_MONEY,	//초과머니/보정머니 중복존재 안내 URL	

	HANDOPOPUP_COLLECT_OVER_GOLDBANK,		// 정액제상품추천 URL

	HANDOPOPUP_MAX,
};

/////////////////////////////////////////////////////////////////////////////
// COwnLimitDlg dialog

class UPG_GAMEBASE_EXPORT COwnLimitDlg : public CUPGDlgBase
{
// Construction
public:

	NMBASE::SKINGDI::CMyBitmap m_Back;

	COwnLimitDlg(CWnd* pParent = NULL);   // standard constructor

	NMBASE::UTIL::CNMSSOWebBrowser *m_pBrowser;

	HANDOPOPUP m_HandoPopup;
	
	CString GetNotTodayKeyString();

	BOOL CreateBrowser();
	void DestroyBrowser();
	void Navigate();

	static void ResetNotToday(const char *pMyID);

// Dialog Data
	//{{AFX_DATA(COwnLimitDlg)
	enum { IDD = IDD_OWNLIMIT_DIALOG };
	NMBASE::SKINGDI::CMyButton	m_BtnCancel;
	NMBASE::SKINGDI::CMyButton	m_BtnOk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnLimitDlg)
	public:
	virtual int DoModal(HANDOPOPUP HandoPopup);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	LRESULT OnDoModal(WPARAM &wParam, LPARAM &lParam);
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COwnLimitDlg)
	virtual BOOL OnInitDialog();
	virtual  void OnDestroy();
	virtual  void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	void ClosePopup(short i);
	DECLARE_DISPATCH_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNLIMITDLG_H__4BC0E703_5E41_46FC_BAA6_1403A1A34A93__INCLUDED_)
