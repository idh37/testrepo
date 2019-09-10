#pragma once

class UPG_COMMON_EXPORT CBlackBuddyAddDlg : public LSY::CLSYLibDialogBase
{
public:
	enum BLACKBUDDYADDDLGTYPE
	{
		BBADT_EDIT = 0, //������Ʈ�� ����Ʈ�� �Է��ϴ� ���̾˷α�
		BBADT_QUESTION, //������Ʈ�� ������� ���θ� ����� ���̾˷α�
		BBADT_MESSAGE,	//������Ʈ �Ϲ� �޽��� ��� ���̾˷α�
	};

	LSY::CWindows		*m_pWndBack;
	LSY::CText			*m_pTxtResult;
	LSY::CEdit			*m_pEditBlackBuddyIDorNickName;
	LSY::CButton		*m_pBtnAddBlackBuddy;
	LSY::CButton		*m_pBtnCancelBlackBuddy;
	LSY::CButton		*m_pBtnClose;
	LSY::CButton		*m_pBtnOK;
	LSY::CButton		*m_pBtnCancel;

	BLACKBUDDYADDDLGTYPE	m_nStartType;
	BLACKBUDDYADDDLGTYPE	m_nCurType;
	std::string				m_strMessage;
public:
	CBlackBuddyAddDlg(CWnd* pParent = NULL);
	virtual ~CBlackBuddyAddDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(BLACKBUDDYADDDLGTYPE nType, std::string strMessage, CWnd *pParent);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool OnAddGroup(const std::string &strName);
protected:
	DECLARE_MESSAGE_MAP()

	void SetCurType(BLACKBUDDYADDDLGTYPE nType);
};