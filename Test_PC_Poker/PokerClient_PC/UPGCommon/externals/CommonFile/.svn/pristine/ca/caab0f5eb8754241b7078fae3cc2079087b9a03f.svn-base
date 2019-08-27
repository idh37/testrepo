// GFUtil.cpp: implementation of the CGFUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GFUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static ENUM_SITENO g_eSiteNo = SNO_NETMARBLE;
static char g_szExclusiveKey[256] = {0, };
map<int, NMSITEINFO> g_SiteInfo;

void InitGFUtil()
{
	g_SiteInfo.clear();
	//////////////////////////////////////////////////////////////////////////
	// 싸이트이름, 한글 싸이트 이름, 프리픽스, 랭킹 주소, 홈 주소, HELP 주소
	{
		NMSITEINFO stNmSiteInfo = 
		{
			"netmarble",
			"넷마블",
			"",
			"http://www.netmarble.net/game/baduk/rank/gm_rank.asp",
			"http://baduk.netmarble.net/",
			"http://www.netmarble.net/game/baduk/help/index.asp?menu=1"
		};

		g_SiteInfo.insert(pair<int, NMSITEINFO>( SNO_NETMARBLE, stNmSiteInfo));

	}

	//////////////////////////////////////////////////////////////////////////
	{
		NMSITEINFO stNmSiteInfo = 
		{
			"nate",
			"네이트게임",
			"",
			"",
			"",
			""
		};

		g_SiteInfo.insert(pair<int, NMSITEINFO>( SNO_NATE, stNmSiteInfo));
	}

	//< CYS 120227
	//////////////////////////////////////////////////////////////////////////
	{
		NMSITEINFO stNmSiteInfo = 
		{
			"toctoc",
			"톡톡게임",
			"",
			"",
			"",
			""
		};

		g_SiteInfo.insert( pair< int, NMSITEINFO >( SNO_TOCTOC, stNmSiteInfo ));
	}
	//> CYS 120227

}


static BOOL GetTokenValue(LPCTSTR szString, LPCTSTR szToken, char* szRtnVal)
{
	char *pstr = (char*)szString;

	char str_token[1024] = {0,};
	wsprintf(str_token, "%s=", szToken);
	int toklen = strlen(str_token);

	for(;*pstr; pstr++)
	{
		if(strnicmp(pstr, str_token, toklen) == 0) {
			pstr += toklen;
			char *pnow = pstr;
			int slen = 0;
			for(;; pnow++)
			{
				if(*pnow == '|' || *pnow == 0) {
					strncpy(szRtnVal, pstr, slen);
					szRtnVal[slen] = 0;
					return TRUE;
				}

				slen++;
			}
		}
	}

	return FALSE;
}

BOOL SetCurSiteNo_BySpareParam(LPCTSTR strSpareParam)
{
	char strSiteName[1024] = {0,};

	g_eSiteNo = SNO_NETMARBLE;

	GetTokenValue(strSpareParam, "site", strSiteName);


	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.begin();
	for( ; iter != g_SiteInfo.end() ; iter++ )
	{
		if( stricmp(iter->second.szSite, strSiteName) == 0 )
			break;
	}

	if( iter == g_SiteInfo.end() )
	{
		g_eSiteNo = SNO_NETMARBLE;
	}
	else
	{
		g_eSiteNo = (ENUM_SITENO)iter->first;
	}

	strSiteName[0] = '\0';
	GetTokenValue(strSpareParam, "exclusivekey", strSiteName);
	strncpy_s(g_szExclusiveKey, strSiteName, sizeof(g_szExclusiveKey)-1);

	return TRUE;
}

void SetCurSiteNo_ByID(char *szID)
{
	g_eSiteNo = GetSiteNoFromID(szID);
}

void SetCurSiteNo(int nSNO)
{
	g_eSiteNo = (ENUM_SITENO)nSNO;
}

ENUM_SITENO GetCurSiteNo()
{
	if(g_eSiteNo == SNO_NOINIT) {
		ASSERT(!("site name is not initialized."));
	}

	return g_eSiteNo;
}

char* GetCurSiteName()
{

	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(g_eSiteNo);
			
	if( g_eSiteNo == SNO_NOINIT ||
		iter == g_SiteInfo.end() )
	{
		ASSERT(!("site name is not initialized."));
		return NULL;
	}

	return iter->second.szSite;
}

char* GetCurHSiteName()
{
	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(g_eSiteNo);
			
	if( g_eSiteNo == SNO_NOINIT ||
		iter == g_SiteInfo.end() )
	{
		ASSERT(!("site name is not initialized."));
		return NULL;
	}

	return iter->second.szHSite;
}

ENUM_SITENO GetSiteNoFromID(const char *szID)
{
	char szBuf[8] = {0, };
	char *pPrefix = szBuf;
	char *pID = const_cast<char*>(szID);

	if (*pID != '#') return SNO_NETMARBLE;

	int i = 0;
	for (i = 0; *pID && *pID != '_' && i < 7; i++)
		*pPrefix++ = *pID++;

	if (i < 7 && *pID == '_')
		*pPrefix = *pID;


	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.begin();
	for( ; iter != g_SiteInfo.end() ; iter++ )
	{
		if( stricmp(iter->second.szPrefix, szBuf) == 0 )
		{
			return (ENUM_SITENO)iter->first;
		}
	}

	return SNO_NETMARBLE;
}

char* GetHSiteName(ENUM_SITENO SNO)
{
	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(g_eSiteNo);
			
	if( g_eSiteNo == SNO_NOINIT ||
		iter == g_SiteInfo.end() )
	{
		ASSERT(!("site name is not initialized."));
		return NULL;
	}

	return iter->second.szHSite;
}

char* GetCurSitePrefix()
{
	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(g_eSiteNo);
			
	if( g_eSiteNo == SNO_NOINIT ||
		iter == g_SiteInfo.end() )
	{
		ASSERT(!("site name is not initialized."));
		return NULL;
	}

	return iter->second.szPrefix;
}

char *GetCurRankingURL()
{
	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(g_eSiteNo);
			
	if( g_eSiteNo == SNO_NOINIT ||
		iter == g_SiteInfo.end() )
	{
		ASSERT(!("site name is not initialized."));
		return NULL;
	}

	return iter->second.szRankingURL;
}

char *GetCurHomeURL()
{
	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(g_eSiteNo);
			
	if( g_eSiteNo == SNO_NOINIT ||
		iter == g_SiteInfo.end() )
	{
		ASSERT(!("site name is not initialized."));
		return NULL;
	}

	return iter->second.szHomeURL;
}

char *GetCurHelpURL()
{
	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(g_eSiteNo);
			
	if( g_eSiteNo == SNO_NOINIT ||
		iter == g_SiteInfo.end() )
	{
		ASSERT(!("site name is not initialized."));
		return NULL;
	}

	return iter->second.szHelpURL;
}

const char* GetExclusiveKey()
{
	return g_szExclusiveKey;
}

char *RemoveSitePrefix(const char *szID)
{
	ENUM_SITENO SNO = GetSiteNoFromID(szID);
	char *psID = const_cast<char*>(szID);
	
	if (SNO > SNO_NETMARBLE)
	{
		map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(SNO);
		if( iter == g_SiteInfo.end() )
		{
			ASSERT(!("site name is not initialized."));
			return NULL;
		}

		return psID + strlen(iter->second.szPrefix);
	}

	return psID;
}

BOOL AddSitePrefix(ENUM_SITENO SNO, char *szID, char *szBuf, int nBufSize)
{
	map<int, NMSITEINFO>::iterator iter = g_SiteInfo.find(SNO);
	if( iter == g_SiteInfo.end() )
	{
		ASSERT(!("site name is not initialized."));
		return FALSE;
	}


	int nSize = strlen(szID) + strlen(iter->second.szPrefix);

	szBuf[0] = '\0';
	if (nSize >= nBufSize) return FALSE;

	strcpy(szBuf, iter->second.szPrefix);
	strcat(szBuf, szID);

	return TRUE;
}

int GetGroupNo_BySpareParam(LPCTSTR strSpareParam)
{
	char strGroupNo[1024] = {0,};

	if(!GetTokenValue(strSpareParam, "group", strGroupNo)) {
		return -1;
	}

	return ::atoi(strGroupNo);
}

int Get_BySpareParam(LPCTSTR strSpareParam, char* pArg )
{
	char szValue[ MAX_PATH ] = { 0,};
	
	if ( !GetTokenValue( strSpareParam, pArg, szValue )) 
	{
		return 0;
	}
	
	return ::atoi( szValue );
}
