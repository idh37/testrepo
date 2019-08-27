#pragma once

class UPG_COMMON_EXPORT CReceiveAdminMessageDlg : public LSY::CLSYLibDialogBase
{
public:
	Camel::Dar2::SS::Memo				m_cMemo;
public:
	CReceiveAdminMessageDlg(CWnd* pParent = NULL);
	virtual ~CReceiveAdminMessageDlg(){}

	virtual bool OnCreatedProject(void);

	afx_msg void OnDestroy();

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(Camel::Dar2::SS::Memo *pMemo, CWnd *pParent);
	virtual void ChangeMemo(Camel::Dar2::SS::Memo *pMemo);
protected:
	DECLARE_MESSAGE_MAP()
};