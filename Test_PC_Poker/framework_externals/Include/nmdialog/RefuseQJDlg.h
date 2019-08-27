#if !defined(AFX_REFUSEQJDLG_H__CFBF349B_0A9F_472E_B9E4_D5DEC296908D__INCLUDED_)
#define AFX_REFUSEQJDLG_H__CFBF349B_0A9F_472E_B9E4_D5DEC296908D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RefuseQJDlg.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
#include "MYButton.h"
#include "MyBitmap.h"
#include "hTransRegion.h"
/////////////////////////////////////////////////////////////////////////////
// CRefuseQJDlg dialog

class NMDIALOG_CLASS CRefuseQJDlg : public CDialog
{
// Construction
public:
	CRefuseQJDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRefuseQJDlg)
	enum { IDD = IDD_DIALOG_REFUSEQJ };
	CMyButton	m_BtnReConfig;
	CMyButton	m_BtnCreateRoom;
	//}}AFX_DATA

	CMyBitmap m_BackBit;
	CHTransRegion m_Rgn;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRefuseQJDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRefuseQJDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REFUSEQJDLG_H__CFBF349B_0A9F_472E_B9E4_D5DEC296908D__INCLUDED_)
