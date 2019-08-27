#if !defined(AFX_KISADLG_H__AD1A6DA6_72C7_4777_9352_8EFB37905B4D__INCLUDED_)
#define AFX_KISADLG_H__AD1A6DA6_72C7_4777_9352_8EFB37905B4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KisaDlg.h : header file
//

#include "UPGDlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// CKisaDlg dialog

class UPG_COMMON_EXPORT CKisaDlg : public CUPGDlgBase
{
// Construction
public:
	CKisaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKisaDlg)
	enum { IDD = IDD_DIALOG_KISA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

private:


	
	NMBASE::SKINGDI::CMyBitmap m_BackBit;

	//browser
	NMBASE::UTIL::CNMSSOWebBrowser* m_pBrowser;
	BOOL CreateBrowser();
	void DestroyBrowser();

	static CKisaDlg *s_pKisaDlg;
	static void OnBeforeKisaNavigate(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKisaDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKisaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KISADLG_H__AD1A6DA6_72C7_4777_9352_8EFB37905B4D__INCLUDED_)
