#pragma once

class CChattingPopupMemu;

class UPG_COMMON_EXPORT CChattingRoomDlg : public LSY::CLSYLibWindowsBase
{
protected:
	LSY::CAvatar		*m_pAvatarYou;
	LSY::CAvatar		*m_pAvatarMy;
	LSY::CTextList		*m_pUserList;
	LSY::CRichText		*m_pChattingList;
	LSY::CEdit			*m_pEdit;
	LSY::CText			*m_pLastMessageTime;

	std::string			m_strRoomID;
	std::string			m_strID;
	bool				m_bMaster;
	bool				m_bDisconnect;
	bool				m_bSendClose;

	std::string			m_strSelectID;
	std::string			m_strSelectNickName;

	class CChatData
	{
	protected:
		std::string		m_strID;
		std::string		m_strNickName;
		std::string		m_strChatting;
		int				m_nLen;
	public:
		CChatData(const std::string &strID, const std::string &strNick, const std::string &strMessage);
		void CalculationLen(void);

		std::string			GetText(void);
		const int			&GetLength(void){return m_nLen;}
		const std::string	&GetID(void){return m_strID;}
		const std::string	&GetNickName(void){return m_strNickName;}
	};

	std::list<CChatData>	m_listChat;
	std::list<std::string>	m_listInviateID;
	int						m_nChatLength;
	CChattingPopupMemu		*m_pPopupMenu;
public:
	CChattingRoomDlg();
	virtual ~CChattingRoomDlg();

	virtual bool OnCreatedProject(void);
	bool Create(CWnd *pParent, const std::string &strRoomID, const std::list<std::string> &listInviteID, const bool &bMaster);

	void AddUser(CChatUser *pUser);
	const std::string &GetID(void){return m_strID;}

	void SendInviteRoom(void);
	void OnJoinFaileRoom(void);
	void OnJoinSuccessRoom(void);
	void OnEnterRoom(CChatUser *pUser);
	void OnLeaveRoom(const std::string &strLeaveID);
	void OnSendFaileChat(const std::string &strMessage);
	void OnReceiveChat(const std::string &strID, const std::string &strMessage);
	void OnInvateFaileUser(const std::string &strID);
	void DisconnectDisable(void);

	void CheckChangeCaption(void);

	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	void SendChat(void);

	const std::string &GetRoomID(void){return m_strRoomID;}

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnReturn(LSY::CMessage *pMsg);
	LRESULT OnUserListRClick(LSY::CMessage *pMsg);

	void SendInviteCahtRoom(std::string strID, std::string strNickName, bool bSendResult, bool bReceiveResult);
protected:
	void CheckMaxNotifyMsg(void);
	void AddChatData(CChatUser *pUser, const std::string &strMessage);
	void OnNotifyUser(void);

	CChatUser *FindUserFromNickName(const std::string &strNIckName);
	CChatUser *FindUserFromID(const std::string &strID);
	int	GetNickNameToListIndex(const std::string &strNickName);
	int	GetIDToListIndex(const std::string &strID);
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};