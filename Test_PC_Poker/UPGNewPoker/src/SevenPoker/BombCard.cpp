// BombCard.cpp: implementation of the CBombCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BombCard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBombCard::CBombCard()
{
	m_nStatus = BCS_NOSHOW;
}

CBombCard::~CBombCard()
{
}

void CBombCard::Clear()
{
	m_nStatus = BCS_NOSHOW;
	m_ptPos = CPoint(0, 0);
}

void CBombCard::ForceSet(int nCardNo)
{
	m_nCardNo = nCardNo;
	m_nStatus = BCS_BOMBRESET;
	m_ptPos = g_cUIData.m_ptBonusCardBack + g_cUIData.m_ptBonusCardGab;
}

void CBombCard::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{	
	switch(m_nStatus)
	{
	case BCS_NOSHOW:
		break;
	case BCS_DEALING:
		m_cMove.DrawSprite(pPage);
		break;
	case BCS_BOMBSET_EFFECT:
		//g_cUIData.m_ptBonusCardBack + g_cUIData.m_ptBonusCardGab - CPoint(50, 30), 
		m_cMove.DrawSprite(pPage);
	case BCS_NOTDEAL_BOMBSET:
		pPage->PutSprAuto(m_ptPos.x , m_ptPos.y , &g_sprSmallBombCard, 52);
		break;
	case BCS_DEAL_SHOW:
		{
			POINT pt = m_cMove.GetCurPos();
			pPage->PutSprAuto(pt.x, pt.y, &g_sprBigBombCard, 52);
		}
		break;
	case BCS_DEAL_BOMBSET_EFFECT:
		//g_cUIData.m_ptBonusOpen - CPoint(50, 30), 
		m_cMove.DrawSprite(pPage);
		pPage->PutSprAuto(m_ptPos.x , m_ptPos.y , &g_sprBigBombCard, m_nCardNo);
		break;
	case BCS_DEAL_JOKER_EFFECT:
		m_cMove.DrawSprite(pPage);
		pPage->PutSprAuto(m_ptPos.x , m_ptPos.y , &g_sprBigBombCard, 53);
		break;
	case BCS_DEAL_BOMBRESET:
		{
			CPoint pt = m_cMove.GetCurPos();

			if(m_nCardNo == 100)
			{
				pPage->PutSprAuto(pt.x, pt.y, &g_sprBigBombCard, 52);
			}
			else
			{
				pPage->PutSprAuto(pt.x, pt.y, &g_sprBigBombCard, m_nCardNo);
			}
		}
		break;
	case BCS_BOMBRESET:
		if(m_nCardNo == 100)
		{
			pPage->PutSprAuto(m_ptPos.x , m_ptPos.y , &g_sprSmallBombCard, 53);
		}
		else
		{
			pPage->PutSprAuto(m_ptPos.x , m_ptPos.y , &g_sprSmallBombCard, m_nCardNo);
		}
		break;
	}
}

void CBombCard::SetBombCard()
{	
	m_cMove.StartMoveMSprite(&g_sprBombCardDealing, m_ptPos, g_cUIData.m_ptBonusCardBack + g_cUIData.m_ptBonusCardGab, 0, 0);
	m_nStatus = BCS_DEALING;
}

void CBombCard::SetOPenBombCard(char cardno)
{	
	switch(g_RI.FormKind)
	{
	case BOMB_GAME:
		g_pGameView->m_cDisplayBoard.SetText(EDBT_BOMBCARD, (LPARAM)strCard_Name[cardno]);
		break;
	case JOKER_GAME:
		if(g_bGetJokerCard) g_pGameView->m_cDisplayBoard.SetText(EDBT_JOKERCARD);
		else g_pGameView->m_cDisplayBoard.SetText(EDBT_BOMBCARD, (LPARAM)strCard_Name[cardno]);
		break;
	}

	//g_cUIData.m_dwBombCardOpenMoveTime == 200 . 폭탄카드 딜레이. 이 값이 커질 수록 폭탄카드가 중앙으로 날라왔다가 돌아가는 시간이 길어짐.
	m_cMove.StartMove(m_ptPos, g_cUIData.m_ptBonusOpen, 0, g_cUIData.m_dwBombCardOpenMoveTime);
	m_nStatus = BCS_DEAL_SHOW;
	m_nCardNo = cardno;	
	PBPlayEffectSound(SND_BONUS_MOVE_CENTER);
}

void CBombCard::RePtBombCard(void)
{
	m_cMove.StartMove(m_ptPos,  g_cUIData.m_ptBonusCardBack + g_cUIData.m_ptBonusCardGab, 0, g_cUIData.m_dwBombCardOpenMoveTime);
	m_nStatus = BCS_DEAL_BOMBRESET;
	GAME()->OnOPenBombCard(m_nCardNo);
}

void CBombCard::OnSetBombCard(DWORD dwTimeGab)
{
	m_cMove.StartMoveImage(&g_sprBombSetAni, g_cUIData.m_ptBonusCardBack + g_cUIData.m_ptBonusCardGab + g_cUIData.m_ptBombEffectGab, m_ptPos, dwTimeGab, 0, LE);
	m_nStatus = BCS_BOMBSET_EFFECT;
}

void CBombCard::OnShowBombCard(DWORD dwTimeGab)
{
	m_cMove.StartMoveImage(&g_sprBombSetAni, g_cUIData.m_ptBonusOpen + g_cUIData.m_ptBombEffectGab, m_ptPos, dwTimeGab, 0, LE);
	m_nStatus = BCS_DEAL_BOMBSET_EFFECT;
}

void CBombCard::OnShowJokerBombCard(void)
{
	m_cMove.StartMoveSSprite(&g_sprJokerCardEffect, g_cUIData.m_ptJokerEffectGab, m_ptPos, 0, 0);
	m_nStatus = BCS_DEAL_JOKER_EFFECT;
	PBPlayEffectSound(SND_JOKERSHOW);
}

void CBombCard::OnTimer()
{
	if(m_nStatus == BCS_NOSHOW) return;

	m_cMove.OnTimer();
	if(m_cMove.IsMoveEnd())
	{
		m_ptPos = m_cMove.GetCurPos();
		switch(m_nStatus)
		{
		case BCS_DEALING:
			OnSetBombCard(10);
			break;
		case BCS_BOMBSET_EFFECT:
			m_nStatus = BCS_NOTDEAL_BOMBSET;
			break;
		case BCS_DEAL_SHOW:
			//폭탄이 조커일 경우 조커 이펙트 재생
			if(m_nCardNo == 100) OnShowJokerBombCard();
			//일반일때는 폭탄 이펙트 재생
			else OnShowBombCard(10);
			break;
		case BCS_DEAL_BOMBSET_EFFECT:
		case BCS_DEAL_JOKER_EFFECT:
			RePtBombCard();
			break;
		case BCS_DEAL_BOMBRESET:
			m_nStatus = BCS_BOMBRESET;
			break;
		}
	}
}