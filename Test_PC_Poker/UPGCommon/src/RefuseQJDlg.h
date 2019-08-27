#pragma once

#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CRefuseQJDlg : public CUPGDlgBase
{
public:
	CRefuseQJDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DIALOG_REFUSEQJ };
	NMBASE::SKINGDI::CMyButton	m_BtnReConfig;
	NMBASE::SKINGDI::CMyButton	m_BtnCreateRoom;

	NMBASE::SKINGDI::CMyButton	m_BtnClose;

	NMBASE::SKINGDI::CMyBitmap m_BackBit;
	NMBASE::SKINGDI::CHTransRegion m_Rgn;

	bool		m_bClose ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
};