// GFUtil.h: interface for the CGFUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GFUTIL_H__900A3D88_90BE_4508_9A6D_120ED098B69E__INCLUDED_)
#define AFX_GFUTIL_H__900A3D88_90BE_4508_9A6D_120ED098B69E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UtilDefine.h"
#include <map>
using namespace std;

namespace NMBASE
{
	namespace UTIL
	{

		#define	_NMID(a)	RemoveSitePrefix(a)

		enum ENUM_SITENO
		{
			SNO_NOINIT		= -1,	// Not initialized
			SNO_NETMARBLE	= 0,	// 넷마블	
			SNO_HANAFOS		= 3,	// 하나포스	
			SNO_GMARKET		= 10,	// 지마켓
			SNO_AUCTION		= 20,	// 옥션
			SNO_NATE		= 30,	// 네이트
			SNO_TOCTOC		= 55,	// 톡톡게임

			//SNO_MAX		= 4,
		};

		enum ENUM_SCUT_SITE
		{
			SCS_NEWPOKER = 0,
			SCS_CHAMPIONSHIP,
		};

		typedef struct
		{
			char szSite[64];
			char szHSite[64];
			char szPrefix[8];
			char szRankingURL[256];
			char szHomeURL[256];
			char szHelpURL[256];
		} NMSITEINFO;

		extern map<int, NMSITEINFO> g_SiteInfo;

		NMUTIL_API void	InitGFUtil();
		NMUTIL_API BOOL SetCurSiteNo_BySpareParam(LPCTSTR strCmndLine);
		NMUTIL_API BOOL SetCurSiteNo_ByCPParam(LPCTSTR strCPParam);

		NMUTIL_API void SetCurSiteNo(int nSNO);
		NMUTIL_API ENUM_SITENO GetCurSiteNo();
		NMUTIL_API char* GetCurSiteName();
		NMUTIL_API char* GetCurSitePrefix();
		NMUTIL_API char* GetCurHSiteName();
		NMUTIL_API char *GetCurRankingURL();
		NMUTIL_API char *GetCurHomeURL();
		NMUTIL_API char *GetCurHelpURL();
		NMUTIL_API void SetCurSiteNo_ByID(char *szID);
		NMUTIL_API ENUM_SITENO GetSiteNoFromID(const char *szID);
		NMUTIL_API char* GetHSiteName(ENUM_SITENO SNO);
		NMUTIL_API char *RemoveSitePrefix(const char *szID);
		NMUTIL_API BOOL AddSitePrefix(ENUM_SITENO SNO, char *szID, char *szBuf, int nBufSize);
		// 스페어 파라미터로부터 그룹번호를 파싱(-1을 리턴하면 없음)
		NMUTIL_API int GetGroupNo_BySpareParam(LPCTSTR strSpareParam);
		NMUTIL_API int Get_BySpareParam(LPCTSTR strSpareParam, const char* pArg );
		NMUTIL_API BOOL GetTokenValue(LPCTSTR szString, LPCTSTR szToken, char* szRtnVal);

		NMUTIL_API const char* GetExclusiveKey();

	}//namespace UTIL

}//namespace NMBASE



#endif // !defined(AFX_GFUTIL_H__900A3D88_90BE_4508_9A6D_120ED098B69E__INCLUDED_)
