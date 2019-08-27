#include "stdafx.h"
#include "PromotionJokboComplete.h"
#include <EventCommon.h>
#include "PromotionManager.h"
#include <CardRule.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPromotionJokboComplete::CPromotionJokboComplete(CPromotionManager *pManager, int nType)
: CPromotionBase(pManager, nType)
{
	string strDir = ".\\PokerCommon\\data\\Event\\JokboComplete\\";
	_LOADSPRITEDEFINE(&m_sprBack, strDir, "Back.spr");
	_LOADSPRITEDEFINE(&m_sprCard, strDir, "Card.spr");
}

CPromotionJokboComplete::~CPromotionJokboComplete()
{

}

BOOL CPromotionJokboComplete::OnLButtonDownBottom(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnLButtonUpBottom(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnMouseMoveBottom(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnLButtonDownMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnLButtonUpMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnMouseMoveMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnLButtonDownTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnLButtonUpTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnMouseMoveTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnEventTabLButtonDown(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnEventTabLButtonUp(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionJokboComplete::OnEventTabMouseMove(int &x , int &y)
{
	return FALSE;
}

void CPromotionJokboComplete::SetStartInfo( promotion::OCTOBER_ALRAM *pInfo )
{
	memcpy( &m_cPromotionInfo, pInfo, sizeof( promotion::OCTOBER_ALRAM ) );

	m_nPlayRound = m_cPromotionInfo.nPlayCount % 3;
	m_nPlayCount = m_cPromotionInfo.nGold_PlayCount % 5;
}

void CPromotionJokboComplete::SetEventInfo( promotion::OCTOBER_ALRAM *pInfo )
{
	m_nPlayRound = m_cPromotionInfo.nPlayCount % 3;
	m_nPlayCount = m_cPromotionInfo.nGold_PlayCount % 5;

	int nComplete = m_cPromotionInfo.nPlayCount % 15;

	//5개의 코인을 획득했을 경우
	if( m_cPromotionInfo.nGold_PlayCount >= 5 && m_nPlayCount == 0 && nComplete == 0 )
	{
		//ShowComplet();
	}
	//1개의 코인획득을 했을 경우
	else if( m_cPromotionInfo.bComplete == true )
	{
		//ShowCardOpen();
	}
}

void CPromotionJokboComplete::SetLimitCnt( int nCnt )
{
	m_cPromotionInfo.nLimitCnt = nCnt;
}

BOOL CPromotionJokboComplete::OnPacketNotify(int &Sid, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{
	case OPCODE_SV_OCTOBER_START:
		{
			packet::TSendPacket< promotion::OCTOBER_ALRAM > JokboComplete_Packet;

			JokboComplete_Packet.Get( lpData, TotSize );

			promotion::OCTOBER_ALRAM* pAlaram = 
				static_cast< promotion::OCTOBER_ALRAM* >( JokboComplete_Packet.Various() );

			SetActive( true );
			SetStartInfo( pAlaram );
		}
		return TRUE;
	case OPCODE_SV_OCTOBER_ALRAM:
		{
			packet::TSendPacket< promotion::OCTOBER_ALRAM > JokboComplete_Packet;

			JokboComplete_Packet.Get( lpData, TotSize );

			promotion::OCTOBER_ALRAM* pAlaram = 
				static_cast< promotion::OCTOBER_ALRAM* >( JokboComplete_Packet.Various() );

			SetEventInfo( pAlaram );
		}
		return TRUE;
	case OPCODE_SV_OCTOBER_INITIALIZE:
		{
			//추가
			packet::TSendPacket< promotion::OCTOBER_ALRAM > JokboComplete_Packet;

			JokboComplete_Packet.Get( lpData, TotSize );

			promotion::OCTOBER_ALRAM* pAlaram = 
				static_cast< promotion::OCTOBER_ALRAM* >( JokboComplete_Packet.Various() );


			SetLimitCnt( 0 );
			SetEventInfo( pAlaram );
		}
		return TRUE;
	case OPCODE_SV_OCTOBER_END:
		{
			SetActive( false );
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CPromotionJokboComplete::OnCommand(WPARAM &wParam, LPARAM &lParam)
{
	return FALSE;
}

void CPromotionJokboComplete::DrawBottom(CDC *pDC, CPage *pPage)
{
}

void CPromotionJokboComplete::DrawMiddle(CDC *pDC, CPage *pPage)
{
}

void CPromotionJokboComplete::DrawTop(CDC *pDC, CPage *pPage)
{
}

void CPromotionJokboComplete::DrawEventTab(CDC *pDC, CPage *pPage, const CPoint &ptStart)
{
	pPage->DrawMultiScene(0, 0, &m_sprBack, 0, 0);

	if(IsActive())
	{

	}
	//일일 참역 갯수 초과시
	else
	{
		pPage->DrawMultiScene(0, 0, &m_sprBack, 3, 0);
	}
}