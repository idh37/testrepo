#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CBanishmentDlg : public CUPGDlgBase
{
public:
	NMBASE::SKINGDI::CMyBitmap Back;
	CString   BanisCnt;
	CBanishmentDlg(CWnd* pParent = NULL);   // standard constructor
	virtual int DoModal( CString id );

	enum { IDD = IDD_BANISHMENT_DIALOG };
	NMBASE::SKINGDI::CMyButton	m_CloseBtn;
	NMBASE::SKINGDI::CMyButton	m_BanismentBtn;
	CString	m_BanishmentID;
	NMBASE::SKINGDI::CMyEdit	m_BanismentEdit;

public:
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};