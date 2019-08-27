// ResMan.cpp: implementation of the CResMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResMan.h"

#include "mmsystem.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResMan::CResMan()
{
	ClearMember();
}

CResMan::~CResMan()
{
	Destroy();
}

void CResMan::ClearMember()
{
	m_sBaseAvatarDir = "";
}

BOOL CResMan::Init(LPCTSTR BaseAvatarDir)
{
	if(!BaseAvatarDir)
		return FALSE;

	m_sBaseAvatarDir = BaseAvatarDir;
	
	// 다운로드 파일리스트 초기화
	m_FileList.Init(BaseAvatarDir);

	// 다운로드 쓰레드 생성
	for(int i=0; i<MAX_DOWNTHREAD; i++)
	{
		m_DownThread[i].CreateThread(BaseAvatarDir, &m_FileList);
	}

	return TRUE;
}

void CResMan::Destroy()
{
	// 다운로드 쓰레드 제거
	for(int i=0; i<MAX_DOWNTHREAD; i++) 
	{
		m_DownThread[i].Destroy();
	}

	// 스프라이트 파일리스트를 제거
	POSITION pos = m_ResList.GetStartPosition();
	while (pos != NULL)
	{
		CString sKey;
		CSprFile* pSprFile;
		m_ResList.GetNextAssoc(pos, sKey, pSprFile);
		if(pSprFile) 
			delete pSprFile;
	}
	m_ResList.RemoveAll();

	// 다운로드 파일리스트 제거
	m_FileList.Destroy();
}

CString CResMan::GetFullSprFileName(int layer, int itemno, BOOL bSmall)
{
	return GetFullpathOfSprFile(m_sBaseAvatarDir, layer, itemno, bSmall);
}

CSprFile* CResMan::OpenSpr(int layer, int itemno, BOOL bSmall)
{
	static UINT lastchecktime = timeGetTime();

	static BOOL bChecking = FALSE;
	if(!bChecking)
	{
		// 체크중에 중복 체크되는 것을 방지
		bChecking = TRUE;

		// 최소한 1초간격에 한번씩 쓰레드의 다운로드 타임아웃을 검사
		if(timeGetTime() - lastchecktime > 1000) 
		{
			lastchecktime = timeGetTime();
			for(int i=0; i<MAX_DOWNTHREAD; i++)
			{
				if(m_DownThread[i].CheckDownloadTimeOut()) 
				{
					#ifdef _DEBUG
					CString remotename = m_DownThread[i].m_GetHttp.m_sURLToDownload;
					#endif

					int  last_layer = m_DownThread[i].m_LastDown_Layer;
					int	 last_itemno = m_DownThread[i].m_LastDown_ItemNo;
					BOOL last_bSmall = m_DownThread[i].m_LastDown_bSmall;

					// 다운로드 타임아웃이라면 쓰레드를 강제로 죽인후 재생성
					m_DownThread[i].RecreateThread();

					// 다운로드 파일리스트에서 삭제

					m_FileList.Lock();	////////// LOCK

					CFileInfo* pFileInfo = m_FileList.Find(last_layer, last_itemno, last_bSmall);
					if(pFileInfo)
					{				
						// 다운로드 플래그 해제
						if(pFileInfo->m_bSprFileValid && pFileInfo->m_pSprFile) 
						{
							pFileInfo->m_pSprFile->m_bNowDownLoading = FALSE;
						}
						m_FileList.Delete(pFileInfo);
					}
					
					m_FileList.Unlock();	////////// UNLOCK

					#ifdef _DEBUG
					CString str;
					str.Format("다운로드 타임아웃으로 쓰레드 재생성: %s", remotename);
					AfxMessageBox(str);
					#endif
				}
			}
		}
		bChecking = FALSE;
	}

	CSprFile* pSprFile = NULL;
	CString keyname = GetMapKeyName(layer, itemno, bSmall);

	if(m_ResList.Lookup(keyname, pSprFile))
	{
		// 이미 로드된 스프라이트라면 참조 카운트만 증가
		pSprFile->m_RefCount++;
		return pSprFile;
	}
	else
	{		
		pSprFile = new CSprFile;

		if(!pSprFile)
			return NULL;

		pSprFile->m_RefCount = 1;
		pSprFile->m_Layer = layer;
		pSprFile->m_ItemNo = itemno;
		pSprFile->m_bSmall = bSmall;
		pSprFile->m_TotFrameTick = 0;
		
		// 새로운 스프라이트를 읽어들이고 리스트에 추가
		CString sprfname = GetFullSprFileName(layer, itemno, bSmall);

		if(pSprFile->LoadSprite(sprfname))
		{
			if(!pSprFile->m_FileTag.IsValid() || pSprFile->m_FileTag.IsCheckExpired())
			{
				// 파일 태그가 존재하지 않거나 업데이트 검사가 필요하다면 작업 요청
				m_FileList.Lock();		////// LOCK
				pSprFile->m_pFileInfo = m_FileList.Add(layer, itemno, bSmall, 1, pSprFile);
				pSprFile->m_bFileInfoValid = TRUE;
				m_FileList.Unlock();	////// UNLOCK
			}
		}
		else
		{
			// 파일이 존재하지 않는다면 다운로드 요청
			m_FileList.Lock();		////// LOCK
			// 다운로드 쓰레드에서 파일을 받도록 리스트에 삽입(다운로드가 완료되면 자동으로 로드됨)
			pSprFile->m_pFileInfo = m_FileList.Add(layer, itemno, bSmall, 2, pSprFile);
			pSprFile->m_bFileInfoValid = TRUE;
			pSprFile->m_bNowDownLoading = TRUE;
			m_FileList.Unlock();	////// UNLOCK

			// 다운로드 리스트에도 추가되고 스프라이트 리스트에도 추가함
		}

		// 스프라이트 리소스의 이름을 맵의 키로 이용
		m_ResList[keyname] = pSprFile;
	}

	return pSprFile;
}

BOOL CResMan::CloseSpr(int layer, int itemno, BOOL bSmall)
{
	CSprFile* pSprFile;

	CString keyname = GetMapKeyName(layer, itemno, bSmall);
	if(m_ResList.Lookup(keyname, pSprFile))
	{
		if(pSprFile) 
		{
			if(pSprFile->m_RefCount > 0)
				pSprFile->m_RefCount--;

			if(pSprFile->m_RefCount <= 0)
			{
				// 소멸자에서 스프라이트를 파괴하고 다운리스트 파일 포인터의 유효 플래그를 초기화 시켜줌
				delete pSprFile;
				// 맵 리스트에서 삭제
				m_ResList.RemoveKey(keyname);
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CResMan::CloseSpr(CSprFile* pSprFile)
{
	if(!pSprFile) 
		return FALSE;
	return CloseSpr(pSprFile->m_Layer, pSprFile->m_ItemNo, pSprFile->m_bSmall);
}
