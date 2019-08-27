#pragma once

#include "UPGDlgBase.h"
#include "InviteUserListView.h"

class UPG_COMMON_EXPORT CInviteDlg : public CUPGDlgBase
{
public:
	NMBASE::SKINGDI::CCustomHtml			*m_pCustomHtml;
	NMBASE::SKINGDI::CMyBitmap Back;
	NMBASE::SKINGDI::CBmpRegion m_Rgn;
	CRect OrgWindRect, OrgListRect;
	CInviteUserListView m_UserListView;

	enum { IDD = IDD_INVITE_DIALOG };	

	NMBASE::SKINGDI::CMyButton	m_InviteBtn;	
	NMBASE::SKINGDI::CMyButton	m_CancelBtn;	

public:
	CInviteDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL CreateCustomHtml();
	void DestoryCustomHtml();
	void PopUp();
	void RefreshUserList();	
	void SetGameFilter(CDataUserList::KEY_FILTER KeyFilter);	//유저리스트 필터 세팅

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnButtonInvite();	
	afx_msg void OnPaint();
	afx_msg void OnDestroy();	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};