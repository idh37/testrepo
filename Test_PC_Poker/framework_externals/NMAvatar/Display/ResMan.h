// ResMan.h: interface for the CResMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESMAN_H__08C3B1EC_6554_4332_8DFE_89733C33A592__INCLUDED_)
#define AFX_RESMAN_H__08C3B1EC_6554_4332_8DFE_89733C33A592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DownThread.h"
#include "FileList.h"


#define MAX_DOWNTHREAD	2


// 스프라이트 파일 리스트 맵
typedef CMap<CString, LPCTSTR, CSprFile*, CSprFile*&> CMapSprList;


// 스프라이트 리소스 관리자 클래스
class CResMan  
{
public:

	CString		m_sBaseAvatarDir;

	// Resource list
	CMapSprList	m_ResList;

	// Download file list
	CFileList	m_FileList;

	// Download thread
	CDownThread m_DownThread[MAX_DOWNTHREAD];	// class of download-thread
	int			m_ThreadStatus[MAX_DOWNTHREAD];	// status of download-thread

	CResMan();
	virtual ~CResMan();
	void ClearMember();

	BOOL		Init(LPCTSTR BaseAvatarDir);
	void		Destroy();

	CString		GetFullSprFileName(int layer, int itemno, BOOL bSmall);	// get full path of sprite file

	CSprFile*	OpenSpr(int layer, int itemno, BOOL bSmall);
	BOOL		CloseSpr(int layer, int itemno, BOOL bSmall);
	BOOL		CloseSpr(CSprFile* pSprFile);

};

#endif // !defined(AFX_RESMAN_H__08C3B1EC_6554_4332_8DFE_89733C33A592__INCLUDED_)
