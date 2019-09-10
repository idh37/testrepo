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
			// ���� �޽���ó�� ������
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

			// SocketLib �޽��������� �ʱ�ȭ
			//BOOL bResult = m_cSockCmnd.Init(CWnd::FromHandle(m_hSockMsgWnd), &SOCK::GSOCKMAN());
			BOOL bResult = UTIL::MM().NetCall(MSG_SOCKETINIT, (WPARAM&)m_hSockMsgWnd);

			if(FALSE == bResult)
			{
				// ���α׷� ����		
				AfxMessageBox("���� �ʱ�ȭ�� �����Ͽ����ϴ�");		
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

			std::string strSockName  = pNotifyData->szSockName;		///< ���� �̸�
			int nSignal   = pNotifyData->Signal;					///< �޽��� �ñ׳�
			int nTotSize  = pNotifyData->TotSize;					///< �޽��� ������
			char *lpData = pNotifyData->lpData;						///< �޽��� ������ ������

			//	m_cLobby.OnPacketNotify(Sid, Signal, TotSize, lpData);
			//	GM().OnPacketNotify(Sid, Signal, TotSize, lpData);

			// LRESULT NetCall(std::string stCommand, WPARAM wParam = 0, LPARAM lParam = 0);

			// �ñ׳ΰ��� ���ڿ��� �����Ͽ� ��Ŷ�� ȣ���ϵ��� �Ѵ�.
			// �� �κ��� �ּ��� Ǯ�� Packet�� Command�� ���ڿ��� ó���� �ϱ����� ����
			//	CString strCommand;
			//	strCommand.Format("%d", Signal);


#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			//sprintf(szTemp, "CNetWorkController::OnSocketReceive() - Command:%s", strCommand.GetBuffer());
			sprintf(szTemp, "CNetWorkController::OnSocketReceive() - Command:%d", nSignal);
			TTrace::Debug()->Send(szTemp, "nmcontroller");   // single byte string	
#endif	// TRACE_OUTPUT

			// �ñ׳��� ���ڸ� ���ڿ��� ��ȯ�� �Ŀ� ������ �� ��Ʈ�� �޽����� ȣ���Ѵ�.	
			//UTIL::MM().NetCall(strCommand.GetBuffer(), (WPARAM&)TotSize, (LPARAM&)pNotifyData);
			UTIL::MM().NetCall(nSignal, (WPARAM&)nTotSize, (LPARAM&)pNotifyData);


			return TRUE;
		}


		BOOL CNetWorkController::OnSocketClose( LPARAM lParam )
		{
			LPSTR pszSockName = (LPSTR)lParam;

			UTIL::MM().NetCall(MSG_SOCKET_CLOSE, 0, lParam);

			// Socket�� �����Ѵ�.
			SOCK::GSOCKMAN().Destroy();


			//// ������ �������� ������ ������ �����
			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
			//{
			//	// ��ȯ����
			//	//if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST) 
			//	{
			//		// ������ �������� ������ �������� �������ϰ� ��� ������ ���ϵ� ������ ���´�
			//		SOCK::GSOCKMAN().Destroy();

			//		// ��ȯ����
			//		/*if(!GM().IsNowLogout())
			//		{				
			//		ShowMainMessageDlg(false, "���� �������� ������ ���������ϴ�");
			//		}
			//		SetChangeWhere(IDX_GAMEWHERE_FIRST,false);			
			//		OnCancel();
			//		*/
			//	}
			//}

			//// ���Ӽ����� ������ ������ �����
			//if(!strcmp(NMBASE::DATA::GINFO()->GetGameServerName(), pszSockName))
			//{
			//	// ���μ������� ������ �������� �������ϰ� ��� ������ ���ϵ� ������ ���´�
			//	SOCK::GSOCKMAN().Destroy();	

			//	// ��ȯ����
			//	//// �α��� ���� �ƴϸ� ���� ������ �˸���
			//	//if(GetCurrentWhere() != IDX_GAMEWHERE_LOGIN && GetCurrentWhere() != IDX_GAMEWHERE_FIRST)
			//	//{
			//	//	if(!m_bAskLogout)
			//	//	{
			//	//		// �α׾ƿ� ��û�� ���°� �ƴϸ� ���� ������ �˸�
			//	//		ShowMainMessageDlg(false,"���� �������� ������ ���������ϴ�");

			//	//		SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
			//	//		//ChangeView(0);
			//	//	}

			//	//	// ���α׷� ����
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

			//// ������ ����
			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
			//{

			//}

			//// ���� ����
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

			//// ������ ����
			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
			//{
			//	// ��ȯ����
			//	// ������ ������ ������ ������ �����
			//	// 		if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST)
			//	// 		{
			//	// 			SetChangeWhere(IDX_GAMEWHERE_FIRST, false);		
			//	// 			GM().SetWaitResponse(false);
			//	// 
			//	// 			m_dlgLoading.OnCancel();
			//	// 
			//	// 			ShowMainMessageDlg(false, "���� ������ ������ �����Ͽ����ϴ�");	
			//	// 			OnCancel();
			//	// 
			//	// 		}
			//	// 
			//	// 		// �ٸ� ���� ������ ä�η� �̵��� ������ ������ �����
			//	// 		if(GetCurrentWhere() == IDX_GAMEWHERE_LOBY && m_cLobby.IsMovingGame())
			//	// 		{
			//	// 			SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
			//	// 
			//	// 			GM().SetWaitResponse(false);		
			//	// 			ShowMainMessageDlg(false, "���� �̵��� �����Ͽ����ϴ�. �������� �ֽñ� �ٶ��ϴ�.");			
			//	// 
			//	// 			// ���α׷� ����
			//	// 			GM().SetNowLogout(true);
			//	// 			OnCancel();
			//	// 		}
			//}

			//// ���� ����
			//if(!strcmp(NMBASE::DATA::GINFO()->GetGameServerName(), pszSockName))
			//{
			//	// ��ȯ����
			//	// �α����ϴ����϶� ���μ����� ������ ������ �����
			//	// 		if(GetCurrentWhere()==IDX_GAMEWHERE_LOGIN)
			//	// 		{
			//	// 			SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
			//	// 			GM().SetWaitResponse(false);
			//	// 
			//	// 			m_dlgLoading.OnCancel();		
			//	// 			ShowMainMessageDlg(false, "���� ������ ������ �����Ͽ����ϴ�");		
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