#pragma once
#include "afxwin.h"

class UPG_COMMON_EXPORT CResetQuickStarterDlg : public LSY::CLSYLibDialogBase
{

public:
	CResetQuickStarterDlg(CWnd* pParent = NULL);
	virtual ~CResetQuickStarterDlg(){}

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal(CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
};