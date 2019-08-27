#if !defined(AFX_JACKPOTRANKPOPUPDLG_H__DD94746C_C1C3_46E4_BF8D_14F823025CD1__INCLUDED_)
#define AFX_JACKPOTRANKPOPUPDLG_H__DD94746C_C1C3_46E4_BF8D_14F823025CD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JackpotRankPopupDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CJackpotRankPopupDlg dialog
#include "../resource.h"
#include "DialogDefine.h"
#include "UPGSSOWebLib.h"
#include "MyBitmap.h"
#include "HTransRegion.h"
#include "MYButton.h"

class NMDIALOG_CLASS CJackpotRankPopupDlg : public CDialog
{
// Construction
public:
	CJackpotRankPopupDlg(CWnd* pParent = NULL);   // standard constructor
	
	CHTransRegion		m_Rgn;
	CMyBitmap			m_Back;	
	CUPGSSOWebBrowser*		m_pBrowser;	

	BOOL CreateBrowser();
	void DestroyBrowser();

// Dialog Data
	//{{AFX_DATA(CJackpotRankPopupDlg)
	enum { IDD = IDD_DIALOG_JACKPOTRANK };
	CMyButton	m_OKBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJackpotRankPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	//}}AFX_VIRTUAL
public:

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJackpotRankPopupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JACKPOTRANKPOPUPDLG_H__DD94746C_C1C3_46E4_BF8D_14F823025CD1__INCLUDED_)
