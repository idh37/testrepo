#include "StdAfx.h"
#include "DataUserList.h"

bool UserListNickNameCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLUSERINFO *pInfo1 = (SMALLUSERINFO *)pItem1->GetItemData();
	SMALLUSERINFO *pInfo2 = (SMALLUSERINFO *)pItem2->GetItemData();

	if(bIncrement)
		return strcmp(pInfo1->NickName, pInfo2->NickName) < 0;
	else
		return strcmp(pInfo1->NickName, pInfo2->NickName) > 0;
}

bool UserListHaveMoneyCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLUSERINFO *pInfo1 = (SMALLUSERINFO *)pItem1->GetItemData();
	SMALLUSERINFO *pInfo2 = (SMALLUSERINFO *)pItem2->GetItemData();

	if(bIncrement)
		return pInfo1->PMoney < pInfo2->PMoney;
	else
		return pInfo1->PMoney > pInfo2->PMoney;
}

bool UserListPositionCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLUSERINFO *pInfo1 = (SMALLUSERINFO *)pItem1->GetItemData();
	SMALLUSERINFO *pInfo2 = (SMALLUSERINFO *)pItem2->GetItemData();

	if(bIncrement)
	{
		if ( pInfo1->nGameIndex == pInfo2->nGameIndex )
		{
			return (pInfo1->RoomNum < pInfo2->RoomNum);
		}
		else if (pInfo1->nGameIndex != IDX_GAME_BD && pInfo2->nGameIndex == IDX_GAME_BD)
			return false;
		else if (pInfo1->nGameIndex == IDX_GAME_BD && pInfo2->nGameIndex != IDX_GAME_BD)
			return true;
		else
			return (pInfo1->nGameIndex < pInfo2->nGameIndex);
	}
	else
	{
		if ( pInfo2->nGameIndex == pInfo1->nGameIndex )
		{
			return (pInfo2->RoomNum < pInfo1->RoomNum);
		}
		else if (pInfo2->nGameIndex != IDX_GAME_BD && pInfo1->nGameIndex == IDX_GAME_BD)
			return false;
		else if (pInfo2->nGameIndex == IDX_GAME_BD && pInfo1->nGameIndex != IDX_GAME_BD)
			return true;
		else
			return (pInfo2->nGameIndex < pInfo1->nGameIndex);
	}
}

CDataUserList::CDataUserList(void)
{
	m_GameFilterKey = KEY_FILTER_NULL;
}

CDataUserList::~CDataUserList(void)
{
}


void CDataUserList::SortBy_Nickname(BOOL bAscending)
{
	if (bAscending)
		Sort(CompareBy_Nickname_Asc);
	else
		Sort(CompareBy_Nickname_Desc);
}

void CDataUserList::SortBy_Money(BOOL bAscending)
{
	if (bAscending)
		Sort(CompareBy_Money_Asc);
	else
		Sort(CompareBy_Money_Desc);
}

void CDataUserList::SortBy_Position(BOOL bAscending)
{
	if (bAscending)
		Sort(CompareBy_Position_Asc);
	else
		Sort(CompareBy_Position_Desc);
}


void CDataUserList::Sort_Default()	///디폴트 정렬
{
	Sort(Compare_Default);
}


//////////////////////////////////비교 함수 /////////////////////////////



bool CDataUserList::CompareBy_Nickname_Asc(DATAENTITY* a, DATAENTITY* b)
{
	return strcmp(a->Data.NickName, b->Data.NickName) > 0;

}
bool CDataUserList::CompareBy_Nickname_Desc(DATAENTITY* a, DATAENTITY* b)
{
	return CompareBy_Nickname_Asc(b,a);
}

bool CDataUserList::CompareBy_Money_Asc(DATAENTITY* a, DATAENTITY* b)
{
	return a->Data.PMoney< b->Data.PMoney;
}

bool CDataUserList::CompareBy_Money_Desc(DATAENTITY* a, DATAENTITY* b)
{
	return CompareBy_Money_Asc(b,a);
}

bool CDataUserList::CompareBy_Position_Asc(DATAENTITY* a, DATAENTITY* b)
{
	return CompareBy_Position_Desc(b,a);
}
bool CDataUserList::CompareBy_Position_Desc(DATAENTITY* a, DATAENTITY* b)
{
	//게임: 로우바둑이, 7포커, 뉴포커, 맞포커 순으로 노출
	if ( a->Data.nGameIndex == b->Data.nGameIndex )
	{
		return (a->Data.RoomNum < b->Data.RoomNum);
	}
	else if (a->Data.nGameIndex != IDX_GAME_BD && b->Data.nGameIndex == IDX_GAME_BD)
		return false;
	else if (a->Data.nGameIndex == IDX_GAME_BD && b->Data.nGameIndex != IDX_GAME_BD)
		return true;
	else
		return (a->Data.nGameIndex < b->Data.nGameIndex);
	
}

bool CDataUserList::Compare_Default(DATAENTITY* a, DATAENTITY* b)
{
	//유저 디폴트 정렬 : 게임, 보유머니구간을 만족하는 사용자를 접속게임 구분없이 보유머니 내림차순으로 정렬
	return CompareBy_Money_Desc(a, b);
}



class CCompareGame
{
public:
	CCompareGame(const IDX_GAME& idxGame)
	{
		m_idxGmae = idxGame;  
	}

	bool operator()(SMALLUSERINFO* pSUI)
	{
		return (pSUI->nGameIndex == m_idxGmae);
	} 

	IDX_GAME m_idxGmae;
};

IDX_GAME g_idxGame_To_Compare=IDX_GAME_NULL;
bool CompareGame(SMALLUSERINFO *pSUI)
{
	return (pSUI->nGameIndex== g_idxGame_To_Compare);
}

CDataUserList::KEY_FILTER CDataUserList::MakeGameFilter(IDX_GAME idxGame)
{
	g_idxGame_To_Compare = idxGame;
	m_GameFilterKey  =	AddFilter(CompareGame);

	return m_GameFilterKey;
	
}

CDataUserList::KEY_FILTER CDataUserList::GetGameFilter()
{

	return m_GameFilterKey;

}

SMALLUSERINFO *CDataUserList::FindData(const std::string &strID)
{
	CDataUserList::DATAPOSITION pos;
	SMALLUSERINFO *pData = GetFirstData(pos);
	while(pData)
	{
		if(strID == pData->ID) return pData;
		pData = GetNextData(pos);
	}

	return NULL;
}