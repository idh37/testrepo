// FileTag.cpp: implementation of the CFileTag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileTag.h"

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ID_SPRFILETAG ("SFILETAG")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileTag::CFileTag()
{
	ClearMember();
}

CFileTag::~CFileTag()
{

}

void CFileTag::ClearMember()
{
	m_bValid = FALSE;
	memset(&m_TagInfo, 0, sizeof(SPRFILETAGINFO));
}

BOOL CFileTag::IsCheckExpired()
{
	CTime checktime(m_TagInfo.LastCheckedDate);
	CTimeSpan timespan;
	timespan = CTime::GetCurrentTime() - checktime;

	long exhour = 12;	// 12시간
	if(timespan.GetTotalHours() > exhour || timespan.GetTotalHours() < -exhour) {
		return TRUE;
	}
	return FALSE;
}

BOOL CFileTag::ReadTag(LPCTSTR fname)
{
	m_bValid = FALSE;

	if(!fname) return FALSE;

	FILE* fp = fopen(fname, "rb");
	if(!fp) return FALSE;

	long flength = _filelength(_fileno(fp));
	if(flength <= sizeof(SPRFILETAGINFO)) {
		// 파일 사이즈가 너무 작으면 읽기 실패
		fclose(fp);
		return FALSE;
	}

	if(fseek(fp, -(sizeof(SPRFILETAGINFO)), SEEK_END) != 0) {
		fclose(fp);
		return FALSE;
	}

	memset(&m_TagInfo, 0, sizeof(SPRFILETAGINFO));

	fread(&m_TagInfo, sizeof(SPRFILETAGINFO), 1, fp);

	if(strncmp(m_TagInfo.IDString, ID_SPRFILETAG, 8) != 0) {
		// 파일 태그가 아니면 읽기 실패
		memset(&m_TagInfo, 0, sizeof(SPRFILETAGINFO));
		fclose(fp);
		return FALSE;
	}

	fclose(fp);

	m_bValid = TRUE;

	return TRUE;
}

BOOL CFileTag::WriteTag(LPCTSTR fname, SYSTEMTIME lastmodified)
{
	if(!fname) 
		return FALSE;

	// 태그를 읽어봐서 이미 태그가 존재하는지 검사
	CFileTag test;
	BOOL bTagExist = test.ReadTag(fname);

	// 이어 쓰기용으로 파일을 오픈
	FILE* fp = fopen(fname, "rb+");
	if(!fp) return FALSE;

	if(bTagExist) 
	{
		if(fseek(fp, -(sizeof(SPRFILETAGINFO)), SEEK_END) != 0)
		{
			fclose(fp);
			return FALSE;
		}

		fread(&m_TagInfo, sizeof(SPRFILETAGINFO), 1, fp);

		fseek(fp, -(sizeof(SPRFILETAGINFO)), SEEK_END);
		
	}
	else
	{
		if(fseek(fp, 0, SEEK_END) != 0)
		{
			fclose(fp);
			return FALSE;
		}
	}

	// ID문자열 초기화
	memcpy(m_TagInfo.IDString, ID_SPRFILETAG, 8);
	// 최근 수정일자 초기화
	m_TagInfo.LastModifiedDate = lastmodified;
	// 마지막 점검일자 초기화(현재 시간)
	CTime checktime = CTime::GetCurrentTime();
	checktime.GetAsSystemTime(m_TagInfo.LastCheckedDate);

	if(fwrite(&m_TagInfo, sizeof(SPRFILETAGINFO), 1, fp) == 0)
	{
		fclose(fp);
		return FALSE;
	}

	fclose(fp);

	return TRUE;
}