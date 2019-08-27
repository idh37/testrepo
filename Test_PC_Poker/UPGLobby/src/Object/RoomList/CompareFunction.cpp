#include "StdAfx.h"
#include "CompareFunction.h"

bool RoomListRoomNumberCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();

	if(bIncrement)
		return pInfo1->sRoomNum < pInfo2->sRoomNum;
	else
		return pInfo1->sRoomNum > pInfo2->sRoomNum;
}

bool RoomListMinimumMoneyCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();

	if(bIncrement)
		return pInfo1->llEnterLimitMoney < pInfo2->llEnterLimitMoney;
	else
		return pInfo1->llEnterLimitMoney > pInfo2->llEnterLimitMoney;
}

bool RoomListSeedMoneyCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();

	if(pInfo1->llSeedMoney == pInfo2->llSeedMoney)
	{
		if(bIncrement)
			return pInfo1->sAutoBetKind < pInfo2->sAutoBetKind;
		else
			return pInfo1->sAutoBetKind > pInfo2->sAutoBetKind;
	}
	else
	{
		if(bIncrement)
			return pInfo1->llSeedMoney < pInfo2->llSeedMoney;
		else
			return pInfo1->llSeedMoney > pInfo2->llSeedMoney;
	}
}

bool RoomListGameTypeCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();

	if(bIncrement)
		return pInfo1->cRuleType < pInfo2->cRuleType;
	else
		return pInfo1->cRuleType > pInfo2->cRuleType;
}

bool RoomListGameModeCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();

	if(pInfo1->cFormKind == pInfo2->cFormKind)
	{
		if(bIncrement)
			return strcmp(pInfo1->szTitle, pInfo2->szTitle) < 0;
		else
			return strcmp(pInfo1->szTitle, pInfo2->szTitle) > 0;
	}
	else
	{
		if(bIncrement)
			return pInfo1->cFormKind < pInfo2->cFormKind;
		else
			return pInfo1->cFormKind > pInfo2->cFormKind;
	}
}

bool RoomListTitleCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();

	if(bIncrement)
		return strcmp(pInfo1->szTitle, pInfo2->szTitle) < 0;
	else
		return strcmp(pInfo1->szTitle, pInfo2->szTitle) > 0;
}

bool RoomListUserCountCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();

	if(bIncrement)
		return pInfo1->cNowUserNum < pInfo2->cNowUserNum;
	else
		return pInfo1->cNowUserNum > pInfo2->cNowUserNum;
}

bool RoomListTotalMoneyCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();

	if(bIncrement)
		return pInfo1->llTotalPMoneySum < pInfo2->llTotalPMoneySum;
	else
		return pInfo1->llTotalPMoneySum > pInfo2->llTotalPMoneySum;
}

bool RoomListDefaultCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();


// 	if (pInfo1->szClientSideVar[CS_PREV_ROOM] > pInfo2->szClientSideVar[CS_PREV_ROOM]) 	return true;		//직전방 제일 위로
// 	else if (pInfo1->szClientSideVar[CS_PREV_ROOM] < pInfo2->szClientSideVar[CS_PREV_ROOM]) return false;
// 	else 
// 	if (pInfo1->szClientSideVar[CS_JOINABLE] > pInfo2->szClientSideVar[CS_JOINABLE]) return true;
	
	
	if (pInfo1->szClientSideVar[CS_PREV_ROOM] > pInfo2->szClientSideVar[CS_PREV_ROOM]) 	return true;		//1. 직전방 제일 위로
	else if (pInfo1->szClientSideVar[CS_PREV_ROOM] < pInfo2->szClientSideVar[CS_PREV_ROOM]) return false;
	else 
		if (pInfo1->szClientSideVar[CS_SELECTED_ROOM] > pInfo2->szClientSideVar[CS_SELECTED_ROOM]) return true;		//2. 선택방 제일 위로
	else if (pInfo1->szClientSideVar[CS_SELECTED_ROOM] < pInfo2->szClientSideVar[CS_SELECTED_ROOM]) return false; 
	else if (pInfo1->szClientSideVar[CS_JOINABLE] > pInfo2->szClientSideVar[CS_JOINABLE]) return true;		//3. 입장가능이 위로
	else if (pInfo1->szClientSideVar[CS_JOINABLE] < pInfo2->szClientSideVar[CS_JOINABLE])	return false;
	else
	{
		if((pInfo1->cNowUserNum >= pInfo1->cMaxUserNum) == (pInfo2->cNowUserNum >= pInfo2->cMaxUserNum))
		{
			if (pInfo1->llEnterLimitMoney == pInfo2->llEnterLimitMoney)
			{		
				if (pInfo1->llSeedMoney == pInfo2->llSeedMoney)
				{
					if (pInfo1->sAutoBetKind == pInfo2->sAutoBetKind)
					{
						return (pInfo1->cNowUserNum < pInfo2->cNowUserNum);
					}
					else
						return (pInfo1->sAutoBetKind > pInfo2->sAutoBetKind);
				}
				else 
					return (pInfo1->llSeedMoney > pInfo2->llSeedMoney);		
			}
			else
				return (pInfo1->llEnterLimitMoney > pInfo2->llEnterLimitMoney);

		}
		else if ((pInfo1->cNowUserNum < pInfo1->cMaxUserNum) && (pInfo2->cNowUserNum >= pInfo2->cMaxUserNum))
		{
			return true;
		}
	}

	return false;
}

bool RoomListEnterMinimumMoneyCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	SMALLROOMINFO *pInfo1 = (SMALLROOMINFO *)pItem1->GetItemData();
	SMALLROOMINFO *pInfo2 = (SMALLROOMINFO *)pItem2->GetItemData();
	INT64 *pEnterLimitMoney = (INT64 *)lParam;

	if ( (pInfo1->llEnterLimitMoney == *pEnterLimitMoney  && pInfo2->llEnterLimitMoney == *pEnterLimitMoney) ||
		(pInfo1->llEnterLimitMoney != *pEnterLimitMoney  && pInfo2->llEnterLimitMoney != *pEnterLimitMoney) )
	{		
		return RoomListDefaultCompareFunction(pItem1, pItem2, bIncrement, lParam);
	}
	else if (pInfo1->llEnterLimitMoney == *pEnterLimitMoney &&  pInfo2->llEnterLimitMoney != *pEnterLimitMoney)
	{
		return true;
	}

	return false;
}