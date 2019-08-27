#pragma once

#include "afxwin.h"
//#include "GlobalBase.h"

class UPG_COMMON_EXPORT CDailyPopup : public LSY::CLSYLibDialogBase
{

public:
	CDailyPopup(CWnd* pParent = NULL);
	virtual ~CDailyPopup(){}

	LSY::CWindows *m_pWndBack ;

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal(CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
};