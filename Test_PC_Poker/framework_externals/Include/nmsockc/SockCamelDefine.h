/**    
@file    SockDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   SocketCamel Import / Export Define
*/
#pragma once


#ifndef NMSOCKCAMEL_API
	#ifdef NMSOCKCAMEL_EXPORTS
		#define NMSOCKCAMEL_API		__declspec(dllexport )
	#else
		#define NMSOCKCAMEL_API		__declspec(dllimport )
	#endif 
#endif

#ifndef NMSOCKCAMEL_CLASS 
	#define NMSOCKCAMEL_CLASS NMSOCKCAMEL_API
#endif