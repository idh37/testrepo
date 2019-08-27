// MyCard.cpp: implementation of the CMyCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyCard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMyCard::CMyCard()
{
	Clear();	
}

CMyCard::~CMyCard()
{

}

void CMyCard::Init(int cardnum, int pnum , BOOL bbomb , int index)
{
	Clear();	
	m_nCardNo = cardnum;
	m_nPNum = pnum;
	m_nIndex = index;

	if(bbomb){
		m_bIsBomb = TRUE;
	}
	m_nIndex = index;
	m_cMove.SetSoundID(SND_CARD_MOVE);
}

void CMyCard::Clear()
{	
	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));
	ZeroMemory(&m_cEvent1, sizeof(CARDEVENT));

	// ### [ 관전기능 ] ###
	m_cMove.ReSet();
	
	m_nCardNo = 0;
	m_nPNum  = -1;

	m_bShow = FALSE;
	m_bFront = FALSE;
	m_bMove = FALSE;

	m_bEvent = FALSE;

	m_ptPos.x = m_ptPos.y = 0;

	m_bBombCard = FALSE;
	m_nJokerCard = -1 ;
	m_nIndex = 0;
	
	m_bIsBomb = FALSE;
	m_bBombShow = FALSE;
	
	m_bMoveStart = FALSE;
	m_nDelay = 0;
	m_nMovelen = 0;
	m_bDirect = 0;
	m_bOPen = FALSE;	
	m_bJokerCard = FALSE;
	m_nJokerCard = -1 ; 
	m_bDark = FALSE;
}

void CMyCard::Reset()
{
	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));
	ZeroMemory(&m_cEvent1, sizeof(CARDEVENT));

	m_bShow = FALSE;
	m_bFront = FALSE;
	m_bMove = FALSE;
	m_bEvent = FALSE;
	m_bEvent1 = FALSE;
	m_ptPos.x = m_ptPos.y = 0;
	m_bBombCard = FALSE;
	m_bJokerCard = FALSE;
	m_nJokerCard = -1 ;
	m_nIndex = 0;

	m_bMoveStart = FALSE;
	m_nDelay = 0;
	m_nMovelen = 0;
	m_bDirect = 0;
	m_bOPen = FALSE;
	m_bIsBomb = FALSE;	

	m_bJokerCard = FALSE;
	m_nJokerCard = -1 ;

	m_bDark = FALSE;
	m_bBombShow = FALSE;
}

// 1 번째 이벤트
void CMyCard::SetEvent(int eventcode, int option, int option1, int option2)
{
	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));

	m_bEvent = TRUE;
	m_cEvent.CardNo = m_nCardNo;
	m_cEvent.EventCode = eventcode;
	m_cEvent.Option = option;
	m_cEvent.Option2  = option1;
}

// 2번째 이벤트
void CMyCard::SetEvent1(int eventcode, int option, int option1, int option2)
{	
	ZeroMemory(&m_cEvent1, sizeof(CARDEVENT));

	m_bEvent1 = TRUE;
	m_cEvent1.CardNo = m_nCardNo;
	m_cEvent1.EventCode = eventcode;
	m_cEvent1.Option = option;
	m_cEvent1.Option2  = option1;
}

void CMyCard::ClearEvent()
{
	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));
	m_bEvent = FALSE;
}

void CMyCard::ClearEvent1()
{
	ZeroMemory(&m_cEvent1, sizeof(CARDEVENT));
	m_bEvent1 = FALSE;
}

void CMyCard::Draw(NMBASE::GRAPHICGDI::CPage *pPage, BOOL bFold, BOOL bFoldOpen)
{
	if (pPage == NULL) return;
	if (m_bShow == FALSE) return;
	if (m_nPNum < 0 ) return;
	if (m_bBombCard && !m_bBombShow) return;

	int sprno = DUMY_CARD;

	if (m_bFront || bFoldOpen)
		sprno = m_nCardNo;
	else if (m_bBombCard && (bFold == FALSE))
		sprno = m_nCardNo;
	
	CPoint pt = m_cMove.GetCurPos();
	
	int xp = pt.x;
	int yp = pt.y;
	
	//이동이 끝났다면 카드를 직접 그린다.
	if(m_cMove.IsMoveEnd())
	{
		// 기권이고 본인 패 오픈하지 않을 경우
		if(bFold && bFoldOpen == FALSE)
		//if( bFold )
		{
			// 회색 카드
			pPage->PutSprAuto(xp, yp, &g_sprDarkBigCard, sprno);

			// 뒷면이 아니고 조커 카드일 경우
			if(sprno != DUMY_CARD && g_bGetJokerCard == TRUE && m_bJokerCard == TRUE)
			{
				// 번호 있는 회색 조커 카드
				pPage->PutSprAuto(xp, yp, &g_sprDarkBigCard, 54);
			}
		}
		// 카드를 오픈하는 상황
		else
		{
			// 회색 카드가 아닐 경우
			//if(m_bDark == FALSE || bFoldOpen == TRUE)
			if(m_bDark == FALSE)
			{
				// 폭탄 카드고 카드 뒷면이 아닐 경우
				if(m_bBombCard && sprno <= 100 && sprno != DUMY_CARD)
				{
					// 조커 카드일 경우
					if(g_bGetJokerCard == TRUE && m_bJokerCard == TRUE)
					{
						if(sprno == 100)
						{
							// 번호 없는 폭탄 조커 카드
							pPage->PutSprAuto(xp, yp, &g_sprBigBombCard, 53);
						}
						else if(sprno > -1)
						{
							pPage->PutSprAuto(xp, yp, &g_sprBigBombCard, sprno); // 폭탄 카드
							pPage->PutSprAuto(xp, yp, &g_sprBigBombCard, 54); // 번호 있는 폭탄 조커 카드
							// 조커에 번호가 표시됨
						}
					}
					else
					{
						pPage->PutSprAuto(xp, yp, &g_sprBigBombCard, sprno);
					}
				}
				// 일반 카드일 경우
				else
				{
					pPage->PutSprAuto(xp, yp, &g_sprBigCard, sprno);
				}
			}
			// 회색 카드일 경우
			else
			{
				// 내가 폭탄 카드일때 다른걸로 그린다						
				// 어두운 걸로 바꾼다.				
				if(sprno != DUMY_CARD && sprno == 100)
				{
					// 번호 없는 회색 조커 카드
					pPage->PutSprAuto(xp, yp, &g_sprDarkBigCard, 53);
				}
				// 본인 카드 오픈 상태에서 카드 뒷면일 경우
				else if (sprno == DUMY_CARD && bFoldOpen == TRUE)
				{
					// 회색 카드 뒷면 대신 밝은 카드 뒷면을 그린다
					pPage->PutSprAuto(xp, yp, &g_sprBigCard, sprno);
				}
				else
				{
					// 회색 카드
					pPage->PutSprAuto(xp, yp, &g_sprDarkBigCard, sprno);
					
					// 조커일 경우
					if(g_bGetJokerCard == TRUE && m_bJokerCard == TRUE)
					{
						// 번호 있는 회색 조커 카드
						pPage->PutSprAuto(xp, yp, &g_sprDarkBigCard, 54);
					}
				}
			}
		}
	}
	// 카드 이동일 경우
	else 
	{
		// 일반 이동일 경우
		if(m_cMove.GetMoveType() != CMovementEx::MT_NORMAL)
		{
			m_cMove.DrawSprite(pPage);
		}
		// 폭탄 카드고 뒷면이 아닐 경우
		else if(m_bBombCard && sprno <= 100 && sprno != DUMY_CARD)
		{
			// 조커 카드일 경우
			if(g_bGetJokerCard == TRUE && m_bJokerCard == TRUE)
			{
				if(sprno == 100)
				{
					// 번호 없는 폭탄 조커 카드
					pPage->PutSprAuto(xp, yp, &g_sprBigBombCard, 53);
				}
				else if(sprno > -1)
				{
					pPage->PutSprAuto(xp, yp, &g_sprBigBombCard, sprno); // 폭탄 카드
					pPage->PutSprAuto(xp, yp, &g_sprBigBombCard, 54); // 번호 있는 폭탄 조커 카드
				}
			}
			else
			{
				// 폭탄 카드
				pPage->PutSprAuto(xp, yp, &g_sprBigBombCard, sprno);
			}
		}
		else
		{
			// 밝은색 카드 앞면
			pPage->PutSprAuto(xp, yp, &g_sprBigCard, sprno);
		}
	}
}

BOOL CMyCard::PtInCard(int x, int y)
{
	BOOL m_bOPen =  FALSE;
	CRect rc;
	POINT cur;
	cur.x = m_ptPos.x;
	cur.y = m_ptPos.y;
	rc.SetRect( cur.x, cur.y, cur.x + g_szCard.cx, cur.y + g_szCard.cy );
	
	POINT pt;
	pt.x = x;
	pt.y = y;

	if(rc.PtInRect( pt )) 
	{
		m_bOPen = TRUE;	
	}
	
	return m_bOPen;

}

void CMyCard::OnTimer()
{
	m_cMove.OnTimer(); // 속도
	if( m_cMove.IsMoveEnd() )
	{
		// 이벤트가 있다면 이벤트를 처리
		if(m_bEvent)
		{
			if(m_cEvent.EventCode == EVENT_PREPARESTART_ONEEND)
			{
				GAME()->OnCardEvent(&m_cEvent);
				ClearEvent();
			}
			else if(m_cEvent.EventCode == EVENT_THREEFLYEND)
			{
				GAME()->OnCardEvent(&m_cEvent);
				ClearEvent();
			}
			else if(m_cEvent.EventCode == EVENT_ONEFLYEND)
			{			
				GAME()->OnCardEvent(&m_cEvent);
				ClearEvent();
			} 
			else if(m_cEvent.EventCode == EVENT_BOMBFLYEND)
			{
				GAME()->OnCardEvent(&m_cEvent);
				ClearEvent();
			}
		}
		
		if(m_bEvent1)
		{
			if(m_cEvent1.EventCode == EVENT_ONEFLYTOTEND)
			{
				GAME()->OnCardEvent(&m_cEvent1);
				ClearEvent1();
			}
			else if(m_cEvent1.EventCode == EVENT_BOMBFLY_TO_END)
			{
				GAME()->OnCardEvent(&m_cEvent1);
				ClearEvent1();
			}
		}
	}
	
	CardMove();	
}

void CMyCard::SetCardFace(BOOL bopen , int delay , BOOL bdirect)
{
	m_bOPen = bopen;
	m_nDelay = delay;	
	m_bDirect = bdirect;	
}

#define MOVELEN 16
void CMyCard::CardMove()
{
	if( m_nDelay > 0 ){
		m_nDelay--;
		if( m_nDelay == 0  ){ 
			m_bMoveStart = TRUE;
			PBPlayEffectSound(SND_CARD_MOVE);
		}
	}

	if(m_bMoveStart == FALSE)return;		
	
	POINT pt = m_cMove.GetCurPos();		
	pt.x += MOVELEN * (m_bDirect == TRUE?-1:1);
	m_nMovelen += MOVELEN;	

	if(m_nMovelen >= MOVELEN * m_nIndex ){		
		m_nMovelen = 0;
		m_bMoveStart = FALSE;
		m_nDelay = 0;

		if(m_bDirect == FALSE){
			pt.x = m_ptPos.x;	
			if(m_bOPen == FALSE)
			{
				m_bFront = TRUE;				
				//JOKER_GAME
				if(g_RI.FormKind == JOKER_GAME && g_bGetJokerCard == TRUE && m_bJokerCard == TRUE)
				{					
					// 다른 유저들에게 조커 카드를 넣어준다
					if(m_nJokerCard > -1)m_nCardNo = m_nJokerCard;
				}
			}
		}
		else{
			pt.x = m_ptPos.x + (-MOVELEN * m_nIndex);
		}

		//m_cMove.SetCurPos(pt);
	}
	//else m_cMove.SetCurPos(pt);	
}

int  CMyCard::GetCardNum()
{	
	return m_nJokerCard > -1 ? m_nJokerCard : m_nCardNo;
}