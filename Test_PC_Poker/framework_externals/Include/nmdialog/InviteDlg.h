#if !defined(AFX_INVITEDLG_H__F8F83B77_21B2_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_INVITEDLG_H__F8F83B77_21B2_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DialogDefine.h"
#include "InviteUserListView.h"
#include "MYButton.h"
#include "MyBitmap.h"
#include "BmpToRegion.h"
/////////////////////////////////////////////////////////////////////////////
// CInviteDlg dialog
class NMDIALOG_CLASS CInviteDlg : public CDialog
{
// Construction
public:
	CCustomHtml			*m_pCustomHtml;

	CMyBitmap Back;
	CBmpRegion m_Rgn;
	

	CRect OrgWindRect, OrgListRect;

	CInviteUserListView m_UserListView;

	

// 	void MoveToSide();
// 	void User_InitList();
// //	void User_InsItem(int unum, char *id, BOOL sex, int winnum, int loosenum, int point, int position, int roomnum, int level );
// 	void User_InsItem(SMALLUSERINFO *pList);
// //	void User_SetItem(int unum, char *id, int winnum, int loosenum, int point, int position, int roomnum, int level );
// 	void User_SetItem(SMALLUSERINFO *pList);
// 	void User_DelItem(int unum, char *id);
// 	void User_ReloadList();

// 	void OnButtonProfile();
// 
// 	CString get_InviteMsg();
// Dialog Data
	//{{AFX_DATA(CInviteDlg)
	enum { IDD = IDD_INVITE_DIALOG };	
	CMyButton	m_InviteBtn;	
	CMyButton	m_CancelBtn;	
	//}}AFX_DATA

public:

	CInviteDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL CreateCustomHtml();
	void DestoryCustomHtml();
	void PopUp();
	void RefreshUserList();	
	void SetGameFilter(CDataUserList::KEY_FILTER KeyFilter);	//유저리스트 필터 세팅

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInviteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInviteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnButtonInvite();	
	afx_msg void OnPaint();
	afx_msg void OnDestroy();	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVITEDLG_H__F8F83B77_21B2_11D4_97A5_0050BF0FBE67__INCLUDED_)
