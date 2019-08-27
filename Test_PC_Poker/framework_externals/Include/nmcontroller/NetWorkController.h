/**    
@file    NetWorkController.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Sock DLL에서 보내준 패킷을 처리한다.
*/
#pragma once
#include "ControllerDefine.h"

namespace NMBASE
{
	namespace CONTROLLER
	{

		class NMCONTROLLER_CLASS CNetWorkController : public IComponent
		{
		public:
			CNetWorkController();
			virtual ~CNetWorkController();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

		protected:
			bool CreateSockMsgWnd();

			virtual BOOL OnSocketReceive ( LPARAM lParam );
			virtual BOOL OnSocketClose	 ( LPARAM lParam );
			virtual BOOL OnSuccessConnect( LPARAM lParam );
			virtual BOOL OnFailConnect   ( LPARAM lParam );

			//virtual int OnSocketConnect( int nErrorCode = 0 );
			//virtual int OnSocketSend	 ( int nErrorCode = 0 );

			friend LRESULT CALLBACK SockMsgWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		private:
			HWND		m_hSockMsgWnd;
		};

	}//namespace CONTROLLER
}//namespace NMBASE