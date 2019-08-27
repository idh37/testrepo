#pragma once
#include "../Resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "MYButton.h"

// CLobbyEventDlg 대화 상자입니다.

class NMDIALOG_CLASS CLobbyEventDlg : public CDialog
{
	DECLARE_DYNAMIC(CLobbyEventDlg)


private:
	CMyBitmap m_BackBit;
	CMyButton	m_CloseBtn;

	CString m_strParam;

public:
	//browser
	CUPGSSOWebBrowser* m_pBrowser;
	BOOL CreateBrowser();
	void DestroyBrowser();
	


public:
	CLobbyEventDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLobbyEventDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOBBYEVENT };

	virtual int DoModal(LPCTSTR szParam);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
