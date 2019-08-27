//#if !defined(__EXTRACTFILE_H__)
//#define __EXTRACTFILE_H__
#pragma once

#define UM_EXTRACT_NOTIFY				(WM_USER + 4001)

#define UM_EXTRACTTHREAD_STARTED		(WM_USER + 4002)
#define UM_EXTRACTTHREAD_FAIL			(WM_USER + 4003)
#define UM_EXTRACTTHREAD_COMPLEATED		(WM_USER + 4004)

#define EXTRACT_FILEEXTRACTSTART	1
#define EXTRACT_FILEEXTRACTSUCCESS	2

typedef struct
{
	int Kind;
	UINT ReadByte;
	CString SrcFileName;
	CString TarFileName;
} EXTRACT_NOTIFY;


BOOL GetZipFileSize(const char *zipfname, UINT *pCompSize, UINT *pUncompSize);
BOOL UnzipFile(HWND hwnd, const char *zipfname, const char *targetpath);

BOOL BeginExtractThread(HWND hWnd);
BOOL TerminateExtractThread();

//#endif