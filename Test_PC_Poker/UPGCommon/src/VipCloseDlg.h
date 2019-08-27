#pragma once

#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CVipCloseDlg : public CUPGDlgBase
{
public:
	CVipCloseDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DIALOG_VIPCLOSE };
	NMBASE::SKINGDI::CMyButton	m_NoBtn;
	NMBASE::SKINGDI::CMyButton	m_YesBtn;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};