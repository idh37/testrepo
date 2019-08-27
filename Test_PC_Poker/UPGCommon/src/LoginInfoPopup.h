#pragma once
#include "afxwin.h"

class UPG_COMMON_EXPORT CLoginInfoPopup : public LSY::CLSYLibDialogBase
{

public:
	CLoginInfoPopup(CWnd* pParent = NULL);
	virtual ~CLoginInfoPopup(){}

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal(CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
};