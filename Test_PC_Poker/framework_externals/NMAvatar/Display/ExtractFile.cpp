#include "StdAfx.h"
//#include <afxwin.h>         // MFC core and standard components
//#include <afx.h>         // MFC core and standard components

#include "ExtractFile.h"

#include "UnzipFile.h"
#include "ZipException.h"

//#include "Global.h"
#include "ExtractFile.h"
#include "MyMisc.h"

//#ifndef _DEBUG
//#pragma comment(lib, "ZipFunc_Release.lib")
//#else
//#pragma comment(lib, "ZipFunc_Debug.lib")
//#endif

#define BUF_SIZE 16384

BOOL GetZipFileSize(const char *zipfname, UINT *pCompSize, UINT *pUncompSize)
{
	if(zipfname == NULL) return FALSE;

	UINT totalcompsize = 0;
	UINT totaluncompsize = 0;

	try
	{
		CUnzipFile uf(zipfname);
		uf.GoToFirstFile();

		do
		{
			unz_file_info ui;

			// 	getting the information about the file 
			// (date, attributes, the size of the filename)
			uf.GetCurrentFileInfo(&ui);
		
			totalcompsize += ui.compressed_size;
			totaluncompsize += ui.uncompressed_size;

		}
		while(uf.GoToNextFile() != false);

		// cannot be called by the destructor 
		uf.Close();	
	}
	catch (CException* e)
	{
		e->Delete();
		return FALSE;
	}

	*pCompSize = totalcompsize;
	*pUncompSize = totaluncompsize;

	return TRUE;
}


BOOL UnzipFile(HWND hwnd, const char *zipfname, const char *targetpath)
{
	if(zipfname == NULL) return FALSE;
	if(targetpath == NULL) return FALSE;

	EXTRACT_NOTIFY en;
	en.Kind = 0;
	en.ReadByte = 0;
	en.SrcFileName = zipfname;
	en.TarFileName = targetpath;

	try
	{
		CUnzipFile uf(zipfname);
		uf.GoToFirstFile();

		do
		{
			// 여러파일이 압축되어있을경우 루프를 돌며 해제
			unz_file_info ui;

			// 	getting the information about the file 
			// (date, attributes, the size of the filename)
			uf.GetCurrentFileInfo(&ui);

			int iNameSize = ui.size_filename + 1;
			char* pName = new char [iNameSize];

			// get the name of the file
			uf.GetCurrentFileInfo(NULL, pName, iNameSize);

			TCHAR szDir[_MAX_DIR];
			TCHAR szDrive[_MAX_DRIVE];
			_tsplitpath(targetpath, szDrive, szDir,NULL, NULL);
			CString szPath = CString(szDrive) + szDir;
			
			CString strExtFileName = szPath + pName;
			CString strExtFolderName = szPath;

			BOOL bFindFolder = FALSE;
			int slen = strExtFileName.GetLength();
			for(int i=slen-1; i>=0; i--) {
				if(strExtFileName.GetAt(i) == '/' || strExtFileName.GetAt(i) == '\\') {
					if(strExtFileName.GetAt(i) == '/') strExtFileName.SetAt(i, '\\');
					if(!bFindFolder) {
						strExtFolderName = strExtFileName.Mid(0, i);
						bFindFolder = TRUE;
						//::MessageBox(NULL, strExtFolderName, "info", MB_OK);
					}
				}
			}

			// 압축을 풀 디렉토리 생성
			CreateMultipleDirectory(strExtFolderName);

			BOOL bIsFolder = FALSE;
			if(pName[strlen(pName)-1] == '\\') {
				// 파일이 아니라 빈 폴더인 경우임
				bIsFolder = TRUE;
			}

			delete[] pName;
			pName = NULL;

			// 부모 윈도우에 파일 추출 시작을 통지
			en.TarFileName = strExtFileName;
			en.ReadByte = ui.compressed_size;
			en.Kind = EXTRACT_FILEEXTRACTSTART;
			if(hwnd) ::SendMessage(hwnd, UM_EXTRACT_NOTIFY, (WPARAM)&en, 0);

			if(!bIsFolder) {
				// 폴더가 아닌 파일인 경우
				
				// 파일의 속성을 검사하여 쓰기 가능하도록 변경
				CFileStatus fStatus;
				if(CFile::GetStatus(strExtFileName, fStatus)) {
					// 원래의 속성값을 기억
					BYTE orgattrib = fStatus.m_attribute;
					// 각종 속성 체크
					if(fStatus.m_attribute & CFile::readOnly) fStatus.m_attribute &= ~CFile::readOnly;
					if(fStatus.m_attribute & CFile::hidden  ) fStatus.m_attribute &= ~CFile::hidden;
					if(fStatus.m_attribute & CFile::system  ) fStatus.m_attribute &= ~CFile::system;
					// 파일 속성플래그가 변경되었다면 적용
					if(orgattrib != fStatus.m_attribute) {
						CFile::SetStatus(strExtFileName, fStatus);
					}
				}

				// 타겟 파일을 생성
				CFile f(strExtFileName, CFile::modeWrite | CFile::modeCreate);

				uf.OpenCurrentFile();
				char buf[BUF_SIZE];

				int size_read;
				do
				{
					size_read = uf.ReadCurrentFile(buf, BUF_SIZE);
					if(size_read > 0) f.Write(buf, size_read);
				}
				while (size_read == BUF_SIZE);

				// set the original date stamp and attributes to the unpacked file
				// (this function closes the file "f" which is needed to apply
				// the attributes)
				uf.UpdateFileStatus(f, ui);
			}

			// 부모 윈도우에 파일 추출 성공을 통지
			en.Kind = EXTRACT_FILEEXTRACTSUCCESS;
			if(hwnd) ::SendMessage(hwnd, UM_EXTRACT_NOTIFY, (WPARAM)&en, 0);

		}
		while(uf.GoToNextFile() != false);

		// cannot be called by the destructor 
		uf.Close();	
	}
	catch (CException* e)
	{
		if(e->IsKindOf( RUNTIME_CLASS( CZipException )))
		{
			CZipException* p = (CZipException*) e;
			//... and so on 
		}
		else if(e->IsKindOf( RUNTIME_CLASS( CFileException )))
		{
			CFileException* p = (CFileException*) e;
			//... and so on 
		}
		else // the only possibility is a memory exception I suppose
		{
			//... and so on 
		}
		e->Delete();

		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////

static HWND hWndNotify = NULL;
static CWinThread *pExtractThread=NULL;
static BOOL bExtractThreadStop = FALSE;
static UINT ExtractThreadFunc(LPVOID pParam);

BOOL BeginExtractThread(HWND hWnd)
{
	hWndNotify = hWnd;
	pExtractThread = ::AfxBeginThread(ExtractThreadFunc, NULL);
	if(pExtractThread == NULL) return FALSE;
	return TRUE;
}

BOOL TerminateExtractThread()
{
	if(pExtractThread == NULL) return TRUE;

	// 쓰레드 종료 플래그 세팅
	bExtractThreadStop = TRUE;
	
	// 쓰레드 종료를 일정시간동안 기다림
	DWORD dwRetCode;
	dwRetCode = ::WaitForSingleObject(pExtractThread->m_hThread, 1000);

	if(dwRetCode == WAIT_OBJECT_0)
	{
		// 정상적인 종료를 수행하였음
		return TRUE;
	}
	else if(dwRetCode == WAIT_TIMEOUT)
	{
		// 쓰레드가 시간내에 저절로 종료되지 않으면 강제로 종료시킨다
		::TerminateThread(pExtractThread->m_hThread, 0);
	}

	return TRUE;
}

UINT ExtractThreadFunc(LPVOID pParam)
{
	int totextractfnum = 0;
	UINT totcompsize=0, totuncompsize=0;

	/*
	for(int i=0; i<FileList.nTotFileNum; i++)
	{
		if(FileList.bZipFile[i] == TRUE)
		{
			totextractfnum++;
			UINT compsize=0, uncompsize=0;
			BOOL rtn = GetZipFileSize((LPCTSTR)FileList.GetFullLocalFileName(i), &compsize, &uncompsize);
			if(rtn==TRUE) { totcompsize+=compsize; totuncompsize+=uncompsize; }
		}

		if(bExtractThreadStop) return 0;
	}

	if(hWndNotify) ::SendMessage(hWndNotify, UM_EXTRACTTHREAD_STARTED, totextractfnum, totcompsize);

	for(i=0; i<FileList.nTotFileNum; i++)
	{
		if(FileList.bZipFile[i] == TRUE)
		{
			// 압축을 풀 디렉토리 생성
			//CreateMultipleDirectory((LPCTSTR)FileList.GetFullExtractPath(i));

			BOOL rtn = UnzipFile(hWndNotify, (LPCTSTR)FileList.GetFullLocalFileName(i), (LPCTSTR)FileList.GetFullExtractPath(i));
			if(rtn == FALSE)
			{
				if(hWndNotify) ::SendMessage(hWndNotify, UM_EXTRACTTHREAD_FAIL, (WPARAM)((LPCTSTR)FileList.GetFullLocalFileName(i)), (LPARAM)((LPCTSTR)FileList.GetFullExtractPath(i)));
				pExtractThread = NULL;
				return 0;
			}
		}

		if(bExtractThreadStop) return 0;
	}
	*/


	pExtractThread = NULL;
	if(hWndNotify) ::SendMessage(hWndNotify, UM_EXTRACTTHREAD_COMPLEATED, 0, 0);

	return 0;
}
