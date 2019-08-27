#pragma once
#include "datalist.h"

class UPG_COMMON_EXPORT CDataRoomOptionList : public CDataList<int, ROOM_CREATEOPTION>
{
protected:
	//입장제한 금액별 게임방 정보
	map<INT64, ROOMOPTION>		m_mapRoomOptions;		//map<방종류금액, 방옵션> //방 종류별 룸디폴트옵션정보; //더미방을 만들때 참고하기위해 저장
	ROOMOPTION					m_QJRoom;
public:
	CDataRoomOptionList(void);
	virtual ~CDataRoomOptionList(void);
	virtual DATAENTITY* AddData(int key, ROOM_CREATEOPTION *data);
	virtual void Clear();

	ROOMOPTION *GetDefaultRoomOption(INT64 i64SeedMoney = 0, ROOM_CHIPKIND chipkind = ROOM_CHIPKIND_NORMAL );			//자신에 맞는 기본 시드 머니를 계산해 준다.
	ROOMOPTION *GetDefaultFreeRoomOption(INT64 i64SeedMoney = 0);		//규제안 추가로 디폴트 자유방 옵션이 필요
	ROOM_CREATEOPTION *GetRoomCreationOption( INT64 llPMoney, ROOM_CHIPKIND chipkind = ROOM_CHIPKIND_NORMAL );
	ROOM_CREATEOPTION *GetFreeRoomCreationOption();	
	ROOM_CREATEOPTION *GetRoomCreationOption( ROOM_CHIPKIND chipkind = ROOM_CHIPKIND_NORMAL );
	BOOL IsAbleToJoinRoom(SMALLROOMINFO *pSRI);		//자신이 참여할수있는 방인지 아닌지 알려준다.
	BOOL IsTopClass(INT64 llPmoney);	//머니가 최상위 등급인지 여부를 가져온다.	
	
	map<INT64, ROOMOPTION> *GetDefalutRoomOption()//디폴트 옵션방정보 
	{
		return &m_mapRoomOptions;
	}	


	BOOL GetEnterLimitMoneyKind(INT64 llPMoney, vector<INT64> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom);	//PMoney 구간에 속하는 방종류(10조방, 100조방) 를 가져온다
	BOOL GetEnterLimitMoneyKind(vector<INT64> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom);// 모든 방종류(10조방, 100조방) 를 가져온다
	BOOL GetWholeRoomOption(vector<ROOMOPTION> &vecEnterLimitMoneyKind, BOOL bExceptFreeRoom);	// 모든 방옵션 종류를 가져온다.

	ROOMOPTION *GetQJRoomOption(bool bUseGameMode, int nGameMode, bool bUseSeedMoney, int nSeedType) ;
	ROOMOPTION *GetQJRoomOption(bool bUseBetRule, bool bAutoBet, bool bUseSeedMoney, int nSeedType, short sBetRuleKind);

	//yoo
	INT64 FindMaxSeedMoney(INT64 llMoney, INT64 llUserMoney, char &cCardSetting);
};







