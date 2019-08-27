/**    
@file    LogInModel.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   로그인 데이터 처리
*/
#pragma once
#include "ModelDefine.h"

namespace NMBASE
{
	namespace MODEL
	{
		NMMODEL_API void URLEncode(char* output, char* input);

		class NMMODEL_CLASS CLogInModel : public IComponent
		{
		public:
			CLogInModel();
			virtual ~CLogInModel();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

			virtual LRESULT	 SendDestServerInfoRequest(WPARAM& wParam, LPARAM& lParam){return TRUE;}		///< 마스터서버 이동할 게임서버정보 요청

			virtual LRESULT	 MasterServerCheckVersion(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< 마스터서버 버젼체크
			virtual LRESULT	 DestinationServerInfo(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< 마스터서버 이동할 게임서버 정보

			virtual LRESULT	 GameServerCheckVersion(WPARAM& wParam, LPARAM& lParam){return TRUE;};			///< 게임서버 버젼체크
			virtual LRESULT	 GameServerServerInfo(WPARAM& wParam, LPARAM& lParam) {return TRUE;}			///< 게임서버 정보
			virtual LRESULT	 GameServerAcceptLogin(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< 게임서버 로그인 허용
			virtual LRESULT	 GameServerRefuseLogin(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< 게임서버 로그인 거부
			virtual LRESULT	 SendMacAddress(WPARAM& wParam, LPARAM& lParam){return TRUE;}					///< 게임서버 맥주소 보냄
			virtual LRESULT	 SendLogIn(WPARAM& wParam, LPARAM& lParam){return TRUE;}						///< 게임서버 로그인 요청

		protected:	
			virtual BOOL	SetChangeGame(){return TRUE;}													///< 게임 전환
	
		};

	}//namespace MODEL
}//namespace NMBASE
