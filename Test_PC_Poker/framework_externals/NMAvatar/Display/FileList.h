#if !defined(__FILELIST_H__)
#define __FILELIST_H__

#include <afxtempl.h>
#include <afxmt.h>

#include "xSprite.h"
#include "FileTag.h"


CString GetMapKeyName(int layer, int itemno, BOOL bSmall);										// 스프라이트의 키 이름
CString GetFullpathOfSprFolder(const char* BaseAvatarDir, int layer, BOOL bSmall);				// 스프라이트를 저장할 폴더의 이름을 얻어옴
CString GetFullpathOfSprFile(const char* BaseAvatarDir, int layer, int itemno, BOOL bSmall);	// 읽어들일 스프라이트의 전체 이름
CString GetFullpathOfDownFile(const char* BaseDownURL, int layer, int itemno);					// 다운로드 받을 GIF 이미지의 URL


class CFileInfo;

// 스프라이트 파일 클래스
class CSprFile
{
public:
	
	int			m_RefCount;
	int			m_Layer;
	int			m_ItemNo;
	BOOL		m_bSmall;
	DWORD		m_TotFrameTick;
	
	BOOL		m_bReloadSpr;
	BOOL		m_bNowDownLoading;

	xSprite		m_Spr;
	CFileTag	m_FileTag;

	BOOL		m_bFileInfoValid;
	CFileInfo*	m_pFileInfo;

	CSprFile();
	~CSprFile();

	BOOL LoadSprite(LPCTSTR sprfname);
};


// 다운로드 파일 정보 클래스
class CFileInfo
{
public:
	int			m_Layer;
	int			m_ItemNo;
	BOOL		m_bSmall;
	int			m_WorkKind;		///< 0: 이 경우는 이미지가 웹서버에 존재하지 않을 확률이 크다
								///< 1: 스프라이트 파일은 존재하나 업데이트 검사가 요청된 상태
								///< 2: 스프라이트 파일이 존재하지 않거나 업데이트되었으므로 다운로드 요청된 상태
								
	int			m_WorkStatus;	///< 0:Not Work, 1:Now Working, 100:Working Finished
	
	BOOL		m_bSprFileValid;
	CSprFile*	m_pSprFile;

	CFileInfo();
	~CFileInfo();
};

// 다운로드 파일 맵
typedef CMap<CString, LPCTSTR, CFileInfo*, CFileInfo*&> CDownMap;

// 다운르도 파일 리스트 클래스
class CFileList  
{
public:

	CString				m_sBaseAvatarDir;
	CCriticalSection	m_CsLock;
	CDownMap			m_DownMap;

	CFileList();
	~CFileList();

	void		Init(const char* BaseAvatarDir);
	void		Destroy();

	CFileInfo*	Add(int layer, int itemno, BOOL bSmall, int workkind, CSprFile* pSprFile);
	CFileInfo*	Find(int layer, int itemno, BOOL bSmall);
	BOOL		Delete(int layer, int itemno, BOOL bSmall);
	BOOL		Delete(CFileInfo* pfi);

	CFileInfo*	GetFirst();

	/////// inline function

	inline	void	Lock() { m_CsLock.Lock(); }
	inline	void	Unlock() { m_CsLock.Unlock(); }
};

#endif