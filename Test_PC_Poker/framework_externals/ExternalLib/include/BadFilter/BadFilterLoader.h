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
#pragma message("	�� ����� ����Ϸ��� 'wchar_t Ÿ���� �⺻ ���� �������� ó��' �ɼ��� ���� �Ѵ�.")
#pragma message("		1. ������Ʈ ���� ��ȭ���ڸ� ����. (ALT+F7)")
#pragma message("		2. '�������>C/C++>���' ī�װ��� ����.")
#pragma message("		3. 'wchar_t Ÿ���� �⺻ ���� �������� ó��' �ɼ��� '�ƴϿ� (/Zc:wchar_t)'���� ����")
#error Fatal error.
#endif

//- fatal error LNK1103: debugging information corrupt; recompile module
//- http://cherami.maru.net/zbxe/info/4136
/* ���� ���� �˻��� ���� ����� ���� ��ġ�� �ֽŹ����� SDK�� ������ �־��� ������ ����̳����ϴ�. ��, 
Platform SDK February 2003
Full Down : http://www.microsoft.com/msdownload/platformsdk/sdkupdate/psdk-full.htm
������ �����ȴٳ׿�..������ �ֽŹ����� SDK�� ������]->���α׷� �߰��׻������� �����ֽñ���.
�� ��ũ�� SDK�� �ٿ�޾� ��ġ�ϴ� ���������� ������� �����߽��ϴ�..���� �ϼ���..^^
*/

enum BADFILTER_ERROR
{
	SUCCESS = 0,
	INITVALUE_FAIL = 1,		//- �ʱ�ȭ ���� Error
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

//- Filter Level ���� ( 1���� �������, 2���� �߰��� ���(����) )
void				SetFilterLevel( int nLevel );
//- Filter ��� ��뿩��
void				SetFilterEnable( BOOL bEnable );
BOOL				IsFilterEnable();


//////-------------------------------------------------------------------------//
//////------------------------ UNICODE FUNCTION -------------------------------//
//////_________________________________________________________________________//
// [9/30/2008 Bad Filter]
//- BadFilter �ʱ�ȭ �Լ�
//- shared���, szDllName��  BadFilterSHD.dll�� �Ұ�
//- Static�̶��, szDllName��  BadFilterSTD.dll�� �Ұ�
extern "C"	BADFILTER_ERROR		InitFilterW( wchar_t* szAvatar );
//extern "C"	BADFILTER_ERROR		InitFilter( wchar_t*  szAvatar );
//- Dll Path ���� ( �ش� Path���� Bad Filter File�� �����ؾ� �Ѵ�. )
extern "C"	void				SetFilterDllPathW( wchar_t* szDllFullPath );
//- Filter File ���� ( ��ο� �Բ� �־��־�� �Ѵ�. )
extern "C"	void				SetFilterFileW( wchar_t* szFilterFile );

//- Bad Word�� ���� ���θ� Return
extern "C"	BOOL				IsBadW( wchar_t* str );
//- Bad Word�� rep�� ��ü �ϴ� �Լ� ( strOut���� return )
extern "C"	BOOL				IsBadToRepW( wchar_t* str, wchar_t* strOut, DWORD nOut, wchar_t rep = '*' );


//////-------------------------------------------------------------------------//
//////------------------------ MULTIBYTE FUNCTION -------------------------------//
//////_________________________________________________________________________//
// Multibyte
	extern "C"	BADFILTER_ERROR		InitFilter( char* szAvatar );
	//- Dll Path ���� ( �ش� Path���� Bad Filter File�� �����ؾ� �Ѵ�. )
	extern "C"	void				SetFilterDllPath( char* szDllFullPath );
	//- Filter File ���� ( ��ο� �Բ� �־��־�� �Ѵ�. )
	extern "C"	void				SetFilterFile( char* szFilterFile );

	//- Bad Word�� ���� ���θ� Return
	extern "C"	BOOL				IsBad( char* str );
	//- Bad Word�� rep�� ��ü �ϴ� �Լ� ( strOut���� return )
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


