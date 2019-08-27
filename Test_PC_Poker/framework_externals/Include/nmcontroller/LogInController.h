/**    
@file    LogInController.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   LogIn 이벤트 처리
*/
#pragma once
#include "ControllerDefine.h"

namespace NMBASE
{
	namespace CONTROLLER
	{

		class NMCONTROLLER_CLASS CLogInCtroller : public IComponent
		{
		public:
			CLogInCtroller();
			virtual ~CLogInCtroller();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

		protected:

			virtual LRESULT	 Master_CheckVersion(WPARAM& wParam, LPARAM& lParam);			///< 마스트서버 버전 체크	
			virtual LRESULT	 Master_DestServerInfo(WPARAM& wParam, LPARAM& lParam);			///< 마스트서버 접속할 게임서버 정보

			virtual LRESULT	 Game_CheckVersion(WPARAM& wParam, LPARAM& lParam);				///< 게임서버 버전 체크
			virtual LRESULT	 Game_ServerInfo(WPARAM& wParam, LPARAM& lParam);				///< 게임서버 서버 정보
			virtual LRESULT	 Game_AcceptLogIn(WPARAM& wParam, LPARAM& lParam);				///< 게임서버 로그인 허용
			virtual LRESULT	 Game_RefuseLogIn(WPARAM& wParam, LPARAM& lParam);				///< 게임서버 로그인 거부	

		private:

		};

	}//namespace CONTROLLER
}//namespace NMBASE
