/**    
@file    LogInController.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   LogIn �̺�Ʈ ó��
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

			virtual LRESULT	 Master_CheckVersion(WPARAM& wParam, LPARAM& lParam);			///< ����Ʈ���� ���� üũ	
			virtual LRESULT	 Master_DestServerInfo(WPARAM& wParam, LPARAM& lParam);			///< ����Ʈ���� ������ ���Ӽ��� ����

			virtual LRESULT	 Game_CheckVersion(WPARAM& wParam, LPARAM& lParam);				///< ���Ӽ��� ���� üũ
			virtual LRESULT	 Game_ServerInfo(WPARAM& wParam, LPARAM& lParam);				///< ���Ӽ��� ���� ����
			virtual LRESULT	 Game_AcceptLogIn(WPARAM& wParam, LPARAM& lParam);				///< ���Ӽ��� �α��� ���
			virtual LRESULT	 Game_RefuseLogIn(WPARAM& wParam, LPARAM& lParam);				///< ���Ӽ��� �α��� �ź�	

		private:

		};

	}//namespace CONTROLLER
}//namespace NMBASE
