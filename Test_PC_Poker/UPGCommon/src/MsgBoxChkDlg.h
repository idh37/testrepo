#pragma once

class UPG_COMMON_EXPORT CMsgBoxChkDlg : public LSY::CLSYLibDialogBase
{
public:
	enum ENUM_MSGBOX_TYPE
	{
		EMBT_NOTICE = 0, // �Ϲ� �˸� �޽���
		EMBT_NOCHECK, // üũ�ڽ� ����
	};

protected:
	LSY::CImage *m_pImgBack;
	LSY::CText *m_pTextMsg;
	LSY::CCheckBox *m_pChkAgree;

	ENUM_MSGBOX_TYPE m_nType;
	std::string m_strMsg;
	bool m_bChecked;

public:
	CMsgBoxChkDlg(CWnd* pParent = NULL); // standard constructor

public:
	virtual INT_PTR DoModal(LPCTSTR strMsg, int nType);
	virtual bool OnCreatedProject(void);

	virtual void OnOK();
	virtual void OnCancel();
	
	void CloseDlg(void);

public:
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnCheckChange(LSY::CMessage *pMsg);
	LRESULT OnSetTextLineCount(LSY::CMessage *pMsg);
};