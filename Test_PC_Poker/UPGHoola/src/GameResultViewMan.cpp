#include "StdAfx.h"
#include "GameResultViewMan.h"
#include "GlobalGame.h"

CGameResultViewMan::CGameResultViewMan(void)
{
	m_dwDelayEndTime = 0;
	m_dwDelayStartTime = 0;
	m_bJackpotDelay = FALSE;
	m_bTimer = FALSE;
	m_bShow = FALSE;
}

CGameResultViewMan::~CGameResultViewMan(void)
{
}

void CGameResultViewMan::PlayGameOverEffect()
{
	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		GAMEOVERINFO* pGOI = &m_GameResult.Ui[i];

		if ( strlen(pGOI->ID) > 0 )
		{
			if (pGOI->PMoney <= 0 )
			{
				//규제안 : 실계 이슈 같은데 올인(Allin)인 사람이 있으면 채팅 영역에 무조건 이펙트가 활성화 된다. - 규제안 작업중 QA에서 BTS로 등록됨... 
				if(GM().GetMyInfo()->UI.GetRealPMoney() == 0) g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_ALLIN, pGOI->ID );
			}
			else if ( pGOI->cJokerBak &&  pGOI->bRegisterBak &&  pGOI->cSevenBakCnt )
			{
				g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_JOKER_SEVEN_REIGIST_BAK, pGOI->ID );
			}
			else if ( pGOI->cJokerBak &&  pGOI->bRegisterBak )
			{
				g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_REIGIST_JOKER_BAK, pGOI->ID );
			}
			else if ( pGOI->bRegisterBak &&  pGOI->cSevenBakCnt )
			{
				g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_SEVEN_REIGIST_BAK, pGOI->ID );
			}
			else if ( pGOI->cJokerBak && pGOI->cSevenBakCnt )
			{
				g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_JOKER_SEVEN_BAK, pGOI->ID );
			}
			else if ( pGOI->cJokerBak )
			{
				g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_JOKER_BAK, pGOI->ID );
			}
			else if ( pGOI->cSevenBakCnt )
			{
				g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_SEVEN_BAK, pGOI->ID );
			}
			else if ( pGOI->bRegisterBak )
			{
				g_pGameView->m_cEffectDrawMan.PlayAnimation( eNUM_EFFECT_REGIST_BAK, pGOI->ID );
			}
		}
	}
}

void CGameResultViewMan::OnTimer()
{
	if ( TRUE == m_bTimer )
	{
		if ( m_dwDelayEndTime <= ( timeGetTime() - m_dwDelayStartTime ) )
		{
			Show(TRUE);
			m_dwDelayStartTime = 0;
			m_dwDelayEndTime = 0;
			m_bTimer = FALSE;
		}
	}
}
void CGameResultViewMan::StopAllResultView()
{
	Show(FALSE);
	g_pGameView->m_cEffectDrawMan.StopAllAnimation();
	m_bTimer = FALSE;
	m_dwDelayStartTime = 0;
	m_dwDelayEndTime = 0;
}

void CGameResultViewMan::Draw( CDC *pDC )
{
	DrawResultView( pDC );
}

BOOL CGameResultViewMan::OnLButtonDown(int x , int y)
{	
	return m_btnResultOK.OnLButtonDown(x,y);
}
BOOL CGameResultViewMan::OnLButtonUp(int x , int y)
{
	if ( TRUE == m_btnResultOK.OnLButtonUp(x,y) )
	{
		Show( FALSE );
	}
	return m_btnResultOK.OnLButtonUp(x,y);
}
BOOL CGameResultViewMan::OnMouseMove(int x , int y)
{
	return m_btnResultOK.OnMouseMove(x,y);
}
void CGameResultViewMan::Show( BOOL bFlag )
{
	if ( TRUE == bFlag )
	{
		m_btnResultOK.Show( TRUE );
		m_btnResultOK.Enable( TRUE );
		m_bShow = TRUE;
		
		PlayWinLoseSound();
		PlayGameOverEffect();
	}
	else
	{
		if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
		{
			MM().Call(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, WPARAM(0), LPARAM(0));
		}
		else
		{		
			MM().Call(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, (WPARAM)0, (LPARAM)TRUE);
		}
		m_btnResultOK.Show( FALSE );
		m_btnResultOK.Enable( FALSE );
		m_bShow = FALSE;
	}
}

void CGameResultViewMan::Init(NMBASE::GRAPHICGDI::CPage *ppage, CWnd *pParent)
{
	m_pPage = ppage;
	m_pParent = pParent;

	//게임 결과창 확인 버튼 초기화
	CPoint ptPos(434, 427);
	m_btnResultOK.Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &g_SprResultOK, 0, IDM_HOOLA_RESULTOK );
	m_btnResultOK.Show( FALSE );

	//폰트 초기화
	if(m_cFont.m_hObject == NULL)
	{
		m_cFont.CreateFont(12,0,0,0,	FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	}
	//멤버 변수 초기화
	Reset();
}
void CGameResultViewMan::Reset()
{
	m_dwDelayEndTime = 0;
	m_dwDelayStartTime = 0;
	m_bJackpotDelay = FALSE;

	Show(FALSE);
	StopAllResultView();
}
void CGameResultViewMan::PlayWinLoseSound()
{
	//게임창이 활성화 되면서 관련 사운드 역시 호출된다
	if ( (m_GameResult.Ui[g_GAMEPROCESS()->GetServPNum_ByPN(0)].cRanking == 1) && FALSE == g_ObserverMan.IsMyObserver())
	{	//Win Sound 
		NMBASE::SOUND::g_Sound.PlayWav( SND06 );
	}
	else
	{	//Lose Sound
		NMBASE::SOUND::g_Sound.PlayWav( SND07 );
	}
}
void CGameResultViewMan::StopResultTimer()
{
	m_bTimer = FALSE;
	Reset();
}
void CGameResultViewMan::OnGameOver(GAMEOVERRESULT* pGOR)
{ 
	SetGameOverResult(*pGOR);

	m_dwDelayEndTime = GetCalculatedResultDelay(pGOR);
	
	if ( m_dwDelayEndTime > 0 )
	{
		m_dwDelayStartTime = timeGetTime();
		m_bTimer = TRUE;
	}
	else
	{
		Show(TRUE);
	}
}
DWORD CGameResultViewMan::GetCalculatedResultDelay(GAMEOVERRESULT* pGOR)
{
	//자신만 방에 존재할경우 연습게임 버튼 활성화 시기와 중복된다
	//이경우 이펙트 노출 시기를 앞당긴다
	int nPlayerCnt = 0;
	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		if ( strlen( g_PLAYER(i)->UI.ID ) > 0 )
		{
			nPlayerCnt++;
		}
	}
	if ( nPlayerCnt <= 1 )
	{
		return 0;
	}	
	BOOL bStopbakDelay = FALSE;
	BOOL bSevenHighLowDelay = FALSE;
	BOOL bJackpotDelay = FALSE;
	BOOL bHoolaDelay = FALSE;

	const DWORD dwStopBak = 2270;
	const DWORD dwSevenHighLowDelay = 1880;
	const DWORD dwHoola = 5000;
	const DWORD dwJackpot = 4000;//9860;


	if ( pGOR->Hg.sWinnerCase == 1 || pGOR->Hg.sWinnerCase == 2 )
	{
		bHoolaDelay = TRUE;
	}
	bJackpotDelay = m_bJackpotDelay;
	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		if (strlen(pGOR->Ui[i].ID) > 0)
		{
			if ( pGOR->Ui[i].bStopBak)
			{
				bStopbakDelay = TRUE;
			}
			if ( pGOR->Ui[i].bSeven4Win )
			{
				bSevenHighLowDelay = TRUE;
			}
			if ( pGOR->Ui[i].bDaebbangWin )
			{
				bSevenHighLowDelay = TRUE;
			}
			if ( pGOR->Ui[i].bSobbangWin)
			{
				bSevenHighLowDelay = TRUE;
			}
		}		
	}

	if ( TRUE == bJackpotDelay )
	{
		if ( TRUE == bSevenHighLowDelay )
		{
			return dwJackpot + dwSevenHighLowDelay;
		}
		else
		{
			return dwJackpot;
		}
	}
	else if ( TRUE == bSevenHighLowDelay )
	{
		return dwSevenHighLowDelay;
	}
	else if ( TRUE == bHoolaDelay )
	{
		return dwHoola;
	}
	else if ( TRUE == bStopbakDelay )
	{
		return dwStopBak;
	}

	
	return 0;
}

void CGameResultViewMan::DrawResultView( CDC *pDC )
{
	if ( FALSE == m_bShow )
	{
		return;
	}
	
	bool first = false;
	char Count = 0;
	BOOL bAmIWin = FALSE;
	int nMyPnum = -1;
	BOOL bGold = FALSE;
	
	GAMEOVERINFO *pGOI = NULL;
	CHANGE_USERINFO_ROOM *pCui=NULL;
	char Order[MAX_PLAYER];
	memset( Order, -1, sizeof(char) * MAX_PLAYER );
	
	for ( int nPnum = 0 ; nPnum < MAX_PLAYER ; nPnum++ )
	{
		pGOI = &m_GameResult.Ui[nPnum];
		if( strlen( pGOI->ID ) <= 0 ) continue;
		
		int nRank = pGOI->cRanking - 1;
		
		while( Order[ nRank ] >= 0 ) nRank++;
		Order[ nRank ] = nPnum;
		Count++;
	}
	
	if ( !g_ObserverMan.IsMyObserver() )
	{
		//결과창을 보는 사용자의 Pnum저장
		nMyPnum = g_PLAYER(0)->ServPNum;
		
		//승자 판별
		pGOI = &m_GameResult.Ui[nMyPnum];
		if ( pGOI->cRanking == 1 )
		{
			bAmIWin = TRUE;
		}
	}
	
	// 이겼을 경우
	if ( bAmIWin )
	{
		//특정 위치에 저장된 값을 호출하는데 이거 그냥 여기서 불러서 처리하면 되잖아 -_-;뭐냐이거
		if (  pGOI->bSeven4Win ) // 7 포카드 승리
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 42, timeGetTime() );
		}
		else if ( IsWinCaseHoola( m_GameResult.Hg.sWinnerCase ) ) // 훌라승
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 4, timeGetTime() );
		}
		else if ( m_GameResult.Hg.sWinnerCase == 4 ) // 대빵승
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 2, timeGetTime() );
		}
		else if ( m_GameResult.Hg.sWinnerCase == 5 ) // 소빵승
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 3, timeGetTime() );
		}
		else // 나머지 승
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 0, timeGetTime() );
		}
	}
	else
	{
		if ( !g_ObserverMan.IsMyObserver() && ( g_PLAYER(0)->JoinState == EGAME_JOININ && g_PLAYER(0)->PlayState == EPLAYER_GAMEEND ) )
		{
			//자신이 옵저버가 아니고, 게임방에 참여했으며, 대기중일 경우
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 1, timeGetTime() );
		}
		else // 관전 
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 40, timeGetTime() );
		}
	}
	
	COLORREF crOldColor = pDC->GetTextColor();
	if ( bAmIWin )
	{
		pDC->SetTextColor(RGB( 108, 1, 46 ));
	}
	else
	{
		pDC->SetTextColor(RGB( 4 , 33, 87 ));
	}
	
	INT64 LostMoney = 0;
	CRect rect;
	
	for ( int i = 0 ; i < Count ; i++ )
	{
		CString str;
		
		pGOI =  &m_GameResult.Ui[Order[i]];
		pCui =  &m_GameResult.Cui[Order[i]];
		
		if( strlen( pGOI->ID ) <= 0 )
		{
			continue;
		}
		
		if ( nMyPnum == Order[i] )
		{
			if ( i == 0 )
			{
				m_pPage->DrawMultiScene( 217, 248, &g_sprResult, 5, timeGetTime() ); // 위치 표
			}
			else
			{
				m_pPage->DrawMultiScene( 217, 288 + (i-1)*32, &g_sprResult, i + 5, timeGetTime() ); // 위치 표
			}
		}
		
		for ( int nick = 0; nick < MAX_PLAYER; nick++ )
		{
			if ( strlen( pGOI->NickName ) > 0 )
			{
				if ( i == 0 )
				{
					CFont *pFont = pDC->SelectObject( &m_cFont );
					str.Format( "%s", pGOI->NickName );
					rect.SetRect( 288, 263, 390, 275 );
					pDC->DrawText( str, &rect, DT_LEFT );
					
					int nCardSum = pGOI->CardSum % 1000;
					
					// 점수
					m_pPage->DrawMultiScene( 405, 260, &g_sprResult, 28 + (nCardSum / 10), timeGetTime() );
					m_pPage->DrawMultiScene( 419, 260, &g_sprResult, 28 + (nCardSum % 10), timeGetTime() );
					
					str = (CString)NMBASE::UTIL::g_MoneyMark(pGOI->nResultMoney, 2);
					
					rect.SetRect( 640, 263, 763, 275 );
					pDC->DrawText( str, &rect, DT_RIGHT );
					pDC->SelectObject( pFont);					
				}
				else
				{
					// 아이디 
					str.Format( "%s", pGOI->NickName );
					rect.SetRect( 288, 301 + (i-1)*32 , 390, 312 + (i-1)*32 );
					pDC->DrawText( str, &rect, DT_LEFT );

					int nCardSum = pGOI->CardSum % 1000;

					// 점수
					m_pPage->DrawMultiScene( 405, 297 + (i-1)*32, &g_sprResult, 28 + (nCardSum / 10), timeGetTime() );
					m_pPage->DrawMultiScene( 419, 297 + (i-1)*32, &g_sprResult, 28 + (nCardSum % 10), timeGetTime() );

					// 규제안
					// 한도 적용 노출.
					if (pGOI->nResultMoney <= -(INT64)GM().GetLossMaxMoneyForGame())
					{
						m_pPage->DrawMultiScene( 630, 295 + (i-1)*32, &g_sprResult, 44, timeGetTime() );
						
						// 금액
						/*str = "-9조9000억";*/
						/*str = "-11조7000억";*/
						/*str = "-15조6000억";*/
						str.Format("-%s", NMBASE::UTIL::g_MoneyMark(GM().GetLossMaxMoneyForGame(), 2));
						rect.SetRect( 642, 300+ (i-1)*32, 763, 312 + (i-1)*32 );
						pDC->DrawText( str, &rect, DT_RIGHT );
					}
					else
					{
						// 금액
						str = (CString)NMBASE::UTIL::g_MoneyMark(pGOI->nResultMoney, 2);
						rect.SetRect( 642, 300+ (i-1)*32, 763, 312 + (i-1)*32 );
						pDC->DrawText( str, &rect, DT_RIGHT );
					}
					//yoo 2015.01 규제안
					if (0ll >= pCui->llMaxLossMoney)
					{
						m_pPage->DrawMultiScene(776, 255+i*32, &g_sprLimitIcon, 2, timeGetTime());
					}
				}
				break;
			}
		}
		
		// 1등일 경우
		if ( pGOI->cRanking == 1 )
		{
			int nX, nY;
			nX = 439;
			nY = 254;
			
			int nGoldX = -100;
			int nMissionX = -100;
			int nHoolaX = -100;
			int n7X = -100;
			
			// 1등 훌라
			if ( IsWinCaseHoola( m_GameResult.Hg.sWinnerCase ) )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 25, timeGetTime() );
				nHoolaX = nX;
				nX += 47;
			}
			// 1등 대빵
			if ( pGOI->bDaebbangWin )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 23, timeGetTime() );
				n7X = nX;
				nX += 47;
			}
			// 1등 소빵
			if ( pGOI->bSobbangWin )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 24, timeGetTime() );
				n7X = nX;
				nX += 47;
			}
			
			// 7포카드
			if ( pGOI->bSeven4Win )
			{
				m_pPage->PutSprAuto( nX, nY, &g_sprResult, 54 );
				n7X = nX;
				nX += 47;
			}
			
			// 골드
			if ( pGOI->cGold == 2 )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 27, timeGetTime() );
				
				nGoldX = nX;

				nX += 47;
			}
			// 골드
			else if ( pGOI->cGold == 4 )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 27, timeGetTime() );

				nGoldX = nX;

				nX += 47;
			}

			// 미션
			if ( (pGOI->nMission == 2) || (pGOI->nMission == 3) )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 26, timeGetTime() );
				nMissionX = nX;
			}
			
			// 훌라? x4
			if ( IsWinCaseHoola( m_GameResult.Hg.sWinnerCase ) )
			{
				m_pPage->DrawMultiScene( nHoolaX, nY, &g_sprResult, 17, timeGetTime() );
			}

			// 대빵 1등 x7
			if ( pGOI->bDaebbangWin )
			{
				m_pPage->DrawMultiScene( n7X, nY, &g_sprResult, 41, timeGetTime() );
			}
			// 소빵 1등 x7
			if ( pGOI->bSobbangWin )
			{
				m_pPage->DrawMultiScene( n7X, nY, &g_sprResult, 41, timeGetTime() );
			}
			if ( pGOI->bSeven4Win )
			{
				m_pPage->DrawMultiScene( n7X, nY, &g_sprResult, 41, timeGetTime() );
			}

			// 골드 x2
			if ( pGOI->cGold == 2 )
			{
				m_pPage->DrawMultiScene( nGoldX, nY, &g_sprResult, 15, timeGetTime() );
			}
			// 골드 x4
			else if ( pGOI->cGold == 4 )
			{
				m_pPage->DrawMultiScene( nGoldX, nY, &g_sprResult, 17, timeGetTime() );
			}

			// 미션 x2
			if ( pGOI->nMission == 2 )
			{
				m_pPage->DrawMultiScene( nMissionX, nY, &g_sprResult, 15, timeGetTime() );
			}
			// 미션 x3
			else if ( pGOI->nMission == 3 )
			{
				m_pPage->DrawMultiScene( nMissionX, nY, &g_sprResult, 16, timeGetTime() );
			}
			
		}
		// 1등이 아닐 경우
		else
		{
			int nX, nY;
			nX = 439;
			nY = 293 + (i-1)*32;

			int nStopX = -100;
			int nRegX = -100;
			int nSevenX = -100;
			
			// 스톱박
			if ( pGOI->bStopBak )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 13, timeGetTime() );
				nStopX = nX;
				nX += 47;
			}

			// 땡큐박
			if ( pGOI->bThankYouBak )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 11, timeGetTime() );

				nX += 47;
			}

			// 등록박
			if ( pGOI->bRegisterBak )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 10, timeGetTime() );
				nRegX = nX;
				nX += 47;
			}

			// 세븐박
			if ( pGOI->cSevenBakCnt )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 12, timeGetTime() );
				
				nSevenX = nX;
				
				nX += 47;
			}

			// 조커박
			if ( pGOI->cJokerBak )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 14, timeGetTime() );
			}
			
			// 기권패
			if (m_GameResult.Hg.bGiveUpGame)
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 45, timeGetTime() );
				g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Enable(false);
			}
			
			// 스톱박 x2
			if ( pGOI->bStopBak )
			{
				m_pPage->DrawMultiScene( nStopX, nY, &g_sprResult, 19, timeGetTime() );
			}
			// 등록박 x2
			if ( pGOI->bRegisterBak )
			{
				m_pPage->DrawMultiScene( nRegX, nY, &g_sprResult, 19, timeGetTime() );
			}

			// 세븐박 x2
			if ( pGOI->cSevenBakCnt == 1 )
			{
				m_pPage->DrawMultiScene( nSevenX, nY, &g_sprResult, 19, timeGetTime() );
			}
			// 세븐박 x4
			else if ( pGOI->cSevenBakCnt == 2 )
			{
				m_pPage->DrawMultiScene( nSevenX, nY, &g_sprResult, 21, timeGetTime() );
			}
			// 세븐박 x8
			else if ( pGOI->cSevenBakCnt == 3 )
			{
				m_pPage->DrawMultiScene( nSevenX, nY, &g_sprResult, 22, timeGetTime() );
			}
			// 세븐박 x16
			else if ( pGOI->cSevenBakCnt == 4 )
			{
				m_pPage->DrawMultiScene( nSevenX, nY, &g_sprResult, 43, timeGetTime() );
			}

			// 조커박 x3
			if ( pGOI->cJokerBak == 1 )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 20, timeGetTime() );
			}
			// 조커박 x9
			else if ( pGOI->cJokerBak == 2 )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 38, timeGetTime() );
			}
		}

		if ( nMyPnum == Order[i] )
		{
			LostMoney = pGOI->nResultMoney;
		}
	}
	pDC->SetTextColor( crOldColor );

	//결과 확인 버튼을 그린다
	m_btnResultOK.Draw(pDC);
}