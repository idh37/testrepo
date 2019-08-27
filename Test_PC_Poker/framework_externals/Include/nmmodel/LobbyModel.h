/**    
@file    LobbyModel.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Lobby 데이터 처리
*/
#pragma once
#include "ModelDefine.h"

namespace NMBASE
{
	namespace MODEL
	{

		class NMMODEL_CLASS CLobbyModel : public IComponent
		{
		public:
			CLobbyModel();
			virtual ~CLobbyModel();

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

		protected:
			virtual LRESULT	 SendRoomListRequest(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< 룸리스트 요청
			virtual LRESULT	 SendUserListRequest(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< 유저리스트 요청
			virtual LRESULT	 SendLogOutRequest(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< 로그아웃 요청

			virtual LRESULT	 GameServerPingCheck(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< 게임서버 핑 체크

			virtual LRESULT	 AllUserInfo(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< 모든 유저 정보
			virtual LRESULT	 AllRoomInfo(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< 모든 방 정보

			virtual LRESULT	 Logout(WPARAM& wParam, LPARAM& lParam){return TRUE;}				///< 로그 아웃

		private:

		};

	}//namespace MODEL
}//namespace NMBASE
