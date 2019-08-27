#pragma once

class UPG_LOBBY_EXPORT CRoomListHeaderHoola : public LSY::CGroup
{
protected:
	LSY::CTabControl		*m_pTabRoomType;
	LSY::CTextButton		*m_pBtnRoomNumber;
	LSY::CTextButton		*m_pBtnTitle;
	LSY::CTextButton		*m_pBtnMinimumMoney;
	LSY::CTextButton		*m_pBtnSeedMoney;
	LSY::CTextButton 		*m_pBtnGameMode;
	LSY::CTextButton 		*m_pBtnRuleType;


	//LSY::CTextButton		*m_pBtnGameMode;
	LSY::CTextButton		*m_pBtnUserCount;
	//LSY::CTextButton		*m_pBtnRoomTotalMoney;
	LSY::CTextButton		*m_pBtnDefaultSort;
	LSY::CList				*m_pList;

	int						m_nCurSortType;
	std::map<int, bool>		m_mapSortTypeIncrement;
	int						m_nCurRoomType;

	bool					m_bJoinable;
public:
	CRoomListHeaderHoola();
	virtual ~CRoomListHeaderHoola();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnRoomTypeSelChange(LSY::CMessage *msg);

	LRESULT OnListItemLClick(LSY::CMessage *msg);
	LRESULT OnListItemLDbClick(LSY::CMessage *msg);

	LRESULT OnClickEnterRoom(LSY::CMessage *msg);
	LRESULT OnClickObserverRoom(LSY::CMessage *msg);

	void RemoveAll(void);
	void AddItem(SMALLROOMINFO *pRoomInfo);
	void DelItem(SMALLROOMINFO *pRoomInfo);
	void ChangeItem(SMALLROOMINFO *pRoomInfo);

	void ChangeRoomTab( ROOMLIST_TYPE eRoomListType );
	void SetListEnable(bool bEnable);

	void SetJoinable( bool bstate );
	void AddDataRoomList();
	void RemoveSelectedRoom(void);
	void ChangeRoomList(void);
protected:
	void Sort(int nSortType);
	void SortDefault(void);
	void SortEnterRoomMinimumMoney(INT64 *i64Money);


	friend class CRoomListGroup;
};
