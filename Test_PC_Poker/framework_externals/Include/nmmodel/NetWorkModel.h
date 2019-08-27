/**    
@file    NetWorkModel.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Model Socket 관리자 ( SingleTon 패턴 사용 )
*/
#pragma once
#include "ModelDefine.h"

namespace NMBASE
{
	namespace MODEL
	{

		//class CSockCmnd;
		/**      
		@brief	소켓 메니저(Singleton)
		*/
		class NMMODEL_CLASS CNetWorkModel
		{
		public:
			static	CNetWorkModel& Get();

		public:
			void Init(){}											///< 객체를 생성하기 위한 의미없는 함수	

			LPCTSTR ConnectSocket(char *strIP, UINT nPortNum);		///< 클라이언트용으로 데이터 소켓을 생성(성공시 SocketName 리턴)

			int ReceiveData(char* szSockName);						///< 소켓으로부터 데이터를 받는다

			BOOL SendData(char* szSockName, void *pdata, int size);

			BOOL PostCloseMsg(char* szSockName);

			BOOL CloseDataSocket(const char* szSockName);			///< 소켓 클래스ID로 소켓 삭제 - Overrided

			void Destroy();

			char* GetHostIP();
			char* GetHostDomain();
			char* GetHostPortStr();

			LPCTSTR GetServerIP(char* szSockName);					///< 접속한 서버의 IP문자열을 얻는다.
			UINT GetServerPort(char* szSockName);

			virtual LRESULT	 SocketInit(WPARAM& wParam, LPARAM& lParam);		///< 소켓 초기화

		private:
			CNetWorkModel();
			virtual ~CNetWorkModel();

		private:
			//CSockCmnd*	m_pSockCmnd;	// 소켓 메세지 처리 클래스
		};

		NMMODEL_API CNetWorkModel& GNET();


	}//namespace MODEL
}//namespace NMBASE
