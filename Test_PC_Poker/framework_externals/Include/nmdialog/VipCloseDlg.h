#if !defined(AFX_VIPCLOSEDLG_H__48FBACC9_1196_4EE2_964A_FBA8FDAC9832__INCLUDED_)
#define AFX_VIPCLOSEDLG_H__48FBACC9_1196_4EE2_964A_FBA8FDAC9832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VipCloseDlg.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
#include "MYButton.h"

/////////////////////////////////////////////////////////////////////////////
// CVipCloseDlg dialog

class NMDIALOG_CLASS CVipCloseDlg : public CDialog
{
// Construction
public:
	CVipCloseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVipCloseDlg)
	enum { IDD = IDD_DIALOG_VIPCLOSE };
	CMyButton	m_NoBtn;
	CMyButton	m_YesBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVipCloseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVipCloseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIPCLOSEDLG_H__48FBACC9_1196_4EE2_964A_FBA8FDAC9832__INCLUDED_)
