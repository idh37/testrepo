// Client.cpp : Defines the class behaviors for the application.
//





#include "stdafx.h"
#include "Client.h"
#include <io.h>	// for _access



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_HELPPROMPTADDR   0x0376
#define HID_BASE_PROMPT     0x00030000UL		// IDP

/////////////////////////////////////////////////////////////////////////////
// CClientApp
//������ �ݿ��� Ŭ���̾�Ʈ


BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	//{{AFX_MSG_MAP(CClientApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientApp construction

CClientApp::CClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClientApp object

CClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClientApp initialization



void DeleteNMContegoCtrl()
{

	char WinDir[_MAX_PATH]={0,};
	char szModulePath[_MAX_PATH]={0,};
	char szDrive[_MAX_DRIVE]={0,};
	char szDir[_MAX_PATH]={0,};
	char szDirectory[_MAX_PATH]={0,};
	char szMkDirectory[_MAX_PATH]={0,};
	::GetModuleFileName(NULL, szModulePath, _MAX_PATH);
	_tsplitpath(szModulePath, szDrive, szDir, NULL, NULL);
	if(szDir[_tcslen(szDir)-1]=='\\') szDir[_tcslen(szDir)-1]=NULL;
	wsprintf(WinDir, "%s%s", szDrive, szDir);

	CString strModulePath, strHookModulePath;
	strModulePath.Format( "%s\\%s", WinDir, "NMContegoCtrl.dll" );
	strHookModulePath.Format( "%s\\%s", WinDir, "NMCtgAh.dll" );

	// ������ ���� �ϴ��� �˻�
	if( _access( (char*)strModulePath.operator LPCTSTR(), 0 ) != -1)
	{
		::DeleteFile(strModulePath);
	}

	if( _access( (char*)strHookModulePath.operator LPCTSTR(), 0 ) != -1 )
	{
		::DeleteFile(strHookModulePath);
	}
}

BOOL CClientApp::InitInstance()
{
	//afxAmbientActCtx = 0;
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
// ���������϶���....
// ###���׸���ƶ�

	//��ġ �ý��� ���� 090708
	//SetExeptionInfo();

	const CString strKey = _T("SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION");
	HKEY hSubKey = NULL; // open the key 
	if ( ::RegOpenKeyEx( HKEY_CURRENT_USER, (LPCSTR)strKey, 0, KEY_ALL_ACCESS, &hSubKey ) == ERROR_SUCCESS ) 
	{ 
		DWORD value = 9000;
#ifndef _DEBUG
		::RegSetValueEx( hSubKey, "Poker.exe", 0, REG_DWORD, (const BYTE*)&value, sizeof(value) );
#else
		::RegSetValueEx( hSubKey, "Poker_D.exe", 0, REG_DWORD, (const BYTE*)&value, sizeof(value) );
#endif
		::RegCloseKey( hSubKey ); 
	}

	const CString strKeyR = _T("SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_GPU_RENDERING");
	hSubKey = NULL; // open the key 
	if ( ::RegOpenKeyEx( HKEY_CURRENT_USER, (LPCSTR)strKeyR, 0, KEY_ALL_ACCESS, &hSubKey ) == ERROR_SUCCESS ) 
	{ 
		DWORD value = 1;
#ifndef _DEBUG
		::RegSetValueEx( hSubKey, "Poker.exe", 0, REG_DWORD, (const BYTE*)&value, sizeof(value) );
#else
		::RegSetValueEx( hSubKey, "Poker_D.exe", 0, REG_DWORD, (const BYTE*)&value, sizeof(value) );
#endif
		::RegCloseKey( hSubKey ); 
	}
	else if ( ::RegCreateKeyEx( HKEY_CURRENT_USER, (LPCSTR)strKeyR, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY, NULL, &hSubKey, NULL ) == ERROR_SUCCESS ) 
	{ 
		DWORD value = 1;
#ifndef _DEBUG
		::RegSetValueEx( hSubKey, "Poker.exe", 0, REG_DWORD, (const BYTE*)&value, sizeof(value) );
#else
		::RegSetValueEx( hSubKey, "Poker_D.exe", 0, REG_DWORD, (const BYTE*)&value, sizeof(value) );
#endif
		::RegCloseKey( hSubKey ); 
	}
 
	DeleteNMContegoCtrl();

	// ����� Ŀ�ǵ� ���� �м�
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

//#ifdef _TEST
//	Test_Init();	//�׽�Ʈ ��ȯ�� ����
//#endif

// ###���׸���ƶ�
	// ���������϶���....
#ifndef _DEBUG	
#ifndef RELEASE_BUILD
	// ����� ���ڰ� �ϳ��� ������ ���α׷� ����
	if(cmdInfo.m_nShellCommand==0)
	{	
		AfxMessageBox("Ȩ����������\n�����ؾ߸� �ùٸ��� �۵��մϴ�.");
		return FALSE;
	}
#endif
#endif

	// ���������϶���....
#ifndef _DEBUG
#ifndef RELEASE_BUILD
	if(!AnalyzeArgument()) return FALSE;
#endif
#endif

#ifndef _DEBUG 
#ifndef RELEASE_BUILD
	//// �ߺ� ���� ����
	if(CheckMutex(m_pszAppName) == FALSE) 
		return FALSE;
#endif
#endif
	
	// ���� ���丮�� ������ ����� ������ �����Ѵ�.
	SetCurPathToRunFilePath();

	if (!AfxSocketInit())
	{
		AfxMessageBox("Windows ���� �ʱ�ȭ�� �����Ͽ����ϴ�.");
		return FALSE;
	}

	if(CreateStrManager() == FALSE) return FALSE;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	//Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	//Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	m_pMainWnd = CreateMainFrame();
	if(m_pMainWnd == NULL) return FALSE;

	//if(!m_pMainWnd->CreateEx(NULL, AfxRegisterWndClass(CS_DBLCLKS | CS_OWNDC), "MainWnd", WS_POPUP | WS_SYSMENU | WS_OVERLAPPED | WS_CLIPCHILDREN | WS_MINIMIZEBOX, 0, 0, 800, 600, NULL, NULL))
	//{
	//	return FALSE;
	//}

	m_pMainWnd->CenterWindow(NULL);

	return TRUE;
}

int CClientApp::Run() 
{
	MSG m_msgCur;

	try
	{
		srand( (DWORD)time(NULL) );

		do
		{
			while( PeekMessage( &m_msgCur, NULL, 0U, 0U, PM_REMOVE ) )
			{
				if(WM_QUIT == m_msgCur.message) 
					break;

				MainFrameDraw();

				if( !PreTranslateMessage( &m_msgCur ) )
				{
					::TranslateMessage(&m_msgCur);
					::DispatchMessage(&m_msgCur);
				}
			}

			if(WM_QUIT != m_msgCur.message)
				MainFrameDraw();

			Sleep( 1 );
		}
		while( WM_QUIT != m_msgCur.message );

		ExitInstance();
	}
	catch(...)
	{
	}

	return (int)m_msgCur.wParam;
}

BOOL CClientApp::ExitInstance()
{
	if(m_pMainWnd)
	{
		DestroyMainFrame();
		m_pMainWnd = NULL;
	}
	BOOL bRet = __super::ExitInstance();
	return bRet;
}