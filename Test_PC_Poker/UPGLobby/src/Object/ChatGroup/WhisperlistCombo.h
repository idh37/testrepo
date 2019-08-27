#pragma once

class CChatGroup;
class UPG_LOBBY_EXPORT CWhisperlistCombo : public LSY::CCombo
{
protected:
	bool				m_bCloseCombo;
	LSY::CTabControl	*m_pTab;
	LSY::CTextList		*m_pTxtListAllUser;
	LSY::CTextList		*m_pTxtListLately;
	LSY::CTextList		*m_pTxtListConnectBuddy;
	CChatGroup			*m_pChatGroup;
	LSY::CImage			*m_pConnectBuddyInfo;
	LSY::CImage			*m_pConnectBuddyInfoPopup;
	LSY::CRadio			*m_pRadioConnectBuddy;
public:
	CWhisperlistCombo(const bool &bCloseCombo, CChatGroup *pChatGroup);
	virtual ~CWhisperlistCombo();

	virtual void OnCreated(void);
	virtual void OnCreatedClose(void);
	virtual void OnCreatedOpen(void);

	LRESULT OnSelChangeList(LSY::CMessage *msg);
	virtual LRESULT OnClickButton(LSY::CMessage *pMsg);
	virtual LRESULT OnEditReturn(LSY::CMessage *pMsg);

	LRESULT OnInfoMouseOver(LSY::CMessage *msg);
	LRESULT OnInfoMouseLeave(LSY::CMessage *msg);

	LRESULT OnPopupLButtonDown(LSY::CMessage *msg);
	LRESULT OnPopupLButtonUp(LSY::CMessage *msg);
	LRESULT OnPopupDBClick(LSY::CMessage *msg);
	LRESULT OnPopupLButtonClick(LSY::CMessage *msg);

	virtual void AddWhisper(const std::string &strWhisper, const bool &bSetCurWhisper, const bool &bStartAnimation);
	void SetAllUser(void);
	bool IsAllUser(void);
};