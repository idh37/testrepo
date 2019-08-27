#include "stdafx.h"
#include "NetWorkCamelController.h"
#include "CommonDefine.h"
#include "CommandDefine.h"
#include "CommonUserWndMsg.h"


#define CAMEL_USERDATA	(GWL_USERDATA+1)

namespace NMBASE
{
	namespace CONTROLLER
	{

		CNetWorkCamelController::CNetWorkCamelController()
		{

		}

		CNetWorkCamelController::~CNetWorkCamelController()
		{

		}

		BOOL CNetWorkCamelController::OnAttached()
		{
			// 소켓 메시지처리 윈도우
			CreateSockMsgWnd();

			return TRUE;
		}

		BOOL CNetWorkCamelController::OnDetached()
		{
			delete this;
			return TRUE;
		}


		bool CNetWorkCamelController::CreateSockMsgWnd()
		{
			WNDCLASS wcx; 

			wcx.style			= 0; 
			wcx.lpfnWndProc		= SockCamelMsgWndProc;
			wcx.cbClsExtra		= 0;
			wcx.cbWndExtra		= 4;
			wcx.hInstance		= AfxGetInstanceHandle();
			wcx.hIcon			= NULL;
			wcx.hCursor			= NULL;
			wcx.hbrBackground	= NULL;
			wcx.lpszMenuName	= NULL;
			wcx.lpszClassName	= "SockCamelMsgWnd";

			RegisterClass( &wcx ); 

			m_hSockMsgWnd = CreateWindow( "SockCamelMsgWnd", "", WS_POPUP, 0,0,0,0, NULL, NULL, AfxGetInstanceHandle(), NULL );

			_ASSERTE( m_hSockMsgWnd != NULL );

			SetWindowLong(m_hSockMsgWnd, CAMEL_USERDATA, (LONG)this);

			// SocketCamelLib 메시지윈도우 초기화			
			BOOL bResult = UTIL::MM().NetCall(MSG_SOCKETCAMELINIT, (WPARAM&)m_hSockMsgWnd);

			if(FALSE == bResult)
			{
				// 프로그램 종료		
				AfxMessageBox("소켓 초기화에 실패하였습니다");		
			}

			return (m_hSockMsgWnd != NULL);
		}


		BOOL CNetWorkCamelController::OnSocketReceive( LPARAM lParam )
		{
			// Camel::Dar::DarDispatcher<unsigned int>* pDispacher = reinterpret_cast<Camel::Dar::DarDispatcher<unsigned int>*>(pData);
			// if ( NULL == pDispacher)
			// {
			// 	return false;
			// }
			// if ( pDispacher->DoDispatch( rHeader, rHeader.GetPacketMsgId(), hStream, nPacketSize) == false)
			// {
			// 	printf("DoDispatch Fail...[MsgId:%d]\n", rHeader.GetPacketMsgId());
			// 	return false;
			// }

			SOCKCAMEL::SCAMELNOTIFY *pNotifyData = (SOCKCAMEL::SCAMELNOTIFY*)lParam;
			if(!pNotifyData) 
			{
				return FALSE;
			}

			void* strSockData							 = pNotifyData->pData;											///< 데이터
			Camel::Dar::PacketHeader* rHeader			 = (Camel::Dar::PacketHeader*)pNotifyData->rHeader;				///< 헤더
			Camel::Base::System::Stream::Handle* hStream = (Camel::Base::System::Stream::Handle*)pNotifyData->hStream;	///< 핸들
			int nPacketSize								 = pNotifyData->nPacketSize;									///< 패킷 사이즈

			// 시그널값을 문자열로 변경하여 패킷을 호출하도록 한다.
			CString strCommand;
			strCommand.Format("%d", rHeader->GetPacketMsgId());


#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CNetWorkCamelController::OnSocketReceive() - Command:%s", strCommand.GetBuffer());
			TTrace::Debug()->Send(szTemp, "nmcontroller");   // single byte string	
#endif	// TRACE_OUTPUT

			// 시그널의 숫자를 문자열로 변환한 후에 대입한 후 네트웍 메시지를 호출한다.	
			UTIL::MM().NetCall(strCommand.GetBuffer(), (WPARAM&)nPacketSize, (LPARAM&)pNotifyData);

			return TRUE;
		}


// 		BOOL CNetWorkCamelController::OnSocketClose( LPARAM lParam )
// 		{
// 			//LPSTR pszSockName = (LPSTR)lParam;
// 
// 			UTIL::MM().NetCall(MSG_SOCKETCAMEL_CLOSE, 0, 0);
// 
// 			//// 마스터 서버와의 접속이 끊어진 경우라면
// 			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
// 			//{
// 			//	// 종환복구
// 			//	//if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST) 
// 			//	{
// 			//		// 마스터 서버와의 접속이 끊어지면 서버소켓과 모든 데이터 소켓도 접속을 끊는다
// 			//		SOCKCAMEL::GSOCKCAMELMAN().Destroy();
// 
// 			//		// 종환복구
// 			//		/*if(!GM().IsNowLogout())
// 			//		{				
// 			//		ShowMainMessageDlg(false, "메인 서버와의 접속이 끊어졌습니다");
// 			//		}
// 			//		SetChangeWhere(IDX_GAMEWHERE_FIRST,false);			
// 			//		OnCancel();
// 			//		*/
// 			//	}
// 			//}
// 
// 			//// 게임서버와 접속이 끊어진 경우라면
// 			//if(!strcmp(NMBASE::DATA::GINFO()->GetGameServerName(), pszSockName))
// 			//{
// 			//	// 메인서버와의 접속이 끊어지면 서버소켓과 모든 데이터 소켓도 접속을 끊는다
// 			//	SOCKCAMEL::GSOCKCAMELMAN().Destroy();	
// 
// 			//	// 종환복구
// 			//	//// 로그인 중이 아니면 접속 끊김을 알린다
// 			//	//if(GetCurrentWhere() != IDX_GAMEWHERE_LOGIN && GetCurrentWhere() != IDX_GAMEWHERE_FIRST)
// 			//	//{
// 			//	//	if(!m_bAskLogout)
// 			//	//	{
// 			//	//		// 로그아웃 요청한 상태가 아니면 접속 끊김을 알림
// 			//	//		ShowMainMessageDlg(false,"게임 서버와의 접속이 끊어졌습니다");
// 
// 			//	//		SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
// 			//	//		//ChangeView(0);
// 			//	//	}
// 
// 			//	//	// 프로그램 종료
// 			//	//	GM().SetNowLogout(true);
// 			//	//	OnCancel();
// 			//	//}
// 			//	//else
// 			//	//{					
// 			//	//	m_dlgLoading.OnCancel();
// 			//	//}
// 			//}
// 
// 			return TRUE;
// 		}
// 
// 
// 		BOOL CNetWorkCamelController::OnSuccessConnect( LPARAM lParam )
// 		{
// 			//LPSTR pszSockName = (LPSTR)lParam;
// 
// 			UTIL::MM().NetCall(MSG_SOCKETCAMEL_SUCCESSCONNECT, 0, 0);
// 
// 			//CString str;
// 
// 			//// 마스터 서버
// 			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
// 			//{
// 
// 			//}
// 
// 			//// 게임 서버
// 			//if(!strcmp(NMBASE::DATA::GINFO()->GetGameServerName(), pszSockName))
// 			//{
// 
// 			//}
// 
// 
// 			return TRUE;
// 		}


		BOOL CNetWorkCamelController::OnFailConnect(LPARAM lParam)
		{		
			SOCKCAMEL::SCAMELERROR* pCamelError = (SOCKCAMEL::SCAMELERROR*)lParam;

// 			SCAMELERROR sCamelError;
// 			sCamelError.pData		= pData;
// 			sCamelError.hStream		= NULL;
// 			sCamelError.nErrorCode	= Camel::Base::System::Runtime::GetLastError();

			UTIL::MM().NetCall(MSG_SOCKETCAMEL_FAILCONNECT, 0, lParam);

			//CString str;

			//// 마스터 서버
			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
			//{
			//	// 종환복구
			//	// 마스터 서버와 접속이 실패한 경우라면
			//	// 		if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST)
			//	// 		{
			//	// 			SetChangeWhere(IDX_GAMEWHERE_FIRST, false);		
			//	// 			GM().SetWaitResponse(false);
			//	// 
			//	// 			m_dlgLoading.OnCancel();
			//	// 
			//	// 			ShowMainMessageDlg(false, "메인 서버와 접속이 실패하였습니다");	
			//	// 			OnCancel();
			//	// 
			//	// 		}
			//	// 
			//	// 		// 다른 게임 서버의 채널로 이동중 접속이 실패한 경우라면
			//	// 		if(GetCurrentWhere() == IDX_GAMEWHERE_LOBY && m_cLobby.IsMovingGame())
			//	// 		{
			//	// 			SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
			//	// 
			//	// 			GM().SetWaitResponse(false);		
			//	// 			ShowMainMessageDlg(false, "게임 이동에 실패하였습니다. 재접속해 주시기 바랍니다.");			
			//	// 
			//	// 			// 프로그램 종료
			//	// 			GM().SetNowLogout(true);
			//	// 			OnCancel();
			//	// 		}
			//}

			//// 게임 서버
			//if(!strcmp(NMBASE::DATA::GINFO()->GetGameServerName(), pszSockName))
			//{
			//	// 종환복구
			//	// 로그인하는중일때 메인서버와 접속이 실패한 경우라면
			//	// 		if(GetCurrentWhere()==IDX_GAMEWHERE_LOGIN)
			//	// 		{
			//	// 			SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
			//	// 			GM().SetWaitResponse(false);
			//	// 
			//	// 			m_dlgLoading.OnCancel();		
			//	// 			ShowMainMessageDlg(false, "게임 서버와 접속이 실패하였습니다");		
			//	// 			OnCancel();
			//	// 		}
			//}

			return TRUE;
		}


		BOOL CNetWorkCamelController::OnReceiveError( LPARAM lParam )
		{
			SOCKCAMEL::SCAMELERROR* pCamelError = (SOCKCAMEL::SCAMELERROR*)lParam;	

//			SCAMELERROR sCamelError;
// 			sCamelError.pData		= pData;
// 			sCamelError.hStream		= (void*)hStream;	// Camel::Base::System::Stream::Handle
// 			sCamelError.nErrorCode	= Camel::Base::System::Runtime::GetLastError();


			UTIL::MM().NetCall(MSG_SOCKETCAMEL_RECEIVEERROR, 0, lParam);			


			//printf("OnRecievedError...\n");
			//Base::Connector* pConnector = reinterpret_cast<Base::Connector* >(pData);
			//if (NULL != pConnector)
			//{
			//	// 어떻게 처리하지??
			//}	

			return TRUE;
		}

		LRESULT CALLBACK SockCamelMsgWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		{
			switch(uMsg)
			{
			case UM_SOCKETCAMEL_NOTIFY:
				{
					CNetWorkCamelController* pController = (CNetWorkCamelController*)(GetWindowLong(hwnd, CAMEL_USERDATA));
					_ASSERTE( pController != NULL );

					UINT nCommand = (UINT)wParam;

					switch(nCommand)
					{
					case UM_SOCKETCAMEL_RECEIVE:
						{
							pController->OnSocketReceive(lParam);				
						}
						break;

// 					case UM_SOCKETCAMEL_CLOSE:
// 						{					
// 							pController->OnSocketClose(lParam);				
// 						}
// 						break;
// 
// 					case UM_SOCKETCAMEL_CONNECT_SUCCESS:
// 						{
// 							pController->OnSuccessConnect(lParam);		
// 						}
// 						break;

					case UM_SOCKETCAMEL_CONNECT_FAIL:
						{
							pController->OnFailConnect(lParam);					
						}
						break;		

					case UM_SOCKETCAMEL_RECEIVE_ERROR:
						{
							pController->OnReceiveError(lParam);					
						}
						break;

					default:
						break;
					}
				}		

			default:
				break;
			}

			return DefWindowProc ( hwnd, uMsg,  wParam, lParam );
		}

	}//namespace CONTROLLER
}//namespace NMBASE