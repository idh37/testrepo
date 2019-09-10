#pragma once
#include "../Resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "MYButton.h"

// CLobbyEventDlg ��ȭ �����Դϴ�.

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
	CLobbyEventDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLobbyEventDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LOBBYEVENT };

	virtual int DoModal(LPCTSTR szParam);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
