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

			m_strNotice[0] = "��ð��� �����̿��� �ǰ��� ��ĥ �� �ֽ��ϴ�.\n";
			m_strNotice[1] = "���ο� �Ͽ� �޽��� �ʿ��ϵ��� �����̿뿡�� �޽��� �ʿ��մϴ�.\n";
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

				// ��ȯ����
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

			str.Format( "���� ���� �� %d�ð��� ����Ͽ����ϴ�.", nHour );

			if( nHour >= 3 )
			{
				str = str + " ��� ������ �Ͻñ� �ٶ��ϴ�.\n";
			}
			else
			{
				str = str + "\n";
			}

			// ��ȯ����
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