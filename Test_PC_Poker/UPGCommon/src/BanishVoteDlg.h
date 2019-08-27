#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

#define MAX_BANISHVOTE_USER (30)

class UPG_COMMON_EXPORT CBanishVoteDlg : public CUPGDlgBase
{
public:
	CString TarID[MAX_BANISHVOTE_USER];
	CString TarNickName[MAX_BANISHVOTE_USER];
	CBrush		m_HollowBrush;	
	CString Direct_TarID;

	int m_IDcnt;
	int m_Index_Tarid;

	CBanishVoteDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_BANISHVOTE };
	NMBASE::SKINGDI::CMyButton	m_CancelBtn;
	NMBASE::SKINGDI::CMyButton	m_OkBtn;
	CComboBox	m_TargetIDCombo;
	CString	m_Reson;

public:
	void AddUser(const char *pID, const char *pNick);

public:
	virtual int DoModal( char *id = NULL );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboReson();
};