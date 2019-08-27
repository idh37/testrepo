#pragma once

class UPG_COMMON_EXPORT CBuddyAddResultDlg : public LSY::CLSYLibDialogBase
{
public:
	LSY::CText			*m_pTxtMessage;
	LSY::CButton		*m_pBtnClose;

	std::string			m_strMessage;
public:
	CBuddyAddResultDlg(CWnd* pParent = NULL);
	virtual ~CBuddyAddResultDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(std::string strMessage, CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
};