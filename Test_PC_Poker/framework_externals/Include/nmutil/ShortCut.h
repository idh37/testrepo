// ShortCut.h: interface for the CShortCut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHORTCUT_H__167DC44C_8774_44A4_9F41_4D00601B61AE__INCLUDED_)
#define AFX_SHORTCUT_H__167DC44C_8774_44A4_9F41_4D00601B61AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{
		NMUTIL_API std::string GetiExplorerPath();
		NMUTIL_API int SetIEStartPage(char *sPage);
		NMUTIL_API int GetProgramFilePath(LPCTSTR PathBuffer, int DefualtAll=0);
		NMUTIL_API int GetCommonStartupFolder(char *PathBuffer, int DefualtAll=0);
		NMUTIL_API int GetDeskTopFolder(char *PathBuffer, int DefualtAll=0);

		NMUTIL_API BOOL CreateInternetShortCutToDesktopFolder(LPCSTR pszURL, LPCSTR pszlnkName, LPCSTR szDescription, LPCTSTR szIconFile, int nIndex);
		NMUTIL_API BOOL CreateShortCutToDesktopFolder(LPCSTR pszlnkName, LPCSTR pszExePath, LPCSTR pszArgs, LPCSTR szDescription, LPCTSTR szIconFile, int nIndex);

	}//namespace UTIL

}//namespace NMBASE


#endif // !defined(AFX_SHORTCUT_H__167DC44C_8774_44A4_9F41_4D00601B61AE__INCLUDED_)
