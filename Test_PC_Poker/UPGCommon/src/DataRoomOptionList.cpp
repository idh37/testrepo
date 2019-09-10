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

	//�� ������ �����Ʈ�ɼ������� �����Ͽ� ���� �����Ѵ�. (���̹��� ���鶧 �����ϱ����� ����)
	for(int i =0; i<data->nArrayCnt; i++)
	{
		if (data->arrRoomOption[i].cDefaultQS)
		{
			if (m_mapRoomOptions.find(data->arrRoomOption[i].llEnterLimitMoney)==m_mapRoomOptions.end())
			{
				//��ɼ��߰�
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
	//���� pSRI �濡 ���������� TRUE, ������ FALSE ��ȯ	
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
	//�����Ӵ� ������ ROOM_CREATEOPTION ������ �����´�.

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
	//�����Ӵ� ������ ROOM_CREATEOPTION ������ �����´�.

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

BOOL CDataRoomOptionList::IsTopClass(INT64 llPmoney)	//�Ӵϰ� �ֻ��� ������� ���θ� �����´�.
{

	DATAPOSITION_REVERSE pos_rev;
	ROOM_CREATEOPTION *pLastCreateOption = GetLastData(pos_rev);
	
	if (pLastCreateOption==NULL)
		return FALSE;

	return pLastCreateOption->stUserMoneyRange.IsInRange(llPmoney);	
}


BOOL CDataRoomOptionList::GetEnterLimitMoneyKind(INT64 llPMoney, vector<INT64> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom)
{
	//PMoney ������ ���ϴ� ������(10����, 100���� ��)�� �����´�
	//bExceptFreeRoom : ������(���Ѿ�����) ���ܿ���

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
// 		if (bExceptFreeRoom && llEnterLimiteMoneyKind==0) //bExceptFreeRoom�� TRUE�̸� ���Ѿ������� �����Ѵ�.
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

				if (bExceptFreeRoom && llEnterLimiteMoneyKind==0) //���Ѿ������� �����Ѵ�.
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
	//��� ������(10����, 100���� ��)�� �����´�
	//bExceptFreeRoom : ������(���Ѿ�����) ���ܿ���

	vecEnterLimitMoneyKind.clear();

	CDataRoomOptionList::DATAPOSITION pos;
	ROOM_CREATEOPTION *pRoomCreateOption = GetFirstData(pos);

	while(pRoomCreateOption)
	{
		for (int i = 0 ; i <pRoomCreateOption->nArrayCnt ;  i++ )
		{
			INT64 llEnterLimiteMoneyKind = pRoomCreateOption->arrRoomOption[i].llEnterLimitMoney;

			if (bExceptFreeRoom && llEnterLimiteMoneyKind==0) //���Ѿ������� �����Ѵ�.
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
	//����ɼ�����(10����, 100���� ��)�� �����´� 
	//bExceptFreeRoom : ������ ���� ����

	
	////////////////////////////////////////////////////////////////////////////
	//1. �������� ��ɼ� �����ͼ� �ӽú��Ϳ� ����.

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
			if (bExceptFreeRoom && pRoomCreateOption->arrRoomOption[i].llEnterLimitMoney==0)	//bExceptFreeRoom�� TRUE �ϰ�� ������(���Ѿ�����)�� �����Ѵ�.
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

// ���������� �߰� �Լ�. �̰ɷ� �����ϵ��� ����.
ROOMOPTION *CDataRoomOptionList::GetQJRoomOption(bool bUseGameMode, int nGameMode, bool bUseSeedMoney, int nSeedType)
{
	ROOM_CREATEOPTION *pRoomCreationOption = NULL;
	pRoomCreationOption = GetRoomCreationOption(ROOM_CHIPKIND_NORMAL);
	if(pRoomCreationOption == NULL) return NULL;

	ROOMOPTION *pDefaultRoomOption = GetDefaultRoomOption();
	if(pDefaultRoomOption == NULL) return NULL;

	m_QJRoom = *pDefaultRoomOption;

	// ���⼭ ���� �ٷ����� ���� �۾�.
	
	//���� �Ը� �����Ǿ� ���� ������ �Ϲݹ����� ���� ����
	if(!bUseSeedMoney)
	{
		//nSeedType = 2; //���������� ���� ����
		nSeedType = 0; //ū ������ ���� ����
	}

	map<INT64, list<ROOMOPTION *> > mapTemp;
	int nCount = pRoomCreationOption->nArrayCnt;
	ROOMOPTION *pRoomOption = NULL;
	ROOMOPTION *pResultRoomOption = NULL;
	INT64 i64SeedMoney = -1;
	for(int i=0;i<nCount;i++)
	{
		pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);

		//if (pDefaultRoomOption->llEnterLimitMoney != pRoomOption->llEnterLimitMoney)	//(���׼��� by terry) ����Ʈ EnterLimitMoney���� �������θ� ����Ѵ� ����Ʈ�� �ƴѹ��� �õ�Ӵϴ� ������� �ʴ´�.
		if (GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false) != pRoomOption->llEnterLimitMoney)	//(���׼��� by terry) ����Ʈ EnterLimitMoney���� �������θ� ����Ѵ� ����Ʈ�� �ƴѹ��� �õ�Ӵϴ� ������� �ʴ´�.
			continue;

		//������ : ��� ���ӿ� ���� �Ӵ� ���� ����
		mapTemp[pRoomOption->llSeedMoney].push_back(pRoomOption); 

		//������ : �ڵ�����, �������� �ƴϰ�� ����
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
		case 0:	//ū��
			nFind = 1;
			break;
		case 1: //�Ϲݹ�
			nFind = 1;
			break;
		case 2:	//������
			nFind = 0;
			break;
		}
		break;
	case 3:
		switch(nSeedType)
		{
		case 0:	//ū��
			nFind = 2;
			break;
		case 1:	//�Ϲݹ�
			nFind = 1;
			break;
		case 2:	//������
			nFind = 0;
			break;
		}
		break;
	default:
		switch(nSeedType)
		{
		case 0:	//ū��
			nFind = nCount-1;
			break;
		case 1:	//�Ϲݹ�
			nFind = rand()%(nCount-2) + 1;
			break;
		case 2:	//������
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
		//���� �Ը� �����Ǿ� ���� ������ �Ϲݹ����� ���� ����
		if(!bUseSeedMoney)
		{
			nSeedType = 2; //���������� ���� ����
		}

		map<INT64, list<ROOMOPTION *> > mapTemp;
		int nCount = pRoomCreationOption->nArrayCnt;
		ROOMOPTION *pRoomOption = NULL;
		ROOMOPTION *pResultRoomOption = NULL;
		INT64 i64SeedMoney = -1;
		for(int i=0;i<nCount;i++)
		{
			pRoomOption = &(pRoomCreationOption->arrRoomOption[i]);

			//if (pDefaultRoomOption->llEnterLimitMoney != pRoomOption->llEnterLimitMoney)	//(���׼��� by terry) ����Ʈ EnterLimitMoney���� �������θ� ����Ѵ� ����Ʈ�� �ƴѹ��� �õ�Ӵϴ� ������� �ʴ´�.
			if (GM().GetRoomOptionLimitMoney(pDefaultRoomOption, false) != pRoomOption->llEnterLimitMoney)	//(���׼��� by terry) ����Ʈ EnterLimitMoney���� �������θ� ����Ѵ� ����Ʈ�� �ƴѹ��� �õ�Ӵϴ� ������� �ʴ´�.
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
			case 0:	//ū��
				nFind = 1;
				break;
			case 1: //�Ϲݹ�
				nFind = 1;
				break;
			case 2:	//������
				nFind = 0;
				break;
			}
			break;
		case 3:
			switch(nSeedType)
			{
			case 0:	//ū��
				nFind = 2;
				break;
			case 1:	//�Ϲݹ�
				nFind = 1;
				break;
			case 2:	//������
				nFind = 0;
				break;
			}
			break;
		default:
			switch(nSeedType)
			{
			case 0:	//ū��
				nFind = nCount-1;
				break;
			case 1:	//�Ϲݹ�
				nFind = rand()%(nCount-2) + 1;
				break;
			case 2:	//������
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