#include "stdafx.h"
#include "NetWorkController.h"
#include "CommonDefine.h"
#include "CommandDefine.h"
#include "CommonUserWndMsg.h"


namespace NMBASE
{
	namespace CONTROLLER
	{

		CNetWorkController::CNetWorkController()
		{

		}

		CNetWorkController::~CNetWorkController()
		{

		}

		BOOL CNetWorkController::OnAttached()
		{
			// 소켓 메시지처리 윈도우
			CreateSockMsgWnd();

			return TRUE;
		}

		BOOL CNetWorkController::OnDetached()
		{
			delete this;
			return TRUE;
		}


		bool CNetWorkController::CreateSockMsgWnd()
		{
			WNDCLASS wcx; 

			wcx.style			= 0; 
			wcx.lpfnWndProc		= SockMsgWndProc;
			wcx.cbClsExtra		= 0;
			wcx.cbWndExtra		= 4;
			wcx.hInstance		= AfxGetInstanceHandle();
			wcx.hIcon			= NULL;
			wcx.hCursor			= NULL;
			wcx.hbrBackground	= NULL;
			wcx.lpszMenuName	= NULL;
			wcx.lpszClassName	= "SockMsgWnd";

			RegisterClass( &wcx ); 

			m_hSockMsgWnd = CreateWindow( "SockMsgWnd", "", WS_POPUP, 0,0,0,0, NULL, NULL, AfxGetInstanceHandle(), NULL );

			_ASSERTE( m_hSockMsgWnd != NULL );

			SetWindowLong( m_hSockMsgWnd, GWL_USERDATA, (LONG)this );

			// SocketLib 메시지윈도우 초기화
			//BOOL bResult = m_cSockCmnd.Init(CWnd::FromHandle(m_hSockMsgWnd), &SOCK::GSOCKMAN());
			BOOL bResult = UTIL::MM().NetCall(MSG_SOCKETINIT, (WPARAM&)m_hSockMsgWnd);

			if(FALSE == bResult)
			{
				// 프로그램 종료		
				AfxMessageBox("소켓 초기화에 실패하였습니다");		
			}

			return (m_hSockMsgWnd != NULL);
		}


		BOOL CNetWorkController::OnSocketReceive( LPARAM lParam )
		{
			SOCK::SGAMENOTIFY *pNotifyData = (SOCK::SGAMENOTIFY*)lParam;
			if(!pNotifyData) 
			{
				return FALSE;
			}

			std::string strSockName  = pNotifyData->szSockName;		///< 소켓 이름
			int nSignal   = pNotifyData->Signal;					///< 메시지 시그널
			int nTotSize  = pNotifyData->TotSize;					///< 메시지 사이즈
			char *lpData = pNotifyData->lpData;						///< 메시지 데이터 포인터

			//	m_cLobby.OnPacketNotify(Sid, Signal, TotSize, lpData);
			//	GM().OnPacketNotify(Sid, Signal, TotSize, lpData);

			// LRESULT NetCall(std::string stCommand, WPARAM wParam = 0, LPARAM lParam = 0);

			// 시그널값을 문자열로 변경하여 패킷을 호출하도록 한다.
			// 이 부분의 주석을 풀면 Packet의 Command를 문자열로 처리를 하기위해 변경
			//	CString strCommand;
			//	strCommand.Format("%d", Signal);


#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			//sprintf(szTemp, "CNetWorkController::OnSocketReceive() - Command:%s", strCommand.GetBuffer());
			sprintf(szTemp, "CNetWorkController::OnSocketReceive() - Command:%d", nSignal);
			TTrace::Debug()->Send(szTemp, "nmcontroller");   // single byte string	
#endif	// TRACE_OUTPUT

			// 시그널의 숫자를 문자열로 변환한 후에 대입한 후 네트웍 메시지를 호출한다.	
			//UTIL::MM().NetCall(strCommand.GetBuffer(), (WPARAM&)TotSize, (LPARAM&)pNotifyData);
			UTIL::MM().NetCall(nSignal, (WPARAM&)nTotSize, (LPARAM&)pNotifyData);


			return TRUE;
		}


		BOOL CNetWorkController::OnSocketClose( LPARAM lParam )
		{
			LPSTR pszSockName = (LPSTR)lParam;

			UTIL::MM().NetCall(MSG_SOCKET_CLOSE, 0, lParam);

			// Socket을 삭제한다.
			SOCK::GSOCKMAN().Destroy();


			//// 마스터 서버와의 접속이 끊어진 경우라면
			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
			//{
			//	// 종환복구
			//	//if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST) 
			//	{
			//		// 마스터 서버와의 접속이 끊어지면 서버소켓과 모든 데이터 소켓도 접속을 끊는다
			//		SOCK::GSOCKMAN().Destroy();

			//		// 종환복구
			//		/*if(!GM().IsNowLogout())
			//		{				
			//		ShowMainMessageDlg(false, "메인 서버와의 접속이 끊어졌습니다");
			//		}
			//		SetChangeWhere(IDX_GAMEWHERE_FIRST,false);			
			//		OnCancel();
			//		*/
			//	}
			//}

			//// 게임서버와 접속이 끊어진 경우라면
			//if(!strcmp(NMBASE::DATA::GINFO()->GetGameServerName(), pszSockName))
			//{
			//	// 메인서버와의 접속이 끊어지면 서버소켓과 모든 데이터 소켓도 접속을 끊는다
			//	SOCK::GSOCKMAN().Destroy();	

			//	// 종환복구
			//	//// 로그인 중이 아니면 접속 끊김을 알린다
			//	//if(GetCurrentWhere() != IDX_GAMEWHERE_LOGIN && GetCurrentWhere() != IDX_GAMEWHERE_FIRST)
			//	//{
			//	//	if(!m_bAskLogout)
			//	//	{
			//	//		// 로그아웃 요청한 상태가 아니면 접속 끊김을 알림
			//	//		ShowMainMessageDlg(false,"게임 서버와의 접속이 끊어졌습니다");

			//	//		SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
			//	//		//ChangeView(0);
			//	//	}

			//	//	// 프로그램 종료
			//	//	GM().SetNowLogout(true);
			//	//	OnCancel();
			//	//}
			//	//else
			//	//{					
			//	//	m_dlgLoading.OnCancel();
			//	//}
			//}

			return TRUE;
		}


		BOOL CNetWorkController::OnSuccessConnect( LPARAM lParam )
		{
			LPSTR pszSockName = (LPSTR)lParam;

			UTIL::MM().NetCall(MSG_SOCKET_SUCCESSCONNECT, 0, lParam);

			//CString str;

			//// 마스터 서버
			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
			//{

			//}

			//// 게임 서버
			//if(!strcmp(NMBASE::DATA::GINFO()->GetGameServerName(), pszSockName))
			//{

			//}


			return TRUE;
		}


		BOOL CNetWorkController::OnFailConnect(LPARAM lParam)
		{
			LPSTR pszSockName = (LPSTR)lParam;

			UTIL::MM().NetCall(MSG_SOCKET_FAILCONNECT, 0, lParam);

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


		LRESULT CALLBACK SockMsgWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		{
			switch(uMsg)
			{
			case UM_SOCKET_NOTIFY:
				{
					CNetWorkController* pController = (CNetWorkController*)(GetWindowLong(hwnd, GWL_USERDATA));
					_ASSERTE( pController != NULL );

					UINT nCommand = (UINT)wParam;

					switch(nCommand)
					{
					case UM_SOCKET_RECEIVE:
						{
							pController->OnSocketReceive(lParam);				
						}
						break;

					case UM_SOCKET_CLOSE:
						{					
							pController->OnSocketClose(lParam);				
						}
						break;

					case UM_SOCKET_CONNECT_SUCCESS:
						{
							pController->OnSuccessConnect(lParam);		
						}
						break;

					case UM_SOCKET_CONNECT_FAIL:
						{
							pController->OnFailConnect(lParam);					
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