#if !defined(AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_)
#define AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "HTransRegion.h"
#include "MyButton.h"

// NewAskChargeDlg.h : header file
//
// 
// enum {
// 	ACD_GO_CHARGE = 1000,			// 충전 페이지 이동
// 	ACD_GO_LEADERS,				// 리더스 페이지 이동
// 	ACD_ALLIN_INSURANCE,		// 올인 보험 자세히 보기 
// 	ACD_ALLIN_INSURANCE_USE,	// 올인 보험 사용하기 
// 	ACD_ALLIN_INSURANCE_BUY,	// 올임 보험 구매하기 
// 	ACD_INVINCIBLE_BUY,			// 천하무적 구매하기 
// 	ACD_INVINCIBLE_DETAILS,		// 천하무적 자세히 보기 
// 	ACD_CLOSEWINDOW,			// 창닫기
// 	ACD_GO_MAINPAGE,			// 게임 페이지로 이동 
// 
// 	ACD_GAME_CONTINUE,			// [DB기록 Process] 2004.11.29  게임계속하기
// 	ACD_GO_LEADERS_ZONE,			// [DB기록 Process] 2004.11.29  리더스 회원존
// 	ACD_GO_AVATA_ZONE,			// [DB기록 Process] 2004.11.29  아바타 상점
// 	ACD_GO_SEARCH_PCROOM,       // [올인창 수정] 2006.01.04 피시방 찾기
// 	// -> 아래 한줄 추가
// 	ACD_CLOSEPOPUP, // [아이템 유료화 영역]
// };

enum NEW_ASK_CHARGE_DLG_TYPE
{	
	NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU=0,	//등급보험지급 우선 무료충전 (등급보험이있다면 등급보험 부터 받는다)	
	NEW_ASK_CHARGE_DLG_TYPE_INSU,		//등급보험 팝업 (등급보험만 체크 )
	NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE,	//무료충전 팝업 (무료충전만 체크 )
	
};



/////////////////////////////////////////////////////////////////////////////
// CNewAskChargeDlg dialog

class NMDIALOG_CLASS CNewAskChargeDlg : public CDialog
{
// Construction
protected:

	int m_nKind;

	CUPGSSOWebBrowser	m_Browser;
	CString             m_strURL;

	CMyBitmap           m_Back;
	CHTransRegion       m_Rgn;

	BOOL CreateBrowser();
	void DestroyBrowser();

	DWORD m_dwStartTime;	

	CMyButton m_BtnX;

public:

// 	static CNewAskChargeDlg *m_pNewAskChargeDlg;
// 	static void OnBeforeNavigate_NewAskCharge(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
// 	static void OnBeforeNavigate_OnNavigateComplete2(LPCTSTR lpszURL);
	
	void Navigate( CString& strURL );

	CNewAskChargeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewAskChargeDlg)
	enum { IDD = IDD_NEW_ASKCHARGEDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewAskChargeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int DoModal( int nKind = NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU, const char *pStrURL=NULL);	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNewAskChargeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


// 아이템 객체 등록 함수
protected:
	void ClosePopup( short wParam );
	void UpdateMoney( BOOL bClosePopup );
	void OpenMarket();
	void JoinInsuRequest();
	void ShowItemPopup( int nTab, LPCTSTR sURL );
	DECLARE_DISPATCH_MAP()

public:
	afx_msg void OnBnClickedButtonX();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWASKCHARGEDLG_H__CF792989_1B69_40CC_93FA_2F6F8A79FEA4__INCLUDED_)
