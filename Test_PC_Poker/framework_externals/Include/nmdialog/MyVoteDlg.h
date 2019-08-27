#if !defined(AFX_MYVOTEDLG_H__8A6A0FB7_C1CC_4D0D_9816_ECCCFF2CFFCF__INCLUDED_)
#define AFX_MYVOTEDLG_H__8A6A0FB7_C1CC_4D0D_9816_ECCCFF2CFFCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyVoteDlg.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
#include "DialogDefine.h"
#include "MYButton.h"
#include "MyBitmap.h"
#include "ProgressCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CMyVoteDlg dialog


#define MYVOTE_TIMER1 1132121

class NMDIALOG_CLASS CMyVoteDlg : public CDialog
{
// Construction
public:
	UINT hTimer;
	int NowLeftTime;
	CMyVoteDlg(CWnd* pParent = NULL);   // standard constructor
	CMyBitmap	Back;
	//int			StartTime;

	BOOL DoModaless(char *ID,char *Reson);
// Dialog Data
	//{{AFX_DATA(CMyVoteDlg)
	enum { IDD = IDD_MYVOTE };
	CMyButton	m_btnAssent;
	CMyButton	m_btnConcept;
	CProgressCtrlEx	m_TimeProgress;
	CString	m_BanishReson;
		// 닉네임수정 
	//CString	m_TargetID;
	CString	m_NickName;
// 여기까지
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyVoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyVoteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYVOTEDLG_H__8A6A0FB7_C1CC_4D0D_9816_ECCCFF2CFFCF__INCLUDED_)
