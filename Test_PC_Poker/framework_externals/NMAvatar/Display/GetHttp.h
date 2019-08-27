// GetHttp.h: interface for the CGetHttp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETHTTP_H__225A2DD6_5B1B_4782_9C08_540FE4904B6A__INCLUDED_)
#define AFX_GETHTTP_H__225A2DD6_5B1B_4782_9C08_540FE4904B6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<AFXINET.H>


#define UM_GETHTTP_NOTIFY			(WM_USER + 5001)

#define GETHTTP_RECEIVESTART		1
#define GETHTTP_RECEIVESUCCESS		2
#define GETHTTP_RECEIVEBYTE			3
#define GETHTTP_RECEIVEFAIL			4

//#define GETHTTP_STATUSBUFSIZE		32
#define GETHTTP_STATUSBUFSIZE		256
#define GETHTTP_DATABUFSIZE			100000

typedef struct
{
	int Kind;
	UINT TotReadByte;
	UINT ReadByte;
	CString FileName;
} GETHTTP_NOTIFY;


class CGetHttp  
{

public:
    CString m_sURLToDownload;
    CString m_sFileToDownloadInto;
    CString m_sUserName;
    CString m_sPassword;

	BOOL	m_bDownloadLoopStarted;
	UINT	m_LastDownloadTime;

	void Destroy();
	void ClearMember();
	void CloseAllHandles();

	BOOL SetAccessRight(CString userName,CString userPass); // Step 1 아뒤 패스워드 입력
	
	BOOL GetFileSize(CString remoteFile, int *pSize);
	BOOL GetFileLastModifiedDate(CString remoteFile, SYSTEMTIME *pTime);
	BOOL GetFileStatusCode(CString remoteFile, int *pCode);

	BOOL GetFile(HWND hWndNotify, CString remoteFile,CString localFile);
	BOOL GetFileToMemory(HWND hWndNotify, CString remoteFile, void** pBinary, DWORD* pRecvSize);	// pBinary에 할당된 메모리는 반드시 해제해야함

public:
	CGetHttp();
	virtual ~CGetHttp();


public:
  CWnd *Papa;
  
  CString       m_sServer; 
  CString       m_sObject; 
  CString       m_sFilename;
  INTERNET_PORT m_nPort;
  DWORD         m_dwServiceType;
  HINTERNET     m_hInternetSession;
  HINTERNET     m_hHttpConnection;
  HINTERNET     m_hHttpFile;
  CFile         m_FileToWrite;
};

#endif // !defined(AFX_GETHTTP_H__225A2DD6_5B1B_4782_9C08_540FE4904B6A__INCLUDED_)
