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

		/**      
		@brief	소켓 메니저(Singleton)
		*/
		class NMMODEL_CLASS CNetWorkCamelModel
		{
		public:
			static	CNetWorkCamelModel& Get();

		public:
			void Init(){}											///< 객체를 생성하기 위한 의미없는 함수	

			BOOL ConnectSocket(char *strIP, UINT nPortNum);			///< 클라이언트용으로 데이터 소켓을 생성

			void Disconnect();

			BOOL Reconnect();

			BOOL SendData(void* rMessage);		// Camel::Dar::IDarMsg*

			void Destroy();


			LPCTSTR GetServerIP();									///< 접속한 서버의 IP문자열을 얻는다.
			UINT GetServerPort();

			virtual LRESULT	 SocketInit(WPARAM& wParam, LPARAM& lParam);		///< 소켓 초기화

		private:
			CNetWorkCamelModel();
			virtual ~CNetWorkCamelModel();		
		};

		NMMODEL_API CNetWorkCamelModel& GCAMELNET();

	}//namespace MODEL
}//namespace NMBASE
