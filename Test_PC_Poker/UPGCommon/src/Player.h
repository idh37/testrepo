#pragma once

#include "UserStruct.h"

class UPG_COMMON_EXPORT CPlayer
{
protected:
	CHANGE_USERINFO_ROOM m_cRoomChangeInfo;
public:
	USERINFO			UI;
	int					ServPNum;				// 서버 기준의 플레이어 번호
	int					PNum;					// 플레이어 번호
	NMBASE::AVATAR::CAvatar			Avatar;					// 아바타 클래스

	int					JoinState;				// 게임 참여 상태(0:불참, 1:참여)
	int					PlayState;				// 플레이 상태(0:게임종료, 1:게임중)

	int					nSndFxKind;
	int					MyIpCheck;

	INT64				nRoundPrevMoney;		//판시작머니
	
	BOOL				bFold;
	INT64				nRealGold;				//골드
	INT64				nPlayGold;				//골드

	BOOL				bFoldOpen;				//기권오픈(본인카드오픈)

public:
	virtual void Clear();
	virtual	void SetPlayerNum(int pnum);
	virtual void SetNewPlayer(USERINFO *pUI);
	virtual	void Reset();
	virtual	void SetGold(INT64 gold);

	virtual void SetChangeRoomInfo(CHANGE_USERINFO_ROOM &changeInfo);
	virtual CHANGE_USERINFO_ROOM *GetChangeRoomInfo(void){return &m_cRoomChangeInfo;}
	//virtual void InitGameJoinData(USERINFO *pUI = NULL);
public:
	CPlayer(void);
	virtual ~CPlayer(void);
};