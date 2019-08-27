#include "stdafx.h"
#include "SockMan.h"
#include "CommonUserWndMsg.h"
#include "dbghelp.h"
#pragma comment(lib,"dbghelp.lib")
#define TRACE_MAX_STACK_FRAMES 1024
#define TRACE_MAX_FUNCTION_NAME_LENGTH 1024

namespace NMBASE
{
	namespace SOCK
	{
		//yoo
		pFuncI_Ptr CSockMan::m_pFuncI_Ptr_Type=NULL;
		std::string CSockMan::m_sSocketTxt="socket.txt";
		std::string CSockMan::m_sSocketLogTxt="";
		void CSockMan::SetSocketTxt(std::string sSocketTxt)
		{
			m_sSocketTxt=sSocketTxt;
		}
		void CSockMan::SetSocketTxt(int nProcessCnt)
		{
			char cBuff[_MAX_PATH]={0,};
			sprintf(cBuff, "[%02d]", nProcessCnt);
			m_sSocketTxt=std::string(cBuff)+"socket.txt";
		}
		std::string CSockMan::GetSocketTxt(void)
		{
			return m_sSocketTxt;
		}
		void CSockMan::SetSocketLogTxt(std::string sSocketLogTxt)
		{
			m_sSocketLogTxt=sSocketLogTxt;
		}
		std::string CSockMan::GetSocketLogTxt(void)
		{
			return m_sSocketLogTxt;
		}
		CSockMan& GSOCKMAN()
		{
			return CSockMan::Get();
		}

		CSockMan& CSockMan::Get()
		{
			static CSockMan sSocketManager;
			return sSocketManager;
		}

		CSockMan::CSockMan()
		{
			//AfxSocketInit();	///< 윈속DLL 초기화(MFC에서는 자동으로 해주지만 API상에서도 쓰일지 모르므로)
			if(!AfxSocketInit())
			{
				AfxMessageBox("Windows 소켓 초기화에 실패하였습니다.");		
			}
		}

		CSockMan::~CSockMan()
		{
			WSACleanup();		///< 윈속DLL 클린업
			Destroy();
		}


		void CSockMan::Init(HWND hwnd)
		{
			Destroy();

			m_hWindow = hwnd;	///< PostMessage보낼 윈도우 핸들 
			CheckHostInfo();	///< Host 정보를 체크한다
		}

		///< 생성된 모든 소켓과 클래스를 파괴
		void CSockMan::Destroy()
		{
			if(!m_mapSock.empty())
			{
				for(SOCKMAP_IT itr = m_mapSock.begin(); itr != m_mapSock.end(); ++itr )
				{
					_ASSERTE( itr->second != NULL );
					itr->second->Close();
					delete itr->second;			
				}

				m_mapSock.clear();
			}
		}


		BOOL CSockMan::PostCloseMsg(char* szSockName)
		{
			SOCKMAP_IT itr = m_mapSock.find( std::string(szSockName) );

			if( itr != m_mapSock.end() ) 
			{
				if(m_mapSock[std::string(szSockName)]->IsPostCloseMsg() == TRUE)
				{
					//TRACE("CSockMan::PostCloseMsg() - 이미 소켓 닫기 메세지를 보냈음(m_nSockCName=%s)\n", m_mapSock[std::string(szSockName)]->GetSockIDName());
#ifdef TRACE_OUTPUT		
					char szTemp[256] = {0, };
					sprintf(szTemp, "CSockMan::PostCloseMsg() - 이미 소켓 닫기 메세지를 보냈음(m_nSockCName=%s)\n", m_mapSock[std::string(szSockName)]->GetSockIDName());
					TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

					return FALSE;
				}

				PostMessage(m_hWindow, UM_DATA_CLOSE, NULL, (long)this); 
				m_mapSock[std::string(szSockName)]->SetPostCloseMsg(TRUE);

				return TRUE;
			}

			return FALSE;	
		}


		void CSockMan::CheckHostInfo()
		{
			///< 호스트 정보 조회
			char strHostName[256] = {0, };
			char strPort[10] = {0, };

			PHOSTENT pHostEntry = NULL;
			IN_ADDR inAddr;
			memset(&inAddr, 0, sizeof(IN_ADDR));

			if(gethostname(strHostName, sizeof(strHostName)) != 0) 
			{
				m_strError.Format("gethostname() error : %d", WSAGetLastError());
			}
			else
			{
				pHostEntry = gethostbyname(strHostName);
				if(pHostEntry==NULL)
				{
					m_strError.Format("gethostbyname() error : %d", WSAGetLastError());
				}
				else 
				{
					memcpy(&inAddr, pHostEntry->h_addr, 4);
				}
			}

			m_strSrvIP = inet_ntoa(inAddr);			///< 호스트의 IP를 문자열로 변환

			if(pHostEntry)
			{
				m_strDomain = pHostEntry->h_name;	///< 호스트의 Domain Name을 얻음
			}
			else
			{
				m_strDomain = "";
			}

			m_strPort = strPort;					///< 변환된 포트번호를 문자열로 저장
		}

		///< 클라이언트용으로 데이터 소켓을 생성, 원하는 주소에 연결시킨다
		///< -1을 리턴하면 접속 실패를 뜻하지만 실제 결과는 나중에 메세지로 통보된다
		///< (접속 성패 여부는 UM_DATA_CONNECT 혹은 UM_DATA_CONNECT_FAILURE 메세지로 알 수 있다)
		LPCTSTR CSockMan::ConnectSocket(char *strIP, UINT nPortNum)
		{
			// 접속한 서버의 Name을 생성한다.
			m_strServerName.Format("%s:%d", strIP, nPortNum);

			CDataSock* pTempSock = NULL;

			if(m_mapSock.find(m_strServerName.GetBuffer()) == m_mapSock.end())
			{
				CDataSock* pNewSock = new CDataSock();

				m_mapSock[m_strServerName.GetBuffer()] = pNewSock;
				_ASSERTE( m_mapSock.find(m_strServerName.GetBuffer()) != m_mapSock.end());

				pTempSock = pNewSock;
			}
			else
			{
				pTempSock = m_mapSock[m_strServerName.GetBuffer()];
				pTempSock->Close();	 
			}


			if(pTempSock && pTempSock->ConnectSocket(m_hWindow, strIP, nPortNum, m_strServerName.GetBuffer()) == FALSE)
			{
				AfxMessageBox(pTempSock->GetErrorString());		

				// Map에서 삭제 한다.
				CloseDataSocket(m_strServerName.GetBuffer());

				//TRACE("CSockMan::ConnectSocket() - 소켓 생성 개수 초과\n");
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CSockMan::ConnectSocket() - Socket Connect Fail");
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT
			
				m_strServerName = CONNECT_FAIL;
				return (LPCTSTR)m_strServerName;
			}

#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CSockMan::ConnectSocket() - Name:%s, IP:%s, Port:%d", m_strServerName.GetBuffer(), strIP, nPortNum);
			TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

			return (LPCTSTR)m_strServerName;	
		}


		CDataSock* CSockMan::GetDataSocketPtr(char* szSockName)
		{
			//_ASSERTE( m_mapSock.find( std::string(szSockName) ) != m_mapSock.end() );

			if(m_mapSock.find(std::string(szSockName) ) == m_mapSock.end()) 
			{
				//TRACE("CSockMan::Socket():: no socket\n");
#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CSockMan::Socket():: no socket");
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

				return NULL;
			}

			return m_mapSock[std::string(szSockName)];
		}


		BOOL CSockMan::SendData(char* szSockName, void *pdata, int size)
		{
			SOCKMAP_IT itr = m_mapSock.find( std::string(szSockName) );
			if( itr != m_mapSock.end() ) 
			{
				SPACKETHEADER* phead = (SPACKETHEADER*)pdata;

				if(phead)
				{
#ifdef TRACE_OUTPUT		
					char szTemp[256] = {0, };
					sprintf(szTemp, "CSockMan::SendData() - Signal : %d", phead->Signal);
					TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT
				}

				BOOL bResult = m_mapSock[ std::string(szSockName) ]->SendData(pdata, size);
				CString sendLog;
				//yoo
				SYSTEMTIME systemTime;
				GetLocalTime(&systemTime);
				CString timeStr;
				timeStr.Format("%04d.%02d.%02d.%02d:%02d.%02d\n"
					,systemTime.wYear 
					,systemTime.wMonth
					,systemTime.wDay
					,systemTime.wHour
					,systemTime.wMinute
					,systemTime.wSecond);
				if(NULL!=CSockMan::m_pFuncI_Ptr_Type){
					ErrorLogError(CSockMan::GetSocketTxt().c_str(), "Send = %s, %d, %s, %s", CSockMan::m_pFuncI_Ptr_Type(phead->Signal).c_str(), phead->MsgLen, pdata, szSockName);
					sendLog.Format("Send = %s, %d, %s, %s, %s, %d\r\n", (char*)CSockMan::m_pFuncI_Ptr_Type(phead->Signal).c_str(), phead->MsgLen, pdata, szSockName, timeStr, (int)bResult); 
				}
				else{
					ErrorLogError(CSockMan::GetSocketTxt().c_str(), "Send = %d, %d, %s, %s", phead->Signal, phead->MsgLen, pdata, szSockName);
					sendLog.Format("Send = %d, %d, %s, %s, %s, %d\r\n", phead->Signal, phead->MsgLen, pdata, szSockName, timeStr, (int)bResult); 
				}
				sendLog.AppendFormat("SendBuff %d, %d, RecvBuff %d, %d\r\n",
					m_mapSock[ std::string(szSockName) ]->GetSendBuf()->GetBufSize(), m_mapSock[ std::string(szSockName) ]->GetSendBuf()->GetUsedSize(),
					m_mapSock[ std::string(szSockName) ]->GetRecvBuf()->GetBufSize(), m_mapSock[ std::string(szSockName) ]->GetRecvBuf()->GetUsedSize());
				m_sSocketLogTxt.append(sendLog);

				return bResult;
			}

			return FALSE;
		}


		///< 소켓으로부터 데이터를 받는다
		int CSockMan::ReceiveData(char* szSockName)				
		{
			SOCKMAP_IT itr = m_mapSock.find( std::string(szSockName) );
			if( itr != m_mapSock.end() ) 
			{
				int nByte = m_mapSock[std::string(szSockName)]->ReceiveData();

				return nByte;
			}

			return -1;
		}


		///< 소켓 클래스ID로 소켓 삭제 - Overrided
		BOOL CSockMan::CloseDataSocket(const char* szSockName)			
		{
			CString strSockName = szSockName;

			{
				// 강제 소켓 close callstack
				CString callstackLog;
				callstackLog.Format("\n\nprintStackTrace()\n\n");
				void *stack[TRACE_MAX_STACK_FRAMES];
				HANDLE process = GetCurrentProcess();
				SymInitialize(process, NULL, TRUE);
				WORD numberOfFrames = CaptureStackBackTrace(0, TRACE_MAX_STACK_FRAMES, stack, NULL);
				SYMBOL_INFO *symbol = (SYMBOL_INFO *)malloc(sizeof(SYMBOL_INFO)+(TRACE_MAX_FUNCTION_NAME_LENGTH - 1) * sizeof(TCHAR));
				symbol->MaxNameLen = TRACE_MAX_FUNCTION_NAME_LENGTH;
				symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
				DWORD displacement;
				IMAGEHLP_LINE64 *line = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
				line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
				for (int i = 0; i < numberOfFrames; i++)
				{
					DWORD64 address = (DWORD64)(stack[i]);
					SymFromAddr(process, address, NULL, symbol);
					if (SymGetLineFromAddr64(process, address, &displacement, line))
					{
						callstackLog.AppendFormat("\tat %s in %s: line: %lu: address: 0x%0X\n", symbol->Name, line->FileName, line->LineNumber, symbol->Address);
					}
					else
					{
						callstackLog.AppendFormat("\tSymGetLineFromAddr64 returned error code %lu.\n", GetLastError());
						callstackLog.AppendFormat("\tat %s, address 0x%0X.\n", symbol->Name, symbol->Address);
					}
				}
				NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(callstackLog.GetString());
				///////////////////////////////////////
			}

			SOCKMAP_IT itr = m_mapSock.find( std::string(szSockName) );
			if( itr != m_mapSock.end() ) 
			{
				m_mapSock[std::string(szSockName)]->Close();

				delete itr->second;
				itr->second = NULL;
				m_mapSock.erase( itr );

#ifdef TRACE_OUTPUT		
				char szTemp[256] = {0, };
				sprintf(szTemp, "CSockMan::CloseDataSocket() - Success:%s", strSockName.GetBuffer());
				TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT


				return TRUE;
			}

			return FALSE;
		}


		void CSockMan::CloseAllDataSocket()
		{
			if(!m_mapSock.empty())
			{
				for(SOCKMAP_IT itr = m_mapSock.begin(); itr != m_mapSock.end(); ++itr )
				{
					_ASSERTE( itr->second != NULL );
					itr->second->Close();
					delete itr->second;			
				}

				m_mapSock.clear();
			}
		}


		///< 서버의 IP를 구함
		char* CSockMan::GetHostIP()
		{
			return ((char*)m_strSrvIP.operator LPCTSTR());
		}

		///< 서버의 도메인을 구함
		char* CSockMan::GetHostDomain()
		{
			return ((char*)m_strDomain.operator LPCTSTR());
		}

		///< 포트 번호를 문자열로 넘겨줌
		char* CSockMan::GetHostPortStr()
		{
			return ((char*)m_strPort.operator LPCTSTR());
		}


		///< 에러 메세지 처리 
		void CSockMan::Error(char *strErr)
		{
#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CSockMan::Error() - Error:%s", strErr);
			TTrace::Debug()->Send(szTemp, "nmsock");   // single byte string	
#endif	// TRACE_OUTPUT

			AfxMessageBox(strErr);
		}

		///< 접속한 서버의 IP문자열을 얻는다.	
		LPCTSTR CSockMan::GetServerIP(char* szSockName)
		{
			CDataSock* pTemp = GetDataSocketPtr(szSockName);

			if(pTemp)
			{
				return pTemp->GetServerIP();
			}

			return NULL;	
		}


		UINT CSockMan::GetServerPort(char* szSockName)
		{
			CDataSock* pTemp = GetDataSocketPtr(szSockName);

			if(pTemp)
			{
				return pTemp->GetServerPort();
			}

			return 0;
		}

	} //namespace SOCK

} // namespace NMBASE



namespace LOG
{

	void* CConsoleManager::m_pConsole_In=INVALID_HANDLE_VALUE;
	void* CConsoleManager::m_pConsole_Out=INVALID_HANDLE_VALUE;
	CCritical_Section CConsoleManager::m_cCS;
	CONSOLE_SCREEN_BUFFER_INFO m_ScreenBufferInfo;
	void CConsoleManager::InitConsole(void)
	{
		ZeroMemory(&m_ScreenBufferInfo, sizeof(CONSOLE_SCREEN_BUFFER_INFO));
	}
	void CConsoleManager::FreeGonsole(void)
	{
		UnSetConsole();
		//::OleUninitialize();
	}
	void CConsoleManager::SetConsole(void)
	{
#if defined(_DEBUG)
		//if(false==IsDebuggerPresent()) return;
		//if(S_OK!=::OleInitialize(NULL)) return;
		//if(NULL==GetConsoleWindow()){
		//	if(0==AllocConsole()) return;
		//}
		::AllocConsole();
		m_pConsole_In=::GetStdHandle(STD_INPUT_HANDLE);
		m_pConsole_Out=::GetStdHandle(STD_OUTPUT_HANDLE);
		//if((NULL==m_pConsole_In)&&(NULL==m_pConsole_Out)){
		//	m_pConsole_In=::GetStdHandle(STD_INPUT_HANDLE);
		//	m_pConsole_Out=::GetStdHandle(STD_OUTPUT_HANDLE);
		//}

		if((INVALID_HANDLE_VALUE!=m_pConsole_In)&&(INVALID_HANDLE_VALUE!=m_pConsole_Out)){
			GetConsoleScreenBufferInfo(m_pConsole_Out, &m_ScreenBufferInfo);
			SetConsoleMode(m_pConsole_In, ENABLE_PROCESSED_INPUT|ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT);
			SetConsoleMode(m_pConsole_Out, ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT);
			::SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandlerRoutine, 1);
		}
#endif
	}
	void CConsoleManager::UnSetConsole(void)
	{
#if defined(_DEBUG)
		SYNCHRONIZED(m_cCS);
		BOOL bFree=FALSE;
		if(INVALID_HANDLE_VALUE!=m_pConsole_In){
			::CloseHandle(m_pConsole_In);
			m_pConsole_In=INVALID_HANDLE_VALUE;
			bFree=TRUE;
		}
		if(INVALID_HANDLE_VALUE!=m_pConsole_Out){
			::SetConsoleTextAttribute(m_pConsole_Out, m_ScreenBufferInfo.wAttributes);
			::CloseHandle(m_pConsole_Out);
			m_pConsole_Out=INVALID_HANDLE_VALUE;
		}
		if(TRUE==bFree){
			::FreeConsole();
		}
#endif
	}
	bool CConsoleManager::SetConsoleColor(int nForeGroundColor, int nBackGroundColor)
	{
		SYNCHRONIZED(m_cCS);
		CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
		if(INVALID_HANDLE_VALUE==m_pConsole_Out){
			return false;
		}
		if(false==::GetConsoleScreenBufferInfo(m_pConsole_Out, &ScreenBufferInfo)){
			DWORD dwError=::GetLastError();
			return false;
		}
		unsigned short ForegroundcolorDefault=ScreenBufferInfo.wAttributes&0xFF0F; 
		unsigned short BackgroundcolorDefault=ScreenBufferInfo.wAttributes&0xFFF0;
		unsigned short ForegroundcolorNew=(unsigned short)((CONSOLECOLOR_FOREGROUND_DEFAULT==nForeGroundColor)?(ForegroundcolorDefault):(nForeGroundColor));
		unsigned short BackgroundcolorNew=(unsigned short)((CONSOLECOLOR_BACKGROUND_DEFAULT==nBackGroundColor)?(BackgroundcolorDefault):(nBackGroundColor));
		::SetConsoleTextAttribute(m_pConsole_Out, ForegroundcolorNew|BackgroundcolorNew);
		return true;
	}
	void CConsoleManager::ResetConsoleColor(void)
	{
		if(INVALID_HANDLE_VALUE!=m_pConsole_Out){
			::SetConsoleTextAttribute(m_pConsole_Out, m_ScreenBufferInfo.wAttributes);
		}
	}
	bool CConsoleManager::SetConsolePos(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
		if(INVALID_HANDLE_VALUE==m_pConsole_Out){
			return false;
		}
		if(false==::GetConsoleScreenBufferInfo(m_pConsole_Out, &ScreenBufferInfo)){
			DWORD dwError=::GetLastError();
			return false;
		}
		COORD Coord=::GetLargestConsoleWindowSize(m_pConsole_Out);
		SMALL_RECT Rect={0,};
		Rect.Left=0;
		Rect.Top=0;
		Rect.Right=Coord.X;
		Rect.Bottom=Coord.Y;
		::SetConsoleWindowInfo(m_pConsole_Out, true, &Rect);
		return true;
	}
	bool CConsoleManager::WriteFormatConsole(const char* format, ...)
	{
		SYNCHRONIZED(m_cCS);
		unsigned long written=0;
		char Stirng[2048]={0,};
		int* arg=NULL;
		arg=(int*)&format;
		arg+=(sizeof(const char*)/sizeof(int));
		vsprintf(Stirng, format, (char*)arg);
		return CConsoleManager::ConsoleWrite(Stirng, (int)strlen(Stirng), &written);
	}
	bool CConsoleManager::WriteFormatConsole(const char* format, va_list ArgPtr)
	{
		SYNCHRONIZED(m_cCS);
		unsigned long written=0;
		char Stirng[2048]={0,};
		vsprintf(Stirng, format, ArgPtr);
		return CConsoleManager::ConsoleWrite(Stirng, (int)strlen(Stirng), &written);
	}
	bool CConsoleManager::WriteFormatConsoleColor(int nForeGroundColor, int nBackGroundColor, const char* format, ...)
	{
		SYNCHRONIZED(m_cCS);
		CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
		if(INVALID_HANDLE_VALUE==m_pConsole_Out){
			return false;
		}
		if(false==::GetConsoleScreenBufferInfo(m_pConsole_Out, &ScreenBufferInfo)){
			DWORD dwError=::GetLastError();
			return false;
		}
		unsigned short ForegroundcolorDefault=ScreenBufferInfo.wAttributes&0xFF0F; 
		unsigned short BackgroundcolorDefault=ScreenBufferInfo.wAttributes&0xFFF0;
		unsigned short ForegroundcolorNew=((CONSOLECOLOR_FOREGROUND_DEFAULT==nForeGroundColor)?(ForegroundcolorDefault):(nForeGroundColor));
		unsigned short BackgroundcolorNew=((CONSOLECOLOR_BACKGROUND_DEFAULT==nBackGroundColor)?(BackgroundcolorDefault):(nBackGroundColor));
		::SetConsoleTextAttribute(m_pConsole_Out, ForegroundcolorNew|BackgroundcolorNew);
		unsigned long written=0;
		char Stirng[2048]={0,};
		int* arg=NULL;
		arg=(int*)&format;
		arg+=(sizeof(const char*)/sizeof(int));
		vsprintf(Stirng, format, (char*)arg);
		bool bReturn=false;
		if(0!=::WriteConsole(m_pConsole_Out, Stirng, (int)strlen(Stirng), &written, NULL)){
			bReturn=true;
		}
		::SetConsoleTextAttribute(m_pConsole_Out, m_ScreenBufferInfo.wAttributes);
		return bReturn;
	}
	bool CConsoleManager::WriteFormatConsoleColor(int nForeGroundColor, int nBackGroundColor, const char* format, va_list ArgPtr)
	{
		SYNCHRONIZED(m_cCS);
		CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
		if(INVALID_HANDLE_VALUE==m_pConsole_Out){
			return false;
		}
		if(false==::GetConsoleScreenBufferInfo(m_pConsole_Out, &ScreenBufferInfo)){
			DWORD dwError=::GetLastError();
			return false;
		}
		unsigned short ForegroundcolorDefault=ScreenBufferInfo.wAttributes&0xFF0F; 
		unsigned short BackgroundcolorDefault=ScreenBufferInfo.wAttributes&0xFFF0;
		unsigned short ForegroundcolorNew=((CONSOLECOLOR_FOREGROUND_DEFAULT==nForeGroundColor)?(ForegroundcolorDefault):(nForeGroundColor));
		unsigned short BackgroundcolorNew=((CONSOLECOLOR_BACKGROUND_DEFAULT==nBackGroundColor)?(BackgroundcolorDefault):(nBackGroundColor));
		::SetConsoleTextAttribute(m_pConsole_Out, ForegroundcolorNew|BackgroundcolorNew);
		unsigned long written=0;
		char Stirng[2048]={0,};
		vsprintf(Stirng, format, ArgPtr);
		bool bReturn=false;
		if(0!=::WriteConsole(m_pConsole_Out, Stirng, (int)strlen(Stirng), &written, NULL)){
			bReturn=true;
		}
		::SetConsoleTextAttribute(m_pConsole_Out, m_ScreenBufferInfo.wAttributes);
		return bReturn;
	}
	bool CConsoleManager::ConsoleWrite(char* pBuffer, int nBufferSize, DWORD* pWrite)
	{
		SYNCHRONIZED(m_cCS);
		if(INVALID_HANDLE_VALUE==m_pConsole_Out){
			return false;
		}
		::WriteConsole(m_pConsole_Out, pBuffer, nBufferSize, pWrite, NULL);
		//::WriteConsole(m_pConsole_Out, pBuffer, nBufferSize, pWrite, NULL);
		return true;
	}
	bool CConsoleManager::WriteTextColor(char* pText, int nForeGroundColor, int nBackGroundColor)
	{
		SYNCHRONIZED(m_cCS);
		static CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
		unsigned short ForegroundcolorDefault=0, BackgroundcolorDefault=0;
		unsigned short ForegroundcolorNew=0, BackgroundcolorNew=0;
		unsigned long ByteWitten=0;
		if(INVALID_HANDLE_VALUE==m_pConsole_Out){
			return false;
		}
		if(0!=::GetConsoleScreenBufferInfo(m_pConsole_Out, &ScreenBufferInfo)){
			ForegroundcolorDefault=ScreenBufferInfo.wAttributes&0xFF0F; 
			BackgroundcolorDefault=ScreenBufferInfo.wAttributes&0xFFF0;
			ForegroundcolorNew=((CONSOLECOLOR_FOREGROUND_DEFAULT==nForeGroundColor)?(ForegroundcolorDefault):(nForeGroundColor));
			BackgroundcolorNew=((CONSOLECOLOR_BACKGROUND_DEFAULT==nBackGroundColor)?(BackgroundcolorDefault):(nBackGroundColor));
			::SetConsoleTextAttribute(m_pConsole_Out, ForegroundcolorNew|BackgroundcolorNew);
			::WriteFile(m_pConsole_Out, pText, (DWORD)strlen(pText), (LPDWORD)&ByteWitten, 0);
			//resotre original attribute
			::SetConsoleTextAttribute(m_pConsole_Out, ScreenBufferInfo.wAttributes);
			return true;
		}
		DWORD dwError=::GetLastError();
		return false;
	}
	bool CConsoleManager::WriteText(char* pText)
	{
		SYNCHRONIZED(m_cCS);
		unsigned long ByteWitten=0;
		if(INVALID_HANDLE_VALUE==m_pConsole_Out){
			return false;
		}
		::WriteFile(m_pConsole_Out, pText, (DWORD)strlen(pText), (LPDWORD)&ByteWitten, 0);
		return true;
	}
	bool CConsoleManager::ConsoleRead(char* pBuffer, int nBufferSize, DWORD* pRead)
	{
		SYNCHRONIZED(m_cCS);
		if(INVALID_HANDLE_VALUE==m_pConsole_In){
			return false;
		}
		::ReadConsole(m_pConsole_In, pBuffer, nBufferSize, pRead, NULL);
		return true;
	}
	void CConsoleManager::Flush(void)
	{
		::FlushConsoleInputBuffer(m_pConsole_Out);
	}
	void CConsoleManager::ClearScreen(void)
	{
		SYNCHRONIZED(m_cCS);
		DWORD y;
		COORD Coord;
		Coord.X=0;
		Coord.Y=0;
		::FillConsoleOutputCharacter(m_pConsole_In, ' ', 80*25, Coord, &y);
		::SetConsoleCursorPosition(m_pConsole_In, Coord);
	}
	void CConsoleManager::ConsoleEvent(void)
	{
		DWORD dw=0;
		INPUT_RECORD Input_Record={0,};
		while(true){
			ReadConsoleInput(m_pConsole_In, &Input_Record, 1, &dw);
			if((KEY_EVENT==Input_Record.EventType)&&(0!=Input_Record.Event.KeyEvent.bKeyDown)){
			}
			if(MOUSE_EVENT==Input_Record.EventType){
				if((FROM_LEFT_1ST_BUTTON_PRESSED&Input_Record.Event.MouseEvent.dwButtonState)&&(MOUSE_MOVED&Input_Record.Event.MouseEvent.dwEventFlags)){
				}
				if(Input_Record.Event.MouseEvent.dwEventFlags&DOUBLE_CLICK){
					break;
				}
			}
		}
	}
	int CConsoleManager::ConsoleHandlerRoutine(DWORD dwCtrlType)
	{
		switch(dwCtrlType){
		case CTRL_C_EVENT: //Ctrl+C
			{
				Sleep(0);
			}
			break;
		case CTRL_BREAK_EVENT: //Ctrl+Break
			{
				Sleep(0);
			}
			break;
		case CTRL_CLOSE_EVENT: //콘솔창이 닫힐때
			{
				//UnSetConsole();
				Sleep(0);
			}
			break;
		case CTRL_LOGOFF_EVENT: //사용자가 로그오프할때
			{
				Sleep(0);
			}
			break;
		case CTRL_SHUTDOWN_EVENT: //시스템이 셧다운일때
			{
				//UnSetConsole();
				Sleep(0);
			}
			break;
		}
		return false;
	}
	int CConsoleManager::ConvertColorRGBToConsoleFore(int Rgb)
	{
		switch(Rgb){
		case COLOR_RGB_WHITE: return CONSOLECOLOR_FOREGROUND_WHITE;
		case COLOR_RGB_BLACK: return CONSOLECOLOR_FOREGROUND_BLACK;
		case COLOR_RGB_GRAY: return CONSOLECOLOR_FOREGROUND_WHITE;
		case COLOR_RGB_RED: return CONSOLECOLOR_FOREGROUND_RED;
		case COLOR_RGB_GREEN: return CONSOLECOLOR_FOREGROUND_GREEN;
		case COLOR_RGB_BLUE: return CONSOLECOLOR_FOREGROUND_BLUE;
		case COLOR_RGB_YELLOW: return CONSOLECOLOR_FOREGROUND_YELLOW;
		case COLOR_RGB_CYAN: return CONSOLECOLOR_FOREGROUND_CYAN;
		case COLOR_RGB_MAGENTA: return CONSOLECOLOR_FOREGROUND_MAGENTA;
		case COLOR_RGB_SCARLETRED: return CONSOLECOLOR_FOREGROUND_RED;
		case COLOR_RGB_DARKGREEN: return CONSOLECOLOR_FOREGROUND_GREEN;
		case COLOR_RGB_NAVYBLUE: return CONSOLECOLOR_FOREGROUND_BLUE;
		}
		return CONSOLECOLOR_FOREGROUND_DEFAULT;
	}
	int CConsoleManager::ConvertColorRGBToConsoleBack(int Rgb)
	{
		switch(Rgb){
		case COLOR_RGB_WHITE: return CONSOLECOLOR_BACKGROUND_WHITE;
		case COLOR_RGB_BLACK: return CONSOLECOLOR_BACKGROUND_BLACK;
		case COLOR_RGB_GRAY: return CONSOLECOLOR_BACKGROUND_WHITE;
		case COLOR_RGB_RED: return CONSOLECOLOR_BACKGROUND_RED;
		case COLOR_RGB_GREEN: return CONSOLECOLOR_BACKGROUND_GREEN;
		case COLOR_RGB_BLUE: return CONSOLECOLOR_BACKGROUND_BLUE;
		case COLOR_RGB_YELLOW: return CONSOLECOLOR_BACKGROUND_YELLOW;
		case COLOR_RGB_CYAN: return CONSOLECOLOR_BACKGROUND_CYAN;
		case COLOR_RGB_MAGENTA: return CONSOLECOLOR_BACKGROUND_MAGENTA;
		case COLOR_RGB_SCARLETRED: return CONSOLECOLOR_BACKGROUND_RED;
		case COLOR_RGB_DARKGREEN: return CONSOLECOLOR_BACKGROUND_GREEN;
		case COLOR_RGB_NAVYBLUE: return CONSOLECOLOR_BACKGROUND_BLUE;
		}
		return CONSOLECOLOR_BACKGROUND_DEFAULT;
	}


	void Write_Console(const char* format, ...)
	{
#if defined(_DEBUG)
		char cLog[4096+1]={0,};
		int nBuf=0;
		va_list vptr;
		va_start(vptr, format);
		nBuf=_vsnprintf(cLog, 4096, format, vptr);
		OutputDebugStringA(cLog);
		OutputDebugStringA("\n");
		CConsoleManager::WriteFormatConsole(format, vptr);
		CConsoleManager::WriteFormatConsole("\n");
		va_end(vptr);
#endif
	}
	void Write_Error(const char* pFileName, const char* format, ...)
	{
#if defined(_DEBUG)
		char cLog[4096+1]={0,};
		int nBuf=0;
		va_list vptr;
		va_start(vptr, format);
		nBuf=_vsnprintf(cLog, 4096, format, vptr);
		if(0<nBuf){
			FILE* pSaveFile=fopen(pFileName, "a+t");
			if(NULL!=pSaveFile){
				SYSTEMTIME st;
				ZeroMemory(&st, sizeof(SYSTEMTIME));
				char cTime[1024]={0,};
				int nTime=0;
				::GetLocalTime(&st);
				sprintf(cTime, "[%d/%02d/%02d %02d:%02d:%02d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				nTime=(int)strlen(cTime);
				fwrite((void*)cTime, sizeof(char), nTime, pSaveFile);
				fwrite((void*)cLog, sizeof(char), nBuf, pSaveFile);
				nBuf=(int)strlen("\r\n");
				fwrite((void*)(char*)"\r\n", sizeof(char), nBuf, pSaveFile);
				fclose(pSaveFile);
			}
		}
		OutputDebugStringA(cLog);
		OutputDebugStringA("\n");
		CConsoleManager::WriteFormatConsole(format, vptr);
		CConsoleManager::WriteFormatConsole("\n");
		va_end(vptr);
#endif
	}
	void Write_Console(int nForColor, int nBackColor, const char* format, ...)
	{
#if defined(_DEBUG)
		char cLog[4096+1]={0,};
		int nBuf=0;
		va_list vptr;
		va_start(vptr, format);
		nBuf=_vsnprintf(cLog, 4096, format, vptr);
		OutputDebugStringA(cLog);
		OutputDebugStringA("\n");
		CConsoleManager::WriteFormatConsoleColor(CConsoleManager::ConvertColorRGBToConsoleFore(nForColor), CConsoleManager::ConvertColorRGBToConsoleBack(nBackColor), format, vptr);
		CConsoleManager::WriteFormatConsole("\n");
		va_end(vptr);
#endif
	}
	void Write_Error(const char* pFileName, int nForColor, int nBackColor, const char* format, ...)
	{
#if defined(_DEBUG)
		char cLog[4096+1]={0,};
		int nBuf=0;
		va_list vptr;
		va_start(vptr, format);
		nBuf=_vsnprintf(cLog, 4096, format, vptr);
		if(0<nBuf){
			FILE* pSaveFile=fopen(pFileName, "a+t");
			if(NULL!=pSaveFile){
				SYSTEMTIME st;
				ZeroMemory(&st, sizeof(SYSTEMTIME));
				char cTime[1024]={0,};
				int nTime=0;
				::GetLocalTime(&st);
				sprintf(cTime, "[%d/%02d/%02d %02d:%02d:%02d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				nTime=(int)strlen(cTime);
				fwrite((void*)cTime, sizeof(char), nTime, pSaveFile);
				fwrite((void*)cLog, sizeof(char), nBuf, pSaveFile);
				nBuf=(int)strlen("\r\n");
				fwrite((void*)(char*)"\r\n", sizeof(char), nBuf, pSaveFile);
				fclose(pSaveFile);
			}
		}
		OutputDebugStringA(cLog);
		OutputDebugStringA("\n");
		CConsoleManager::WriteFormatConsoleColor(CConsoleManager::ConvertColorRGBToConsoleFore(nForColor), CConsoleManager::ConvertColorRGBToConsoleBack(nBackColor), format, vptr);
		CConsoleManager::WriteFormatConsole("\n");
		va_end(vptr);
#endif
	}
	void Write_ConsoleFFL(const char *pFile, const char *pFunction, int nLine, const char* format, ...)
	{
#if defined(_DEBUG)
		char cLog[4096+1]={0,};
		char cFileLine[256+100]={0,};
		int nBuf=0;
		va_list vptr;
		va_start(vptr, format);
		nBuf=_vsnprintf(cLog, 4096, format, vptr);
		sprintf(cFileLine, "[%s:%s:%d]\n", pFile, pFunction, nLine);
		OutputDebugStringA(cLog);
		OutputDebugStringA(cFileLine);
		CConsoleManager::WriteFormatConsole(format, vptr);
		CConsoleManager::WriteFormatConsole(cFileLine);
		va_end(vptr);
#endif
	}
	void Write_ErrorFFL(const char* pFileName, const char *pFile, const char *pFunction, int nLine, const char* format, ...)
	{
#if defined(_DEBUG)
		char cLog[4096+1]={0,};
		char cFileLine[256+100]={0,};
		int nBuf=0;
		va_list vptr;
		va_start(vptr, format);
		nBuf=_vsnprintf(cLog, 4096, format, vptr);
		sprintf(cFileLine, "[%s:%s:%d]\n", pFile, pFunction, nLine);
		if(0<nBuf){
			FILE* pSaveFile=fopen(pFileName, "a+t");
			if(NULL!=pSaveFile){
				SYSTEMTIME st;
				ZeroMemory(&st, sizeof(SYSTEMTIME));
				char cTime[1024]={0,};
				int nTime=0;
				::GetLocalTime(&st);
				sprintf(cTime, "[%d/%02d/%02d %02d:%02d:%02d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				nTime=(int)strlen(cTime);
				fwrite((void*)cTime, sizeof(char), nTime, pSaveFile);
				fwrite((void*)cLog, sizeof(char), nBuf, pSaveFile);
				sprintf(cFileLine, "[%s:%s:%d]\r\n", pFile, pFunction, nLine);
				nBuf=(int)strlen(cFileLine);
				fwrite((void*)(char*)cFileLine, sizeof(char), nBuf, pSaveFile);
				fclose(pSaveFile);
			}
		}
		OutputDebugStringA(cLog);
		OutputDebugStringA(cFileLine);
		CConsoleManager::WriteFormatConsole(format, vptr);
		CConsoleManager::WriteFormatConsole(cFileLine);
		va_end(vptr);
#endif
	}
}
