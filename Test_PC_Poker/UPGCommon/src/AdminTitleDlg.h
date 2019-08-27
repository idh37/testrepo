#pragma once

#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CAdminTitleDlg : public CUPGDlgBase
{
public:
	CAdminTitleDlg(CString strName, CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_ADMIN_TITLE };
	CComboBox	m_ctrlTitle;
	CString	m_strPrevTitle;
	CString	m_strName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};