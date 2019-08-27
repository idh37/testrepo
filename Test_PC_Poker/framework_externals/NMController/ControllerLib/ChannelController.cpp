#include "stdafx.h"
#include "ChannelController.h"
//#include "PlatformLayerPacketDefine.h"
// #include "CommonDefine.h"				// 공통 Define
// #include "CommandDefine.h"

namespace NMBASE
{
	namespace CONTROLLER
	{

		CChannelController::CChannelController()
		{

		}


		CChannelController::~CChannelController()
		{

		}

		BOOL CChannelController::OnAttached()
		{
			//	UTIL::MM().AddNetMsgH(NET_GAME_RCV_CHANNELNFO,	Fnt(this, &CChannelController::ReceiveChannelInfo));	

			return TRUE;
		}

		BOOL CChannelController::OnDetached()
		{
			///< Delete
			delete this;

			return TRUE;
		}


		LRESULT CChannelController::ReceiveChannelInfo(WPARAM& wParam, LPARAM& lParam)
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
// 				return UTIL::MM().NetCall(MSG_GAME_ALLCHANNELINFO, (WPARAM&)pNotifyData->TotSize, (LPARAM&)pNotifyData);
// 			}	

			return FALSE;
		}

	}//namespace CONTROLLER

}//namespace NMBASE
