#pragma once

class UPG_COMMON_EXPORT CBuddyGroupAddDlg : public LSY::CLSYLibDialogBase
{
public:
	LSY::CEdit			*m_pEditName;
public:
	CBuddyGroupAddDlg(CWnd* pParent = NULL);
	virtual ~CBuddyGroupAddDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnReturn(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(CWnd *pParent);

	bool OnAddGroup(const std::string &strName);
protected:
	DECLARE_MESSAGE_MAP()
};