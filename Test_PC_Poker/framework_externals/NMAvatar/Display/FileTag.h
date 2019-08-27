// FileTag.h: interface for the CFileTag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILETAG_H__96634046_8B00_44B9_A8FA_177AEDE1DEA3__INCLUDED_)
#define AFX_FILETAG_H__96634046_8B00_44B9_A8FA_177AEDE1DEA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct SPRFILETAGINFO
{
	char		IDString[8];
	SYSTEMTIME	LastModifiedDate;
	SYSTEMTIME	LastCheckedDate;
	char		Reserved[32];
};

class CFileTag  
{
public:

	BOOL			m_bValid;
	SPRFILETAGINFO	m_TagInfo;

	CFileTag();
	~CFileTag();
	void ClearMember();

	BOOL IsValid() { return m_bValid; }
	BOOL IsCheckExpired();

	BOOL ReadTag(LPCTSTR fname);
	BOOL WriteTag(LPCTSTR fname, SYSTEMTIME lastmodified);

	SYSTEMTIME GetLastModifiedDate() { return m_TagInfo.LastModifiedDate; }
	SYSTEMTIME GetLastCheckedDate() { return m_TagInfo.LastCheckedDate; }
};

#endif // !defined(AFX_FILETAG_H__96634046_8B00_44B9_A8FA_177AEDE1DEA3__INCLUDED_)
