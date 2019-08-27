// NMyCard.cpp: implementation of the CMyCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MyCard.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyCard::CMyCard()
{
	Clear();	
}

CMyCard::~CMyCard()
{

}

void CMyCard::Clear()
{	
	m_cMove.ReSet();

	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));
	ZeroMemory(&m_cEvent1, sizeof(CARDEVENT));

	m_nCardNum = -1;

	SetCardShow(FALSE);
	m_bCardFront = FALSE;
	SetResultDarkDraw(FALSE);
	
	SetHiddenCard(FALSE);
	SetShowHiddenCard(FALSE);
}

void CMyCard::Reset()
{
	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));
	ZeroMemory(&m_cEvent1, sizeof(CARDEVENT));

	SetCardShow(FALSE);
	m_bCardFront = FALSE;
	SetResultDarkDraw(FALSE);	

	m_ptCardDrawPos.x = m_ptCardDrawPos.y = 0;
}

void CMyCard::Init(int nCardnum)
{
	Clear();	
	m_nCardNum	= nCardnum;	

	// 화면에 카드를 보이도록 세팅한다.
	SetCardShow(TRUE);
	m_cMove.SetSoundID(SND_CARD_MOVE);
}

void CMyCard::SetCardPos(int tx, int ty)
{
	m_ptCardDrawPos.x = tx;
	m_ptCardDrawPos.y = ty;
	
	m_cMove.SetCurPos(m_ptCardDrawPos);
}

void CMyCard::Draw(NMBASE::GRAPHICGDI::CPage *pPage, BOOL bDie, BOOL bFoldOpen)
{
	if(pPage == NULL || m_bCardShow == FALSE)
	{
		return;
	}

	if(m_cMove.IsMoveEnd())
	{
		int nSprNum = (m_bCardFront) ? m_nCardNum : g_nDUMY_CARD;

		CPoint ptCardEnd = m_cMove.GetCurPos();

		NMBASE::GRAPHICGDI::xSprite *pCardSpr = NULL;

		if(bDie && (bFoldOpen == false))
		{
			pCardSpr = &g_sprDarkCard;
		}
		else
		{
			pCardSpr = &g_sprBigCard;		
		}

		if( pCardSpr != NULL )
		{		
			// 오픈된 카드
			if(m_bCardFront)
			{	
				// 결과 제외 카드 효과
				if(IsResultDarkDraw())
				{
					pCardSpr = &g_sprDarkCard;		
					pPage->PutSprAuto(ptCardEnd.x, ptCardEnd.y, pCardSpr, nSprNum);
				}	
				else
				{
					pPage->PutSprAuto(ptCardEnd.x, ptCardEnd.y, pCardSpr, nSprNum);
				}				
			}
			// 뒤집혀 있는 카드
			else
			{			
				pPage->PutSprAuto(ptCardEnd.x, ptCardEnd.y, pCardSpr, g_nDUMY_CARD);				
			}		
		}
	}
	else
	{
		if(m_cMove.GetMoveType() != CMovementEx::MT_NORMAL)
		{
			m_cMove.DrawSprite(pPage);
		}		
	}	
}


BOOL CMyCard::IsPtInCard(POINT ptPoint)
{
	BOOL bOPen =  FALSE;
	
	CRect rc;	
	rc.SetRect( m_ptCardDrawPos.x, m_ptCardDrawPos.y, m_ptCardDrawPos.x + CARD_WIDTH, m_ptCardDrawPos.y + CARD_HEIGHT );

	if(rc.PtInRect( ptPoint )) 
	{
		bOPen = TRUE;
	}

	return bOPen;

}


void CMyCard::OnTimer()
{
	// 이동카드 처리
	m_cMove.OnTimer(); 


	// 카드 이동이 끝났다면 아래 처리
	if( m_cMove.IsMoveEnd() )
	{
		if(m_bEvent)
		{
			if(m_cEvent.EventCode == EVENT_PREPARESTART_ONEEND)		// 카드 선택때 한장 받았따
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent);
				ClearEvent();
			}
			else if(m_cEvent.EventCode == EVENT_THREEFLYEND)		// 첫 3장을 모두 날렸다
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent);
				ClearEvent();
			}
			else if(m_cEvent.EventCode == EVENT_ONEFLYEND)			// 한장씩	
			{			
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent);
				ClearEvent();
			} 		
		}

		if(m_bEvent1)
		{
			if(m_cEvent1.EventCode == EVENT_ONEFLYTOTEND)			// 한장씩 모두
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent1);
				ClearEvent1();
			}		
		}
	}	
}


// 1 번째 이벤트
void CMyCard::SetEvent(int eventcode, int option, int option1, int option2)
{
	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));

	m_bEvent = TRUE;
	m_cEvent.CardNo		= m_nCardNum;
	m_cEvent.EventCode	= eventcode;
	m_cEvent.Option		= option;
	m_cEvent.Option2	= option1;
}

// 2번째 이벤트
void CMyCard::SetEvent1(int eventcode, int option, int option1, int option2)
{	
	ZeroMemory(&m_cEvent1, sizeof(CARDEVENT));

	m_bEvent1 = TRUE;
	m_cEvent1.CardNo	= m_nCardNum;
	m_cEvent1.EventCode = eventcode;
	m_cEvent1.Option	= option;
	m_cEvent1.Option2	= option1;
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