#include "stdafx.h"
#include "BackGround.h"
#include "../UIInfoData.h"
#include "../Sound.h"

CBackGround::CBackGround()
{
	m_pUIInfoData = NULL;
	m_dwStartTime = 0;
	m_bVipRoom = false;
	m_nPlayIndex = 1;
	m_dwBackPlayTotalTime = 0;
	m_nDeckIndex = 5;
	m_bResult = false;
}

CBackGround::~CBackGround()
{
}

void CBackGround::LoadImage(LPCTSTR lpBackPath)
{
	AddLoadImageList(&m_sprBack,lpBackPath,true);
}

void CBackGround::Init(CUIInfoData *pUIInfoData, CGameViewBase *pView)
{
	m_pUIInfoData = pUIInfoData;
}

void CBackGround::OnEnterRoom(void)
{
	m_dwStartTime = timeGetTime();
	m_bVipRoom = IsVIPRoom(&g_RI);
	m_nPlayIndex = m_bVipRoom?0:1;

	//°ñµå °æ±âÀå back multiscene index
	if(g_RI.ChipKind==1)
	{
		if(GM().GetCurrentGameType()==IDX_GAME_SP)	m_nPlayIndex = 2;
		if(GM().GetCurrentGameType()==IDX_GAME_BD)	m_nPlayIndex = 3;
		if(GM().GetCurrentGameType()==IDX_GAME_NP)	m_nPlayIndex = 2;
	}

	m_dwBackPlayTotalTime = m_sprBack.GetMScenePlayTime(m_nPlayIndex);

	m_nDeckIndex = m_bVipRoom?5:6;

	if(m_bVipRoom) PBPlayEffectSound(SND_VIPROOMJOIN);
}

void CBackGround::DrawBottom(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	DWORD dwBackTime = timeGetTime() - m_dwStartTime;
	if(dwBackTime >= m_dwBackPlayTotalTime) dwBackTime = m_dwBackPlayTotalTime - 1;

	pPage->DrawMultiScene(0, 0, &m_sprBack, m_nPlayIndex, dwBackTime, FALSE);

	// ÆøÅº BG  ±×¸®±â
	if(g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME){
		if(GM().GetCurrentGameType()!=IDX_GAME_HA)
		pPage->PutSprAuto(m_pUIInfoData->m_ptBonusCardBack.x, m_pUIInfoData->m_ptBonusCardBack.y, &m_sprBack,11);
	}
	//pPage->DrawMultiScene(0, 0, &m_sprBack, 3, timeGetTime(), TRUE);
}

void CBackGround::DrawMiddle(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	pPage->PutSprAuto(m_pUIInfoData->m_ptCardDeck.x, m_pUIInfoData->m_ptCardDeck.y, &m_sprBack, m_nDeckIndex, AB);
}

void CBackGround::DrawTop(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
}