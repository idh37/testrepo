/**    
@file    FrameDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Frame Import / Export Define
*/
#pragma once


#ifndef NMFRAME_API

	#ifdef NMFRAME_EXPORTS
		#define NMFRAME_API	__declspec(dllexport )
	#else
		#define NMFRAME_API	__declspec(dllimport )
	#endif 

#endif

#ifndef NMFRAME_CLASS 
	#define NMFRAME_CLASS NMFRAME_API
#endif