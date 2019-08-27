// AutoBetSetUpWnd.cpp: implementation of the CAutoBetSetUpWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AutoBetSetUpWnd.h"
#include "GlobalGame.h"

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
	//string str;	
	//str = ".\\7poker\\data\\";	

	//_LOADSPRITEDEFINE(AutoBetSetUpSpr		, AUTOBETSETUP_PATH);
	//_LOADSPRITEDEFINE(AutoBettingInfoMsgSpr , AUTOBETPANEL_PATH);	

	//m_AutobetBtn[0].Init(g_pGameView, &g_pGameView->Page, 794, 480, &AutoBetSetUpSpr, 0, IDM_AUTO_OFF,4,TRUE);
	//m_AutobetBtn[1].Init(g_pGameView, &g_pGameView->Page, 851, 480, &AutoBetSetUpSpr, 1, IDM_AUTO_M,4,TRUE);
	//m_AutobetBtn[2].Init(g_pGameView, &g_pGameView->Page, 908, 480, &AutoBetSetUpSpr, 2, IDM_AUTO_N,4,TRUE);
	//m_AutoSetupBtn.Init(g_pGameView, &g_pGameView->Page, 970,472, &AutoBetSetUpSpr, 3, IDM_AUTO_BTN,4,TRUE);	
	//m_AutoSetupBtn.Show(TRUE);
	nAutoBet = 0;
}

BOOL CAutoBetSetUpWnd::IsObserver_Valid()
{
	if( g_ObserverMan.IsMyObserver() == TRUE )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAutoBetSetUpWnd::IsChan_Valid()
{		
	return FALSE; //사용안함

// 	if( g_GetGroupGrade(g_NowGroupNo) >= GROUP_GRADE_PLATINUM2 )
// 	{
// 		return FALSE;
// 	}

	return TRUE;
}

//void CAutoBetSetUpWnd::set_auto_betting_btn(BOOL bClear)
//{
//	//if( !IsObserver_Valid())
//	//	return;	
//	//if( !IsChan_Valid())
//	//	return;	
//
//	//if(g_RI.nAutoBetKind == 0)
//	//{
//	//	if( bClear ) 
//	//	{
//	//		m_AutobetBtn[0].Show( TRUE  );
//	//		m_AutobetBtn[1].Show( TRUE  );			
//	//		m_AutobetBtn[2].Show( TRUE  );
//	//		m_AutobetBtn[0].Enable(FALSE);
//	//		m_AutobetBtn[1].Enable(TRUE);
//	//		m_AutobetBtn[2].Enable(TRUE);
//	//		return;
//	//	}
//
//	//	int value = GetIsAKindBet();
//	//	if( value == 0)
//	//	{
//	//		m_AutobetBtn[0].Show( TRUE);
//	//		m_AutobetBtn[1].Show( TRUE );
//	//		m_AutobetBtn[2].Show( TRUE  );
//
//	//		m_AutobetBtn[0].Enable(FALSE);
//	//		m_AutobetBtn[1].Enable(TRUE);
//	//		m_AutobetBtn[2].Enable(TRUE);
//	//	}
//	//	else if( value == 1)
//	//	{
//
//	//		m_AutobetBtn[0].Show( TRUE);
//	//		m_AutobetBtn[1].Show( TRUE );
//	//		m_AutobetBtn[2].Show( TRUE  );
//
//	//		m_AutobetBtn[0].Enable(TRUE);
//	//		m_AutobetBtn[1].Enable(FALSE);
//	//		m_AutobetBtn[2].Enable(TRUE);
//	//	}
//	//	else if( value == 2)
//	//	{
//	//		m_AutobetBtn[0].Show( TRUE);
//	//		m_AutobetBtn[1].Show( TRUE );
//	//		m_AutobetBtn[2].Show( TRUE );
//
//	//		m_AutobetBtn[0].Enable(TRUE);
//	//		m_AutobetBtn[1].Enable(TRUE);	
//	//		m_AutobetBtn[2].Enable(FALSE);	
//	//	}		
//	//}
//	//else
//	//{
//	//	//m_AutoSetupBtn.Show(FALSE);
//	//	m_AutoSetupBtn.Enable(FALSE);
//	//	m_AutobetBtn[1].Show( FALSE);
//	//	m_AutobetBtn[0].Show( FALSE );
//	//	m_AutobetBtn[2].Show( FALSE );		
//	//}
//}


void CAutoBetSetUpWnd::Set_auto_betting_flag()
{		
	if( !IsObserver_Valid())
		return;		
	m_nAutoBetCnt = g_GameSpeed.get_AutoBetActiveDelay(0,0);// 지연시간
	m_bAutoBet = TRUE;	
}

void CAutoBetSetUpWnd::auto_betting_call()
{		
	//[7월정책변경]2008.07.01	
	if(g_Poker.nAutoBetCmd > 0)
	{
		auto_betting_func();
	}		
	else
	{
		if( g_RI.nAutoBetKind <= 0 )
		{
			if( GetIsAKindBet() == 0) 
				return;

			if( GetIsAKindBet() >= g_Poker.nRound ) 
			{
				auto_betting_func();
			}
		}
	}
}

void CAutoBetSetUpWnd::auto_betting_func()
{	
	if(g_pGameView->m_cGameViewBtnMananager.m_bBDDa )
	{
		g_GAMEPROCESS()->SendDdaButton();
	}
	else if(g_pGameView->m_cGameViewBtnMananager.m_bBCheck ) 
	{
		g_GAMEPROCESS()->SendCheckButton();
	}
	else if( g_pGameView->m_cGameViewBtnMananager.m_bBCall ) 
	{
		g_GAMEPROCESS()->SendCallButton();
	}	
	else if(g_pGameView->m_cGameViewBtnMananager.m_bBPing )
	{
		g_GAMEPROCESS()->SendPingButton();
	}
}

void CAutoBetSetUpWnd::SetIsAKindBet(int value)
{ 	
	if( !IsObserver_Valid())
		return;		
	if( g_NowGroupClass > IDX_GROUPCLASS_MASTER )
	{
		m_IsAutoKindBet = 0;
		return;
	}
	m_IsAutoKindBet = value;
}

void CAutoBetSetUpWnd::set_GameMode()
{	
	/*m_AutobetBtn[1].Init(g_pGameView, &g_pGameView->Page, 851, 480, &AutoBetSetUpSpr, 5, IDM_AUTO_M,4,TRUE);
	m_AutobetBtn[2].Init(g_pGameView, &g_pGameView->Page, 908, 480, &AutoBetSetUpSpr, 6, IDM_AUTO_N,4,TRUE);*/
}

void CAutoBetSetUpWnd::OnEnterRoom()
{
	if( !IsObserver_Valid())
		return;		

	//m_IsAutoKindBet 사용 하지 않는다.
	if( g_NowGroupClass > IDX_GROUPCLASS_MASTER)
	{
		m_IsAutoKindBet = 0;
	}

	set_GameMode();
	//set_auto_betting_btn(0);	

	if(g_RI.nAutoBetKind == 0)
	{		
		SetIsAKindBet( nAutoBet);
		/*if(nAutoBet ==0)
			set_auto_betting_btn( TRUE );				
		else
			set_auto_betting_btn( FALSE );*/
		SetShowWnd(TRUE);
	}
	else
	{
		//set_auto_betting_btn();
		SetShowWnd(FALSE);
	}	
}

void CAutoBetSetUpWnd::OnTimer()
{
	if( m_bAutoBet )
	{
		if( m_nAutoBetCnt-- < 0 )
		{
			m_nAutoBetCnt = 0;
			m_bAutoBet = FALSE;
			auto_betting_call();						
		}
	}
}

void CAutoBetSetUpWnd::Draw(CDC *pDC)
{	
	if( !IsObserver_Valid())
		return;	
	if( !IsChan_Valid())
		return;		

	/*if( g_RI.nAutoBetKind == 0 )
	{
		m_AutoSetupBtn.Draw(pDC);
	}	*/

	if( !IsShow() )
		return;
	if( m_pPage == NULL )
		return;
	//[7월정책변경]2008.07.01
	// 그래픽 버튼 그리기	

	//m_pPage->DrawScene(784,472,&AutoBetSetUpSpr,28,1);


	//m_AutobetBtn[0].Draw(pDC);
	//m_AutobetBtn[1].Draw(pDC);	
	//m_AutobetBtn[2].Draw(pDC);	

}

void CAutoBetSetUpWnd::Draw_Top(CDC *pDC)
{	
	if( m_pPage != NULL )
	{
		if( g_RI.State == EGAME_PLAYING && g_Poker.nAutoBetCmd > 0 )	
		{
			m_pPage->DrawMultiScene(g_cUIData.m_ptAutoBettingPos.x, g_cUIData.m_ptAutoBettingPos.y, &g_SprAutoBettingInfoMsg, 0, timeGetTime());
		}			
	}
}

bool CAutoBetSetUpWnd::OnCommand( WPARAM wParam )
{
	if( !IsObserver_Valid())
		return false;	
	if( !IsChan_Valid())
		return false;	

	switch(wParam) 
	{

		//[7월정책변경]2008.07.01
		// [자동베팅]	0:X 1:39 2:49 3:59 4:69
	case IDM_AUTO_BTN:	
		{				
			//[7월정책변경]2008.07.01
			if(g_RI.nAutoBetKind == 0)
			{
				BOOL bshow = !m_bShow;
				SetShowWnd(bshow);
//				set_auto_betting_btn(FALSE);
			}
		}
		break;
	case IDM_AUTO_OFF:// 자동 배팅 오프
		{
			SetIsAKindBet( 0 );
//			set_auto_betting_btn( TRUE );
			nAutoBet = 0;
			SetShowWnd(FALSE);
		}
		break;

	case IDM_AUTO_M:// 자동 배팅 오프
		{
			SetIsAKindBet( 1 );			
//			set_auto_betting_btn();
			if(g_RI.State == EGAME_PLAYING)
			{
				Set_auto_betting_flag();
			}
			nAutoBet = 1;
			SetShowWnd(FALSE);
		}
		break;

	case IDM_AUTO_N:
		{
			SetIsAKindBet( 2 );
//			set_auto_betting_btn();

			if(g_RI.State == EGAME_PLAYING)	// (g_RI.State -> 0:대기중 1:게임중)
			{
				Set_auto_betting_flag();
			}
			nAutoBet = 2;
			SetShowWnd(FALSE);
		} 
		break;

	default:
		{
			return false;
		}

	}
	return true; 
}

void CAutoBetSetUpWnd::OnLButtonDown(int x, int y)
{
	if( !IsObserver_Valid())
		return;	
	if( !IsChan_Valid())
		return;	

	//[7월정책변경]2008.07.01
	/*m_AutobetBtn[0].OnLButtonDown(x, y);			
	m_AutobetBtn[1].OnLButtonDown(x, y);	
	m_AutobetBtn[2].OnLButtonDown(x, y);	
	m_AutoSetupBtn.OnLButtonDown(x, y);	*/
}

void CAutoBetSetUpWnd::OnLButtonUp(int x, int y)
{
	if( !IsObserver_Valid())
		return;	
	if( !IsChan_Valid())
		return;	

	/*m_AutobetBtn[0].OnLButtonUp(x, y);
	m_AutobetBtn[1].OnLButtonUp(x, y);
	m_AutobetBtn[2].OnLButtonUp(x, y);
	m_AutoSetupBtn.OnLButtonUp(x, y);	*/
}

void CAutoBetSetUpWnd::OnMouseMove(int x, int y)
{
	if( !IsObserver_Valid())
		return;	
	if( !IsChan_Valid())
		return;	

	////[7월정책변경]2008.07.01
	//m_AutobetBtn[0].OnMouseMove(x, y);
	//m_AutobetBtn[1].OnMouseMove(x, y);	
	//m_AutobetBtn[2].OnMouseMove(x, y);	
	//m_AutoSetupBtn.OnMouseMove(x, y);	
}

void CAutoBetSetUpWnd::SetShowWnd(BOOL bshow)
{
	if( !IsObserver_Valid())
		return;	
	if( !IsChan_Valid())
		return;	

	m_bShow = bshow;

	if( g_RI.nAutoBetKind > 0 || g_ObserverMan.IsMyObserver() )
	{
		m_bShow = FALSE;
	}

	/*if( m_bShow )
	{
		m_AutoSetupBtn.Init(g_pGameView, &g_pGameView->Page, 970,472, &AutoBetSetUpSpr, 4, IDM_AUTO_BTN,4,TRUE);		
	}
	else
	{
		m_AutoSetupBtn.Init(g_pGameView, &g_pGameView->Page, 970,472, &AutoBetSetUpSpr, 3, IDM_AUTO_BTN,4,TRUE);		
	}	*/

	/*if( g_RI.nAutoBetKind > 0 || g_ObserverMan.IsMyObserver() )
	{
		m_AutoSetupBtn.Show(FALSE);
		m_AutoSetupBtn.Enable(FALSE);		
	}*/
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
