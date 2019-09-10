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
				//������ : �ǰ� �̽� ������ ����(Allin)�� ����� ������ ä�� ������ ������ ����Ʈ�� Ȱ��ȭ �ȴ�. - ������ �۾��� QA���� BTS�� ��ϵ�... 
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

	//���� ���â Ȯ�� ��ư �ʱ�ȭ
	CPoint ptPos(434, 427);
	m_btnResultOK.Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &g_SprResultOK, 0, IDM_HOOLA_RESULTOK );
	m_btnResultOK.Show( FALSE );

	//��Ʈ �ʱ�ȭ
	if(m_cFont.m_hObject == NULL)
	{
		m_cFont.CreateFont(12,0,0,0,	FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����ü");
	}
	//��� ���� �ʱ�ȭ
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
	//����â�� Ȱ��ȭ �Ǹ鼭 ���� ���� ���� ȣ��ȴ�
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
	//�ڽŸ� �濡 �����Ұ�� �������� ��ư Ȱ��ȭ �ñ�� �ߺ��ȴ�
	//�̰�� ����Ʈ ���� �ñ⸦ �մ���
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
		//���â�� ���� ������� Pnum����
		nMyPnum = g_PLAYER(0)->ServPNum;
		
		//���� �Ǻ�
		pGOI = &m_GameResult.Ui[nMyPnum];
		if ( pGOI->cRanking == 1 )
		{
			bAmIWin = TRUE;
		}
	}
	
	// �̰��� ���
	if ( bAmIWin )
	{
		//Ư�� ��ġ�� ����� ���� ȣ���ϴµ� �̰� �׳� ���⼭ �ҷ��� ó���ϸ� ���ݾ� -_-;�����̰�
		if (  pGOI->bSeven4Win ) // 7 ��ī�� �¸�
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 42, timeGetTime() );
		}
		else if ( IsWinCaseHoola( m_GameResult.Hg.sWinnerCase ) ) // �Ƕ��
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 4, timeGetTime() );
		}
		else if ( m_GameResult.Hg.sWinnerCase == 4 ) // �뻧��
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 2, timeGetTime() );
		}
		else if ( m_GameResult.Hg.sWinnerCase == 5 ) // �һ���
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 3, timeGetTime() );
		}
		else // ������ ��
		{
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 0, timeGetTime() );
		}
	}
	else
	{
		if ( !g_ObserverMan.IsMyObserver() && ( g_PLAYER(0)->JoinState == EGAME_JOININ && g_PLAYER(0)->PlayState == EPLAYER_GAMEEND ) )
		{
			//�ڽ��� �������� �ƴϰ�, ���ӹ濡 ����������, ������� ���
			m_pPage->DrawMultiScene( 211, 112, &g_sprResult, 1, timeGetTime() );
		}
		else // ���� 
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
				m_pPage->DrawMultiScene( 217, 248, &g_sprResult, 5, timeGetTime() ); // ��ġ ǥ
			}
			else
			{
				m_pPage->DrawMultiScene( 217, 288 + (i-1)*32, &g_sprResult, i + 5, timeGetTime() ); // ��ġ ǥ
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
					
					// ����
					m_pPage->DrawMultiScene( 405, 260, &g_sprResult, 28 + (nCardSum / 10), timeGetTime() );
					m_pPage->DrawMultiScene( 419, 260, &g_sprResult, 28 + (nCardSum % 10), timeGetTime() );
					
					str = (CString)NMBASE::UTIL::g_MoneyMark(pGOI->nResultMoney, 2);
					
					rect.SetRect( 640, 263, 763, 275 );
					pDC->DrawText( str, &rect, DT_RIGHT );
					pDC->SelectObject( pFont);					
				}
				else
				{
					// ���̵� 
					str.Format( "%s", pGOI->NickName );
					rect.SetRect( 288, 301 + (i-1)*32 , 390, 312 + (i-1)*32 );
					pDC->DrawText( str, &rect, DT_LEFT );

					int nCardSum = pGOI->CardSum % 1000;

					// ����
					m_pPage->DrawMultiScene( 405, 297 + (i-1)*32, &g_sprResult, 28 + (nCardSum / 10), timeGetTime() );
					m_pPage->DrawMultiScene( 419, 297 + (i-1)*32, &g_sprResult, 28 + (nCardSum % 10), timeGetTime() );

					// ������
					// �ѵ� ���� ����.
					if (pGOI->nResultMoney <= -(INT64)GM().GetLossMaxMoneyForGame())
					{
						m_pPage->DrawMultiScene( 630, 295 + (i-1)*32, &g_sprResult, 44, timeGetTime() );
						
						// �ݾ�
						/*str = "-9��9000��";*/
						/*str = "-11��7000��";*/
						/*str = "-15��6000��";*/
						str.Format("-%s", NMBASE::UTIL::g_MoneyMark(GM().GetLossMaxMoneyForGame(), 2));
						rect.SetRect( 642, 300+ (i-1)*32, 763, 312 + (i-1)*32 );
						pDC->DrawText( str, &rect, DT_RIGHT );
					}
					else
					{
						// �ݾ�
						str = (CString)NMBASE::UTIL::g_MoneyMark(pGOI->nResultMoney, 2);
						rect.SetRect( 642, 300+ (i-1)*32, 763, 312 + (i-1)*32 );
						pDC->DrawText( str, &rect, DT_RIGHT );
					}
					//yoo 2015.01 ������
					if (0ll >= pCui->llMaxLossMoney)
					{
						m_pPage->DrawMultiScene(776, 255+i*32, &g_sprLimitIcon, 2, timeGetTime());
					}
				}
				break;
			}
		}
		
		// 1���� ���
		if ( pGOI->cRanking == 1 )
		{
			int nX, nY;
			nX = 439;
			nY = 254;
			
			int nGoldX = -100;
			int nMissionX = -100;
			int nHoolaX = -100;
			int n7X = -100;
			
			// 1�� �Ƕ�
			if ( IsWinCaseHoola( m_GameResult.Hg.sWinnerCase ) )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 25, timeGetTime() );
				nHoolaX = nX;
				nX += 47;
			}
			// 1�� �뻧
			if ( pGOI->bDaebbangWin )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 23, timeGetTime() );
				n7X = nX;
				nX += 47;
			}
			// 1�� �һ�
			if ( pGOI->bSobbangWin )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 24, timeGetTime() );
				n7X = nX;
				nX += 47;
			}
			
			// 7��ī��
			if ( pGOI->bSeven4Win )
			{
				m_pPage->PutSprAuto( nX, nY, &g_sprResult, 54 );
				n7X = nX;
				nX += 47;
			}
			
			// ���
			if ( pGOI->cGold == 2 )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 27, timeGetTime() );
				
				nGoldX = nX;

				nX += 47;
			}
			// ���
			else if ( pGOI->cGold == 4 )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 27, timeGetTime() );

				nGoldX = nX;

				nX += 47;
			}

			// �̼�
			if ( (pGOI->nMission == 2) || (pGOI->nMission == 3) )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 26, timeGetTime() );
				nMissionX = nX;
			}
			
			// �Ƕ�? x4
			if ( IsWinCaseHoola( m_GameResult.Hg.sWinnerCase ) )
			{
				m_pPage->DrawMultiScene( nHoolaX, nY, &g_sprResult, 17, timeGetTime() );
			}

			// �뻧 1�� x7
			if ( pGOI->bDaebbangWin )
			{
				m_pPage->DrawMultiScene( n7X, nY, &g_sprResult, 41, timeGetTime() );
			}
			// �һ� 1�� x7
			if ( pGOI->bSobbangWin )
			{
				m_pPage->DrawMultiScene( n7X, nY, &g_sprResult, 41, timeGetTime() );
			}
			if ( pGOI->bSeven4Win )
			{
				m_pPage->DrawMultiScene( n7X, nY, &g_sprResult, 41, timeGetTime() );
			}

			// ��� x2
			if ( pGOI->cGold == 2 )
			{
				m_pPage->DrawMultiScene( nGoldX, nY, &g_sprResult, 15, timeGetTime() );
			}
			// ��� x4
			else if ( pGOI->cGold == 4 )
			{
				m_pPage->DrawMultiScene( nGoldX, nY, &g_sprResult, 17, timeGetTime() );
			}

			// �̼� x2
			if ( pGOI->nMission == 2 )
			{
				m_pPage->DrawMultiScene( nMissionX, nY, &g_sprResult, 15, timeGetTime() );
			}
			// �̼� x3
			else if ( pGOI->nMission == 3 )
			{
				m_pPage->DrawMultiScene( nMissionX, nY, &g_sprResult, 16, timeGetTime() );
			}
			
		}
		// 1���� �ƴ� ���
		else
		{
			int nX, nY;
			nX = 439;
			nY = 293 + (i-1)*32;

			int nStopX = -100;
			int nRegX = -100;
			int nSevenX = -100;
			
			// �����
			if ( pGOI->bStopBak )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 13, timeGetTime() );
				nStopX = nX;
				nX += 47;
			}

			// ��ť��
			if ( pGOI->bThankYouBak )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 11, timeGetTime() );

				nX += 47;
			}

			// ��Ϲ�
			if ( pGOI->bRegisterBak )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 10, timeGetTime() );
				nRegX = nX;
				nX += 47;
			}

			// �����
			if ( pGOI->cSevenBakCnt )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 12, timeGetTime() );
				
				nSevenX = nX;
				
				nX += 47;
			}

			// ��Ŀ��
			if ( pGOI->cJokerBak )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 14, timeGetTime() );
			}
			
			// �����
			if (m_GameResult.Hg.bGiveUpGame)
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 45, timeGetTime() );
				g_pGameView->m_cGameViewBtnMananager.m_btnGiveUp.Enable(false);
			}
			
			// ����� x2
			if ( pGOI->bStopBak )
			{
				m_pPage->DrawMultiScene( nStopX, nY, &g_sprResult, 19, timeGetTime() );
			}
			// ��Ϲ� x2
			if ( pGOI->bRegisterBak )
			{
				m_pPage->DrawMultiScene( nRegX, nY, &g_sprResult, 19, timeGetTime() );
			}

			// ����� x2
			if ( pGOI->cSevenBakCnt == 1 )
			{
				m_pPage->DrawMultiScene( nSevenX, nY, &g_sprResult, 19, timeGetTime() );
			}
			// ����� x4
			else if ( pGOI->cSevenBakCnt == 2 )
			{
				m_pPage->DrawMultiScene( nSevenX, nY, &g_sprResult, 21, timeGetTime() );
			}
			// ����� x8
			else if ( pGOI->cSevenBakCnt == 3 )
			{
				m_pPage->DrawMultiScene( nSevenX, nY, &g_sprResult, 22, timeGetTime() );
			}
			// ����� x16
			else if ( pGOI->cSevenBakCnt == 4 )
			{
				m_pPage->DrawMultiScene( nSevenX, nY, &g_sprResult, 43, timeGetTime() );
			}

			// ��Ŀ�� x3
			if ( pGOI->cJokerBak == 1 )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprResult, 20, timeGetTime() );
			}
			// ��Ŀ�� x9
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

	//��� Ȯ�� ��ư�� �׸���
	m_btnResultOK.Draw(pDC);
}