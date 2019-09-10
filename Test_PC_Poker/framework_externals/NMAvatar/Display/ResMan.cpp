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
	
	// �ٿ�ε� ���ϸ���Ʈ �ʱ�ȭ
	m_FileList.Init(BaseAvatarDir);

	// �ٿ�ε� ������ ����
	for(int i=0; i<MAX_DOWNTHREAD; i++)
	{
		m_DownThread[i].CreateThread(BaseAvatarDir, &m_FileList);
	}

	return TRUE;
}

void CResMan::Destroy()
{
	// �ٿ�ε� ������ ����
	for(int i=0; i<MAX_DOWNTHREAD; i++) 
	{
		m_DownThread[i].Destroy();
	}

	// ��������Ʈ ���ϸ���Ʈ�� ����
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

	// �ٿ�ε� ���ϸ���Ʈ ����
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
		// üũ�߿� �ߺ� üũ�Ǵ� ���� ����
		bChecking = TRUE;

		// �ּ��� 1�ʰ��ݿ� �ѹ��� �������� �ٿ�ε� Ÿ�Ӿƿ��� �˻�
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

					// �ٿ�ε� Ÿ�Ӿƿ��̶�� �����带 ������ ������ �����
					m_DownThread[i].RecreateThread();

					// �ٿ�ε� ���ϸ���Ʈ���� ����

					m_FileList.Lock();	////////// LOCK

					CFileInfo* pFileInfo = m_FileList.Find(last_layer, last_itemno, last_bSmall);
					if(pFileInfo)
					{				
						// �ٿ�ε� �÷��� ����
						if(pFileInfo->m_bSprFileValid && pFileInfo->m_pSprFile) 
						{
							pFileInfo->m_pSprFile->m_bNowDownLoading = FALSE;
						}
						m_FileList.Delete(pFileInfo);
					}
					
					m_FileList.Unlock();	////////// UNLOCK

					#ifdef _DEBUG
					CString str;
					str.Format("�ٿ�ε� Ÿ�Ӿƿ����� ������ �����: %s", remotename);
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
		// �̹� �ε�� ��������Ʈ��� ���� ī��Ʈ�� ����
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
		
		// ���ο� ��������Ʈ�� �о���̰� ����Ʈ�� �߰�
		CString sprfname = GetFullSprFileName(layer, itemno, bSmall);

		if(pSprFile->LoadSprite(sprfname))
		{
			if(!pSprFile->m_FileTag.IsValid() || pSprFile->m_FileTag.IsCheckExpired())
			{
				// ���� �±װ� �������� �ʰų� ������Ʈ �˻簡 �ʿ��ϴٸ� �۾� ��û
				m_FileList.Lock();		////// LOCK
				pSprFile->m_pFileInfo = m_FileList.Add(layer, itemno, bSmall, 1, pSprFile);
				pSprFile->m_bFileInfoValid = TRUE;
				m_FileList.Unlock();	////// UNLOCK
			}
		}
		else
		{
			// ������ �������� �ʴ´ٸ� �ٿ�ε� ��û
			m_FileList.Lock();		////// LOCK
			// �ٿ�ε� �����忡�� ������ �޵��� ����Ʈ�� ����(�ٿ�ε尡 �Ϸ�Ǹ� �ڵ����� �ε��)
			pSprFile->m_pFileInfo = m_FileList.Add(layer, itemno, bSmall, 2, pSprFile);
			pSprFile->m_bFileInfoValid = TRUE;
			pSprFile->m_bNowDownLoading = TRUE;
			m_FileList.Unlock();	////// UNLOCK

			// �ٿ�ε� ����Ʈ���� �߰��ǰ� ��������Ʈ ����Ʈ���� �߰���
		}

		// ��������Ʈ ���ҽ��� �̸��� ���� Ű�� �̿�
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
				// �Ҹ��ڿ��� ��������Ʈ�� �ı��ϰ� �ٿ��Ʈ ���� �������� ��ȿ �÷��׸� �ʱ�ȭ ������
				delete pSprFile;
				// �� ����Ʈ���� ����
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
