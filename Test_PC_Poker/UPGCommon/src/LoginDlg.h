#if !defined(AFX_LOGINDLG_H__1F31AF84_B89F_4648_90FC_D57158C49727__INCLUDED_)
#define AFX_LOGINDLG_H__1F31AF84_B89F_4648_90FC_D57158C49727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UPGDlgBase.h"
/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

class UPG_COMMON_EXPORT CLoginDlg : public CUPGDlgBase
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGIN_DIALOG };
	CString	m_ID;
	CString	m_Pass;
	CString	m_ServerIP;
	int		m_ServerPort;
	int		m_Group;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSel7poker();
	afx_msg void OnBnClickedSelLowbd();
	afx_msg void OnBnClickedSelNewpoker();
	afx_msg void OnBnClickedSelHighlow();
	afx_msg void OnBnClickedSelSut();
	afx_msg void OnBnClickedSelHoola();
	afx_msg void OnBnClickedSelDualpoker();

	afx_msg void OnBnClickedSelTournament();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__1F31AF84_B89F_4648_90FC_D57158C49727__INCLUDED_)
