#if !defined(__FILELIST_H__)
#define __FILELIST_H__

#include <afxtempl.h>
#include <afxmt.h>

#include "xSprite.h"
#include "FileTag.h"


CString GetMapKeyName(int layer, int itemno, BOOL bSmall);										// ��������Ʈ�� Ű �̸�
CString GetFullpathOfSprFolder(const char* BaseAvatarDir, int layer, BOOL bSmall);				// ��������Ʈ�� ������ ������ �̸��� ����
CString GetFullpathOfSprFile(const char* BaseAvatarDir, int layer, int itemno, BOOL bSmall);	// �о���� ��������Ʈ�� ��ü �̸�
CString GetFullpathOfDownFile(const char* BaseDownURL, int layer, int itemno);					// �ٿ�ε� ���� GIF �̹����� URL


class CFileInfo;

// ��������Ʈ ���� Ŭ����
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


// �ٿ�ε� ���� ���� Ŭ����
class CFileInfo
{
public:
	int			m_Layer;
	int			m_ItemNo;
	BOOL		m_bSmall;
	int			m_WorkKind;		///< 0: �� ���� �̹����� �������� �������� ���� Ȯ���� ũ��
								///< 1: ��������Ʈ ������ �����ϳ� ������Ʈ �˻簡 ��û�� ����
								///< 2: ��������Ʈ ������ �������� �ʰų� ������Ʈ�Ǿ����Ƿ� �ٿ�ε� ��û�� ����
								
	int			m_WorkStatus;	///< 0:Not Work, 1:Now Working, 100:Working Finished
	
	BOOL		m_bSprFileValid;
	CSprFile*	m_pSprFile;

	CFileInfo();
	~CFileInfo();
};

// �ٿ�ε� ���� ��
typedef CMap<CString, LPCTSTR, CFileInfo*, CFileInfo*&> CDownMap;

// �ٿ�� ���� ����Ʈ Ŭ����
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