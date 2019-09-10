// ExHandle.h
//
//////////////////////////////////////////////////////////////////////
#ifndef _COMPILELIB
// ���̺귯�� ������ �߿��� ���ϴ� ������ ���� �ʴ´�
//- Exception Handler 1.1 Version (2008.10.01)
//		1. Dump File ���� ��� �߰�
//		2. Dump File�� report.exe�� �����Ͽ� report �Ҽ� �ֵ��� ����(Patch System)
//		   report.exe�� multipart�� �̿��Ͽ�, ftp�� dump file�� upload�ϰ� �˴ϴ�.
//		3. Dump File�� �뷮�� ���Ͽ�, dump file�� ���� ����� �߰�
//		4. Dump File�� client�� version ������ �����ϵ��� ����� �߰�.
//- Exception Handler 1.2 Version (2009.03.01)
//		1. Force Parsing ��� �߰�(stack�� dbghlp.dll�� �̿����� �ʰ�,
//		   ��ü ������� parsing�Ͽ� windbg�� parsing �Ұ����� dump file����
//		   ���� ������ ŉ�� �Ҽ� �ֵ��� �Ѵ�.
//		2. Module�� ����( Client, Server, Test(Tool) Module )�Ͽ�
//		   ��Ȳ�� �´� ���� ���� �۾��� ����
//		   Client Module: Report.exe�� �̿��Ͽ� ���� ����
//		   Server Module: Local�� Dump File�� ����
//		   Test(Tool) Module: Local�� Dump File�� ����
//		3. Server Dump File ������ version ������ ����,
//		   Server Module�ÿ� local�� version������ �����ϵ��� ����߰�.
//		4. Crush Module�� ���� �����Ͽ� �����ϵ��� ��� �߰� (�� ���� Ȯ���� ����)
//		5. EIP�� ���������ν�, ������ version�� ������ ���ؼ�,
//		   ���� ���� ������ �����ϵ��� ����߰�.
//		6. UNICODE ����
//- Exception Handler 1.3 Version (2009.03.01)
//		1. Stack Overflow�� ���Ͽ�, dump file�� ������ ���ϴ� ��Ȳ�� ���Ͽ�
//		   "���� stack" ����� �̿��Ͽ�, dump file�� �����ϵ��� ����
//		   Stack Guard page ���� code �߰� ��
//		   (�ش� ����� stack overflow ��Ȳ�� ���ؼ� �� ���� test�� �ʿ�)
//		   Dr.watson�� �ش� ��Ȳ�϶�, dump�� ������ ���մϴ�.
//		2. server module���� crush�� �߻��� ���,
//		   Dr.watson�� ����ؼ� dump�� ������ ���ϰ� �Ǵ� bug ����
//		3. ���� ȯ�濡 ����, dump ������ ���� �ʴ� bug ����
//		4. Log Buffer�� �뷮 ���� �ʰ��� �߻� �Ҽ� �ִ� ���� crush�� ���ɼ� ����
//		5. Crush Module�� ���Ͽ�, stack�� ���� ������ �ּ�ȭ �ϱ� ���Ͽ�,
//		   �߿� dump file ���� �κ��� thread ����
#pragma once

#ifndef __MSJEXHND_H__
#define __MSJEXHND_H__
///////////////////////////////

#ifndef		_CRT_SECURE_NO_DEPRECATE
#define		_CRT_SECURE_NO_DEPRECATE
#endif

// �Ʒ��� define�� �����ϸ� �ͼ��� �ڵ鷯 ����� ����� �� �ִ�
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