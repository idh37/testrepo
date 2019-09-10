/**    
@file    SockMan.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Socket ������ ( SingleTon ���� ��� )
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
		@brief	���� �޴���(Singleton)
		*/
		class NMSOCKCAMEL_CLASS CSockCamelMan
		{
		public:
			static	CSockCamelMan& Get();

		public:
			BOOL Init(HWND hwnd);		///< Ŭ���� �ʱ�ȭ
			
			BOOL ConnectSocket(char *strIP, UINT nPortNum, UINT nThreadCount = 2);		///< Ŭ���̾�Ʈ������ ������ ������ ����(������ Ŭ���� �ڵ� ��ȯ, ���н� 0���� �������� ����)
			void Disconnect();

			BOOL Reconnect();

			BOOL Send(Camel::Dar::IDarMsg& rMessage);

			LPCTSTR GetIP(){return m_strSrvIP.GetBuffer(0);}
			UINT    GetPORT(){return m_nPort;}

			enum Base::Connector::ConnectorState GetState();

			HWND GetNetworkHandle(){return m_hWindow;}						///< SendMessage���� ������ �ڵ�

		private:
			CSockCamelMan();
			virtual ~CSockCamelMan();

		protected:

			CString m_strSrvIP ;				///< IP
			UINT    m_nPort;					///< PORT
			//CString m_strError;				///< ���� ��¿� ��Ʈ��

			HWND m_hWindow;						///< PostMessage���� ������ �ڵ�(SockCmndŬ������ ���ǵǾ� ����) 


		private:
			CString m_strServerName;								///< ���ο����� �����.

			/// ���� �ʱ�ȭ
			Camel::Base::System::ThreadPool::Handle m_hThreadPool;// = Camel::Base::System::ThreadPool::CreateHandle(2);
			Camel::Dar::DarDispatcher<unsigned int> m_oDispatcher;
			Base::Connector m_oConnector;
		};


		NMSOCKCAMEL_API CSockCamelMan& GSOCKCAMELMAN();

	} //namespace SOCK
}  // namespace NMBASE