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


// Ŀ�ýô� �����̴� �������� �����Ѵ�.
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
				// �ٸ����� ó�����̸� ��� ������ �ʴ´�.
				if ( pLastMoveCard->nPNum == pData->nPNum && pLastMoveCard->GetLastCMD()  > -1 ) // ��ī���� �������忡�� �̺�Ʈ�� ����ϱ� ���ؼ�!
				{
					pLastMoveCard->SetLastCMD( -1 );
				}
			}
		}	
	}

	pData->SetF4CardUpdate( false );		  // Ŀ�ýô� ���� ������Ʈ �����Ѵ�
	m_cMoveCardList.push_back( ( CNMovingCard* )pData );	
}

// ó�� 4���� ������Ʈ ���ÿ� �����Ѵ�.
void CNMovingCardMan::ADDCardF4( CNMovingCard *pData ) 
{
	TPASSERT( pData != NULL );
	if ( !m_cMoveCardList.empty() )
	{				
		CNMovingCard* pLastMoveCard = m_cMoveCardList.back();			
		if ( pLastMoveCard != NULL )
		{
			pLastMoveCard->SetLastCMD( -1 ); // �������忡 �̺�Ʈ ����ϱ� ���� ��� �����.
		}
	}	

	pData->SetF4CardUpdate( true );		  // ó�� 4���� ���ÿ� ������Ʈ �Ѵ� ���� ó��
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
		
		// ī�� Ŀ�ý� ���� ������Ʈ �ϱ� ����, ó�� 4���� ���ÿ� ������Ʈ
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

//�ٵ��̿����� ����
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