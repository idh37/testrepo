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
			// ���� �޽���ó�� ������
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

			// SocketCamelLib �޽��������� �ʱ�ȭ			
			BOOL bResult = UTIL::MM().NetCall(MSG_SOCKETCAMELINIT, (WPARAM&)m_hSockMsgWnd);

			if(FALSE == bResult)
			{
				// ���α׷� ����		
				AfxMessageBox("���� �ʱ�ȭ�� �����Ͽ����ϴ�");		
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

			void* strSockData							 = pNotifyData->pData;											///< ������
			Camel::Dar::PacketHeader* rHeader			 = (Camel::Dar::PacketHeader*)pNotifyData->rHeader;				///< ���
			Camel::Base::System::Stream::Handle* hStream = (Camel::Base::System::Stream::Handle*)pNotifyData->hStream;	///< �ڵ�
			int nPacketSize								 = pNotifyData->nPacketSize;									///< ��Ŷ ������

			// �ñ׳ΰ��� ���ڿ��� �����Ͽ� ��Ŷ�� ȣ���ϵ��� �Ѵ�.
			CString strCommand;
			strCommand.Format("%d", rHeader->GetPacketMsgId());


#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CNetWorkCamelController::OnSocketReceive() - Command:%s", strCommand.GetBuffer());
			TTrace::Debug()->Send(szTemp, "nmcontroller");   // single byte string	
#endif	// TRACE_OUTPUT

			// �ñ׳��� ���ڸ� ���ڿ��� ��ȯ�� �Ŀ� ������ �� ��Ʈ�� �޽����� ȣ���Ѵ�.	
			UTIL::MM().NetCall(strCommand.GetBuffer(), (WPARAM&)nPacketSize, (LPARAM&)pNotifyData);

			return TRUE;
		}


// 		BOOL CNetWorkCamelController::OnSocketClose( LPARAM lParam )
// 		{
// 			//LPSTR pszSockName = (LPSTR)lParam;
// 
// 			UTIL::MM().NetCall(MSG_SOCKETCAMEL_CLOSE, 0, 0);
// 
// 			//// ������ �������� ������ ������ �����
// 			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
// 			//{
// 			//	// ��ȯ����
// 			//	//if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST) 
// 			//	{
// 			//		// ������ �������� ������ �������� �������ϰ� ��� ������ ���ϵ� ������ ���´�
// 			//		SOCKCAMEL::GSOCKCAMELMAN().Destroy();
// 
// 			//		// ��ȯ����
// 			//		/*if(!GM().IsNowLogout())
// 			//		{				
// 			//		ShowMainMessageDlg(false, "���� �������� ������ ���������ϴ�");
// 			//		}
// 			//		SetChangeWhere(IDX_GAMEWHERE_FIRST,false);			
// 			//		OnCancel();
// 			//		*/
// 			//	}
// 			//}
// 
// 			//// ���Ӽ����� ������ ������ �����
// 			//if(!strcmp(NMBASE::DATA::GINFO()->GetGameServerName(), pszSockName))
// 			//{
// 			//	// ���μ������� ������ �������� �������ϰ� ��� ������ ���ϵ� ������ ���´�
// 			//	SOCKCAMEL::GSOCKCAMELMAN().Destroy();	
// 
// 			//	// ��ȯ����
// 			//	//// �α��� ���� �ƴϸ� ���� ������ �˸���
// 			//	//if(GetCurrentWhere() != IDX_GAMEWHERE_LOGIN && GetCurrentWhere() != IDX_GAMEWHERE_FIRST)
// 			//	//{
// 			//	//	if(!m_bAskLogout)
// 			//	//	{
// 			//	//		// �α׾ƿ� ��û�� ���°� �ƴϸ� ���� ������ �˸�
// 			//	//		ShowMainMessageDlg(false,"���� �������� ������ ���������ϴ�");
// 
// 			//	//		SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
// 			//	//		//ChangeView(0);
// 			//	//	}
// 
// 			//	//	// ���α׷� ����
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
// 			//// ������ ����
// 			//if(!strcmp(NMBASE::DATA::GINFO()->GetMasterServerName(), pszSockName))
// 			//{
// 
// 			//}
// 
// 			//// ���� ����
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
			//	// ��� ó������??
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