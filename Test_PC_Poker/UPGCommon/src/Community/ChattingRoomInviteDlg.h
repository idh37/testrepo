#pragma once

class UPG_COMMON_EXPORT CChattingRoomInviteDlg : public LSY::CLSYLibDialogBase
{
protected:
	std::string				m_strID;
	std::string				m_strRoomID;

	DWORD					m_dwStartTime;
	DWORD					m_dwEndTime;
	LSY::CProgressiveBar	*m_pProg;
	bool					m_bFirst;
public:
	CChattingRoomInviteDlg(CWnd* pParent = NULL);
	virtual ~CChattingRoomInviteDlg(){}

	void Create(CWnd *pParent, const std::string &strRoomID, const std::string &strSenderID);
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
	virtual void OnOK();

	virtual bool OnCreatedProject(void);
	LRESULT OnClickButton(LSY::CMessage *pMsg);

	virtual const std::string &GetRoomID(void){return m_strRoomID;}
	virtual const std::string &GetSender(void){return m_strID;}

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
protected:
	DECLARE_MESSAGE_MAP()
};