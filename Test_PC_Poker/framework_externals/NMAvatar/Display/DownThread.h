// DownThread.h: interface for the CDownThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNTHREAD_H__B06E74CE_124B_44C3_A235_C06EAAB68052__INCLUDED_)
#define AFX_DOWNTHREAD_H__B06E74CE_124B_44C3_A235_C06EAAB68052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GetHttp.h"
#include "FileList.h"

#define DEFAULT_REMOTEURL		("http://c2.img.netmarble.kr/item_img/images")
#define DEFAULT_SMALL_REMOTEURL	("http://c2.img.netmarble.kr/item_img/small_images")
//#define DEFAULT_REMOTEURL		("http://test2.netmarble.org/item_img/images")
//#define DEFAULT_SMALL_REMOTEURL	("http://test2.netmarble.org/item_img/small_images")


class CDownThread  
{
public:

	CWinThread*	m_pThread;			// 쓰레드 포인터	
	BOOL		m_bThreadWorking;
	BOOL		m_bStopThread;
	BOOL		m_bBusy;

	CGetHttp	m_GetHttp;
	CString		m_sBaseAvatarDir;
	CFileList*	m_pFileList;

	int			m_LastDown_Layer;
	int			m_LastDown_ItemNo;
	BOOL		m_LastDown_bSmall;

	CEvent m_threadKillEvent;   // public에 데이터멤버로 스레드를 종료시킬 이벤트 객체를 추가합니다

	CDownThread();
	~CDownThread();
	void		ClearMember();
	void		Destroy();

	BOOL		CreateThread(LPCTSTR BaseAvatarDir, CFileList* pFileList);
	BOOL		TerminateThread();
	BOOL		RecreateThread();
	BOOL		CheckDownloadTimeOut();
	
	UINT		DoThreadLoop();

	CString		GetFullSprFolderName(int layer, BOOL bSmall);				// get full folder path of sprite 
	CString		GetFullSprFileName(int layer, int itemno, BOOL bSmall);		// get full path of sprite file
	CString		GetRemoteDownFileName(int layer, int itemno, BOOL bSmall);	// get remote path of gif file for download

	static UINT DownThreadFunc(LPVOID pParam);								// DownLoad Thread Function
};

#endif // !defined(AFX_DOWNTHREAD_H__B06E74CE_124B_44C3_A235_C06EAAB68052__INCLUDED_)
