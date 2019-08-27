// PlayTimeWarning.cpp: implementation of the CPlayTimeWarning class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlayTimeWarning.h"
#include "mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayTimeWarning::CPlayTimeWarning()
{
	m_nTotalMin = 0;

	m_nFirstPrint = FALSE;
	m_dwLoginTime = 0;
	m_nLastPrintTime = 0;
	m_nLastPrintTimeMin = 0;

	m_nHourToShow = 0;
	m_nMinToShow = 0;
	m_bShowTimeMessageAtEnterRoom = false;

	/*m_strNotice[0] = "장시간의 게임이용은 건강을 해칠 수 있습니다.\n";
	m_strNotice[1] = "공부와 일에 휴식이 필요하듯이 게임이용에도 휴식이 필요합니다.\n";*/

	m_strNotice[0] = "장시간의 게임은 건강을 해칠 수 있으므로 게임참여는 하루 5시간 이용하실 수 있습니다.\n";
	m_strNotice[1] = "공부와 일에 휴식이 필요하듯이 게임이용에도 휴식이 필요합니다.\n";

	m_bDisplay30MinWarnig = FALSE;
	MM().AddMsgH(UPG_CMK_RESET_WARNINGTIMER, Fnt(this, &CPlayTimeWarning::OnChangeWaringType));
}
LRESULT  CPlayTimeWarning::OnChangeWaringType(WPARAM &wParam, LPARAM &lParam)
{
	//경고 메시지 간격 변경시 초기화
	BOOL bDisplay30MinWarnig = m_bDisplay30MinWarnig;

	int nTimeWarning = (int)wParam;
	if (nTimeWarning == 1800)
		m_bDisplay30MinWarnig = TRUE;
	else
		m_bDisplay30MinWarnig = FALSE;

	if ( bDisplay30MinWarnig != m_bDisplay30MinWarnig )
		m_nTotalMin = 0;

	return TRUE;
}

CPlayTimeWarning::~CPlayTimeWarning()
{
	MM().RemoveMsgH(UPG_CMK_RESET_WARNINGTIMER);
}


void CPlayTimeWarning::Login()
{
	m_dwLoginTime = timeGetTime();
	m_nLastPrintTime = 0;
	m_nLastPrintTimeMin = 0;

	m_nTotalMin = 0;
}

void CPlayTimeWarning::EnterTheGame()
{
	if ( m_nFirstPrint == FALSE )
	{
		m_nFirstPrint = TRUE;

		if ( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME )
		{
			/*AddGameInfoViewEdit( m_strNotice[rand()%2].GetString(), RGB(143,141,102));*/
			AddGameInfoViewEdit( m_strNotice[0].GetString(), RGB(143,141,102));
		}
	}

	if (m_bShowTimeMessageAtEnterRoom == true)
	{
		m_bShowTimeMessageAtEnterRoom = false;

		PrintPerHour(m_nHourToShow, m_nMinToShow);	
	}
}

void CPlayTimeWarning::OnTimer()
{
	//타이머 1분 단위로 호출

	m_nTotalMin++;
	int nHour = m_nTotalMin / 60;
	int nMin = m_nTotalMin % 60;

	if ( GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY)
	{
		if ( m_nTotalMin % 60 == 0)
		{
			m_nHourToShow = nHour;
			m_nMinToShow = nMin;

			m_bShowTimeMessageAtEnterRoom = true;
		}
		else if ( m_bDisplay30MinWarnig )
		{
			if ( ( m_nTotalMin % 60 ) == 30 )
			{
				m_nHourToShow = nHour;
				m_nMinToShow = nMin;

				m_bShowTimeMessageAtEnterRoom = true;
			}
		}

		return;
	}
	else
	{
		if ( ( m_nTotalMin % 60 ) == 0 )
		{
			PrintPerHour( nHour );
		}
		else if ( m_bDisplay30MinWarnig )
		{
			if ( ( m_nTotalMin % 60 ) == 30 )
			{
				PrintPerHour( nHour, nMin );
			}
	// 		else
	// 		{
	// 			PrintPerHour( nHour, nMin );
	// 		}
		}
	}

// 	DWORD dwHour = timeGetTime();
// 
// 	int nHour = (int)(dwHour - m_dwLoginTime) / (1000*60*60);
// 	int nMin = (int)(dwHour - m_dwLoginTime) / (1000*60);
// 	nMin = nMin % 60;
// 
// 	if (m_bDisplay30MinWarnig)
// 	{			
// 		PrintPerHour( m_nLastPrintTime, nMin );
// 	}
// 	else
// 	{
// 		if ( nHour != m_nLastPrintTime )
// 		{
// 			m_nLastPrintTime = nHour;
// 			PrintPerHour( m_nLastPrintTime );
// 		}
// 	}
}

void CPlayTimeWarning::PrintPerHour( int nHour, int nMin )
{
	CString str;

	if ( nMin > 0 )
	{
		if ( nHour > 0 )
		{
			str.Format( "게임 접속 후 %d시간 %d분이 경과하였습니다.\n", nHour, nMin );
//			str = str + "과도한 게임이용은 정상적인 일상생활에 지장을 줄 수 있습니다.\n";
		}
		else
		{
			str.Format( "게임 접속 후 %d분이 경과하였습니다.\n", nMin );
		}			
	}
	else if ( nHour > 0 )
	{
		str.Format( "게임 접속 후 %d시간이 경과하였습니다.\n", nHour );
		str = str + "과도한 게임이용은 정상적인 일상생활에 지장을 줄 수 있습니다.\n";
	}
	else return;

/*
	if ( nHour >= 3 )
	{
		str = str + " 잠시 쉬었다 하시기 바랍니다.\n";
	}
	else
	{
		str = str + "\n";
	}
*/


	if ( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME )
	{
		AddGameInfoViewEdit( str.GetString(),  RGB(143,141,102) );
	}
//	else if ( GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY )
//	{
//		AddLobbyChatViewEdit( str.GetString(),  RGB(115,0,0) );
//	}
}
