// ExHandle.h
//
//////////////////////////////////////////////////////////////////////
#ifndef _COMPILELIB
// 라이브러리 컴파일 중에는 이하는 컴파일 되지 않는다
//- Exception Handler 1.1 Version (2008.10.01)
//		1. Dump File 생성 기능 추가
//		2. Dump File을 report.exe와 연계하여 report 할수 있도록 구성(Patch System)
//		   report.exe는 multipart를 이용하여, ftp에 dump file을 upload하게 됩니다.
//		3. Dump File의 용량에 대하여, dump file의 압축 기능을 추가
//		4. Dump File에 client의 version 정보를 저장하도록 기능을 추가.
//- Exception Handler 1.2 Version (2009.03.01)
//		1. Force Parsing 기능 추가(stack을 dbghlp.dll을 이용하지 않고,
//		   자체 방식으로 parsing하여 windbg로 parsing 불가능한 dump file에서
//		   참고 정보를 흭득 할수 있도록 한다.
//		2. Module을 구분( Client, Server, Test(Tool) Module )하여
//		   상황에 맞는 오류 보고 작업을 진행
//		   Client Module: Report.exe를 이용하여 오류 보고
//		   Server Module: Local에 Dump File을 저장
//		   Test(Tool) Module: Local에 Dump File을 저장
//		3. Server Dump File 생성과 version 정보를 위해,
//		   Server Module시에 local의 version정보를 참조하도록 기능추가.
//		4. Crush Module을 따로 추출하여 저장하도록 기능 추가 (주 원인 확인을 위해)
//		5. EIP를 저장함으로써, 동일한 version의 오류에 대해서,
//		   동일 오류 판정이 가능하도록 기능추가.
//		6. UNICODE 지원
//- Exception Handler 1.3 Version (2009.03.01)
//		1. Stack Overflow에 대하여, dump file을 남기지 못하는 상황에 대하여
//		   "가상 stack" 기술을 이용하여, dump file을 생성하도록 제작
//		   Stack Guard page 해제 code 추가 및
//		   (해당 기술은 stack overflow 상황에 대해서 더 많은 test가 필요)
//		   Dr.watson은 해당 상황일때, dump를 남기지 못합니다.
//		2. server module에서 crush가 발생한 경우,
//		   Dr.watson이 계속해서 dump를 남기지 못하게 되는 bug 수정
//		3. 실행 환경에 따라, dump 생성이 되지 않는 bug 수정
//		4. Log Buffer의 용량 제한 초과시 발생 할수 있는 이중 crush의 가능성 수정
//		5. Crush Module에 대하여, stack에 대한 접근을 최소화 하기 위하여,
//		   중요 dump file 생성 부분을 thread 구성
#pragma once

#ifndef __MSJEXHND_H__
#define __MSJEXHND_H__
///////////////////////////////

#ifndef		_CRT_SECURE_NO_DEPRECATE
#define		_CRT_SECURE_NO_DEPRECATE
#endif

// 아래의 define을 설정하면 익셉션 핸들러 사용을 취소할 수 있다
 //#define DISABLE_EXHANDLE
#ifdef _ONLY_SERVER
#define DISABLE_EXHANDLE	// Do not use! server side	2011.02.15
#endif	// _ONLY_SERVER


#define		MAX_GAMECODE		128
#define		MAX_SITENAME		80

class CExHandle
{
public:
	enum 
	{
		EXHAN_UNKNOWN_MODULE = 0,
		EXHAN_CLIENT = 1,
		EXHAN_GAMESERVER = 2,
		EXHAN_MASTERSERVER = 3,
	};
	CExHandle();

	static void	SetExeptionInfo(TCHAR* szGameCode, TCHAR* szRegistryDir, int nKind);
	static void PresetSetSiteName(TCHAR* strSiteName);

protected:
	static BOOL	GT_CommonFolder(OUT TCHAR* szCommonDir, IN int nBufSize=MAX_PATH);
	static void	GT_ModuleName(OUT TCHAR* szModuleName);
	static BOOL	GT_Version(IN TCHAR* szRegistryDir, OUT int &nVersion);
	static BOOL	GT_ServerInfo(IN OUT TCHAR* szGameCode, OUT int &nVersion, OUT int &nKind);
	static void	GT_ReportFileName(IN TCHAR* szCommonDir, OUT TCHAR* szFileName);

	static void GenerateMulticode( wchar_t* szString, char* szData_In, int cbBufferSize );

public:
	static TCHAR s_strSiteName[MAX_SITENAME];
};

#ifdef UNICODE
	#pragma message( "It is a UNICODE Project" )
#else
	#pragma message( "It is a MULTIBYTE Project" )
#endif

extern CExHandle g_ExHandle;
//////////////////////////////
#endif


#endif