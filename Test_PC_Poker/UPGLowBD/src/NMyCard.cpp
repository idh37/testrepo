// NMyCard.cpp: implementation of the CNMyCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NMyCard.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

CNMyCard::CNMyCard()
:m_pMovement( NULL ),
m_pSpr( NULL ),
m_pSprDark( NULL ),
m_bSelected( false )
{
	SAFE_NEW( m_pMovement, CNMovement );
	Clear();	
}

CNMyCard::~CNMyCard()
{
	SAFE_DELETE( m_pMovement );
}

void CNMyCard::Init(int cardnum, int pnum, int index )
{
	Clear();	
	m_nCardNo = cardnum;
	m_nPNum   = pnum;
	m_size.x  = GLOBAL_SPR[ spr::GSPR_GAME_BIGCARD ].spr[ 0 ].xl;
	m_size.y  = GLOBAL_SPR[ spr::GSPR_GAME_BIGCARD ].spr[ 0 ].yl;
	m_nIndex  = index;	

	nMovelen=0;
}

void CNMyCard::Clear()
{	
	// ### [ 관전기능 ] ###
	m_pMovement->ReSet();
	
	m_nCardNo = -1;
	m_nPNum  = -1;
	m_bShow = false;
	m_bFront = false;		
	m_Xp = m_Yp = 0;
	m_nIndex = -1;		
	m_bDark = false;
	m_bSelected = false;
	m_bStay  = false;
	nDelay = 0;
	bDirect = FALSE;
	bMoveStart = FALSE;
	bFoldDark = FALSE;

	m_bHiddenCard = false;
	m_bHiddenMovingComplete = false;
	m_nHinddenArrow = 0;
}

void CNMyCard::Reset()
{
	m_bShow = false;
	m_bFront = false;		
	m_Xp = m_Yp = 0;		
	m_nIndex = -1;	

	m_bDark = false;	
	m_bSelected = false;
	m_bStay  = false;
	bFoldDark = FALSE;
}

void CNMyCard::SetPos(int tx, int ty)
{
	m_Xp = tx;
	m_Yp = ty;
	POINT pt;
	pt.x = tx;
	pt.y = ty;
	m_pMovement->SetCurPos(pt);
}

bool CNMyCard::PtInCard( int x, int y )
{
	bool bOPen =  false;
	
	CRect rtCardSize;
	
	if ( m_bSelected ) // 선택된 카드는 위로 CARD_GAB_Y_SELECT 위로 그려진다.
	{
		rtCardSize.SetRect( m_Xp, m_Yp - CARD_GAB_Y_SELECT, m_Xp + m_size.x, m_Yp + m_size.y );
	}
	else
	{
		rtCardSize.SetRect( m_Xp, m_Yp, m_Xp + m_size.x, m_Yp + m_size.y );
	}
		
	POINT pt;
	pt.x = x;
	pt.y = y;

	if ( rtCardSize.PtInRect( pt ) ) 
	{
		bOPen = true;
	}
	
	return bOPen;
}


void CNMyCard::OnTimer()
{
	m_pMovement->OnTimer(); // 속도	
	CardMove();
}

void CNMyCard::SetCardFace(BOOL bopen , int delay , BOOL bdirect)
{
	bOPen = bopen;
	nDelay = delay;	
	bDirect = bdirect;		
}

#define MOVELEN 20
void CNMyCard::CardMove()
{
	if ( m_nCardNo == -1 )
	{
		return;
	}
	
	if ( nDelay > 0 )
	{
		nDelay--;
		if ( nDelay == 0  )
		{ 
			bMoveStart = TRUE;
			PBPlayEffectSound(SND_CARD_MOVE);
		}
	}

	if(bMoveStart == FALSE)return;		
	
	POINT pt = m_pMovement->GetCurPos();

	pt.x += MOVELEN * (bDirect == TRUE?-1:1);
	nMovelen += MOVELEN;	

	if ( nMovelen >= MOVELEN * m_nIndex )
	{
		nMovelen = 0;
		bMoveStart = FALSE;
		nDelay = 0;

		if (bDirect == FALSE)
		{		
			pt.x = m_Xp;
			if (bOPen == FALSE)
			{
				m_bFront = true;
			}
		}
		else
		{
			pt.x = m_Xp + (-MOVELEN * m_nIndex);
		}
		m_pMovement->SetCurPos(pt);
	}
	else m_pMovement->SetCurPos(pt);
}


void CNMyCard::StraightMoveTo(  POINT start ,POINT dest, float spd, int delay  ,BOOL bChange)
{
	m_pMovement->StraightMoveTo( start ,dest, spd, delay, bChange );	
}


void CNMyCard::Draw(NMBASE::GRAPHICGDI::CPage *pPage, BOOL bFold, BOOL bFoldOpen )
{
	if ( pPage == NULL ) return;
	if ( !m_bShow ) return;
	if ( m_nPNum < 0 ) return;	

	if ( m_bStay )
	{
		return;	
	}

	if ( m_nCardNo < 0 )
	{
		return;
	}
	//오픈된 카드
	//int nSprNo = ( m_bFront ) ? m_nCardNo : DUMY_CARD;	
	int nSprNo;
	if ( m_bFront )
		nSprNo = m_nCardNo;
	else
		nSprNo = DUMY_CARD;

	POINT ptPos = m_pMovement->GetCurPos();
	
	NMBASE::GRAPHICGDI::xSprite *pSpr = NULL;
	
	// 결과 제외 카드 효과( 어두운 카드를 쓰는 효과나 기권, 기권off 상태가 아니라면 쓴다.
	if ( ( bFold && !bFoldOpen ) || m_bDark ) 			
	{
		pSpr = &GLOBAL_SPR[ spr::GSPR_GAME_DARKCARD ];
	}
	else
	{
		pSpr = &GLOBAL_SPR[ spr::GSPR_GAME_BIGCARD ];
	}

	if ( pSpr != NULL )
	{		
		pPage->PutSprAuto( ptPos.x, ptPos.y, pSpr, nSprNo);
			
		if ( m_bHiddenCard && m_bHiddenMovingComplete )
		{
			// 컷팅후 그려야 한다.
			pPage->PutSprAuto( ptPos.x, ptPos.y + m_nHinddenArrow, pSpr, DUMY_CARD );
		}
	
		if ( m_nPNum == 0 && !g_ObserverMan.IsMyObserver() )
		{
			if ( !g_GAME_BD()->m_bWindCardEnd && g_PLAYER(0)->PlayState == 1 && 
				 !g_PLAYER(0)->bFold && !g_GAME_BD()->GetMovingCardMgr()->IsCardMoving(0))
			{
				// 단축키
				if ( g_Config.bCutStyle )
				{
					if ( !g_pGameView->m_cPlayerDrawManager.IsGameOver() )
					{
						int nSprIndex = 11;
						int nYGab = GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ].spr[ nSprIndex ].yl;
						pPage->PutSprAuto( ptPos.x, ptPos.y + m_size.y - nYGab, &GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ], nSprIndex + m_nIndex ); // F1 ~ F4
					
					}
				}
				else
				{
					/*
					if ( g_Poker.nState != RSTATE_STARTGAME ) // 베팅시 
					{
						if ( g_GAME_BD()->GetCuttingStageInfo()->bChangeStage == true ) // 커팅 타임
						{
							int nSprIndex = 15;
							int nYGab = GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ].spr[ nSprIndex ].yl;
							pPage->PutSprAuto( ptPos.x, ptPos.y + m_size.y - nYGab, &GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ], nSprIndex + m_nIndex ); // 1 ~ 4
						}
					}
					*/

					// 원래 위쪽이랑 아래쪽 조건이 달랐는데 이 조건이 맞는거 같다.
					// 일단은 원래 있던 조건 부분은 주석처리.
					// 2012. 04. 25 최종학
					if ( !g_pGameView->m_cPlayerDrawManager.IsGameOver() )
					{
						int nSprIndex = 15;
						int nYGab = GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ].spr[ nSprIndex ].yl;
						pPage->PutSprAuto( ptPos.x, ptPos.y + m_size.y - nYGab, &GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ], nSprIndex + m_nIndex ); // 1 ~ 4
					}
				}				
			}
		}			
	}
}