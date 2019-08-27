// NMSSOWebLib.h: interface for the CSSOWebLib class.
//////////////////////////////////////////////////////////////////////
#pragma once

#include "UtilDefine.h"

#ifdef  _DEBUG
#ifdef	_MT
#ifdef	_DLL
#pragma comment(lib, "ExHandle_MDd.lib")
#pragma message( "Export ExHandle_MDd.lib MultiThread Dll Debug" )
#else
#pragma comment(lib, "ExHandle_MTd.lib")
#pragma message( "Export ExHandle_MTd.lib MultiThread Debug" )
#endif
#else
#pragma comment(lib, "ExHandle_MLd.lib")
#pragma message( "Export ExHandle_MLd.lib SingleThread Debug" )
#endif
#else
#ifdef	_MT
#ifdef	_DLL
#pragma comment(lib, "ExHandle_MD.lib")
#pragma message( "Export ExHandle_MD.lib MultiThread Dll" )
#else
#pragma comment(lib, "ExHandle_MT.lib")
#pragma message( "Export ExHandle_MT.lib MultiThread" )
#endif
#else
#pragma comment(lib, "ExHandle_ML.lib")
#pragma message( "Export ExHandle_ML.lib SingleThread" )
#endif
#endif

namespace NMBASE
{
	namespace UTIL
	{
		class NMUTIL_CLASS CNMExHandle
		{
		public:

			enum 
			{
				EXHAN_UNKNOWN_MODULE = 0,
				EXHAN_CLIENT = 1,
				EXHAN_GAMESERVER = 2,
				EXHAN_MASTERSERVER = 3,
			};

		public:
			CNMExHandle();
			virtual ~CNMExHandle();

		public:
			static void	SetExeptionInfo(TCHAR* szGameCode, TCHAR* szRegistryDir, int nKind);
			static void PresetSetSiteName(TCHAR* strSiteName);	
		};

	}//namespace UTIL

}//namespace NMBASE