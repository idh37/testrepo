// DesktopIconManEx.h: interface for the CDesktopIconManEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKTOPICONMANEX_H__F0054A07_3B6C_46C6_A3A8_AF24CA9A504C__INCLUDED_)
#define AFX_DESKTOPICONMANEX_H__F0054A07_3B6C_46C6_A3A8_AF24CA9A504C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UtilDefine.h"
#include "DesktopIconMan.h"

namespace NMBASE
{
	namespace UTIL
	{
		class NMUTIL_CLASS CDesktopIconManEx  : public CDesktopIconMan
		{
		public:
			CDesktopIconManEx();
			virtual ~CDesktopIconManEx();

			void ClearChampDesktopIcon(CString FileName);
		};

	}//namespace UTIL

}//namespace NMBASE


#endif // !defined(AFX_DESKTOPICONMANEX_H__F0054A07_3B6C_46C6_A3A8_AF24CA9A504C__INCLUDED_)
