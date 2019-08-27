// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 클래스입니다.
#include <afxodlgs.h>       // MFC OLE 대화 상자 클래스입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 데이터베이스 클래스입니다.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 데이터베이스 클래스입니다.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>					// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>            // MFC 소켓 확장입니다.

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
	#include "..\\..\\outlib\\include\\tracetool.h"		// Trace tool ( 종환 )
#endif


//#pragma comment (lib, "../../UPGCommon/lib/UPGCommon_D.lib")
//#include <BadFilterLoader.h>

#pragma comment (lib, "winmm.lib")

#pragma warning (disable:4018)

#define UPGRADE_20120308_TOCTOC_CHANNELING
// 클릭로그 추가 작업
#define UPGRADE_10210619_ADD_CLICKLOG