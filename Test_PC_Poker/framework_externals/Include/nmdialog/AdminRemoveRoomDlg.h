#if !defined(AFX_ADMINREMOVEROOMDLG_H__FEBF70A8_3948_4A75_8F83_D8B27275861F__INCLUDED_)
#define AFX_ADMINREMOVEROOMDLG_H__FEBF70A8_3948_4A75_8F83_D8B27275861F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminRemoveRoomDlg.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
/////////////////////////////////////////////////////////////////////////////
// CAdminRemoveRoomDlg dialog

class NMDIALOG_CLASS CAdminRemoveRoomDlg : public CDialog
{
// Construction
public:
	CAdminRemoveRoomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdminRemoveRoomDlg)
	enum { IDD = IDD_ADMIN_REMOVEROOM };
	CComboBox	m_cmbMsg;
	CString	m_strMsg;
	CString	m_text;
	//}}AFX_DATA

	int m_RoomNo;
	CString m_strRoomTitle;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminRemoveRoomDlg)
	public:
	virtual int DoModal(int RoomNo, const char* szRoomTitle);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdminRemoveRoomDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINREMOVEROOMDLG_H__FEBF70A8_3948_4A75_8F83_D8B27275861F__INCLUDED_)
