#if !defined(AFX_NEWGAMEITEMDLG_H__C545D06C_C13B_4EE7_B19A_FEE5E3E47273__INCLUDED_)
#define AFX_NEWGAMEITEMDLG_H__C545D06C_C13B_4EE7_B19A_FEE5E3E47273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewGameItemDlg.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "MYButton.h"
#include "hTransRegion.h"
/////////////////////////////////////////////////////////////////////////////
// CNewGameItemDlg dialog

#define DEF_MAX_URLs 6
// #define DEF_FUNCITEM_INDEX 0
// #define DEF_MYITEM_INDEX 1
// #define DEF_CASH_INDEX 2
// #define DEF_GAMEMONEY_ITEM_INDEX 3
// #define DEF_FAMILY_INDEX 4

class NMDIALOG_CLASS CNewGameItemDlg : public CDialog
{
public:
	CMyBitmap Back;	
	
	CHTransRegion m_Rgn;

	BOOL m_bModal;
	int  m_nTab;
	BOOL m_bRefreshList;
	int  m_nDefaultItem;

	CString				m_CookieData;
	int m_ItemCode;
	int m_GameCode;
	SYSTEMTIME			m_RecvTime;

	int		m_AvatarIndex;
	CString m_AvatarURL;
	
	BOOL m_bMyItemRefresh;
	DWORD m_dwStartTime;
	
	CString m_URLs[DEF_MAX_URLs];
	CString m_LastURLs[DEF_MAX_URLs];
//	CWebCtrl* m_pWebGameItem[DEF_MAX_URLs];
	CUPGSSOWebBrowser* m_pWebGameItem[DEF_MAX_URLs]; // 초이스 아바타

	// 아이템 객체 대응 함수 : 초이스 아바타
protected:
	void ShowItemPopup( int nItemType, LPCTSTR url );
	void UpdateMoney( BOOL bClosePopup );
	void ClosePopup( short wParam );
	void ChangeTab( short wParam );
	void OnCompleteUseItem(int nItemCode);
	DECLARE_DISPATCH_MAP()


// Construction
public:
	// -> 아래 추가
//////////////////////////////////////////////////////////
	void ResetFlags() {
		m_AvatarIndex = -1;
	}
//////////////////////////////////////////////////////////
	void SetMyParent( CWnd* pWnd );
	void RefreshMyItem();
	void Reset();
	void Refresh();
	void ShowTab( int nIndex, BOOL bRefresh = TRUE );
	void ItemShopNavigate( CString &URL, int nIndex, BOOL bRefresh );
	void CreateSSOWebGameItem( int nIndex );

	BOOL DoModaless( int nTab = 0, BOOL bRefreshList = TRUE, int iDefSelItemKind = 0, BOOL bThread = FALSE );
	BOOL SafeClose();

	CNewGameItemDlg(CWnd* pParent = NULL);   // standard constructor

	int AvatarDoModal( const char *pURL ,int tab );

	void SetURL();

	void EnableTabBtn(int i);

// Dialog Data
	//{{AFX_DATA(CNewGameItemDlg)
	enum { IDD = IDD_DIALOG_NEWGAMEITEM };
	CMyButton	m_PokerItemBtn;
	CMyButton	m_PokerAvatarBtn;
	CMyButton	m_LuckAvatarBtn;
	CMyButton	m_FamilyAvatarBtn;
	CMyButton	m_MyItemBtn;
	CMyButton	m_ChargeCashBtn;
	CMyButton	m_CloseBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewGameItemDlg)
	public:
	virtual int DoModal( int nTab = 0, BOOL bRefreshList = TRUE, int iDefSelItemKind = 0 );
	virtual int DoModal( char* cookiedata, int ItemCode, int GameCode, SYSTEMTIME ItemRecvTime );

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewGameItemDlg)
	virtual void OnCancel();
	afx_msg void OnButtonFamilyavatar();
	afx_msg void OnButtonPokeravatar();
	afx_msg void OnButtonLuckavatar();
	afx_msg void OnButtonPokeritem();
	afx_msg void OnButtonMyitem();
	afx_msg void OnButtonChargecash();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWGAMEITEMDLG_H__C545D06C_C13B_4EE7_B19A_FEE5E3E47273__INCLUDED_)
