#include "StdAfx.h"
#include "DataRoomOptionList.h"

CDataRoomOptionList::CDataRoomOptionList(void)
{
}

CDataRoomOptionList::~CDataRoomOptionList(void)
{
	m_mapRoomOptions.clear();
}


CDataRoomOptionList::DATAENTITY* CDataRoomOptionList::AddData(int key, ROOM_CREATEOPTION *data)
{
	if (data==NULL) 
		return NULL;

	//방 종류별 룸디폴트옵션정보를 추출하여 따로 저장한다. (더미방을 만들때 참고하기위해 저장)
	for(int i =0; i<data->nArrayCnt; i++)
	{
		if (data->arrRoomOption[i].cDefaultQS)
		{
			if (m_mapRoomOptions.find(data->arrRoomOption[i].llEnterLimitMoney)==m_mapRoomOptions.end())
			{
				//방옵션추가
				m_mapRoomOptions[data->arrRoomOption[i].llEnterLimitMoney] = data->arrRoomOption[i];
			}			
		}
	}
	
	return CDataList::AddData(key, data);
}

void CDataRoomOptionList::Clear()
{
	CDataList::Clear();

	m_mapRoomOptions.clear();
}



ROOMOPTION *CDataRoomOptionList::GetDefaultRoomOption(INT64 i64SeedMoney, ROOM_CHIPKIND chipKind )
{
	if(GM().GetMyInfo() == NULL) return NULL;

	ROOM_CREATEOPTION *pRoomCreationOption = GetRoomCreationOption( chipKind );
	if(pRoomCreationOption == NULL || pRoomCreationOption->nArrayCnt == 0)
	{
		ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU, "");
		return NULL;
	}

	return &(pRoomCreationOption->stDefaultCreateRoom);
}

ROOMOPTION *CDataRoomOptionList::GetDefaultFreeRoomOption(INT64 i64SeedMoney)
{
	if(GM().GetMyInfo() == NULL) return NULL;

	ROOM_CREATEOPTION *pRoomCreationOption = GetFreeRoomCreationOption();
	if(pRoomCreationOption == NULL || pRoomCreationOption->nArrayCnt == 0)
	{
		ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU, "");
		return NULL;
	}

	return &(pRoomCreationOption->stDefaultCreateRoom);
}

ROOM_CREATEOPTION *CDataRoomOptionList::GetRoomCreationOption( ROOM_CHIPKIND chipKind )
{
	CPlayer *pPlayer = GM().GetMyInfo();
	ASSERT(pPlayer != NULL);

	if( chipKind == ROOM_CHIPKIND_NORMAL )
	{
		if(pPlayer)
			return GetRoomCreationOption( pPlayer->UI.GetRealPMoney(), chipKind );
	}
	else if( chipKind == ROOM_CHIPKIND_GOLD )
	{
		return GetRoomCreationOption( GM().GetMyGold(), chipKind );
	}

	return NULL;
}

BOOL CDataRoomOptionList::IsAbleToJoinRoom(SMALLROOMINFO *pSRI)
{
	//내가 pSRI 방에 들어갈수있으면 TRUE, 없으면 FALSE 반환	
	if (pSRI==NULL)
		return FALSE;

	ROOM_CREATEOPTION *pRoomCreationOption = GetRoomCreationOption(ROOM_CHIPKIND_NORMAL);
	if(pRoomCreationOption == NULL) return FALSE;
		
	int nMaxDefault = -1;
	for(int i=0;i<pRoomCreationOption->nArrayCnt;i++)
	{
		if(pRoomCreationOption->arrRoomOption[i].llEnterLimitMoney == pSRI->llEnterLimitMoney)
			return TRUE;			
	}
	return FALSE;
}

ROOM_CREATEOPTION *CDataRoomOptionList::GetRoomCreationOption(INT64 llPMoney, ROOM_CHIPKIND chipKind )
{
	//보유머니 구간의 ROOM_CREATEOPTION 정보를 가져온다.

	DATAPOSITION pos;
	ROOM_CREATEOPTION *pRoomCreateOption = GetFirstData(pos);
	ROOM_CREATEOPTION *pRoomCreateOptionTemp=NULL;

	while(pRoomCreateOption)
	{
		if (pRoomCreateOption->stUserMoneyRange.IsInRange(llPMoney) && pRoomCreateOption->stDefaultCreateRoom.ChipKind == (int)chipKind)
		{
			if((NULL==pRoomCreateOptionTemp)||(pRoomCreateOptionTemp->stDefaultCreateRoom.llEnterLimitMoney<=pRoomCreateOption->stDefaultCreateRoom.llEnterLimitMoney)){
				pRoomCreateOptionTemp=pRoomCreateOption;
			}
		}

		pRoomCreateOption = GetNextData(pos);
	}
	return pRoomCreateOptionTemp;

}

ROOM_CREATEOPTION *CDataRoomOptionList::GetFreeRoomCreationOption()
{
	//보유머니 구간의 ROOM_CREATEOPTION 정보를 가져온다.

	DATAPOSITION pos;
	ROOM_CREATEOPTION *pRoomCreateOption = GetFirstData(pos);

	while(pRoomCreateOption)
	{
		if( pRoomCreateOption->stDefaultCreateRoom.llEnterLimitMoney == 0 && pRoomCreateOption->stDefaultCreateRoom.ChipKind == ROOM_CHIPKIND_NORMAL )
		{
			return pRoomCreateOption ;
		}else
		{
			pRoomCreateOption = GetNextData(pos);
		}

	}

	return NULL;
}

BOOL CDataRoomOptionList::IsTopClass(INT64 llPmoney)	//머니가 최상위 등급인지 여부를 가져온다.
{

	DATAPOSITION_REVERSE pos_rev;
	ROOM_CREATEOPTION *pLastCreateOption = GetLastData(pos_rev);
	
	if (pLastCreateOption==NULL)
		return FALSE;

	return pLastCreateOption->stUserMoneyRange.IsInRange(llPmoney);	
}


BOOL CDataRoomOptionList::GetEnterLimitMoneyKind(INT64 llPMoney, vector<INT64> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom)
{
	//PMoney 구간에 속하는 방종류(10조방, 100조방 등)를 가져온다
	//bExceptFreeRoom : 자유방(제한없음방) 제외여부

// 	vecEnterLimitMoneyKind.clear();
// 
// 	ROOM_CREATEOPTION *pRoomCreateOption = GetRoomCreationOption(llPMoney);
// 	if (pRoomCreateOption==NULL)
// 		return FALSE;
// 
// 
// 	for (int i = 0 ; i <pRoomCreateOption->nArrayCnt ;  i++ )
// 	{
// 		INT64 llEnterLimiteMoneyKind = pRoomCreateOption->arrRoomOption[i].llEnterLimitMoney;
// 
// 		if (bExceptFreeRoom && llEnterLimiteMoneyKind==0) //bExceptFreeRoom가 TRUE이면 제한없음방은 제외한다.
// 			continue;
// 
// 		if (find(vecEnterLimitMoneyKind.begin(), vecEnterLimitMoneyKind.end(), llEnterLimiteMoneyKind) == vecEnterLimitMoneyKind.end())
// 		{
// 			vecEnterLimitMoneyKind.push_back(llEnterLimiteMoneyKind);
// 		}
// 	}
// 
// 	return TRUE;


	vecEnterLimitMoneyKind.clear();

	CDataRoomOptionList::DATAPOSITION pos;
	ROOM_CREATEOPTION *pRoomCreateOption = GetFirstData(pos);

	while(pRoomCreateOption)
	{
		if (pRoomCreateOption->stUserMoneyRange.IsInRange(llPMoney))
		{	
			for (int i = 0 ; i <pRoomCreateOption->nArrayCnt ;  i++ )
			{			
				INT64 llEnterLimiteMoneyKind = pRoomCreateOption->arrRoomOption[i].llEnterLimitMoney;

				if (bExceptFreeRoom && llEnterLimiteMoneyKind==0) //제한없음방은 제외한다.
					continue;

				if (find(vecEnterLimitMoneyKind.begin(), vecEnterLimitMoneyKind.end(), llEnterLimiteMoneyKind) == vecEnterLimitMoneyKind.end())
				{
					vecEnterLimitMoneyKind.push_back(llEnterLimiteMoneyKind);
				}
			}
		}

		pRoomCreateOption = GetNextData(pos);
	}

	return TRUE;

}


BOOL CDataRoomOptionList::GetEnterLimitMoneyKind(vector<INT64> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom)
{
	//모든 방종류(10조방, 100조방 등)를 가져온다
	//bExceptFreeRoom : 자유방(제한없음방) 제외여부

	vecEnterLimitMoneyKind.clear();

	CDataRoomOptionList::DATAPOSITION pos;
	ROOM_CREATEOPTION *pRoomCreateOption = GetFirstData(pos);

	while(pRoomCreateOption)
	{
		for (int i = 0 ; i <pRoomCreateOption->nArrayCnt ;  i++ )
		{
			INT64 llEnterLimiteMoneyKind = pRoomCreateOption->arrRoomOption[i].llEnterLimitMoney;

			if (bExceptFreeRoom && llEnterLimiteMoneyKind==0) //제한없음방은 제외한다.
				continue;

			if (find(vecEnterLimitMoneyKind.begin(), vecEnterLimitMoneyKind.end(), llEnterLimiteMoneyKind) == vecEnterLimitMoneyKind.end())
			{
				vecEnterLimitMoneyKind.push_back(llEnterLimiteMoneyKind);
			}
		}
		pRoomCreateOption = GetNextData(pos);
	}

	return TRUE;
}


BOOL CDataRoomOptionList::GetWholeRoomOption(vector<ROOMOPTION> &vecRoomOptionKind, BOOL bExceptFreeRoom)
{
	//모든방옵션종류(10조방, 100조방 등)를 가져온다 
	//bExceptFreeRoom : 자유방 제외 여부

	
	////////////////////////////////////////////////////////////////////////////
	//1. 방종류별 방옵션 가져와서 임시벡터에 저장.

	class CFindRoomOption
	{
	public:
		CFindRoomOption(const ROOMOPTION& RoomOption)
		{
			m_RoomOption = RoomOption;  
		}

		bool operator()(const ROOMOPTION &RoomOptionCompare)
		{
			return (memcmp(&RoomOptionCompare, &m_RoomOption, sizeof(ROOMOPTION))==0);
		} 

		ROOMOPTION m_RoomOption;	
	};

	vecRoomOptionKind.clear();

	CDataRoomOptionList::DATAPOSITION pos;
	ROOM_CREATEOPTION *pRoomCreateOption = GetFirstData(pos);

	while(pRoomCreateOption)
	{
		for(int i=0; i<pRoomCreateOption->nArrayCnt; i++)
		{
			if (bExceptFreeRoom && pRoomCreateOption->arrRoomOption[i].llEnterLimitMoney==0)	//bExceptFreeRoom이 TRUE 일경우 자유방(제한없음방)은 제외한다.
				continue;

			if (find_if(vecRoomOptionKind.begin(), vecRoomOptionKind.end(), CFindRoomOption(pRoomCreateOption->arrRoomOption[i]))==vecRoomOptionKind.end())
			{
				vecRoomOptionKind.push_back(pRoomCreateOption->arrRoomOption[i]);
			}			
		}
		pRoomCreateOption = GetNextData(pos);
	}

	return TRUE;
}

// 자율규제안 추가 함수. 이걸로 동작하도록 수정.
ROOMOPTION *CDataRoomOptionList::GetQJRoomOption(bool bUseGameMode, int nGameMode, bool bUseSeedMoney, int nSeedType)
{
	ROOM_CREATEOPTION *pRoomCreationOption = NULL;
	pRoomCreationOption = GetRoomCreationOption(ROOM_CHIPKIND_NORMAL);
	if(pRoomCreationOption == NULL) return NULL;

	ROOMOPTION *pDefaultRoomOption = GetDefaultRoomOption();
	if(pDefaultRoomOption == NULL) return NULL;

	m_QJRoom = *pDefaultRoomOption;

	// 여기서 부터 바로입장 수정 작업.
	
	//게임 규모가 설정되어 있지 않으면 일반방으로 강제 셋팅
	if(!bUseSeedMoney)
	{
		//nSeedType = 2; //작은방으로 강제 셋팅
		nSeedType = 0; //큰 방으로 강제 셋팅
	}

	map<INT64, list<ROOMOPTION *> > mapTemp;
	int nCount = pRoomCreationOption->nArrayCnt;
	ROOMOPTION *pRoomOption = NULL;
	ROOMOPTION *pResultRoomOption = NULL;
	INT64 i64SeedMoney = -1;
	for(int i=0;i<nCount;i++)
	{
		pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);

		//if (pDefaultRoomOption->llEnterLimitMoney != pRoomOption->llEnterLimitMoney)	//(버그수정 by terry) 디폴트 EnterLimitMoney방을 기준으로만 고려한다 디폴트가 아닌방의 시드머니는 고려되지 않는다.
		if (GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false) != pRoomOption->llEnterLimitMoney)	//(버그수정 by terry) 디폴트 EnterLimitMoney방을 기준으로만 고려한다 디폴트가 아닌방의 시드머니는 고려되지 않는다.
			continue;

		//규제안 : 모든 게임에 입장 머니 구간 설정
		mapTemp[pRoomOption->llSeedMoney].push_back(pRoomOption); 

		//규제안 : 자동배팅, 자유방이 아니경우 제거
		/*
		if(GM().GetCurrentGameType() == IDX_GAME_HA)
		{
			if(pRoomOption->llEnterLimitMoney != 0)
			{
				mapTemp[pRoomOption->llSeedMoney].push_back(pRoomOption);
			}
		}else
		{
			if((pRoomOption->sAutoBet==0)?false:true && (pRoomOption->llEnterLimitMoney != 0))
			{
				mapTemp[pRoomOption->llSeedMoney].push_back(pRoomOption);
			}
		}
		*/
	}

	nCount = (int)mapTemp.size();
	int nFind = -1;

	switch(nCount)
	{
	case 0: 
		break;
	case 1:
		nFind = 0;
		break;
	case 2:
		switch(nSeedType)
		{
		case 0:	//큰방
			nFind = 1;
			break;
		case 1: //일반방
			nFind = 1;
			break;
		case 2:	//작은방
			nFind = 0;
			break;
		}
		break;
	case 3:
		switch(nSeedType)
		{
		case 0:	//큰방
			nFind = 2;
			break;
		case 1:	//일반방
			nFind = 1;
			break;
		case 2:	//작은방
			nFind = 0;
			break;
		}
		break;
	default:
		switch(nSeedType)
		{
		case 0:	//큰방
			nFind = nCount-1;
			break;
		case 1:	//일반방
			nFind = rand()%(nCount-2) + 1;
			break;
		case 2:	//작은방
			nFind = 0;
			break;
		}
		break;
	}

	list<ROOMOPTION *> *plistOption = NULL;

	map<INT64, list<ROOMOPTION *> >::iterator start = mapTemp.begin();
	map<INT64, list<ROOMOPTION *> >::iterator end = mapTemp.end();

	for(int i=0;start != end;start++,i++)
	{
		if(i == nFind) plistOption = &(start->second);
	}

	if(plistOption)
	{
		list<ROOMOPTION *>::iterator start = plistOption->begin();
		list<ROOMOPTION *>::iterator end = plistOption->end();

		INT64 i64LimitMoney = -1;

		for(;start != end;start++)
		{
			pRoomOption = *start;
			if(i64LimitMoney <= pRoomOption->llEnterLimitMoney)
			{
				pResultRoomOption = pRoomOption;
				i64LimitMoney = pRoomOption->  llEnterLimitMoney;
			}
		}
	}

	if(pResultRoomOption == NULL) return NULL;

	m_QJRoom = *pResultRoomOption;

	if(bUseGameMode)
		m_QJRoom.cFormKind = nGameMode ;
	else
	{
		if(GM().GetCurrentGameType() == IDX_GAME_SP || GM().GetCurrentGameType() == IDX_GAME_HL )
			m_QJRoom.cFormKind = 1;
		else
			m_QJRoom.cFormKind = 0;
	}

	return &m_QJRoom;
}

ROOMOPTION *CDataRoomOptionList::GetQJRoomOption(bool bUseBetRule, bool bAutoBet, bool bUseSeedMoney, int nSeedType, short sBetRuleKind )
{
	ROOM_CREATEOPTION *pRoomCreationOption = NULL;
	pRoomCreationOption = GetRoomCreationOption(ROOM_CHIPKIND_NORMAL);
	if(pRoomCreationOption == NULL) return NULL;

	ROOMOPTION *pDefaultRoomOption = GetDefaultRoomOption();
	if(pDefaultRoomOption == NULL) return NULL;

	m_QJRoom = *pDefaultRoomOption;

	if(bUseBetRule)
	{
		//게임 규모가 설정되어 있지 않으면 일반방으로 강제 셋팅
		if(!bUseSeedMoney)
		{
			nSeedType = 2; //작은방으로 강제 셋팅
		}

		map<INT64, list<ROOMOPTION *> > mapTemp;
		int nCount = pRoomCreationOption->nArrayCnt;
		ROOMOPTION *pRoomOption = NULL;
		ROOMOPTION *pResultRoomOption = NULL;
		INT64 i64SeedMoney = -1;
		for(int i=0;i<nCount;i++)
		{
			pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);

			//if (pDefaultRoomOption->llEnterLimitMoney != pRoomOption->llEnterLimitMoney)	//(버그수정 by terry) 디폴트 EnterLimitMoney방을 기준으로만 고려한다 디폴트가 아닌방의 시드머니는 고려되지 않는다.
			if (GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false) != pRoomOption->llEnterLimitMoney)	//(버그수정 by terry) 디폴트 EnterLimitMoney방을 기준으로만 고려한다 디폴트가 아닌방의 시드머니는 고려되지 않는다.
				continue;

			if((((pRoomOption->sAutoBet==0)?false:true) == bAutoBet) && (pRoomOption->llEnterLimitMoney != 0) && pRoomOption->sBetRuleKind == sBetRuleKind )
			{
				mapTemp[pRoomOption->llSeedMoney].push_back(pRoomOption);
			}
		}

		nCount = (int)mapTemp.size();
		int nFind = -1;

		switch(nCount)
		{
		case 0: 
			break;
		case 1:
			nFind = 0;
			break;
		case 2:
			switch(nSeedType)
			{
			case 0:	//큰방
				nFind = 1;
				break;
			case 1: //일반방
				nFind = 1;
				break;
			case 2:	//작은방
				nFind = 0;
				break;
			}
			break;
		case 3:
			switch(nSeedType)
			{
			case 0:	//큰방
				nFind = 2;
				break;
			case 1:	//일반방
				nFind = 1;
				break;
			case 2:	//작은방
				nFind = 0;
				break;
			}
			break;
		default:
			switch(nSeedType)
			{
			case 0:	//큰방
				nFind = nCount-1;
				break;
			case 1:	//일반방
				nFind = rand()%(nCount-2) + 1;
				break;
			case 2:	//작은방
				nFind = 0;
				break;
			}
			break;
		}

		list<ROOMOPTION *> *plistOption = NULL;
		
		map<INT64, list<ROOMOPTION *> >::iterator start = mapTemp.begin();
		map<INT64, list<ROOMOPTION *> >::iterator end = mapTemp.end();

		for(int i=0;start != end;start++,i++)
		{
			if(i == nFind) plistOption = &(start->second);
		}

		if(plistOption)
		{
			list<ROOMOPTION *>::iterator start = plistOption->begin();
			list<ROOMOPTION *>::iterator end = plistOption->end();

			INT64 i64LimitMoney = -1;

			for(;start != end;start++)
			{
				pRoomOption = *start;
				if(i64LimitMoney <= pRoomOption->llEnterLimitMoney)
				{
					pResultRoomOption = pRoomOption;
					i64LimitMoney = pRoomOption->llEnterLimitMoney;
				}
			}
		}

		if(pResultRoomOption == NULL) return NULL;

		m_QJRoom = *pResultRoomOption;
		//if(!bUseSeedMoney) m_QJRoom.llSeedMoney = (INT64)-1;
	}
	//else
	//{
	//	m_QJRoom.sAutoBet = (short)-1;
	//	m_QJRoom.llSeedMoney = (INT64)-1;
	//}

	return &m_QJRoom;
}



INT64 CDataRoomOptionList::FindMaxSeedMoney(INT64 llMoney, INT64 llUserMoney, char &cCardSetting)
{

	vector<ROOMOPTION> vecRoomOption;
	if (GetWholeRoomOption(vecRoomOption, TRUE)==FALSE)
	{
		return 0ll;
	}
	INT64 llFindMoney=0ll;
	bool bEnterRoom = false;
	vector<ROOMOPTION>::reverse_iterator it;
	for(it = vecRoomOption.rbegin(); it!=vecRoomOption.rend();it++){
		bEnterRoom = it->stMoneyRange.IsInRange(llUserMoney)?true:false;
		if(false == bEnterRoom) continue;
		if(llMoney>=it->llSeedMoney){
			if(0ll==llFindMoney){
				llFindMoney=it->llSeedMoney;
				cCardSetting=it->cCardSetting;
			}
			if(llFindMoney<it->llSeedMoney){
				llFindMoney=it->llSeedMoney;
				cCardSetting=it->cCardSetting;
			}
		}
	}
	return llFindMoney;
}