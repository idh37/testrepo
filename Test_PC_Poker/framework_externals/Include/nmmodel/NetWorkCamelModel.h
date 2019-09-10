/**    
@file    NetWorkModel.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Model Socket ������ ( SingleTon ���� ��� )
*/
#pragma once
#include "ModelDefine.h"

namespace NMBASE
{
	namespace MODEL
	{

		/**      
		@brief	���� �޴���(Singleton)
		*/
		class NMMODEL_CLASS CNetWorkCamelModel
		{
		public:
			static	CNetWorkCamelModel& Get();

		public:
			void Init(){}											///< ��ü�� �����ϱ� ���� �ǹ̾��� �Լ�	

			BOOL ConnectSocket(char *strIP, UINT nPortNum);			///< Ŭ���̾�Ʈ������ ������ ������ ����

			void Disconnect();

			BOOL Reconnect();

			BOOL SendData(void* rMessage);		// Camel::Dar::IDarMsg*

			void Destroy();


			LPCTSTR GetServerIP();									///< ������ ������ IP���ڿ��� ��´�.
			UINT GetServerPort();

			virtual LRESULT	 SocketInit(WPARAM& wParam, LPARAM& lParam);		///< ���� �ʱ�ȭ

		private:
			CNetWorkCamelModel();
			virtual ~CNetWorkCamelModel();		
		};

		NMMODEL_API CNetWorkCamelModel& GCAMELNET();

	}//namespace MODEL
}//namespace NMBASE
