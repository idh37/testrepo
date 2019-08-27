#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

enum TYPE_ENDPOPUP
{
	TYPE_ENDPOPUP_NO_EVENT=0,		// 이벤트가 없을 때 
	TYPE_ENDPOPUP_EVENT1,		// 이벤트 안내영역이 하단에 있을 때 
	TYPE_ENDPOPUP_EVENT2,		// 팝업전체가 이벤트안내 영역일 때 
	TYPE_ENDPOPUP_EVENT3,		// 이벤트 안내영역이 상단에만 있을때 
};

class UPG_COMMON_EXPORT CEndPopup : public CUPGDlgBase
{
public:
	CEndPopup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEndPopup();

	DECLARE_MESSAGE_MAP()

	enum { IDD = IDD_DIALOG_ENDPOPUP };
	NMBASE::SKINGDI::CMyButton	m_btnEndGame;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CPoint				m_ptPromotionBanner;

public:
	NMBASE::SKINGDI::CMyButton			m_btnEvent1;
	NMBASE::SKINGDI::CMyButton			m_btnEvent2;
	NMBASE::SKINGDI::CMyButton			m_btnEvent3;

protected:
	NMBASE::SKINGDI::CMyBitmap			m_Back;
	NMBASE::SKINGDI::CHTransRegion		m_Rgn;
	NMBASE::UTIL::CNMSSOWebBrowser	*m_pBrowser;
	BOOL				m_bEventRunning;	
	NMBASE::SKINGDI::CMyBitmap			m_EventBack;
	NMBASE::SKINGDI::CMyBitmap			m_bitProgBar;

	int			m_nPremNo;
	CString m_strURL;

	CString m_strMyNickname;
	INT64   m_llEarnedMoney;		//

	NMBASE::SKINGDI::CCustomHtml			*m_pCustomHtml;

	TYPE_ENDPOPUP m_eEndpopupType;

	DWORD	m_dwStartTime;	
	int		m_nElapsedTime;

public:
	void SetURL(const char * pURL);	
	
	BOOL CreateCustomHtml();
	void DestoryCustomHtml();
	BOOL CreateBrowser();
	void DestroyBrowser();

	LRESULT OnShowEndingURL(WPARAM &wParam, LPARAM &lParam);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickEvent1();
	afx_msg void OnClickEvent2();
	afx_msg void OnClickEvent3();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	afx_msg void OnTimer(UINT nIDEvent);

	void ClosePopup();
//	void Cancel();
	DECLARE_DISPATCH_MAP()

public:
	virtual INT_PTR DoModal(INT64 llEarnedMoney);	
};