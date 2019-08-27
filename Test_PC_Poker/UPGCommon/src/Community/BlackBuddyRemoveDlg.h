#pragma once

class UPG_COMMON_EXPORT CBlackBuddyRemoveDlg : public LSY::CLSYLibDialogBase
{
public:
	enum BLACKBUDDYREMOVEDLGTYPE
	{
		BBRDT_QUESTION = 0, //블랙리스트에 삭제할지 여부를 물어보는 다이알로그
		BBRDT_MESSAGE,	//블랙리스트 일반 메시지 출력 다이알로그
	};

	LSY::CText			*m_pTxtResult;
	LSY::CButton		*m_pBtnClose;
	LSY::CButton		*m_pBtnOK;
	LSY::CButton		*m_pBtnCancel;

	BLACKBUDDYREMOVEDLGTYPE	m_nStartType;
	BLACKBUDDYREMOVEDLGTYPE	m_nCurType;
	std::string				m_strMessage;
public:
	CBlackBuddyRemoveDlg(CWnd* pParent = NULL);
	virtual ~CBlackBuddyRemoveDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(BLACKBUDDYREMOVEDLGTYPE nType, std::string strMessage, CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()

	void SetCurType(BLACKBUDDYREMOVEDLGTYPE nType);
};