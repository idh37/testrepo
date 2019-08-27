#include "stdafx.h"
#include "FrameController.h"
#include "CommandDefine.h"

namespace NMBASE
{
	namespace CONTROLLER
	{

		CFrameController::CFrameController()
		{

		}


		CFrameController::~CFrameController()
		{

		}


		BOOL CFrameController::OnAttached()
		{
			UTIL::MM().AddMsgH(EVT_MAINFRAM_ONEXITBTN,	UTIL::Fnt(this, &CFrameController::Evt_OnClickExitBtn));	
			UTIL::MM().AddMsgH(EVT_MAINFRAM_ONALTF4,	UTIL::Fnt(this, &CFrameController::Evt_OnClickAltF4));	

			return TRUE;
		}

		BOOL CFrameController::OnDetached()
		{
			///< Delete
			delete this;

			return TRUE;
		}


		LRESULT CFrameController::Evt_OnClickExitBtn(WPARAM& wParam, LPARAM& lParam)
		{
			//UNREFERENCED_PARAMETER(param);
			return TRUE;
		}

 		LRESULT CFrameController::Evt_OnClickAltF4(WPARAM& wParam, LPARAM& lParam)
 		{

			// 다른 호출을 할곳이 있으면 호출을 한다.

			// MainFrame의 종료
 			UTIL::MM().Call( EVT_MAINFRAM_ONDESTROY, wParam, lParam);		

 			return TRUE;
 		}


	}//namespace CONTROLLER
}//namespace NMBASE