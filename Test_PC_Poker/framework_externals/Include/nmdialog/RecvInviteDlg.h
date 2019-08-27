#if !defined(AFX_RECVINVITEDLG_H__503DD5A2_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_RECVINVITEDLG_H__503DD5A2_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecvInviteDlg.h : header file
//
#include "DialogDefine.h"
#include "UserStruct.h"
#include "MYButton.h"
#include "MyBitmap.h"
/////////////////////////////////////////////////////////////////////////////
// CRecvInviteDlg dialog
//#include "CharView.h"

class NMDIALOG_CLASS CRecvInviteDlg : public CDialog
{
// Construction
public:

	
 	USERINFO AskedUI;
 	SMALLROOMINFO RoomInfo;

	CMyBitmap Back;

	CRecvInviteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecvInviteDlg)
	enum { IDD = IDD_RECVINVITE_DIALOG };
	CBmpCheckButton	m_NoInviteCheckBtn;
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;	
	//}}AFX_DATA


	void DrawBack(CDC &DC);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecvInviteDlg)
	public:	
	virtual int DoModal(int tarunum, char *tarid, int roomnum, char *strmsg);
	virtual int DoModal(USERINFO *pAskedUI, SMALLROOMINFO* pSRI);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecvInviteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECVINVITEDLG_H__503DD5A2_33D4_11D4_97A5_0050BF0FBE67__INCLUDED_)
