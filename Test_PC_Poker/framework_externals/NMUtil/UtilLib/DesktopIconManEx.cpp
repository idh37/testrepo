// DesktopIconManEx.cpp: implementation of the CDesktopIconManEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DesktopIconManEx.h"
#include "ShortCut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace UTIL
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CDesktopIconManEx::CDesktopIconManEx()
		{

		}

		CDesktopIconManEx::~CDesktopIconManEx()
		{

		}


		void CDesktopIconManEx::ClearChampDesktopIcon(CString FileName)
		{
			char DeskTopPath[MAX_PATH] = {0,};
			char URLfinename[MAX_PATH] = {0,};

			if(GetDeskTopFolder(DeskTopPath, 0))
			{
				int nLen = strlen(DeskTopPath);
				if(DeskTopPath[nLen] != '\\')
				{
					::strcat_s(DeskTopPath, MAX_PATH, "\\");
				}
				wsprintf(URLfinename, "%s%s.url", DeskTopPath, FileName.GetString());
				DeleteFile(URLfinename);
			}

		}

	}//namespace UTIL

}//namespace NMBASE
