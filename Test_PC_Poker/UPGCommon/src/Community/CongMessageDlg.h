#pragma once

class UPG_COMMON_EXPORT CCongMessageDlg : public LSY::CLSYLibDialogBase
{
protected:
	LSY::CButton		*m_pBtnSend;
	LSY::CButton		*m_pBtnCancel;
	LSY::CText			*m_pTextID;
	LSY::CEdit			*m_pEdit;

	std::string			m_id; 
public:
	CCongMessageDlg(CWnd* pParent = NULL);
	virtual ~CCongMessageDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal(CWnd *pParent, std::string id);
protected:
	DECLARE_MESSAGE_MAP()
};