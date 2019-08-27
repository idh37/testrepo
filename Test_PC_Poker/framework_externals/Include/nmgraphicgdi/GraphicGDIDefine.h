/**    
@file    GraphicGDIDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   GraphicGDI Import / Export Define
*/
#pragma once


#ifndef NMGRAPHICGDI_API

	#ifdef NMGRAPHICGDI_EXPORTS
		#define NMGRAPHICGDI_API		__declspec(dllexport )
	#else
		#define NMGRAPHICGDI_API		__declspec(dllimport )
	#endif 

#endif

#ifndef NMGRAPHICGDI_CLASS 
	#define NMGRAPHICGDI_CLASS NMGRAPHICGDI_API
#endif