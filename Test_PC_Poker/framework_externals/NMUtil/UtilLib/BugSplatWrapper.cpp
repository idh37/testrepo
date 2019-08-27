#include "StdAfx.h"
#include "BugSplat.h"
#include "./BugSplat/BugSplatWrapper.h"

#include <iostream>
#include <fstream>
using namespace std;
 

//bool ExceptionCallback(UINT nCode, LPVOID lpVal1, LPVOID lpVal2);

namespace NMBASE
{
	namespace UTIL
	{
		/*
		//BugSplat exception callback
		// 전역함수
		bool ExceptionCallback(UINT nCode, LPVOID lpVal1, LPVOID lpVal2)
		{
			switch (nCode)
			{
				case MDSCB_EXCEPTIONCODE:
				{
					//EXCEPTION_RECORD *p = (EXCEPTION_RECORD *)lpVal1;
					//DWORD code = p ? p->ExceptionCode : 0;

					// Create some files in the %temp% directory and attach them
					wchar_t cmdString[2 * MAX_PATH];
					wchar_t filePath[MAX_PATH];
					wchar_t tempPath[MAX_PATH];
					GetTempPathW(MAX_PATH, tempPath);


					TCHAR filePath1[MAX_PATH];
					wsprintf(filePath1, "%sBugSplatLog.txt", NMBASE::UTIL::BugSplatWrapper::GetInstance().ConvUnicodeToMultiByte(tempPath));

					ofstream writeFile(filePath1);
					if (writeFile.is_open())
					{
						//writeFile << "test1" << "\n";
						//writeFile << "test2" << "\n";
						//writeFile << _T("한글도?") << "\n";

						list<CString>::iterator itor;
						for(itor = NMBASE::UTIL::BugSplatWrapper::GetInstance().lstLog.begin(); itor != NMBASE::UTIL::BugSplatWrapper::GetInstance().lstLog.end(); itor++)
						{
							writeFile << *itor << endl;
						}
						writeFile.close();
					}

					NMBASE::UTIL::BugSplatWrapper::GetInstance().SendAdditionFile( (const __wchar_t*)NMBASE::UTIL::BugSplatWrapper::GetInstance().ConvMultiByteToUnicode(filePath1) );
				}
				break;
			}

			return false;
		}
		*/

		BugSplatWrapper& BugSplatWrapper::GetInstance()
		{
			static BugSplatWrapper s_cBugSplatWrapper;
			return s_cBugSplatWrapper;
		}

		void BugSplatWrapper::Init(char *id, char *uniqNo, char* nickname)
		{
			std::locale::global(locale("kor"));

			CString userinfo;
			//userinfo.Format("%s, %s, %s", GM().GetMyInfo()->UI.ID, GM().GetMyInfo()->UI.UniqNo, GM().GetMyInfo()->UI.NickName);
			userinfo.Format("%s, %s, %s", id, uniqNo, nickname);
			
			mpSender = new MiniDmpSender((const __wchar_t*)L"DB_PCPoker_Test", 
													  (const __wchar_t*)L"PCPoker", 
													  (const __wchar_t*)L"1.3.3", 
													  NULL, 
													  // MDSF_NONINTERACTIVE |
													  MDSF_LOGCONSOLE | 
													  MDSF_USEGUARDMEMORY | 
													  MDSF_LOGFILE);
			
			//mpSender->setCallback(ExceptionCallback);
		}
		
		wchar_t* BugSplatWrapper::ConvMultiByteToUnicode(char *src)
		{
			wchar_t strUnicode[256] = {0,};
			char    strMultibyte[256] = {0,};
			strcpy_s(strMultibyte, 256, src);
			int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

			return strUnicode;
		}

		char* BugSplatWrapper::ConvUnicodeToMultiByte(wchar_t *src)
		{
			wchar_t strUnicode[256] = {0,}; 
			char	strMultibyte[256] = {0,}; 
			wcscpy_s(strUnicode, 256, src); 
			int len = WideCharToMultiByte( CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL );	
			WideCharToMultiByte( CP_ACP, 0, strUnicode, -1, strMultibyte, len, NULL, NULL );

			return strMultibyte;
		}

		void BugSplatWrapper::AddMemLog(CString log)
		{
			lstLog.push_back(log);
		}

		void BugSplatWrapper::ClearMemLog()
		{
			lstLog.clear();
		}

		void BugSplatWrapper::SendAdditionFile(const __wchar_t* filePath)
		{
			//NMBASE::UTIL::BugSplatWrapper::GetInstance().SendAdditionalFile(reinterpret_cast<const __wchar_t*>(filePath));
			mpSender->sendAdditionalFile(filePath);
		}

		void BugSplatWrapper::SetCallBack()
		{
			//mpSender->setCallback(ExceptionCallback);
		}

		/*void BugSplatWrapper::SetCallBack(MiniDmpSenderCallback fnCallback)
		{
			mpSender->setCallback(fnCallback);
		}*/
	}
}

// BugSplat exception callback
// 
/*
bool ExceptionCallback(UINT nCode, LPVOID lpVal1, LPVOID lpVal2)
{

	switch (nCode)
	{
	case MDSCB_EXCEPTIONCODE:
	{
		EXCEPTION_RECORD *p = (EXCEPTION_RECORD *)lpVal1;
		DWORD code = p ? p->ExceptionCode : 0;

		// Create some files in the %temp% directory and attach them
		wchar_t cmdString[2 * MAX_PATH];
		wchar_t filePath[MAX_PATH];
		wchar_t tempPath[MAX_PATH];
		GetTempPathW(MAX_PATH, tempPath);

		TCHAR filePath1[MAX_PATH];
		wsprintf(filePath1, "%sBugSplatLog.txt", NMBASE::UTIL::BugSplatWrapper::GetInstance().ConvUnicodeToMultiByte(tempPath));

		ofstream writeFile(filePath1);
		if (writeFile.is_open())
		{
			//writeFile << "test1" << "\n";
			//writeFile << "test2" << "\n";
			//writeFile << _T("한글도?") << "\n";

			list<CString>::iterator itor;
			for(itor = NMBASE::UTIL::BugSplatWrapper::GetInstance().lstLog.begin(); itor != NMBASE::UTIL::BugSplatWrapper::GetInstance().lstLog.end(); itor++)
			{
				writeFile << *itor << endl;
			}
			writeFile.close();
		}

		//NMBASE::UTIL::BugSplatWrapper::GetInstance().SendAdditionFile->sendAdditionalFile((const __wchar_t*)NMBASE::UTIL::BugSplatWrapper::GetInstance().ConvMultiByteToUnicode(filePath1) );
		NMBASE::UTIL::BugSplatWrapper::GetInstance().SendAdditionFile((const __wchar_t*)NMBASE::UTIL::BugSplatWrapper::GetInstance().ConvMultiByteToUnicode(filePath1) );
		
	}
	break;
	}

	return false;
}
*/