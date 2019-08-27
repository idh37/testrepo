// DownThread.cpp: implementation of the CDownThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownThread.h"

#include "Gif2Spr.h"
#include "MyMisc.h"

#include "mmsystem.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownThread::CDownThread()
{
	ClearMember();
}

CDownThread::~CDownThread()
{
	Destroy();
}

void CDownThread::ClearMember()
{
	m_bThreadWorking = FALSE;
	m_bStopThread = FALSE;
	m_bBusy = FALSE;

	m_pFileList = NULL;

	m_LastDown_Layer = 0;
	m_LastDown_ItemNo = 0;
	m_LastDown_bSmall = FALSE;
}

void CDownThread::Destroy()
{
	TerminateThread();
	ClearMember();
}

BOOL CDownThread::CreateThread(LPCTSTR BaseAvatarDir, CFileList* pFileList)
{
	if(m_pThread)
		return FALSE;

	if(m_bThreadWorking)
		return FALSE;

	// 변수 초기화
	m_sBaseAvatarDir = BaseAvatarDir; 
	m_pFileList = pFileList;

	// 쓰레드를 서스펜드 모드로 생성한다(쓰레드 프로시저에서 사용될 포인터의 초기화를 기다려야하기때문)
	m_pThread = AfxBeginThread(DownThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	if(m_pThread == NULL) 
	{
		m_bThreadWorking = FALSE;
		return FALSE;
	}

	// 쓰레드 가동
	m_bThreadWorking = TRUE;
	m_bStopThread = FALSE;
	m_pThread->ResumeThread();

	return TRUE;
}

BOOL CDownThread::TerminateThread()
{
	if(!m_pThread || !m_bThreadWorking)
		return FALSE;

	// 쓰레드 종료 플래그 세팅
	m_bStopThread = TRUE;

	if(m_pThread && m_pThread->m_hThread)
	{
		// 쓰레드 종료를 0.1초간 기다림 ( 100 )
		DWORD dwRetCode = WaitForSingleObject(m_pThread->m_hThread, 100 );

		if(dwRetCode == WAIT_OBJECT_0)
		{
			// 정상적인 종료를 수행하였음
		}
		else if(dwRetCode == WAIT_TIMEOUT)
		{
			// 쓰레드가 시간내에 저절로 종료되지 않으면 강제로 종료시킨다
			::TerminateThread(m_pThread->m_hThread, 0);
		}
	}

	// 종결 처리
	m_bThreadWorking = FALSE;
	m_bStopThread = FALSE;
	m_pThread = NULL;

	return TRUE;
}

BOOL CDownThread::RecreateThread()
{
	if(!m_pThread) return FALSE;

	CString BaseAvatarDir = m_sBaseAvatarDir;
	CFileList* pFileList = m_pFileList;

	TerminateThread();
	return CreateThread(BaseAvatarDir, pFileList);
}

BOOL CDownThread::CheckDownloadTimeOut()
{
	if(!m_bBusy || !m_GetHttp.m_bDownloadLoopStarted) 
		return FALSE;
	
	if(timeGetTime() - m_GetHttp.m_LastDownloadTime > 15000)
	{
		// 다운로드가 15초이상 지연된 경우 타임아웃으로 간주
		return TRUE;
	}

	return FALSE;
}

UINT CDownThread::DownThreadFunc(LPVOID pParam)
{
	CDownThread* pThis = (CDownThread*)pParam;

	return pThis->DoThreadLoop();
}

UINT CDownThread::DoThreadLoop()
{
	while(TRUE) 
	{
		///< Thread Stop
		if(m_bStopThread) 
		{
			goto END_OF_THREAD;
		}

		m_bBusy = TRUE;

		m_pFileList->Lock();	///< Thread Lock

		int workkind= 0;
		int layer	= 0;
		int itemno	= 0;
		BOOL bSmall = FALSE;

		CString sKey;
		CFileInfo* pFileInfo = NULL;
		
		POSITION pos = m_pFileList->m_DownMap.GetStartPosition();
		while (pos != NULL)
		{	
			m_pFileList->m_DownMap.GetNextAssoc(pos, sKey, pFileInfo);
			
			if(pFileInfo && pFileInfo->m_WorkStatus == 0) 
			{
				if(pFileInfo->m_WorkKind == 1)
				{
					workkind= 1;
					layer	= pFileInfo->m_Layer;
					itemno	= pFileInfo->m_ItemNo;
					bSmall	= pFileInfo->m_bSmall;
					
					// start work
					pFileInfo->m_WorkStatus = 1;
					break;
				} 
				else if(pFileInfo->m_WorkKind == 2) 
				{
					workkind= 2;
					layer	= pFileInfo->m_Layer;
					itemno	= pFileInfo->m_ItemNo;
					bSmall	= pFileInfo->m_bSmall;
					
					// start work
					pFileInfo->m_WorkStatus = 1;
					break;
				}
				else 
				{
					// 잘못된 경우
					workkind = 0;
					// end of work
					pFileInfo->m_WorkStatus = 100;
					break;
				}
			}
		}

		m_pFileList->Unlock();	///< Thread UnLock

		
		if(m_bStopThread) 
		{
			goto END_OF_THREAD;
		}

		BOOL bSuccessedGetDate = FALSE;
		SYSTEMTIME	lastmodtime = {0, };

		if(workkind == 1 || workkind == 2)
		{
			CString remotename = GetRemoteDownFileName(layer, itemno, bSmall);
		
			// 웹서버에 존재하는 GIF파일의 최근 수정일자를 얻어옴
			if(m_GetHttp.GetFileLastModifiedDate(remotename, &lastmodtime)) 
			{
				bSuccessedGetDate = TRUE;
			}
			else
			{
				#ifdef _DEBUG
				CString str;
				str.Format("1. 최근 수정일자 읽기 실패: %s", remotename);
				AfxMessageBox(str);
				#endif

				// 한번 더 시도
				if(m_GetHttp.GetFileLastModifiedDate(remotename, &lastmodtime))
				{
					bSuccessedGetDate = TRUE;
				}
				else
				{
					#ifdef _DEBUG
					CString str;
					str.Format("2. 최근 수정일자 읽기 실패: %s", remotename);
					AfxMessageBox(str);
					#endif
				}
			}
		}

		// 스프라이트 파일은 존재하나 업데이트 검사가 요청된 상태
		if(workkind == 1) 
		{
			if(bSuccessedGetDate) 
			{
				CString sprfname = GetFullSprFileName(layer, itemno, bSmall);
				CFileTag filetag;
				if(filetag.ReadTag(sprfname))
				{
					// 태그를 성공적으로 읽었다면 업데이트 여부를 검사한다
					CTime t1(filetag.GetLastModifiedDate());
					CTime t2(lastmodtime);
					if(t1 != t2) 
					{
						// 최근 수정일자가 틀리면 업데이트된 파일을 다운로드 받도록 변경
						workkind = 2;
					}
					else 
					{
						// 마지막으로 검사한 날짜를 저장하기위하여 태그를 기록
						filetag.WriteTag(sprfname, lastmodtime);
					}
				}
				else 
				{
					// 태그가 존재하지 않는다면 실시간 다운로드 이전방식의 파일이므로
					// 일단 태그만 기록해둠
					filetag.WriteTag(sprfname, lastmodtime);
				}
			}
		}

		// 스프라이트 파일이 존재하지 않거나 업데이트되었으므로 다운로드 요청된 상태
		if(workkind == 2) 
		{
			if(bSuccessedGetDate)
			{
				void* pGif = NULL;
				DWORD gifsize = 0;

				m_LastDown_Layer = layer;
				m_LastDown_ItemNo = itemno;
				m_LastDown_bSmall = bSmall;

				CString remotename = GetRemoteDownFileName(layer, itemno, bSmall);
				
				BOOL bDownSuccessed = FALSE;

				// GIF 이미지를 메모리에 다운로드
				if(m_GetHttp.GetFileToMemory(NULL, remotename, &pGif, &gifsize)) 
				{
					bDownSuccessed = TRUE;
				}
				else 
				{
					#ifdef _DEBUG
					CString str;
					str.Format("1. 다운로드 실패: %s", remotename);
					AfxMessageBox(str);
					#endif

					if(pGif) 
					{
						delete pGif;
						pGif = NULL;
					}
					gifsize = 0;
					// 두번째 다운로드 시도
					if(m_GetHttp.GetFileToMemory(NULL, remotename, &pGif, &gifsize)) 
					{
						bDownSuccessed = TRUE;
					}
					else 
					{
						if(pGif) 
						{
							delete pGif;
							pGif = NULL;
						}
						gifsize = 0;

						#ifdef _DEBUG
						CString str;
						str.Format("2. 다운로드 실패: %s", remotename);
						AfxMessageBox(str);
						#endif
					}
				}

				if(m_bStopThread)
				{
					goto END_OF_THREAD;
				}

				// 다운로드가 성공하였다면
				if(bDownSuccessed) 
				{
					// GIF를 스프라이트로 변환
					xSprite spr;
					// 작은 아바타 이미지는 사이즈 최적화하지 않음
					if(ConvertGif2SprFromMemory(pGif, gifsize, &spr, (bSmall?FALSE:TRUE))) 
					{
						CString foldername = GetFullSprFolderName(layer, bSmall);
						CreateMultipleDirectory(foldername);
						CString sprfname = GetFullSprFileName(layer, itemno, bSmall);
						if(spr.Save((char*)(LPCTSTR)sprfname))
						{
							// 스프라이트 저장에 성공하면 파일 태그를 기록
							CFileTag filetag;
							filetag.WriteTag(sprfname, lastmodtime);
						}
					}

					if(pGif)
					{
						delete pGif;
					}
				}
			}
			else
			{
				// 이 경우는 이미지가 웹서버에 존재하지 않을 확률이 크다
			}
		}

		if(m_bStopThread) 
		{
			goto END_OF_THREAD;
		}

		m_pFileList->Lock();	////////// LOCK

		pFileInfo = m_pFileList->Find(layer, itemno, bSmall);
		if(pFileInfo)
		{			
			pFileInfo->m_WorkStatus = 100;
		
			// 작업이 끝난 항목을 삭제하기전에 연결된 스프라이트 파일 클래스의 스프라이트를 읽어도록 해준다
			if(pFileInfo->m_bSprFileValid && pFileInfo->m_pSprFile)
			{
				// 다운로드 플래그 해제
				pFileInfo->m_pSprFile->m_bNowDownLoading = FALSE;

				int itemno = pFileInfo->m_pSprFile->m_ItemNo;
				if(itemno > 0) 
				{
					// 스프라이트 리로드 플래그를 세팅
					pFileInfo->m_pSprFile->m_bReloadSpr = TRUE;
				}
			}

			m_pFileList->Delete(pFileInfo);
		}
		
		m_pFileList->Unlock();	////////// UNLOCK

		m_bBusy = FALSE;

		if(m_bStopThread)
		{
			goto END_OF_THREAD;
		}

		::Sleep(1);
	}

	///< Goto End
	END_OF_THREAD:

	// 쓰레드 작동 정지
	m_bThreadWorking = FALSE;
	m_bBusy			 = FALSE;

	return 0;
}

CString CDownThread::GetFullSprFolderName(int layer, BOOL bSmall)
{
	return GetFullpathOfSprFolder(m_sBaseAvatarDir, layer, bSmall);
}

CString CDownThread::GetFullSprFileName(int layer, int itemno, BOOL bSmall)
{
	return GetFullpathOfSprFile(m_sBaseAvatarDir, layer, itemno, bSmall);
}

CString CDownThread::GetRemoteDownFileName(int layer, int itemno, BOOL bSmall)
{
	if(bSmall) 
		return GetFullpathOfDownFile(DEFAULT_SMALL_REMOTEURL, layer, itemno);

	return GetFullpathOfDownFile(DEFAULT_REMOTEURL, layer, itemno);
}

