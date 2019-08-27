//-----------------------------------------------------------------------------//
// FileName	: ExHandle.cpp
// Author	: 
// Update	: 2008-05-22 12시
// Desc		: Mini Dump 추가
//-----------------------------------------------------------------------------//
#include "stdafx.h"
#include <atlbase.h>
#include "ExHandle.h"

// 라이브러리 컴파일 중에는 이하는 컴파일 되지 않는다
#ifndef _COMPILELIB

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CExHandle g_ExHandle;

#ifndef DISABLE_EXHANDLE

	// 링커 옵션에 "Generate debug info" 옵션을 설정하도록 알려준다(함수 콜스택을 남기기 위함)
	#pragma message( "================================ NOTICE BY ExHandle Lib ==============================" )
	#pragma message( "  * SET LINKER OPTION FOR '/DEBUG' and '/debugtype:both'" )
	#pragma message( "    1. Turn on 'Generate debug info' At Project->Setting->Link(Tab)->General(Category)" )
	#pragma message( "    2. Select 'Debug Info->Program database' At Project->Setting->C/C++(Tab)->General(Category)" )
	#pragma message( "    3. Select 'Both formats' At Project->Setting->Link(Tab)->Debug(Category)" )
	#pragma message( "    If you do not turn on /DEBUG option, linking will be failed!" )
	#pragma message( "  > FOR KOREAN: 반드시 아래의 두 옵션을 활성화 할 것!" )
	#pragma message( "    1. 'Generate debug info' 옵션을 체크할 것(Project->Setting->Link(Tab)->General(Category))" )
	#pragma message( "    2. 'Debug Info->Program database' 옵션을 선택할 것(Project->Setting->C/C++(Tab)->General(Category))" )
	#pragma message( "    3. 'Both formats' 옵션을 선택할 것(Project->Setting->Link(Tab)->Debug(Category))" )
	#pragma message( "======================================================================================" )

	// 이유는 모르겠지만 메뉴에서 직접 Generate debug info을 설정하지 않으면 링크 에러가 발생한다
	// ( 아래의 옵션들은 프로젝트 세팅 메뉴에서 직접 설정하지 않으면 적용되지 않는 듯하다 )
	#pragma comment(linker, "/DEBUG")
	//#pragma comment(linker, "/debugtype:cv")
	//#pragma comment(linker, "/debugtype:both")	// 디버그 정보를 Both format으로 설정(기본값은 Microsoft format)
#endif

/*
#if defined(_NATIVE_WCHAR_T_DEFINED)
#pragma message( "================================ Error with native type wchar_t ==============================" )
#pragma message("	For english : ")
#pragma message("	Using this module, Turn off option 'the Treat wchar_t as Built-in Type'.")
#pragma message("		1. Open Project Setting Dialog (ALT+F7)")
#pragma message("		2. Select Category 'Property Pages>C/C++>Language'")
#pragma message("		3. Modify \"the Treat wchar_t as Built-in Type\" value with 'No (/Zc:wchar_t)")
#pragma message("	For korean : ")
#pragma message("	이 모듈을 사용하려면 'wchar_t 타입을 기본 제공 형식으로 처리' 옵션을 꺼야 한다.")
#pragma message("		1. 프로젝트 설정 대화상자를 연다. (ALT+F7)")
#pragma message("		2. '구성요소>C/C++>언어' 카테고리를 선택.")
#pragma message("		3. 'wchar_t 타입을 기본 제공 형식으로 처리' 옵션을 '아니오 (/Zc:wchar_t)'으로 수정")
#error Fatal error.
#endif
*/
#ifdef  _DEBUG

#ifdef	_MT
	#ifdef	_DLL
		#pragma comment(lib, "ExHandle_MDd.lib")
		#pragma message( "Export ExHandle_MDd.lib MultiThread Dll Debug" )
	#else
		#pragma comment(lib, "ExHandle_MTd.lib")
		#pragma message( "Export ExHandle_MTd.lib MultiThread Debug" )
	#endif
#else
	#pragma comment(lib, "ExHandle_MLd.lib")
	#pragma message( "Export ExHandle_MLd.lib SingleThread Debug" )
#endif


#else

#ifdef	_MT
	#ifdef	_DLL
		#pragma comment(lib, "ExHandle_MD.lib")
		#pragma message( "Export ExHandle_MD.lib MultiThread Dll" )
	#else
		#pragma comment(lib, "ExHandle_MT.lib")
		#pragma message( "Export ExHandle_MT.lib MultiThread" )
	#endif
#else
	#pragma comment(lib, "ExHandle_ML.lib")
	#pragma message( "Export ExHandle_ML.lib SingleThread" )
#endif

#endif

#pragma warning( disable : 4267 )
#pragma warning( disable : 4005 )
#ifndef STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_DEPRECATE
#endif
#pragma warning( disable : 4996 )

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////      Extern Function        //////////////////////////
/////////////////////////////////////////////////////////////////////////////////
extern void ExHnd_Disable();
//- Multibyte Function
extern "C" void ExHnd_ExceptionInfo(char* szReportName, char* szGameCode, int nVersion, int nKind);
extern "C" void ExHnd_SetCommonDir(char* szCommonDir);

//- Unicode Function
extern "C" void ExHnd_ExceptionInfoW(wchar_t *szReportName, wchar_t *szGameCode, int nVersion, int nKind);
extern "C" void ExHnd_SetCommonDirW(wchar_t* szCommonDir);

/////////////////////////////////////////////////////////////////////////////////

TCHAR CExHandle::s_strSiteName[MAX_SITENAME] = {0,};


void CExHandle::GenerateMulticode( wchar_t* szString, char* szData_In, int cbBufferSize )
{
	int iNeedByte = ::WideCharToMultiByte( 949, 0, szString, -1, NULL, 0, NULL, NULL );

	if ( cbBufferSize != -1 )
	{
		if ( iNeedByte > cbBufferSize )
			iNeedByte = cbBufferSize;
	}

	if ( iNeedByte > 0 )
	{
		::WideCharToMultiByte( 949, 0, szString, -1, szData_In, iNeedByte, NULL, NULL);
	}
}


//- Create에서 자동으로 실행되는 함수
//- Report 실행 화일 이름을 Registry로 부터 얻는다.
//- Game문자 Code를 입력받는다. (Default: 실행 File 이름)
//- Program의 Kind(종류)를 얻는다.(Default: EXHAN_CLIENT )
//- Registry 경로를 입력받는다. (Default: 실행 File 이름)

CExHandle::CExHandle()
{
#ifndef DISABLE_EXHANDLE
	SetExeptionInfo( _T(""), _T(""), EXHAN_UNKNOWN_MODULE);
#else
	ExHnd_Disable();
#endif
}


#if _MSC_VER >= 1300
	//- lorddan( 2009.06.05) - VC .net의 CRT 함수의 SetUnhandledExceptionFilter(NULL)을
	//- 호출하지 못하도록, Signal을 따로 처리하기 위한 함수.

	void __cdecl invalid_parameter_handler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t)
	{
		//_asm int 3;
	}

	void __cdecl PurecallHandler(void)
	{
	}

	int __cdecl NewHandler( size_t )
	{
		return  1;
	}
#endif

void	CExHandle::SetExeptionInfo(TCHAR* szTGameCode, TCHAR* szRegistryDir, int nKind)
{
#ifdef DISABLE_EXHANDLE
	return;
#endif

#if _MSC_VER >= 1300
	//- lorddan( 2009.06.05) - VC .net의 CRT 함수의 SetUnhandledExceptionFilter(NULL)을
	//- 호출하지 못하도록, Signal을 따로 처리하기 위한 함수.

	//- CRT Function Invalid Parameter Handler
	_set_invalid_parameter_handler(invalid_parameter_handler);
	//- Virtual Call Exception Handler
	_set_purecall_handler(PurecallHandler);
	//- Memory Allocation Exception Handler
	_set_new_handler( NewHandler );
#endif
	
	int nVersion = -1;
	
	//- Get Module Name
	TCHAR szModuleName[MAX_PATH];
	GT_ModuleName(szModuleName);

	TCHAR szGameCode[MAX_GAMECODE] = {0,};
	memcpy( szGameCode, szTGameCode, MAX_GAMECODE );

	//- Netmarble Registry Open
	CRegKey cRegistry;
	if(ERROR_SUCCESS != cRegistry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Netmarble") ))
	{
		switch( nKind )
		{
		case EXHAN_GAMESERVER:
		case EXHAN_MASTERSERVER:
			GT_ServerInfo( szGameCode, nVersion, nKind );
			break;
		default:
			nKind = EXHAN_UNKNOWN_MODULE;
			GT_ServerInfo( szGameCode, nVersion, nKind );
			break;
		}
		//- Netmarble이 설치되어 있지 않다.
#ifdef _UNICODE
		ExHnd_ExceptionInfoW(NULL, szGameCode, nVersion, nKind);
		ExHnd_SetCommonDirW(NULL);
#else
		ExHnd_ExceptionInfo(NULL, szGameCode, nVersion, nKind);
		ExHnd_SetCommonDir(NULL);
#endif
		return;
	}

	//- Common Folder 얻기
	DWORD dwSize	= MAX_PATH-1;
	TCHAR szNetmarbleFolder[MAX_PATH] = {0,};
	TCHAR szCommonDir[MAX_PATH] = {0,};
	TCHAR szReportFileName[MAX_PATH] = {0,};

#if _MSC_VER >= 1300
	if(_tcsncmp(s_strSiteName, _T("daum"), MAX_SITENAME) == 0) {
		if(ERROR_SUCCESS != cRegistry.QueryStringValue( _T("DaumGamePath"), szNetmarbleFolder, &dwSize)) 
		{
			cRegistry.Close();
			return;
		}
	}
	else if(_tcsncmp(s_strSiteName, _T("hanafos"), MAX_SITENAME) == 0) {
		if(ERROR_SUCCESS != cRegistry.QueryStringValue( _T("HanafosGamePath"), szNetmarbleFolder, &dwSize)) 
		{
			cRegistry.Close();
			return;
		}
	}
	else {
		if(ERROR_SUCCESS != cRegistry.QueryStringValue( _T("DefaultPath"), szNetmarbleFolder, &dwSize)) 
		{
		//- Netmarble이 설치되어 있지 않다.
#ifdef _UNICODE
		ExHnd_ExceptionInfoW(NULL, szGameCode, nVersion, nKind);
		ExHnd_SetCommonDirW(NULL);
#else
		ExHnd_ExceptionInfo(NULL, szGameCode, nVersion, nKind);
		ExHnd_SetCommonDir(NULL);
#endif
			cRegistry.Close();
			return;
		}
	}
#else	// VS 6.0 ++
	if(_tcsncmp(s_strSiteName, _T("daum"), MAX_SITENAME ) == 0) {
		if(ERROR_SUCCESS != cRegistry.QueryValue( szNetmarbleFolder, _T("DaumGamePath"), &dwSize)) 
		{
			cRegistry.Close();
			return;
		}
	}
	else if(_tcsncmp(s_strSiteName, _T("hanafos"), MAX_SITENAME ) == 0) {
		if(ERROR_SUCCESS != cRegistry.QueryValue(szNetmarbleFolder, _T("HanafosGamePath"), &dwSize)) 
		{
			cRegistry.Close();
			return;
		}
	}
	else {
		if(ERROR_SUCCESS != cRegistry.QueryValue(szNetmarbleFolder, _T("DefaultPath"), &dwSize)) 
		{
		//- Netmarble이 설치되어 있지 않다.
#ifdef _UNICODE
		ExHnd_ExceptionInfoW(NULL, szGameCode, nVersion, nKind);
		ExHnd_SetCommonDirW(NULL);
#else
		ExHnd_ExceptionInfo(NULL, szGameCode, nVersion, nKind);
		ExHnd_SetCommonDir(NULL);
#endif
			cRegistry.Close();
			return;
		}
	}
#endif

	cRegistry.Close();
	_stprintf(szCommonDir, _T("%s\\Common"), szNetmarbleFolder);

//- Report 실행 화일 이름을 Registry로 부터 얻는다.
	GT_ReportFileName( szCommonDir, szReportFileName );

//- Game문자 Code를 입력받는다. (Default: 실행 File 이름)
//- Netmarble Game의 Version을 얻는다.
//- Program의 Kind(종류)를 얻는다.(Default: EXHAN_CLIENT )
//- Registry 경로를 입력받는다. (Default: 실행 File 이름)
	switch( nKind )
	{
	case EXHAN_GAMESERVER:
	case EXHAN_MASTERSERVER:
		GT_ServerInfo( szGameCode, nVersion, nKind );
		break;

	case EXHAN_CLIENT:
		GT_Version(szRegistryDir, nVersion);
		break;

	default:
		nKind = EXHAN_UNKNOWN_MODULE;
		GT_ServerInfo( szGameCode, nVersion, nKind );
		break;
	}

#ifdef _UNICODE
		ExHnd_ExceptionInfoW(szReportFileName, szGameCode, nVersion, nKind);
		ExHnd_SetCommonDirW(szCommonDir);
#else
		ExHnd_ExceptionInfo(szReportFileName, szGameCode, nVersion, nKind);
		ExHnd_SetCommonDir(szCommonDir);
#endif
}

void	CExHandle::PresetSetSiteName(TCHAR* strSiteName)
{
	_tcsncpy(s_strSiteName, strSiteName, MAX_SITENAME );
}



//- Registry에서 Netmarble Common Folder를 얻는다.
BOOL	CExHandle::GT_CommonFolder(TCHAR* szCommonDir, int nBufSize)
{
	if( !szCommonDir )
		return FALSE;

	// 특정 위치에 기록된 넷마블 기본 폴더를 얻는다. 
	CRegKey reg;
	if(ERROR_SUCCESS != reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Netmarble") ))
		return FALSE;

	DWORD strsize = 1024-1;
	TCHAR strDefPath[1024] = {0,};

#if _MSC_VER >= 1300
	if(_tcsncmp(s_strSiteName, _T("daum"), MAX_SITENAME) == 0) {
		if(ERROR_SUCCESS != reg.QueryStringValue( _T("DaumGamePath"), strDefPath, &strsize)) 
		{
			reg.Close();
			return FALSE;
		}
	}
	else if(_tcsncmp(s_strSiteName, _T("hanafos"), MAX_SITENAME) == 0) {
		if(ERROR_SUCCESS != reg.QueryStringValue( _T("HanafosGamePath"), strDefPath, &strsize)) 
		{
			reg.Close();
			return FALSE;
		}
	}
	else {
		if(ERROR_SUCCESS != reg.QueryStringValue( _T("DefaultPath"), strDefPath, &strsize)) 
		{
			reg.Close();
			return FALSE;
		}
	}

#else	// VS 6.0 ++
	if(_tcsncmp(s_strSiteName, _T("daum"), MAX_SITENAME ) == 0) {
		if(ERROR_SUCCESS != reg.QueryValue( strDefPath, _T("DaumGamePath"), &strsize)) 
		{
			reg.Close();
			return FALSE;
		}
	}
	else if(_tcsncmp(s_strSiteName, _T("hanafos"), MAX_SITENAME ) == 0) {
		if(ERROR_SUCCESS != reg.QueryValue(strDefPath, _T("HanafosGamePath"), &strsize)) 
		{
			reg.Close();
			return FALSE;
		}
	}
	else {
		if(ERROR_SUCCESS != reg.QueryValue(strDefPath, _T("DefaultPath"), &strsize)) 
		{
			reg.Close();
			return FALSE;
		}
	}
#endif

	TCHAR strBaseDir[1024] = {0,};
	_stprintf( strBaseDir, _T("%s\\Common"), strDefPath);

	memset(szCommonDir, 0x00, nBufSize*sizeof(TCHAR));
	_tcsncpy(szCommonDir, strBaseDir, nBufSize);
	return TRUE;
}


void	CExHandle::GT_ModuleName(TCHAR* szModuleName)
{
	TCHAR szModule[MAX_PATH] = { 0,};
 	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName((HMODULE)hModule, szModule, sizeof(szModule));
	
	_tcsncpy( szModuleName, _tcsrchr( szModule, '\\' )+1, MAX_PATH );
	_tcstok( szModuleName, _T(".") );
}


BOOL	CExHandle::GT_Version(TCHAR* szRegistryDir, int &nVersion)
{
	CRegKey cRegKey;

	nVersion = -1;
	if( !szRegistryDir )
		return FALSE;
	
	TCHAR szOpenKey[MAX_PATH] = {0,};
	_stprintf( szOpenKey, _T("SOFTWARE\\Netmarble\\%s"), szRegistryDir );
	if ( ERROR_SUCCESS != cRegKey.Open(HKEY_LOCAL_MACHINE, szOpenKey ))
		return FALSE;

	//- Version 정보를 얻는다.
	TCHAR szVersion[128] = {0,};
	DWORD dwSize = 128;
#if _MSC_VER >= 1300
	if ( ERROR_SUCCESS != cRegKey.QueryStringValue( _T("Version"), szVersion, &dwSize)) 
#else
	if ( ERROR_SUCCESS != cRegKey.QueryValue( szVersion, _T("Version"), &dwSize)) 
#endif
	{
		cRegKey.Close();
		return FALSE;
	}

	nVersion = _ttoi(szVersion);
	cRegKey.Close();
	return TRUE;
}


BOOL	CExHandle::GT_ServerInfo(TCHAR* szGameCode, int &nVersion, int &nKind)
{
	TCHAR szModule[MAX_PATH];
 	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName((HMODULE)hModule, szModule, sizeof(szModule));

	// Module Name
	TCHAR szModName[MAX_PATH];
	_tcsncpy( szModName, _tcsrchr( szModule, '\\' )+1, MAX_PATH );
	
	// Module Path
	TCHAR szModPath[MAX_PATH];
	int nLen = (int)_tcslen(szModule)-(int)_tcslen(szModName);
	_tcsncpy( szModPath, szModule, nLen );
	szModPath[nLen] = '\0';

	//- Version File Path
	TCHAR *pVersionFile = NULL;
	TCHAR szVersionFile[MAX_PATH];
	_tcsncpy( szVersionFile, szModName, MAX_PATH );
	pVersionFile = _tcsrchr( szVersionFile, '.' )+1;

	if( pVersionFile == NULL )
		return FALSE;
	if( pVersionFile == NULL )
		return FALSE;

	pVersionFile[0] = 'v';
	pVersionFile[1] = 'e';
	pVersionFile[2] = 'r';
		
	FILE* fp = NULL;
	short nTKind = 0;
	TCHAR szTBuf[MAX_PATH];
	wchar_t	szUniGameCode[MAX_PATH];
	memset( szUniGameCode, 0x00, sizeof(wchar_t)*MAX_PATH );
	
	_stprintf( szTBuf, _T("%s\\%s"), szModPath, szVersionFile );
	fp = _tfopen( szTBuf, _T("r") );
	if ( fp )
	{
		//- version file 속성
		short nTVersion = 0;
		fread( &nTVersion,		1, sizeof(short), fp );
		fread( &nTKind,			1, sizeof(short), fp );
		fread( szGameCode,		MAX_GAMECODE, sizeof(char), fp );
		fclose(fp);

		nVersion	= nTVersion;
		nKind		= nTKind;
		return TRUE;
	}

	return FALSE;
}

void	CExHandle::GT_ReportFileName(TCHAR* szCommonDir, TCHAR* szFileName)
{
	_stprintf(szFileName, _T("%s\\ExhReport.exe"), szCommonDir);
}

#endif




