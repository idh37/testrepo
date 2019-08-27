#include "StdAfx.h"
#include "HighValueEffect.h"

namespace effect
{

CHighValueEffect::CHighValueEffect()
	:m_bJackpot( false ),
	m_dwWinDrawTime( 0 ),
	m_dwCurTime( 0 ),
	m_pMultiSceneJackpot( NULL )
{		
	m_pMultiScene = NULL;
	pSpr = NULL;
}

CHighValueEffect::~CHighValueEffect()
{	
	Destroy();
}


//int pnum, int wincase , int valuekind , CString str
void CHighValueEffect::Init( GAMERESULTDATA *data, bool bJackpPot )
{
	if( data == NULL )return;
	/*
	wincase = 0 승 , 1 패 
	*/
	m_nObjID = ANF_HIGHVALUE;
	DrawFaceKind = DRAW_TOP;

	memcpy(&m_stResultData,data,sizeof(m_stResultData));

	if( m_stResultData.nResultMoney <= 0 ) return;

	bShowValue = FALSE;

	if ( m_pMultiScene != NULL)
	{
		delete m_pMultiScene;
		m_pMultiScene = NULL;
	}

	m_dwWinDrawTime = timeGetTime();

	pSpr = &GLOBAL_SPR[ spr::GSPR_ET_HIVALUE ];	

	int nScenNumber = -1;

	int nValue = m_stResultData.valuekind;

	m_bJackpot = bJackpPot;

	switch( nValue ) 
	{
	case CBadugiRule::B4C_GOLF:
		{
			nScenNumber = 0;
		} break;
	case CBadugiRule::B4C_SECOND:
		{
			nScenNumber = 1;
		} break;
	case CBadugiRule::B4C_THIRD:
		{
			nScenNumber = 2;
		} break;		
	}

	if ( nScenNumber == -1 )
	{
		return;
	}

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene( 0, 0, NULL, &g_pGameView->Page, pSpr, nScenNumber );
	m_pMultiScene->StartMScene(1);

	if ( m_bJackpot )
	{
		m_pMultiSceneJackpot = new NMBASE::SKINGDI::CNMultiScene( 0, 0, NULL, &g_pGameView->Page, pSpr, 3 );
		m_pMultiSceneJackpot->StartMScene(1);
	}

	m_rtstrID = g_cUIData.m_rtHighJokboUserName;
	m_rtstrID.OffsetRect(g_cUIData.m_ptHighJokboUserName);

	m_rtstrJokbo = g_cUIData.m_rtHighJokboName;
	m_rtstrJokbo.OffsetRect(g_cUIData.m_ptHighJokboName);

	m_rtstrMoney = g_cUIData.m_rtHighJokboWinMoney;
	m_rtstrMoney.OffsetRect(g_cUIData.m_ptHighJokboWinMoney);

	m_rtJackPot = g_cUIData.m_rtHighJokboJacpotMoney;
	m_rtJackPot.OffsetRect(g_cUIData.m_ptHighJokboJacpotMoney);

	m_strMoney = (CString)NMBASE::UTIL::g_MoneyMark( m_stResultData.nResultMoney );
	m_strJackPotMoney = (CString)NMBASE::UTIL::g_MoneyMark( g_GAME_BD()->m_i64JackPotActive );


	MM().Call(UPG_CMK_SET_JACKPOTEFFECT_SHOW, (WPARAM)0, (LPARAM)TRUE);
}

void CHighValueEffect::Destroy()
{		
	SAFE_DELETE( m_pMultiScene );
	SAFE_DELETE( m_pMultiSceneJackpot );
	bShowValue = FALSE;
}

int CHighValueEffect::ProcessObject()
{	
	m_dwCurTime = timeGetTime() - m_dwWinDrawTime; 		

	if ( m_pMultiScene != NULL  || m_pMultiSceneJackpot )
	{
		if ( m_pMultiScene && m_pMultiScene->m_stSceneState.bScenePlay == TRUE )
		{
			NMBASE::GRAPHICGDI::QUERYMSCENE_SCRIPT *SCRIPT = m_pMultiScene->OnTimer();

			if ( m_pMultiSceneJackpot )
			{
				m_pMultiSceneJackpot->OnTimer();
			}

			if( SCRIPT == NULL )
			{
				bShowValue = TRUE;			
			}
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}
	else
	{
		Destroy();
		return PROCESS_KILLME;
	}

	return PROCESS_OK;
}

void CHighValueEffect::DrawObject(CDC *pDC)
{
	if ( pDC == NULL )return;

	if ( m_pMultiScene != NULL )
	{
		m_pMultiScene->DrawMScene( NULL );

		if ( bShowValue )
		{
			if ( m_dwCurTime >= 810 )
			{	
				//족보 일때 금액, 카드 , 아이디 그려줌
				CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
				COLORREF clrOld = pDC->SetTextColor(g_cUIData.m_clrHighJokboUserName);
				pDC->DrawText( m_stResultData.ID, &m_rtstrID, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				pDC->SetTextColor(g_cUIData.m_clrHighJokboName);
				pDC->DrawText( m_stResultData.str_value, &m_rtstrJokbo, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14_BOLD));
				pDC->SetTextColor(g_cUIData.m_clrHighJokboWinMoney);
				pDC->DrawText( m_strMoney.c_str(), &m_rtstrMoney, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

				pDC->SetTextColor(clrOld);
				pDC->SelectObject(pOldFont);
			}

            if(m_dwCurTime >= 3600)
            {
				if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
				{
					MM().Call(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, WPARAM(0), LPARAM(0));
				}
				else
				{
	                MM().Call(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, (WPARAM)0, (LPARAM)TRUE);
				}
            }
		}
	}

	//if (  bShowValue && m_pMultiSceneJackpot != NULL )
	//{
	//	m_pMultiSceneJackpot->DrawMScene( NULL );
	//	//잭팟 머니를 표시한다. 1.1초후 잭팟 머니를 표시한다.

	//	if ( m_bJackpot && m_dwCurTime >= 800 && m_dwCurTime <= 2800  )
	//	{ 

	//		pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
	//		pDC->SetTextColor(g_cUIData.m_clrHighJokboJacpotMoney);
	//		pDC->DrawText( m_strJackPotMoney.c_str(), &m_rtJackPot, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	//	}

	//}
}

}


