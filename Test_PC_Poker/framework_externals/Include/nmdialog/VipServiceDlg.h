#if !defined(AFX_VIPSERVICEDLG_H__23448C8B_9303_41C4_9AEA_41E53A063644__INCLUDED_)
#define AFX_VIPSERVICEDLG_H__23448C8B_9303_41C4_9AEA_41E53A063644__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VipServiceDlg.h : header file
//
#include "DialogDefine.h"
#include "VipServiceSockCmnd.h"
#include "VipStruct.h"
#include "afxwin.h"
#include "MYButton.h"

/////////////////////////////////////////////////////////////////////////////
// CVipServiceDlg dialog

#define MODE_REGUEST_DIALOG		0
#define MODE_WATING_DIALOG		1
#define MODE_COUNSEL_DIALOG		2

class NMDIALOG_CLASS CVipServiceDlg : public CDialog
{
// Construction
public:
	CVipServiceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVipServiceDlg)
	enum { IDD = IDD_DIALOG_VIP_SERVICE };
	CMyButton		m_HideBtn;
	CMyButton		m_CancelBtn;
	CMyButton		m_CloseBtn;
	CMyButton		m_SendBtn;
	CMyButton		m_RequestBtn;
	CGrpChatEdit2	m_ChatViewEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVipServiceDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	CSockMan				m_SockMan;
	CVipServiceSockCmnd		m_SockCmnd;
	int						m_nServerId;

	ST_SERVICE_ON_OFF		m_ServiceCfg;

	int						m_nMode;

	CString					m_strBack;

	// 웨이팅시 쓰는 변수
	int						m_nWaitingNum;

	BOOL					m_bCounselFinished;
	int						m_nLogin;

public:
	void ConnectServer();
	void DisconnectServer();
	void ChangeMode( int nMode );

	void DrawRequest( CDC *pDC );
	void DrawWaiting( CDC *pDC );
	void DrawCounsel( CDC *pDC );

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVipServiceDlg)
	virtual BOOL OnInitDialog();
	afx_msg LONG OnPacketNotify(UINT, LONG);
	afx_msg LONG OnConnect(UINT, LONG);
	afx_msg LONG OnDisconnect(UINT, LONG);
	afx_msg LONG OnConnectFailure(UINT, LONG);
	afx_msg void OnBtnSend();
	afx_msg void OnBtnRequestCounsel();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnHide();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIPSERVICEDLG_H__23448C8B_9303_41C4_9AEA_41E53A063644__INCLUDED_)
