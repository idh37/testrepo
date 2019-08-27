#include "stdafx.h"
#include "NMApp.h"
#include <afxsock.h>

#include "NMMainFrmView.h"
//#include "ExHandle.h"			// Exception
#include "CommonDefine.h"		// 공통 Define
#include "RunparamInfo.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace FRAME
	{

		BEGIN_MESSAGE_MAP(CNMApp, CWinApp)
		END_MESSAGE_MAP()


		CNMApp::CNMApp()
		{
			m_nWindowWidth = 1024;		///< 윈도우 해상도 Width
			m_nWindowHeight= 768;		///< 윈도우 해상도 Height

			m_bExitFlag = FALSE;

			m_eSocketType = SOCKETTYPE_DEFAULT;	///< Default Sock사용

			UTIL::MM().AddMsgH("NMApp.ProcessModalDialog", UTIL::Fnt(this, &CNMApp::ProcessModalDialog));
		}

		CNMApp::~CNMApp()
		{

		}


		// 기본 오브젝트 등록
		BOOL CNMApp::LoadDefaultObjects()
		{
			// 계속 추가 된다.

			// Attach Default components

			UTIL::MM().AttachComponent(new MODEL::CLogInModel());
			UTIL::MM().AttachComponent(new CONTROLLER::CLogInCtroller());			///< 로그인( 마스터서버에 접속해서 받는 정보 )	

			UTIL::MM().AttachComponent(new MODEL::CChannelModel());
			UTIL::MM().AttachComponent(new CONTROLLER::CChannelController());

			UTIL::MM().AttachComponent(new MODEL::CFrameModel());
			UTIL::MM().AttachComponent(new CONTROLLER::CFrameController());

			UTIL::MM().AttachComponent(new MODEL::CLobbyModel());
			UTIL::MM().AttachComponent(new CONTROLLER::CLobbyController());

			UTIL::MM().AttachComponent(new CONTROLLER::CNetWorkController());
	

			return TRUE;
		}


		BOOL CNMApp::LoadProcessObjects()
		{

			// 	UTIL::MM().AttachComponent( new CNMGameProcess );
			// 	UTIL::MM().AttachComponent( new CNMChannelProcess( new CNMChannelView ) );
			// 	UTIL::MM().AttachComponent( new CNMLobbyProcess( new CNMLobbyView )	);

			// 계속 추가 된다.

			return TRUE;
		}


		BOOL CNMApp::InitInstance( void )
		{
			if(SOCKETTYPE_DEFAULT == m_eSocketType)
			{
				// 소켓 초기화
				if(!AfxSocketInit())
				{
					AfxMessageBox("Windows 소켓 초기화에 실패하였습니다.");
					return FALSE;
				}
			}
			

			// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
			// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
			// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
			INITCOMMONCONTROLSEX InitCtrls;
			InitCtrls.dwSize = sizeof(InitCtrls);
			// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
			// 이 항목을 설정하십시오.
			InitCtrls.dwICC = ICC_WIN95_CLASSES;
			InitCommonControlsEx(&InitCtrls);

			CWinApp::InitInstance();
			afxAmbientActCtx = FALSE;	// Dll 함수 호출이 예기치 않게 종료될 때(Crash아님). assert 비슷하게 경고 팝업이 뜨는데 그 팝업이 안뜨도록하는... [8/12/2011 guesty]
			AfxEnableControlContainer();
			::CoInitialize(NULL);

#ifdef TRACE_OUTPUT	
			//	TTrace::Options()->sendMode = WinMsg;
			//	TTrace::Show(true);
			TTrace::Debug()->Send("CNMApp시작-InitInstance", "nmframe");   // single byte string	
#endif	// TRACE_OUTPUT


			///< 게임마다 Exception정보가 다르기 때문에 필수로 세팅
			SetExceptionInfo();	

			// 명령행 커맨드 정보 분석
			CCommandLineInfo cmdInfo;
			ParseCommandLine(cmdInfo);

#ifndef _DEBUG
 			// 명령행 인자가 하나도 없으면 프로그램 종료
 			if(cmdInfo.m_nShellCommand==0)
 			{	
 				AfxMessageBox("홈페이지에서\n실행해야만 올바르게 작동합니다.");
 				return FALSE;
 			}
#endif

#ifdef _DEBUG	
			///< Dialog또는 CommandLine을 통해 얻는다.
			if(!SetLoinInfo())
			{
				return FALSE;
			}
#else
		///< RunParam값을 얻어온다.  ( 릴리즈모드일때만.... )
			if(!AnalyzeRunParam())
			{
				return FALSE;
			}
#endif

			// Release Only...	
#ifndef _DEBUG 
			// 중복 실행 방지
			if(CheckMutex(m_pszAppName) == FALSE) 
			{
				return FALSE;
			}
#endif

			// 현재 디렉토리를 게임이 실행된 곳으로 설정한다.
			SetCurPathToRunFilePath();


			// 종환복구(일단패스)
			// 게임에서 사용되는 문자열을 가져오도록 한다.
			/*if(CreateStrManager() == FALSE) 
			{
			return FALSE;
			}*/

			// MainFrame Window Pointer를 얻는다.
			CNMMainFrmView* pNewWnd = dynamic_cast<CNMMainFrmView*>(MakeFrameInstance(m_nWindowWidth, m_nWindowHeight));
			_ASSERTE(pNewWnd && "m_pMainWnd is not derived class of CNMMainFrmView.");

			if(pNewWnd == NULL )
			{
				return FALSE;
			}

			m_pMainWnd = pNewWnd;

			// 메인 윈도우를 생성한다.
			if(!pNewWnd->CreateEx( 0, AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW), "임시-게임이름", 
									WS_POPUP | WS_CLIPCHILDREN, 0, 0, m_nWindowWidth, m_nWindowHeight, NULL, NULL) )
			{
				return FALSE;
			}

			// 객체를 생성한다.
			LoadDefaultObjects();

 			if(SOCKETTYPE_DEFAULT == m_eSocketType)
 			{
 				// Master서버 Connet
 				CString strServerName = (CString)MODEL::GNET().ConnectSocket((char*)(LPCTSTR)DATA::GINFO()->GetRunParm()->GetServerIP(), DATA::GINFO()->GetRunParm()->GetServerPort());
 				if(strServerName.IsEmpty())
 				{
 					// 마스터서버 접속 실패
 					ASSERT(FALSE);
 					return FALSE;
 				}
 
 				NMBASE::DATA::GINFO()->SetMasterServerName(strServerName.GetBuffer());
 			}
 			else
 			{
 				// Camel Socket
 				BOOL bResult = MODEL::GCAMELNET().ConnectSocket((char*)(LPCTSTR)DATA::GINFO()->GetRunParm()->GetServerIP(), DATA::GINFO()->GetRunParm()->GetServerPort());
 
 				if(!bResult)
 				{
 					return FALSE;
 				}
 			}		

			if(!InitEngine(m_pMainWnd))
			{
				PostQuitMessage(-1);
			}

			// View 및 ProcessObject를 생성 및 등록을 한다.
			LoadProcessObjects();

			// 윈도우를 화면 중앙에 위치 시키는 함수. (엔진 초기화시 위치를 변경할 수 있기 때문에 이곳에서 세팅한다.)
			m_pMainWnd->CenterWindow();


			///> 사용암함 - 숫자대신 문자열 사용
			//	DATA::GINFO()->SetMasterSID(nMasterSID);
			//	DATA::GINFO()->SetMainSrvSID(-1);

			///< ICon을 세팅한다.
			SetIcon();

			// 로그인 -> 채널 선택 -> 대기실까지 자동으로 입장하는 로직을 추가를 한다. - 임시로 되어 있는데 상속받아 게임을 넣어 주도록 한다.
			UTIL::NMGoGameDirect_Init(&m_hMutex, "spoker2");

			return TRUE;
		}


		int CNMApp::ExitInstance( void )
		{
			// 뮤텍스 제거
			DestroyMutex();

			int nRet = CWinApp::ExitInstance();

			return nRet;
		}


		CFrameWnd* CNMApp::MakeFrameInstance(int nWidth, int nHeight)
		{
			return new CNMMainFrmView();
		}


		void* CNMApp::MakeGameInstance()
		{
			return NULL;
		}


		// MFC 메인 루프
		int	CNMApp::Run()
		{
			// TODO: Add your specialized code here and/or call the base class
			MSG m_msgCur;

			srand( (DWORD)time(NULL) );

			PeekMessage(&m_msgCur, NULL, 0U, 0U, PM_NOREMOVE);

			while(WM_QUIT != m_msgCur.message) 
			{
				while(PeekMessage(&m_msgCur, NULL, 0U, 0U, PM_REMOVE) != 0)
				{		
					if(m_msgCur.message == WM_QUIT)
					{
						goto Exit_loop;
						break;
					}

					if(!PreTranslateMessage( &m_msgCur ) )
					{
						::TranslateMessage(&m_msgCur);
						::DispatchMessage(&m_msgCur);
					}
				}

				if(!m_bExitFlag)
				{
					OnDraw();			///< GameLoop(Rendering... )
					OnUpdate();			///< Timer대신 매 틱마다 업데이트가 필요 할때...
				}			

				Sleep(1);
			}

			///< Quit Message가오면 루프를 종료 한다.
			Exit_loop:

			ExitInstance();

			return (int)m_msgCur.wParam;
		}
		// {
		// 	return CWinApp::Run(); 
		// }




		BOOL CNMApp::InitEngine(CWnd *pWindow)
		{	 
			return TRUE;
		} 


		BOOL CNMApp::OnIdle(LONG lCount)
		{
			CWinApp::OnIdle(lCount);


			return TRUE;//CWinApp::OnIdle(lCount);
		}


		BOOL CNMApp::CheckMutex(LPCSTR lpAppName)
		{
			// 중복 실행 방지
			m_hMutex=::CreateMutex(NULL, TRUE, _T("WoolalaMutex")); // 같은 뮤택스가 존재하면 이미 다른 프로그램이 실행중임

			if(ERROR_ALREADY_EXISTS == ::GetLastError())
			{       
				HWND hWnd=::FindWindow(_T("#32770 (Dialog)"), lpAppName); // ClassName, WindowsName
				::SetForegroundWindow(hWnd);

				if(m_hMutex)
				{
					::CloseHandle(m_hMutex);		
				}

				AfxMessageBox( "이미 다른 게임이 실행중입니다");

				return FALSE;
			}
			return TRUE;
		}

		void CNMApp::DestroyMutex()
		{
			if(m_hMutex != NULL )		
			{
				::CloseHandle( m_hMutex );
				m_hMutex= NULL;
			}
		}

		void CNMApp::SetCurPathToRunFilePath()
		{
			UTIL::NMNMRUNPARAMAPIFUNC_CMDARG::SetCurPathToRunFilePath();
		}


		// 
		BOOL CNMApp::AnalyzeRunParam()
		{
			NMRUNPARAM_RETCODE nReturncode = 0;


			// 클립보드 인자 전달 규칙(각각의 인자는 컴마(,)로 구분되며 인자 사이의 공백은 없다)
			// 전달 인자의 순서 : ServerIP,Port,AuthCookie,DataCookie,CpCookie,SpareParam (인자는 암호화되어 있다)
			// - 넷마블 게임에서는 CpCookie를 전달하지 않는다.
			// - CP 게임에서는 AuthCookie, DataCookie를 전달하지 않는다(몇몇 CP게임은 예외)
			// - Extra 인자는 생략되기도 한다.

			UTIL::CNMNMRunParam nmRunParam;
			nReturncode = nmRunParam.Load();

			BOOL bLoad = TRUE;

			if(nReturncode != ERROR_NMRUNPARAM_SUCCESS)
			{
				bLoad = FALSE;
			}

			if( !nmRunParam.ExistsParam(NMPARAMNAME_SERVIP)		||
				!nmRunParam.ExistsParam(NMPARAMNAME_SERVPORT)	||
				!nmRunParam.ExistsParam(NMPARAMNAME_AUTHCOOKIE)	||
				!nmRunParam.ExistsParam(NMPARAMNAME_DATACOOKIE)	||
				!nmRunParam.ExistsParam(NMPARAMNAME_CPCOOKIE) )
			{
				nReturncode = ERROR_NMRUNPARAM_LIB_MISSING_PARAM;
				bLoad		= FALSE;
			}

			if(bLoad && DATA::GINFO()->GetRunParm())
			{	
				DATA::GINFO()->GetRunParm()->SetServerIP(nmRunParam.GetParam(NMPARAMNAME_SERVIP));									// 서버 IP	
				DATA::GINFO()->GetRunParm()->SetServerPort((UINT)::atoi(nmRunParam.GetParam(NMPARAMNAME_SERVPORT)));				// 서버 Port
				DATA::GINFO()->GetRunParm()->SetAuthCookie(nmRunParam.GetParam(NMPARAMNAME_AUTHCOOKIE));							// 인증 쿠키.
				DATA::GINFO()->GetRunParm()->SetDataCookie(nmRunParam.GetParam(NMPARAMNAME_DATACOOKIE));							// 데이터 쿠키.
				DATA::GINFO()->GetRunParm()->SetCpCookie(nmRunParam.GetParam(NMPARAMNAME_CPCOOKIE));								// CP쿠키.
				DATA::GINFO()->GetRunParm()->SetSpareParam(nmRunParam.GetParam(NMPARAMNAME_SPAREPARAM));							// 여분의 인자(CP게임등에서 사용)
				DATA::GINFO()->GetRunParm()->SetLoginGroup(UTIL::GetGroupNo_BySpareParam(DATA::GINFO()->GetRunParm()->GetSpareParam()));		//
				DATA::GINFO()->GetRunParm()->SetLoginGameCode(UTIL::Get_BySpareParam(DATA::GINFO()->GetRunParm()->GetSpareParam(), "gamecode"));//
			}
			else
			{
				AfxMessageBox("프로그램에 필요한 환경 값을 얻지 못 했습니다."); 
				return FALSE;
			}

			// 인자를 분석해서 게임 접속 환경을 초기화.
			if(ERROR_NMRUNPARAM_SUCCESS != nReturncode)
			{
				CString strMsg;
				CString strErrorCode;

				strErrorCode = UTIL::NMNMRUNPARAMAPIFUNC::ConvertRetCode2String(nReturncode);

				if(strErrorCode.IsEmpty() )
				{
					strErrorCode.Format("E%05d", nReturncode);
				}

				strMsg.Format("프로그램에 필요한 환경을 초기화할 수 없습니다.\r\n\r\n코드 : %s", strErrorCode.GetBuffer()); 

				AfxMessageBox(strMsg); 
				return FALSE;
			}

			CString strServerIP = DATA::GINFO()->GetRunParm()->GetServerIP();
			if(strServerIP.IsEmpty() || strServerIP.GetLength() > 16 )
			{
				AfxMessageBox("프로그램에 필요한 환경이 올바르지 않습니다."); 
				return FALSE;
			}

			CString strAuthCookie = DATA::GINFO()->GetRunParm()->GetAuthCookie();
			if(strAuthCookie.IsEmpty() || strAuthCookie.IsEmpty()) 
			{
				AfxMessageBox("인증이 되지 않았습니다. 로그인 하시기 바랍니다."); 
				return FALSE;
			}

			return TRUE;
		}


		// 게임에서 사용하는 문자열을 얻도록 한다.
		BOOL CNMApp::CreateStrManager()
		{
			char strDir[500]={0, };
			GetCurrentDirectory(500, strDir);

			// 수정해야함 - 알아서 언어를 찾아 세팅하도록 한다.
			// 0: Korean 1: English 2:Japanese 3:Chinese
			int g_MSCode = 0;

			switch(g_MSCode)
			{
			case 0:  // Korean
				{
					strcat(strDir, "\\common\\Data\\NPoker.str");
					if(DATA::GINFO()->GetStringManager() && !DATA::GINFO()->GetStringManager()->Open(strDir) )
					{
						AfxMessageBox(_T("Can't find [NPoker.str] file!"));
						return FALSE;
					}
				} 
				break;

			case 1:  // English
				{
					strcat(strDir, ".\\common\\Data\\SPoker_ENG.str");
					if(DATA::GINFO()->GetStringManager() && !DATA::GINFO()->GetStringManager()->Open(strDir) )
					{
						AfxMessageBox(_T("Can't find [SPoker_ENG.str] file!")); 
						return FALSE;
					}
				}
				break;

			case 2:  // Japanese
				{
					strcat(strDir, ".\\common\\Data\\SPoker_JPN.str");
					if(DATA::GINFO()->GetStringManager() && !DATA::GINFO()->GetStringManager()->Open(strDir) )
					{
						AfxMessageBox(_T("Can't find [SPoker_JPN.str] file!"));
						return FALSE;
					}
				} 
				break;

			case 3:  // Chinese
				{
					strcat(strDir, ".\\common\\Data\\SPoker_CHN.str");
					if(DATA::GINFO()->GetStringManager() && !DATA::GINFO()->GetStringManager()->Open(strDir) )
					{
						AfxMessageBox(_T("Can't find [SPoker_CHN.str] file!")); 
						return FALSE;
					}
				} 
				break;

			default: // Korean
				{
					strcat(strDir, ".\\common\\Data\\NPoker.str");
					if(DATA::GINFO()->GetStringManager() && !DATA::GINFO()->GetStringManager()->Open(strDir) )
					{
						AfxMessageBox(_T("Can't find [NPoker.str] file!")); 
						return FALSE;
					}
				}
				break;
			}

			return TRUE;
		}

		void CNMApp::OnUpdate()
		{

		}

		///< ICon을 세팅한다.
		void CNMApp::SetIcon()
		{
			// The framework does this automatically when the application's main window is not a dialog
			//SetIcon(m_hIcon, TRUE);			// Set big icon
			//SetIcon(m_hIcon, FALSE);		// Set small icon
		}


		///< 로그인 정보를 얻는다.(ID/PW/IP/PORT....)
		BOOL CNMApp::SetLoinInfo()
		{
			CString strCommand = GetCommandLine();
			int nTemp = strCommand.Find(":");
			nTemp = strCommand.Find(":", nTemp+1);

			if(nTemp == -1)
			{	
				// 종환복구(다른 프로젝트에는 의미가 없어 보임)
				//"poker_d.exe" 프로세스가 몇 개 실행되어있는지?
				/*int nProcCnt = 0;
				HANDLE         hProcessSnap = NULL; 		
				PROCESSENTRY32 pe32         = {0};
				hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
				if (hProcessSnap != INVALID_HANDLE_VALUE) 
				{
				pe32.dwSize = sizeof(PROCESSENTRY32);

				if (Process32First(hProcessSnap, &pe32))
				{ 
				DWORD Code = 0;
				do 
				{ 
				HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); 

				if (stricmp("poker_d.exe", pe32.szExeFile)==0)
				{
				nProcCnt++;
				}
				CloseHandle (hProcess);
				} 
				while (Process32Next(hProcessSnap, &pe32));
				} 
				CloseHandle (hProcessSnap);					
				}

				////로그인아이디 결정하기
				//char PCName[128] ={0, };
				//DWORD BufSize = sizeof(PCName)-1;
				//GetComputerName(PCName, &BufSize);
				//int nMyIDNO = 0;
				//int i = 0;
				//while(PCName[i]!=0)
				//{
				//	nMyIDNO+= PCName[i++];	
				//}

				//nMyIDNO = ( (nMyIDNO %100) *10 ) + nProcCnt; 

				CLoginDlg LoginDlg;
				LoginDlg.m_ID.Format("t_test%05d", nMyIDNO);		
				LoginDlg.m_Pass			= "a12a12";
				LoginDlg.m_ServerIP		= "192.168.62.230";
				LoginDlg.m_ServerPort	= 24200;	
				LoginDlg.m_ServerPort	= 24200;
				LoginDlg.m_Group		= 120;


				if(LoginDlg.DoModal()==IDOK) 
				{
				g_LoginID    = LoginDlg.m_ID;
				g_LoginPass  = LoginDlg.m_Pass;
				g_ServIP     = LoginDlg.m_ServerIP;
				g_ServPort   = LoginDlg.m_ServerPort;
				g_LoginGroup = LoginDlg.m_Group;
				}
				else 
				{
				// 프로그램 종료
				ExitGameClient(false);
				return FALSE;
				}*/

				DATA::GINFO()->GetRunParm()->SetServerIP("192.168.62.230");	// 서버 IP	
				DATA::GINFO()->GetRunParm()->SetServerPort(24200);			// 서버 Port
				DATA::GINFO()->GetRunParm()->SetLoginGroup(110);				//
				DATA::GINFO()->GetRunParm()->SetLoginID("t_test00491");		
				DATA::GINFO()->GetRunParm()->SetLoginPass("a12a12");			//						
			}
			else
			{
				// 종환복구
				//아이피를 얻어온다.
				//int nStart = strCommand.Find("ip:");
				//if(nStart == -1)
				//{
				//	::MessageBox(NULL, "커멘드라인에 아이피 입력을 찾을수 없습니다. ip:xxx.xxx.xxx.xxx 형식으로 추가해 주세요", "경고", MB_OK);
				//	ExitGameClient(false);
				//	return FALSE;
				//}
				//nStart += 3;
				//int nEnd = strCommand.Find(" ", nStart);
				//if(nEnd == -1) nEnd = strCommand.GetLength();
				//g_ServIP = strCommand.Mid(nStart, nEnd - nStart);

				////포트를 얻어온다.
				//nStart = strCommand.Find("port:");
				//if(nStart == -1)
				//{
				//	::MessageBox(NULL, "커멘드라인에 포트 입력을 찾을수 없습니다. port:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
				//	ExitGameClient(false);
				//	return FALSE;
				//}
				//nStart += 5;
				//nEnd = strCommand.Find(" ", nStart);
				//if(nEnd == -1) nEnd = strCommand.GetLength();
				//CString strTemp = strCommand.Mid(nStart, nEnd - nStart);
				//g_ServPort = atoi(strTemp);

				////게임종류를 얻어온다.
				//nStart = strCommand.Find("game:");
				//if(nStart == -1)
				//{
				//	::MessageBox(NULL, "커멘드라인에 게임 종류를 찾을수 없습니다. game:xxxxx 형식으로 추가해 주세요\n ex)101:세븐포커, 103:로우바둑이, 104:뉴포커, 105:맞포커", "경고", MB_OK);
				//	ExitGameClient(false);
				//	return FALSE;
				//}
				//nStart += 5;
				//nEnd = strCommand.Find(" ", nStart);
				//if(nEnd == -1) nEnd = strCommand.GetLength();
				//strTemp = strCommand.Mid(nStart, nEnd - nStart);
				//g_LoginGroup = atoi(strTemp);

				////아이디를 얻어온다.
				//nStart = strCommand.Find("id:");
				//if(nStart == -1)
				//{
				//	::MessageBox(NULL, "커멘드라인에 아이디를 찾을수 없습니다. id:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
				//	ExitGameClient(false);
				//	return FALSE;
				//}
				//nStart += 3;
				//nEnd = strCommand.Find(" ", nStart);
				//if(nEnd == -1) nEnd = strCommand.GetLength();
				//g_LoginID = strCommand.Mid(nStart, nEnd - nStart);

				////패스워드를 얻어온다.
				//nStart = strCommand.Find("pass:");
				//if(nStart == -1)
				//{
				//	::MessageBox(NULL, "커멘드라인에 패스워드를 찾을수 없습니다. pass:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
				//	ExitGameClient(false);
				//	return FALSE;
				//}
				//nStart += 5;
				//nEnd = strCommand.Find(" ", nStart);
				//if(nEnd == -1) 
				//{
				//	nEnd = strCommand.GetLength();
				//}

				//g_LoginPass = strCommand.Mid(nStart, nEnd - nStart);
				// SSO 웹브라우저 모듈을 초기화 한다 - [SSO 작업]
// 				if(!NMBASE::UTIL::NMInitSSOWebBrowser(GetSafeHwnd(), g_AuthCookie, g_DataCookie, g_CpCookie)) 
// 				{
// 					// 릴리즈 모드에서 실패하는 경우 프로그램 종료
// #ifndef _DEBUG
// 					// 프로그램 종료
// 					CloseLoadingDlg();
// 					MessageBox("SSO WebBrowser 모듈을 초기화할 수 없습니다.", "Error", MB_OK|MB_ICONERROR);
// 
// 					return FALSE;
// #endif
// 				}
			}

			return TRUE;
		}


		///< GameLoop
		void CNMApp::OnDraw()
		{

		}

		LRESULT CNMApp::ProcessModalDialog(WPARAM &wParam, LPARAM &lParam)
		{
			OnDraw();			///< GameLoop(Rendering... )
			OnUpdate();			///< Timer대신 매 틱마다 업데이트가 필요 할때..
			return 0;
		}

		///< 게임마다 Exception정보가 다르기 때문에 필수로 세팅
		void CNMApp::SetExceptionInfo()
		{
			// Exception 적용( 인자 값에 대해서 수정을 해야 한다. - 종환)
			NMBASE::UTIL::CNMExHandle::SetExeptionInfo("poker", "NetmarblePoker", NMBASE::UTIL::CNMExHandle::EXHAN_CLIENT);
		}

	}//namespace FRAME

}//namespace NMBASE