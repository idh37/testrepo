#pragma once

#include "Resource.h"
// CVIPChanceAlert 대화 상자입니다.

class UPG_COMMON_EXPORT CVIPChanceAlert : public CUPGDlgBase
{
	DECLARE_DYNAMIC(CVIPChanceAlert)

public:
	CVIPChanceAlert(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVIPChanceAlert();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_VIPCHANCEALERT };


	NMBASE::SKINGDI::CMyBitmap  m_Back;
	NMBASE::SKINGDI::CMyButton	m_CancelBtn;
	NMBASE::SKINGDI::CMyButton	m_OkBtn;	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
