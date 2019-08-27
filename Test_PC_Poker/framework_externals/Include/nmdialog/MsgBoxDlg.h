#if !defined(AFX_MSGBOXDLG_H__108F2455_B6B4_49F2_92C8_B7C8310697DA__INCLUDED_)
#define AFX_MSGBOXDLG_H__108F2455_B6B4_49F2_92C8_B7C8310697DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgBoxDlg.h : header file
//

#include "../resource.h"
#include "DialogDefine.h"
#include "MYButton.h"
#include "MyBitmap.h"
#include <list>

using namespace std;

#define MSGBOXDLG_WAIT_TIMER 339
#define MAX_NOTICE_STR_NUM		 1024

/////////////////////////////////////////////////////////////////////////////
// CMsgBoxDlg dialog

class NMDIALOG_CLASS CMsgBoxDlg : public CDialog
{
// Construction
protected:
	struct MESSAGEBOX_DATA 
	{
	bool	bYesNo;
	CString	strMessage;
	};

	CMyButton		m_btnOK;
	CMyButton		m_btnCancel;
	CMyBitmap		m_bmpBack;

	list<MESSAGEBOX_DATA>	m_listMsgData;
	CRect					m_rtText;


protected:
	void InitButton(bool bYesNo);
public:
	CMsgBoxDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgBoxDlg)
	enum { IDD = IDD_MSG_BOX };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgBoxDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int MsgBox(bool bYesNo, CString &msgstr);
	int MsgBox(bool bYesNo, LPCTSTR lpMsg);

	// Generated message map functions
	//{{AFX_MSG(CMsgBoxDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGBOXDLG_H__108F2455_B6B4_49F2_92C8_B7C8310697DA__INCLUDED_)