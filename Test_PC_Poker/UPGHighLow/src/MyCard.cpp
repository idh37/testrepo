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

	// ȭ�鿡 ī�带 ���̵��� �����Ѵ�.
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
			// ���µ� ī��
			if(m_bCardFront)
			{	
				// ��� ���� ī�� ȿ��
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
			// ������ �ִ� ī��
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
	// �̵�ī�� ó��
	m_cMove.OnTimer(); 


	// ī�� �̵��� �����ٸ� �Ʒ� ó��
	if( m_cMove.IsMoveEnd() )
	{
		if(m_bEvent)
		{
			if(m_cEvent.EventCode == EVENT_PREPARESTART_ONEEND)		// ī�� ���ö� ���� �޾ҵ�
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent);
				ClearEvent();
			}
			else if(m_cEvent.EventCode == EVENT_THREEFLYEND)		// ù 3���� ��� ���ȴ�
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent);
				ClearEvent();
			}
			else if(m_cEvent.EventCode == EVENT_ONEFLYEND)			// ���徿	
			{			
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent);
				ClearEvent();
			} 		
		}

		if(m_bEvent1)
		{
			if(m_cEvent1.EventCode == EVENT_ONEFLYTOTEND)			// ���徿 ���
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent1);
				ClearEvent1();
			}		
		}
	}	
}


// 1 ��° �̺�Ʈ
void CMyCard::SetEvent(int eventcode, int option, int option1, int option2)
{
	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));

	m_bEvent = TRUE;
	m_cEvent.CardNo		= m_nCardNum;
	m_cEvent.EventCode	= eventcode;
	m_cEvent.Option		= option;
	m_cEvent.Option2	= option1;
}

// 2��° �̺�Ʈ
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