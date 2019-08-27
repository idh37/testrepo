#pragma once


#include "UPGDlgBase.h"

#define INDEX_TAB_CASHCHARGE (5)
#define DEF_MAX_URLs 6

class UPG_COMMON_EXPORT CNewGameItemDlg : public CUPGDlgBase
{
public:
	NMBASE::SKINGDI::CMyBitmap Back;	
	NMBASE::SKINGDI::CHTransRegion m_Rgn;

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
	NMBASE::UTIL::CNMSSOWebBrowser* m_pWebGameItem[DEF_MAX_URLs]; // 초이스 아바타
	
protected:
	void ShowItemPopup( int nItemType, LPCTSTR url );
	void UpdateMoney( BOOL bClosePopup );
	void ClosePopup( short wParam );
	void ChangeTab( short wParam );
	void OnCompleteUseItem(int nItemCode);

	DECLARE_DISPATCH_MAP()
public:
	void ResetFlags() {
		m_AvatarIndex = -1;
	}
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

	static void WebClose();


	enum { IDD = IDD_DIALOG_NEWGAMEITEM };
	NMBASE::SKINGDI::CMyButton	m_PokerItemBtn;
	NMBASE::SKINGDI::CMyButton	m_PokerAvatarBtn;
	NMBASE::SKINGDI::CMyButton	m_LuckAvatarBtn;
	NMBASE::SKINGDI::CMyButton	m_FamilyAvatarBtn;
	NMBASE::SKINGDI::CMyButton	m_MyItemBtn;
	NMBASE::SKINGDI::CMyButton	m_ChargeCashBtn;
	NMBASE::SKINGDI::CMyButton	m_CloseBtn;

public:
	virtual int DoModal( int nTab = 0, BOOL bRefreshList = TRUE, int iDefSelItemKind = 0 );
	virtual int DoModal( char* cookiedata, int ItemCode, int GameCode, SYSTEMTIME ItemRecvTime );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
protected:
	virtual void OnOK();
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
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};