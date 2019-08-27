/**    
@file    SockDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   SocketCamel Import / Export Define
*/
#pragma once


#ifndef NMSOCKC_API
	#ifdef NMSOCKC_EXPORTS
		#define NMSOCKC_API		__declspec(dllexport )
	#else
		#define NMSOCKC_API		__declspec(dllimport )
	#endif 
#endif

#ifndef NMSOCKC_CLASS 
	#define NMSOCKC_CLASS NMSOCKC_API
#endif