// GFUtil.h: interface for the CGFUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GFUTIL_H__900A3D88_90BE_4508_9A6D_120ED098B69E__INCLUDED_)
#define AFX_GFUTIL_H__900A3D88_90BE_4508_9A6D_120ED098B69E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	_NMID(a)	RemoveSitePrefix(a)
enum ENUM_SITENO
{
	SNO_NOINIT		= -1,	// Not initialized
	SNO_NETMARBLE	= 0,	// 넷마블	
	SNO_NATE		= 30,	// 네이트
	SNO_TOCTOC		= 55,	// 톡톡게임 //< CYS 120227 >
	//SNO_MAX		= 4,
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

void	InitGFUtil();
BOOL SetCurSiteNo_BySpareParam(LPCTSTR strCmndLine);

void SetCurSiteNo(int nSNO);
ENUM_SITENO GetCurSiteNo();
char* GetCurSiteName();
char* GetCurSitePrefix();
char* GetCurHSiteName();
char *GetCurRankingURL();
char *GetCurHomeURL();
char *GetCurHelpURL();
void SetCurSiteNo_ByID(char *szID);
ENUM_SITENO GetSiteNoFromID(const char *szID);
char* GetHSiteName(ENUM_SITENO SNO);
char *RemoveSitePrefix(const char *szID);
BOOL AddSitePrefix(ENUM_SITENO SNO, char *szID, char *szBuf, int nBufSize);
// 스페어 파라미터로부터 그룹번호를 파싱(-1을 리턴하면 없음)
int GetGroupNo_BySpareParam(LPCTSTR strSpareParam);
int Get_BySpareParam(LPCTSTR strSpareParam, char* pArg ); //< CYS 100525 >

const char* GetExclusiveKey();

#endif // !defined(AFX_GFUTIL_H__900A3D88_90BE_4508_9A6D_120ED098B69E__INCLUDED_)
