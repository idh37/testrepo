/**    
@file    LobbyController.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Lobby �̺�Ʈ ó��
*/
#pragma once
#include "ControllerDefine.h"

namespace NMBASE
{
	namespace CONTROLLER
	{

		class NMCONTROLLER_CLASS CLobbyController : public IComponent
		{
		public:
			CLobbyController();
			virtual ~CLobbyController();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

		protected:
			virtual LRESULT	 ReceiveAllUserInfo(WPARAM& wParam, LPARAM& lParam);	///< ������ ���
			virtual LRESULT	 ReceiveAllRoomInfo(WPARAM& wParam, LPARAM& lParam);	///< ����ü ���

			virtual LRESULT	 GameServerPingCheck(WPARAM& wParam, LPARAM& lParam);	

			virtual LRESULT	 GameServerLogOut(WPARAM& wParam, LPARAM& lParam);	

		private:

		};

	}//namespace CONTROLLER
}//namespace NMBASE