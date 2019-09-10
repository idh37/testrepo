/**    
@file    LogInModel.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   �α��� ������ ó��
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

			virtual LRESULT	 SendDestServerInfoRequest(WPARAM& wParam, LPARAM& lParam){return TRUE;}		///< �����ͼ��� �̵��� ���Ӽ������� ��û

			virtual LRESULT	 MasterServerCheckVersion(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< �����ͼ��� ����üũ
			virtual LRESULT	 DestinationServerInfo(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< �����ͼ��� �̵��� ���Ӽ��� ����

			virtual LRESULT	 GameServerCheckVersion(WPARAM& wParam, LPARAM& lParam){return TRUE;};			///< ���Ӽ��� ����üũ
			virtual LRESULT	 GameServerServerInfo(WPARAM& wParam, LPARAM& lParam) {return TRUE;}			///< ���Ӽ��� ����
			virtual LRESULT	 GameServerAcceptLogin(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< ���Ӽ��� �α��� ���
			virtual LRESULT	 GameServerRefuseLogin(WPARAM& wParam, LPARAM& lParam){return TRUE;}			///< ���Ӽ��� �α��� �ź�
			virtual LRESULT	 SendMacAddress(WPARAM& wParam, LPARAM& lParam){return TRUE;}					///< ���Ӽ��� ���ּ� ����
			virtual LRESULT	 SendLogIn(WPARAM& wParam, LPARAM& lParam){return TRUE;}						///< ���Ӽ��� �α��� ��û

		protected:	
			virtual BOOL	SetChangeGame(){return TRUE;}													///< ���� ��ȯ
	
		};

	}//namespace MODEL
}//namespace NMBASE
