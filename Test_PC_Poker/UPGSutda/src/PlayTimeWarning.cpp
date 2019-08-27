// PlayTimeWarning.cpp: implementation of the CPlayTimeWarning class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "client.h"
#include "PlayTimeWarning.h"
#include "Global.h"
#include "mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define GAMECHATFUNC(x,y) g_ChatWinMan->m_pChatting->AddText(x,y)
#define LOBYCHATFUNC(x,y) g_pLobyDlg->m_ChatViewEdit.AddText(x,y)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayTimeWarning::CPlayTimeWarning()
{
	m_nFirstPrint = 0;
	m_dwLoginTime = 0;
	m_nLastPrintTime = 0;

	m_strNotice[0] = "장시간의 게임이용은 건강을 해칠 수 있습니다.\n";
	m_strNotice[1] = "공부와 일에 휴식이 필요하듯이 게임이용에도 휴식이 필요합니다.\n";
}

CPlayTimeWarning::~CPlayTimeWarning()
{

}


void CPlayTimeWarning::Login()
{
	m_dwLoginTime = timeGetTime();
	m_nLastPrintTime = 0;
}

void CPlayTimeWarning::EnterTheGame()
{
	if ( m_nFirstPrint == FALSE )
	{
		m_nFirstPrint = TRUE;

		if ( g_Where == IDX_GAMEWHERE_GAME )
		{
			GAMECHATFUNC( &m_strNotice[rand()%2], SERVERMSG_COLOR);
		}
	}
}

void CPlayTimeWarning::OnTimer()
{
	DWORD dwHour = timeGetTime();
	dwHour = (dwHour - m_dwLoginTime) / (1000*60*60);

	if ( dwHour != m_nLastPrintTime )
	{
		m_nLastPrintTime = dwHour;
		PrintPerHour( m_nLastPrintTime );
	}
}

void CPlayTimeWarning::PrintPerHour( int nHour )
{
	CString str;

	str.Format( "게임 시작 후 %d시간이 경과하였습니다.", nHour );

	if ( nHour >= 3 )
	{
		str = str + " 잠시 쉬었다 하시기 바랍니다.\n";
	}
	else
	{
		str = str + "\n";
	}

	if ( g_Where == IDX_GAMEWHERE_GAME )
	{
		GAMECHATFUNC( &str,  SERVERMSG_COLOR );
	}
	else if ( g_Where == IDX_GAMEWHERE_LOBY )
	{
		LOBYCHATFUNC( &str,  SERVERMSG_COLOR );
	}
}
