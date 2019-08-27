// WebCallThread.cpp: implementation of the CWebCallThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CWebCallThread.h"
#include <functional>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebCallThread::CWebCallThread()
{
	ClearMember();
}

CWebCallThread::~CWebCallThread()
{
	Destroy();
}

void CWebCallThread::ClearMember()
{
	m_bThreadWorking = FALSE;
	m_bBusy = FALSE;
	m_bResult = FALSE;
	m_bHttps = false;
	m_bPopup = false;
}

void CWebCallThread::Destroy()
{
	TerminateThread();
	ClearMember();
}


//Json::Value root;
//Json::Value item;
//Json::StyledWriter writer;
//item["type"]=11;
//item["pid"]="abcdefg12345678";
//root["loginReq"]=item;
//std::string sJsonData=writer.write(root);

BOOL CWebCallThread::CreateThread(std::string sAddress, int nPort, std::string sPost, std::string sJson, bool bPostOrGet, bool bHttps/* =false */)
{
	if(m_pThread)
		return FALSE;
	if(m_bThreadWorking)
		return FALSE;
	m_sAddress=sAddress;
	m_nPort=nPort;
	m_sPost=sPost;
	m_sJson=sJson;
	m_bHttps=bHttps;
	m_sReturn.clear();

	// 쓰레드를 서스펜드 모드로 생성한다(쓰레드 프로시저에서 사용될 포인터의 초기화를 기다려야하기때문)
	if(true==bPostOrGet){
		m_pThread = AfxBeginThread(WebCallThreadFunc_Post, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	}
	else{
		m_pThread = AfxBeginThread(WebCallThreadFunc_Get, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	}
	if(m_pThread == NULL) 
	{
		m_bThreadWorking = FALSE;
		return FALSE;
	}

	// 쓰레드 가동
	m_bThreadWorking = TRUE;
	m_pThread->ResumeThread();
	ErrorLogConsole("CreateThread");
	return TRUE;
}

BOOL CWebCallThread::TerminateThread()
{
	if(!m_pThread)
		return FALSE;

	// 쓰레드 종료 플래그 세팅

	if(m_pThread && m_pThread->m_hThread)
	{
		// 쓰레드 종료를 0.1초간 기다림 ( 100 )
		DWORD dwRetCode = WaitForSingleObject(m_pThread->m_hThread, INFINITE );

		if(dwRetCode == WAIT_OBJECT_0)
		{
			// 정상적인 종료를 수행하였음
		}
		else if(dwRetCode == WAIT_TIMEOUT)
		{
			DWORD ThreadID=0;
			if(0!=GetExitCodeThread(m_pThread->m_hThread, &ThreadID)){
				if(STILL_ACTIVE==ThreadID) ::TerminateThread(m_pThread->m_hThread, ThreadID);
				if(NULL!=m_pThread->m_hThread){
					CloseHandle(m_pThread->m_hThread);
					m_pThread->m_hThread=NULL;
				}
			}
			//// 쓰레드가 시간내에 저절로 종료되지 않으면 강제로 종료시킨다
			//::TerminateThread(m_pThread->m_hThread, 0);
		}
	}
	// 종결 처리
	m_bThreadWorking = FALSE;
	m_pThread = NULL;
	return TRUE;
}

UINT CWebCallThread::WebCallThreadFunc_Post(LPVOID pParam)
{
	CWebCallThread* pThis = (CWebCallThread*)pParam;

	int nReturn=pThis->DoThreadLoop_Post();
	return nReturn;
}

UINT CWebCallThread::WebCallThreadFunc_Get(LPVOID pParam)
{
	CWebCallThread* pThis = (CWebCallThread*)pParam;

	int nReturn=pThis->DoThreadLoop_Get();
	return nReturn;
}

UINT CWebCallThread::DoThreadLoop_Post()
{
	m_bBusy = TRUE;
	HINTERNET hInternet = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	hInternet = ::InternetOpen(_T("HTTPPOST"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(NULL!=hInternet)
	{
		unsigned int nFlag = 0;
		if( m_bHttps == false )
		{
			nFlag = INTERNET_FLAG_RELOAD | INTERNET_FLAG_EXISTING_CONNECT;
		}
		else
		{
			nFlag = INTERNET_FLAG_SECURE;
		}
		hConnect = InternetConnect(hInternet, _T(m_sAddress.c_str()), m_nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
		hRequest = HttpOpenRequest(hConnect, _T("POST"), _T(m_sPost.c_str()), HTTP_VERSION, NULL, NULL, nFlag, 0);
		if(NULL!=hInternet)
		{
			if( m_bHttps == true )
			{
				DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | \
					SECURITY_FLAG_IGNORE_REVOCATION             | \
					SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP       | \
					SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS      | \
					SECURITY_FLAG_IGNORE_CERT_DATE_INVALID      | \
					SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
				InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));			
			}
			bool bReturn=false;
			try{
				HttpSendRequest(hRequest, NULL, 0, (LPVOID)m_sJson.c_str(), m_sJson.size());
			}
			catch(...){
				goto EndLoop;
			}
			//char *JsonData = "{\"loginReq\":{\"type\": 11, \"pid\": \"abcdefg12345678\"}}"; // your actual JSON data here
			//HttpSendRequest(hRequest, NULL, 0, JsonData, strlen(JsonData));
			DWORD StatusCode = 0;
			DWORD StatusCodeLen = sizeof(StatusCode);
			HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &StatusCode, &StatusCodeLen, NULL);
			if(StatusCode == 200){
				//request 대한 response를 받는 부분
				int bufSize=4096;
				DWORD dwBytesRead = 0;
				char* pszBuf = (char *)malloc(sizeof(char) * bufSize + 1);
				memset(pszBuf, '\0', bufSize + 1);
				m_sReturn.clear();
				BOOL bRead = FALSE;
				do {
					bRead = InternetReadFile(hRequest, pszBuf, bufSize, &dwBytesRead);
					if ( (bRead) && (dwBytesRead>0) ){
						m_sReturn.append(pszBuf);
						memset(pszBuf, '\0', bufSize + 1);
					}
				} while ((bRead == TRUE) && (dwBytesRead > 0));
				// use InternetQueryDataAvailable() and InternetReadFile()
				// to read any response data as needed...
				bReturn=true;
			}
			if(true==bReturn){
				//try{
				//	Json::Value root;
				//	Json::Value item;
				//	Json::Reader reader;
				//	bool bRet=reader.parse(m_sReturn, root);
				//	if(false==bRet) goto EndLoop;
				//	bool bSuccess=root["success"].asBool();
				//	int nCode=root["code"].asInt();
				//	item=root["result"];
				//	for(Json::Value::iterator it=item.begin(); it!=item.end(); it++){
				//		std::string sResult=(*it)["result"].asString();
				//		std::string sServerIp=(*it)["serverIP"].asString();
				//		int nServerPort=(*it)["serverPort"].asInt();
				//	}
				//}
				//catch(...){
				//	goto EndLoop;
				//}
				MM().Call(UPG_WEB_CALL_LIST, 1, 0);
			}
			else{
				MM().Call(UPG_WEB_CALL_LIST, 0, 0);
			}
		}
	}
EndLoop:
	if(NULL!=hRequest){
		InternetCloseHandle(hRequest);
	}
	if(NULL!=hConnect){
		InternetCloseHandle(hConnect);
	}
	if(NULL!=hInternet){
		InternetCloseHandle(hInternet);
	}
	// 쓰레드 작동 정지
	m_bThreadWorking = FALSE;
	m_bBusy			 = FALSE;
	m_pThread = NULL;
	return 0;
}


UINT CWebCallThread::DoThreadLoop_Get()
{
	m_bBusy = TRUE;
	m_bResult=FALSE;
	HINTERNET hInternet = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	//hInternet = ::InternetOpen(_T("HTTPGET"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hInternet = ::InternetOpen(_T("HTTPGET"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);	
	if(NULL!=hInternet){
		//hConnect = InternetConnect(hInternet, _T(m_sAddress.c_str()), m_nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL); //http://
		//hRequest = HttpOpenRequest(hConnect, _T("GET"), _T(m_sPost.c_str()), HTTP_VERSION, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_EXISTING_CONNECT, 0);
		int nFlag = INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		hConnect = InternetConnect(hInternet, _T(m_sAddress.c_str()), m_nPort, _T("yo7464"), _T("#tkfkdgo02"), INTERNET_SERVICE_HTTP, 0, 0); //https::
		//hConnect = InternetConnect(hInternet, _T(m_sAddress.c_str()), m_nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0); //https::
		hRequest = HttpOpenRequest(hConnect, _T("GET"), _T(m_sPost.c_str()), HTTP_VERSION, NULL, NULL, nFlag, 0);		
		if(NULL!=hRequest){
			DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | \
				SECURITY_FLAG_IGNORE_REVOCATION             | \
				SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP       | \
				SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS      | \
				SECURITY_FLAG_IGNORE_CERT_DATE_INVALID      | \
				SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
			InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

			BOOL bisRead=HttpSendRequest(hRequest, NULL, 0, NULL, 0);
			if(0==bisRead){
				DWORD dwError=GetLastError();
				ErrorLogConsole("GetLastError %d", dwError);
			}
			DWORD StatusCode = 0;
			DWORD StatusCodeLen = sizeof(StatusCode);
			HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &StatusCode, &StatusCodeLen, NULL);			
			if(StatusCode == HTTP_STATUS_OK){
				//request 대한 response를 받는 부분
				int bufSize=4096*2;
				DWORD dwBytesRead = 0;
				char* pszBuf = (char *)malloc(sizeof(char) * bufSize + 1);
				memset(pszBuf, '\0', bufSize + 1);
				m_sReturn.clear();
				BOOL bRead = FALSE;
				do {
					bRead = InternetReadFile(hRequest, pszBuf, bufSize, &dwBytesRead);
					if ( (bRead) && (dwBytesRead>0) ){
						m_sReturn.append(pszBuf);
						memset(pszBuf, '\0', bufSize + 1);
					}
				} while ((bRead == TRUE) && (dwBytesRead > 0));
				// use InternetQueryDataAvailable() and InternetReadFile()
				// to read any response data as needed...
				m_bResult=TRUE;
			}
		}
	}
	if(NULL!=hRequest){
		InternetCloseHandle(hRequest);
	}
	if(NULL!=hConnect){
		InternetCloseHandle(hConnect);
	}
	if(NULL!=hInternet){
		InternetCloseHandle(hInternet);
	}
#if defined(_DEBUG)
	m_bResult=true;
#endif
	MM().Call(UPG_WEB_CALL_LIST, m_bResult, 0);
	// 쓰레드 작동 정지
	m_bThreadWorking = FALSE;
	m_bBusy			 = FALSE;
	m_pThread = NULL;
	return 0;
}

bool CWebCallThread::MakeMemoData()
{
	m_BackupMemoData = m_MemoData;
	try{
		Json::Value root;
		Json::Value item;
		Json::Reader reader;
		bool bRet=reader.parse(m_sReturn, root);
		if(true==bRet){
			m_MemoData.m_bSuccess=root["success"].asBool();
			m_MemoData.m_sCode=root["code"].asString();
			m_MemoData.m_sMessage=root["msg"].asString();
			m_MemoData.m_nTotalCount=root["totalCount"].asInt();
		}
	}
	catch(...){
		return false;
	}
	return true;
}