// SysUtil.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMExHandle.h"
#include "ExHandle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



#ifndef STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_DEPRECATE
#endif

namespace NMBASE
{
	namespace UTIL
	{

		CNMExHandle::CNMExHandle()
		{

		}

		CNMExHandle::~CNMExHandle()
		{

		}

		void CNMExHandle::SetExeptionInfo(TCHAR* szGameCode, TCHAR* szRegistryDir, int nKind)
		{
			CExHandle::SetExeptionInfo(szGameCode, szRegistryDir, nKind);
		}


		void CNMExHandle::PresetSetSiteName(TCHAR* strSiteName)
		{
			CExHandle::PresetSetSiteName(strSiteName);
		}

	}//namespace UTIL

}//namespace NMBASE