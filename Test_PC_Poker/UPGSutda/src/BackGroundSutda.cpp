#include "StdAfx.h"
#include "BackGroundSutda.h"

CBackGroundSutda::CBackGroundSutda(void)
{
}

CBackGroundSutda::~CBackGroundSutda(void)
{
}

void CBackGroundSutda::OnEnterRoom(void)
{

	m_dwStartTime = timeGetTime();
	m_bVipRoom = IsVIPRoom(&g_RI);
	m_nPlayIndex = 0 ; //vip

	m_dwBackPlayTotalTime = m_sprBack.GetMScenePlayTime(m_nPlayIndex);
	
	if(m_bVipRoom) 
		PBPlayEffectSound(SND_VIPROOMJOIN);

}
void CBackGroundSutda::DrawBottom(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	pPage->PutSprAuto(0,0, &m_sprBack, 0);

	if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD)
	{
		pPage->PutSprAuto(0,33, &m_sprBack, 15);
	}

	//pPage->PutSprAuto(332,115, &m_sprBack, 1);	//ÀÓ½Ã


}
void CBackGroundSutda:: DrawMiddle(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	pPage->DrawMultiScene(0,0, &m_sprBack, 1, 0); //Ä«µåµ¦

	if (m_bVipRoom)
	{
		DWORD dwBackTime = timeGetTime() - m_dwStartTime;
		if(dwBackTime >= m_dwBackPlayTotalTime) dwBackTime = m_dwBackPlayTotalTime - 1;
		pPage->DrawMultiScene(0,0, &m_sprBack, m_nPlayIndex, dwBackTime, FALSE);
	}

}

