#pragma once

#include "resource.h"
#include "ConfirmVote.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CConfirmVote : public CUPGDlgBase
{
	NMBASE::SKINGDI::CMyBitmap Back;	
	char m_ID[16];
public:
	void SetID( char *pID );
	CConfirmVote(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_CONFIRM_VOTE };
	NMBASE::SKINGDI::CMyButton	m_CancelBtn;
	NMBASE::SKINGDI::CMyButton	m_OkBtn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};