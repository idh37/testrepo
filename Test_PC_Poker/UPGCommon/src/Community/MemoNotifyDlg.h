#pragma once

class UPG_COMMON_EXPORT CMemoNotifyDlg : public LSY::CLSYLibDialogBase
{
public:
	enum ENUM_MEMONOTIFY_TYPE
	{
		EMNT_MEMO_DELETE = 0,
		EMNT_MEMO_STORAGE_FULL,
		EMNT_MEMO_MOVE_STORAGE,
	};
protected:
	LSY::CButton			*m_pBtnDelete;
	LSY::CButton			*m_pBtnCancel;
	LSY::CButton			*m_pBtnOK;
	LSY::CWindows			*m_pBack;

	ENUM_MEMONOTIFY_TYPE	m_nType;
public:
	CMemoNotifyDlg(CWnd* pParent = NULL);
	virtual ~CMemoNotifyDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal(CWnd *pParent, const ENUM_MEMONOTIFY_TYPE &nType);
protected:
	DECLARE_MESSAGE_MAP()
};