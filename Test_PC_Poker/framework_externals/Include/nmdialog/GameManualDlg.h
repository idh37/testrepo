#pragma once

#include "../Resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "MYButton.h"

// CGameManualDlg ��ȭ �����Դϴ�.

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
	CGameManualDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGameManualDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_GAMEMANUAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
