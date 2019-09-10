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

	
	//ī�� ���� ��ġ
	
	// ���� (ī�� �������� ���������� �����ǥ)
	CPoint ptCard = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_CARD_POS_0 + m_nPNum ) );	

	// �������� 
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

	//���� ��� ǥ�� ���� ��
	m_ptCutNum = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_CUTNUM_0 + m_nPNum ) );	


	// ��� ����Ʈ ���� ����, �Ӵ� ��ǥ
	m_rtstrJokbo = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetRectBD( ui::ERECT_WININFO ); // ��¿���
	m_rtstrMoney = m_rtstrJokbo;

	CPoint ptJokbo = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_RESULT_0 + m_nPNum ) ); // ������ǥ

	CPoint ptGab;
	ptGab.x = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetGabBD( ui::EGAB_RESULT_X ); // ������ ������ �����ǥ X
	ptGab.y = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetGabBD( ui::EGAB_RESULT_Y ); // ������ ������ �����ǥ Y

	CPoint ptMoney = ptJokbo + ptGab;	// �Ӵ� ��ǥ
	m_rtstrJokbo.OffsetRect( ptJokbo );
	m_rtstrMoney.OffsetRect( ptMoney );

	// ����Ű ����
	m_ptHiddenKey = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_HIDDEN_OPENKEY ) );	
}

// �� ���� ���
void CPlayerDraw_BD::DrawJokboName(CDC *pDC)
{
	//���� ī�带 ������ �ִ��� �˻�
 	if ( g_GAME_BD()->IsHiddenCardFlag() && !m_pManager->IsHiddenCardShow() ) 
 	{
 		return;
 	}

	if ( m_pJokboName != NULL && !g_bMyObserver && m_pPlayer->JoinState == 1 )
	{	
		if ( m_nPNum == 0 )
		{
			if ( g_PLAYER( 0 )->m_bShowValue ) // ī�� ������ �׸��� �ʱ� ���ؼ�
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
		
	Draw_CutBackAndNumber( pDC );		// Ŀ�� ����
}

void CPlayerDraw_BD::DrawMiddle(CDC *pDC)
{
	CPlayerDraw::DrawMiddle( pDC );
	Draw_HiddenOpenImage( pDC ); // ����ī�� ī�� ���� �׸���( ���� ��ư )

	m_pPlayerBD->GetEffectObjectList()->MiddleDrawAll( pDC );

	// ��Ĩ �׸���
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
	// �ٵ��� ��ǥ ����	
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

			// ### [ ������� ] ###		
			if((!m_pPlayer->bFold && m_pManager->m_bOpen) || m_bDrawMyValue)
			{
				m_pManager->m_bHiddenCardShow = true;					
			}
			
			if(strJokbo == "")
			{
				if(bWin) strJokbo = "��ǽ�";
				else strJokbo = "�����";
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
 		//�ٵ��� ����ǥ��
 		m_pPage->PutSprAuto( m_ptDealer.x, m_ptDealer.y, &GLOBAL_SPR[ spr::GSPR_GAME_HOSTDEALER ], EDEALER_TYPE );
 	}
}

void CPlayerDraw_BD::Draw_HiddenOpenImage( CDC *pDC )
{
	if ( !g_ObserverMan.IsMyObserver() && m_nPNum == 0 && m_pPlayerBD->PlayState != 0 && g_Poker.nState  )
	{
		//���� ī�带 ������ �ִ��� �˻�
		if ( !g_GAME_BD()->IsHiddenCardFlag() ) 
		{
			return;
		}

		CNMyCard* pNMyCard = &m_pPlayerBD->m_cMyCard[ 0 ];

		// ����ī�� �̵��߿��� �ٲٱ� �̹��� �׸��� �ʴ´�.
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
			// ���� ���� �̹���
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

	// ��ħ, ����, ���� ���
	//m_pPage->PutSprAuto( m_ptCutNum.x , m_ptCutNum.y , &GLOBAL_SPR[ spr::GSPR_GAME_CUT_BACK ] , m_nPNum );

	//���� ��� ǥ��
	int nYGab = static_cast< ui::CUIInfoData_BD* >( m_pUIInfoData )->GetGabBD( ui::EGAB_CUTNUM_Y );

	for ( int i = 0 ; i < 3 ; i ++ )
	{
		if ( m_pPlayerBD->GetCutInfo().m_arrbShow[ i ] == true )
		{
			m_pPage->PutSprAuto( m_ptCutNum.x , m_ptCutNum.y + ( i * nYGab ) , &GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ] , 6 + m_pPlayerBD->GetCutInfo().m_arrCutCount[ i ], AB );
		}
	}
}

// ī�� �׸���
void CPlayerDraw_BD::DrawCard( CDC* pDC )
{
	for ( int i = 0; i < m_pPlayerBD->m_nTotalCardNum; i++)
	{
		m_pPlayerBD->m_cMyCard[ i ].Draw( m_pPage, m_pPlayerBD->bFold, m_pPlayerBD->bFoldOpen );
	}	
}