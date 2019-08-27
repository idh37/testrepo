#include "stdafx.h"
#include <process.h>
#include <afxinet.h>
#include "ContegoThread.h"

CContegoThread& gContegoThread()
{
	return CContegoThread::GetInstance();
}

DWORD WINAPI ContegoThreadStartProc( LPVOID lpParameter )
{
	CContegoThread* pContegoThread = reinterpret_cast<CContegoThread*>( lpParameter );

	pContegoThread->LoaderThread_Entry();

	return(0);
}

CContegoThread::CContegoThread() 
: m_hContegoThread( NULL )
{
	::InitializeCriticalSection( &m_Tasks_cs );
	m_nContegoThreadID = 0;
	m_bExit = FALSE;

	m_nGameCode = 0;
	m_strServer = "";
	m_strKeyObject = "";
	m_strObject = "";

	memset(m_szSecurityKey, 0, sizeof(m_szSecurityKey));

	// start threads
	BeginManager();
}


CContegoThread::~CContegoThread()
{
	Destroy();
}


void CContegoThread::Destroy()
{
	m_bExit = TRUE;		

	::DeleteCriticalSection( &m_Tasks_cs );

	if( m_hContegoThread )
	{		
		::CloseHandle( m_hContegoThread );			
		m_hContegoThread = NULL;
	}
}

CContegoThread& CContegoThread::GetInstance()
{
	static CContegoThread s_oStatisticsManager;
	return s_oStatisticsManager;
}

void CContegoThread::Initilize( int nGameCode, char* szServer, char* szKeyObject, char* szObject )
{
	m_nGameCode = nGameCode;
	m_strServer = szServer;
	m_strKeyObject = szKeyObject;
	m_strObject = szObject;
}

// Data Load Thread
void CContegoThread::LoaderThread_Entry()
{
	while(!m_bExit)
	{
		Sleep(1000);

		if(m_ContegoQueue.GetQueueCount())
		{
			CONTEGODATA stData;

			bool bReturn = m_ContegoQueue.RemoveFromFront( stData );	

			if(bReturn)
			{
				SendContegoData( &stData );
			}
		}						
	}
}

void CContegoThread::AddContegoData(CONTEGODATA* pstData)
{
	m_ContegoQueue.AddToBack(*pstData);
}

// start Data Loading threads
bool CContegoThread::BeginManager()
{	
	if(NULL == (m_hContegoThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void*))ContegoThreadStartProc, (void*)this, 0, (unsigned *)&m_nContegoThreadID))) 
	{
		throw( "CStatisticsManager::BeginLoading(): can't create loader thread!" );
	}

	return true;
}

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

// 코드프로젝트에서 가져와서 whitespace 부분만 뺌
CString URLEncode1(CString sIn)
{
	CString sOut;

	int k;
	const int nLen = sIn.GetLength() + 1;

	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	BYTE b = 0;

	//count not alphanumeric characters
	k = 0;

	pInTmp = pInBuf;
	while(*pInTmp)
	{
		if (!isalnum(*pInTmp) && !isalnum(*pInTmp))
			k++;
		pInTmp++;
	}

	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen  + 2 * k); //new BYTE [nLen  + 3 * k];

	if(pOutBuf)
	{
		pInTmp	= pInBuf;
		pOutTmp = pOutBuf;

		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp))
				*pOutTmp++ = *pInTmp;
			else
			{
				*pOutTmp++ = '%';
				*pOutTmp++ = toHex(*pInTmp>>4);
				*pOutTmp++ = toHex(*pInTmp%16);
			}
			pInTmp++;
		}

		*pOutTmp = '\0';
		//sOut=pOutBuf;
		//delete [] pOutBuf;
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();
	return sOut;
}

void CContegoThread::RecvSecurityKey(HINTERNET hConnect)
{
	HINTERNET hkeyObject = ::HttpOpenRequest( hConnect,
											NULL,
											m_strKeyObject,
											_T("HTTP/1.1"),
											NULL, NULL,
											INTERNET_FLAG_RELOAD, NULL);
	if( hkeyObject )
	{
		if( ::HttpSendRequest(hkeyObject, NULL, 0L, NULL, 0) )
		{
			DWORD dwRead = 0;
			::InternetReadFile(hkeyObject, m_szSecurityKey, 256, &dwRead);

			for (UINT i = 0; i < dwRead; i++)
			{
				if ( !isdigit(m_szSecurityKey[i]) )
				{
					m_szSecurityKey[i] = 0;
					break;
				}
			}
		}
		::InternetCloseHandle(hkeyObject);
	}
}

int CContegoThread::SendContegoData(CONTEGODATA* pstData)
{
	int nReturn = -1;

	if( pstData == NULL )
	{
		nReturn = -1;
	}
	else
	{
		DWORD dwServiceTye = 0;
		INTERNET_PORT nPort = 80;

		HINTERNET hSession = ::InternetOpen( "StartPage", 0, NULL, NULL, NULL );

		if( hSession == NULL )
		{
			nReturn = -3;
		}
		else
		{
			HINTERNET hConnect = ::InternetConnect( hSession, m_strServer, nPort, NULL, NULL, INTERNET_SCHEME_HTTP, NULL, NULL);

			if( hConnect == NULL )
			{
				nReturn = -4;
			}
			else
			{
				if (!m_szSecurityKey[0])
				{
					RecvSecurityKey(hConnect);
					if (!m_szSecurityKey[0])
					{
						InternetCloseHandle(hConnect);
						OutputDebugString("failed to recv security key");
						return -5;
					}
				}
				CString	strImageName = URLEncode1(pstData->szImage);

				////////////////////////////////////////////////////////////////////////
				// MakeObject
				CString strNowTime = COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
				CString strURLObject;
				strURLObject.Format( "%s?game_code=%d&err_code=0x%08X&detect_code=0x%08X&image=%s&click_dt_client=%s.000&key=%s",
									 m_strObject,
									 m_nGameCode,
									 pstData->dwErrCode,
									 pstData->dwDetectCode,
									 strImageName.operator PCTSTR(),
									 strNowTime,
									 m_szSecurityKey );

				////////////////////////////////////////////////////////////////////////
				// Send strURLObject
				HINTERNET hObject = ::HttpOpenRequest( hConnect,
													   NULL,
													   strURLObject,
													   _T("HTTP/1.1"),
													   NULL, NULL,
													   INTERNET_FLAG_RELOAD, NULL);

				if( hObject == NULL )
				{
					nReturn = -3;
				}
				else
				{
					if(::HttpSendRequest(hObject, NULL, 0L, NULL, 0) == FALSE )
					{
						nReturn = -4;
					}
					else
					{
						DWORD dwRead = 0;
						char szRet  [1024] = "\0";

						::InternetReadFile(hObject, szRet, 1023, &dwRead);

						nReturn = dwRead;
					}
					::InternetCloseHandle(hObject);
				}
				::InternetCloseHandle(hConnect);
			}	
			::InternetCloseHandle(hSession);
		}
	}

	return nReturn;
}


// TCHAR szServerName[] = _T("www.xxxxx.co.kr");        //http:// 를 빼고 넣어야 합니다.
// TCHAR szFormAction[] = _T("Member.aspx");
// TCHAR szHeader[500] = _T("Content-Type: application/x-www-form-urlencoded\r\n");
// char szPostData[200]=("PhoneNumber=01012345678&ID=happy2012");    //유니코드 문자열이 아닙니다.
// LPCWSTR accept[2]={_T("*/*"), NULL};
// char szPDAPhoneNumber[50];
// char szDeviceName[50];
// 
// DWORD_PTR dwContext=1;
// LPTSTR  m_html = NULL ;
// BOOL  bResult;
// DWORD  dwSize;
// 
// HINTERNET hInternet = InternetOpen( _T("HTTP-POST"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
// 
// HINTERNET hConnect = InternetConnect( hInternet, szServerName, INTERNET_DEFAULT_HTTP_PORT, NULL, _T("HTTP/1.1"), INTERNET_SERVICE_HTTP, 0, dwContext);
// 
// HINTERNET hRequest = HttpOpenRequest( hConnect, _T("POST"), szFormAction, _T("HTTP/1.1"), NULL, accept,INTERNET_FLAG_DONT_CACHE, dwContext);
// 
// WideCharToMultiByte(CP_ACP, 0, g_szPDAPhoneNumber, -1, szPDAPhoneNumber, 50, NULL, NULL);
// WideCharToMultiByte(CP_ACP, 0, g_szDeviceName, -1, szDeviceName, 50, NULL, NULL);
// 
// sprintf(szPostData, "SmartPhoneNumber=%s&DeviceName=%s", szPDAPhoneNumber, szDeviceName);
// //Header를 두개 이상 사용할 경우 ---
// bResult = HttpAddRequestHeaders(hRequest, szHeader, _tcslen(szHeader), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
// //PostData
// bResult = HttpSendRequest( hRequest, NULL, 0, (LPVOID) szPostData, strlen(szPostData));
// 
// BOOL bQuery = InternetQueryDataAvailable( hRequest, &dwSize, 0, 0 );
// if ( bQuery && dwSize > 0 )
// {
// 	DWORD dwTotal = 0;
// 	char* pData = (char*)malloc( dwTotal + dwSize + 1 );
// 	while ( pData && dwSize )
// 	{
// 		DWORD dwRead = 0;
// 		InternetReadFile( hRequest, pData+dwTotal, dwSize, &dwRead );
// 		dwTotal += dwRead;
// 		pData[dwTotal] = NULL;
// 
// 		InternetQueryDataAvailable( hRequest, &dwSize, 0, 0 );
// 		if ( dwSize > 0 )
// 		{
// 			pData = (char*)realloc( pData, dwTotal + dwSize+1 );
// 		}
// 	}
// 	if ( pData )
// 	{
// 		int len = MultiByteToWideChar( CP_UTF8, 0, pData, dwTotal, NULL, NULL );
// 		m_html = (LPWSTR)calloc( len+1, sizeof(WCHAR) );
// 		if ( m_html )
// 		{
// 			MultiByteToWideChar( CP_UTF8, 0, pData, dwTotal, m_html, len+1 );
// 		}
// 		free(pData);
// 	}
// }
// 
// InternetCloseHandle( hRequest );
// InternetCloseHandle( hConnect );
// InternetCloseHandle( hInternet );
// 
// return TRUE;
//  }
//  return (FALSE);
// }
