#pragma once

class UPG_COMMON_EXPORT CUPGDlgBase : public CDialog
{
protected:
	bool m_bModal;
	DECLARE_DYNAMIC(CUPGDlgBase)
public:
	CUPGDlgBase(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CUPGDlgBase();

	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
protected:
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()
};