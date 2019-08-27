
#include "stdafx.h"
#include "FileList.h"


CString GetMapKeyName(int layer, int itemno, BOOL bSmall)
{
	CString str;
	if(bSmall) {
		str.Format("SMALL_L%d_%d", layer, itemno);
	}
	else {
		str.Format("L%d_%d", layer, itemno);
	}
	return str;
}

CString GetFullpathOfSprFolder(const char* BaseAvatarDir, int layer, BOOL bSmall)
{
	CString str;
	if(bSmall) {
		str.Format("%s\\Small\\Layer%d", BaseAvatarDir, layer);
	}
	else {
		str.Format("%s\\Layer%d", BaseAvatarDir, layer);
	}
	return str;
}

CString GetFullpathOfSprFile(const char* BaseAvatarDir, int layer, int itemno, BOOL bSmall)
{
	CString str;
	if(bSmall) {
		str.Format("%s\\Small\\Layer%d\\L%d_%d.spr", BaseAvatarDir, layer, layer, itemno);
	}
	else {
		str.Format("%s\\Layer%d\\L%d_%d.spr", BaseAvatarDir, layer, layer, itemno);
	}
	return str;
}

CString GetFullpathOfDownFile(const char* BaseDownURL, int layer, int itemno)
{
	CString str;
	str.Format("%s/Layer%d/L%d_%d.gif", BaseDownURL, layer, layer, itemno);
	return str;
}


/////////////////////////////////////

CSprFile::CSprFile() 
{
	m_RefCount = m_Layer = m_ItemNo = m_TotFrameTick = 0;
	m_bSmall = FALSE;

	m_bReloadSpr = FALSE;
	m_bNowDownLoading = FALSE;

	m_bFileInfoValid = FALSE;
	m_pFileInfo = NULL;
}

CSprFile::~CSprFile() 
{ 
	if(m_bFileInfoValid && m_pFileInfo) {
		m_pFileInfo->m_bSprFileValid = FALSE;
	}

	m_Spr.Remove(); 
}

BOOL CSprFile::LoadSprite(LPCTSTR sprfname) 
{
	m_Spr.Remove();
	m_TotFrameTick = 0;
	if(m_Spr.Load((char*)sprfname, 555)) {
		// 각 프레임의 타임틱의 총계를 구한다
		for(int s = 0; s < m_Spr.sfhead.TotalSprNum; s++) {
			DWORD tick = max(10, m_Spr.spr[s].byUserVal * 100);
			m_TotFrameTick += (WORD)tick;
		}

		m_FileTag.ReadTag(sprfname);

		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////

CFileInfo::CFileInfo() { 
	m_Layer = m_ItemNo = m_WorkKind = m_WorkStatus = 0;
	m_bSmall = FALSE;
	m_bSprFileValid = FALSE;
	m_pSprFile = NULL;
}

CFileInfo::~CFileInfo() { 
	if(m_bSprFileValid && m_pSprFile) {
		m_pSprFile->m_bFileInfoValid = FALSE;
	}
}

/////////////////////////////////////

CFileList::CFileList()
{
}

CFileList::~CFileList()
{
	Destroy();
}

void CFileList::Init(const char* BaseAvatarDir)
{
	m_sBaseAvatarDir = BaseAvatarDir;

}

void CFileList::Destroy()
{
	// 스프라이트 파일리스트를 제거
	POSITION pos = m_DownMap.GetStartPosition();
	while (pos != NULL)
	{
		CString sKey;
		CFileInfo* pFileInfo;
		m_DownMap.GetNextAssoc(pos, sKey, pFileInfo);
		if(pFileInfo) delete pFileInfo;
	}
	m_DownMap.RemoveAll();
}

CFileInfo* CFileList::Add(int layer, int itemno, BOOL bSmall, int workkind, CSprFile* pSprFile)
{
	CFileInfo* pFileInfo;

	CString keyname = GetMapKeyName(layer, itemno, bSmall);
	if(m_DownMap.Lookup(keyname, pFileInfo)) {
		// 이미 존재하는 경우
		return NULL;
	}
	else {
		pFileInfo = new CFileInfo;
		if(!pFileInfo) return NULL;

		pFileInfo->m_Layer = layer;
		pFileInfo->m_ItemNo = itemno;
		pFileInfo->m_bSmall = bSmall;
		pFileInfo->m_WorkKind = workkind;
		pFileInfo->m_pSprFile = pSprFile;
		pFileInfo->m_bSprFileValid = TRUE;
		
		// 스프라이트 이름을 맵의 키로 이용
		m_DownMap[keyname] = pFileInfo;
	}

	return pFileInfo;
}

CFileInfo* CFileList::Find(int layer, int itemno, BOOL bSmall)
{
	CFileInfo* pFileInfo;
	CString keyname = GetMapKeyName(layer, itemno, bSmall);
	if(m_DownMap.Lookup(keyname, pFileInfo)) {
		return pFileInfo;
	}

	return NULL;
}

BOOL CFileList::Delete(int layer, int itemno, BOOL bSmall)
{
	CFileInfo* pFileInfo;
	CString keyname = GetMapKeyName(layer, itemno, bSmall);
	if(m_DownMap.Lookup(keyname, pFileInfo)) {
		if(pFileInfo) delete pFileInfo;
		return m_DownMap.RemoveKey(GetMapKeyName(layer, itemno, bSmall));
	}

	return TRUE;
}

BOOL CFileList::Delete(CFileInfo* pfi)
{
	if(!pfi) return FALSE;

	return Delete(pfi->m_Layer, pfi->m_ItemNo, pfi->m_bSmall);
}

CFileInfo*	CFileList::GetFirst()
{
	POSITION pos = m_DownMap.GetStartPosition();
	if(!pos) return NULL;

	CString sKey;
	CFileInfo* pFileInfo = NULL;
	m_DownMap.GetNextAssoc(pos, sKey, pFileInfo);

	return pFileInfo;
}

