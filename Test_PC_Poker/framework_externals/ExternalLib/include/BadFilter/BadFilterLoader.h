#pragma once
#define WIN32_LEAN_AND_MEAN

//warning LNK4217
#ifndef	_BADFILTER_LOADER_
#define	_BADFILTER_LOADER_

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

//- fatal error LNK1103: debugging information corrupt; recompile module
//- http://cherami.maru.net/zbxe/info/4136
/* 에러 내용 검색해 보니 결론은 저희가 설치한 최신버전의 SDK가 문제가 있었던 것으로 결론이났습니다. 즉, 
Platform SDK February 2003
Full Down : http://www.microsoft.com/msdownload/platformsdk/sdkupdate/psdk-full.htm
까지만 지원된다네요..기존의 최신버전의 SDK를 제어판]->프로그램 추가및삭제에서 지워주시구요.
위 링크의 SDK를 다운받아 설치하니 정상적으로 디버깅이 가능했습니다..참고 하세요..^^
*/

enum BADFILTER_ERROR
{
	SUCCESS = 0,
	INITVALUE_FAIL = 1,		//- 초기화 변수 Error
	DLLLOAD_FAIL = 2,		//- Dll load error
	FUNCLOAD_FAIL = 3,		//- Dll Function Load error
	FILELOAD_FAIL = 4,		//- File Load error
};

#ifndef _MAKELIB_
#ifndef	_BADFILTER_LINK_
	#define _BADFILTER_LINK_
	#pragma message( "BadFilterLoader library Link" )

	#ifdef  _DEBUG

	#ifdef	_MT
		#ifdef	_DLL
			//#pragma comment(lib, "BadFilterLoader_MTd.lib")
			#pragma comment(lib, "BadFilterLoader_MDd.lib")
			#pragma message( "Export BadFilter/BadFilterLoader_MDd.lib MultiThread Dll Debug" )
		#else
			#pragma comment(lib, "BadFilterLoader_MTd.lib")
			#pragma message( "Export BadFilter/BadFilterLoader_MTd MultiThread Debug" )
		#endif
	#else
		#pragma comment(lib, "BadFilterLoader_MLd.lib")
		#pragma message( "Export BadFilter/BadFilterLoader_MLd SingleThread Debug" )
	#endif


	#else

	#ifdef	_MT
		#ifdef	_DLL
			#pragma comment(lib, "BadFilterLoader_MD.lib")
			#pragma message( "Export BadFilter/BadFilterLoader_MD MultiThread Dll" )
		#else
			#pragma comment(lib, "BadFilterLoader_MT.lib")
			#pragma message( "Export BadFilter/BadFilterLoader_MT.lib MultiThread" )
		#endif
	#else
		#pragma comment(lib, "BadFilterLoader_ML.lib")
		#pragma message( "Export BadFilter/BadFilterLoader_ML.lib SingleThread" )
	#endif

#endif


#endif	//_BADFILTER_LINK_
#endif	//_MAKELIB_

//- Common Function !!!

//- Filter Level 변경 ( 1번은 기존방식, 2번은 추가된 방식(권장) )
void				SetFilterLevel( int nLevel );
//- Filter 기능 사용여부
void				SetFilterEnable( BOOL bEnable );
BOOL				IsFilterEnable();


//////-------------------------------------------------------------------------//
//////------------------------ UNICODE FUNCTION -------------------------------//
//////_________________________________________________________________________//
// [9/30/2008 Bad Filter]
//- BadFilter 초기화 함수
//- shared라면, szDllName을  BadFilterSHD.dll로 할것
//- Static이라면, szDllName을  BadFilterSTD.dll로 할것
extern "C"	BADFILTER_ERROR		InitFilterW( wchar_t* szAvatar );
//extern "C"	BADFILTER_ERROR		InitFilter( wchar_t*  szAvatar );
//- Dll Path 변경 ( 해당 Path에는 Bad Filter File도 존재해야 한다. )
extern "C"	void				SetFilterDllPathW( wchar_t* szDllFullPath );
//- Filter File 변경 ( 경로와 함께 넣어주어야 한다. )
extern "C"	void				SetFilterFileW( wchar_t* szFilterFile );

//- Bad Word의 존재 여부를 Return
extern "C"	BOOL				IsBadW( wchar_t* str );
//- Bad Word를 rep로 대체 하는 함수 ( strOut으로 return )
extern "C"	BOOL				IsBadToRepW( wchar_t* str, wchar_t* strOut, DWORD nOut, wchar_t rep = '*' );


//////-------------------------------------------------------------------------//
//////------------------------ MULTIBYTE FUNCTION -------------------------------//
//////_________________________________________________________________________//
// Multibyte
	extern "C"	BADFILTER_ERROR		InitFilter( char* szAvatar );
	//- Dll Path 변경 ( 해당 Path에는 Bad Filter File도 존재해야 한다. )
	extern "C"	void				SetFilterDllPath( char* szDllFullPath );
	//- Filter File 변경 ( 경로와 함께 넣어주어야 한다. )
	extern "C"	void				SetFilterFile( char* szFilterFile );

	//- Bad Word의 존재 여부를 Return
	extern "C"	BOOL				IsBad( char* str );
	//- Bad Word를 rep로 대체 하는 함수 ( strOut으로 return )
	extern "C"	BOOL				IsBadToRep( char* str, char* strOut, DWORD nOut, char rep = '*' );


#ifdef UNICODE
	#pragma message( "It is a UNICODE Project" )
	#define		InitFilter			InitFilterW
	#define		SetFilterDllPath	SetFilterDllPathW
	#define		SetFilterFile		SetFilterFileW
	#define		IsBad				IsBadW
	#define		IsBadToRep			IsBadToRepW
#else
	#pragma message( "It is a MULTIBYTE Project" )
#endif

#endif	//- _BADFILTER_LOADER_


