// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>            // MFC socket extensions


#define _UPG_GAME_DLL

#include <deque>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <list>
using namespace std;

//#define UPG_PLATFORM_EXPORT		__declspec(dllimport)
#define UPG_COMMON_EXPORT		__declspec(dllimport)
#define UPG_GAME_EXPORT			__declspec(dllexport)
#define UPG_GAMEBASE_EXPORT		__declspec(dllimport)

#include "SockInclude.h"		///< Sock		DLL
#include "UtilInclude.h"		///< Util		DLL
#include "SoundInclude.h"		///< Sound		DLL
#include "GraphicGDIInclude.h"	///< GraphicGDI	DLL
#include "AvatarInclude.h"		///< Avatar		DLL
#include "SkinGDIInclude.h"		///< SkinGDI	DLL

//#include <UPGPlatform.h>
#include <UPGCommon.h>
#include <UPGPokerBase.h>
#include "GlobalGame.h"

#pragma comment (lib, "winmm.lib")

#define UPGRADE_20120308_TOCTOC_CHANNELING
//#define UPGRADE_20120502_TIMING_TEST_CODE
// 클릭로그 추가 작업
#define UPGRADE_10210619_ADD_CLICKLOG