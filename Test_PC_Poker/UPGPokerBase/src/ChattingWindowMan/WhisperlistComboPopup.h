#pragma once
#include "WhisperlistComboPopupInfo.h"
#define _DEF_GAME_WHISPERLIST_ALL_USER "일반대화"

class UPG_GAMEBASE_EXPORT CWhisperlistComboPopup : public LSY::CLSYLibWindowsBase
{
protected:
	DECLARE_DYNAMIC(CWhisperlistComboPopup)
	DECLARE_DISPATCH_MAP()
	DECLARE_MESSAGE_MAP()

protected:
	LSY::CTabControl			*m_pTab;
	LSY::CTextList				*m_pTxtListAllUser;
	LSY::CTextList				*m_pTxtListLately;
	LSY::CTextList				*m_pTxtListConnectBuddy;
	LSY::CImage					*m_pConnectBuddyInfo;
	LSY::CRadio					*m_pRadioConnectBuddy;

	std::string					m_strText;
	CRect						m_rtRect;
	bool						m_bNotHideKillFocus;
	CWhisperlistComboPopupInfo	m_cPopup;
public:
	CWhisperlistComboPopup();
	virtual ~CWhisperlistComboPopup();

	bool Create(int x, int y);
	void SetShow(bool bShow);
	bool IsShow(void);

	LRESULT OnSelChangeList(LSY::CMessage *msg);
	LRESULT OnInfoMouseOver(LSY::CMessage *msg);
	LRESULT OnInfoMouseLeave(LSY::CMessage *msg);
	LRESULT OnClickList(LSY::CMessage *msg);

	LRESULT OnPopupLButtonDown(LSY::CMessage *msg);
	LRESULT OnPopupLButtonUp(LSY::CMessage *msg);
	LRESULT OnPopupDBClick(LSY::CMessage *msg);
	LRESULT OnPopupLButtonClick(LSY::CMessage *msg);

	LRESULT OnAddWhisper(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnSelChangeChatList(LSY::CMessage *msg);

	virtual bool OnCreatedProject(void);

	void SetText(const std::string &strText);
	const std::string &GetText(void){return m_strText;}

	virtual void AddWhisper(const std::string &strWhisper, const bool &bSetCurWhisper, const bool &bStartAnimation);
	void SetAllUser(void);
	bool IsAllUser(void);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};