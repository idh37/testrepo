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
			// 게임 패킷 도착 메시지 준비
			SCAMELNOTIFY sGameNotiData;			
			
			sGameNotiData.pData			= pData;
			sGameNotiData.rHeader		= (void*)&rHeader;		// Camel::Dar::PacketHeader
			sGameNotiData.hStream		= (void*)hStream;		// Camel::Base::System::Stream::Handle
			sGameNotiData.nPacketSize	= nPacketSize;
		
			// 완성된 패킷이 도착하였음을 메인 뷰에 알림
			// 메시지:UM_SOCKET_NOTIFY(공통),  Command : UM_SOCKET_CONNECT_FAIL 로 처리함
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
				// 다시한번 접속 시도한다.
 				if(true == pConnector->Reconnect())
 				{
 					return true;
 				}
 			}

			SCAMELERROR sCamelError;

			sCamelError.pData		= pData;
			sCamelError.hStream		= NULL;
			sCamelError.nErrorCode	= Camel::Base::System::Runtime::GetLastError();

			// Disconnection 발생하면 메시지를 보내준다.
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
			//AfxSocketInit();	///< 윈속DLL 초기화(MFC에서는 자동으로 해주지만 API상에서도 쓰일지 모르므로)
// 			if(!AfxSocketInit())
// 			{
// 				AfxMessageBox("Windows 소켓 초기화에 실패하였습니다.");		
// 			}

			m_hThreadPool = NULL;

		}

		CSockCamelMan::~CSockCamelMan()
		{
			//WSACleanup();		///< 윈속DLL 클린업
			//Destroy();
		}


		BOOL CSockCamelMan::Init(HWND hwnd)
		{
			//Destroy();

			m_hWindow = hwnd;	///< PostMessage보낼 윈도우 핸들 

			return TRUE;			
			
			//CheckHostInfo();	///< Host 정보를 체크한다
		}

		///< 클라이언트용으로 데이터 소켓을 생성, 원하는 주소에 연결시킨다
		///< -1을 리턴하면 접속 실패를 뜻하지만 실제 결과는 나중에 메세지로 통보된다
		///< (접속 성패 여부는 UM_DATA_CONNECT 혹은 UM_DATA_CONNECT_FAILURE 메세지로 알 수 있다)
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

			// Connect를 초기화 한다.
			//if(false == m_oConnector.Initialize("127.0.0.1", 10090, hThreadPool))
			if(false == m_oConnector.Initialize(strIP, nPortNum, m_hThreadPool))
			{
				printf("Initialize Error.[LastError:%x]\n", Camel::Base::System::Runtime::GetLastError());
				return FALSE;
			}

			// Function Pointer를 세팅한다.
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

