#pragma once

#include "DataList.h"
#include "UserStruct.h"

#define DEF_USERLIST_SORT_UNKNOWN	-1
#define DEF_USERLIST_SORT_NICKNAME	0
#define DEF_USERLIST_SORT_HAVEMONEY	1
#define DEF_USERLIST_SORT_POSITION	2

UPG_COMMON_EXPORT extern bool UserListNickNameCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam);
UPG_COMMON_EXPORT extern bool UserListHaveMoneyCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam);
UPG_COMMON_EXPORT extern bool UserListPositionCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam);

class UPG_COMMON_EXPORT CDataUserList :public CFilterDataList<CString, SMALLUSERINFO>
{
public:
	CDataUserList(void);
	virtual ~CDataUserList(void);

	CDataUserList::KEY_FILTER m_GameFilterKey;		//게임별 구분

	CDataUserList::KEY_FILTER MakeGameFilter(IDX_GAME idxGame);
	CDataUserList::KEY_FILTER GetGameFilter();

	void SortBy_Nickname(BOOL bAscending);
	static bool CompareBy_Nickname_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_Nickname_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_Money(BOOL bAscending);
	static bool CompareBy_Money_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_Money_Desc(DATAENTITY* a, DATAENTITY* b);

	void SortBy_Position(BOOL bAscending);
	static bool CompareBy_Position_Asc(DATAENTITY* a, DATAENTITY* b);
	static bool CompareBy_Position_Desc(DATAENTITY* a, DATAENTITY* b);


	void Sort_Default();	///디폴트 정렬
	static bool Compare_Default(DATAENTITY* a, DATAENTITY* b);

	SMALLUSERINFO *FindData(const std::string &strID);
};
