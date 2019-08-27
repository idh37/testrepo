#pragma once

class CRoomListHeader;
class CRoomListHeaderLowBD;
class CRoomListHeaderHoola;
class CRoomListHeaderJoin;
class CRoomListHeaderLowBDJoin;
class CRoomListHeaderHoolaJoin;

class UPG_LOBBY_EXPORT CRoomListGroup : public LSY::CGroup
{
protected:
	CRoomListHeader				*m_pHeader;
	CRoomListHeaderLowBD		*m_pHeaderLowBD;
	CRoomListHeaderHoola		*m_pHeaderHoola;
	CRoomListHeaderJoin			*m_pHeaderJoin;
	CRoomListHeaderLowBDJoin	*m_pHeaderLowBDJoin;
	CRoomListHeaderHoolaJoin	*m_pHeaderHoolaJoin;

	LSY::CSprite				*m_pSprLoading;
	INT64						m_i64EnterRoomMinimumMoney;

	
private:
	LSY::CCheckButton			*m_pJoinable_ckbtn;

public:
	CRoomListGroup();
	virtual ~CRoomListGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	void ShowLoadingRoom(bool bShow);
	void RemoveAll(void);
	void AddItem(SMALLROOMINFO *pRoomInfo);
	void DelItem(SMALLROOMINFO *pRoomInfo);
	void ChangeItem(SMALLROOMINFO *pRoomInfo);
	void ChangeRoomListState(void);

	void SetGameType(IDX_GAME nType);
	void ChangeRoomTab( ROOMLIST_TYPE eRoomListType );
	void ChangeGoldTabState(bool bGold, int tabIndex);

	void SortDefault(void);
	void SortEnterRoomMinimumMoney(INT64 i64Money);

	LRESULT OnRoomTypeSelChange(LSY::CMessage *msg);


	void RemoveSelectedRoom(void);		//선택방을 지운다.
};