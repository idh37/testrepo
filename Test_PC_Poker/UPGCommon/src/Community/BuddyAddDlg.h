#pragma once

class UPG_COMMON_EXPORT CBuddyAddDlg : public LSY::CLSYLibDialogBase
{
public:
	LSY::CEdit			*m_pEditNickName;
	LSY::CCombo			*m_pComboGroup;
	LSY::CButton		*m_pBtnOK;
	LSY::CButton		*m_pBtnClose;

	std::string			m_strNickName;
public:
	CBuddyAddDlg(CWnd* pParent = NULL);
	virtual ~CBuddyAddDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnReturn(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(std::string strNickName, CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
};