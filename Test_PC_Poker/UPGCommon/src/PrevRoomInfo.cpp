#include "StdAfx.h"
#include "PrevRoomInfo.h"

CPrevRoomInfo::CPrevRoomInfo(void)
{

	ZeroMemory(m_PrevRoomInfo, sizeof(m_PrevRoomInfo));
}

CPrevRoomInfo::~CPrevRoomInfo(void)
{
}




void CPrevRoomInfo::SetPrevRoomInfo(IDX_GAME eGameIdx, ROOMINFO *pRI)
{
	if (eGameIdx < 0 || eGameIdx >= MAX_GAME)
	{
		return;
	}	

	m_PrevRoomInfo[eGameIdx] = *pRI;
}
ROOMINFO *CPrevRoomInfo::GetPrevRoomInfo(IDX_GAME eGameIdx)
{
	if (eGameIdx < 0 || eGameIdx >= MAX_GAME)
	{
		return NULL;
	}	

	return &m_PrevRoomInfo[eGameIdx];
}
 
BOOL CPrevRoomInfo::IsPrevJoinedRoom(SMALLROOMINFO *pSRI)
{
	//���� ���� ������ üũ

	//���ȣ�� ������ð��� ���Ѵ�.
	IDX_GAME eGameIdx = (IDX_GAME) pSRI->nGameIndex;	
	if (eGameIdx < 0 || eGameIdx >= MAX_GAME)
	{
		return FALSE;
	}

	return (m_PrevRoomInfo[eGameIdx].RoomNum == pSRI->sRoomNum && m_PrevRoomInfo[eGameIdx].tCreatedTime == pSRI->tCreatedTime);
	
}


