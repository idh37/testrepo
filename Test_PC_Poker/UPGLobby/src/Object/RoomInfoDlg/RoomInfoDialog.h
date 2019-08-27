#pragma once

class UPG_LOBBY_EXPORT CRoomInfoDialog : public LSY::CWindows
{
protected:
	LSY::CText			*m_pGameName;
	LSY::CTextNumber	*m_pRoomNumber;
	LSY::CText			*m_pRoomName;
	LSY::CText			*m_pMinimumMoney;
	LSY::CText			*m_pGameMode;
	LSY::CTextNumber	*m_pSeedMoney;

	LSY::CImage			*m_pGameBack;

	LSY::CTabControl	*m_pTabUserType;

	LSY::CImage			*m_pScrollBack;
	LSY::CList			*m_pUserList;
	LSY::CButton		*m_pBtnEnterRoom;
	LSY::CButton		*m_pBtnObserverEnter;
	LSY::CButton		*m_pBtnClose;

	LSY::CTextNumber	*m_pTotalMoney;
	LSY::CImage			*m_pImgVIP;
	LSY::CImage			*m_pImgFullUser;

	LSY::CText			*m_pGameType;

	std::vector<LSY::CImage *>	m_listUser;

	GAMEINFO			m_stGameInfo;
	bool				m_bPlayerList;
public:
	CRoomInfoDialog();
	virtual ~CRoomInfoDialog();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	virtual bool OnClickOutside(CObjectBase *pFocus);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnTabSelChange(LSY::CMessage *msg);
	LRESULT OnListItemDbClick(LSY::CMessage *msg);

	void ShowGameRoomInfo(GAMEINFO *pGI);
	void SetUserListType(bool bPlayUser);
};