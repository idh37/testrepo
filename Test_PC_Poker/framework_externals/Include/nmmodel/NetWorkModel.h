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

		//class CSockCmnd;
		/**      
		@brief	���� �޴���(Singleton)
		*/
		class NMMODEL_CLASS CNetWorkModel
		{
		public:
			static	CNetWorkModel& Get();

		public:
			void Init(){}											///< ��ü�� �����ϱ� ���� �ǹ̾��� �Լ�	

			LPCTSTR ConnectSocket(char *strIP, UINT nPortNum);		///< Ŭ���̾�Ʈ������ ������ ������ ����(������ SocketName ����)

			int ReceiveData(char* szSockName);						///< �������κ��� �����͸� �޴´�

			BOOL SendData(char* szSockName, void *pdata, int size);

			BOOL PostCloseMsg(char* szSockName);

			BOOL CloseDataSocket(const char* szSockName);			///< ���� Ŭ����ID�� ���� ���� - Overrided

			void Destroy();

			char* GetHostIP();
			char* GetHostDomain();
			char* GetHostPortStr();

			LPCTSTR GetServerIP(char* szSockName);					///< ������ ������ IP���ڿ��� ��´�.
			UINT GetServerPort(char* szSockName);

			virtual LRESULT	 SocketInit(WPARAM& wParam, LPARAM& lParam);		///< ���� �ʱ�ȭ

		private:
			CNetWorkModel();
			virtual ~CNetWorkModel();

		private:
			//CSockCmnd*	m_pSockCmnd;	// ���� �޼��� ó�� Ŭ����
		};

		NMMODEL_API CNetWorkModel& GNET();


	}//namespace MODEL
}//namespace NMBASE
