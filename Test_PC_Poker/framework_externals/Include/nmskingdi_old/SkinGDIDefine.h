/**    
@file    SkinGDIDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   SkinGDI Import / Export Define
*/
#pragma once


#ifndef NMSKINGDI_API

	#ifdef NMSKINGDI_EXPORTS
		#define NMSKINGDI_API		__declspec(dllexport )
	#else
		#define NMSKINGDI_API		__declspec(dllimport )
	#endif 

#endif

#ifndef NMSKINGDI_CLASS 
	#define NMSKINGDI_CLASS NMSKINGDI_API
#endif