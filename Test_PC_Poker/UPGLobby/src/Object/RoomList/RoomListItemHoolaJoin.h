#pragma once


class UPG_LOBBY_EXPORT CRoomListItemHoolaJoin : public LSY::CListItem
{

protected:
	LSY::CTextNumber			*m_pRoomNumber;
	//LSY::CText					*m_pGameStyle;
	LSY::CText					*m_pMinmumMoney;
	LSY::CText					*m_pTitle;
	LSY::CTextNumber			*m_pSeedMoney;
	
//	LSY::CText					*m_pRule;
	LSY::CText					*m_pStopMode;
	LSY::CText					*m_pJokerMode;

	LSY::CTextNumber			*m_pPlayerCnt;
	//	std::vector<LSY::CImage *>	m_listUserCount;
	//	LSY::CTextNumber			*m_pTatalMoney;
	LSY::CImage					*m_pImg_SeedMoney;
	//LSY::CImage					*m_pImg_Gamestyle;
	LSY::CImage					*m_pImg_Mode;
	LSY::CImage					*m_pImg_FullUser;
	LSY::CButton				*m_pBtnEnterRoom;
	LSY::CButton				*m_pBtnObserberRoom;
	LSY::CText					*m_pTitle_PrevRoom;		//[������] ǥ��
	LSY::CText					*m_pTitle_PrevRoomTitle; //�������� ��� Ÿ��Ʋ
public:
	CRoomListItemHoolaJoin();
	virtual ~CRoomListItemHoolaJoin();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);

	virtual void OnMouseOver(void);
	virtual void OnMouseLeave(void);

	void SetBarType(int nStartIndex);
	//void SetUserCount(int nUserCount, bool bJoinable);


	friend class CRoomListHeaderHoola;
};
