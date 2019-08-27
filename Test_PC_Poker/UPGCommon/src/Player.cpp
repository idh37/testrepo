#include "StdAfx.h"
#include "Player.h"

CPlayer::CPlayer(void)
{
	Clear();
}

CPlayer::~CPlayer(void)
{
}

void CPlayer::Clear()
{
	ZeroMemory(&UI, sizeof(UI));

	ServPNum=0;
	PNum=0;
	nSndFxKind=0;
	MyIpCheck=-1;
	nRoundPrevMoney=0;
	ZeroMemory(&m_cRoomChangeInfo, sizeof(CHANGE_USERINFO_ROOM));

	Reset();
}
void CPlayer::Reset()
{
	JoinState=0;
	PlayState=0;
	bFold=0;
	bFoldOpen=0;
}

void CPlayer::SetPlayerNum(int pnum)
{
	PNum = pnum;
}
void CPlayer::SetNewPlayer(USERINFO *pUI)
{
	ASSERT(pUI != NULL);
	memcpy(&UI, pUI, sizeof(USERINFO));
}

void CPlayer::SetGold(INT64 gold)
{
	nRealGold = gold;
	nPlayGold = nRealGold;
}

void CPlayer::SetChangeRoomInfo(CHANGE_USERINFO_ROOM &changeInfo)
{
	memcpy(&m_cRoomChangeInfo, &changeInfo, sizeof(CHANGE_USERINFO_ROOM));
}