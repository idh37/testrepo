#include "StdAfx.h"
#include "PlayerDraw_BD.h"

CPlayerDraw_BD::CPlayerDraw_BD(void)
:m_pPlayerBD( NULL )
{
	m_ptDealer = CPoint( 0, 0 );
	m_ptCutNum = CPoint( 0, 0 );
	m_ptTurn   = CPoint( 0, 0 );
}

CPlayerDraw_BD::~CPlayerDraw_BD(void)
{
	m_pPlayerBD = NULL;
}

void CPlayerDraw_BD::Init( CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager )
{
	CPlayerDraw::Init( pPlayer, nNum, pPage, pManager );
	
	m_pPlayerBD = static_cast< CPlayer_BD* >( m_pPlayer );

	
	//카드 시작 위치
	
	// 딜러 (카드 시작점을 기준으로한 상대좌표)
	CPoint ptCard = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_CARD_POS_0 + m_nPNum ) );	

	// 마지막장 
	int nGabX = 0;

	if ( nNum == 0 )
	{
		nGabX = g_cUIData.GetGabBD( ui::EGAB_CARD_POS_X );
	}
	else
	{
		nGabX = g_cUIData.GetGabBD( ui::EGAB_CARD_POS_OTHER_X );
	}

	ptCard.x += ( 3 * ( nGabX ) );
 
	int nSizeX = GLOBAL_SPR[ spr::GSPR_GAME_BIGCARD ].spr[0].xl;
	int nSizeY = GLOBAL_SPR[ spr::GSPR_GAME_BIGCARD ].spr[0].yl;

	ptCard.x += nSizeX;
	ptCard.y += nSizeY;

	m_ptDealer	= static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_DEALER_POS_0 + m_nPNum ) );	

	//컷팅 장수 표시 시작 점
	m_ptCutNum = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_CUTNUM_0 + m_nPNum ) );	


	// 결과 이펙트 위에 족보, 머니 좌표
	m_rtstrJokbo = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetRectBD( ui::ERECT_WININFO ); // 출력영역
	m_rtstrMoney = m_rtstrJokbo;

	CPoint ptJokbo = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_RESULT_0 + m_nPNum ) ); // 족보좌표

	CPoint ptGab;
	ptGab.x = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetGabBD( ui::EGAB_RESULT_X ); // 족보로 부터의 상대좌표 X
	ptGab.y = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetGabBD( ui::EGAB_RESULT_Y ); // 족보로 부터의 상대좌표 Y

	CPoint ptMoney = ptJokbo + ptGab;	// 머니 좌표
	m_rtstrJokbo.OffsetRect( ptJokbo );
	m_rtstrMoney.OffsetRect( ptMoney );

	// 히든키 오픈
	m_ptHiddenKey = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_HIDDEN_OPENKEY ) );	
}

// 내 족보 출력
void CPlayerDraw_BD::DrawJokboName(CDC *pDC)
{
	//히든 카드를 가지고 있는지 검사
 	if ( g_GAME_BD()->IsHiddenCardFlag() && !m_pManager->IsHiddenCardShow() ) 
 	{
 		return;
 	}

	if ( m_pJokboName != NULL && !g_bMyObserver && m_pPlayer->JoinState == 1 )
	{	
		if ( m_nPNum == 0 )
		{
			if ( g_PLAYER( 0 )->m_bShowValue ) // 카드 컷팅중 그리지 않기 위해서
			{
				m_pJokboName->DrawText(pDC);
			}
		}
		else
		{
			m_pJokboName->DrawText(pDC);
		}
	}	
}

void CPlayerDraw_BD::DrawBottom(CDC *pDC)
{
	CPlayerDraw::DrawBottom( pDC );

	m_pPlayerBD->GetEffectObjectList()->BottomDrawAll( pDC );
		
	Draw_CutBackAndNumber( pDC );		// 커팅 숫자
}

void CPlayerDraw_BD::DrawMiddle(CDC *pDC)
{
	CPlayerDraw::DrawMiddle( pDC );
	Draw_HiddenOpenImage( pDC ); // 히든카드 카드 위에 그린다( 오픈 버튼 )

	m_pPlayerBD->GetEffectObjectList()->MiddleDrawAll( pDC );

	// 내칩 그리기
	if ( m_pPlayerBD->m_cMyChip.IsMyChipShow() )
	{
		m_pPlayerBD->m_cMyChip.OnMyChipDraw();
	}
}

void CPlayerDraw_BD::DrawTop(CDC *pDC)
{
	CPlayerDraw::DrawTop( pDC );

	m_pPlayerBD->GetEffectObjectList()->TopDrawAll( pDC );
}

void CPlayerDraw_BD::DrawResult(CDC *pDC)
{
	// 바둑이 좌표 예외	
	CFont *oldf = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));

	if ( IsGameOver() && m_pManager->IsGameOver() && m_pPlayer->JoinState == 1 )
	{
		bool bWin = false;
// 		int nWinner = m_pGameProcess->GetPNum_ByServPN( m_pManager->GetWinnerPNum() );
// 		if(nWinner == m_nPNum) bWin = true;
		if ( ((CPlayerDrawManager_BD*)m_pManager)->IsLowBDWinnerPNum(m_pGameProcess->GetServPNum_ByPN(m_nPNum)) )
		{
			bWin =true;
		}

		DWORD dwDeltaGab = timeGetTime() - m_pManager->GetGameOverStartTime();

		int nIndex = m_nPNum;
		if(bWin == false)
		{
			nIndex += m_pManager->m_nMaxPlayer;
			if(g_RI.bIsMaxRoom)
			{
				if(m_pPlayer->UI.GetInMoney() < 1)
					nIndex += m_pManager->m_nMaxPlayer;
			}
			else if(g_RI.ChipKind==0 && m_pPlayer->UI.GetMoney() < 100) 
			{
				nIndex += m_pManager->m_nMaxPlayer;
			}
			else if(g_RI.ChipKind==1 && m_pPlayer->nRealGold < 1)
			{
				nIndex += m_pManager->m_nMaxPlayer;
			}
		}

		m_pPage->DrawMultiScene(0, 0, m_pManager->m_pWinLoseBackSpr, nIndex, dwDeltaGab, FALSE);		

		if(dwDeltaGab >= 200)
		{
			if(bWin)
			{			
				pDC->SetTextColor(m_pUIInfoData->m_clrResultJokboNameWinColor);
			}
			else 
			{				
				pDC->SetTextColor(m_pUIInfoData->m_clrResultJokboNameLoseColor);
			}

			int nServPNum = GM().GetCurrentProcess()->GetServPNum_ByPN(m_nPNum);
			CString strJokbo= m_pManager->m_listResultJokbo[nServPNum];

			// ### [ 관전기능 ] ###		
			if((!m_pPlayer->bFold && m_pManager->m_bOpen) || m_bDrawMyValue)
			{
				m_pManager->m_bHiddenCardShow = true;					
			}
			
			if(strJokbo == "")
			{
				if(bWin) strJokbo = "기권승";
				else strJokbo = "기권패";
			}

			pDC->DrawText(strJokbo, &m_rtstrJokbo, DT_LEFT | DT_SINGLELINE | DT_CENTER);

			CString rmstr = "";
			INT64 i64ResultMoney = m_pManager->m_listResultMoney[nServPNum];

			if(bWin)
			{
				rmstr.Format("+%s",NMBASE::UTIL::g_MoneyMark(i64ResultMoney, 2));
				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));
				pDC->SetTextColor(m_pUIInfoData->m_clrResultMoneyWinColor);
			}
			else 
			{
				rmstr.Format("%s",NMBASE::UTIL::g_MoneyMark(i64ResultMoney, 2));
				pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
				pDC->SetTextColor(m_pUIInfoData->m_clrResultMoneyLoseColor);
			}

			rmstr.Append(strChip_Name[g_RI.ChipKind]);

			pDC->DrawText(rmstr, &m_rtstrMoney,  DT_LEFT | DT_SINGLELINE | DT_CENTER);

			pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		}
	}

	pDC->SelectObject(oldf);
}


void CPlayerDraw_BD::DrawBossMark(CDC *pDC)
{	
 	if ( g_Poker.nImageBoss > -1 && g_Poker.nImageBoss == m_pPlayerBD->ServPNum && m_pPlayerBD->JoinState == 1 )
 	{		
 		//바둑이 딜러표시
 		m_pPage->PutSprAuto( m_ptDealer.x, m_ptDealer.y, &GLOBAL_SPR[ spr::GSPR_GAME_HOSTDEALER ], EDEALER_TYPE );
 	}
}

void CPlayerDraw_BD::Draw_HiddenOpenImage( CDC *pDC )
{
	if ( !g_ObserverMan.IsMyObserver() && m_nPNum == 0 && m_pPlayerBD->PlayState != 0 && g_Poker.nState  )
	{
		//히든 카드를 가지고 있는지 검사
		if ( !g_GAME_BD()->IsHiddenCardFlag() ) 
		{
			return;
		}

		CNMyCard* pNMyCard = &m_pPlayerBD->m_cMyCard[ 0 ];

		// 히든카드 이동중에는 바꾸기 이미지 그리지 않는다.
		for ( int i = 0; i < TOTAL_CARD_BD; i++ )
		{
			if ( !pNMyCard[ i ].IsHiddenCard() )
			{
				continue;
			}

			if ( !pNMyCard[ i ].IsHiddenMovingComplete() ) 
			{
				return;
			}
		}
	
		if ( !m_pManager->IsHiddenCardShow() )
		{
			// 히든 오픈 이미지
			m_pPage->PutSprAuto( m_ptHiddenKey.x, m_ptHiddenKey.y, &GLOBAL_SPR[ spr::GSPR_V_H_OPEN ], 1 ); 
		}
	}
}

void CPlayerDraw_BD::Draw_CutBackAndNumber( CDC* pDC )
{
	if ( m_pPlayerBD->PlayState != 1 )
	{
		return ;
	}

	// 아침, 점심, 저녁 배경
	//m_pPage->PutSprAuto( m_ptCutNum.x , m_ptCutNum.y , &GLOBAL_SPR[ spr::GSPR_GAME_CUT_BACK ] , m_nPNum );

	//컷팅 장수 표시
	int nYGab = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetGabBD( ui::EGAB_CUTNUM_Y );

	for ( int i = 0 ; i < 3 ; i ++ )
	{
		if ( m_pPlayerBD->GetCutInfo().m_arrbShow[ i ] == true )
		{
			m_pPage->PutSprAuto( m_ptCutNum.x , m_ptCutNum.y + ( i * nYGab ) , &GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ] , 6 + m_pPlayerBD->GetCutInfo().m_arrCutCount[ i ], AB );
		}
	}
}

// 카드 그리기
void CPlayerDraw_BD::DrawCard( CDC* pDC )
{
	for ( int i = 0; i < m_pPlayerBD->m_nTotalCardNum; i++)
	{
		m_pPlayerBD->m_cMyCard[ i ].Draw( m_pPage, m_pPlayerBD->bFold, m_pPlayerBD->bFoldOpen );
	}	
}