#pragma once

class UPG_COMMON_EXPORT CBuddyMoveGroupDlg : public LSY::CLSYLibDialogBase
{
public:
	enum BUDDYMOVEGROUPDLGTYPE
	{
		BMGDT_EDIT = 0, //이동할 그룹 선택창
		BMGDT_MESSAGE,	//그룹이동 결과창
	};

	LSY::CWindows		*m_pWndBack;
	LSY::CText			*m_pTxtResult;
	LSY::CCombo			*m_pComboGroup;
	LSY::CButton		*m_pBtnClose;
	LSY::CButton		*m_pBtnOK;
	LSY::CButton		*m_pBtnCancel;

	BUDDYMOVEGROUPDLGTYPE	m_nType;

	std::string				m_strMessage;
	std::string				m_strID;
public:
	CBuddyMoveGroupDlg(CWnd* pParent = NULL);
	virtual ~CBuddyMoveGroupDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(std::string strMessage, CWnd *pParent);
	virtual INT_PTR DoModal(std::string strID, std::string strGroup, CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
};