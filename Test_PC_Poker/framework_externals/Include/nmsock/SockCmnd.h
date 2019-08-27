/**    
@file    SockCmnd.
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   네트웍 메시지를 Controller에게 보내준다.
*/
#pragma once

#include "SockDefine.h"
#include "SockMan.h"
#include "CommMessage.h"

namespace NMBASE
{
	namespace SOCK
	{
		/**      
		@brief	소켓 Command 클래스 (CWnd 상속)
		*/
		class NMSOCK_CLASS CSockCmnd : public CWnd
		{
		public:
			CSockCmnd();
			virtual ~CSockCmnd();

		public:
			BOOL Init(CWnd *parent, CSockMan *pSockMan);

		protected:

			CWnd *m_pParentWnd;			///< 메세지를 보낼 부모윈도우
			CString m_strMainServerIP;	///< 메인서버의 IP
			CSockMan *m_pSockMan;		///< 소켓 매니저 포인터

		protected:	
			afx_msg LONG OnDataReceive(WPARAM, LONG);
			afx_msg LONG OnDataClose(WPARAM, LONG);
			afx_msg LONG OnDataConnect(WPARAM, LONG);
			afx_msg LONG OnDataConnectFailure(WPARAM, LONG);

			DECLARE_MESSAGE_MAP()
		};

	} //namespace SOCK

} // namespace NMBASE

