/**    
@file    ChannelController.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   채널 이벤트 처리
*/
#pragma once
#include "ControllerDefine.h"


namespace NMBASE
{
	namespace CONTROLLER
	{

		class NMCONTROLLER_CLASS CChannelController : public IComponent
		{
		public:
			CChannelController();
			virtual ~CChannelController();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

		protected:

			virtual LRESULT	 ReceiveChannelInfo(WPARAM& wParam, LPARAM& lParam);	///< 채널 정보

		private:

		};

	}//namespace CONTROLLER
}//namespace NMBASE