#pragma once

#include "UPGDlgBase.h"
//#include "VipServiceSockCmnd.h"
#include "VipStruct.h"
#include "afxwin.h"

#define MODE_REGUEST_DIALOG		0
#define MODE_WATING_DIALOG		1
#define MODE_COUNSEL_DIALOG		2

class UPG_COMMON_EXPORT CVipServiceDlg : public CUPGDlgBase
{
public:
	CVipServiceDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DIALOG_VIP_SERVICE };
	NMBASE::SKINGDI::CMyButton		m_HideBtn;
	NMBASE::SKINGDI::CMyButton		m_CancelBtn;
	NMBASE::SKINGDI::CMyButton		m_CloseBtn;
	NMBASE::SKINGDI::CMyButton		m_SendBtn;
	NMBASE::SKINGDI::CMyButton		m_RequestBtn;
	NMBASE::SKINGDI::CGrpChatEdit2	m_ChatViewEdit;

public:
	LONG OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);
	LONG OnSocketClose(UINT, LONG);
	LONG OnFailConnect(UINT, LONG);	
	LONG OnSuccessConnect(UINT, LONG);
	BOOL IsConnectServer();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);	

public:	
	CString					m_strServerName;

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

protected:
	virtual BOOL OnInitDialog();	
	afx_msg void OnBtnSend();
	afx_msg void OnBtnRequestCounsel();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnHide();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};