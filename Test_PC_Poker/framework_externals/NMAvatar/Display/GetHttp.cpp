// GetHttp.cpp: implementation of the CGetHttp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetHttp.h"
#include "MyMisc.h"

#include "mmsystem.h"

//#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGetHttp::CGetHttp()
{
	ClearMember();
}

CGetHttp::~CGetHttp()
{
	Destroy();
}

void CGetHttp::Destroy()
{
	CloseAllHandles();
	ClearMember();
}

void CGetHttp::ClearMember()
{
	m_sURLToDownload = "";
	m_sFileToDownloadInto = "";
	m_sUserName = "";
	m_sPassword = "";

	Papa = NULL;
	m_nPort = INTERNET_DEFAULT_HTTP_PORT;
	m_dwServiceType = INTERNET_SERVICE_HTTP;
	m_hInternetSession = NULL;
	m_hHttpConnection = NULL;
	m_hHttpFile = NULL;

	m_bDownloadLoopStarted = FALSE;
	m_LastDownloadTime = 0;
}

void CGetHttp::CloseAllHandles()
{
	if(m_FileToWrite.m_hFile != CFile::hFileNull)
	{
		m_FileToWrite.Close();
	}

	if(m_hHttpFile)
	{ 
		InternetCloseHandle(m_hHttpFile);
		m_hHttpFile=NULL;
	}
	
	if(m_hHttpConnection)
	{ 
		InternetCloseHandle(m_hHttpConnection);
		m_hHttpConnection=NULL;
	}

	if(m_hInternetSession)
	{
		InternetCloseHandle(m_hInternetSession);
		m_hInternetSession=NULL;
	}

	m_bDownloadLoopStarted = FALSE;
	m_LastDownloadTime = 0;
}

BOOL CGetHttp::SetAccessRight(CString userName, CString userPass)
{
	this->m_sUserName = userName;
	this->m_sPassword = userPass;
	if((userName == "") || (userPass == "")) return 0;
	return 1;
}

BOOL CGetHttp::GetFileSize(CString remoteFile, int *pSize)
{
	CloseAllHandles();

	this->m_sURLToDownload = remoteFile;					// HTTP URL

	if(!AfxParseURL(m_sURLToDownload,m_dwServiceType,m_sServer,m_sObject,m_nPort))
	{
		m_sURLToDownload = _T("http://") + m_sURLToDownload;
		if(!AfxParseURL(m_sURLToDownload, m_dwServiceType, m_sServer, m_sObject, m_nPort)) 
		{
#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFileSize() - URL Parsing Failed.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
			return FALSE;
		}
	}

	m_hInternetSession = ::InternetOpen("MyApp", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(m_hInternetSession == NULL) 
	{
#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileSize() - Internet Session Open Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		return FALSE;
	}

	char		szSizeBuffer[GETHTTP_STATUSBUFSIZE]={0,};
	DWORD		dwLengthSizeBuffer = GETHTTP_STATUSBUFSIZE;
	HINTERNET	hFileInfo=NULL;

	//hFileInfo = InternetOpenUrl(m_hInternetSession, (LPCTSTR)remoteFile, NULL, 0, 0, 0);
	hFileInfo = InternetOpenUrl(m_hInternetSession, (LPCTSTR)remoteFile, NULL, 0, INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_PRAGMA_NOCACHE, 0);

	if(hFileInfo == NULL)
	{
#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileSize() - InternetOpenUrl is NULL.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		//::InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();
		return FALSE;
	}

	BOOL bQuery = ::HttpQueryInfo(hFileInfo,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer, &dwLengthSizeBuffer, NULL) ;

	::InternetCloseHandle(hFileInfo);
	//::InternetCloseHandle(m_hInternetSession);
	CloseAllHandles();

	if(bQuery)
	{    
		*pSize = atol(szSizeBuffer);
		//::MessageBox(NULL, szSizeBuffer,"Error",MB_OK | MB_ICONWARNING);
	}
	else
	{
		*pSize = 0;
		//::MessageBox(NULL, "Unable to retrieve file sizes!","Error",MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}

BOOL CGetHttp::GetFileLastModifiedDate(CString remoteFile, SYSTEMTIME *pTime)
{
	CloseAllHandles();

	memset(pTime, 0, sizeof(SYSTEMTIME));

	this->m_sURLToDownload = remoteFile;					// HTTP URL

	if(!AfxParseURL(m_sURLToDownload,m_dwServiceType,m_sServer,m_sObject,m_nPort))
	{
		m_sURLToDownload = _T("http://") + m_sURLToDownload;
		if(!AfxParseURL(m_sURLToDownload, m_dwServiceType, m_sServer, m_sObject, m_nPort))
		{
#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFileLastModifiedDate() - URL Parsing Failed.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
			return FALSE;
		}
	}

	m_hInternetSession = ::InternetOpen("MyApp", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(m_hInternetSession == NULL)
	{
#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileLastModifiedDate() - Internet Session Open Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		return FALSE;
	}

	HINTERNET	hFileInfo=NULL;
	//hFileInfo = InternetOpenUrl(m_hInternetSession, (LPCTSTR)remoteFile, NULL, 0, 0, 0);
	hFileInfo = InternetOpenUrl(m_hInternetSession, (LPCTSTR)remoteFile, NULL, 0, INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_PRAGMA_NOCACHE, 0);

	if(hFileInfo == NULL)
	{
#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileLastModifiedDate() - InternetOpenUrl is NULL.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		//::InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();
		return FALSE;
	}

	DWORD	dwLengthSizeBuffer = sizeof(SYSTEMTIME);
	BOOL bQuery = ::HttpQueryInfo(hFileInfo, HTTP_QUERY_LAST_MODIFIED|HTTP_QUERY_FLAG_SYSTEMTIME, pTime, &dwLengthSizeBuffer, NULL);
	//BOOL bQuery = ::HttpQueryInfo(hFileInfo, HTTP_QUERY_DATE|HTTP_QUERY_FLAG_SYSTEMTIME,

	::InternetCloseHandle(hFileInfo);
	//::InternetCloseHandle(m_hInternetSession);
	CloseAllHandles();

	if(!bQuery)
	{    
		//::MessageBox(NULL, "Unable to retrieve!","Error",MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}

BOOL CGetHttp::GetFileStatusCode(CString remoteFile, int *pCode)
{
	CloseAllHandles();

	this->m_sURLToDownload = remoteFile;					// HTTP URL

	if(!AfxParseURL(m_sURLToDownload,m_dwServiceType,m_sServer,m_sObject,m_nPort))
	{
		m_sURLToDownload = _T("http://") + m_sURLToDownload;
		if(!AfxParseURL(m_sURLToDownload, m_dwServiceType, m_sServer, m_sObject, m_nPort))
		{
#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFileStatusCode() - URL Parsing Failed.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
			return FALSE;
		}
	}

	m_hInternetSession = ::InternetOpen("MyApp", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(m_hInternetSession == NULL)
	{
#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileStatusCode() - Internet Session Open Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		return FALSE;
	}

	char		szSizeBuffer[GETHTTP_STATUSBUFSIZE]={0,};
	DWORD		dwLengthSizeBuffer = GETHTTP_STATUSBUFSIZE;
	HINTERNET	hFileInfo=NULL;

	//hFileInfo = InternetOpenUrl(m_hInternetSession, (LPCTSTR)remoteFile, NULL, 0, 0, 0);
	hFileInfo = InternetOpenUrl(m_hInternetSession, (LPCTSTR)remoteFile, NULL, 0, INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_PRAGMA_NOCACHE, 0);

	if(hFileInfo == NULL) 
	{
#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileStatusCode() - InternetOpenUrl is NULL.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		//::InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();
		return FALSE;
	}

	BOOL bQuery = ::HttpQueryInfo(hFileInfo,HTTP_QUERY_STATUS_CODE, szSizeBuffer, &dwLengthSizeBuffer, NULL) ;

	::InternetCloseHandle(hFileInfo);
	//::InternetCloseHandle(m_hInternetSession);
	CloseAllHandles();

	if(!bQuery) 
	{    
		*pCode = 0;
		//::MessageBox(NULL, "Unable to retrieve!","Error",MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	*pCode = atol(szSizeBuffer);

	return TRUE;
}

BOOL CGetHttp::GetFile(HWND hWndNotify, CString remoteFile, CString localFile)
{
	CloseAllHandles();

	GETHTTP_NOTIFY gnotify;
	gnotify.Kind = 0;
	gnotify.ReadByte = 0;
	gnotify.FileName = remoteFile;

	UINT iTotReadByte = 0;

	gnotify.Kind = GETHTTP_RECEIVESTART;
	if(hWndNotify)
	{
		::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
	}

	this->m_sURLToDownload = remoteFile;					// HTTP URL

	if(!AfxParseURL(m_sURLToDownload,m_dwServiceType,m_sServer,m_sObject,m_nPort))
	{
		m_sURLToDownload = _T("http://") + m_sURLToDownload;
		if(!AfxParseURL(m_sURLToDownload, m_dwServiceType, m_sServer, m_sObject, m_nPort))
		{
			gnotify.Kind = GETHTTP_RECEIVEFAIL;
			if(hWndNotify)
			{
				::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
			}
#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFile() - URL Parsing Failed.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
			return FALSE;
		}
	}

	// 다운로드할 디렉토리명 추출
	CString fname = localFile;
	CString DirPath;
	int slen = strlen(localFile);
	int cnt = 0;
	for(int k=slen-1; k>=0; k--)
	{
		if(fname.GetAt(k) == '/' || fname.GetAt(k) == '\\') 
		{
			DirPath = fname.Mid(0, k);
			break;
		}
	}

	// 다운로드할 디렉토리 생성
	CreateMultipleDirectory((LPCTSTR)DirPath);
	/*
	if(!CreateMultipleDirectory((LPCTSTR)DirPath)) {
	#ifdef _DEBUG
	char debugstr[1024]={0,};
	wsprintf(debugstr, "CGetHttp::GetFile() - Create Directory Failed.\n%s", DirPath);
	::MessageBox(NULL, debugstr, "Error", MB_OK);
	#endif
	return FALSE;
	}
	*/

	// 파일 생성
	DWORD dwFileFlags = CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite;
	if(!m_FileToWrite.Open(localFile, dwFileFlags)) 
	{
		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify)
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}
#ifdef _DEBUG
		char debugstr[1024]={0,};
		wsprintf(debugstr, "CGetHttp::GetFile() - File Create Failed.\n%s", localFile);
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_bDownloadLoopStarted = TRUE;
	m_LastDownloadTime = timeGetTime();

	//m_hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	m_hInternetSession = ::InternetOpen("MyApp", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	//m_hInternetSession = ::InternetOpen("MyApp", INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	if(m_hInternetSession == NULL)
	{
		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify)
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFile() - Internet Session Open Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	if(m_sUserName.GetLength())
	{
		m_hHttpConnection = ::InternetConnect(m_hInternetSession, m_sServer, m_nPort, m_sUserName, m_sPassword, m_dwServiceType, 0, (DWORD) this);
	}
	else
	{
		m_hHttpConnection = ::InternetConnect(m_hInternetSession, m_sServer, m_nPort, NULL, NULL, m_dwServiceType, 0, (DWORD) this);
	}

	if(m_hHttpConnection == NULL)
	{
		//InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();

		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify) 
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFile() - InternetConnect Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;

	//DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION;
	//DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_KEEP_CONNECTION;
	DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE;
	if(m_nPort == INTERNET_DEFAULT_HTTPS_PORT) 
	{
		dwFlags	|= (INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	}

	m_hHttpFile = HttpOpenRequest(m_hHttpConnection, NULL, m_sObject, _T("HTTP/1.1"), NULL, ppszAcceptTypes, dwFlags, (DWORD) this);
	if(m_hHttpFile == NULL)
	{
		//InternetCloseHandle(m_hHttpConnection);
		//InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();

		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify)
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFile() - HttpOpenRequest Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	BOOL bSend = ::HttpSendRequest(m_hHttpFile, NULL, 0, NULL, 0);

	if(!bSend) 
	{
		//InternetCloseHandle(m_hHttpFile);
		//InternetCloseHandle(m_hHttpConnection);
		//InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();

		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify)
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFile() - HttpSendRequest Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	TCHAR szStatusCode[GETHTTP_STATUSBUFSIZE];
	DWORD dwInfoSize = GETHTTP_STATUSBUFSIZE;
	if(!HttpQueryInfo(m_hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		//InternetCloseHandle(m_hHttpFile);
		//InternetCloseHandle(m_hHttpConnection);
		//InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();

		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify) 
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFile() - HttpQueryInfo Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

		return FALSE;
	}
	else
	{
		long nStatusCode = _ttol(szStatusCode);
		if(nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
		{
			// We have to read all outstanding data on the Internet handle
			// before we can resubmit request. Just discard the data.
			char szData[GETHTTP_DATABUFSIZE];
			DWORD dwSize;
			do
			{
				::InternetReadFile(m_hHttpFile, (LPVOID)szData, GETHTTP_DATABUFSIZE-1, &dwSize);

				// 마지막으로 다운로드한 시간을 기록
				m_LastDownloadTime = timeGetTime();
			}
			while (dwSize != 0);
		}
		else if(nStatusCode != HTTP_STATUS_OK && nStatusCode != HTTP_STATUS_PARTIAL_CONTENT)
		{
			//InternetCloseHandle(m_hHttpFile);
			//InternetCloseHandle(m_hHttpConnection);
			//InternetCloseHandle(m_hInternetSession);
			CloseAllHandles();

			gnotify.Kind = GETHTTP_RECEIVEFAIL;
			if(hWndNotify)
			{
				::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
			}

#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFile() - nStatusCode is Invalid.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

			return FALSE;
		}
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

#define READBUFSIZE	8192

	char szReadBuf[READBUFSIZE];
	DWORD dwBytesToRead = READBUFSIZE;
	DWORD dwTotalBytesRead = 0;
	//Setup the struct used for the read
	INTERNET_BUFFERS netBuf;
	ZeroMemory(&netBuf, sizeof(netBuf));
	netBuf.dwStructSize = sizeof(netBuf);
	netBuf.lpvBuffer = szReadBuf;
	netBuf.dwBufferLength = READBUFSIZE;
	netBuf.dwBufferTotal = READBUFSIZE;
	netBuf.dwOffsetHigh = READBUFSIZE - 1;

	do
	{
		netBuf.dwBufferLength = READBUFSIZE;
		//if(!::InternetReadFileEx(m_hHttpFile, &netBuf, IRF_NO_WAIT, 0))
		if(!::InternetReadFileEx(m_hHttpFile, &netBuf, IRF_SYNC, 0))
		{
			//InternetCloseHandle(m_hHttpFile);
			//InternetCloseHandle(m_hHttpConnection);
			//InternetCloseHandle(m_hInternetSession);
			CloseAllHandles();

			gnotify.Kind = GETHTTP_RECEIVEFAIL;
			if(hWndNotify)
			{
				::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
			}

#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFile() - InternetReadFileEx Failed.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
			return FALSE;
		}
		else
		{
			if(netBuf.dwBufferLength)
			{
				// 마지막으로 다운로드한 시간을 기록
				m_LastDownloadTime = timeGetTime();

				TRY
				{
					m_FileToWrite.Write(szReadBuf, netBuf.dwBufferLength);

					gnotify.ReadByte = 0;
					if(netBuf.dwBufferLength >0 )
					{
						iTotReadByte += netBuf.dwBufferLength;
						gnotify.ReadByte = netBuf.dwBufferLength;
					}
					gnotify.Kind = GETHTTP_RECEIVEBYTE;
					gnotify.TotReadByte = iTotReadByte;
					if(hWndNotify)
					{
						::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
					}
				}
				CATCH(CFileException, e)
				{
					//InternetCloseHandle(m_hHttpFile);
					//InternetCloseHandle(m_hHttpConnection);
					//InternetCloseHandle(m_hInternetSession);
					CloseAllHandles();

					e->Delete();

					gnotify.Kind = GETHTTP_RECEIVEFAIL;
					if(hWndNotify)
					{
						::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
					}

#ifdef _DEBUG
					char *debugstr = "CGetHttp::GetFile() - File Exception Occured.";
					::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

					return FALSE;
				}
				END_CATCH
			}	
		}

	} while (netBuf.dwBufferLength);
	//Just close the file before we return
	m_FileToWrite.Close();
	//InternetCloseHandle(m_hHttpFile);
	//InternetCloseHandle(m_hHttpConnection);
	//InternetCloseHandle(m_hInternetSession);
	CloseAllHandles();

	gnotify.Kind = GETHTTP_RECEIVESUCCESS;
	if(hWndNotify) 
	{
		::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
	}
	return TRUE;
}

BOOL CGetHttp::GetFileToMemory(HWND hWndNotify, CString remoteFile, void** pBinary, DWORD* pRecvSize)
{
	CloseAllHandles();

	GETHTTP_NOTIFY gnotify;
	gnotify.Kind = 0;
	gnotify.ReadByte = 0;
	gnotify.FileName = remoteFile;

	UINT iTotReadByte = 0;

	gnotify.Kind = GETHTTP_RECEIVESTART;
	if(hWndNotify) 
	{
		::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
	}

	this->m_sURLToDownload = remoteFile;					// HTTP URL

	if(!AfxParseURL(m_sURLToDownload,m_dwServiceType,m_sServer,m_sObject,m_nPort))
	{
		m_sURLToDownload = _T("http://") + m_sURLToDownload;
		if(!AfxParseURL(m_sURLToDownload, m_dwServiceType, m_sServer, m_sObject, m_nPort))
		{
			gnotify.Kind = GETHTTP_RECEIVEFAIL;
			if(hWndNotify)
			{
				::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
			}
#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFileToMemory() - URL Parsing Failed.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
			return FALSE;
		}
	}

	// 마지막으로 다운로드한 시간을 기록
	m_bDownloadLoopStarted = TRUE;
	m_LastDownloadTime = timeGetTime();

	//m_hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	m_hInternetSession = ::InternetOpen("MyApp", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	//m_hInternetSession = ::InternetOpen("MyApp", INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	if(m_hInternetSession == NULL)
	{
		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify)
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}
#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileToMemory() - Internet Session Open Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	if(m_sUserName.GetLength())
	{
		m_hHttpConnection = ::InternetConnect(m_hInternetSession, m_sServer, m_nPort, m_sUserName, m_sPassword, m_dwServiceType, 0, (DWORD) this);
	}
	else
	{
		m_hHttpConnection = ::InternetConnect(m_hInternetSession, m_sServer, m_nPort, NULL, NULL, m_dwServiceType, 0, (DWORD) this);
	}

	if(m_hHttpConnection == NULL) 
	{
		//InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();

		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify) ::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileToMemory() - InternetConnect Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;

	//DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION;
	//DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_KEEP_CONNECTION;
	DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE;
	if(m_nPort == INTERNET_DEFAULT_HTTPS_PORT) 
	{
		dwFlags	|= (INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	}

	m_hHttpFile = HttpOpenRequest(m_hHttpConnection, NULL, m_sObject, _T("HTTP/1.1"), NULL, ppszAcceptTypes, dwFlags, (DWORD) this);
	if(m_hHttpFile == NULL)
	{
		//InternetCloseHandle(m_hHttpConnection);
		//InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();

		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify) 
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileToMemory() - HttpOpenRequest Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	BOOL bSend = ::HttpSendRequest(m_hHttpFile, NULL, 0, NULL, 0);

	if(!bSend)
	{
		//InternetCloseHandle(m_hHttpFile);
		//InternetCloseHandle(m_hHttpConnection);
		//InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();

		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify) 
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileToMemory() - HttpSendRequest Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

		return FALSE;
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	TCHAR szStatusCode[GETHTTP_STATUSBUFSIZE] = {0, };
	DWORD dwInfoSize = GETHTTP_STATUSBUFSIZE;
	if(!HttpQueryInfo(m_hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		//InternetCloseHandle(m_hHttpFile);
		//InternetCloseHandle(m_hHttpConnection);
		//InternetCloseHandle(m_hInternetSession);
		CloseAllHandles();

		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify) 
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

#ifdef _DEBUG
		char *debugstr = "CGetHttp::GetFileToMemory() - HttpQueryInfo Failed.";
		::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

		return FALSE;
	}
	else
	{
		long nStatusCode = _ttol(szStatusCode);
		if(nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
		{
			// We have to read all outstanding data on the Internet handle
			// before we can resubmit request. Just discard the data.
			char szData[GETHTTP_DATABUFSIZE] = {0, };
			DWORD dwSize = 0;
			do
			{
				::InternetReadFile(m_hHttpFile, (LPVOID)szData, GETHTTP_DATABUFSIZE-1, &dwSize);

				// 마지막으로 다운로드한 시간을 기록
				m_LastDownloadTime = timeGetTime();
			}while (dwSize != 0);
		}
		else if(nStatusCode != HTTP_STATUS_OK && nStatusCode != HTTP_STATUS_PARTIAL_CONTENT)
		{
			//InternetCloseHandle(m_hHttpFile);
			//InternetCloseHandle(m_hHttpConnection);
			//InternetCloseHandle(m_hInternetSession);
			CloseAllHandles();

			gnotify.Kind = GETHTTP_RECEIVEFAIL;
			if(hWndNotify)
			{
				::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
			}

#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFileToMemory() - nStatusCode is Invalid.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif

			return FALSE;
		}
	}

	// 마지막으로 다운로드한 시간을 기록
	m_LastDownloadTime = timeGetTime();

	#define READBUFSIZE	8192

	char szReadBuf[READBUFSIZE] = {0, };
	DWORD dwBytesToRead = READBUFSIZE;
	DWORD dwTotalBytesRead = 0;
	//Setup the struct used for the read
	INTERNET_BUFFERS netBuf;
	ZeroMemory(&netBuf, sizeof(netBuf));
	netBuf.dwStructSize = sizeof(netBuf);
	netBuf.lpvBuffer = szReadBuf;
	netBuf.dwBufferLength = READBUFSIZE;
	netBuf.dwBufferTotal = READBUFSIZE;
	netBuf.dwOffsetHigh = READBUFSIZE - 1;

	// 다운로드한 파일 이미지를 보관할 버퍼
	CByteArray FileBuf;
	int filebufsize = 1024 * 20;
	FileBuf.SetSize(filebufsize);
	int usedbufsize = 0;

	do
	{
		netBuf.dwBufferLength = READBUFSIZE;

		//if(!::InternetReadFileEx(m_hHttpFile, &netBuf, IRF_NO_WAIT, 0))
		if(!::InternetReadFileEx(m_hHttpFile, &netBuf, IRF_SYNC, 0))
		{
			//InternetCloseHandle(m_hHttpFile);
			//InternetCloseHandle(m_hHttpConnection);
			//InternetCloseHandle(m_hInternetSession);
			CloseAllHandles();

			gnotify.Kind = GETHTTP_RECEIVEFAIL;
			if(hWndNotify)
			{
				::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
			}

#ifdef _DEBUG
			char *debugstr = "CGetHttp::GetFileToMemory() - InternetReadFileEx Failed.";
			::MessageBox(NULL, debugstr, "Error", MB_OK);
#endif
			return FALSE;
		}
		else
		{
			// 마지막으로 다운로드한 시간을 기록
			m_LastDownloadTime = timeGetTime();

			if(netBuf.dwBufferLength)
			{
				// 버퍼가 초과하는 경우 두배로 늘림
				while(usedbufsize + (int)netBuf.dwBufferLength > filebufsize)
				{
					filebufsize *= 2;
					FileBuf.SetSize(filebufsize);
				}

				if(netBuf.dwBufferLength > 0)
				{
					memcpy(&FileBuf[usedbufsize], szReadBuf, netBuf.dwBufferLength);
					usedbufsize += netBuf.dwBufferLength;
				}

				gnotify.ReadByte = 0;
				if(netBuf.dwBufferLength >0 )
				{
					iTotReadByte += netBuf.dwBufferLength;
					gnotify.ReadByte = netBuf.dwBufferLength;
				}
				gnotify.Kind = GETHTTP_RECEIVEBYTE;
				gnotify.TotReadByte = iTotReadByte;
				if(hWndNotify)
				{
					::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
				}
			}	
		}

	}while (netBuf.dwBufferLength);

	//Just close the file before we return
	//InternetCloseHandle(m_hHttpFile);
	//InternetCloseHandle(m_hHttpConnection);
	//InternetCloseHandle(m_hInternetSession);
	CloseAllHandles();

	if(usedbufsize == 0)
	{
		gnotify.Kind = GETHTTP_RECEIVEFAIL;
		if(hWndNotify)
		{
			::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
		}

		return FALSE;
	}

	// 새로운 메모리를 할당하여 받은 파일의 내용을 복사
	*pBinary = new BYTE[usedbufsize];
	memcpy(*pBinary, FileBuf.GetData(), usedbufsize);
	*pRecvSize = usedbufsize;

	gnotify.Kind = GETHTTP_RECEIVESUCCESS;

	if(hWndNotify)
	{
		::SendMessage(hWndNotify, UM_GETHTTP_NOTIFY, (WPARAM)&gnotify, 0);
	}

	return TRUE;
}
