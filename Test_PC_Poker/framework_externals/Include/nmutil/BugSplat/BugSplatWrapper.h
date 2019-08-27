#pragma once
#include "UtilDefine.h"
#include <List>

class MiniDmpSender;

namespace NMBASE
{
	namespace UTIL
	{
		class NMUTIL_CLASS BugSplatWrapper
		{
		private:
			
			MiniDmpSender *mpSender;

		public:
			std::list<CString> lstLog;
			static BugSplatWrapper& GetInstance();

			void Init(char *id, char *uniqNo, char* nickname);
			void AddMemLog(CString log);
			void ClearMemLog();

			static wchar_t* ConvMultiByteToUnicode(char *src);
			static char* ConvUnicodeToMultiByte(wchar_t *src);
			//static bool ExceptionCallback(UINT nCode, LPVOID lpVal1, LPVOID lpVal2);
			void SendAdditionFile(const __wchar_t* filePath);
			void SetCallBack();
			//void SetCallBack(MiniDmpSenderCallback fnCallback);
		};
	
	}//namespace UTIL

}//namespace NMBASE