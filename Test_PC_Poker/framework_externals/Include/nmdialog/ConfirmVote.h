#if !defined(AFX_CONFIRMVOTE_H__A1BA19E2_DE8F_4229_B9E2_09C2B45AEA1F__INCLUDED_)
#define AFX_CONFIRMVOTE_H__A1BA19E2_DE8F_4229_B9E2_09C2B45AEA1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfirmVote.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
#include "MyBitmap.h"
#include "MYButton.h"

/////////////////////////////////////////////////////////////////////////////
// CConfirmVote dialog

class NMDIALOG_CLASS CConfirmVote : public CDialog
{
	CMyBitmap Back;	

	char m_ID[16];

// Construction
public:
	void SetID( char *pID );
	CConfirmVote(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfirmVote)
	enum { IDD = IDD_CONFIRM_VOTE };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfirmVote)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfirmVote)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIRMVOTE_H__A1BA19E2_DE8F_4229_B9E2_09C2B45AEA1F__INCLUDED_)
