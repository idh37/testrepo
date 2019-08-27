#pragma once
// AskMoveGameDlg.h : header file
//

#include "../resource.h"
#include "MYButton.h"
#include "DialogDefine.h"
/////////////////////////////////////////////////////////////////////////////
// CAskMoveGameDlg dialog

class NMDIALOG_CLASS CAskMoveGameDlg : public CDialog
{
// Construction
public:

	IDX_GAME m_idxGame;

	CAskMoveGameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAskMoveGameDlg)
	enum { IDD = IDD_DIALOG_ASKMOVEGAME };
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAskMoveGameDlg)
	public:
	virtual int DoModal(IDX_GAME idxGame);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAskMoveGameDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
