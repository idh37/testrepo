#include "stdafx.h"
#include "NMContego/NMContegoManager.h"
#include "NMContegoExtErrorCodes.h"
#include "ContegoThread.h"
#include <io.h>	// for _access


CNMContegoManager &gNMContegoMan()
{
	return CNMContegoManager::GetInstance();
}


CNMContegoManager::CNMContegoManager(void)
{
	m_hNMCtrlModule				= NULL;

	m_pfNMCtgInitialize			= NULL;
	m_pfProtectAutoKey			= NULL;
	m_pfReleaseAutoKey			= NULL;
	m_pfProtectAutoMouse		= NULL;
	m_pfReleaseAutoMouse		= NULL;
	m_pfProtectScrCptGlobal		= NULL;
	m_pfProtectScrCptLocal		= NULL;
	m_pfProtectMemory			= NULL;
	m_pfHideProcess				= NULL;
	m_pfProtectInput			= NULL;
	m_pfSetDetectCallBack		= NULL;
	m_pfNMCtgRelease			= NULL;
}

CNMContegoManager::~CNMContegoManager(void)
{
}

CNMContegoManager& CNMContegoManager::GetInstance()
{
	static CNMContegoManager s_oNMContegoManager;
	return s_oNMContegoManager;
}

bool CNMContegoManager::InitHostInfo()
{
	char szHostName[256] = {0,};

	PHOSTENT pHostEntry = NULL;
	IN_ADDR inAddr;

	if( gethostname( szHostName, sizeof(szHostName) ) != 0 ) 
	{
		ThrowError( NMCTG_EXT_ERROR_FILE_NOT_EXIST );
		return false;
	}
	else
	{
		pHostEntry = gethostbyname(szHostName);
		if( pHostEntry == NULL )
		{
			ThrowError( NMCTG_EXT_ERROR_FILE_NOT_EXIST );
			return false;
		}
		else
		{
			memcpy( &inAddr, pHostEntry->h_addr, 4 );
		}
	}

	m_strIP = inet_ntoa(inAddr);
	m_strDomain = pHostEntry->h_name;

	return true;
}

bool CNMContegoManager::InitLibrary()
{
	VIRTUALIZER_START

	char szDir[_MAX_PATH]={0,};
	GetCurrentDirectory( sizeof(szDir)-1, szDir );

	CString strModulePath, strHookModulePath;
	strModulePath.Format( "%s\\%s", szDir, "NMContegoCtrl.dll" );
	strHookModulePath.Format( "%s\\%s", szDir, "NMCtgAh.dll" );

	// 파일이 존재 하는지 검사
	if( _access( (char*)strModulePath.operator LPCTSTR(), 0 ) == -1
		|| _access( (char*)strHookModulePath.operator LPCTSTR(), 0 ) == -1 )
	{
		ThrowError( NMCTG_EXT_ERROR_FILE_NOT_EXIST );
		return false;
	}

	// 라이브러리 로딩
	m_hNMCtrlModule =  LoadLibrary( strModulePath );
	if( !m_hNMCtrlModule )
	{
		DWORD dwError = GetLastError();
		ThrowError( NMCTG_EXT_ERROR_LOAD_FAIL, dwError );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::BindLibraryFunction()
{
	VIRTUALIZER_START

	m_pfNMCtgInitialize				= (NMCTG_INITIALIZE)			GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(1, 0) );
	m_pfProtectAutoKey				= (NMCTG_PROTECT_AUTOKEY)		GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(2, 0) );
	m_pfReleaseAutoKey				= (NMCTG_RELEASE_AUTOKEY)		GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(3, 0) );
	m_pfProtectAutoMouse			= (NMCTG_PROTECT_AUTOMOUSE)		GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(4, 0) );
	m_pfReleaseAutoMouse			= (NMCTG_RELEASE_AUTOMOUSE)		GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(5, 0) );
	m_pfProtectScrCptGlobal			= (NMCTG_PROTECT_SCRCPT_GLOBAL)	GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(6, 0) );
	m_pfProtectScrCptLocal			= (NMCTG_PROTECT_SCRCPT_LOCAL)	GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(7, 0) );
	m_pfProtectMemory				= (NMCTG_PROTECT_MEMORY)		GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(8, 0) );
	m_pfHideProcess					= (NMCTG_HIDE_PROCESS)			GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(9, 0) );
	m_pfProtectInput				= (NMCTG_PROTECT_INPUT)			GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(10, 0) );
	m_pfSetDetectCallBack			= (NMCTG_SET_DETECT_CALLBACK)	GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(11, 0) );
	m_pfNMCtgRelease				= (NMCTG_RELEASE)				GetProcAddress( m_hNMCtrlModule, (LPCTSTR)MAKELONG(12, 0) );

	if( !m_pfNMCtgInitialize
		|| !m_pfProtectAutoKey
		|| !m_pfReleaseAutoKey
		|| !m_pfProtectAutoMouse
		|| !m_pfReleaseAutoMouse
		|| !m_pfProtectScrCptGlobal
		|| !m_pfProtectScrCptLocal
		|| !m_pfProtectMemory
		|| !m_pfHideProcess
		|| !m_pfProtectInput
		|| !m_pfNMCtgRelease 
		|| !m_pfSetDetectCallBack
		|| !m_pfNMCtgRelease )
	{
		FreeLibrary( m_hNMCtrlModule );
		m_hNMCtrlModule = NULL;

		ThrowError( NMCTG_EXT_ERROR_INVALID_EXPORT );

		return false;
	}

	m_pfSetDetectCallBack( CNMContegoManager::NMCtgDetectNotifyProc );

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::ProtectAutoKey()
{
	VIRTUALIZER_START

	if( m_pfProtectAutoKey == NULL )	return false;
	ULONG lResult = m_pfProtectAutoKey( NMCtgInputNotifyProc );

	if( ERROR_SUCCESS != lResult )
	{
		ThrowError( lResult );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::ProtectAutoMouse()
{
	VIRTUALIZER_START

	if( m_pfProtectAutoMouse == NULL )	return false;
	ULONG lResult = m_pfProtectAutoMouse( NMCtgInputNotifyProc );

	if( ERROR_SUCCESS != lResult )
	{
		ThrowError( lResult );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::ProtectInput()
{
	VIRTUALIZER_START

	if( m_pfProtectInput == NULL )
		return false;

	ULONG lResult = m_pfProtectInput( TRUE );

	if( ERROR_SUCCESS != lResult )
	{
		ThrowError( lResult );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::ProtectScreenCaptureGlobal()
{
	VIRTUALIZER_START

	if( m_pfProtectScrCptGlobal == NULL )
		return false;

	ULONG lResult = m_pfProtectScrCptGlobal( true );

	if( ERROR_SUCCESS != lResult )
	{
		ThrowError( lResult );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::ProtectScreenCaptureLocal()
{
	VIRTUALIZER_START

	if( m_pfProtectScrCptLocal == NULL )
		return false;

	ULONG lResult = m_pfProtectScrCptLocal( true );

	if( ERROR_SUCCESS != lResult )
	{
		ThrowError( lResult );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::UnProtectScreenCaptureLocal()
{
	VIRTUALIZER_START

	if( m_pfProtectScrCptLocal == NULL )
		return false;

	ULONG lResult = m_pfProtectScrCptLocal( false );

	if( ERROR_SUCCESS != lResult )
	{
		ThrowError( lResult );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::ProtectMemory()
{
	VIRTUALIZER_START

	if( m_pfProtectMemory == NULL )	return false;
	ULONG lResult = m_pfProtectMemory( true );

	if( ERROR_SUCCESS != lResult )
	{
		ThrowError( lResult );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::HideProcess( TCHAR* pProcessName, TCHAR* pWindowTitle )
{
	VIRTUALIZER_START

	if( m_pfHideProcess == NULL )	return false;
	ULONG lResult = m_pfHideProcess( true, pProcessName, pWindowTitle );

	if( ERROR_SUCCESS != lResult )
	{
		ThrowError( lResult );
		return false;
	}

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::NMCtgInitialize( int game_code, char* szProcess, char* szTitle )
{
	char szDir[_MAX_PATH]={0,};
	GetCurrentDirectory( sizeof(szDir)-1, szDir );

	CString strModulePath, strHookModulePath;
	strModulePath.Format( "%s\\%s", szDir, "NMContegoCtrl.dll" );
	strHookModulePath.Format( "%s\\%s", szDir, "NMCtgAh.dll" );

	// 파일이 존재 하는지 검사
	if( _access( (char*)strModulePath.operator LPCTSTR(), 0 ) != -1)
	{
		::DeleteFile(strModulePath);
	}

	if( _access( (char*)strHookModulePath.operator LPCTSTR(), 0 ) != -1 )
	{
		::DeleteFile(strHookModulePath);
	}
	return false;
#if defined(NMCONTEGOMANAGER_NOUSER)
	return false;
#endif
	VIRTUALIZER_START
	//yoo
#if defined(_DEBUG)
	return false;
#endif

	if( !CheckRandomLoad( 100 ) )
		return false;

	gContegoThread().Initilize( game_code,
								"game1.netmarble.net",
								"/common/SecurityModuleRnd.asp",
								"/common/SecurityModuleLog.asp" );

	//if( !InitHostInfo() )	// 웹쪽에서 체크해 주시기로..
	//	return false;

	if( !InitLibrary() )
		return false;

	if( !BindLibraryFunction() )
		return false;

	if( !CtgModuleInitialize() )
		return false;

	ProtectAutoKey();

	ProtectAutoMouse();

	ProtectInput();

	ProtectScreenCaptureGlobal();

	ProtectMemory();

	HideProcess( szProcess, szTitle );

	VIRTUALIZER_END

	return true;
}

bool CNMContegoManager::CheckRandomLoad( DWORD dwPercent )
{
//	return false;

	if( GetTickCount()%100 >= dwPercent )
		return false;

	return true;
}

bool CNMContegoManager::CtgModuleInitialize()
{
	VIRTUALIZER_START

	if( m_pfNMCtgInitialize == NULL )	return false;

	LONG lError = m_pfNMCtgInitialize( );

	if( lError != ERROR_SUCCESS )
	{
		// lError값 에러로그로 전송("0x00000000" 형태의 문자열로 전송)
		ThrowError( lError );

		return false;
	}

	VIRTUALIZER_END

	return true;
}

void CALLBACK CNMContegoManager::NMCtgInputNotifyProc( NMCTG_INPUT_TYPE eInputType, ULONG lValue )
{
	VIRTUALIZER_START

	switch( eInputType )
	{
		case NMCTG_KEYBOARD:
			break;

		case NMCTG_MOUSE:
			break;
	}

	VIRTUALIZER_END
}

void CALLBACK CNMContegoManager::NMCtgDetectNotifyProc( ULONG lDetectCode, LPCTSTR lpszImageName )
{
	VIRTUALIZER_START

	ThrowError( 0, lDetectCode, lpszImageName );

	VIRTUALIZER_END
}




void CNMContegoManager::NMCtgRelease()
{
	VIRTUALIZER_START

	gContegoThread().Destroy();

	if( m_hNMCtrlModule )
	{
		if( m_pfNMCtgRelease != NULL )
			m_pfNMCtgRelease();

		FreeLibrary( m_hNMCtrlModule );
		m_hNMCtrlModule = NULL;
	}	

	VIRTUALIZER_END
}

void CNMContegoManager::ThrowError( DWORD dwErrCode, DWORD dwDetectCode, LPCTSTR lpszImageName )
{
	CString errmsg;
	errmsg.Format("0x%08x %d %s", dwErrCode, dwDetectCode, lpszImageName);
	OutputDebugString(errmsg);

	VIRTUALIZER_START

	CONTEGODATA stData;
	stData.dwErrCode = dwErrCode;
	stData.dwDetectCode = dwDetectCode;
	ZeroMemory( stData.szImage, sizeof(stData.szImage) );
	if( lpszImageName != NULL )	wsprintf( stData.szImage, lpszImageName );

	gContegoThread().AddContegoData( &stData );	

	VIRTUALIZER_END
}


// #include <atlstr.h> 
// 
// CString PrintErrorMessage(DWORD err)
// {
// 	LPTSTR lpMsgBuf;
// 	CString msg;
// 	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER   // 에러코드 해석 함수
// 		| FORMAT_MESSAGE_FROM_SYSTEM
// 		| FORMAT_MESSAGE_IGNORE_INSERTS,
// 		NULL, err,
// 		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
// 		(LPTSTR)&lpMsgBuf, 0, NULL);
// 	msg = lpMsgBuf;
// 	LocalFree(lpMsgBuf);
// 	return msg;
// }
// 
// 사용법 : MessageBox(NULL, PrintErrorMessage(GetLastError()), "Error", MB_OK);
