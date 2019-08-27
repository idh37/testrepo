#pragma once

#include "UPGDlgBase.h"


class UPG_COMMON_EXPORT CAskContinueGameDlg : public LSY::CLSYLibDialogBase
{
protected:
	std::list<std::string>		m_listMessage;
	bool						m_bClearMessage;
	LSY::CImage					*m_pImgBack;
	LSY::CText					*m_pTextMessage;

public:
	CAskContinueGameDlg(CWnd* pParent = NULL);


	virtual INT_PTR DoModal( LPCTSTR strMsg);
	virtual bool OnCreatedProject(void);

	virtual void OnOK();
	virtual void OnCancel();

public:
	LRESULT OnSetTextLineCount(LSY::CMessage *pMsg);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	DECLARE_MESSAGE_MAP()
};