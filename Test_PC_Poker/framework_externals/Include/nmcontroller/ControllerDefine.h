/**    
@file    ControllerDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Controller Import / Export Define
*/
#pragma once


#ifndef NMCONTROLLER_API

	#ifdef NMCONTROLLER_EXPORTS
		#define NMCONTROLLER_API		__declspec(dllexport )
	#else
		#define NMCONTROLLER_API		__declspec(dllimport )
	#endif 

#endif

#ifndef NMCONTROLLER_CLASS 
	#define NMCONTROLLER_CLASS NMCONTROLLER_API
#endif