#if !defined(AFX_BADUSER_MONEYDLG_H__53FF363B_79B9_4631_B6D2_0498DB2B6693__INCLUDED_)
#define AFX_BADUSER_MONEYDLG_H__53FF363B_79B9_4631_B6D2_0498DB2B6693__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BadUser_MoneyDlg.h : header file
//

#include "../resource.h"

#include "MYButton.h"
#include "DialogDefine.h"
#include "MyBitmap.h"
#include "BmpToRegion.h"
/////////////////////////////////////////////////////////////////////////////
// CBadUser_MoneyDlg dialog

class NMDIALOG_CLASS CBadUser_MoneyDlg : public CDialog
{
// Construction
protected:
	CBmpRegion m_rgn;
	
public:
	CBadUser_MoneyDlg(CWnd* pParent = NULL);   // standard constructor
	
	CMyBitmap Back;
// Dialog Data
	//{{AFX_DATA(CBadUser_MoneyDlg)
	enum { IDD = IDD_BADUSER_MONEY_DIALOG };
	CMyButton	m_OKBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBadUser_MoneyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBadUser_MoneyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BADUSER_MONEYDLG_H__53FF363B_79B9_4631_B6D2_0498DB2B6693__INCLUDED_)
