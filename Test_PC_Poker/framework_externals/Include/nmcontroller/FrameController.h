/**    
@file    FrameController.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   MainFrame �̺�Ʈ ó��
*/
#pragma once
#include "ControllerDefine.h"

namespace NMBASE
{
	namespace CONTROLLER
	{

		class NMCONTROLLER_CLASS CFrameController : public IComponent
		{
		public:
			CFrameController();
			virtual ~CFrameController();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

		protected:
			virtual LRESULT	 Evt_OnClickExitBtn(WPARAM& wParam, LPARAM& lParam);	///< �����ư Ŭ��
			virtual LRESULT	 Evt_OnClickAltF4(WPARAM& wParam, LPARAM& lParam);		///< Alt+F4 Ŭ��

		private:

		};

	}//namespace CONTROLLER
}//namespace NMBASE