// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE Ŭ�����Դϴ�.
#include <afxodlgs.h>       // MFC OLE ��ȭ ���� Ŭ�����Դϴ�.
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC �����ͺ��̽� Ŭ�����Դϴ�.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO �����ͺ��̽� Ŭ�����Դϴ�.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>					// Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>            // MFC ���� Ȯ���Դϴ�.

#define _UPG_GAME_DLL

#include <deque>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <list>
using namespace std;

//#define UPG_PLATFORM_EXPORT __declspec(dllimport)
#define UPG_COMMON_EXPORT   __declspec(dllimport)
#define UPG_GAMEBASE_EXPORT	__declspec(dllimport)
#define UPG_GAME_EXPORT		__declspec(dllexport)

#include "SockInclude.h"		///< Sock		DLL
#include "UtilInclude.h"		///< Util		DLL
#include "SoundInclude.h"		///< Sound		DLL
#include "GraphicGDIInclude.h"	///< GraphicGDI	DLL
#include "AvatarInclude.h"		///< Avatar		DLL
#include "SkinGDIInclude.h"		///< SkinGDI	DLL

//import NMPlatform
//#include <UPGPlatform.h>
#include <UPGCommon.h>
#include <UPGPokerBase.h>


//#define TRACE_OUTPUT
#ifdef TRACE_OUTPUT
	#pragma comment (lib, "WSock32.Lib")
	#pragma comment (lib, "D:\\Project\\netmable\\PokerType\\outlib\\lib\\tracetool.lib")//"..\\..\\outlib\\lib\\tracetool.lib")
	#include "..\\..\\outlib\\include\\tracetool.h"		// Trace tool ( ��ȯ )
#endif


//#pragma comment (lib, "../../UPGCommon/lib/UPGCommon_D.lib")
//#include <BadFilterLoader.h>

#pragma comment (lib, "winmm.lib")

#pragma warning (disable:4018)

#define UPGRADE_20120308_TOCTOC_CHANNELING
// Ŭ���α� �߰� �۾�
#define UPGRADE_10210619_ADD_CLICKLOG