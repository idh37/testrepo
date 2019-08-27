// NMovingCard.cpp: implementation of the CNMovingCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NMovingCard.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNMovingCard::CNMovingCard()
:m_bUseSceneMoving( false ),
m_bF4CardUpdate( false ),
m_pFuncCardEvent( NULL )
{
	Clear();
}

CNMovingCard::~CNMovingCard()
{
	Clear();
}

void CNMovingCard::Initialize( NCARDMOVEDATA *pData,int nCardcnt, int nMainDelayTime, int nMoveDirect, bool bUseSceneMoving, PFN_CARDEVENT pEventCB )
{
	TPASSERT( nCardcnt <= TOTAL_CARD_BD );

	Clear();

	m_pFuncCardEvent = pEventCB;

	for ( int i = 0; i < nCardcnt; ++i ) // 카드 장수만큼
	{
		memcpy( &m_sCardMoveData[ i ], &pData[ i ], sizeof(NCARDMOVEDATA) );
	}

	m_nCardCnt	= nCardcnt;
	m_nDelay	= nMainDelayTime;
	m_bUseSceneMoving = bUseSceneMoving; // 애니메이션카드 사용유무

	if( DIRECT_MOVE_PASS == nMoveDirect )
	{
		SetMovePass();
	}
	else if( DIRECT_MOVE_DOWN == nMoveDirect )
	{
		SetMoveDown();
	}
	else if ( DIRECT_MOVE_UP == nMoveDirect )
	{
		SetMoveUp();
	}	
	else
	{
		TPASSERT( 0 );
	}
}

void CNMovingCard::ReSetMovement()
{
	m_nDelayCount = 0;
	nDebugCnt	  = 0;
}

void CNMovingCard::Clear()
{	
	ReSetMovement();

	m_nDirect =-1;
	m_nDelayCount = 0;
	m_nDelay =0;
	m_bDestroy = false;
	m_nLastChangerCmd =-1;
	m_nLastChangerCmdPNum =-1;
	nPNum = -1;
	nDebugCnt = 0;
	m_bUseSceneMoving = false;
	m_bF4CardUpdate = false;

	for(int i = 0; i < TOTAL_CARD_BD; ++i)
	{
		m_sCardMoveData[i].Clear();	
	}

	m_ltMoveCardList.clear();
}

void CNMovingCard::SetMovePass()
{
	ReSetMovement();

	m_nDirect = DIRECT_MOVE_PASS;
	m_nDelayCount = m_nDelay;

	for ( int i = 0 ; i < 1 ; ++i)
	{
		NCARDMOVEDATA *pCardMoveData = NULL;
		pCardMoveData = &m_sCardMoveData[ i ];

		MOVECARD_LIST sCardData;
		sCardData.Clear();
		memcpy( &sCardData.m_sCardMoveData, &m_sCardMoveData[i] ,sizeof(NCARDMOVEDATA));
		m_ltMoveCardList.push_back(sCardData);		
	}
}

void CNMovingCard::SetMoveUp()
{
	ReSetMovement();

	m_nDirect	  = DIRECT_MOVE_UP;
	m_nDelayCount = m_nDelay;
	int nSceneIndex = 0;

	for(int i = 0 ; i < m_nCardCnt ; ++i)
	{
		NCARDMOVEDATA *pCardMoveData = NULL;
		pCardMoveData = &m_sCardMoveData[i];	

		MOVECARD_LIST stMoveList;
		stMoveList.Clear();
		memcpy( &stMoveList.m_sCardMoveData, &m_sCardMoveData[i], sizeof(NCARDMOVEDATA));

		// 유저당 총 4개의 카드 Animation이 있기 때문에 유저 위치에 8을 곱해 준다.
		nSceneIndex = pCardMoveData->m_nPlayNum *	DEALINGCARDGAP_BD;
		nSceneIndex += pCardMoveData->m_nIndex;  // 카드덱인덱스
		stMoveList.m_cCardMove.SetUseScene( m_bUseSceneMoving ); // 애니메이션 사용유무
	
		stMoveList.m_cCardMove.SetPosIndex( nSceneIndex );// 멀티씬인덱스
		stMoveList.m_cCardMove.StraightMoveTo(pCardMoveData->m_ptStartPos, pCardMoveData->m_ptDestPos, pCardMoveData->m_fSpeed, pCardMoveData->m_nDelay);
		m_ltMoveCardList.push_back(stMoveList);		
	}
}

void CNMovingCard::SetMoveDown()
{
	ReSetMovement();

	m_nDirect	  = DIRECT_MOVE_DOWN;
	m_nDelayCount = m_nDelay;
	int nSceneIndex = 0;

	for(int i = 0; i < m_nCardCnt ; ++i)
	{
		NCARDMOVEDATA *pCardMoveData = NULL;
		pCardMoveData = &m_sCardMoveData[i];		

		MOVECARD_LIST stMoveList;
		stMoveList.Clear();				
		memcpy( &stMoveList.m_sCardMoveData , &m_sCardMoveData[ i ] ,sizeof(NCARDMOVEDATA));

		
		// 유저당 총 4개의 카드 Animation이 있기 때문에 유저 위치에 8을 곱해 준다.
		nSceneIndex = pCardMoveData->m_nPlayNum *	DEALINGCARDGAP_BD;
		nSceneIndex += pCardMoveData->m_nIndex;

		stMoveList.m_cCardMove.SetUseScene( m_bUseSceneMoving ); // 애니메이션 사용유무

		stMoveList.m_cCardMove.SetPosIndex( nSceneIndex );
		stMoveList.m_cCardMove.StraightMoveTo( pCardMoveData->m_ptStartPos_2nd, pCardMoveData->m_ptDestPos_2nd, pCardMoveData->m_fSpeed, pCardMoveData->m_nDelay );
		m_ltMoveCardList.push_back( stMoveList) ;		
	}
}

void CNMovingCard::Draw(NMBASE::GRAPHICGDI::CPage *pPage)
{
	if ( pPage == NULL ) 
	{
		return;
	}

	if ( m_nDelayCount > 0 )
	{
		return;
	}

	if ( m_nDirect == DIRECT_MOVE_PASS )
	{
		return;
	}

	if ( m_ltMoveCardList.empty() )
	{
		return;
	}

	std::list< MOVECARD_LIST >::iterator iter = m_ltMoveCardList.begin();
	
	while( iter != m_ltMoveCardList.end() )
	{
		if ( m_bUseSceneMoving ) // 애니메이션 카드
		{
			(*iter).m_cCardMove.AniDraw( pPage );
		}
		else
		{
			if ( (*iter).m_cCardMove.GetDelay() > 0 )
			{
				++iter;
				continue;
			}

			if ( (*iter).m_cCardMove.IsStopped() )
			{
				++iter;
				continue;
			}

			POINT pt = (*iter).m_cCardMove.GetCurPos();

			int xp = pt.x;
			int yp = pt.y;

			NMBASE::GRAPHICGDI::xSprite *pSpr = &GLOBAL_SPR[ spr::GSPR_GAME_BIGCARD ];		// 뒷면만 쓰는거라 상관없음

			if ( pSpr != NULL )
			{
				int nBackCardIndex = 52;

				pPage->PutSprAuto( xp, yp, pSpr, nBackCardIndex );
			}
		}

		++iter;
	}
}

void CNMovingCard::OnTimer()
{	
	if ( m_nDelayCount > 0 )
	{
		m_nDelayCount--;
		return;
	}

	if ( m_ltMoveCardList.empty() )
	{
		return;
	}

	MOVECARD_LIST *pCardData = NULL;

	std::list< MOVECARD_LIST >::iterator iter = m_ltMoveCardList.begin();

	while ( iter != m_ltMoveCardList.end() )
	{
		pCardData = &(*iter);

		pCardData->m_cCardMove.OnTimer();

		if ( pCardData->m_cCardMove.GetDelay() == 0 ) // 
		{
			CARDEVENT event;
			ZeroMemory( &event, sizeof( CARDEVENT ) );

			if ( m_nDirect == DIRECT_MOVE_UP  )
			{	
				event.EventCode = EVENT_BD_CUTTING_WAIT;   // 카드 위로버리기 명령직후
				event.Option	= pCardData->m_sCardMoveData.m_nPlayNum;
				event.Option2	= pCardData->m_sCardMoveData.m_nIndex;
				m_pFuncCardEvent( &event, pCardData ); 
			}
			else if ( m_nDirect == DIRECT_MOVE_PASS )
			{
				// 사운드 이벤트
				event.EventCode = pCardData->m_sCardMoveData.m_nFirstCmd;
				event.Option	= pCardData->m_sCardMoveData.m_nPlayNum;
				event.Option2	= 0;
				m_pFuncCardEvent( &event, pCardData );

				ZeroMemory(&event,sizeof(CARDEVENT));
				event.Option	= m_nLastChangerCmdPNum;
				event.EventCode = m_nLastChangerCmd;
				event.Option3   = m_nDirect; 
				m_pFuncCardEvent( &event, NULL );
			
				pCardData->Clear();
				iter = m_ltMoveCardList.erase( iter );
				continue;
			}
		}

		// 이동이 끝나면 카드이동 리스트에서 삭제한다.
		if ( pCardData->m_cCardMove.IsStopped() )
		{
			if ( pCardData->m_sCardMoveData.m_nFirstCmd > -1 )
			{
				CARDEVENT event;
				ZeroMemory(&event,sizeof(CARDEVENT));

				if ( m_nDirect == DIRECT_MOVE_UP  )
				{
					event.EventCode = pCardData->m_sCardMoveData.m_nFirstCmd;
				}
				else if ( m_nDirect == DIRECT_MOVE_DOWN )
				{
					event.EventCode = pCardData->m_sCardMoveData.m_nSecondCmd;
				}
				event.Option	= pCardData->m_sCardMoveData.m_nPlayNum;
				event.Option2	= pCardData->m_sCardMoveData.m_nIndex;
				m_pFuncCardEvent( &event, NULL );
				
				pCardData->Clear();
				iter = m_ltMoveCardList.erase( iter );
				continue;
			}
		}
		++iter;
	}
	
	// 현재 작업 일경우 다음 이벤트 등록
	if ( m_ltMoveCardList.empty() )
	{
		if ( m_nDirect == DIRECT_MOVE_UP  )
		{	
			SetMoveDown(); // 다 올렸으면 내린다.
		}
		else if ( m_nDirect == DIRECT_MOVE_DOWN )
		{
			if ( m_nLastChangerCmd == EVENT_BD_PREPARESTART_FOURFLY_END || // 처음 4장 
				 m_nLastChangerCmd == EVENT_BD_CUTTING_ALL_DOWN			|| // 치인지 끝
				 m_nLastChangerCmd == EVENT_BD_CUTTING_ALL_PASS			   // 패스 끝
			   )
			{
				CARDEVENT event;
				ZeroMemory(&event,sizeof(CARDEVENT));

				event.Option	= m_nLastChangerCmdPNum;
				event.EventCode = m_nLastChangerCmd;
				event.Option3   = m_nDirect; 

				m_pFuncCardEvent( &event, NULL );

				m_nLastChangerCmd	  = -1;
				m_nLastChangerCmdPNum = -1;
			}	
			m_bDestroy = true;
			m_nDirect = -1;
		}
		else if ( m_nDirect == DIRECT_MOVE_PASS )
		{
			m_nLastChangerCmd		= -1;
			m_nLastChangerCmdPNum	= -1;
			m_bDestroy				= true;
			m_nDirect				= -1;
		}
	}
}
