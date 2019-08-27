#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CBadUserComplete : public LSY::CLSYLibDialogBase
{
protected:
	CString m_strID;

public:
	CBadUserComplete(CWnd* pParent = NULL);   // standard constructor
	void SetID( CString& ID ){m_strID = ID;}
	
public:
	virtual INT_PTR DoModal();

	virtual bool OnCreatedProject(void);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
protected:
	DECLARE_MESSAGE_MAP()
};