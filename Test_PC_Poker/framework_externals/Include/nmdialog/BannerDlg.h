#if !defined(AFX_BANNERDLG_H__3BEED52E_23C0_4ED5_88E1_C353155CE9AE__INCLUDED_)
#define AFX_BANNERDLG_H__3BEED52E_23C0_4ED5_88E1_C353155CE9AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BannerDlg.h : header file
//
#include "../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CBannerDlg dialog
#include "DialogDefine.h"
#include "SSOWeb.h"
#include "MyBitmap.h"
#include "MYButton.h"
#include "hTransRegion.h"


class NMDIALOG_CLASS CBannerDlg : public CDialog
{
// Construction
public:
	CBannerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBannerDlg)
	enum { IDD = IDD_BANNER_DIALOG };
	CMyButton	m_OKBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBannerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBannerDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CNMSSOWebBrowser *m_pWebCtrl;

	BOOL	m_bTimer;	

	CMyBitmap		Back;
	CHTransRegion	m_rgn;

	CString m_strBannerURL;
	
public:

	void	Draw(CDC* pDC);

	void	SetBannerURL(const char *pURL);
	CString	GetBannerURL();

	void	CreateBanner();
	void	DestroyBanner();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANNERDLG_H__3BEED52E_23C0_4ED5_88E1_C353155CE9AE__INCLUDED_)
