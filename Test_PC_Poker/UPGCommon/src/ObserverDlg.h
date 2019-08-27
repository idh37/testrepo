#pragma once

#include "UPGDlgBase.h"
#include "ObserverListView.h"

class UPG_COMMON_EXPORT CObserverDlg : public CUPGDlgBase
{
public:
	void User_ResetReservation();
	void User_SetItemReservation(int unum, char *id, int seq);
	void User_DelItem(int unum, char* id);
	void User_SetItem(int unum, char *id , char *nick, BOOL sex, int level, INT64 money, int seq);
	void User_InsItem(int unum, char *id , char *nick, BOOL sex, int level, INT64 money, int seq);
	void User_InitList();
	void User_DelAll();
	void RefreshButton();

	NMBASE::SKINGDI::CMyBitmap Back;
	CImageList m_UserImage;

	CRect OrgWindRect, OrgListRect;
	int ViewMode;
	int nUserSel;

	CObserverListView		m_ObserverListView;


	NMBASE::GRAPHICGDI::xSprite *m_pInviteUserListBackSpr, *m_pInviteUserListBtnSpr;
	NMBASE::GRAPHICGDI::xSprite *m_pLevelSpr, *m_pEtcSpr;
	NMBASE::GRAPHICGDI::xSprite m_CheckSpr;


	BOOL IsAbleToBanish();
	void SetSprite(NMBASE::GRAPHICGDI::xSprite *pBackSpr, NMBASE::GRAPHICGDI::xSprite *pButtonSpr , NMBASE::GRAPHICGDI::xSprite *pLevelSpr, NMBASE::GRAPHICGDI::xSprite *pEtcSpr);

	CObserverDlg(CWnd* pParent = NULL);
	virtual ~CObserverDlg();

	enum { IDD = IDD_DLG_OBSERVER };	
	NMBASE::SKINGDI::CMyButton	m_KickOutBtn;
	NMBASE::SKINGDI::CMyButton	m_CloseBtn;		
	NMBASE::SKINGDI::CBmpCheckButton	m_CheckAll;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();	
	afx_msg void OnBtnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonKickOut();
	afx_msg void OnBtnProfile();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};