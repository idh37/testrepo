#pragma once

#include "Resource.h"
// CVIPChanceAlert ��ȭ �����Դϴ�.

class UPG_COMMON_EXPORT CVIPChanceAlert : public CUPGDlgBase
{
	DECLARE_DYNAMIC(CVIPChanceAlert)

public:
	CVIPChanceAlert(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CVIPChanceAlert();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_VIPCHANCEALERT };


	NMBASE::SKINGDI::CMyBitmap  m_Back;
	NMBASE::SKINGDI::CMyButton	m_CancelBtn;
	NMBASE::SKINGDI::CMyButton	m_OkBtn;	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
