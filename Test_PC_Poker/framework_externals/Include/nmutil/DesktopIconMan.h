// DesktopIconMan.h: interface for the CDesktopIconMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKTOPICONMAN_H__640CF097_BAA8_4566_A90B_C864A7D6131B__INCLUDED_)
#define AFX_DESKTOPICONMAN_H__640CF097_BAA8_4566_A90B_C864A7D6131B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UtilDefine.h"
#include "GFUtil.h"

namespace NMBASE
{
	namespace UTIL
	{
		class NMUTIL_CLASS CDesktopIconMan  
		{

		public:

			CDesktopIconMan();
			virtual ~CDesktopIconMan();

			BOOL CheckSetVersion(const char *pProgName,  int nSiteCode);	
			BOOL MakeIcon( CString url, CString ShortcutName, CString Desc, const char *pIconFilePath=NULL, int nIconIndex =0);

		protected:

			void MakeIconFile(ENUM_SCUT_SITE nSite);
			void Copy(LPCTSTR str, LPCTSTR icon, LPCTSTR url );

			HKEY	m_hkey;

		};

	}//namespace UTIL

}//namespace NMBASE


#endif // !defined(AFX_DESKTOPICONMAN_H__640CF097_BAA8_4566_A90B_C864A7D6131B__INCLUDED_)
