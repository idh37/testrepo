/**    
@file    SoundDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Sound Import / Export Define
*/
#pragma once


#ifndef NMSOUND_API

	#ifdef NMSOUND_EXPORTS
		#define NMSOUND_API		__declspec(dllexport )
	#else
		#define NMSOUND_API		__declspec(dllimport )
	#endif 

#endif

#ifndef NMSOUND_CLASS 
	#define NMSOUND_CLASS NMSOUND_API
#endif