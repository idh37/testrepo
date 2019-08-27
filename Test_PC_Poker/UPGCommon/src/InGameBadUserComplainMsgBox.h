#pragma once
#include "resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CInGameBadUserComplainMsgBox: public CUPGDlgBase
{
	DECLARE_DYNAMIC(CInGameBadUserComplainMsgBox)
public:
	CInGameBadUserComplainMsgBox(CWnd* pParent = NULL);
	virtual ~CInGameBadUserComplainMsgBox();
	virtual int DoModal();

	enum { IDD = IDD_COMPLAIN_BOX };

	NMBASE::SKINGDI::CMyBitmap m_BackBit;
	NMBASE::SKINGDI::CMyButton	m_OpenBtn;
	NMBASE::SKINGDI::CMyButton	m_OKBtn;
	NMBASE::SKINGDI::CMyButton	m_CloseBtn;
	CString m_strName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnCancel();
	//}}AFX_MSG
};
