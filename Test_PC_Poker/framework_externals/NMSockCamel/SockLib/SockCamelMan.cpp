#include "stdafx.h"
#include "SockCamelMan.h"
#include "CommonUserWndMsg.h"


namespace NMBASE
{
	namespace SOCKCAMEL
	{

		/*
		#define UM_SOCKETCAMEL_NOTIFY			(WM_USER+510)	
		#define UM_SOCKETCAMEL_RECEIVE			(WM_USER+511)
		#define UM_SOCKETCAMEL_CLOSE			(WM_USER+512)
		#define UM_SOCKETCAMEL_CONNECT_FAIL		(WM_USER+513)
		#define UM_SOCKETCAMEL_CONNECT_SUCCESS	(WM_USER+514)
		#define UM_SOCKETCAMEL_RECEIVE_ERROR	(WM_USER+515)
		*/

		bool __stdcall OnRecieved( void* pData, Camel::Dar::PacketHeader& rHeader, Camel::Base::System::Stream::Handle hStream, int nPacketSize)
		{
			// ���� ��Ŷ ���� �޽��� �غ�
			SCAMELNOTIFY sGameNotiData;			
			
			sGameNotiData.pData			= pData;
			sGameNotiData.rHeader		= (void*)&rHeader;		// Camel::Dar::PacketHeader
			sGameNotiData.hStream		= (void*)hStream;		// Camel::Base::System::Stream::Handle
			sGameNotiData.nPacketSize	= nPacketSize;
		
			// �ϼ��� ��Ŷ�� �����Ͽ����� ���� �信 �˸�
			// �޽���:UM_SOCKET_NOTIFY(����),  Command : UM_SOCKET_CONNECT_FAIL �� ó����
			::SendMessage(GSOCKCAMELMAN().GetNetworkHandle(), UM_SOCKETCAMEL_NOTIFY, (WPARAM)UM_SOCKETCAMEL_RECEIVE, (LPARAM)&sGameNotiData);						

			return true;
		}

		bool __stdcall OnRecievedError( void* pData, Camel::Base::System::Stream::Handle hStream)
		{
			SCAMELERROR sCamelError;

			sCamelError.pData		= pData;
			sCamelError.hStream		= (void*)hStream;	// Camel::Base::System::Stream::Handle
			sCamelError.nErrorCode	= Camel::Base::System::Runtime::GetLastError();

			::SendMessage(GSOCKCAMELMAN().GetNetworkHandle(), UM_SOCKETCAMEL_NOTIFY, (WPARAM)UM_SOCKETCAMEL_RECEIVE_ERROR, (LPARAM)&sCamelError);						

			return true;
		}


		bool __stdcall OnDisconnected( void* pData)
		{
 			printf("Connection Disconnected Fail...\n");
 			
			Base::Connector* pConnector = reinterpret_cast<Base::Connector* >(pData);
 			if (NULL != pConnector)
 			{
				// �ٽ��ѹ� ���� �õ��Ѵ�.
 				if(true == pConnector->Reconnect())
 				{
 					return true;
 				}
 			}

			SCAMELERROR sCamelError;

			sCamelError.pData		= pData;
			sCamelError.hStream		= NULL;
			sCamelError.nErrorCode	= Camel::Base::System::Runtime::GetLastError();

			// Disconnection �߻��ϸ� �޽����� �����ش�.
			::SendMessage(GSOCKCAMELMAN().GetNetworkHandle(), UM_SOCKETCAMEL_NOTIFY, (WPARAM)UM_SOCKETCAMEL_CONNECT_FAIL, (LPARAM)&sCamelError);						

			return true;
		}

		CSockCamelMan& GSOCKCAMELMAN()
		{
			return CSockCamelMan::Get();
		}

		CSockCamelMan& CSockCamelMan::Get()
		{
			static CSockCamelMan sSocketCamelManager;
			return sSocketCamelManager;
		}

		CSockCamelMan::CSockCamelMan()
		{
			//AfxSocketInit();	///< ����DLL �ʱ�ȭ(MFC������ �ڵ����� �������� API�󿡼��� ������ �𸣹Ƿ�)
// 			if(!AfxSocketInit())
// 			{
// 				AfxMessageBox("Windows ���� �ʱ�ȭ�� �����Ͽ����ϴ�.");		
// 			}

			m_hThreadPool = NULL;

		}

		CSockCamelMan::~CSockCamelMan()
		{
			//WSACleanup();		///< ����DLL Ŭ����
			//Destroy();
		}


		BOOL CSockCamelMan::Init(HWND hwnd)
		{
			//Destroy();

			m_hWindow = hwnd;	///< PostMessage���� ������ �ڵ� 

			return TRUE;			
			
			//CheckHostInfo();	///< Host ������ üũ�Ѵ�
		}

		///< Ŭ���̾�Ʈ������ ������ ������ ����, ���ϴ� �ּҿ� �����Ų��
		///< -1�� �����ϸ� ���� ���и� �������� ���� ����� ���߿� �޼����� �뺸�ȴ�
		///< (���� ���� ���δ� UM_DATA_CONNECT Ȥ�� UM_DATA_CONNECT_FAILURE �޼����� �� �� �ִ�)
		BOOL CSockCamelMan::ConnectSocket(char *strIP, UINT nPortNum, UINT nThreadCount)
		{
			m_strSrvIP = strIP;			
			m_nPort = nPortNum;

			// Default Thread Count : 2
			m_hThreadPool = Camel::Base::System::ThreadPool::CreateHandle(nThreadCount);

			if(!m_hThreadPool)
			{
				return FALSE;
			}

			// Connect�� �ʱ�ȭ �Ѵ�.
			//if(false == m_oConnector.Initialize("127.0.0.1", 10090, hThreadPool))
			if(false == m_oConnector.Initialize(strIP, nPortNum, m_hThreadPool))
			{
				printf("Initialize Error.[LastError:%x]\n", Camel::Base::System::Runtime::GetLastError());
				return FALSE;
			}

			// Function Pointer�� �����Ѵ�.
			m_oConnector.SetOnReceived(OnRecieved, &m_oDispatcher);
			m_oConnector.SetOnReceivedError(OnRecievedError, &m_oDispatcher);
			m_oConnector.SetOnDisconnected(OnDisconnected, &m_oDispatcher);


			if(false == m_oConnector.Connect())
			{
				printf("Connect Error.[LastError:%x]\n", Camel::Base::System::Runtime::GetLastError());
				return FALSE;
			}

			return TRUE;		
		}

		BOOL CSockCamelMan::Reconnect()
		{
			return (BOOL)m_oConnector.Reconnect();
		}

		BOOL CSockCamelMan::Send(Camel::Dar::IDarMsg& rMessage)
		{
			return (BOOL)m_oConnector.Send(rMessage);
		}

		enum Base::Connector::ConnectorState CSockCamelMan::GetState()
		{
			return m_oConnector.GetState();
		}

		void CSockCamelMan::Disconnect()
		{
			m_oConnector.Disconnect();
		}

	} //namespace SOCK

} // namespace NMBASE

