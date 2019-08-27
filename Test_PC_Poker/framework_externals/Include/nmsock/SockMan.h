/**    
@file    SockMan.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Socket 관리자 ( SingleTon 패턴 사용 )
*/
#pragma once

#include "SockDefine.h"
#include "DataSock.h"
#include <map>
#include <string>


typedef std::string (*pFuncI_Ptr)(int);

namespace NMBASE
{
	namespace SOCK
	{
		/**      
		@brief	소켓 메니저(Singleton)
		*/
		class NMSOCK_CLASS CSockMan
		{
		public:
			static	CSockMan& Get();

		public:
			void Init(HWND hwnd);									///< 클래스 초기화
			void Destroy();	

			LPCTSTR ConnectSocket(char *strIP, UINT nPortNum);		///< 클라이언트용으로 데이터 소켓을 생성(성공시 클래스 핸들 반환, 실패시 0보다 작은값을 리턴)

			int ReceiveData(char* szSockName);						///< 소켓으로부터 데이터를 받는다

			BOOL SendData(char* szSockName, void *pdata, int size);

			BOOL PostCloseMsg(char* szSockName);

			BOOL CloseDataSocket(const char* szSockName);			///< 소켓 클래스ID로 소켓 삭제 - Overrided

			void CloseAllDataSocket();								///< 모든 소켓 삭제

			CDataSock* GetDataSocketPtr(char* szSockName);			///< 데이터 소켓의 포인터를 얻음(NULL을 반환하면 생성되지 않은 소켓임)

			LPCTSTR GetServerIP(char* szSockName);					///< 접속한 서버의 IP문자열을 얻는다.
			UINT GetServerPort(char* szSockName);

			char* GetHostIP();
			char* GetHostDomain();
			char* GetHostPortStr();

			void Error(char *strErr);

		private:
			CSockMan();
			virtual ~CSockMan();

			void CheckHostInfo();									///< 호스트 정보를 체크한다

		protected:
			typedef std::map<std::string, CDataSock*>	SOCKMAP;
			typedef SOCKMAP::iterator					SOCKMAP_IT;
			SOCKMAP	m_mapSock;

			CString m_strSrvIP, m_strDomain, m_strPort;				///< 호스트 정보
			CString m_strError;										///< 에러 출력용 스트링

			HWND m_hWindow;											///< PostMessage보낼 윈도우 핸들(SockCmnd클래스에 정의되어 있음) 


		private:
			CString m_strServerName;								///< 내부에서만 사용함.
		public:
			static pFuncI_Ptr m_pFuncI_Ptr_Type;
			static std::string m_sSocketTxt;
			static void SetSocketTxt(std::string sSocketTxt);
			static void SetSocketTxt(int nProcessCnt);
			static std::string GetSocketTxt(void);

			static std::string m_sSocketLogTxt;
			static void SetSocketLogTxt(std::string sSocketLogTxt);
			static std::string GetSocketLogTxt(void);
		};


		NMSOCK_API CSockMan& GSOCKMAN();

	} //namespace SOCK
}  // namespace NMBASE



namespace LOG{

	class CCritical_Section : public CRITICAL_SECTION
	{
	public:
		BOOL m_bLock;
	public:
		BOOL IsLock(void){ return m_bLock; }
	public:
		CCritical_Section(void){ InitCritical_Section(); }
		virtual ~CCritical_Section(void){ FreeCritical_Section(); }
		void InitCritical_Section(void){ ::InitializeCriticalSection((CRITICAL_SECTION*)this); m_bLock=FALSE; }
		void FreeCritical_Section(void){ ::DeleteCriticalSection((CRITICAL_SECTION*)this); }
	public:
		void Lock(void){ ::EnterCriticalSection((CRITICAL_SECTION*)this); m_bLock=TRUE; }
		void UnLock(void){ ::LeaveCriticalSection((CRITICAL_SECTION*)this); m_bLock=FALSE; }
	};

	class CSynchronizer
	{
	public:
		CSynchronizer(CCritical_Section *pcCritical_Section){
			m_pcCritical_Section=pcCritical_Section;
			if(NULL!=m_pcCritical_Section){
				m_pcCritical_Section->Lock();
			}
		}
		virtual ~CSynchronizer(void){
			if(NULL!=m_pcCritical_Section){
				m_pcCritical_Section->UnLock();
			}
		}
	protected:
		CCritical_Section *m_pcCritical_Section;
	};

	//yoo
	class NMSOCK_CLASS CConsoleManager
	{
	public:
		enum{
			COLOR_RGB_WHITE=0,
			COLOR_RGB_BLACK=1,
			COLOR_RGB_GRAY=2,
			COLOR_RGB_RED=3,
			COLOR_RGB_GREEN=4,
			COLOR_RGB_BLUE=5,
			COLOR_RGB_YELLOW=6,
			COLOR_RGB_CYAN=7,
			COLOR_RGB_MAGENTA=8,
			COLOR_RGB_SCARLETRED=9,
			COLOR_RGB_DARKGREEN=10,
			COLOR_RGB_NAVYBLUE=11,
		};
		enum{
			FORE_GROUND_BLUE=0x0001, // text color contains blue.
			FORE_GROUND_GREEN=0x0002, // text color contains green.
			FORE_GROUND_RED=0x0004, // text color contains red.
			FORE_GROUND_INTENSITY=0x0008, // text color is intensified.
			BACK_GROUND_BLUE=0x0010, // background color contains blue.
			BACK_GROUND_GREEN=0x0020, // background color contains green.
			BACK_GROUND_RED=0x0040, // background color contains red.
			BACK_GROUND_INTENSITY=0x0080, // background color is intensified.
			CONSOLECOLOR_FOREGROUND_WHITE=FORE_GROUND_BLUE|FORE_GROUND_RED|FORE_GROUND_GREEN,
			CONSOLECOLOR_FOREGROUND_YELLOW=FORE_GROUND_GREEN|FORE_GROUND_BLUE|FORE_GROUND_INTENSITY,
			CONSOLECOLOR_FOREGROUND_CYAN=FORE_GROUND_RED|FORE_GROUND_BLUE|FORE_GROUND_INTENSITY,
			CONSOLECOLOR_FOREGROUND_MAGENTA=FORE_GROUND_RED|FORE_GROUND_GREEN|FORE_GROUND_INTENSITY,
			CONSOLECOLOR_FOREGROUND_BLUE=FORE_GROUND_BLUE|FORE_GROUND_INTENSITY,
			CONSOLECOLOR_FOREGROUND_GREEN=FORE_GROUND_GREEN|FORE_GROUND_INTENSITY,
			CONSOLECOLOR_FOREGROUND_RED=FORE_GROUND_RED|FORE_GROUND_INTENSITY,
			CONSOLECOLOR_FOREGROUND_BLACK=0,
			CONSOLECOLOR_FOREGROUND_DEFAULT=0xffff,
			CONSOLECOLOR_BACKGROUND_WHITE=BACK_GROUND_BLUE|BACK_GROUND_RED|BACK_GROUND_GREEN,
			CONSOLECOLOR_BACKGROUND_YELLOW=BACK_GROUND_RED|BACK_GROUND_GREEN|BACK_GROUND_INTENSITY,
			CONSOLECOLOR_BACKGROUND_CYAN=BACK_GROUND_RED|BACK_GROUND_BLUE|FORE_GROUND_INTENSITY,
			CONSOLECOLOR_BACKGROUND_MAGENTA=BACK_GROUND_RED|BACK_GROUND_GREEN|FORE_GROUND_INTENSITY,
			CONSOLECOLOR_BACKGROUND_BLUE=BACK_GROUND_BLUE|BACK_GROUND_INTENSITY,
			CONSOLECOLOR_BACKGROUND_GREEN=BACK_GROUND_GREEN|BACK_GROUND_INTENSITY,
			CONSOLECOLOR_BACKGROUND_RED=BACK_GROUND_RED|BACK_GROUND_INTENSITY,
			CONSOLECOLOR_BACKGROUND_BLACK=0,
			CONSOLECOLOR_BACKGROUND_DEFAULT=0xffff,
		};
	public:
		static void* m_pConsole_In;
		static void* m_pConsole_Out;
		static CCritical_Section m_cCS;
	public:
		CConsoleManager(void){ InitConsole(); }
		virtual ~CConsoleManager(void){ FreeGonsole(); }
		void InitConsole(void);
		void FreeGonsole(void);
	public:
		static void SetConsole(void);
		static void UnSetConsole(void);
		static bool SetConsoleColor(int nForeGroundColor=CONSOLECOLOR_FOREGROUND_DEFAULT, int nBackGroundColor=CONSOLECOLOR_BACKGROUND_DEFAULT);
		static void ResetConsoleColor(void);
		static bool SetConsolePos(void);
		static bool WriteFormatConsole(const char* format, ...);
		static bool WriteFormatConsole(const char* format, va_list ArgPtr);
		static bool WriteFormatConsoleColor(int nForeGroundColor, int nBackGroundColor, const char* format, ...);
		static bool WriteFormatConsoleColor(int nForeGroundColor, int nBackGroundColor, const char* format, va_list ArgPtr);
		static bool ConsoleWrite(char* pBuffer, int nBufferSize, unsigned long* pWrite);
		static bool WriteTextColor(char* pText, int nForeGroundColor=CONSOLECOLOR_FOREGROUND_DEFAULT, int nBackGroundColor=CONSOLECOLOR_BACKGROUND_DEFAULT);
		static bool WriteText(char* pText);
		static bool ConsoleRead(char* pBuffer, int nBufferSize, unsigned long* pRead);
		static void Flush(void);
		static void ClearScreen(void);
		static void ConsoleEvent(void);
	public:
		static int ConsoleHandlerRoutine(unsigned long dwCtrlType);
	public:
		static int ConvertColorRGBToConsoleFore(int Rgb);
		static int ConvertColorRGBToConsoleBack(int Rgb);
	};

	NMSOCK_CLASS void Write_Console(const char* format, ...);
	NMSOCK_CLASS void Write_Error(const char* pFileName, const char* format, ...);
	NMSOCK_CLASS void Write_Console(int nForColor, int nBackColor, const char* format, ...);
	NMSOCK_CLASS void Write_Error(const char* pFileName, int nForColor, int nBackColor, const char* format, ...);
	NMSOCK_CLASS void Write_ConsoleFFL(const char *pFile, const char *pFunction, int nLine, const char* format, ...);
	NMSOCK_CLASS void Write_ErrorFFL(const char* pFileName, const char *pFile, const char *pFunction, int nLine, const char* format, ...);
}

#define ErrorLogConsole(...) LOG::Write_Console(__VA_ARGS__)
#define ErrorLogError(pFileName, ...) LOG::Write_Error(pFileName, __VA_ARGS__)
#define ErrorLogColorConsole(nForColor, nBackColor, ...) LOG::Write_Console(nForColor, nBackColor, __VA_ARGS__)
#define ErrorLogColorError(pFileName, nForColor, nBackColor, ...) LOG::Write_Error(pFileName, nForColor, nBackColor, __VA_ARGS__)
#define ErrorLogConsoleFFL(...) LOG::Write_ConsoleFFL(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define ErrorLogErrorFFL(pFileName, ...) LOG::Write_ErrorFFL(pFileName, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define SYNCHRONIZED(ccrtical_section)  CSynchronizer _cSynchronizer(&(ccrtical_section))