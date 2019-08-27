#if !defined(AFX_BANISHMENTDLG_H__F9340ECE_95E8_4EF3_9890_94AE17344DB6__INCLUDED_)
#define AFX_BANISHMENTDLG_H__F9340ECE_95E8_4EF3_9890_94AE17344DB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BanishmentDlg.h : header file
//

#include "../resource.h"
#include "MYButton.h"
#include "DialogDefine.h"
#include "MyBitmap.h"
#include "MyEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CBanishmentDlg dialog

class NMDIALOG_CLASS CBanishmentDlg : public CDialog
{
// Construction
public:

	CMyBitmap Back;
	CString   BanisCnt;
	CBanishmentDlg(CWnd* pParent = NULL);   // standard constructor
	virtual int DoModal( CString id );

// Dialog Data
	//{{AFX_DATA(CBanishmentDlg)
	enum { IDD = IDD_BANISHMENT_DIALOG };
	CMyButton	m_CloseBtn;
	CMyButton	m_BanismentBtn;
	CString	m_BanishmentID;
	CMyEdit	m_BanismentEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBanishmentDlg)
	public:
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBanishmentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANISHMENTDLG_H__F9340ECE_95E8_4EF3_9890_94AE17344DB6__INCLUDED_)
