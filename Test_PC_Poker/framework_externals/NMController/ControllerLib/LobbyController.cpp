#include "stdafx.h"
#include "LobbyController.h"

namespace NMBASE
{
	namespace CONTROLLER
	{

		CLobbyController::CLobbyController()
		{
		}

		CLobbyController::~CLobbyController()
		{
		}


		BOOL CLobbyController::OnAttached()
		{
			/*UTIL::MM().AddNetMsgH(NET_GAME_RCV_ALLUSERINFO,			Fnt(this, &CLobbyController::ReceiveAllUserInfo));	
			UTIL::MM().AddNetMsgH(NET_GAME_RCV_ALLROOMINFO,			Fnt(this, &CLobbyController::ReceiveAllRoomInfo));	
			UTIL::MM().AddNetMsgH(NET_GAME_RCV_HEARTBEATGHOSTCHECK,	Fnt(this, &CLobbyController::GameServerPingCheck));
			UTIL::MM().AddNetMsgH(NET_GAME_RCV_LOGOUT,				Fnt(this, &CLobbyController::GameServerLogOut));*/

			return TRUE;
		}

		BOOL CLobbyController::OnDetached()
		{
			///< Delete
			delete this;

			return TRUE;
		}


		LRESULT	 CLobbyController::ReceiveAllUserInfo(WPARAM& wParam, LPARAM& lParam)
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
// 				return UTIL::MM().NetCall(MSG_GAME_ALLUSERINFO, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}

			return FALSE;
		}

		LRESULT	 CLobbyController::ReceiveAllRoomInfo(WPARAM& wParam, LPARAM& lParam)
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
// 				return UTIL::MM().NetCall(MSG_GAME_ALLROOMINFO, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}

			return FALSE;
		}


		LRESULT	 CLobbyController::GameServerPingCheck(WPARAM& wParam, LPARAM& lParam)
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
// 				return UTIL::MM().NetCall(MSG_GAME_PINGCHECK, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}	

			return FALSE;
		}


		LRESULT CLobbyController::GameServerLogOut(WPARAM& wParam, LPARAM& lParam)	
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
// 				return UTIL::MM().NetCall(MSG_GAME_LOGOUT, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}	

			return FALSE;
		}

	}//namespace CONTROLLER
}//namespace NMBASE