// NoChat.h: interface for the CNoChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOCHAT_H__7E121B71_8609_4115_97CE_9E0DA7DB1224__INCLUDED_)
#define AFX_NOCHAT_H__7E121B71_8609_4115_97CE_9E0DA7DB1224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{
		class NMUTIL_CLASS CNoChat  
		{
		public:
			DWORD NoChatTimeTick;
			BOOL bNoChat;

			int NowHPos;
			DWORD AddTime[10];
			CString History[10];

			CNoChat();
			virtual ~CNoChat();

			BOOL CheckNoChat(CString &str);
			BOOL CheckNoChat(LPCTSTR pstr);
			BOOL CheckNoChat2();
			BOOL ChatClac( CString strSource, CString strTarget );
			void AddChat(LPCTSTR pstr);

		};

	}//namespace UTIL

}//namespace NMBASE



#endif // !defined(AFX_NOCHAT_H__7E121B71_8609_4115_97CE_9E0DA7DB1224__INCLUDED_)
