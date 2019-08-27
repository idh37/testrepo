#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CAdminMessageDlg : public CUPGDlgBase
{
public:
	CAdminMessageDlg(CWnd* pParent = NULL);   // standard constructor

	int  m_UNum;
	BOOL m_bShow;
	BOOL IsShow(){ return m_bShow; }
	void Hide();
	void Show(int unum, CString strID = _T(""));

	void OnChatSend();

	enum { IDD = IDD_ADMIN_MESSAGE };
	CString	m_strMessChat;
	CString	m_strMessID;
	int		m_nTime;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnHide();
	afx_msg void OnBtnSend();
	afx_msg void OnButtonWar();

	DECLARE_MESSAGE_MAP()
};