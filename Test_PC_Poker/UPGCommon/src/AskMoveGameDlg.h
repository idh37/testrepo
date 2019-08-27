#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CAskMoveGameDlg : public CUPGDlgBase
{
public:

	IDX_GAME m_idxGame;

	CAskMoveGameDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DIALOG_ASKMOVEGAME };
	NMBASE::SKINGDI::CMyButton	m_CancelBtn;
	NMBASE::SKINGDI::CMyButton	m_OkBtn;	

public:
	virtual int DoModal(IDX_GAME idxGame);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};