#if !defined(AFX_BANISHVOTEDLG_H__F31493DA_EEA9_4BE7_8973_9CCB96095E3E__INCLUDED_)
#define AFX_BANISHVOTEDLG_H__F31493DA_EEA9_4BE7_8973_9CCB96095E3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BanishVoteDlg.h : header file
//

#include "../resource.h"
#include "MYButton.h"
#include "DialogDefine.h"

#define MAX_BANISHVOTE_USER (30)

/////////////////////////////////////////////////////////////////////////////
// CBanishVoteDlg dialog

class NMDIALOG_CLASS CBanishVoteDlg : public CDialog
{
// Construction
public:
	CString TarID[MAX_BANISHVOTE_USER];
		// 닉네임수정 	
	CString TarNickName[MAX_BANISHVOTE_USER];
	CBrush		m_HollowBrush;	
	CString Direct_TarID;

	int m_IDcnt;
	int m_Index_Tarid;
// 여기까지
	CBanishVoteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBanishVoteDlg)
	enum { IDD = IDD_BANISHVOTE };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CComboBox	m_TargetIDCombo;
	CString	m_Reson;
	//}}AFX_DATA

public:
	void AddUser(const char *pID, const char *pNick);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBanishVoteDlg)
	public:
	virtual int DoModal( char *id = NULL );
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBanishVoteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANISHVOTEDLG_H__F31493DA_EEA9_4BE7_8973_9CCB96095E3E__INCLUDED_)
