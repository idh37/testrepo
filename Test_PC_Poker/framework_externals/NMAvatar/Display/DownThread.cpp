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

	// ���� �ʱ�ȭ
	m_sBaseAvatarDir = BaseAvatarDir; 
	m_pFileList = pFileList;

	// �����带 ������� ���� �����Ѵ�(������ ���ν������� ���� �������� �ʱ�ȭ�� ��ٷ����ϱ⶧��)
	m_pThread = AfxBeginThread(DownThreadFunc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	if(m_pThread == NULL) 
	{
		m_bThreadWorking = FALSE;
		return FALSE;
	}

	// ������ ����
	m_bThreadWorking = TRUE;
	m_bStopThread = FALSE;
	m_pThread->ResumeThread();

	return TRUE;
}

BOOL CDownThread::TerminateThread()
{
	if(!m_pThread || !m_bThreadWorking)
		return FALSE;

	// ������ ���� �÷��� ����
	m_bStopThread = TRUE;

	if(m_pThread && m_pThread->m_hThread)
	{
		// ������ ���Ḧ 0.1�ʰ� ��ٸ� ( 100 )
		DWORD dwRetCode = WaitForSingleObject(m_pThread->m_hThread, 100 );

		if(dwRetCode == WAIT_OBJECT_0)
		{
			// �������� ���Ḧ �����Ͽ���
		}
		else if(dwRetCode == WAIT_TIMEOUT)
		{
			// �����尡 �ð����� ������ ������� ������ ������ �����Ų��
			::TerminateThread(m_pThread->m_hThread, 0);
		}
	}

	// ���� ó��
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
		// �ٿ�ε尡 15���̻� ������ ��� Ÿ�Ӿƿ����� ����
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
					// �߸��� ���
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
		
			// �������� �����ϴ� GIF������ �ֱ� �������ڸ� ����
			if(m_GetHttp.GetFileLastModifiedDate(remotename, &lastmodtime)) 
			{
				bSuccessedGetDate = TRUE;
			}
			else
			{
				#ifdef _DEBUG
				CString str;
				str.Format("1. �ֱ� �������� �б� ����: %s", remotename);
				AfxMessageBox(str);
				#endif

				// �ѹ� �� �õ�
				if(m_GetHttp.GetFileLastModifiedDate(remotename, &lastmodtime))
				{
					bSuccessedGetDate = TRUE;
				}
				else
				{
					#ifdef _DEBUG
					CString str;
					str.Format("2. �ֱ� �������� �б� ����: %s", remotename);
					AfxMessageBox(str);
					#endif
				}
			}
		}

		// ��������Ʈ ������ �����ϳ� ������Ʈ �˻簡 ��û�� ����
		if(workkind == 1) 
		{
			if(bSuccessedGetDate) 
			{
				CString sprfname = GetFullSprFileName(layer, itemno, bSmall);
				CFileTag filetag;
				if(filetag.ReadTag(sprfname))
				{
					// �±׸� ���������� �о��ٸ� ������Ʈ ���θ� �˻��Ѵ�
					CTime t1(filetag.GetLastModifiedDate());
					CTime t2(lastmodtime);
					if(t1 != t2) 
					{
						// �ֱ� �������ڰ� Ʋ���� ������Ʈ�� ������ �ٿ�ε� �޵��� ����
						workkind = 2;
					}
					else 
					{
						// ���������� �˻��� ��¥�� �����ϱ����Ͽ� �±׸� ���
						filetag.WriteTag(sprfname, lastmodtime);
					}
				}
				else 
				{
					// �±װ� �������� �ʴ´ٸ� �ǽð� �ٿ�ε� ��������� �����̹Ƿ�
					// �ϴ� �±׸� ����ص�
					filetag.WriteTag(sprfname, lastmodtime);
				}
			}
		}

		// ��������Ʈ ������ �������� �ʰų� ������Ʈ�Ǿ����Ƿ� �ٿ�ε� ��û�� ����
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

				// GIF �̹����� �޸𸮿� �ٿ�ε�
				if(m_GetHttp.GetFileToMemory(NULL, remotename, &pGif, &gifsize)) 
				{
					bDownSuccessed = TRUE;
				}
				else 
				{
					#ifdef _DEBUG
					CString str;
					str.Format("1. �ٿ�ε� ����: %s", remotename);
					AfxMessageBox(str);
					#endif

					if(pGif) 
					{
						delete pGif;
						pGif = NULL;
					}
					gifsize = 0;
					// �ι�° �ٿ�ε� �õ�
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
						str.Format("2. �ٿ�ε� ����: %s", remotename);
						AfxMessageBox(str);
						#endif
					}
				}

				if(m_bStopThread)
				{
					goto END_OF_THREAD;
				}

				// �ٿ�ε尡 �����Ͽ��ٸ�
				if(bDownSuccessed) 
				{
					// GIF�� ��������Ʈ�� ��ȯ
					xSprite spr;
					// ���� �ƹ�Ÿ �̹����� ������ ����ȭ���� ����
					if(ConvertGif2SprFromMemory(pGif, gifsize, &spr, (bSmall?FALSE:TRUE))) 
					{
						CString foldername = GetFullSprFolderName(layer, bSmall);
						CreateMultipleDirectory(foldername);
						CString sprfname = GetFullSprFileName(layer, itemno, bSmall);
						if(spr.Save((char*)(LPCTSTR)sprfname))
						{
							// ��������Ʈ ���忡 �����ϸ� ���� �±׸� ���
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
				// �� ���� �̹����� �������� �������� ���� Ȯ���� ũ��
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
		
			// �۾��� ���� �׸��� �����ϱ����� ����� ��������Ʈ ���� Ŭ������ ��������Ʈ�� �о�� ���ش�
			if(pFileInfo->m_bSprFileValid && pFileInfo->m_pSprFile)
			{
				// �ٿ�ε� �÷��� ����
				pFileInfo->m_pSprFile->m_bNowDownLoading = FALSE;

				int itemno = pFileInfo->m_pSprFile->m_ItemNo;
				if(itemno > 0) 
				{
					// ��������Ʈ ���ε� �÷��׸� ����
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

	// ������ �۵� ����
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

