#pragma once

class CWhisperlistCombo;
class UPG_LOBBY_EXPORT CChatGroup : public LSY::CGroup
{
protected:
	LSY::CGroup			*m_pGrpClose;
	LSY::CEdit			*m_pEditClose;
	LSY::CButton		*m_pBtnOpen;
	CWhisperlistCombo	*m_pComboClose;
	LSY::CSprite		*m_pSprReceiveChat;

	LSY::CImage			*m_pImgBack;
	LSY::CButton		*m_pBtnClose;
	LSY::CEdit			*m_pEditOpen;
	LSY::CRichText		*m_pChatList;
	CWhisperlistCombo	*m_pComboOpen;

	LSY::CButton		*m_pBtn112;

	bool				m_bFirst;
	bool				m_bOpen;
public:
	CChatGroup();
	virtual ~CChatGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);

	virtual void AddWhisper(const std::string &strWhisper, const bool &bSetCurWhisper, const bool &bStartAnimation, const bool &bAllUser);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnEditFocusIn(LSY::CMessage *msg);
	LRESULT OnEditReturn(LSY::CMessage *msg);

	void RaiseChatView(bool bShow);
	void EnableChat(bool bEnable);
	void SetFocusChat(void);
	void ClearChatHistory(void);

	void StartRecevieChatAnimation(void);
	void ShowRoomList(bool bShow);

	void AddText(const char *pMsg, COLORREF color);
	bool CheckSendChat(IN OUT std::string &strText, OUT std::string &strWhisperUserID, OUT bool &bAllChat);
	std::string GetWhisperUserID(IN const std::string &strText, OUT bool &bAllUser);
	
protected:
	NMBASE::UTIL::CNoChat	m_PaperingChat;
	
	bool CheckPapering( char* szBuf );
};