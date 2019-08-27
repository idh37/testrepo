/**    
@file    UtilDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Util Import / Export Define
*/
#pragma once


#ifndef NMUTIL_API

	#ifdef NMUTIL_EXPORTS
		#define NMUTIL_API	__declspec(dllexport )
	#else
		#define NMUTIL_API	__declspec(dllimport )
	#endif 

#endif

#ifndef NMUTIL_CLASS 
	#define NMUTIL_CLASS NMUTIL_API
#endif