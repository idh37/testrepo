// NMovingCardMan.cpp: implementation of the CNMovingCardMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NMovingCardMan.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////	

CNMovingCardMan::CNMovingCardMan()
{
	m_cMoveCardList.clear();
}

CNMovingCardMan::~CNMovingCardMan()
{
	Destroy();
}

void CNMovingCardMan::Destroy()
{
	std::list< CNMovingCard* >::iterator iter =  m_cMoveCardList.begin();	
	
	while ( iter != m_cMoveCardList.end() )
	{
		SAFE_DELETE( *iter );
		++iter;
	}

	m_cMoveCardList.clear();
}


// 커팅시는 업데이는 유저별로 진행한다.
void CNMovingCardMan::ADDCard( CNMovingCard *pData,  bool bLastCommandDelete )
{
	TPASSERT( pData != NULL );
	if ( bLastCommandDelete )
	{
		if ( !m_cMoveCardList.empty() )
		{				
			CNMovingCard* pLastMoveCard = m_cMoveCardList.back();			
			if ( pLastMoveCard != NULL )
			{
				// 다른유저 처리중이면 명령 지우지 않는다.
				if ( pLastMoveCard->nPNum == pData->nPNum && pLastMoveCard->GetLastCMD()  > -1 ) // 내카드의 마지막장에만 이벤트를 등록하기 위해서!
				{
					pLastMoveCard->SetLastCMD( -1 );
				}
			}
		}	
	}

	pData->SetF4CardUpdate( false );		  // 커팅시는 각각 업데이트 진행한다
	m_cMoveCardList.push_back( ( CNMovingCard* )pData );	
}

// 처음 4장은 업데이트 동시에 진행한다.
void CNMovingCardMan::ADDCardF4( CNMovingCard *pData ) 
{
	TPASSERT( pData != NULL );
	if ( !m_cMoveCardList.empty() )
	{				
		CNMovingCard* pLastMoveCard = m_cMoveCardList.back();			
		if ( pLastMoveCard != NULL )
		{
			pLastMoveCard->SetLastCMD( -1 ); // 마지막장에 이벤트 등록하기 위해 모두 지운다.
		}
	}	

	pData->SetF4CardUpdate( true );		  // 처음 4장은 동시에 업데이트 한다 예외 처리
	m_cMoveCardList.push_back( ( CNMovingCard* )pData );	
}

void CNMovingCardMan::Draw(CDC* pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if ( m_cMoveCardList.empty() )
	{
		return;
	}

	std::list< CNMovingCard* >::iterator iter =  m_cMoveCardList.begin();	

	while ( iter != m_cMoveCardList.end() )
	{
		( *iter )->Draw( pPage );
		++iter;
	}
}

void CNMovingCardMan::Process()
{	
	if ( m_cMoveCardList.empty() )
	{
		return;
	}

	std::list< CNMovingCard* >::iterator iter =  m_cMoveCardList.begin();	

	CNMovingCard* pNMBeforeCard = static_cast< CNMovingCard* >( *iter );


	while ( iter != m_cMoveCardList.end() )
	{
		CNMovingCard* pNMCurCard = static_cast< CNMovingCard* >( *iter );
		
		// 카드 커팅시 각각 업데이트 하기 위해, 처음 4장은 동시에 업데이트
		if ( pNMBeforeCard->IsF4CardUpdate() ||  pNMBeforeCard->nPNum == pNMCurCard->nPNum ) 
		{
			pNMCurCard->OnTimer();
		}
		
		if ( pNMCurCard->IsDestroyMovingCard() )
		{
			SAFE_DELETE( pNMCurCard );
			iter = m_cMoveCardList.erase( iter );
			continue;
		}
		else if ( pNMCurCard->nPNum > -1 )
		{
			if ( g_PLAYER( pNMCurCard->nPNum )->JoinState == EGAME_JOINOUT || 
				 g_PLAYER( pNMCurCard->nPNum )->PlayState == EPLAYER_GAMEEND || 
				 g_PLAYER( pNMCurCard->nPNum )->bFold == TRUE )
			{
				SAFE_DELETE( pNMCurCard );
				iter = m_cMoveCardList.erase( iter );
				continue;
			}
		}

		++iter;
	}
}

bool CNMovingCardMan::IsCardMoving()
{	
	return ( m_cMoveCardList.empty() ) ? false : true;
}

//바둑이에서만 쓰임
bool CNMovingCardMan::IsCardMoving( int nPNum )
{	
	if ( m_cMoveCardList.empty() )
	{
		return false;
	}

	std::list< CNMovingCard* >::iterator iter =  m_cMoveCardList.begin();	

	while ( iter != m_cMoveCardList.end() )
	{
		CNMovingCard* pNMCard = static_cast< CNMovingCard* >( *iter );

		if ( pNMCard->m_sCardMoveData[ 0 ].m_nPlayNum == nPNum )
		{
			return true;
		}
		
		++iter;
	}

	return false;
}