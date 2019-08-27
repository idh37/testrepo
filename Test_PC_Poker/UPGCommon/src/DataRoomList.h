#pragma once
#include "DataList.h"
#include "UserStruct.h"

enum ROOMLIST_TYPE{ ROOMLIST_TYPE_ALL, ROOMLIST_TYPE_VIP, ROOMLIST_TYPE_NORMAL, ROOMLIST_TYPE_VIP_GOLD, ROOMLIST_TYPE_NORMAL_GOLD, ROOMLIST_TYPE_FREE, MAX_ROOMLIST_TYPE };

enum SORT_FILTER_TYPE{ FILTER_TYPE_ALL, FILTER_TYPE_NORMAL, FILTER_TYPE_VIP, FILTER_TYPE_FREE,  FILTER_ALL_JOINABLE, FILTER_NORMAL_JOINABLE, FILTER_VIP_JOINABLE, MAX_FILTER_TYPE };

class UPG_COMMON_EXPORT CDataRoomList :public CFilterDataList<UINT64, SMALLROOMINFO>
{
	
public:
	CDataRoomList(void);
	virtual ~CDataRoomList(void);

	static INT64 m_llEnterLimitMoney_forSort;	

	KEY_FILTER m_keyFilter[MAX_FILTER_TYPE];

	void Initialize();

	CDataRoomList::DATAENTITY *InsertRoom(SMALLROOMINFO *pSRI);		//방을 올바른 위치에 인서트 시킨다. 
	
	KEY_FILTER GetRoomFilterType( bool bJoinable, ROOMLIST_TYPE eRoomListType );	

	ROOMLIST_TYPE GetRoomListType(INT64 llEnterLimitMoney);	//llEnterLimitMoney에따른 룸리스트 타입을 가져온다.
	BOOL DoesExistEnterLimitMoney(INT64 llEnterLimitMoney);	//

	static bool Filter_VIPRoom(SMALLROOMINFO *pSRI);
	static bool Filter_NormalRoom(SMALLROOMINFO *pSRI);
	static bool Filter_FreeRoom(SMALLROOMINFO *pSRI);
	static bool Filter_VIPRoom_Joinable( SMALLROOMINFO *pSRI );
	static bool Filter_NormalRoom_Joinable( SMALLROOMINFO *pSRI );
	static bool Filter_AllRoom_Joinable( SMALLROOMINFO *pSRI );

	
	void SortBy_No(BOOL bAscending);
	static bool CompareBy_No_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_No_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_Limit(BOOL bAscending);
	static bool CompareBy_Limit_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_Limit_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_SeedBetRule(BOOL bAscending);
	static bool CompareBy_SeedBetRule_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_SeedBetRule_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_Title(BOOL bAscending);
	static bool CompareBy_Title_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_Title_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_FormKind(BOOL bAscending);
	static bool CompareBy_FormKind_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_FormKind_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_PlayerCnt(BOOL bAscending);
	static bool CompareBy_PlayerCnt_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_PlayerCnt_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_TotalMoney(BOOL bAscending);
	static bool CompareBy_TotalMoney_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_TotalMoney_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_State(BOOL bAscending);
	static bool CompareBy_State_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_State_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_RuleType(BOOL bAscending);
	static bool CompareBy_RuleType_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_RuleType_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_EnterLimitMoney(INT64 llEnterLimitMoney);
	static bool CompareBy_EnterLimitMoney(DATAENTITY* a, DATAENTITY* b);	

	void Sort_Default();	///디폴트 정렬
	static bool Compare_Default(DATAENTITY* a, DATAENTITY* b);

};
