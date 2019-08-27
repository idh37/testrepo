#pragma once
#include "Resource.h"
#include "UPGDlgBase.h"

// CLobbyEventDlg 대화 상자입니다.
enum LobbyEventType
{
	EVENT_NORMAL,
	EVENT_CHICKEN,
	EVENT_CHICKENBIG,
	EVENT_CHICKENREWARD,
};

class UPG_COMMON_EXPORT CLobbyEventDlg : public CUPGDlgBase
{
	DECLARE_DYNAMIC(CLobbyEventDlg)


private:
	NMBASE::SKINGDI::CMyBitmap m_BackBit;
	NMBASE::SKINGDI::CMyButton	m_CloseBtn;

	CString m_strURL;
	LobbyEventType m_eventType;

public:
	//browser
	NMBASE::UTIL::CNMSSOWebBrowser* m_pBrowser;
	BOOL CreateBrowser();
	void DestroyBrowser();


	


public:
	CLobbyEventDlg(CWnd* pParent = NULL, LobbyEventType type = EVENT_NORMAL);   // 표준 생성자입니다.
	virtual ~CLobbyEventDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOBBYEVENT };

	virtual int DoModal(LPCTSTR szURL);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


	//external function 
	DECLARE_DISPATCH_MAP()
	void ClosePopup( short i );
	
};
