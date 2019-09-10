#pragma once

#include "Resource.h"
#include "UPGDlgBase.h"

// CGameManualDlg ��ȭ �����Դϴ�.

class UPG_COMMON_EXPORT CGameManualDlg : public CUPGDlgBase
{
	DECLARE_DYNAMIC(CGameManualDlg)

private:
	NMBASE::SKINGDI::CMyBitmap m_BackBit;
	NMBASE::SKINGDI::CMyButton	m_CloseBtn;

public:
	//browser
	NMBASE::UTIL::CNMSSOWebBrowser* m_pBrowser;
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
