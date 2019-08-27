// WhisperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResourceMan.h"
#include "../ThreadLock/ThreadLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CResourceMan::CResourceMan(void)
{
	InitializeCriticalSection(&m_cLock);
}

CResourceMan::~CResourceMan()
{
	ASSERT(m_listResource.size() == 0);
	ASSERT(m_listFont.size() == 0);
	DeleteCriticalSection(&m_cLock);
}

NMBASE::GRAPHICGDI::xSprite *CResourceMan::GetSprite(LPCTSTR lpPath)
{
	CThreadLock lock(&m_cLock);
	map<std::string, CResourceUnit>::iterator findIt = m_listResource.find(lpPath);
	if(findIt != m_listResource.end())
	{
		++findIt->second.m_nReferenceCount;
		return findIt->second.m_pSprite;
	}

	CResourceUnit res;
	res.m_pSprite = new NMBASE::GRAPHICGDI::xSprite;
	if(res.m_pSprite->Load((char *)lpPath, 555) == FALSE)
	{
		return NULL;
	}

	m_listResource[lpPath] = res;
	return res.m_pSprite;
}

bool CResourceMan::RemoveSprite(NMBASE::GRAPHICGDI::xSprite *pSprite)
{
	CThreadLock lock(&m_cLock);
	map<std::string, CResourceUnit>::iterator findIt = m_listResource.find(pSprite->GetFilePath().c_str());
	ASSERT(findIt != m_listResource.end());
	if(findIt == m_listResource.end()) return false;
	--findIt->second.m_nReferenceCount;
	ASSERT(findIt->second.m_nReferenceCount >= 0);
	if(findIt->second.m_nReferenceCount < 0) return false;
	if(findIt->second.m_nReferenceCount == 0)
	{
		findIt->second.m_pSprite->Remove();
		delete findIt->second.m_pSprite;
		m_listResource.erase(findIt);
	}
	return true;
}

NMBASE::GRAPHICGDI::xSprite *CResourceMan::Clone(NMBASE::GRAPHICGDI::xSprite *pSprite)
{
	CThreadLock lock(&m_cLock);
	ASSERT(pSprite != NULL);
	const string &strPath = pSprite->GetFilePath();
	
	map<std::string, CResourceUnit>::iterator findIt = m_listResource.find(strPath.c_str());
	if(findIt != m_listResource.end())
	{
		++findIt->second.m_nReferenceCount;
		return findIt->second.m_pSprite;
	}

	ASSERT(FALSE);

	return NULL;
}

CFont *CResourceMan::GetFont(LPCTSTR lpFontName, int nSize, bool bBold)
{
	CThreadLock lock(&m_cLock);

	list<CFontUnit>::iterator start = m_listFont.begin();
	list<CFontUnit>::iterator end = m_listFont.end();

	for(;start != end;++start)
	{
		if(start->m_strFontName == lpFontName && start->m_nSize == nSize && start->m_bBold == bBold)
			break;
	}

	if(start != end)
	{
		++start->m_nReferenceCount;
		return start->m_pFont;
	}

	CFontUnit res;
	res.m_pFont = new CFont;
	res.m_strFontName = lpFontName;
	res.m_nSize =nSize;
	res.m_bBold = bBold;
	if(res.m_pFont->CreateFont(nSize,0,0,0,
		bBold?FW_BOLD:FW_NORMAL,
		FALSE,FALSE,
		0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		lpFontName) == FALSE)
	{
		return NULL;
	}

	m_listFont.push_back(res);
	return res.m_pFont;
}

bool CResourceMan::RemoveFont(CFont *pFont)
{
	CThreadLock lock(&m_cLock);
	list<CFontUnit>::iterator start = m_listFont.begin();
	list<CFontUnit>::iterator end = m_listFont.end();

	for(;start != end;++start)
	{
		if(start->m_pFont == pFont) break;
	}

	ASSERT(start != end);
	if(start == end) return false;
	--start->m_nReferenceCount;
	ASSERT(start->m_nReferenceCount >= 0);
	if(start->m_nReferenceCount < 0) return false;
	if(start->m_nReferenceCount == 0)
	{
		delete start->m_pFont;
		m_listFont.erase(start);
	}
	return true;
}

CFont *CResourceMan::Clone(CFont *pFont)
{
	CThreadLock lock(&m_cLock);
	ASSERT(pFont != NULL);

	list<CFontUnit>::iterator start = m_listFont.begin();
	list<CFontUnit>::iterator end = m_listFont.end();

	for(;start != end;++start)
	{
		if(start->m_pFont == pFont) break;
	}

	if(start != end)
	{
		++start->m_nReferenceCount;
		return start->m_pFont;
	}

	ASSERT(FALSE);

	return NULL;
}

CResourceMan &CResourceMan::GetInstance(void)
{
	static CResourceMan s_cResourceMan;
	return s_cResourceMan;
}

CResourceMan &RM()
{
	return CResourceMan::GetInstance();
}
} //namespace LSY