#if !defined(AFX_MYFAMILYDLG_H__A733F2A3_7D18_4B79_A572_C43144DFF17C__INCLUDED_)
#define AFX_MYFAMILYDLG_H__A733F2A3_7D18_4B79_A572_C43144DFF17C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFamilyDlg.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "MYButton.h"
/////////////////////////////////////////////////////////////////////////////
// CMyFamilyDlg dialog

class NMDIALOG_CLASS CMyFamilyDlg : public CDialog
{
// Construction
public:
	CMyFamilyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMyFamilyDlg)
	enum { IDD = IDD_DIALOG_MYFAMILY };
	CMyButton	m_CloseBtn;
	//}}AFX_DATA

	static CMyFamilyDlg* s_pFamilyDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFamilyDlg)
	public:
	virtual int DoModal( int PremMenuNo, int GameCode);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	static void OnBeforeMyFamilyNavigate(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);

// Implementation
protected: 

	CString				m_URL;
	
	CMyBitmap			m_Back;
	CUPGSSOWebBrowser	m_Web;

	SWEBCALLBACK_ONBN2 *m_pSSOCallbackFunc;

	// Generated message map functions
	//{{AFX_MSG(CMyFamilyDlg)
	afx_msg void OnOk();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFAMILYDLG_H__A733F2A3_7D18_4B79_A572_C43144DFF17C__INCLUDED_)
