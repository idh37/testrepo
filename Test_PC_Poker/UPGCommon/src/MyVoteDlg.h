#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

#define MYVOTE_TIMER1 1132121

class UPG_COMMON_EXPORT CMyVoteDlg : public CUPGDlgBase
{
public:
	UINT hTimer;
	int NowLeftTime;
	CMyVoteDlg(CWnd* pParent = NULL);   // standard constructor
	NMBASE::SKINGDI::CMyBitmap	Back;

	BOOL DoModaless(char *ID,char *Reson);

	enum { IDD = IDD_MYVOTE };
	NMBASE::SKINGDI::CMyButton	m_btnAssent;
	NMBASE::SKINGDI::CMyButton	m_btnConcept;
	NMBASE::SKINGDI::CProgressCtrlEx	m_TimeProgress;
	CString	m_BanishReson;
	CString	m_NickName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};