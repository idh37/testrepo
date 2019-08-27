#include "stdafx.h"
#include "TotalBetMoney.h"
#include "../UIInfoData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTotalBetMoney::CTotalBetMoney()
{
	m_pUIInfoData = NULL;
	m_pGameProcess = NULL;
	m_bDraw = false;
}

CTotalBetMoney::~CTotalBetMoney()
{
}

void CTotalBetMoney::LoadImage(LPCTSTR szPathTotalMoney, LPCTSTR szPathCallMoney)
{
	if (szPathTotalMoney)
		AddLoadImageList(&m_sprTotalMoney,szPathTotalMoney, true);
	else
		AddLoadImageList(&m_sprTotalMoney,".\\PokerCommon\\data\\TotalMoney.spr", true);
	
	if (szPathCallMoney)
		AddLoadImageList(&m_sprCallMoney,szPathCallMoney, true);
	else
		AddLoadImageList(&m_sprCallMoney,".\\PokerCommon\\data\\CallMoney.spr", true);

	AddLoadImageList(&m_sprTotalGold,".\\PokerCommon\\data\\TotalGold.spr", true);
	AddLoadImageList(&m_sprCallGold,".\\PokerCommon\\data\\CallGold.spr", true);
}

void CTotalBetMoney::Init(CUIInfoData *pUIInfoData, CGameProcess *pGameProcess, NMBASE::GRAPHICGDI::CPage *pPage)
{
	m_pUIInfoData = pUIInfoData;
	m_pGameProcess = pGameProcess;

 	m_sprPublicTotalMoney.Init(pPage,&m_sprTotalMoney,m_pUIInfoData->m_rtBackTotalMoney,DT_RIGHT, 4, false, -1, &m_sprTotalGold);
 	m_sprPublicCallMoney.Init(pPage,&m_sprCallMoney,m_pUIInfoData->m_rtBackCallMoney,DT_RIGHT, 4, false,-1, &m_sprCallGold);
}

void CTotalBetMoney::Draw(CDC *pDC)
{
	if(!m_bDraw) return;
	INT64 roundingoff = m_pGameProcess->GetRealBet();
	
	// 총베팅액
	m_sprPublicTotalMoney.SetMoney(roundingoff, false);
	m_sprPublicTotalMoney.Draw(g_RI.ChipKind);			
	
	// 콜 금액 
	int spnum = m_pGameProcess->GetServPNum_ByPN(0);			
	INT64 CallMoney = m_pGameProcess->GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
	
	m_sprPublicCallMoney.SetMoney(CallMoney, false);
	m_sprPublicCallMoney.Draw(g_RI.ChipKind);
}

void CTotalBetMoney::Reset(void)
{
	m_bDraw = false;
	m_sprPublicTotalMoney.SetMoney(0, false);
	m_sprPublicCallMoney.SetMoney(0, false);
}

void CTotalBetMoney::Start(void)
{
	m_bDraw = true;
}