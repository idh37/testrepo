#pragma once

class UPG_COMMON_EXPORT CMsgBoxDlg : public LSY::CLSYLibDialogBase
{
public:
	enum ENUM_MSGBOX_TYPE
	{
		EMBT_NOTICE = 0,	//�Ϲ� �˸� �޽���
		EMBT_QUESTION,		//����
		EMBT_ENTERROOM,		//�濡 ���� ����
		EMBT_CREATEROOM,	//���ù� ���� ����
		EMBT_CONFORMREFUSE,	//����, ����
		EMBT_OKCANCEL,		//Ȯ��, ���
		EMBT_EXIT,			//������, ���
		EMBT_GIVEUP,		//����ϱ�, ���
	};
	bool						m_bTimerMessage;
	int							m_nTimerCancel;
protected:
	std::list<std::string>		m_listMessage;
	bool						m_bClearMessage;
	ENUM_MSGBOX_TYPE			m_nType;
	LSY::CImage					*m_pImgBack;
	LSY::CText					*m_pTextMessage;
public:
	CMsgBoxDlg(CWnd* pParent = NULL);   // standard constructor
public:
	void SetTimerCancel(int nTime);
public:
	virtual INT_PTR DoModal(const ENUM_MSGBOX_TYPE &nType, LPCTSTR strMsg, int nTimerCancel = 0);
	virtual bool OnCreatedProject(void);
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	
	void ClearMessage(void);
	void CloseDlg(void);
public:
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnSetTextLineCount(LSY::CMessage *pMsg);

	DECLARE_MESSAGE_MAP()
};