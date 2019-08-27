#include "stdafx.h"
#include "LogInController.h"

namespace NMBASE
{
	namespace CONTROLLER
	{

		CLogInCtroller::CLogInCtroller()
		{

		}

		CLogInCtroller::~CLogInCtroller()
		{

		}

		BOOL CLogInCtroller::OnAttached()
		{
			/*///< 마스터서버 패킷
			UTIL::MM().AddNetMsgH(NET_MASTER_RCV_CHECKVERSION,	Fnt(this, &CLogInCtroller::Master_CheckVersion));	
			UTIL::MM().AddNetMsgH(NET_MASTER_RCV_DESTSERVERINFO,	Fnt(this, &CLogInCtroller::Master_DestServerInfo));	

			///< 게임서버 패킷
			UTIL::MM().AddNetMsgH(NET_GAME_RCV_CHECKVERSION,		Fnt(this, &CLogInCtroller::Game_CheckVersion));
			UTIL::MM().AddNetMsgH(NET_GAME_RCV_SERVERINFO,		Fnt(this, &CLogInCtroller::Game_ServerInfo));
			UTIL::MM().AddNetMsgH(NET_GAME_RCV_ACCEPTLOGIN,		Fnt(this, &CLogInCtroller::Game_AcceptLogIn));
			UTIL::MM().AddNetMsgH(NET_GAME_RCV_REFUSE_LOGIN,		Fnt(this, &CLogInCtroller::Game_RefuseLogIn));*/

			return TRUE;
		}

		BOOL CLogInCtroller::OnDetached()
		{
			///< Delete
			delete this;

			return TRUE;
		}


		LRESULT CLogInCtroller::Master_CheckVersion(WPARAM& wParam, LPARAM& lParam)
		{
// 			SOCK::SGAMENOTIFY *pNotifyData = (SOCK::SGAMENOTIFY*)lParam;
// 			if(!pNotifyData) 
// 			{
// 				return FALSE;
// 			}
// 
// 			///< 소켓을 비교해 본다.
// 			if(!strcmp(MASTERSOCK, pNotifyData->szSockName))
// 			{		
// 				return UTIL::MM().NetCall(MSG_MASTER_CHECKVERSION, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}	

			return FALSE;
		}


		LRESULT CLogInCtroller::Master_DestServerInfo(WPARAM& wParam, LPARAM& lParam)
 		{
// 			SOCK::SGAMENOTIFY *pNotifyData = (SOCK::SGAMENOTIFY*)lParam;
// 			if(!pNotifyData) 
// 			{
// 				return FALSE;
// 			}
// 
// 			///< 소켓을 비교해 본다.
// 			if(!strcmp(MASTERSOCK, pNotifyData->szSockName))
// 			{		
// 				return UTIL::MM().NetCall(MSG_MASTER_DESTSERVERINFO, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}	

			return TRUE;
		}


		LRESULT CLogInCtroller::Game_CheckVersion(WPARAM& wParam, LPARAM& lParam)
		{
// 			SOCK::SGAMENOTIFY *pNotifyData = (SOCK::SGAMENOTIFY*)lParam;
// 			if(!pNotifyData) 
// 			{
// 				return FALSE;
// 			}
// 
// 			///< 소켓을 비교해 본다.
// 			if(!strcmp(GAMESOCK, pNotifyData->szSockName))
// 			{		
// 				return UTIL::MM().NetCall(MSG_GAME_CHECKVERSION, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}	

			return FALSE;
		}


		LRESULT CLogInCtroller::Game_ServerInfo(WPARAM& wParam, LPARAM& lParam)
		{
// 			SOCK::SGAMENOTIFY *pNotifyData = (SOCK::SGAMENOTIFY*)lParam;
// 			if(!pNotifyData) 
// 			{
// 				return FALSE;
// 			}
// 
// 			///< 소켓을 비교해 본다.
// 			if(!strcmp(GAMESOCK, pNotifyData->szSockName))
// 			{		
// 				return UTIL::MM().NetCall(MSG_GAME_SERVERINFO, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}

			return FALSE;
		}

		LRESULT CLogInCtroller::Game_AcceptLogIn(WPARAM& wParam, LPARAM& lParam)
		{
// 			SOCK::SGAMENOTIFY *pNotifyData = (SOCK::SGAMENOTIFY*)lParam;
// 			if(!pNotifyData) 
// 			{
// 				return FALSE;
// 			}
// 
// 			///< 소켓을 비교해 본다.
// 			if(!strcmp(GAMESOCK, pNotifyData->szSockName))
// 			{		
// 				return UTIL::MM().NetCall(MSG_GAME_ACCEPTLOGIN, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}

			return FALSE;
		}


		LRESULT CLogInCtroller::Game_RefuseLogIn(WPARAM& wParam, LPARAM& lParam)
		{
// 			SOCK::SGAMENOTIFY *pNotifyData = (SOCK::SGAMENOTIFY*)lParam;
// 			if(!pNotifyData) 
// 			{
// 				return FALSE;
// 			}
// 
// 			///< 소켓을 비교해 본다.
// 			if(!strcmp(GAMESOCK, pNotifyData->szSockName))
// 			{		
// 				return UTIL::MM().NetCall(MSG_GAME_REFUSELOGIN, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}

			return FALSE;
		}

	}//namespace CONTROLLER
}//namespace NMBASE