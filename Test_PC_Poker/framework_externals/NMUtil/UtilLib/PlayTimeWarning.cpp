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

namespace NMBASE
{
	namespace UTIL
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CPlayTimeWarning::CPlayTimeWarning()
		{
			m_nFirstPrint = FALSE;
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
			if( m_nFirstPrint == FALSE )
			{
				m_nFirstPrint = TRUE;

				// 종환복구
				/*if( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME )
				{
				AddGameChatViewEdit( m_strNotice[rand()%2].GetString(), RGB(143,141,102));
				}*/
			}
		}

		void CPlayTimeWarning::OnTimer()
		{
			DWORD dwHour = timeGetTime();
			dwHour = (dwHour - m_dwLoginTime) / (1000*60*60);

			if( dwHour != (DWORD)m_nLastPrintTime )
			{
				m_nLastPrintTime = dwHour;
				PrintPerHour( m_nLastPrintTime );
			}
		}

		void CPlayTimeWarning::PrintPerHour( int nHour )
		{
			CString str;

			str.Format( "게임 시작 후 %d시간이 경과하였습니다.", nHour );

			if( nHour >= 3 )
			{
				str = str + " 잠시 쉬었다 하시기 바랍니다.\n";
			}
			else
			{
				str = str + "\n";
			}

			// 종환복구
			/*if( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME )
			{
			AddGameChatViewEdit( str.GetString(),  RGB(143,141,102) );
			}
			else if( GM().GetCurrentWhere() == IDX_GAMEWHERE_LOBY )
			{
			AddLobbyChatViewEdit( str.GetString(),  RGB(115,0,0) );
			}*/
		}

	}//namespace UTIL

}//namespace NMBASE