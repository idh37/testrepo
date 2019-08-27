#include "StdAfx.h"
#include "PromotionJackpotRelease.h"

CPromotionJackpotRelease::CPromotionJackpotRelease(  CPromotionManager *pManager, int nType  )
: CPromotionBase(pManager, nType)
{

	string strDir = ".\\PokerCommon\\data\\Event\\Release\\";
	_LOADSPRITEDEFINE(&m_sprEvent, strDir, "Jackpotrelease.spr");	

	m_AniIndex = 0;
	m_AniIndex_DP = 0;

	m_bReleaseInfo = FALSE;
	m_dwRelaseInfoTime = 0;

	m_bMouseOver = FALSE;
	m_JackPokWinnerEft = 0;
	
	memset( &m_Jackpot_Info, 0, sizeof(m_Jackpot_Info) );

	m_rtInfo.SetRect( 20, 42, 20 + 228, 42 + 63 );

}

CPromotionJackpotRelease::~CPromotionJackpotRelease(void)
{

}

void CPromotionJackpotRelease::OnStartGame()
{
	if( !g_RI.bEventJackpotTime ) return;

	SetEventInfo();

#ifdef _DEBUG
	//서버에서 인원 체크정보가 안오므로 클라이언트에서 한다.
	if( GM().GetCurrentGame()->GetGameProcess()->GetStartUserNum() < 3 )
#else
	if( GM().GetCurrentGame()->GetGameProcess()->GetStartUserNum() < 4 )
#endif
	{
		m_AniIndex_DP = 0;
	}
	else
	{
		m_AniIndex_DP = m_AniIndex;
	}
}

void CPromotionJackpotRelease::Accept_CreateRoom()
{
//	m_AniIndex = 1;
	m_bReleaseInfo = FALSE;
	m_dwRelaseInfoTime = 0;
	
	m_JackPokWinnerEft = 0;
}

void CPromotionJackpotRelease::Accept_OutRoom()
{
	m_AniIndex = 0;
	m_AniIndex_DP = 0;

	m_bMouseOver = FALSE;
		
	m_JackPokWinnerEft = 0;

	m_bReleaseInfo = FALSE;
	m_dwRelaseInfoTime = 0;

	memset( &m_Jackpot_Info, 0, sizeof(m_Jackpot_Info) );
}

void CPromotionJackpotRelease::SetEventInfo()
{
	if( !g_RI.bEventJackpotTime ) return;

	//잭팟판 시작
	if( m_Jackpot_Info.bJackPotEventGiftTime )
	{
		if( GM().GetCurrentGameType() == IDX_GAME_SP )	m_AniIndex = 5;
		else if( GM().GetCurrentGameType() == IDX_GAME_BD ) m_AniIndex = 6;
		else m_AniIndex = 7;
	}
	//새로 카운트 시작판
	else if( m_Jackpot_Info.bCurrentRound )
	{
		m_AniIndex = 1;
	}
	//판수
	else
	{
		m_AniIndex = m_Jackpot_Info.nRoundCount + 1;
	}
}

void CPromotionJackpotRelease::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !g_RI.bEventJackpotTime && m_bReleaseInfo == FALSE ) return;
	
	//다른방에서 누군가 당첨됐을경우
	if( m_AniIndex_DP == 8 )
	{
		pPage->DrawMultiScene( 20, 33, &m_sprEvent, m_AniIndex_DP, timeGetTime() - m_dwRelaseInfoTime, FALSE );

		if( m_sprEvent.IsMSceneTimeOver( m_AniIndex_DP, timeGetTime() - m_dwRelaseInfoTime ) )
		{
			m_bReleaseInfo = FALSE;
			m_dwRelaseInfoTime = 0;
		}
	}
	else
	{
		pPage->DrawMultiScene( 20, 33, &m_sprEvent, m_AniIndex_DP, timeGetTime(), TRUE );
	}
}

void CPromotionJackpotRelease::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !g_RI.bEventJackpotTime ) return;
}

void CPromotionJackpotRelease::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !g_RI.bEventJackpotTime ) return;

	//안내 팝업 노출
	if( m_bMouseOver )
	{
		pPage->DrawMultiScene( 347, 126, &m_sprEvent, 9, timeGetTime() - m_dwMouseTimer, TRUE );
	}
}

BOOL CPromotionJackpotRelease::OnMouseMoveBottom(int &x , int &y)
{
	CPoint point;
	point.x = x;
	point.y = y;

	if( m_rtInfo.PtInRect( point ) )
	{
		if( m_bMouseOver == FALSE )
		{
			m_bMouseOver = TRUE;
			m_dwMouseTimer = timeGetTime();
		}
	}
	else
	{
		m_bMouseOver = FALSE;
	}

	return FALSE;
}





BOOL CPromotionJackpotRelease::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
#ifdef _DEBUG

#define TRACE_PACKET_CASE(sig) case sig: \
	{ COleDateTime time; time=COleDateTime::GetCurrentTime(); CString strTime=time.Format("%H:%M:%S");  \
	TRACE( "[%s] Packet Received : " #sig "\n", strTime ); }
#else
#define TRACE_PACKET_CASE(sig) case sig:
#endif
	switch(Signal)
	{
		TRACE_PACKET_CASE(OPCODE_SV_JP_EVENT_GIFTTIME)
		{
			if( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) break;

			packet::TSendPacket< jackpot::JP_EVENT_GIFTTIME > JP_Round_Packet;
			JP_Round_Packet.Get( lpData, TotSize );
			jackpot::JP_EVENT_GIFTTIME* pAlaram = static_cast< jackpot::JP_EVENT_GIFTTIME* >( JP_Round_Packet.Various() );

			memcpy( &m_Jackpot_Info, pAlaram, sizeof( jackpot::JP_EVENT_GIFTTIME ) );

			if( g_RI.bEventJackpotTime )
			{
				SetEventInfo();
			}

// 			char szMsg[ MAX_PATH ] ={ 0,};
// 
// 			sprintf( szMsg, "잭팻 대방출 정보 - 잭팟판시작[%d], 새로운판시작[%d], 현재라운드수[%d]\n", 
// 				pAlaram->bJackPotEventGiftTime, pAlaram->bCurrentRound, pAlaram->nRoundCount );
// 
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB(227, 227, 0) );
		} break;

	TRACE_PACKET_CASE(OPCODE_SV_JP_EVENTTIME_ALLUSER)
		{
			packet::TSendPacket< jackpot::JP_EVENTTIME_ALLUSER > JP_User_Packet;
			JP_User_Packet.Get( lpData, TotSize );
			jackpot::JP_EVENTTIME_ALLUSER* pAlaram = static_cast< jackpot::JP_EVENTTIME_ALLUSER* >( JP_User_Packet.Various() );

			//시간으로 끝내는 경우가 아닌경우는 개별적인 종료 ( 당첨이됐을경우 )
			if( pAlaram->bBroadcast == FALSE )
			{
				if( g_RI.llEnterLimitMoney == pAlaram->llLimitMoney && pAlaram->sGameCode == GM().GetCurrentGameCode() )
				{
					//g_RI.bEventJackpotTime = pAlaram->bJackPotEventTime;
					
					m_JackPokWinnerEft = 1;	//잭팟 당첨 메세지가 오면 이펙트를 나오게함
				}
			}

		} break;
	// 잭팟 당첨금을 클라이언트에 전송 전체메세지 /
	TRACE_PACKET_CASE(OPCODE_SV_JP_WINNER_ALRAM)
		{
			if( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) break;

			packet::TSendPacket< jackpot::JP_WIN > JP_WIN_Packet;
			JP_WIN_Packet.Get( lpData, TotSize );
			jackpot::JP_WIN* pAlaram = static_cast< jackpot::JP_WIN* >( JP_WIN_Packet.Various() );

			//당첨방에서는 이펙트를 보여주지 않는다.
			if( g_RI.RoomNum == pAlaram->sToRoom ) 
			{
				if (GM().GetCurrentGameType() == IDX_GAME_HA)
				{
//					g_GAMEPROCESS()->SetJacpotAnimationState(pAlaram);
//					GM().GetCurrentGame->ReceiveOPCODE_SV_JP_WINNER_ALRAM(pAlaram);
				}
				break;
			}
			

			//같은 그룹의 잭팟 당첨자인가 확인한다.
			if( m_JackPokWinnerEft == 1 && g_RI.llEnterLimitMoney == pAlaram->JData.llLimitMoney && GM().GetCurrentGameCode() == pAlaram->JData.sGameCode )
			{
				m_AniIndex_DP = m_AniIndex = 8;
				m_JackPokWinnerEft = 2;

				m_bReleaseInfo = TRUE;
				m_dwRelaseInfoTime = timeGetTime();
			}

// 			char szMsg[ MAX_PATH ] ={ 0,};
// 			sprintf( szMsg, "!!! 다른방 잭팟 당첨!!! \n");
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB( 227, 227, 0 ) );
		} break;

	//같은 입장 머니 그룹에 대해서만 보내는 메세지
	TRACE_PACKET_CASE(OPCODE_SV_JP_EVENTTIME_ROOM)
		{
			packet::TSendPacket< jackpot::JP_EVENTTIME_ROOM > JP_EventTime_Packet;
			JP_EventTime_Packet.Get( lpData, TotSize );
			jackpot::JP_EVENTTIME_ROOM* pAlaram = static_cast< jackpot::JP_EVENTTIME_ROOM* >( JP_EventTime_Packet.Various() );

			//잭팟 타임 진행 중 잭팟 당첨자가 있을경우
// 			if( m_JackPokWinnerEft != 2 && g_RI.bEventJackpotTime )
// 			{
// 				m_AniIndex_DP = m_AniIndex = 8;
// 				m_JackPokWinnerEft = 2;
// 
// 				m_bReleaseInfo = TRUE;
// 				m_dwRelaseInfoTime = timeGetTime();
// 			}

			//잭팟타임을 켜거나 끈다.
			g_RI.bEventJackpotTime = pAlaram->bJackPotEventTime;

			m_Jackpot_Info.bCurrentRound = false;
			m_Jackpot_Info.bJackPotEventGiftTime = false;
			m_Jackpot_Info.nRoundCount = 0;

// 			char szMsg[ MAX_PATH ] ={ 0,};
// 			sprintf( szMsg, "!!! SV_JP_EVENTTIME_ROOM!!! \n");
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB( 227, 227, 0 ) );
		} break;
	}

	return FALSE;
}

const bool CPromotionJackpotRelease::IsShowTab(void)
{
	return false;
}
