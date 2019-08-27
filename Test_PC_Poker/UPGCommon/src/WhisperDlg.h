#pragma once

class UPG_COMMON_EXPORT CWhisperDlg : public LSY::CLSYLibDialogBase
{
protected:
	UINT64			m_llTargetUKey;
	CString			m_strNickName;
	CString			m_strID;

	LSY::CEdit		*m_pWhisperEdit;
	LSY::CText		*m_pNickName;
public:
	CWhisperDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWhisperDlg(){}

	virtual bool OnCreatedProject(void);

	virtual INT_PTR DoModal(const char *pNickname, const char *pID, UINT64 llUserKey, CWnd *pParent);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnEditReturn(LSY::CMessage *pMsg);
public:
	void SendWhisper(void);

protected:
	DECLARE_MESSAGE_MAP()
};