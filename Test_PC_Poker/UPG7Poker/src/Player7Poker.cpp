// Player7Poker.cpp: implementation of the PLAYER7POKER class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player7Poker.h"
#include "GlobalGame.h"
#include "SelectCard7Poker.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayer7Poker::CPlayer7Poker()
{	
	ServPNum = 0;

	PNum = -1;

	Clear();
}

CPlayer7Poker::~CPlayer7Poker()
{
}

void CPlayer7Poker::Clear()
{
	ZeroMemory(&UI, sizeof(UI));
	ZeroMemory(&m_cRoomChangeInfo, sizeof(CHANGE_USERINFO_ROOM));
	Reset();
	bFold = FALSE;

	MyIpCheck = -1;

	nSndFxKind = 0;
	
	nRoundPrevMoney = 0;
	m_bLastHalf = FALSE;		

	//2006.3.20
	m_nCardChoiceLimtCnt = 0;
}

void CPlayer7Poker::SetPlayerNum(int pnum)
{
	PNum = pnum;
}

void CPlayer7Poker::SetNewPlayer(USERINFO *pUI)
{	
	Clear();
	CPlayer::SetNewPlayer(pUI);

	InitRoomData();
	Avatar.SetAvatar(UI.AvatarInfo);
}

void CPlayer7Poker::Reset()
{
	JoinState = EGAME_JOINOUT;
	PlayState = EPLAYER_GAMEEND;

	m_nCardTotalCount = 0;
	m_nOpenCardTotalCount = 0;
	bFold = FALSE;
	bFoldOpen = FALSE;

	m_n64ResultMoney = 0;
	nRoundPrevMoney  = 0;

	// 카드 관련
	for(int i = 0; i < CARD_TOTALCOUNT; ++i) 
	{
		m_cMyCard[i].Clear();
	}
	m_bResultCardOpenMove = FALSE;

	m_nSDelay = -1;	
	ZeroMemory(m_cCardRectEventEx, sizeof(m_cCardRectEventEx));
	m_nTotalCardCount = 0;

	m_bLastHalf = FALSE;

	//이미지 드로우 관련 리스트 
	m_cPlayerEffectList.Destroy();	

	m_nThrowCardNum = -1;

	m_bHiddenScene = FALSE;

}


//방에 입장할때 한번 초기화 하는것들 
void CPlayer7Poker::InitRoomData()
{
	Reset();

	ENUM_POSTYPE nPosType = EPT_UNKNOWN;
	switch(PNum)
	{
	case 0:	// 자신
		{			
			nPosType = EPT_ME;
		} break;
	case 1:	//좌측
	case 2:
		{
			nPosType = EPT_LEFT;		
		} break;
	case 3:	// 우측
	case 4:
		{
			nPosType = EPT_RIGHT;
		} break;
	default:
		return;
	}

	CPoint pt;
	pt.x = 12;
	pt.y = 0;

	m_ptCardStart   = g_cUIData.m_listCardStartPos[PNum];
	m_ptAvatarStart = g_cUIData.m_listAvatarPos[PNum];

	// 나의 칩 던지는 모듈 초기화
	m_cMyChip.Init(&g_pGameView->Page, &g_sprChip, PNum, &g_cUIData);
}

void CPlayer7Poker::OnTimer()
{	
	// 나의 칩처리
	m_cMyChip.OnMyChipTimer(timeGetTime());

	// 카드 처리
	for(int i = 0; i < m_nTotalCardCount; ++i)
	{
		m_cMyCard[i].OnTimer();
	}

	// 결과때 카드 이동 처리 부분 
	if(m_bResultCardOpenMove == TRUE)
	{
		if(m_nSDelay > 0)
		{
			m_nSDelay--;
			if(m_nSDelay <= 0 )
			{
				m_nSDelay = 0;				
				g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
			}
		}

		int cnt = m_nTotalCardCount;

		for(int i = 0; i < m_nTotalCardCount; ++i)
		{
			if(m_cMyCard[i].GetMove().IsMoveStart() == FALSE /*&& m_cMyCard[i].IsCardOpen() == TRUE &&*/ /*m_cMyCard[i].GetDelay() == 0*/)
			{
				cnt--;
			}
		}

		if(cnt <= 0)
		{			
			m_bResultCardOpenMove = FALSE;
		}		
	}
	else if(m_bResultCardOpenMove == FALSE && m_nSDelay == 0)
	{
		// 결과시 족보를 순서대로 보여준다
		int nCount = 0 ;
		for(int i = 0 ; i < m_nTotalCardCount ; i++)
		{
			if(m_cMyCard[i].GetMove().IsMoveStart() == FALSE && /*m_cMyCard[i].GetDelay() == 0 && */m_cMyCard[i].IsCardFront() == TRUE)
			{
				++nCount;
			}
		}

		if(nCount == m_nTotalCardCount -1)
		{						
			g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
		}
	}

	m_cPlayerEffectList.ProcessAll();	
}

void CPlayer7Poker::OnSetChip(INT64 money)
{
	if(PlayState != EPLAYER_GAMEPLAYING && JoinState != EGAME_JOININ && bFold)
	{
		return;		
	}

	m_cMyChip.SetMoneyChip(money);
	m_cMyChip.SetMyMoveChip();	
}

// 카드는 유저정보 그리는것보다 아래 있다
void CPlayer7Poker::OnDraw_Bottom(CDC *pDC)
{
	// 버린카드 그리기
	OnDrawThrowCard(pDC);	

	m_cPlayerEffectList.BottomDrawAll(pDC);	
}

void CPlayer7Poker::OnDraw_Middle(CDC *pDC)
{
	m_cPlayerEffectList.MiddleDrawAll(pDC);
}

//각 m_cPlayer 최 상위에 들어갈 이미지들 
void CPlayer7Poker::OnDraw_Top(CDC *pDC)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	DrawDummyHiddenCard(pDC);

	if(0 == PNum)
	{
		g_SelectCard().OnDraw(pDC);
	}

	m_cPlayerEffectList.TopDrawAll(pDC);	

	// 내칩 그리기
	if(m_cMyChip.IsMyChipShow())
	{
		m_cMyChip.OnMyChipDraw();
	}	
}


/////////////////카드 관련///////////////////////////////

// 레이어 때문에 카드는 별로로 따로 그린다.
void CPlayer7Poker::OnCardDraw(NMBASE::GRAPHICGDI::CPage *pPage) 
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return;
	}

	if(pPage ==NULL)
	{
		return;
	}

	for(int i = 0; i < m_nTotalCardCount; ++i)
	{
		m_cMyCard[i].Draw( pPage, bFold, bFoldOpen);
	}	
 
	/*if(m_nTotalCardCount == 5)
	{
		for(int i = 0; i < m_nTotalCardCount; ++i) 
		{
			char szTemp1[256] = {0, };
			sprintf(szTemp1, "------ 보유카드 => Player:%d 카드번호:%d 앞뒤:%d \n", PNum, m_cMyCard[i].GetCardNum(), m_cMyCard[i].IsCardFront());
			OutputDebugString(szTemp1);
		}			
	}*/
}

BOOL CPlayer7Poker::IsCardFront(int index)
{
	return m_cMyCard[index].IsCardFront();
}

// 카드의 앞면을 보이게 한다.
void CPlayer7Poker::SetCardFrontShow(int index)
{
	if(index < 0 || index >= m_nTotalCardCount)
		return;
	m_cMyCard[index].SetCardFront(TRUE);		
}

// 카드의 뒷면을 보이게 한다. 
void CPlayer7Poker::SetCardBackShow(int index)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;	
	m_cMyCard[index].SetCardFront(FALSE);
}

void CPlayer7Poker::ShowCard(int index , BOOL bshow)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;	
	m_cMyCard[index].SetCardShow(bshow);
}

void CPlayer7Poker::SetResultFace(int index, BOOL bresult)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;

	for(int i = 0 ; i < m_nTotalCardCount;i++)
	{
		m_cMyCard[i].SetResultDarkDraw(bresult);
	}
}

// 0 ~ 6 총 7장
int CPlayer7Poker::GetCardNum(int index)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return 0;
	return m_cMyCard[index].GetCardNum();
}

// 카드를 다 덮은 상태로 모은다.
void CPlayer7Poker::SetGameResultCardOpen(int delay)	// 카드 오픈 (애니 처리 설정)
{
	m_bResultCardOpenMove = TRUE;
	m_nSDelay = delay;

	int ndelay = delay;	
}

void CPlayer7Poker::SetChioceCardMove(int face)
{
	for(int i = m_nTotalCardCount -1 ; i >= 0; --i)
	{			
		if(face != 0)
		{
			if(face == 1)
			{
				m_cMyCard[i].SetCardFront(FALSE);
			}
			else
			{
				m_cMyCard[i].SetCardFront(TRUE);
			}			
		}
	}	
}

// 카드 영역을 클릭 했는지 체크
BOOL CPlayer7Poker::IsGameOverPtInCardRect(int xp,int yp) 
{
	// 다이 일때 내카드 열기
	if(!g_ObserverMan.IsMyObserver() && JoinState != EGAME_JOINOUT && PlayState == EPLAYER_GAMEEND && bFold == TRUE && PNum == 0 )
	{	
		CPoint pt;
		pt.x = xp;
		pt.y = yp;

		CRect temp[7];

		for( int i = 0 ; i < m_nTotalCardCount ; ++i )
		{
			temp[i] = m_cCardRectEventEx[i];
		}

		temp[4].SetRect(m_cMyCard[4].GetPos().x, m_cMyCard[4].GetPos().y, m_cMyCard[4].GetPos().x + CARD_WIDTH, m_cMyCard[4].GetPos().y + CARD_HEIGHT);

		for( int i = 0 ; i < m_nTotalCardCount ; ++i )
		{
			if( temp[i].PtInRect (pt) == TRUE )
			{
				for(int x = 0 ; x < m_nTotalCardCount ; ++x )
				{
					if(IsCardFront(x) == FALSE)
					{
						SetCardFrontShow(x);
					}
				}
				return TRUE;
			}
		}

		for( int i = 0 ; i < m_nTotalCardCount ; ++i )
		{
			if(IsCardFront(i) == TRUE && bFoldOpen == FALSE)
			{
				SetCardBackShow(i);
			}
		}
	}

	return FALSE;
}

// 카드 영역에 있는지
int CPlayer7Poker::EventOverPtInCardRect(CPoint pt)
{
	for(int i = 0 ; i < m_nTotalCardCount ; ++i)
	{
		if(m_cCardRectEventEx[i].PtInRect (pt) == TRUE && i < m_nTotalCardCount)
		{
			return i;
		}
	}
	return -1;
}


void CPlayer7Poker::OnDrawThrowCard(CDC *pDC)
{
	if( UI.UNum <= 0 || strlen(UI.ID)  <= 0 || pDC == NULL)
		return;

	// 버린 카드 그리기
	if( g_RI.FormKind == CHOICE_GAME )
	{
		if( m_nThrowCardNum > -1 )
		{
			CPoint ptPos = g_cUIData.m_listCardStartPos[0] + g_cUIData.m_ptDisCardGab;
			g_pGameView->Page.PutSprAuto(ptPos.x, ptPos.y, &g_sprDisCard, m_nThrowCardNum);		
		}
	}	
}


// 게임키 처리를 한다.
void CPlayer7Poker::GameKeyProcess(int nVKey, int nExtend)
{
	//규제안 관련 수정 : 본인턴이 안닌 상태에서 키를 누르면 반응하도록 수정 (예약기능)
	if( g_RI.State == EGAME_PLAYING &&  /*g_Poker.CurPlayer == ServPNum &&*/  JoinState == EGAME_JOININ && PlayState == EPLAYER_GAMEPLAYING && /* PNum == 0 && */ !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard())
	{
		for( int i = KEY_BET_DIE; i <= KEY_BET_HAF; ++i )
		{
			for( int j = 0; j < MAX_GAME_KEY; ++j )
			{				
				if( IS_VKEY( G_KEYDEF[i][j], nVKey, nExtend ) )
				{
					switch( i )
					{
					case KEY_BET_DIE:
						{
							if( g_Poker.nAutoBetNodie != 1 ) 
							{
// 								CString szTemp;
// 								szTemp.Format("GameKey(): gPPNum:%d, PNum:%d, 유저ID:%s \n", g_Poker.CurPlayer, ServPNum, g_PLAYER(0)->UI.ID);
// 								AddGameChatViewEdit(szTemp.GetBuffer(), GetChatColor(ECC2_RESULT_JOKBO));
								g_GAMEPROCESS()->SendDieButton();
							}							
						}
						break;
					case KEY_BET_CHK:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{
								g_GAMEPROCESS()->SendCheckButton();
							}
						}
						break;
					case KEY_BET_CAL:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{
								g_GAMEPROCESS()->SendCallButton();
							}
						}
						break;
					case KEY_BET_PNG:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{
								g_GAMEPROCESS()->SendPingButton();
							}
						}
						break;
					case KEY_BET_DAD:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{
								g_GAMEPROCESS()->SendDdaButton();
							}							
						}
						break;
					case KEY_BET_HAF:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{	
								g_GAMEPROCESS()->SendHalfButton();								
							}							
						}
						break;

					default:
						break;
					}					
				}
			}
		}
	}	
}


void CPlayer7Poker::OnLButtonDown(int xp, int yp)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;
	if( PNum != 0 || g_ObserverMan.IsMyObserver() || g_GAMEPROCESS() == NULL) return;

	POINT pt;
	pt.x = xp;
	pt.y = yp;

	if( JoinState != EGAME_JOINOUT && PlayState != EPLAYER_GAMEEND && m_nCardTotalCount == CARD_TOTALCOUNT )
	{
		if( g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() == TRUE && g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() == TRUE )
		{
			return;
		}

		if( m_cMyCard[6].IsPtInCard(pt)  )
		{
			g_GAMEPROCESS()->SetMouseHiddenState( 1 );
			g_GAMEPROCESS()->SetMouseHiddenClick(TRUE);
			g_GAMEPROCESS()->SetHiddenClickPos(pt);
			SetCardFrontShow(6); // 보이기
			return ;
		}
		g_GAMEPROCESS()->SetMouseHiddenClick(FALSE);
	}
}

void CPlayer7Poker::OnLButtonUp(int xp, int yp)
{
	if( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) return;

	if( PNum != 0 || g_ObserverMan.IsMyObserver() || g_GAMEPROCESS() == NULL) return;	

	if(JoinState != EGAME_JOINOUT && PlayState != EPLAYER_GAMEEND && m_nCardTotalCount == CARD_TOTALCOUNT )
	{
		if( g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() == TRUE || g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() == TRUE ) return;

		if( g_GAMEPROCESS()->IsMouseHiddenClick() == TRUE)
		{
			HiddenOpne();
		}
	}
}

void CPlayer7Poker::OnMouseMove(int xp, int yp)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	if( PNum == 0 && !g_ObserverMan.IsMyObserver() && bFold == TRUE )
	{
		IsGameOverPtInCardRect(xp, yp);
	}
	
	if( PNum != 0 || g_ObserverMan.IsMyObserver() || g_GAMEPROCESS() == NULL) return;

	if( JoinState != EGAME_JOINOUT && PlayState != EPLAYER_GAMEEND && m_nCardTotalCount == CARD_TOTALCOUNT )
	{		
		if(g_pGameView->m_cPlayerDrawManager.IsFlyWindCard()) return;

		if( g_GAMEPROCESS()->IsMouseHiddenClick() && g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() == FALSE )
		{			 		
			GetHiddenCardMoveWay( xp, yp );

			/*
			if( g_GAMEPROCESS()->GetHiddenClickPos().y < yp ) 
			{
				int nGapX = xp - g_GAMEPROCESS()->GetHiddenClickPos().x;
				int nGapY = yp - g_GAMEPROCESS()->GetHiddenClickPos().y;

				g_GAMEPROCESS()->SetHiddenArrowPosX(g_GAMEPROCESS()->GetHiddenArrowPosX() + nGapX);
				g_GAMEPROCESS()->SetHiddenArrowPosY(g_GAMEPROCESS()->GetHiddenArrowPosY() + nGapY);

				POINT pt = g_GAMEPROCESS()->GetHiddenClickPos();
				pt.x = xp;
				pt.y = yp;
				
				g_GAMEPROCESS()->SetHiddenClickPos(pt);
			}
			else if( g_GAMEPROCESS()->GetHiddenClickPos().y > yp ) 
			{
 				int nGapX = g_GAMEPROCESS()->GetHiddenClickPos().x - xp;
 				int nGapY = g_GAMEPROCESS()->GetHiddenClickPos().y - yp;

// 				int nGapX = abs( xp - g_GAMEPROCESS()->GetHiddenClickPos().x );
// 				int nGapY = abs( yp - g_GAMEPROCESS()->GetHiddenClickPos().y );

// 				int X = g_GAMEPROCESS()->GetHiddenClickPos().x;
// 				int Y = g_GAMEPROCESS()->GetHiddenClickPos().y;

 				g_GAMEPROCESS()->SetHiddenArrowPosX(g_GAMEPROCESS()->GetHiddenArrowPosX() - nGapX);
 				g_GAMEPROCESS()->SetHiddenArrowPosY(g_GAMEPROCESS()->GetHiddenArrowPosY() - nGapY);			

// 				g_GAMEPROCESS()->SetHiddenArrowPosX(g_GAMEPROCESS()->GetHiddenArrowPosX() + nGapX);
// 				g_GAMEPROCESS()->SetHiddenArrowPosY(g_GAMEPROCESS()->GetHiddenArrowPosY() + nGapY);

 				if(g_GAMEPROCESS()->GetHiddenArrowPosY() < -100)
 				{
 					g_GAMEPROCESS()->SetHiddenArrowPosY(0);
 					g_GAMEPROCESS()->SetMouseHiddenClick(FALSE);
 				}

				POINT pt = g_GAMEPROCESS()->GetHiddenClickPos();
				pt.x = xp;
				pt.y = yp;

				g_GAMEPROCESS()->SetHiddenClickPos(pt);
			}
			*/
		}		
	}	
}

int CPlayer7Poker::GetHiddenCardMoveWay( int xp, int yp )
{

	int moveway = 0;
 	int nMoveGapX = xp - g_GAMEPROCESS()->GetHiddenClickPos().x;
 	int nMoveGapY = yp - g_GAMEPROCESS()->GetHiddenClickPos().y;

	//일정픽셀 이상 이동했으면 히든 오픈한다.
	if( abs( g_GAMEPROCESS()->GetHiddenArrowPosX() ) > 50 || abs( g_GAMEPROCESS()->GetHiddenArrowPosY() ) > 80 )	
	{
		HiddenOpne();
		return 0;
	}

	int nxp = abs( nMoveGapX );
 	int nyp = abs( nMoveGapY );

	//	4	|	1
	//------------------------
	//	3	|	2

	if( ( nMoveGapX > 0 && nMoveGapY > 0 ) || ( nMoveGapX > 0 && nMoveGapY == 0 ) ) 
	{
		if( xp > yp )
		{
			moveway = 1;
		}
		else if( xp < yp )
		{
			moveway = 2;
		}
		else
		{
			moveway = 1;
		}
	}
	else if( ( nMoveGapX > 0 && nMoveGapY < 0 ) || ( nMoveGapX == 0 && nMoveGapY < 0 ) )
	{
		if( nxp > nyp )
		{
			moveway = 1;
		}
		else if( nxp < nyp )
		{
			moveway = 4;
		}
		else
		{
			moveway = 4;
		}
	}
	else if( ( nMoveGapX < 0 && nMoveGapY > 0 )  || ( nMoveGapX == 0 && nMoveGapY > 0 )  )
	{
		if( nxp > nyp )
		{
			moveway = 3;
		}
		else if( nxp < nyp )
		{
			moveway = 2;
		}
		else
		{
			moveway = 3;
		}
	}
	else if( ( nMoveGapX < 0 && nMoveGapY < 0 )  || ( nMoveGapX == 0 && nMoveGapY < 0 ) )
	{
		m_bHiddenScene = TRUE;

		//대각선 히든 오픈 체크
		if( nxp > nyp )
		{
	
		}
		else if( nxp < nyp )
		{

		}
		else
		{

		}
	}

	g_GAMEPROCESS()->SetHiddenArrowPosX( g_GAMEPROCESS()->GetHiddenArrowPosX() + nMoveGapX );
	g_GAMEPROCESS()->SetHiddenArrowPosY( g_GAMEPROCESS()->GetHiddenArrowPosY() + nMoveGapY );

	POINT pt;
	pt.x = xp;
	pt.y = yp;

	g_GAMEPROCESS()->SetHiddenClickPos( pt );

	return moveway;
}

void CPlayer7Poker::HiddenOpne()
{
	PBPlayEffectSound(SND_HIDDEN_OPEN);

	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
	SetCardFrontShow(6); // 보이기

	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

	int nOldPoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0);
	SetCardResult();

	DrawEft_CompleteMadeValue( nOldPoint );
	g_pGameView->m_cGameHelper.CalculationProbability();	

	g_GAMEPROCESS()->EndCardOpenGuide();		//오픈애니메이션추가
}

//이팩트 효과 지우기
void CPlayer7Poker::ClearEffectSpr(ENUM_ANIFILEID kind, BOOL ballclear)
{
	if( ballclear == TRUE )
	{
		m_cPlayerEffectList.Destroy();
	}
	else
	{
		m_cPlayerEffectList.DelObject(kind);
	}
}

void CPlayer7Poker::SetCardResult()
{	
	int nCardNum[10] = {0, };
	int nTotalCardCount = g_Poker.PS[ServPNum].nCardTotal;

	if(nTotalCardCount < 3)
	{
		return;
	}

	for(int i = 0; i < 2; ++i)
	{
		nCardNum[i] = GetCardNum(i);
	}


	int nMinNum = min(nTotalCardCount, CCardRule::TOTAL_CARD);
	for(int i = 2; i < nMinNum; ++i)
	{
		if(i == 6) 
		{
			nCardNum[i] = GetCardNum(6);
		}
		else  
		{
			nCardNum[i] = g_Poker.PS[ServPNum].nCard[i-2];
		}
	}
	//g_GAMEPROCESS()->GetSevenRuleManager().ClearAll();
	g_GAMEPROCESS()->GetSevenRuleManager().set_total_Card(PNum, nCardNum, nTotalCardCount );
	g_GAMEPROCESS()->GetSevenRuleManager().get_cal_rule_user(PNum);
	
	// 자신의 카드위에 족보명을 보위기 위함
	g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
}


//카드 배열 정렬 기본형,일자형
void CPlayer7Poker::set_CardArray()
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME || g_ObserverMan.IsMyObserver() || g_RI.State == EGAME_WAITING || PNum != 0 ) return;

	for(int i = 2 ; i < m_nTotalCardCount; ++i)
	{
		if( g_Config.bCardArray == FALSE )
		{
			// 기본형으로 바꿈
			POINT pt = m_cMyCard[i].GetMove().GetCurPos();
			if( pt.y == m_ptCardStart.y )
			{
				pt.y -= POKER7_CARD_GAB_Y;
				m_cMyCard[i].GetMove().SetCurPos( pt );
			}
		}
		else
		{
			//일자형으로 바꿈
			POINT pt = m_cMyCard[i].GetMove().GetCurPos();
			if( pt.y == ( m_ptCardStart.y - POKER7_CARD_GAB_Y ) )
			{
				pt.y += POKER7_CARD_GAB_Y;
				m_cMyCard[i].GetMove().SetCurPos( pt );
			}
		}
	}
}


BOOL CPlayer7Poker::Is7PokerGame()
{
	if( IDX_GAME_SP == g_RI.nGameIndex )
	{
		return TRUE;
	}

	return FALSE;
}


// 히든카드를 보여준다.
void CPlayer7Poker::DrawDummyHiddenCard(CDC *pDC)
{
	if( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) return;
	
	if( g_ObserverMan.IsMyObserver() || PNum != 0 || PlayState == EPLAYER_GAMEEND || m_nCardTotalCount != CARD_TOTALCOUNT ) return;
	
	if( g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() ) return;
	if( !m_cMyCard[6].IsShowHiddenCard() ) return;

// 	if( !g_ObserverMan.IsMyObserver() && PNum == 0 && PlayState != EPLAYER_GAMEEND && m_nCardTotalCount == CARD_TOTALCOUNT  )
// 	{
		NMBASE::GRAPHICGDI::CPage *pPage = NULL;
		pPage = &g_pGameView->Page;
		if( pPage == NULL ) return;

		CPoint ptPos;

		// 히든이미지 추가
		switch(g_RI.FormKind)
		{
		case 0:
		case 1:
			{
				ptPos.x = g_GAMEPROCESS()->GetPlayerNP(0)->m_cMyCard[6].GetPos().x;
				ptPos.y = g_GAMEPROCESS()->GetPlayerNP(0)->m_cMyCard[6].GetPos().y;					
			} break;
		case 2:
		case 3:
			{
				ptPos.x = g_GAMEPROCESS()->GetPlayerNP(0)->m_cMyCard[7].GetPos().x;
				ptPos.y = g_GAMEPROCESS()->GetPlayerNP(0)->m_cMyCard[7].GetPos().y;					
			} break;
		}

		if( !g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
		{
			int hX = m_cMyCard[6].GetPos().x + g_GAMEPROCESS()->GetHiddenArrowPosX();			
			int hY = m_cMyCard[6].GetPos().y + g_GAMEPROCESS()->GetHiddenArrowPosY();			
		
			if( m_bHiddenScene )
			{
				int mxp = abs( g_GAMEPROCESS()->GetHiddenArrowPosX() );			
				int myp = abs( g_GAMEPROCESS()->GetHiddenArrowPosY() );	

				DWORD dwTemp = 0;
				if( mxp > myp )
				{
					dwTemp = mxp * 28;
				}
				else
				{
					dwTemp = myp * 28;
				}
	
				//히든 오픈 애니메이션이 끝났다.
				if( dwTemp >= 1300 ) 
				{
					g_GAMEPROCESS()->SetMouseHiddenState( 2 );
					g_GAMEPROCESS()->SetHiddenSceneDrawTime();
				}
				else
				{
					pPage->DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, 0, dwTemp, FALSE );
				}
			}
			else
			{
				// 히든 카드이미지
				pPage->PutSprAuto( hX, hY, &g_sprBigCard, g_nDUMY_CARD);		
				//pPage->PutSprAuto(m_cMyCard[6].GetPos().x, hY, &g_sprBigCard, g_nDUMY_CARD);		
			}

			if( g_GAMEPROCESS()->IsMouseHiddenClick() == FALSE && g_GAMEPROCESS()->IskeyboardHiddenClick() == FALSE )
			{
				// 히든카드 오픈 단축키 이미지를 보여준다.
				pPage->PutSprAuto( ptPos.x + 9, ptPos.y + 62, &g_sprGLOBAL[ spr::GSPR_V_H_OPEN ], 0 );

				// 화살표
				pDC->SetTextColor(RGB(255,225,255));
				pDC->TextOut(ptPos.x + 32, ptPos.y + 40, "↑↓");
			}

			//오픈애니메이션추가
			if( g_GAMEPROCESS()->GetCardOpneGuideIndex() != -1 && g_GAMEPROCESS()->GetCardOpneGuideDraw() )
			{
				pPage->DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, g_GAMEPROCESS()->GetCardOpneGuideIndex(), timeGetTime() - g_GAMEPROCESS()->Getm_dwCardOpenGuideTime(), FALSE );

				if( g_sprLastCardOpen.IsMSceneTimeOver(  g_GAMEPROCESS()->GetCardOpneGuideIndex(), timeGetTime() - g_GAMEPROCESS()->Getm_dwCardOpenGuideTime() ) )
				{
					g_GAMEPROCESS()->EndCardOpenGuide();
				}
			}
		}

		//히든카드 오픈 마무리 애니메이션  //오픈애니메이션추가
		if( g_GAMEPROCESS()->GetMouseHiddenState() == 2 )
		{
			pPage->DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, 1, timeGetTime() - g_GAMEPROCESS()->GetHiddenSceneDrawTime(), FALSE );

			if( g_sprLastCardOpen.IsMSceneTimeOver( 1, timeGetTime() - g_GAMEPROCESS()->GetHiddenSceneDrawTime() ) )
			{
				g_GAMEPROCESS()->SetMouseHiddenState( 0 );
				g_GAMEPROCESS()->SetHiddenSceneDrawTime();
			}
		}
//	}
}

// 화면에 카드를 추가 하고 세팅한다.
void CPlayer7Poker::SetAddCard( int nCardNum )
{
	//TRACE("\n============================================================\n");
	//TRACE("PNum : %d, TotalCardCount : %d, cardNum : %d\n", PNum, m_nTotalCardCount, nCardNum);
	//TRACE("============================================================\n\n");
	
	if( !Is7PokerGame() )
	{
		return;
	}

	if(m_nTotalCardCount < 0 || m_nTotalCardCount >= CARD_TOTALCOUNT)
	{
		return;// ###[8포커]###
	}

	// 카드 초기화
	m_cMyCard[m_nTotalCardCount].Init(nCardNum);
	
	// 카드의 시작 Y좌표
	int nCardPosY = m_ptCardStart.y;	

	if(!g_ObserverMan.IsMyObserver() && IsCardFront(2) && PNum == 0)
	{
		if( g_Config.bCardArray == FALSE )
		{
			nCardPosY = m_ptCardStart.y - POKER7_CARD_GAB_Y; 
		}
		else
		{
			nCardPosY = m_ptCardStart.y;
		}		
	}

	int nCardGab = g_cUIData.m_nCardGab;
	if(PNum == 0) nCardGab = g_cUIData.m_nMyCardGab;

	// 목적지 셋팅 
	m_cMyCard[m_nTotalCardCount].SetCardPos(m_ptCardStart.x + (m_nTotalCardCount * nCardGab), nCardPosY);

	// 나 자신
	if( 0 == PNum)
	{
		if( m_nTotalCardCount == 4 )
		{
			m_cCardRectEventEx[3].SetRect(m_cMyCard[3].GetPos().x, 
										  m_cMyCard[3].GetPos().y, 
										  m_cMyCard[3].GetPos().x + nCardGab, 
										  m_cMyCard[3].GetPos().y + CARD_HEIGHT);
		}
		else if( m_nTotalCardCount == 3 )
		{
			m_cCardRectEventEx[m_nTotalCardCount].SetRect(m_cMyCard[m_nTotalCardCount].GetPos().x, 
														  m_cMyCard[m_nTotalCardCount].GetPos().y, 
														  m_cMyCard[m_nTotalCardCount].GetPos().x + CARD_WIDTH, 
														  m_cMyCard[m_nTotalCardCount].GetPos().y + CARD_HEIGHT);
		}
		else if( m_nTotalCardCount <= 2 )
		{
			m_cCardRectEventEx[m_nTotalCardCount].SetRect(m_cMyCard[m_nTotalCardCount].GetPos().x, 
														  m_cMyCard[m_nTotalCardCount].GetPos().y, 
														  m_cMyCard[m_nTotalCardCount].GetPos().x + nCardGab, 
														  m_cMyCard[m_nTotalCardCount].GetPos().y + CARD_HEIGHT);
		}
	}	

	// 받은 카드 개수를 증가 시킨다.
	++m_nTotalCardCount;

	if(m_nTotalCardCount == 5 && nCardNum == 52)
	{
		int aa = 4;
	}
}


void CPlayer7Poker::Draw_DarkCard()
{
	//if(JoinState == EGAME_JOINOUT || PlayState == EPLAYER_GAMEEND || bFold == TRUE )
	if (JoinState == EGAME_JOINOUT)// || PlayState == EPLAYER_GAMEEND || bFold == TRUE )
	{
		return;
	}	

	for(int i = 0 ; i < m_nTotalCardCount ; ++i )
	{
		if( g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[PNum].m_RemainCard[i] > -1 )
		{
			int num = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[PNum].m_RemainCard[i];
			
			for(int j = 0; j < m_nTotalCardCount ; ++j )
			{
				if( m_cMyCard[j].GetCardNum() == num )
				{
					m_cMyCard[j].SetResultDarkDraw(TRUE);
					break;
				}
			}
		}
	}
}

INT64 CPlayer7Poker::GetNowMoney()
{
	if( g_RI.bIsMaxRoom )
	{
		return UI.GetInMoney();
	}
	else if( g_RI.ChipKind==1 )
	{
		return nPlayGold;
	}

	return UI.GetMoney();
}

void CPlayer7Poker::SetNowMoney( INT64 nMoney )
{
	if( g_RI.bIsMaxRoom )
	{
		UI.SetInMoney( nMoney );
	}
	else if( g_RI.ChipKind==1 )
	{
		nPlayGold = nMoney;
	}
	else
	{
		UI.SetMoney( nMoney );
	}
}

bool CPlayer7Poker::IsAllIn()
{
	if( g_RI.bIsMaxRoom ){
		return ((0 == UI.GetInMoney()) ? (true):(false));
	}
	else if( g_RI.ChipKind==1 )
	{
		return ((0 == nPlayGold) ? (true):(false));
	}
	return ((0 == UI.GetMoney()) ? (true):(false));
}


BOOL CPlayer7Poker::OnGameKey(int nVKey, int nExtend, int repeat)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return FALSE;
	}

	if( !Is7PokerGame() || g_GAMEPROCESS() == NULL) 
	{
		return FALSE;
	}
	
	if( PNum != 0 || g_ObserverMan.IsMyObserver() || JoinState == EGAME_JOINOUT || bFold == TRUE ) 
	{
		return FALSE;	
	}

	// 게임에서 사용하는 베팅키 처리
	GameKeyProcess(nVKey, nExtend);

	// 카드 쪼이기 처리
	if( g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() == FALSE && m_nCardTotalCount == g_GAMEPROCESS()->GetTotalCardNum() && !g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
	{
		bool bHave = false;

		for( int i = KEY_HIDDEN_OPEN; i <= KEY_HIDDEN_SLOW; ++i )
		{
			for( int j = 0; j < MAX_GAME_KEY; ++j )
			{
				if( IS_VKEY( G_KEYDEF[ i ][ j ], nVKey, nExtend ) )
				{
					switch ( i )
					{
					case KEY_HIDDEN_OPEN: 
						{
// 							if( j == 1 && g_Poker.CurPlayer == ServPNum) //< CYS 100705 > // 내턴에delete 키는베팅시제외
// 							{
// 								continue;
// 							}

							PBPlayEffectSound(SND_HIDDEN_OPEN);

							SetCardFrontShow(6); // 보이기
							g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

							int nOldPoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0);
							SetCardResult();
							DrawEft_CompleteMadeValue( nOldPoint );

							g_pGameView->m_cGameHelper.CalculationProbability();
							bHave = true;
						} break;

					case KEY_HIDDEN_SLOW:
						{
							if(g_GAMEPROCESS()->GetHiddenArrowPosY() == 0)
							{						
								SetCardFrontShow(6); // 보이기
							}

							g_GAMEPROCESS()->SetHiddenArrowPosY(g_GAMEPROCESS()->GetHiddenArrowPosY() + 4);

							g_GAMEPROCESS()->SetkeyboardHiddenClick( TRUE );

							if(g_GAMEPROCESS()->GetHiddenArrowPosY() > 20)
							{
								PBPlayEffectSound(SND_HIDDEN_OPEN);
								g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

								int nOldPoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0);
								SetCardResult();
								DrawEft_CompleteMadeValue( nOldPoint );

								g_pGameView->m_cGameHelper.CalculationProbability();
							}
							bHave = true;							
						} break;
					}
				}
			}

			if( bHave )
			{
				// 실수로 같은키 셋팅시 2번 호출방지
				break;
			}
		}
	}

	return TRUE;	
}




void CPlayer7Poker::DrawEft_CompleteMadeValue(int oldpoint, bool bForce )
{
	if ( g_ObserverMan.IsMyObserver() || PNum != 0  || bFold == TRUE || PlayState != 1 )
	{
		return;
	}

	int curpoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(PNum);
	//보다 좋은 족보(메이드) 완성시 효과 
	if ( curpoint >= CCardRule::N7C_TITLE)
	{	

		if ( curpoint > oldpoint || bForce ) // 처음 4장, 히든오픈시 메이드인 경우 이펙트 보여준다.
		{
			CMadeEffect *pObj = NULL;
			SAFE_NEW( pObj, CMadeEffect );
			pObj->Init(m_nTotalCardCount);
			g_pGameView->m_cAniSprListManager.ADDObject(pObj);
		}
	}
}
