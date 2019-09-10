/**    
@file    LobbyModel.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Lobby ������ ó��
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
			virtual LRESULT	 SendRoomListRequest(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< �븮��Ʈ ��û
			virtual LRESULT	 SendUserListRequest(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< ��������Ʈ ��û
			virtual LRESULT	 SendLogOutRequest(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< �α׾ƿ� ��û

			virtual LRESULT	 GameServerPingCheck(WPARAM& wParam, LPARAM& lParam){return TRUE;}	///< ���Ӽ��� �� üũ

			virtual LRESULT	 AllUserInfo(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< ��� ���� ����
			virtual LRESULT	 AllRoomInfo(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< ��� �� ����

			virtual LRESULT	 Logout(WPARAM& wParam, LPARAM& lParam){return TRUE;}				///< �α� �ƿ�

		private:

		};

	}//namespace MODEL
}//namespace NMBASE
