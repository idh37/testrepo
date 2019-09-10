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
	ZeroMemory(&m_cEvent2, sizeof(CARDEVENT));

	m_nCardNum = -1;

	SetCardShow(FALSE);
	m_bCardFront = FALSE;
	SetResultDarkDraw(FALSE);
	
	SetHiddenCard(FALSE);
	SetShowHiddenCard(FALSE);

	SetReadyToJJogi(FALSE, CPoint(0,0));

	m_bLButtonDown =FALSE;
	m_bCompeteJJogi=FALSE;



	m_bCardTornOut = FALSE;
	m_nCardMonthValue = 0;
	m_nCardBlinkTime = 0;
	m_bCardBlink=0;

	m_bEvent = FALSE;
	m_bEvent1 = FALSE;
	m_bEvent2 = FALSE;

}

void CMyCard::Reset()
{
	m_bEvent = FALSE;
	m_bEvent1 = FALSE;
	m_bEvent2 = FALSE;
	ZeroMemory(&m_cEvent, sizeof(CARDEVENT));
	ZeroMemory(&m_cEvent1, sizeof(CARDEVENT));
	ZeroMemory(&m_cEvent2, sizeof(CARDEVENT));

	SetCardShow(FALSE);
	m_bCardFront = FALSE;
	SetResultDarkDraw(FALSE);	

	m_ptCardDrawPos.x = m_ptCardDrawPos.y = 0;

	m_bLButtonDown =FALSE;
	m_bCompeteJJogi=FALSE;

	m_bCardTornOut = FALSE;

	m_nCardBlinkTime = 0;
	m_bCardBlink=0;
}

void CMyCard::Init(int nCardnum)
{
	Clear();	
	m_nCardNum	= nCardnum;	

	// ȭ�鿡 ī�带 ���̵��� �����Ѵ�.
	SetCardShow(TRUE);
	m_cMove.SetSoundID(SND03);
}

void CMyCard::SetCardPos(int tx, int ty)
{
	m_ptCardDrawPos.x = tx;
	m_ptCardDrawPos.y = ty;
	
	m_cMove.SetCurPos(m_ptCardDrawPos);
}

void CMyCard::Draw(NMBASE::GRAPHICGDI::CPage *pPage, BOOL bDie, BOOL bFoldOpen )
{
	if(pPage == NULL || m_bCardShow == FALSE)
	{
		return;
	}
	 

	//if(m_cMove.IsMoveEnd())
	if(!m_cMove.IsMoveStart() || m_cMove.IsMoveEnd()) 
	{
		int nCardOpt = 0;
		int nCardGrade = 31;
		WORD nCardColor = 0;

	
		int nSprNum = (m_bCardFront) ? m_nCardNum : g_nDUMY_CARD;	

		CPoint ptCardEnd = m_cMove.GetCurPos();

		NMBASE::GRAPHICGDI::xSprite *pCardSpr = NULL;

		if (m_bCardTornOut)
		{
			pCardSpr = &g_pGameView->m_AniQue.m_AniSpr[ANIMATION_WINLOSS];
			ptCardEnd.Offset(0,-5);//������ ī��� �������� ��´�.
		}
		else if( bDie )
		{
			//pCardSpr = &g_sprDarkCard;		
			pCardSpr = &g_sprBigCard;
			
			if( !bFoldOpen ) nCardOpt = GLOW;
			nCardGrade = 22;
			nCardColor = NMBASE::GRAPHICGDI::GetRGBmix(0,0,0);
		}
		else
		{
			pCardSpr = &g_sprBigCard;		
		}

		//ī�� ������ ȿ��
		if (m_bCardBlink)
		{
			if ((m_nCardBlinkTime/8)%2)
			{
				nCardOpt = GLOW;
				nCardGrade = 18;
				nCardColor = NMBASE::GRAPHICGDI::GetRGBmix(31,11,0);
			}
		}

		if( pCardSpr != NULL )
		{		
			// ���µ� ī��
			if(m_bCardFront)
			{	
				// ��� ���� ī�� ȿ��
				if(IsResultDarkDraw())
				{
					//pCardSpr = &g_sprDarkCard;		
					pPage->PutSprAuto(ptCardEnd.x, ptCardEnd.y, pCardSpr, nSprNum, nCardOpt, nCardGrade,  nCardColor);
				}	
				else
				{
					pPage->PutSprAuto(ptCardEnd.x, ptCardEnd.y, pCardSpr, nSprNum, nCardOpt, nCardGrade,  nCardColor);
				}				
			}
			// ������ �ִ� ī��
			else
			{			
				pPage->PutSprAuto(ptCardEnd.x, ptCardEnd.y, pCardSpr, g_nDUMY_CARD, nCardOpt, nCardGrade,  nCardColor);				
				

				// ����ī�� ���� ����Ű �̹����� �����ش�.
				if (IsReadyToJJogi())
					pPage->PutSprAuto( ptCardEnd.x + 2, ptCardEnd.y + 59, &g_sprGLOBAL[ spr::GSPR_V_H_OPEN ], 0 );
			}		
		}
	}
	else
	{
		 //if(m_cMove.GetMoveType() != CMovementEx::MT_NORMAL)
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
		CPoint pt = m_cMove.GetCurPos();
		SetCardPos(pt.x, pt.y);


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
			else
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent);
				ClearEvent();
			}
		}
	}


	if( m_cMove.IsMoveEnd() )
	{

		if(m_bEvent1)
		{
			if(m_cEvent1.EventCode == EVENT_ONEFLYTOTEND)			// ���徿 ���
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent1);
				ClearEvent1();
			}
			else
			{
				g_GAMEPROCESS()->OnCardEvent(&m_cEvent1);
				ClearEvent1();
			}
		}
	}

	if( m_cMove.IsMoveEnd() )
	{
		if(m_bEvent2)
		{		
			bool b= m_cMove.IsMoveEnd() ;

			g_GAMEPROCESS()->OnCardEvent(&m_cEvent2);
			ClearEvent2();
		
		}		
	}	


	if (m_bCardBlink)
	{
		m_nCardBlinkTime --;
		if (m_nCardBlinkTime<=0)
		{
			m_bCardBlink = FALSE;
			m_nCardBlinkTime = 0;
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

// 3��° �̺�Ʈ
void CMyCard::SetEvent2(int eventcode, int option, int option1, int option2)
{	
	ZeroMemory(&m_cEvent2, sizeof(CARDEVENT));

	m_bEvent2 = TRUE;
	m_cEvent2.CardNo	= m_nCardNum;
	m_cEvent2.EventCode = eventcode;
	m_cEvent2.Option	= option;
	m_cEvent2.Option2	= option1;
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

void CMyCard::ClearEvent2()
{
	ZeroMemory(&m_cEvent2, sizeof(CARDEVENT));
	m_bEvent2 = FALSE;
}



BOOL CMyCard::OnMouseMove(int x, int y)
{	
	if (!IsReadyToJJogi())
		return FALSE;

	if (!m_bLButtonDown)
		return FALSE;
	
// 	CRect rtCard (m_ptCardDrawPos.x, m_ptCardDrawPos.y, m_ptCardDrawPos.x+CARD_WIDTH, m_ptCardDrawPos.y+CARD_HEIGHT);
// 
// 	if (rtCard.PtInRect(CPoint(x,y)))
// 	{
// 
// 	}
	int nMovedDist = x - m_ptJJokgiMouseBackup.x;
	if (nMovedDist < 0)
	{
		return FALSE;
	}
	
	SetCardPos(m_ptCardDrawPos.x + nMovedDist, m_ptCardDrawPos.y);
	m_ptJJokgiMouseBackup =  CPoint(x,y);
	

	if (m_ptJJokgiComplete.x - m_ptCardDrawPos.x  <= 0)
	{
		return TRUE;
	}

	return FALSE;

}
BOOL CMyCard::OnLButtonDown(int x, int y)
{
	if (!IsReadyToJJogi())
		return FALSE;

	if (m_bLButtonDown)
		return FALSE;
	
	CRect rtCard(m_ptCardDrawPos.x, m_ptCardDrawPos.y, m_ptCardDrawPos.x+CARD_WIDTH, m_ptCardDrawPos.y+CARD_HEIGHT);

	if (rtCard.PtInRect(CPoint(x,y)))
	{
		m_bLButtonDown = TRUE;
		m_ptJJokgiMouseBackup = CPoint(x,y);
		return TRUE;
	}	

	return FALSE;
}
BOOL CMyCard::OnLButtonUp(int x, int y)
{
	if (!IsReadyToJJogi())
		return FALSE;

	if (!m_bLButtonDown)
		return FALSE;

	return TRUE;///�����⸦ �Ϸ��Ѵ�.
}

void CMyCard::SetReadyToJJogi(BOOL bReadyToJJogi, CPoint ptJJokgiComplete) 
{
	m_bReadyToJJogi = bReadyToJJogi;		//���콺�� �ɱ��� �� �ִ� �����ΰ�?
	m_ptJJokgiComplete= ptJJokgiComplete; 
	m_bLButtonDown = FALSE; 
	m_bCompeteJJogi = FALSE; 

}		

void CMyCard::CompleteJJogi(BOOL bNotifyServer, CPoint ptFinalPos)	//�����⸦ �Ϸ� ��Ų��.
{	
	if (IsCompleteJJogi())
		return;

	m_bReadyToJJogi = FALSE;
	m_bLButtonDown =FALSE;
	m_bCompeteJJogi =TRUE;
	SetCardPos(ptFinalPos.x, ptFinalPos.y);

	SetCardFront(TRUE);

	if (bNotifyServer)
	{
		//������ ������ �ϷḦ �˷��ش�.
		CCL_SD_JJOGI msg;
		msg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}


}


void CMyCard::MoveCard(CPoint ptEnd, int nIndex, DWORD dwMoveTime, DWORD dwDelay)
{

	GetMove().StartMove(&g_sprBigCard, GetPos(), ptEnd, nIndex, dwMoveTime, dwDelay);


}

void CMyCard::SetCardTornOut(BOOL bFlag)
{

	m_bCardTornOut = bFlag;

}

int CMyCard::GetCardMonthValue()
{	
	return m_nCardMonthValue;

}

void CMyCard::SetCardNum(int nNum)
{
	ASSERT (nNum>=0 && nNum <= 20);
	

	m_nCardNum = nNum;

	m_nCardMonthValue = nNum/2 + (nNum%2);

}

void CMyCard::SetCardBlinking(int nTime)		//ī�� �����Ÿ��� ȿ��
{
	m_bCardBlink = TRUE;
	m_nCardBlinkTime = nTime;
}
