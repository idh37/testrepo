#pragma once

class UPG_COMMON_EXPORT CBuddyRemoveDlg : public LSY::CLSYLibDialogBase
{
public:
	enum BUDDYREMOVEDLGTYPE
	{
		BRDT_QUESTION = 0, //블랙리스트에 삭제할지 여부를 물어보는 다이알로그
		BRDT_MESSAGE,	//블랙리스트 일반 메시지 출력 다이알로그
	};

	LSY::CButton		*m_pBtnClose;
	LSY::CButton		*m_pBtnOK;
	LSY::CButton		*m_pBtnCancel;
	LSY::CText			*m_pTxtResult;

	BUDDYREMOVEDLGTYPE	m_nStartType;
	BUDDYREMOVEDLGTYPE	m_nCurType;
	std::string			m_strMessage;
public:
	CBuddyRemoveDlg(CWnd* pParent = NULL);
	virtual ~CBuddyRemoveDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(BUDDYREMOVEDLGTYPE nType, std::string strMessage, CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()

	void SetCurType(BUDDYREMOVEDLGTYPE nType);
};