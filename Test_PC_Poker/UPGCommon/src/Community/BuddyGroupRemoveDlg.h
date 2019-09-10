#pragma once

class UPG_COMMON_EXPORT CBuddyGroupRemoveDlg : public LSY::CLSYLibDialogBase
{
public:
	enum BUDDYGROUPREMOVEDLGTYPE
	{
		BGRDT_QUESTION = 0, //������Ʈ�� �������� ���θ� ����� ���̾˷α�
		BGRDT_MESSAGE,	//������Ʈ �Ϲ� �޽��� ��� ���̾˷α�
	};

	LSY::CText			*m_pTxtResult;
	LSY::CButton		*m_pBtnClose;
	LSY::CButton		*m_pBtnOK;
	LSY::CButton		*m_pBtnCancel;

	BUDDYGROUPREMOVEDLGTYPE	m_nStartType;
	BUDDYGROUPREMOVEDLGTYPE	m_nCurType;
	std::string				m_strMessage;
public:
	CBuddyGroupRemoveDlg(CWnd* pParent = NULL);
	virtual ~CBuddyGroupRemoveDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(BUDDYGROUPREMOVEDLGTYPE nType, std::string strMessage, CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()

	void SetCurType(BUDDYGROUPREMOVEDLGTYPE nType);
};