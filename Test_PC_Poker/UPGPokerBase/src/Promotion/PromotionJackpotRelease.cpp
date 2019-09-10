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
	//�������� �ο� üũ������ �ȿ��Ƿ� Ŭ���̾�Ʈ���� �Ѵ�.
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

	//������ ����
	if( m_Jackpot_Info.bJackPotEventGiftTime )
	{
		if( GM().GetCurrentGameType() == IDX_GAME_SP )	m_AniIndex = 5;
		else if( GM().GetCurrentGameType() == IDX_GAME_BD ) m_AniIndex = 6;
		else m_AniIndex = 7;
	}
	//���� ī��Ʈ ������
	else if( m_Jackpot_Info.bCurrentRound )
	{
		m_AniIndex = 1;
	}
	//�Ǽ�
	else
	{
		m_AniIndex = m_Jackpot_Info.nRoundCount + 1;
	}
}

void CPromotionJackpotRelease::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !g_RI.bEventJackpotTime && m_bReleaseInfo == FALSE ) return;
	
	//�ٸ��濡�� ������ ��÷�������
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

	//�ȳ� �˾� ����
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
// 			sprintf( szMsg, "���� ����� ���� - �����ǽ���[%d], ���ο��ǽ���[%d], ��������[%d]\n", 
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

			//�ð����� ������ ��찡 �ƴѰ��� �������� ���� ( ��÷�̵������ )
			if( pAlaram->bBroadcast == FALSE )
			{
				if( g_RI.llEnterLimitMoney == pAlaram->llLimitMoney && pAlaram->sGameCode == GM().GetCurrentGameCode() )
				{
					//g_RI.bEventJackpotTime = pAlaram->bJackPotEventTime;
					
					m_JackPokWinnerEft = 1;	//���� ��÷ �޼����� ���� ����Ʈ�� ��������
				}
			}

		} break;
	// ���� ��÷���� Ŭ���̾�Ʈ�� ���� ��ü�޼��� /
	TRACE_PACKET_CASE(OPCODE_SV_JP_WINNER_ALRAM)
		{
			if( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) break;

			packet::TSendPacket< jackpot::JP_WIN > JP_WIN_Packet;
			JP_WIN_Packet.Get( lpData, TotSize );
			jackpot::JP_WIN* pAlaram = static_cast< jackpot::JP_WIN* >( JP_WIN_Packet.Various() );

			//��÷�濡���� ����Ʈ�� �������� �ʴ´�.
			if( g_RI.RoomNum == pAlaram->sToRoom ) 
			{
				if (GM().GetCurrentGameType() == IDX_GAME_HA)
				{
//					g_GAMEPROCESS()->SetJacpotAnimationState(pAlaram);
//					GM().GetCurrentGame->ReceiveOPCODE_SV_JP_WINNER_ALRAM(pAlaram);
				}
				break;
			}
			

			//���� �׷��� ���� ��÷���ΰ� Ȯ���Ѵ�.
			if( m_JackPokWinnerEft == 1 && g_RI.llEnterLimitMoney == pAlaram->JData.llLimitMoney && GM().GetCurrentGameCode() == pAlaram->JData.sGameCode )
			{
				m_AniIndex_DP = m_AniIndex = 8;
				m_JackPokWinnerEft = 2;

				m_bReleaseInfo = TRUE;
				m_dwRelaseInfoTime = timeGetTime();
			}

// 			char szMsg[ MAX_PATH ] ={ 0,};
// 			sprintf( szMsg, "!!! �ٸ��� ���� ��÷!!! \n");
// 			AddLobbyChatViewEdit( szMsg, RGB( 1, 1, 250 ) );
// 			AddGameChatViewEdit( szMsg , RGB( 227, 227, 0 ) );
		} break;

	//���� ���� �Ӵ� �׷쿡 ���ؼ��� ������ �޼���
	TRACE_PACKET_CASE(OPCODE_SV_JP_EVENTTIME_ROOM)
		{
			packet::TSendPacket< jackpot::JP_EVENTTIME_ROOM > JP_EventTime_Packet;
			JP_EventTime_Packet.Get( lpData, TotSize );
			jackpot::JP_EVENTTIME_ROOM* pAlaram = static_cast< jackpot::JP_EVENTTIME_ROOM* >( JP_EventTime_Packet.Various() );

			//���� Ÿ�� ���� �� ���� ��÷�ڰ� �������
// 			if( m_JackPokWinnerEft != 2 && g_RI.bEventJackpotTime )
// 			{
// 				m_AniIndex_DP = m_AniIndex = 8;
// 				m_JackPokWinnerEft = 2;
// 
// 				m_bReleaseInfo = TRUE;
// 				m_dwRelaseInfoTime = timeGetTime();
// 			}

			//����Ÿ���� �Ѱų� ����.
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
