// AutoBetSetUpWnd.cpp: implementation of the CAutoBetSetUpWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoBetSetUpWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoBetSetUpWnd::CAutoBetSetUpWnd()
{
	// [자동베팅] 2004.07.15
	m_IsAutoKindBet = 0;
	m_bAutoBet		= FALSE;
	m_nAutoBetCnt	= 0;
}

CAutoBetSetUpWnd::~CAutoBetSetUpWnd()
{

}


void CAutoBetSetUpWnd::Init()
{
	m_pPage = &g_pGameView->Page;
}
	
void CAutoBetSetUpWnd::Set_auto_betting_flag()
{		
	m_nAutoBetCnt = 26;// 지연시간
	m_bAutoBet = TRUE;	
}

void CAutoBetSetUpWnd::auto_betting_call()
{	
	//[7월정책변경]2008.07.01	
	if(g_Poker.nAutoBetCmd > 0 && g_RI.nAutoBetKind > 0 ){
		auto_betting_func();
	}		
	else{
		if( g_RI.nAutoBetKind <= 0 ){
			if( GetIsAKindBet() == 0) return;
			
			if( GetIsAKindBet() >= g_Poker.nRound ) {
				auto_betting_func();
			}	
		}
	}
}

void CAutoBetSetUpWnd::auto_betting_func()
{	
	if( g_pGameView->m_cGameViewBtnMan.m_bBMax ) {
		g_pGameView->OnBtnMax();
	}
	else if( g_pGameView->m_cGameViewBtnMan.m_bBDDa ) {
		g_pGameView->OnBtnDda();
	}
	else if( g_pGameView->m_cGameViewBtnMan.m_bBCheck ) {
		g_pGameView->OnBtnCheck();
	}
	else if( g_pGameView->m_cGameViewBtnMan.m_bBCall ) {
		g_pGameView->OnBtnCall();
	}	
	else if(g_pGameView->m_cGameViewBtnMan.m_bBPing ) {
		g_pGameView->OnBtnPing();
	}
}

void CAutoBetSetUpWnd::SetIsAKindBet(int value)
{ 	
	m_IsAutoKindBet = value; 			
}

void  CAutoBetSetUpWnd::ReSetAutoBettingButton(int num)
{	
	if(num == 0){
		SetShowWnd(TRUE);		
		SetIsAKindBet( g_Config.nAutoBet);
		return;
	}

	
}

void CAutoBetSetUpWnd::OnEnterRoom()
{
	if(g_RI.nAutoBetKind == 0)
	{		
		SetIsAKindBet( g_Config.nAutoBet);
		SetShowWnd(TRUE);
	}
	else{
		SetShowWnd(FALSE);
	}	
}

void CAutoBetSetUpWnd::OnTimer()
{
	if( m_bAutoBet ) {
		if( m_nAutoBetCnt-- < 0 ){
			m_nAutoBetCnt = 0;
			m_bAutoBet = FALSE;
			auto_betting_call();						
		}
	}
}

void CAutoBetSetUpWnd::Draw_Top(CDC *pDC)
{	
	if( m_pPage != NULL ){
		if( g_RI.State == 1 && g_Poker.nAutoBetCmd > 0 ){
			m_pPage->DrawMultiScene(g_cUIData.m_ptAutoBettingPos.x, g_cUIData.m_ptAutoBettingPos.y, &g_sprAutoBettingInfoMsg, 0, timeGetTime());
		}			
	}	
}

bool CAutoBetSetUpWnd::OnCommand( WPARAM wParam )
{
	return false; 
}

void CAutoBetSetUpWnd::SetShowWnd(BOOL bshow)
{
	m_bShow = bshow;

	if( g_RI.nAutoBetKind > 0 || g_ObserverMan.IsMyObserver() ){
		m_bShow = FALSE;
	}
}

CAutoBetSetUpWnd& CAutoBetSetUpWnd::GetInstance()
{
	static CAutoBetSetUpWnd AutoBetSetUpWnd;
	return AutoBetSetUpWnd;
}

CAutoBetSetUpWnd& AutoBetSetUpWnd()
{
	return CAutoBetSetUpWnd::GetInstance();
}
