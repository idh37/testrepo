/**    
@file    SockMan.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Socket 관리자 ( SingleTon 패턴 사용 )
*/
#pragma once

#include "SockCamelDefine.h"
#include "Connector.h"

#include <map>
#include <string>

namespace NMBASE
{
	namespace SOCKCAMEL
	{
		/**      
		@brief	소켓 메니저(Singleton)
		*/
		class NMSOCKCAMEL_CLASS CSockCamelMan
		{
		public:
			static	CSockCamelMan& Get();

		public:
			BOOL Init(HWND hwnd);		///< 클래스 초기화
			
			BOOL ConnectSocket(char *strIP, UINT nPortNum, UINT nThreadCount = 2);		///< 클라이언트용으로 데이터 소켓을 생성(성공시 클래스 핸들 반환, 실패시 0보다 작은값을 리턴)
			void Disconnect();

			BOOL Reconnect();

			BOOL Send(Camel::Dar::IDarMsg& rMessage);

			LPCTSTR GetIP(){return m_strSrvIP.GetBuffer(0);}
			UINT    GetPORT(){return m_nPort;}

			enum Base::Connector::ConnectorState GetState();

			HWND GetNetworkHandle(){return m_hWindow;}						///< SendMessage보낼 윈도우 핸들

		private:
			CSockCamelMan();
			virtual ~CSockCamelMan();

		protected:

			CString m_strSrvIP ;				///< IP
			UINT    m_nPort;					///< PORT
			//CString m_strError;				///< 에러 출력용 스트링

			HWND m_hWindow;						///< PostMessage보낼 윈도우 핸들(SockCmnd클래스에 정의되어 있음) 


		private:
			CString m_strServerName;								///< 내부에서만 사용함.

			/// 소켓 초기화
			Camel::Base::System::ThreadPool::Handle m_hThreadPool;// = Camel::Base::System::ThreadPool::CreateHandle(2);
			Camel::Dar::DarDispatcher<unsigned int> m_oDispatcher;
			Base::Connector m_oConnector;
		};


		NMSOCKCAMEL_API CSockCamelMan& GSOCKCAMELMAN();

	} //namespace SOCK
}  // namespace NMBASE