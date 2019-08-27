/**    
@file    ModelDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Model Import / Export Define
*/
#pragma once


#ifndef NMMODEL_API

	#ifdef NMMODEL_EXPORTS
		#define NMMODEL_API		__declspec(dllexport )
	#else
		#define NMMODEL_API		__declspec(dllimport )
	#endif 

#endif

#ifndef NMMODEL_CLASS 
	#define NMMODEL_CLASS NMMODEL_API
#endif