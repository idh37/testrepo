#pragma once

#include "../Resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "MYButton.h"

// CGameManualDlg 대화 상자입니다.

class NMDIALOG_CLASS CGameManualDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameManualDlg)

private:
	CMyBitmap	m_BackBit;
	CMyButton	m_CloseBtn;

public:
	//browser
	CUPGSSOWebBrowser* m_pBrowser;
	BOOL CreateBrowser();
	void DestroyBrowser();


public:
	CGameManualDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGameManualDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_GAMEMANUAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
