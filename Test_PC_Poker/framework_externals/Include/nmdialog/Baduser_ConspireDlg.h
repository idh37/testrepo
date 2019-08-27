#if !defined(AFX_BADUSER_CONSPIREDLG_H__03056F11_A85B_44E4_A768_67E6053C0F4A__INCLUDED_)
#define AFX_BADUSER_CONSPIREDLG_H__03056F11_A85B_44E4_A768_67E6053C0F4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Baduser_ConspireDlg.h : header file
//

#include "../resource.h"
#include "MYButton.h"
#include "DialogDefine.h"
#include "MyBitmap.h"
#include "BmpToRegion.h"
/////////////////////////////////////////////////////////////////////////////
// CBaduser_ConspireDlg dialog

class NMDIALOG_CLASS CBaduser_ConspireDlg : public CDialog
{
// Construction
protected:
	CBmpRegion m_rgn;

public:
	CBaduser_ConspireDlg(CWnd* pParent = NULL);   // standard constructor

	CMyBitmap Back;
// Dialog Data
	//{{AFX_DATA(CBaduser_ConspireDlg)
	enum { IDD = IDD_CONSPIRE_DIALOG };
	CMyButton	m_OkBtn;
	CMyButton	m_CancelBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaduser_ConspireDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBaduser_ConspireDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BADUSER_CONSPIREDLG_H__03056F11_A85B_44E4_A768_67E6053C0F4A__INCLUDED_)
